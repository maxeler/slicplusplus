#ifndef SLICPLUSPLUS_MAXFILE_HPP_
#define SLICPLUSPLUS_MAXFILE_HPP_

#include <string>
#include "MaxSLiCInterface.h"
#include "Errors.hpp"
#include "SlicConfig.hpp"

#define SLIC_DESIGN(A)				\
	extern "C" {					\
		max_file_t* A##_init(void);	\
	}

SLIC_BEGIN_NAMESPACE

class Actions;
class Engine;
class EngineArray;
class EngineGroup;
SLIC_DECLARE(MaxFile)

class MaxFile {
	friend class Actions;
	friend class Engine;
	friend class EngineArray;
	friend class EngineGroup;

	typedef max_file_t* (*MaxFileInitFn)(void);

	max_file_t* maxfile;

	MaxFile(MaxFileInitFn initFn)
		: maxfile(initFn()) {}

public:
	static MaxFileSP init(MaxFileInitFn initFn) {
		return MaxFileSP(new MaxFile(initFn));
	}

	~MaxFile() {
		max_file_free(maxfile);
	}

	template <typename T>
	T getConstant(const std::string& name) const {
		return T(max_get_constant_uint64t(maxfile, name.c_str()));
	}

	std::string getStringConstant(const std::string& name) const {
		return getConstant<std::string>(name);
	}

	bool isSimulation() const {
		return getConstant<bool>("IS_SIMULATION");
	}

	int getBurstSize(const std::string& name) const {
		return max_get_burst_size(maxfile, name.c_str());
	}

	max_net_connection_t getTcpStreamNetworkConnection(const std::string& streamName) {
		return max_tcp_get_network_connection(maxfile, streamName.c_str());
	}

	max_net_connection_t getUdpStreamNetworkConnection(const std::string& streamName) {
		return max_udp_get_network_connection(maxfile, streamName.c_str());
	}

	SLIC_ERROR_FUNCTIONS(maxfile->errors)
};

template <>
std::string MaxFile::getConstant(const std::string& name) const {
	return max_get_constant_string(maxfile, name.c_str());
}

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_MAXFILE_HPP_ */
