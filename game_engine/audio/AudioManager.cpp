#include "AudioManager.h"

#include <algorithm>

namespace Audio
{
    void AudioManager::initialize(uint16_t maxSfx)
    {
        InitAudioDevice();
        maxEffects = maxSfx;
        soundEffects.resize(maxSfx);
        freeEffectIndices.resize(maxSfx);
        for (size_t i = 0; i < freeEffectIndices.size(); i++)
            freeEffectIndices[i] = static_cast<uint16_t>(maxSfx - i - 1);
    }

    void AudioManager::deinitialize()
    {
        for (auto& sfx : soundEffects) sfx.unload();
        soundEffects.clear();
        freeEffectIndices.clear();
        maxEffects = 0;

        for (auto& t : musicTracks)
            if (t.loaded) { StopMusicStream(t.stream); UnloadMusicStream(t.stream); }

        CloseAudioDevice();
    }

    SoundId AudioManager::loadAudioAsset(const std::string& path, uint8_t maxVoices,
                                         VoiceExhaustionBehavior behavior)
    {
        if (freeEffectIndices.empty() || maxVoices == 0)
            return INVALID_SOUND_ID;
        auto idx = freeEffectIndices.back();
        freeEffectIndices.pop_back();
        soundEffects[idx].load(path, maxVoices, behavior);
        return SoundId{idx, soundEffects[idx].generation};
    }

    bool AudioManager::unloadAudioAsset(SoundId id)
    {
        if (id.index >= soundEffects.size()) return false;
        auto& sfx = soundEffects[id.index];
        if (sfx.generation != id.generation) return false;
        sfx.unload();
        freeEffectIndices.push_back(id.index);
        return true;
    }

    bool AudioManager::playOneshot(SoundId id)
    {
        if (id.index >= soundEffects.size()) return false;
        auto& sfx = soundEffects[id.index];
        if (sfx.generation != id.generation) return false;
        return sfx.playOneShot();
    }

    MusicId AudioManager::loadMusic(const std::string& path)
    {
        for (uint16_t i = 0; i < MAX_MUSIC_TRACKS; i++)
        {
            if (musicTracks[i].loaded) continue;

            Music m = LoadMusicStream(path.c_str());
            if (!IsMusicValid(m))
            {
                UnloadMusicStream(m);
                return INVALID_MUSIC_ID;
            }
            musicTracks[i].stream     = m;
            musicTracks[i].loaded     = true;
            musicTracks[i].volume     = 1.0f;
            musicTracks[i].fadeTarget = 1.0f;
            musicTracks[i].fadeSpeed  = 0.0f;
            musicTracks[i].generation++;
            return MusicId{i, musicTracks[i].generation};
        }
        return INVALID_MUSIC_ID;
    }

    bool AudioManager::unloadMusic(MusicId id)
    {
        if (id.index >= MAX_MUSIC_TRACKS) return false;
        auto& t = musicTracks[id.index];
        if (!t.loaded || t.generation != id.generation) return false;
        StopMusicStream(t.stream);
        UnloadMusicStream(t.stream);
        t.loaded = false;
        return true;
    }

    bool AudioManager::playMusic(MusicId id, float fadeIn)
    {
        if (id.index >= MAX_MUSIC_TRACKS) return false;
        auto& t = musicTracks[id.index];
        if (!t.loaded || t.generation != id.generation) return false;

        if (fadeIn > 0.0f)
        {
            t.volume     = 0.0f;
            t.fadeTarget = 1.0f;
            t.fadeSpeed  = 1.0f / fadeIn;
        }
        else
        {
            t.volume    = 1.0f;
            t.fadeSpeed = 0.0f;
        }

        SetMusicVolume(t.stream, t.volume);
        PlayMusicStream(t.stream);
        return true;
    }

    bool AudioManager::stopMusic(MusicId id, float fadeOut)
    {
        if (id.index >= MAX_MUSIC_TRACKS) return false;
        auto& t = musicTracks[id.index];
        if (!t.loaded || t.generation != id.generation) return false;

        if (fadeOut > 0.0f)
        {
            t.fadeTarget = 0.0f;
            t.fadeSpeed  = 1.0f / fadeOut;
        }
        else
        {
            StopMusicStream(t.stream);
            t.volume    = 0.0f;
            t.fadeSpeed = 0.0f;
        }
        return true;
    }

    void AudioManager::updateMusic(float dt)
    {
        for (auto& t : musicTracks)
        {
            if (!t.loaded) continue;

            if (t.fadeSpeed > 0.0f)
            {
                if (t.volume < t.fadeTarget)
                    t.volume = std::min(t.volume + t.fadeSpeed * dt, t.fadeTarget);
                else
                    t.volume = std::max(t.volume - t.fadeSpeed * dt, t.fadeTarget);

                SetMusicVolume(t.stream, t.volume);

                if (t.volume == t.fadeTarget)
                {
                    t.fadeSpeed = 0.0f;
                    if (t.fadeTarget == 0.0f)
                        StopMusicStream(t.stream);
                }
            }

            UpdateMusicStream(t.stream);
        }
    }
}
