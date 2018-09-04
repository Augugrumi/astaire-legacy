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
#include <iostream>

namespace connection {

/*
 *
 * TODO implement a packet wrapping up the tins creation and providing a builder class
 * Example to create a packet with libtins
 *
 * Tins::EthernetII pkt = \
 *     Tins::EthernetII(Tins::EthernetII::BROADCAST, iface.getMAC()) /
 *     Tins::IP(argv[2], argv[1]) /
 *     Tins::TCP(13, 15) /
 *     Tins::RawPDU("I'm a payload!");
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

        static const std::string DEFAULT_PAYLOAD;
        static const int DEFAULT_TCP_DEST_PORT;
        static const int DEFAULT_TCP_SOURCE_PORT;
        static const std::string DEFAULT_IP_DEST_ADDRESS;
        static const std::string DEFAULT_IP_SOURCE_ADDRESS;
        static const std::string DEFAULT_ETH_DEST_ADDRESS;
        static const std::string DEFAULT_ETH_SOURCE_ADDRESS;

    public:
        Builder();
        Builder& with_raw_PDU(const std::string& payload);
        Builder& with_TCP_destination_port(const int& dport);
        Builder& with_TCP_source_port(const int& sport);
        Builder& with_IP_destination_address(std::string address);
        Builder& with_IP_source_address(std::string address);
        Builder& with_ethernet_destination_address(std::string address);
        Builder& with_ethernet_source_address(std::string address);
        Packet build() const;
    };
};



} /* namespace connectionmanager */

#endif /* CONNECTION_PACKET_H_ */
