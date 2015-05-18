#ifndef SLICPLUSPLUS_LLSTREAM_HPP_
#define SLICPLUSPLUS_LLSTREAM_HPP_

#include <stdexcept>
#include <MaxSLiCInterface.h>
#include "internal/MemAlignedBuffer.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

class LowLatencyStream {
protected:
	size_t numSlots;
	MemAlignedBuffer buf;
	std::unique_ptr<max_llstream_t, decltype(max_llstream_release)*> ll;

	LowLatencyStream() : ll(nullptr, max_llstream_release) {}

	LowLatencyStream(max_engine_t* engine, const std::string& name, size_t numSlots, size_t slotSize)
	: numSlots(numSlots),
	  buf(numSlots*slotSize),
	  ll(max_llstream_setup(engine, name.c_str(), numSlots, slotSize, buf), max_llstream_release)
	{
		if (!ll) throw std::runtime_error("Failed to instantiate llstream");
	}

public:
	static constexpr size_t MAX_SLOTS = 512;

	size_t getNumSlots() const noexcept {
		return numSlots;
	}
};

class LowLatencyInputStream : public LowLatencyStream {
	friend class Engine;

	LowLatencyInputStream(max_engine_t* engine, const std::string& name, size_t numSlots, size_t slotSize)
	 : LowLatencyStream(engine, name, numSlots, slotSize) {}

public:
	LowLatencyInputStream() = default;

	ssize_t acquire(size_t numSlots, void** slots) {
		return max_llstream_write_acquire(ll.get(), numSlots, slots);
	}

	void write(size_t numSlots) {
		max_llstream_write(ll.get(), numSlots);
	}
};

class LowLatencyOutputStream : public LowLatencyStream {
	friend class Engine;

	LowLatencyOutputStream(max_engine_t* engine, const std::string& name, size_t numSlots, size_t slotSize)
	 : LowLatencyStream(engine, name, numSlots, slotSize) {}

public:
	LowLatencyOutputStream() = default;

	ssize_t read(size_t numSlots, void** slots) {
		return max_llstream_read(ll.get(), numSlots, slots);
	}

	void discard(size_t numSlots) {
		max_llstream_read_discard(ll.get(), numSlots);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_LLSTREAM_HPP_ */
