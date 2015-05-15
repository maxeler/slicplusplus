#ifndef SLICPLUSPLUS_TCPSOCKET_HPP_
#define SLICPLUSPLUS_TCPSOCKET_HPP_

#include <memory>
#include <string>
#include <MaxSLiCInterface.h>
#include "Common.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

class TcpSocket {
	friend class Engine;

	std::unique_ptr<max_tcp_socket_t, decltype(max_tcp_close)*> s;

	TcpSocket(max_engine_t* engine, const std::string& streamName)
	 : s(max_tcp_create_socket(engine, streamName.c_str()), max_tcp_close)
	{
		if (!s) throw std::runtime_error("Failed to create TCP socket");
	}

	TcpSocket(max_engine_t* engine, const std::string& streamName, uint16_t socketNumber)
	 : s(max_tcp_create_socket_with_number(engine, streamName.c_str(), socketNumber), max_tcp_close)
	{
		if (!s) throw std::runtime_error("Failed to create TCP socket");
	}

public:
	TcpSocket() : s(nullptr, max_tcp_close) {}

	max_tcp_socket_t* get() const noexcept {
		return s.get();
	}

	max_tcp_socket_t* release() noexcept {
		return s.release();
	}

	void setupFraming(max_tcp_sized_config_t config) {
		max_tcp_setup_framing(s.get(), config);
	}

	void setupAdvanced(uint32_t rxWindowSizeBytes, uint32_t txWindowSizeBytes) {
		max_tcp_setup_socket_advanced(s.get(), rxWindowSizeBytes, txWindowSizeBytes);
	}

	void connect(const struct in_addr* remoteIP, uint16_t remotePort) {
		max_tcp_connect(s.get(), remoteIP, remotePort);
	}

	void listen(uint16_t localPort) {
		max_tcp_listen(s.get(), localPort);
	}

	uint16_t getSocketNumber() {
		return max_tcp_get_socket_number(s.get());
	}

	int awaitState(max_tcp_connection_state_t state, struct timeval* timeout) {
		return max_tcp_await_state(s.get(), state, timeout);
	}

	max_tcp_connection_state_t getState() {
		return max_tcp_get_connection_state(s.get());
	}

	void close() {
		max_tcp_close(s.release());
	}

	void close(max_tcp_close_mode_t closeMode) {
		max_tcp_close_advanced(s.release(), closeMode);
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_TCPSOCKET_HPP_ */
