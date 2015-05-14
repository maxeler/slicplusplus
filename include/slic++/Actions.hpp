#ifndef SLICPLUSPLUS_ACTIONS_HPP_
#define SLICPLUSPLUS_ACTIONS_HPP_

#include <vector>
#include "MaxFile.hpp"

SLIC_BEGIN_NAMESPACE

class Actions {
	std::unique_ptr<max_actions_t, decltype(max_actions_free)> a;

public:
	explicit Actions(const MaxFile& maxfile, const std::string& mode="")
	 : a(max_actions_init(maxfile.get(), mode.empty() ? NULL : mode.c_str()), max_actions_free)
	{
		max_errors_mode(a->errors, 0);
	}

	max_actions_t* get() const noexcept {
		return a.get();
	}

	max_actions_t* release() noexcept {
		return a.release();
	}

	////////////////////
	// SCALARS
	////////////////////

	void set(const std::string& blockName, const std::string& regName, double value) {
		max_set_double(a.get(), blockName.c_str(), regName.c_str(), value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void set(const std::string& blockName, const std::string& regName, uint64_t value) {
		max_set_uint64t(a.get(), blockName.c_str(), regName.c_str(), value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void get(const std::string& blockName, const std::string& regName, double* ret) {
		max_get_double(a.get(), blockName.c_str(), regName.c_str(), ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void get(const std::string& blockName, const std::string& regName, uint64_t* ret) {
		max_get_uint64t(a.get(), blockName.c_str(), regName.c_str(), ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// PARAMS, OFFSETS
	////////////////////

	void setTicks(const std::string& kernelName, int ticks) {
		max_set_ticks(a.get(), kernelName.c_str(), ticks);
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// MAPPED MEMS
	////////////////////

	void setMem(const std::string& blockName, const std::string& memName, size_t idx, uint64_t value) {
		max_set_mem_uint64t(a.get(), blockName.c_str(), memName.c_str(), idx, value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void setMem(const std::string& blockName, const std::string& memName, size_t idx, double value) {
		max_set_mem_double(a.get(), blockName.c_str(), memName.c_str(), idx, value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, uint64_t* ret) {
		max_get_mem_uint64t(a.get(), blockName.c_str(), memName.c_str(), idx, ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, double* ret) {
		max_get_mem_double(a.get(), blockName.c_str(), memName.c_str(), idx, ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// STREAMS
	////////////////////

	void queueInput(const std::string& streamName, const void* data, size_t length) {
		max_queue_input(a.get(), streamName.c_str(), data, length);
		SLIC_CHECK_ERRORS(a->errors)
	}

	template <typename T>
	void queueInput(const std::string& streamName, const std::vector<T>& data) {
		queueInput(streamName, data.data(), data.size()*sizeof(T));
	}

	void queueOutput(const std::string& streamName, void* data, size_t length) {
		max_queue_output(a.get(), streamName.c_str(), data, length);
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// ROUTING
	////////////////////

	void route(const std::string& fromName, const std::string& toName) {
		max_route(a.get(), fromName.c_str(), toName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ACTIONS_HPP_ */
