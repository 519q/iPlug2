#include "IPlugConstants.h"
//#include "projects/DomeShaper.h"
#include <array>

class RingBuffer
{
private:
  static constexpr size_t chunkSize = 128;   // Fixed chunk size
  static constexpr size_t hopSize = 64;     // Fixed hop size
  static constexpr size_t bufferSize = 1024; // Fixed buffer size
  bool chunkRead{};

  std::array<double, bufferSize> mBuffer{};       // Fixed-size buffer
  std::array<double, chunkSize> mOverlapBuffer{}; // Fixed-size overlap buffer
  std::array<double, chunkSize> mChunkBuffer{};   // Fixed-size overlap buffer
  std::array<double, chunkSize> hannWindow{};
  size_t head = 0; // Write position
  size_t tail = 0; // Read positiond

  std::array<double, bufferSize> mOutputBuffer{}; // Output buffer for processed samples
  size_t headOut = 0;                             // Write position for output buffer
  size_t tailOut = 0;                             // Read position for output buffer

public:
  RingBuffer() { initializeHannWindow(); }

  bool getChunkRead() const { return chunkRead; }

  std::array<double, chunkSize>& getChunkBuffer() { return mChunkBuffer; }

  std::array<double, chunkSize>& getOverlapBuffer() { return mOverlapBuffer; }

  void rotateBuffer(std::array<double, chunkSize>& buffer, size_t shift);

  static constexpr size_t getBufferSize() { return bufferSize; }

  static constexpr size_t getChunkSize() { return chunkSize; };

  size_t getHopSize() const;

  void initializeHannWindow();

  // Write a sample to the buffer
  void write(double sample);

  // Read a sample from the buffer
  double read();

  void readChunk();

  void writeChunk();

  // Get the number of available samples for reading
  size_t getAvailableSamples() const;

  size_t getAvailableOutputSamples() const;

  // Get the number of free slots for writing
  size_t getFreeSlots() const;
};