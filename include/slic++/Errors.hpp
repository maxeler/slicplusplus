#ifndef SLICPLUSPLUS_ERRORS_HPP_
#define SLICPLUSPLUS_ERRORS_HPP_

#include <stdexcept>
#include <MaxSLiCInterface.h>

#define SLIC_CHECK_ERRORS(E)                        \
	if (!max_ok(E)) {                               \
		auto msg = max_errors_trace(E);             \
		auto err = std::runtime_error(msg);         \
		max_clear_errors(E);                        \
		free(msg);                                  \
		throw err;                                  \
	}

#endif /* SLICPLUSPLUS_ERRORS_HPP_ */
