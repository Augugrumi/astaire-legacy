/*
 * Packet.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#include "Packet.h"

namespace connectionmanager {

Packet::Packet() {}

Packet::~Packet() {
	delete packet;
}

Packet::Builder::Builder() : payload(Packet::Builder::DEFAULT_PAYLOAD),
		tcp_dest_port(Packet::Builder::DEFAULT_TCP_DEST_PORT),
		tcp_source_port(Packet::Builder::DEFAULT_TCP_SOURCE_PORT),
		ip_dest_address(Packet::Builder::DEFAULT_IP_DEST_ADDRESS),
		ip_source_address(Packet::Builder::DEFAULT_IP_SOURCE_ADDRESS),
		eth_dest_address(Packet::Builder::DEFAULT_ETH_DEST_ADDRESS),
		eth_source_address(Packet::Builder::DEFAULT_ETH_SOURCE_ADDRESS){}

Packet::Builder* Packet::Builder::withRawPDU(const std::string& payload) {
	this->payload = payload;
	return this;
}

Packet::Builder* Packet::Builder::withTCPDestinationPort(const int& dport) {
	this->tcp_dest_port = dport;
	return this;
}

Packet::Builder* Packet::Builder::withTCPSourcePort(const int& sport) {
	this->tcp_source_port = sport;
	return this;
}

Packet::Builder* Packet::Builder::withIPDestinationAddress(std::string address) {
	this->ip_dest_address = address;
	return this;
}

Packet::Builder* Packet::Builder::withIPSourceAddress(std::string address) {
	this->ip_source_address = address;
	return this;
}

Packet::Builder* Packet::Builder::withEthernetDestinationAddress(std::string address) {
	this->eth_dest_address = address;
	return this;
}

Packet::Builder* Packet::Builder::withEthernetSourceAddress(std::string address) {
	this->eth_source_address = address;
	return this;
}

Packet Packet::Builder::build() {
	Packet p;
	p.packet = new Tins::RawPDU(payload);
	if ((((this->eth_source_address).compare(DEFAULT_ETH_SOURCE_ADDRESS) ||
			(this->eth_dest_address).compare(DEFAULT_ETH_DEST_ADDRESS))) != 0) {
		Tins::EthernetII temp = Tins::EthernetII(this->eth_source_address, this->eth_dest_address);
		p.packet = new Tins::EthernetII(temp / *p.packet);
	}
	if ((this->tcp_source_port != DEFAULT_TCP_SOURCE_PORT) &&
			(this->tcp_dest_port != DEFAULT_TCP_DEST_PORT)) {
		Tins::TCP temp = Tins::TCP(this->tcp_source_port, this->tcp_dest_port);
		p.packet = new Tins::TCP(temp / *p.packet);
	}
	if ((((this->ip_source_address).compare(DEFAULT_IP_SOURCE_ADDRESS) ||
			(this->ip_dest_address).compare(DEFAULT_IP_DEST_ADDRESS))) != 0) {
		Tins::IP temp = Tins::IP(this->ip_source_address, this->ip_dest_address);
		p.packet = new Tins::IP(temp / *p.packet);
	}
	return p;
}

} /* namespace connectionmanager */
