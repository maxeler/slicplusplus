#ifndef SLICPLUSPLUS_RUN_HPP_
#define SLICPLUSPLUS_RUN_HPP_

#include "MaxSLiCInterface.h"
#include "SlicConfig.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;
class EngineArray;
class EngineGroup;

class Run {
	friend class Engine;
	friend class EngineArray;
	friend class EngineGroup;

	max_run_t* run;

	Run(max_run_t* run) : run(run) {}

public:
	void wait() {
		max_wait(run);
	}

	void noWait() {
		max_nowait(run);
	}

	SLIC_ERROR_FUNCTIONS(run->errors)
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_RUN_HPP_ */
