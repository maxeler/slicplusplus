#ifndef SLICPLUSPLUS_MAXFILE_HPP_
#define SLICPLUSPLUS_MAXFILE_HPP_

#include <memory>
#include <string>
#include "Common.hpp"
#include "Errors.hpp"

#define SLIC_DESIGN(A)              \
	extern "C" {                    \
		max_file_t* A##_init(void); \
	}

SLIC_BEGIN_NAMESPACE

class MaxFile {
	std::unique_ptr<max_file_t, decltype(max_file_free)*> mf;

public:
	explicit MaxFile(const std::function<max_file_t*()>& initFn) : mf(initFn(), max_file_free) {
		if (!mf) throw std::runtime_error("Failed to instantiate maxfile");
		max_errors_mode(mf->errors, 0);
	}

	max_file_t* get() const noexcept {
		return mf.get();
	}

	max_file_t* release() noexcept {
		return mf.release();
	}

	uint64_t getConstant(const std::string& name) const {
		auto ret = max_get_constant_uint64t(mf.get(), name.c_str());
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	double getDoubleConstant(const std::string& name) const {
		auto ret = max_get_constant_double(mf.get(), name.c_str());
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	const char* getStringConstant(const std::string& name) const {
		auto ret = max_get_constant_string(mf.get(), name.c_str());
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	bool isSimulation() const {
		return (getConstant("IS_SIMULATION") == 1);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_MAXFILE_HPP_ */
