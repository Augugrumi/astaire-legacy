/*
 * Packet.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef CONNECTION_PACKET_H_
#define CONNECTION_PACKET_H_

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
public:
	virtual ~Packet();

private:
	Packet();
};

} /* namespace connectionmanager */

#endif /* CONNECTION_PACKET_H_ */
