#ifndef SLICPLUSPLUS_ACTIONS_HPP_
#define SLICPLUSPLUS_ACTIONS_HPP_

#include <vector>
#include "MaxFile.hpp"

SLIC_BEGIN_NAMESPACE

class ActionsArray;
SLIC_DECLARE(Actions)

class Actions {
	friend class ActionsArray;
	friend class Engine;
	friend class EngineGroup;

	max_actions_t* actions;

	Actions(MaxFileSP maxfile, const std::string& mode)
		: actions(max_actions_init(maxfile->maxfile, mode.empty() ? NULL : mode.c_str())) {}

public:
	static ActionsSP init(MaxFileSP maxfile, const std::string& mode = "") {
		return ActionsSP(new Actions(maxfile, mode));
	}

	~Actions() {
		max_actions_free(actions);
	}

	////////////////////
	// SCALARS
	////////////////////

	void set(const std::string& blockName, const std::string& regName, double value) {
		max_set_double(actions, blockName.c_str(), regName.c_str(), value);
	}

	void set(const std::string& blockName, const std::string& regName, uint64_t value) {
		max_set_uint64t(actions, blockName.c_str(), regName.c_str(), value);
	}

	void get(const std::string& blockName, const std::string& regName, double* ret) {
		max_get_double(actions, blockName.c_str(), regName.c_str(), ret);
	}

	void get(const std::string& blockName, const std::string& regName, uint64_t* ret) {
		max_get_uint64t(actions, blockName.c_str(), regName.c_str(), ret);
	}

	void ignoreScalar(const std::string& blockName, const std::string& regName) {
		max_ignore_scalar(actions, blockName.c_str(), regName.c_str());
	}

	////////////////////
	// PARAMS, OFFSETS
	////////////////////

	void setParam(const std::string& name, double value) {
		max_set_param_double(actions, name.c_str(), value);
	}

	void setParam(const std::string& name, uint64_t value) {
		max_set_param_uint64t(actions, name.c_str(), value);
	}

	void ignoreKernel(const std::string& kernelName) {
		max_ignore_kernel(actions, kernelName.c_str());
	}

	void setTicks(const std::string& kernelName, int ticks) {
		max_set_ticks(actions, kernelName.c_str(), ticks);
	}

	void setOffset(const std::string& kernelName, const std::string& varName, int value) {
		max_set_offset(actions, kernelName.c_str(), varName.c_str(), value);
	}

	void ignoreOffset(const std::string& kernelName, const std::string& varName) {
		max_ignore_offset(actions, kernelName.c_str(), varName.c_str());
	}

	////////////////////
	// MAPPED MEMS
	////////////////////

	void setMem(const std::string& blockName, const std::string& memName, size_t idx, uint64_t value) {
		max_set_mem_uint64t(actions, blockName.c_str(), memName.c_str(), idx, value);
	}

	void setMem(const std::string& blockName, const std::string& memName, size_t idx, double value) {
		max_set_mem_double(actions, blockName.c_str(), memName.c_str(), idx, value);
	}

	template <typename T>
	void setMem(const std::string& blockName, const std::string& memName, const std::vector<T>& values) {
		for (size_t idx = 0; idx < values.size(); ++idx)
			setMem(blockName, memName, idx, values[idx]);
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, uint64_t* ret) {
		max_get_mem_uint64t(actions, blockName.c_str(), memName.c_str(), idx, ret);
	}

	void getMem(const std::string& blockName, const std::string& memName, size_t idx, double* ret) {
		max_get_mem_double(actions, blockName.c_str(), memName.c_str(), idx, ret);
	}

	/**
	 * Note that the receiving vector must be sized correctly before calling this function
	 */
	template <typename T>
	void getMem(const std::string& blockName, const std::string& memName, std::vector<T>& values) {
		for (size_t idx = 0; idx < values.size(); ++idx)
			getMem(blockName, memName, idx, &values[idx]);
	}

	void ignoreMem(const std::string& blockName, const std::string& memName) {
		max_ignore_mem(actions, blockName.c_str(), memName.c_str());
	}

	void ignoreMemInput(const std::string& blockName, const std::string& memName) {
		max_ignore_mem_input(actions, blockName.c_str(), memName.c_str());
	}

	void ignoreMemOutput(const std::string& blockName, const std::string& memName) {
		max_ignore_mem_output(actions, blockName.c_str(), memName.c_str());
	}

	////////////////////
	// STREAMS
	////////////////////

	/**
	 * IMPORTANT: Note that the length parameter NOT bytes but # elements
	 */
	template <typename T>
	void queueInput(const std::string& streamName, const T* data, size_t length) {
		max_queue_input(actions, streamName.c_str(), data, length*sizeof(T));
	}

	template <typename T>
	void queueInput(const std::string& streamName, const std::vector<T>& data) {
		queueInput(streamName, &data.front(), data.size());
	}

	/**
	 * IMPORTANT: Note that the length parameter NOT bytes but # elements
	 */
	template <typename T>
	void queueOutput(const std::string& streamName, T* data, size_t length) {
		max_queue_output(actions, streamName.c_str(), data, length*sizeof(T));
	}

	/**
	 * Note that the vector must be sized correctly before calling this function
	 */
	template <typename T>
	void queueOutput(const std::string& streamName, std::vector<T>& data) {
		queueOutput(streamName, &data.front(), data.size());
	}

	void ignoreStream(const std::string& streamName) {
		max_ignore_stream(actions, streamName.c_str());
	}

	////////////////////
	// ROUTING
	////////////////////

	void route(const std::string& fromName, const std::string& toName) {
		max_route(actions, fromName.c_str(), toName.c_str());
	}

	void route(const std::string& route) {
		max_route_string(actions, route.c_str());
	}

	void ignoreRoute(const std::string& blockName) {
		max_ignore_route(actions, blockName.c_str());
	}

	////////////////////
	// MISC
	////////////////////

	void disableReset() {
		max_disable_reset(actions);
	}

	void disableValidation() {
		max_disable_validation(actions);
	}

	/**
	 * 1 = valid, 0 = not valid
	 */
	int validate() {
		return max_validate(actions);
	}

	void setDebugDir(const std::string& kernelName, const std::string& debugDir) {
		max_debug_dir(actions, kernelName.c_str(), debugDir.c_str());
	}

	void setWatchRange(const std::string& kernelName, int minTick, int maxTick) {
		max_watch_range(actions, kernelName.c_str(), minTick, maxTick);
	}

	void setPrintTo(const std::string& kernelName, const std::string& filename) {
		max_printto(actions, kernelName.c_str(), filename.c_str());
	}

	SLIC_ERROR_FUNCTIONS(actions->errors)
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ACTIONS_HPP_ */
