#include "Packet.h"

namespace connection {

Packet::Packet() {}

Packet::~Packet() {
    delete packet;
}

std::vector<uint8_t> Packet::serialize() {
    return packet->serialize();
}

const std::string Packet::Builder::DEFAULT_PAYLOAD = "";
const int Packet::Builder::DEFAULT_TCP_DEST_PORT = -1;
const int Packet::Builder::DEFAULT_TCP_SOURCE_PORT = -1;
const std::string Packet::Builder::DEFAULT_IP_DEST_ADDRESS = "-1";
const std::string Packet::Builder::DEFAULT_IP_SOURCE_ADDRESS = "-1";
const std::string Packet::Builder::DEFAULT_ETH_DEST_ADDRESS = "-1";
const std::string Packet::Builder::DEFAULT_ETH_SOURCE_ADDRESS = "-1";

Packet::Builder::Builder() : payload(""),
        tcp_dest_port(-1),
        tcp_source_port(-1),
        ip_dest_address(""),
        ip_source_address(""),
        eth_dest_address(""),
        eth_source_address(""){}

Packet::Builder& Packet::Builder::with_raw_PDU(const std::string& payload) {
    this->payload = payload;
    return *this;
}

Packet::Builder& Packet::Builder::with_TCP_destination_port(const int& dport) {
    this->tcp_dest_port = dport;
    return *this;
}

Packet::Builder& Packet::Builder::with_TCP_source_port(const int& sport) {
    this->tcp_source_port = sport;
    return *this;
}

Packet::Builder& Packet::Builder::with_IP_destination_address(std::string address) {
    this->ip_dest_address = address;
    return *this;
}

Packet::Builder& Packet::Builder::with_IP_source_address(std::string address) {
    this->ip_source_address = address;
    return *this;
}

Packet::Builder& Packet::Builder::with_ethernet_destination_address(std::string address) {
    this->eth_dest_address = address;
    return *this;
}

Packet::Builder& Packet::Builder::with_ethernet_source_address(std::string address) {
    this->eth_source_address = address;
    return *this;
}

Packet Packet::Builder::build() const{
    Packet p;
    p.packet = new Tins::RawPDU(payload);
    if ((((this->eth_source_address).compare(DEFAULT_ETH_SOURCE_ADDRESS) ||
            (this->eth_dest_address).compare(DEFAULT_ETH_DEST_ADDRESS))) != 0) {
        Tins::EthernetII temp = Tins::EthernetII(this->eth_source_address, this->eth_dest_address);
        Tins::PDU* to_delete = p.packet;
        p.packet = new Tins::EthernetII(temp / *p.packet);
        delete to_delete;
    }
    if ((this->tcp_source_port != DEFAULT_TCP_SOURCE_PORT) &&
            (this->tcp_dest_port != DEFAULT_TCP_DEST_PORT)) {
        Tins::TCP temp = Tins::TCP(this->tcp_source_port, this->tcp_dest_port);
        Tins::PDU* to_delete = p.packet;
        p.packet = new Tins::TCP(temp / *p.packet);
        delete to_delete;
    }
    if ((((this->ip_source_address).compare(DEFAULT_IP_SOURCE_ADDRESS) ||
            (this->ip_dest_address).compare(DEFAULT_IP_DEST_ADDRESS))) != 0) {
        std::cout << this->ip_source_address << std::endl;
        std::cout << this->ip_dest_address << std::endl;
        Tins::IP temp = Tins::IP(this->ip_source_address, this->ip_dest_address);
        Tins::PDU* to_delete = p.packet;
        p.packet = new Tins::IP(temp / *p.packet);
        delete to_delete;
    }
    return p;
}

} /* namespace connectionmanager */
