#ifndef SLICPLUSPLUS_UDPSOCKET_HPP_
#define SLICPLUSPLUS_UDPSOCKET_HPP_

#include <memory>
#include <MaxSLiCInterface.h>
#include "Common.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

class UdpSocket {
	friend class Engine;

	std::unique_ptr<max_udp_socket_t, decltype(max_udp_close)*> s;

	UdpSocket(max_engine_t* engine, const std::string& streamName)
	 : s(max_udp_create_socket(engine, streamName.c_str()), max_udp_close)
	{
		if (!s) throw std::runtime_error("Failed to create UDP socket");
	}

	UdpSocket(max_engine_t* engine, const std::string& streamName, uint16_t socketNumber)
	 : s(max_udp_create_socket_with_number(engine, streamName.c_str(), socketNumber), max_udp_close)
	{
		if (!s) throw std::runtime_error("Failed to create UDP socket");
	}

public:
	max_udp_socket_t* get() const noexcept {
		return s.get();
	}

	max_udp_socket_t* release() noexcept {
		return s.release();
	}

	void bind(uint16_t localPort) {
		max_udp_bind(s.get(), localPort);
	}

	void bind(const struct in_addr* multicastIP, uint16_t localPort) {
		max_udp_bind_ip(s.get(), multicastIP, localPort);
	}

	void connect(const struct in_addr* remoteIP, uint16_t remotePort) {
		max_udp_connect(s.get(), remoteIP, remotePort);
	}

	void setTtl(uint8_t ttl) {
		max_udp_set_ttl(s.get(), ttl);
	}

	uint16_t getSocketNumber() {
		return max_udp_get_socket_number(s.get());
	}

	void close() {
		max_udp_close(s.release());
	}
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_UDPSOCKET_HPP_ */
