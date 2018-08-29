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

void ConnectionManager::send(std::vector<uint8_t> &raw_packet) const {
	auto lambda = [this](std::vector<uint8_t> &raw_packet) {
		this->iface->send(raw_packet);
	};

	boost::thread sender_thread(lambda, raw_packet);
	sender_thread.detach();
}

void ConnectionManager::receive_method() const {
	// method to add eventual logging
}

void ConnectionManager::receive() const {

}

} /* namespace connectionmanager */

