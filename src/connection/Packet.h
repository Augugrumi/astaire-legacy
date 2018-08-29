/*
 * Packet.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef CONNECTION_PACKET_H_
#define CONNECTION_PACKET_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <tins/tins.h>

namespace connectionmanager {

/*
 *
 * TODO implement a packet wrapping up the tins creation and providing a builder class
 * Example to create a packet with libtins
 *
 * Tins::EthernetII pkt = \
 *	   Tins::EthernetII(Tins::EthernetII::BROADCAST, iface.getMAC()) /
 *	   Tins::IP(argv[2], argv[1]) /
 *	   Tins::TCP(13, 15) /
 *	   Tins::RawPDU("I'm a payload!");
 *
 */
class Packet {
private:
	Tins::PDU* packet;
	Packet();

public:
	virtual ~Packet();
	std::vector<uint8_t> serialize();

	class Builder {
	private:
		std::string payload;
		int tcp_dest_port;
		int tcp_source_port;
		std::string ip_dest_address;
		std::string ip_source_address;
		std::string eth_dest_address;
		std::string eth_source_address;

		const std::string& DEFAULT_PAYLOAD = "";
		const int& DEFAULT_TCP_DEST_PORT = -1;
		const int& DEFAULT_TCP_SOURCE_PORT = -1;
		const std::string& DEFAULT_IP_DEST_ADDRESS = "-1";
		const std::string& DEFAULT_IP_SOURCE_ADDRESS = "-1";
		const std::string& DEFAULT_ETH_DEST_ADDRESS = "-1";
		const std::string& DEFAULT_ETH_SOURCE_ADDRESS = "-1";

	public:
		Builder();
		Builder* withRawPDU(const std::string& payload);
		Builder* withTCPDestinationPort(const int& dport);
		Builder* withTCPSourcePort(const int& sport);
		Builder* withIPDestinationAddress(std::string address);
		Builder* withIPSourceAddress(std::string address);
		Builder* withEthernetDestinationAddress(std::string address);
		Builder* withEthernetSourceAddress(std::string address);
		Packet build();
	};
};

} /* namespace connectionmanager */

#endif /* CONNECTION_PACKET_H_ */
