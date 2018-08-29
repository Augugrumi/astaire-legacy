/*
 * ConnectionManager.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#include "ConnectionManager.h"

namespace connectionmanager {

// TODO think better initializer
ConnectionManager::ConnectionManager() : iface(), handler() {}

ConnectionManager::~ConnectionManager() {
	// TODO Auto-generated destructor stub
}

// TODO Better move to constructor?
void ConnectionManager::setupInterface(std::string interface_name, std::string ip) {
	iface = boost::shared_ptr<viface::VIface>(new viface::VIface(interface_name));
	iface->setIPv4(ip);
}

void ConnectionManager::start() {
	if (iface && !iface->isUp())
		iface->up();
}

void ConnectionManager::setMessageHandler(handler::Handler* handler){
	this->handler = boost::shared_ptr<handler::Handler>(handler);
}

void ConnectionManager::send_method(std::vector<uint8_t> raw_packet) {
	// method to add eventual logging
	iface->send(raw_packet);
}

void ConnectionManager::send(std::vector<uint8_t> raw_packet) {
	boost::thread t{send_method, raw_packet};
	t.detach();
}

void ConnectionManager::receive_method() {
	// method to add eventual logging
}

void ConnectionManager::receive(){

}

} /* namespace connectionmanager */

