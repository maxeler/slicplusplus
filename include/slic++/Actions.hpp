#ifndef SLICPLUSPLUS_ACTIONS_HPP_
#define SLICPLUSPLUS_ACTIONS_HPP_

#include <vector>
#include "MaxFile.hpp"

SLIC_BEGIN_NAMESPACE

class Actions {
	std::unique_ptr<max_actions_t, decltype(max_actions_free)*> a;

public:
	explicit Actions(const MaxFile& maxfile, const std::string& mode="")
	 : a(max_actions_init(maxfile.get(), mode.empty() ? NULL : mode.c_str()), max_actions_free)
	{
		if (!a) throw std::runtime_error("Failed to instantiate actions");
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

	void ignoreScalar(const std::string& blockName, const std::string& regName) {
		max_ignore_scalar(a.get(), blockName.c_str(), regName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// PARAMS, OFFSETS
	////////////////////

	void setParam(const std::string& name, double value) {
		max_set_param_double(a.get(), name.c_str(), value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void setParam(const std::string& name, uint64_t value) {
		max_set_param_uint64t(a.get(), name.c_str(), value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreKernel(const std::string& kernelName) {
		max_ignore_kernel(a.get(), kernelName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void setTicks(const std::string& kernelName, int ticks) {
		max_set_ticks(a.get(), kernelName.c_str(), ticks);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void setOffset(const std::string& kernelName, const std::string& varName, int value) {
		max_set_offset(a.get(), kernelName.c_str(), varName.c_str(), value);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreOffset(const std::string& kernelName, const std::string& varName) {
		max_ignore_offset(a.get(), kernelName.c_str(), varName.c_str());
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

	template <typename T>
	void setMem(const std::string& blockName, const std::string& memName, const std::vector<T>& values) {
		for (size_t idx = 0; idx < values.size(); ++idx)
			setMem(blockName, memName, idx, values[idx]);
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, uint64_t* ret) {
		max_get_mem_uint64t(a.get(), blockName.c_str(), memName.c_str(), idx, ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, double* ret) {
		max_get_mem_double(a.get(), blockName.c_str(), memName.c_str(), idx, ret);
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreMem(const std::string& blockName, const std::string& memName) {
		max_ignore_mem(a.get(), blockName.c_str(), memName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreMemInput(const std::string& blockName, const std::string& memName) {
		max_ignore_mem_input(a.get(), blockName.c_str(), memName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreMemOutput(const std::string& blockName, const std::string& memName) {
		max_ignore_mem_output(a.get(), blockName.c_str(), memName.c_str());
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
		max_route(a.get(), fromName.c_str(), toName.empty() ? nullptr : toName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void route(const std::string& route) {
		max_route_string(a.get(), route.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void ignoreRoute(const std::string& blockName) {
		max_ignore_route(a.get(), blockName.c_str());
		SLIC_CHECK_ERRORS(a->errors)
	}

	////////////////////
	// MISC
	////////////////////

	void disableReset() {
		max_disable_reset(a.get());
		SLIC_CHECK_ERRORS(a->errors)
	}

	void disableValidation() {
		max_disable_validation(a.get());
		SLIC_CHECK_ERRORS(a->errors)
	}

	/**
	 * 1 = valid, 0 = not valid
	 */
	int validate() {
		auto ret = max_validate(a.get());
		SLIC_CHECK_ERRORS(a->errors)
		return ret;
	}

	void setWatchRange(const std::string& kernelName, int minTick, int maxTick) {
		max_watch_range(a.get(), kernelName.c_str(), minTick, maxTick);
		SLIC_CHECK_ERRORS(a->errors)
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ACTIONS_HPP_ */
