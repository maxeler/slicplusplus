#ifndef SLICPLUSPLUS_FRAMEDSTREAM_HPP_
#define SLICPLUSPLUS_FRAMEDSTREAM_HPP_

#include <stdexcept>
#include "internal/MemAlignedBuffer.hpp"

SLIC_BEGIN_NAMESPACE

class FramedStream {
	MemAlignedBuffer buf;
	std::unique_ptr<max_framed_stream_t, decltype(max_framed_stream_release)> fs;

	FramedStream(max_engine_t* engine, const std::string& name, size_t bufferSize, size_t maxFrameSize)
	 : buf(bufferSize),
	   fs(max_framed_stream_setup(engine, name.c_str(), buffer, bufferSize, maxFrameSize), max_framed_stream_release)
	{
		if (!fs) throw std::runtime_error("Failed to instantiate framed stream");
	}

public:
	size_t read(size_t numFrames, void** frames, size_t* frameSizes) {
		return max_framed_stream_read(stream, numFrames, frames, frameSizes);
	}

	void readDiscard(size_t numFrames) {
		max_framed_stream_discard(stream, numFrames);
	}

	size_t writeAcquire(size_t numFrames, void** frames) {
		return max_framed_stream_write_acquire(stream, numFrames, frames);
	}

	void write(size_t numFrames, size_t* sizes) {
		max_framed_stream_write(stream, numFrames, sizes);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_FRAMEDSTREAM_HPP_ */
