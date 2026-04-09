//
// Created by greg8 on 3/14/2026.
//

#include "SwapBuffer.h"

#include <mutex>

Rendering::RenderFrame* Rendering::SwapBuffer::getWriteFrame()
{
    return &renderFrames[writeIndex];
}

Rendering::RenderFrame* Rendering::SwapBuffer::swapWriteFrame()
{
    // Note: Lock-free implementations are possible.
    std::unique_lock lock(swapMutex);
    int temp = writeIndex;
    writeIndex = idleIndex;
    idleIndex = temp;
    newDataReady = true;
    lock.unlock();

    notifier.notify_one();
    return &renderFrames[writeIndex];
}

Rendering::RenderFrame* Rendering::SwapBuffer::awaitReadFrameAndSwap()
{
    std::unique_lock lock(swapMutex);
    // wait releases the lock before sleeping and acquires it again before returning.
    notifier.wait(lock, [this] { return newDataReady; });
    newDataReady = false;
    int temp = readIndex;
    readIndex = idleIndex;
    idleIndex = temp;
    lock.unlock();

    return &renderFrames[readIndex];
}
