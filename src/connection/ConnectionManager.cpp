/*
 * ConnectionManager.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#include "ConnectionManager.h"

namespace connection {


ConnectionManager::ConnectionManager(
		const std::string &interface_name,
		const std::string &ip
		) : iface(), handler() {

	BOOST_LOG_TRIVIAL(debug) << "Setting up interface " << interface_name <<", with IP: " << ip;
	iface = boost::shared_ptr<viface::VIface>(new viface::VIface(interface_name));
	iface->setIPv4(ip);

	BOOST_LOG_TRIVIAL(debug) << "Setting up message handler";

#if BOOST_VERSION >= 106600
	BOOST_LOG_TRIVIAL(debug) << "Loading thread pool";
	this->t_pool = boost::shared_ptr<boost::asio::thread_pool>(
			new boost::asio::thread_pool(std::thread::hardware_concurrency()
	));
#endif
}

void ConnectionManager::setHandler(handler::Handler* handler) {
	this->handler = boost::shared_ptr<handler::Handler>(handler);
}

ConnectionManager::~ConnectionManager() {
	// None to delete at the moment since all the heap is managed with
	// shared_ptrs
}

void ConnectionManager::start() {
	BOOST_LOG_TRIVIAL(debug) << "ConnectionManager has started";
	if (iface && !iface->isUp()) {
		iface->up();
	}

	std::set<viface::VIface*> ifaces = {&(*iface)};

	while(true) {
		std::vector<uint8_t> result = this->iface->receive();
		//BOOST_LOG_TRIVIAL(trace) << "old packet size: " << result.size();
		if (result.size() > 0)
		for (std::vector<uint8_t>::const_iterator it = result.cbegin(); it!=result.cend(); it++) {
			BOOST_LOG_TRIVIAL(trace) << *it << " old packet";
		}
	}

	viface::dispatch(ifaces, std::bind<bool>(
				&ConnectionManager::receive,
				this,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3));

}

void ConnectionManager::send(std::vector<uint8_t> &raw_packet) const {

	auto lambda = [this](std::vector<uint8_t> &raw_packet) {
		BOOST_LOG_TRIVIAL(trace) << "Sending packet in a new thread";
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

bool ConnectionManager::receive(std::string const& name, uint id, std::vector<uint8_t>& packet) const {
	BOOST_LOG_TRIVIAL(trace) << "Receiving raw packet...";
	std::vector<uint8_t> result = this->iface->receive();
	BOOST_LOG_TRIVIAL(trace) << "old packet size: " << result.size();
	for (std::vector<uint8_t>::const_iterator it = result.cbegin(); it!=result.cend(); it++) {
		BOOST_LOG_TRIVIAL(trace) << *it << " old packet";
	}
	boost::shared_ptr<std::vector<uint8_t>> packet_received = boost::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(packet));
	this->handler->handleMessage(packet_received);
	/*boost::shared_ptr<std::vector<uint8_t>> packet_received = boost::shared_ptr<std::vector<uint8_t>>(&result);
	auto lambda = [this](boost::shared_ptr<std::vector<uint8_t>> packet_received) {
		BOOST_LOG_TRIVIAL(trace) << "Receiving packet in a new thread";
		this->handler->handleMessage(packet_received);
	};

#if BOOST_VERSION >= 106600
	boost::asio::post(*t_pool, boost::bind<void>(lambda, packet_received));
#else
	boost::thread receive_thread(lambda, packet_received);
	receive_thread.detach();
#endif*/
	return true;
}

} /* namespace connectionmanager */

