/*
 * ConnectionManager.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#include "ConnectionManager.h"

namespace connectionmanager {

// TODO think better initializer
ConnectionManager::ConnectionManager() : iface(), handler() {
#if BOOST_VERSION >= 106600
	this->t_pool = boost::shared_ptr<boost::asio::thread_pool>(
			new boost::asio::thread_pool(std::thread::hardware_concurrency()
	));
#endif
}

ConnectionManager::~ConnectionManager() {
	// TODO Auto-generated destructor stub
}

// TODO Better move to constructor?
void ConnectionManager::setupInterface(std::string interface_name, std::string ip) {
	BOOST_LOG_TRIVIAL(debug) << "Setting up interface " << interface_name ;
	iface = boost::shared_ptr<viface::VIface>(new viface::VIface(interface_name));
	iface->setIPv4(ip);
}

void ConnectionManager::start() {
	BOOST_LOG_TRIVIAL(debug) << "ConnectionManager has started";
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

#if BOOST_VERSION >= 106600
	boost::asio::post(*t_pool, boost::bind<void>(lambda, raw_packet));

#else
	BOOST_LOG_TRIVIAL(trace) << "Sending raw packet...";
	boost::thread sender_thread(lambda, raw_packet);
	sender_thread.detach();
#endif
}

void ConnectionManager::receive() const {
	BOOST_LOG_TRIVIAL(trace) << "Receiving raw packet...";
	std::vector<uint8_t> result = this->iface->receive();
	boost::shared_ptr<std::vector<uint8_t>> packet_received = boost::shared_ptr<std::vector<uint8_t>>(&result);

	auto lambda = [this](boost::shared_ptr<std::vector<uint8_t>> packet_received) {
		this->handler->handleMessage(packet_received);
	};

	boost::thread receive_thread(lambda, packet_received);
	receive_thread.detach();
}

} /* namespace connectionmanager */

