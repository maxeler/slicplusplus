#ifndef SLICPLUSPLUS_LLSTREAM_HPP_
#define SLICPLUSPLUS_LLSTREAM_HPP_

#include <stdexcept>
#include "internal/MemAlignedBuffer.hpp"

SLIC_BEGIN_NAMESPACE

class LLStream {
	static constexpr size_t MAX_SLOTS = 512;

	MemAlignedBuffer buf;
	std::unique_ptr<max_llstream_t, decltype(max_llstream_release)> ll;

	LLStream(max_engine_t* engine, const std::string& name, size_t slotSize, size_t numSlots=MAX_SLOTS)
	 : buf(numSlots*slotSize),
	   ll(max_llstream_setup(engine, name.c_str(), numSlots, slotSize, buf), max_llstream_release)
	{
		if (!ll) throw std::runtime_error("Failed to instantiate llstream");
	}

public:
	ssize_t read(size_t maxSlots, void** slots) {
		return max_llstream_read(ll.get(), maxSlots, slots);
	}

	void readDiscard(size_t numSlots) {
		max_llstream_read_discard(ll.get(), numSlots);
	}

	ssize_t writeAcquire(size_t maxSlots, void** slots) {
		return max_llstream_write_acquire(ll.get(), maxSlots, slots);
	}

	void write(size_t numSlots) {
		max_llstream_write(ll.get(), numSlots);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_LLSTREAM_HPP_ */
