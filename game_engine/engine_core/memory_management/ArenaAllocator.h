//
// Created by greg8 on 11/23/2025.
//

#ifndef GAME_ENGINE_ARENAALLOCATOR_H
#define GAME_ENGINE_ARENAALLOCATOR_H
#include <cstdint>
#include <memory>

namespace EngineCore
{
    struct ArenaAllocator
    {
        explicit ArenaAllocator(size_t capacity)
            : capacity(capacity), start(new uint8_t[capacity]), next(start)
        {
        }

        template <typename T>
        T* allocate(size_t count)
        {
            if (count == 0)
                return nullptr;

            size_t bytesWanted = count * sizeof(T);
            size_t alignment = alignof(T);
            size_t bytesUsed = next - start;
            size_t bytesAvailable = capacity - bytesUsed;

            void* nextAligned = next;
            if (std::align(alignment, bytesWanted, nextAligned, bytesAvailable))
            {
                next = static_cast<uint8_t*>(nextAligned) + bytesWanted;
                return static_cast<T*>(nextAligned);
            }
            return nullptr;
        }

        void resetFully() { next = start; }

        ~ArenaAllocator() { delete[] start; }

        // Prevent double free:
        ArenaAllocator(const ArenaAllocator&) = delete;
        ArenaAllocator& operator=(const ArenaAllocator&) = delete;
        ArenaAllocator(ArenaAllocator&&) = delete;
        ArenaAllocator& operator=(ArenaAllocator&&) = delete;

    private:
        size_t capacity;
        uint8_t* const start;
        uint8_t* next;
    };
}


#endif //GAME_ENGINE_ARENAALLOCATOR_H
