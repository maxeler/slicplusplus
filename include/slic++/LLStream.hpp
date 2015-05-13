#ifndef SLICPLUSPLUS_LLSTREAM_HPP_
#define SLICPLUSPLUS_LLSTREAM_HPP_

#include "MaxSLiCInterface.h"
#include "SlicConfig.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

template <typename SlotT>
class LLStream {
	friend class Engine;

	SlotT* buffer;
	max_llstream_t* llstream;

	LLStream(max_engine_t* engine, const std::string& name, size_t numSlots) {
		posix_memalign(&buffer, 4096, numSlots * sizeof(SlotT));
		llstream = max_llstream_setup(engine, name.c_str(), numSlots, sizeof(SlotT), buffer.data());
	}

public:
	~LLStream() {
		release();
	}

	ssize_t read(size_t maxSlots, SlotT** slots) {
		return max_llstream_read(llstream, maxSlots, slots);
	}

	void readDiscard(size_t numSlots = 1) {
		max_llstream_read_discard(llstream, numSlots);
	}

	ssize_t writeAcquire(size_t maxSlots, SlotT** slots) {
		return max_llstream_write_acquire(llstream, maxSlots, slots);
	}

	void write(size_t numSlots = 1) {
		max_llstream_write(llstream, numSlots);
	}

	void release() {
		if (llstream) {
			max_llstream_release(llstream);
			llstream = 0;
		}

		free(buffer);
		buffer = 0;
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_LLSTREAM_HPP_ */
