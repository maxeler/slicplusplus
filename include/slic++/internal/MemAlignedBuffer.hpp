#ifndef SLICPLUSPLUS_MEMALIGNEDBUFFER_HPP
#define SLICPLUSPLUS_MEMALIGNEDBUFFER_HPP

#include <memory>
#include "../Common.hpp"

SLIC_BEGIN_NAMESPACE

class MemAlignedBuffer {
	static constexpr size_t PAGE_SIZE = 4096;

	std::unique_ptr<void, decltype(free)*> buf;

public:
	MemAlignedBuffer() : buf(nullptr, free) {}

	MemAlignedBuffer(size_t size) : buf(nullptr, free) {
		void* tmp;
		if (posix_memalign(&tmp, PAGE_SIZE, size)) throw std::bad_alloc();
		buf = { tmp, free };
	}

	operator void* () const noexcept {
		return buf.get();
	}
};

SLIC_END_NAMESPACE

#endif //SLICPLUSPLUS_MEMALIGNEDBUFFER_HPP
