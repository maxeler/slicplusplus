#ifndef SLICPLUSPLUS_ENGINE_HPP_
#define SLICPLUSPLUS_ENGINE_HPP_

#include "Actions.hpp"
#include "FramedStream.hpp"
#include "LLStream.hpp"
#include "NetConnection.hpp"
#include "Run.hpp"
#include "TcpSocket.hpp"
#include "UdpSocket.hpp"

extern "C" max_file_t* max_engine_get_max_file(max_engine_t *engine);

SLIC_BEGIN_NAMESPACE

class Engine {
	std::unique_ptr<max_engine_t, decltype(max_unload)*> e;

public:
	explicit Engine(const MaxFile& maxfile, const std::string& id="*")
	 : e(max_load(maxfile.get(), id.c_str()), max_unload)
	{
		if (!e) throw std::runtime_error("Failed to load maxfile");
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

	NetConnection getNetworkConnection(max_net_connection_t connection) const noexcept {
		return { e.get(), connection };
	}

	NetConnection getTcpStreamNetworkConnection(const std::string& streamName) {
		auto mf  = max_engine_get_max_file(e.get());
		auto ret = getNetworkConnection(max_tcp_get_network_connection(mf, streamName.c_str()));
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	NetConnection getUdpStreamNetworkConnection(const std::string& streamName) {
		auto mf  = max_engine_get_max_file(e.get());
		auto ret = getNetworkConnection(max_udp_get_network_connection(mf, streamName.c_str()));
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	LLStream createLowLatencyStream(const std::string& name, size_t slotSize, size_t numSlots=LLStream::MAX_SLOTS) {
		return { e.get(), name, numSlots, slotSize };
	}

	FramedStream createFramedStream(const std::string& name, size_t bufferSize, size_t maxFrameSize) {
		return { e.get(), name, bufferSize, maxFrameSize };
	}

	TcpSocket createTcpSocket(const std::string& name) {
		return { e.get(), name };
	}

	TcpSocket createTcpSocket(const std::string& name, uint16_t socketNumber) {
		return { e.get(), name, socketNumber };
	}

	UdpSocket createUdpSocket(const std::string& name) {
		return { e.get(), name };
	}

	UdpSocket createUdpSocket(const std::string& name, uint16_t socketNumber) {
		return { e.get(), name, socketNumber };
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ENGINE_HPP_ */
