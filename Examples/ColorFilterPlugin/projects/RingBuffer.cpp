#include "RingBuffer.h"
#include <algorithm>

void RingBuffer::rotateBuffer(std::array<double, chunkSize>& buffer, size_t shift) { std::rotate(buffer.begin(), buffer.begin() + shift, buffer.end()); }

size_t RingBuffer::getHopSize() const { return hopSize; }

void RingBuffer::initializeHannWindow()
{
  for (size_t i = 0; i < chunkSize; ++i)
  {
    hannWindow[i] = 0.5 * (1 - cos(2 * iplug::PI * i / (chunkSize - 1)));
  }
}

// Write a sample to the buffer
void RingBuffer::write(double sample)
{
  mBuffer[head] = sample;
  head = (head + 1) % bufferSize; // Wrap around if necessary
}

// Read a sample from the buffer
double RingBuffer::read()
{
  if (getAvailableOutputSamples() > 0)
  {
    double sample = mOutputBuffer[tailOut];
    tailOut = (tailOut + 1) % bufferSize; // Wrap around if necessary
    return sample;
  }
  else
  {
    return 0.0; // No samples available, output silence
  }
}

void RingBuffer::readChunk()
{
  if (getAvailableSamples() >= getChunkSize())
  {
    chunkRead = true;
    // Read samples from the main buffer
    for (size_t i = 0; i < chunkSize; ++i)
    {
      mChunkBuffer[i] = mBuffer[tail];
      tail = (tail + 1) % bufferSize;
    }

    // Copy the current chunk to the overlap buffer
    mOverlapBuffer = mChunkBuffer;
    // Rotate the overlap buffer by hopSize
    rotateBuffer(mOverlapBuffer, hopSize);

    // Apply Hann window to the chunk
    for (size_t i = 0; i < chunkSize; ++i)
    {
      mChunkBuffer[i] *= hannWindow[i];
      mOverlapBuffer[i] *= hannWindow[i];
    }
  }
}

void RingBuffer::writeChunk()
{
  if (chunkRead)
  {
    chunkRead = false;
    // Rotate the overlap buffer by hopSize
    rotateBuffer(mOverlapBuffer, hopSize);

    // Apply overlap-add
    for (size_t i = 0; i < chunkSize; ++i)
    {
      mChunkBuffer[i] += mOverlapBuffer[i]; // Add overlap from previous chunk
    }

    // Write the processed chunk to the output buffer
    for (size_t i = 0; i < chunkSize; ++i)
    {
      mOutputBuffer[headOut] = mChunkBuffer[i];
      headOut = (headOut + 1) % bufferSize;
    }
  }
}

// Get the number of available samples for reading
size_t RingBuffer::getAvailableSamples() const
{
  if (head >= tail)
  {
    return head - tail; // No wrap-around
  }
  else
  {
    return bufferSize - tail + head; // Wrap-around case
  }
}

// Get the number of free slots for writing
size_t RingBuffer::getFreeSlots() const
{
  return bufferSize - getAvailableSamples() - 1; // Leave one slot empty to distinguish full/empty
}

size_t RingBuffer::getAvailableOutputSamples() const
{
  if (headOut >= tailOut)
  {
    return headOut - tailOut; // No wrap-around
  }
  else
  {
    return bufferSize - tailOut + headOut; // Wrap-around case
  }
}