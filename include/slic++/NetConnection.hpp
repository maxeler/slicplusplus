#ifndef SLICPLUSPLUS_NETWORKCONNECTION_HPP
#define SLICPLUSPLUS_NETWORKCONNECTION_HPP

#include "Common.hpp"
#include "Errors.hpp"

SLIC_BEGIN_NAMESPACE

class Engine;

class NetConnection {
	friend class Engine;

	max_engine_t* engine;
	max_net_connection_t connection;

	NetConnection(max_engine_t* engine, max_net_connection_t connection)
	 : engine(engine), connection(connection) {}

public:
	struct ether_addr getDefaultMacAddress() {
		struct ether_addr mac;
		max_eth_get_default_mac_address(engine, connection, &mac);
		SLIC_CHECK_ERRORS(engine->errors)
		return mac;
	}

	void setManualChecksumBad(max_net_manual_checksum_bad_t mode) {
		max_net_set_manual_checksum_bad(engine, connection, mode);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	int getArpEntry(const struct in_addr* ip, struct ether_addr* mac) {
		int ret = max_arp_get_entry(engine, connection, ip, mac);
		SLIC_CHECK_ERRORS(engine->errors)
		return ret;
	}

	void removeArpEntry(const struct in_addr* ip) {
		max_arp_remove_entry(engine, connection, ip);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void addArpEntry(const struct in_addr* ip, const struct ether_addr* mac) {
		max_arp_add_entry(engine, connection, ip, mac);
	}

	void configureIP(const struct in_addr* ip, const struct in_addr* netmask) {
		max_ip_config(engine, connection, ip, netmask);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void joinIPMulticastGroup(const struct in_addr* multicastIP) {
		max_ip_multicast_join_group(engine, connection, multicastIP);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void leaveIPMulticastGroup(const struct in_addr* multicastIP) {
		max_ip_multicast_leave_group(engine, connection, multicastIP);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void setIPRouteDefaultGateway(const struct in_addr* gateway) {
		max_ip_route_set_default_gw(engine, connection, gateway);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void addIPRoute(
			const struct in_addr* destination,
			const struct in_addr* netmask,
			const struct in_addr* gateway)
	{
		max_ip_route_add(engine, connection, destination, netmask, gateway);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	void removeIPRoute(const struct in_addr* destination, const struct in_addr* netmask) {
		max_ip_route_remove(engine, connection, destination, netmask);
		SLIC_CHECK_ERRORS(engine->errors)
	}

	int getIPRoute(
			const struct in_addr* destination,
			const struct in_addr* netmask,
			struct in_addr* gateway)
	{
		int ret = max_ip_route_get(engine, connection, destination, netmask, gateway);
		SLIC_CHECK_ERRORS(engine->errors)
		return ret;
	}

	struct in_addr getNextIPRouteHop(const struct in_addr* destination) {
		struct in_addr nextHop;
		max_ip_route_get_next_hop(engine, connection, destination, &nextHop);
		SLIC_CHECK_ERRORS(engine->errors)
		return nextHop;
	}
};

SLIC_END_NAMESPACE

#endif //SLICPLUSPLUS_NETWORKCONNECTION_HPP
