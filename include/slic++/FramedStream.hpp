#ifndef SLICPLUSPLUS_FRAMEDSTREAM_HPP_
#define SLICPLUSPLUS_FRAMEDSTREAM_HPP_

#include "MaxSLiCInterface.h"
#include "SlicConfig.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;
SLIC_DECLARE(FramedStream)

class FramedStream {
	friend class Engine;

	void* buffer;
	max_framed_stream_t* stream;

	FramedStream(max_engine_t* engine, const std::string& name, size_t bufferSize, size_t maxFrameSize) {
		posix_memalign(&buffer, 4096, bufferSize);
		stream = max_framed_stream_setup(engine, name.c_str(), buffer, bufferSize, maxFrameSize);
	}

public:
	~FramedStream() {
		release();
	}

	size_t read(size_t numFrames, void** frames, size_t* frameSizes) {
		return max_framed_stream_read(stream, numFrames, frames, frameSizes);
	}

	void readDiscard(size_t numFrames = 1) {
		max_framed_stream_discard(stream, numFrames);
	}

	size_t writeAcquire(size_t numFrames, void** frames) {
		return max_framed_stream_write_acquire(stream, numFrames, frames);
	}

	void write(size_t numFrames, size_t* sizes) {
		max_framed_stream_write(stream, numFrames, sizes);
	}

	void release() {
		if (stream) {
			max_framed_stream_release(stream);
			stream = 0;
		}

		free(buffer);
		buffer = 0;
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_FRAMEDSTREAM_HPP_ */
