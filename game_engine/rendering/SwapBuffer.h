//
// Created by greg8 on 3/14/2026.
//

#ifndef GAME_ENGINE_SWAPBUFFER_H
#define GAME_ENGINE_SWAPBUFFER_H
#include "RenderFrame.h"

#include <mutex>
#include <condition_variable>

namespace Rendering
{
    // AKA "Mailbox" pattern.
    struct SwapBuffer
    {
        RenderFrame renderFrames[3];

        RenderFrame* getWriteFrame();
        RenderFrame* swapWriteFrame();
        RenderFrame* awaitReadFrameAndSwap();

    private:
        int writeIndex = 0;
        int readIndex = 1;
        int idleIndex = 2;
        std::condition_variable notifier;
        bool newDataReady = false;
        std::mutex swapMutex;
    };
}

#endif //GAME_ENGINE_SWAPBUFFER_H
