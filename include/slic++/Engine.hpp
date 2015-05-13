#ifndef SLICPLUSPLUS_ENGINE_HPP_
#define SLICPLUSPLUS_ENGINE_HPP_

#include "Actions.hpp"
#include "FramedStream.hpp"
#include "LLStream.hpp"
#include "Run.hpp"
#include "TcpSocket.hpp"
#include "UdpSocket.hpp"

SLIC_BEGIN_NAMESPACE

SLIC_DECLARE(Engine)

class Engine {
	friend class EngineGroup;

	MaxFileSP     maxfile;
	max_engine_t* engine;

private:
	Engine(MaxFileSP maxfile, const std::string& id)
		: maxfile(maxfile), engine(max_load(maxfile->maxfile, id.c_str())) {}

	// Constructor called by EngineGroup::lock()
	Engine(MaxFileSP maxfile, max_engine_t* engine)
		: maxfile(maxfile), engine(engine) {}

public:
	static EngineSP load(MaxFileSP maxfile, const std::string& id = "*") {
		return EngineSP(new Engine(maxfile, id));
	}

	~Engine() {
		unload();
	}

	void run(ActionsSP actions) {
		max_run(engine, actions->actions);
	}

	Run runNonBlock(ActionsSP actions) {
		return Run(max_run_nonblock(engine, actions->actions));
	}

	void reload(MaxFileSP maxfile) {
		max_reload(engine, maxfile->maxfile);
		this->maxfile = maxfile;
	}

	void unlock() {
		max_unlock(engine);
	}

	void unload() {
		if (engine) {
			max_unload(engine);
			engine = 0;
		}
	}

	bool isTimesourceReady() {
		return max_is_timesource_ready(engine);
	}

	void setTimesourceThreshold(uint32_t nsecs) {
		max_set_timesource_threshold(engine, nsecs);
	}

	NetConnection getNetworkConnection(max_net_connection_t conn) {
		return NetConnection(engine, conn);
	}

	TcpSocketSP createTcpSocket(const std::string& streamName) {
		return TcpSocketSP(new TcpSocket(engine, streamName));
	}

	TcpSocketSP createTcpSocket(const std::string& streamName, uint16_t socketNumber) {
		return TcpSocketSP(new TcpSocket(engine, streamName, socketNumber));
	}

	UdpSocketSP createUdpSocket(const std::string& streamName) {
		return UdpSocketSP(new UdpSocket(engine, streamName));
	}

	UdpSocketSP createUdpSocket(const std::string& streamName, uint16_t socketNumber) {
		return UdpSocketSP(new UdpSocket(engine, streamName, socketNumber));
	}

	FramedStreamSP setupFramedStream(const std::string& name, size_t bufferSize) {
		return FramedStreamSP(new FramedStream(engine, name, bufferSize));
	}

	template <typename SlotT>
	boost::shared_ptr<LLStream<SlotT> > setupLowLatencyStream(const std::string& name, size_t numSlots) {
		return boost::shared_ptr<LLStream<SlotT> >(new LLStream<SlotT>(engine, name, numSlots));
	}

	struct ether_addr getDefaultMacAddress(max_net_connection_t connection) {
		struct ether_addr mac;
		max_eth_get_default_mac_address(engine, connection, &mac);
		return mac;
	}

	void setManualChecksumBad(max_net_connection_t connection, max_net_manual_checksum_bad_t mode) {
		max_net_set_manual_checksum_bad(engine, connection, mode);
	}

	// returns NULL if MAC address not found (unlike SLiC C function which returns int)
	struct ether_addr* getArpEntry(max_net_connection_t connection, const struct in_addr* ip) {
		struct ether_addr* mac;
		int retval = max_arp_get_entry(engine, connection, ip, mac);
		return (retval == 0) ? mac : NULL;
	}

	void removeArpEntry(max_net_connection_t connection, const struct in_addr* ip) {
		max_arp_remove_entry(engine, connection, ip);
	}

	void addArpEntry(max_net_connection_t connection, const struct in_addr* ip, const struct ether_addr* mac) {
		max_arp_add_entry(engine, connection, ip, mac);
	}

	void configureIP(max_net_connection_t connection, const struct in_addr* ip, const struct in_addr* netmask) {
		max_ip_config(engine, connection, ip, netmask);
	}

	void joinIPMulticastGroup(max_net_connection_t connection, const struct in_addr* multicastIP) {
		max_ip_multicast_join_group(engine, connection, multicastIP);
	}

	void leaveIPMulticastGroup(max_net_connection_t connection, const struct in_addr* multicastIP) {
		max_ip_multicast_leave_group(engine, connection, multicastIP);
	}

	void setIPRouteDefaultGateway(max_net_connection_t connection, const struct in_addr* gateway) {
		max_ip_route_set_default_gw(engine, connection, gateway);
	}

	void addIPRoute(max_net_connection_t connection, const struct in_addr* destination, const struct in_addr* netmask, const struct in_addr* gateway) {
		max_ip_route_add(engine, connection, destination, netmask, gateway);
	}

	void removeIPRoute(max_net_connection_t connection, const struct in_addr* destination, const struct in_addr* netmask) {
		max_ip_route_remove(engine, connection, destination, netmask);
	}

	// returns NULL if route not found (unlike SLiC C function which returns int)
	struct in_addr* getIPRoute(max_net_connection_t connection, const struct in_addr* destination, const struct in_addr* netmask) {
		struct in_addr* gateway;
		int retval = max_ip_route_get(engine, connection, destination, netmask, gateway);
		return (retval == 0) ? gateway : NULL;
	}

	struct in_addr* getNextIPRouteHop(max_net_connection_t connection, const struct in_addr* destination) {
		struct in_addr* nextHop;
		max_ip_route_get_next_hop(engine, connection, destination, nextHop);
		return nextHop;
	}

	SLIC_ERROR_FUNCTIONS(engine->errors)
};

SLIC_END_NAMESPACE

#endif /* SLICPLUSPLUS_ENGINE_HPP_ */
