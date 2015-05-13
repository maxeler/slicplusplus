#ifndef SLICPLUSPLUS_UDPSOCKET_HPP_
#define SLICPLUSPLUS_UDPSOCKET_HPP_

#include "MaxSLiCInterface.h"
#include "SlicConfig.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;
SLIC_DECLARE(UdpSocket)

class UdpSocket {
	friend class Engine;

	max_udp_socket_t* socket;

	UdpSocket(max_engine_t* engine, const std::string& streamName)
		: socket(max_udp_create_socket(engine, streamName.c_str())) {}

	UdpSocket(max_engine_t* engine, const std::string& streamName, uint16_t socketNumber)
		: socket(max_udp_create_socket_with_number(engine, streamName.c_str(), socketNumber)) {}

public:
	~UdpSocket() {
		close();
	}

	void bind(uint16_t localPort) {
		max_udp_bind(socket, localPort);
	}

	void bind(const struct in_addr* multicastIP, uint16_t localPort) {
		max_udp_bind_ip(socket, multicastIP, localPort);
	}

	void connect(const struct in_addr* remoteIP, uint16_t remotePort) {
		max_udp_connect(socket, remoteIP, remotePort);
	}

	void setTtl(uint8_t ttl) {
		max_udp_set_ttl(socket, ttl);
	}

	uint16_t getSocketNumber() {
		return max_udp_get_socket_number(socket);
	}

	void close() {
		if (socket) {
			max_udp_close(socket);
			socket = 0;
		}
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_UDPSOCKET_HPP_ */
