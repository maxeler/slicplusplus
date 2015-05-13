#ifndef SLICPLUSPLUS_TCPSOCKET_HPP_
#define SLICPLUSPLUS_TCPSOCKET_HPP_

#include "MaxSLiCInterface.h"
#include "SlicConfig.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;
SLIC_DECLARE(TcpSocket)

class TcpSocket {
	friend class Engine;

	max_tcp_socket_t* socket;

	TcpSocket(max_engine_t* engine, const std::string& streamName)
		: socket(max_tcp_create_socket(engine, streamName.c_str())) {}

	TcpSocket(max_engine_t* engine, const std::string& streamName, uint16_t socketNumber)
		: socket(max_tcp_create_socket_with_number(engine, streamName.c_str(), socketNumber)) {}

public:
	~TcpSocket() {
		close();
	}

	void setupFraming(max_tcp_sized_config_t config) {
		max_tcp_setup_framing(socket, config);
	}

	void setupAdvanced(uint32_t rxWindowSizeBytes, uint32_t txWindowSizeBytes) {
		max_tcp_setup_socket_advanced(socket, rxWindowSizeBytes, txWindowSizeBytes);
	}

	void connect(const struct in_addr* remoteIP, uint16_t remotePort) {
		max_tcp_connect(socket, remoteIP, remotePort);
	}

	void listen(uint16_t localPort) {
		max_tcp_listen(socket, localPort);
	}

	uint16_t getSocketNumber() {
		return max_tcp_get_socket_number(socket);
	}

	int awaitState(max_tcp_connection_state_t state, struct timeval* timeout) {
		return max_tcp_await_state(socket, state, timeout);
	}

	max_tcp_connection_state_t getState() {
		return max_tcp_get_connection_state(socket);
	}

	void close() {
		if (socket) {
			max_tcp_close(socket);
			socket = 0;
		}
	}

	void close(max_tcp_close_mode_t closeMode) {
		if (socket) {
			max_tcp_close_advanced(socket, closeMode);
			socket = 0;
		}
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_TCPSOCKET_HPP_ */
