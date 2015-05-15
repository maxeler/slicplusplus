#ifndef SLICPLUSPLUS_ENGINE_HPP_
#define SLICPLUSPLUS_ENGINE_HPP_

#include "Actions.hpp"
#include "FramedStream.hpp"
#include "LLStream.hpp"
#include "NetConnection.hpp"
#include "Run.hpp"
#include "TcpSocket.hpp"
#include "UdpSocket.hpp"

SLIC_BEGIN_NAMESPACE

class Engine {
	const MaxFile& mf;
	std::unique_ptr<max_engine_t, decltype(max_unload)*> e;

public:
	explicit Engine(const MaxFile& maxfile, const std::string& id="*")
	 : mf(mf), e(max_load(maxfile.get(), id.c_str()), max_unload)
	{
		if (!e) throw std::runtime_error("Failed to load maxfile");
		max_errors_mode(e->errors, 0);
	}

	max_engine_t* get() const noexcept {
		return e.get();
	}

	max_engine_t* release() noexcept {
		return e.release();
	}

	const MaxFile& getMaxFile() const noexcept {
		return mf;
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
		auto mf  = getMaxFile().get();
		auto ret = getNetworkConnection(max_tcp_get_network_connection(mf, streamName.c_str()));
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	NetConnection getUdpStreamNetworkConnection(const std::string& streamName) {
		auto mf  = getMaxFile().get();
		auto ret = getNetworkConnection(max_udp_get_network_connection(mf, streamName.c_str()));
		SLIC_CHECK_ERRORS(mf->errors)
		return ret;
	}

	LowLatencyInputStream createLowLatencyInputStream(
			const std::string& name, size_t slotSize, size_t numSlots=LowLatencyStream::MAX_SLOTS)
	{
		return { e.get(), name, numSlots, slotSize };
	}

	LowLatencyOutputStream createLowLatencyOutputStream(
			const std::string& name, size_t slotSize, size_t numSlots=LowLatencyStream::MAX_SLOTS)
	{
		return { e.get(), name, numSlots, slotSize };
	}

	FramedInputStream createFramedInputStream(const std::string& name, size_t bufferSize, size_t maxFrameSize) {
		return { e.get(), name, bufferSize, maxFrameSize };
	}

	FramedOutputStream createFramedOutputStream(const std::string& name, size_t bufferSize) {
		return { e.get(), name, bufferSize };
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
