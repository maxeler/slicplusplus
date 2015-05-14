#ifndef SLICPLUSPLUS_ENGINE_HPP_
#define SLICPLUSPLUS_ENGINE_HPP_

#include "Actions.hpp"
#include "NetConnection.hpp"
#include "Run.hpp"

SLIC_BEGIN_NAMESPACE

class Engine {
	std::unique_ptr<max_engine_t, decltype(max_unload)> e;

public:
	explicit Engine(const MaxFile& maxfile, const std::string& id="*")
	 : e(max_load(maxfile.get(), id.c_str()), max_unload)
	{
		max_errors_mode(e->errors, 0);
	}

	void run(const Actions& actions) {
		max_run(e.get(), actions.get());
		SLIC_CHECK_ERRORS(e->errors)
	}

	Run runNonBlock(const Actions& actions) {
		auto ret = Run(max_run_nonblock(e.get(), actions.get()));
		SLIC_CHECK_ERRORS(e->errors)
		return ret;
	}

	void reload(const MaxFile& maxfile) {
		max_reload(e.get(), maxfile.get());
		SLIC_CHECK_ERRORS(e->errors)
	}

	void unlock() {
		max_unlock(e.get());
		SLIC_CHECK_ERRORS(e->errors)
	}

	bool isTimesourceReady() {
		auto ret = max_is_timesource_ready(e.get());
		SLIC_CHECK_ERRORS(e->errors)
		return ret;
	}

	void setTimesourceThreshold(uint32_t nsecs) {
		max_set_timesource_threshold(e.get(), nsecs);
		SLIC_CHECK_ERRORS(e->errors)
	}

	NetConnection getNetworkConnection(max_net_connection_t connection) {
		return NetConnection(e.get(), connection);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ENGINE_HPP_ */
