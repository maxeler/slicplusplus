#ifndef SLICPLUSPLUS_FRAMEDSTREAM_HPP_
#define SLICPLUSPLUS_FRAMEDSTREAM_HPP_

#include <stdexcept>
#include <MaxSLiCInterface.h>
#include "internal/MemAlignedBuffer.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

class FramedStream {
protected:
	MemAlignedBuffer buf;
	std::unique_ptr<max_framed_stream_t, decltype(max_framed_stream_release)*> fs;

	FramedStream(max_engine_t* engine, const std::string& name, size_t bufferSize, size_t maxFrameSize)
	 : buf(bufferSize),
	   fs(max_framed_stream_setup(engine, name.c_str(), buf, bufferSize, maxFrameSize), max_framed_stream_release)
	{
		if (!fs) throw std::runtime_error("Failed to instantiate framed stream");
	}

	FramedStream() = default;

public:
	static constexpr size_t MAX_BUFFER_SIZE = 2*4096*512;
};

class FramedInputStream : public FramedStream {
	friend class Engine;

	FramedInputStream(max_engine_t* engine, const std::string& name, size_t bufferSize, size_t maxFrameSize)
	 : FramedStream(engine, name, bufferSize, maxFrameSize) {}

public:
	FramedInputStream() = default;

	size_t writeAcquire(size_t numFrames, void** frames) {
		return max_framed_stream_write_acquire(fs.get(), numFrames, frames);
	}

	void write(size_t numFrames, size_t* sizes) {
		max_framed_stream_write(fs.get(), numFrames, sizes);
	}
};

class FramedOutputStream : public FramedStream {
	friend class Engine;

	FramedOutputStream(max_engine_t* engine, const std::string& name, size_t bufferSize)
	 : FramedStream(engine, name, bufferSize, -1) {}

public:
	FramedOutputStream() = default;

	size_t read(size_t numFrames, void** frames, size_t* frameSizes) {
		return max_framed_stream_read(fs.get(), numFrames, frames, frameSizes);
	}

	void readDiscard(size_t numFrames) {
		max_framed_stream_discard(fs.get(), numFrames);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_FRAMEDSTREAM_HPP_ */
