#ifndef SLICPLUSPLUS_ERRORS_HPP_
#define SLICPLUSPLUS_ERRORS_HPP_

#define SLIC_ERROR_FUNCTIONS(E)					\
	int ok() {									\
		return max_ok(E);						\
	}											\
												\
	void clearErrors() {						\
		return max_errors_clear(E);				\
	}											\
												\
	int checkError(int errorCode) {				\
		return max_errors_check(E, errorCode);	\
	}											\
												\
	char* getErrorTrace() {						\
		return max_errors_trace(E);				\
	}											\
												\
	void setErrorMode(bool abortOnError) {		\
		max_errors_mode(E, abortOnError);		\
	}											\

#endif /* SLICPLUSPLUS_ERRORS_HPP_ */
