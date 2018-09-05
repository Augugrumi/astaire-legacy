/*
 * ConnectionManager.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#include "ConnectionManager.h"

namespace connection {

  ConnectionManager::ConnectionManager(const std::string& ifname,
                                       const std::string& ip_address,
                                       int net_mask) : tun(), handler() {

  BOOST_LOG_TRIVIAL(debug) << "Setting up tun connection...";
  tun = boost::shared_ptr<tuntap::tun>(new tuntap::tun());
  BOOST_LOG_TRIVIAL(debug) << "Setting up tuntap with interface name: " << ifname;
  tun->name(ifname);
  tun->ip(ip_address, net_mask);
  BOOST_LOG_TRIVIAL(debug) << "Network ip set successfully to: " << ip_address << "/" << net_mask;

  BOOST_LOG_TRIVIAL(trace) << "mtu: " << tun->mtu();

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
  BOOST_LOG_TRIVIAL(trace) << "Cleaning up resources";
  tun->down();
}

void ConnectionManager::start() {
	BOOST_LOG_TRIVIAL(debug) << "ConnectionManager has started";

  tun->up();

  while(1) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

void ConnectionManager::send(std::vector<uint8_t> &raw_packet) const {

	auto lambda = [this](std::vector<uint8_t> &raw_packet) {
		BOOST_LOG_TRIVIAL(trace) << "Sending packet in a new thread";
		//this->iface->send(raw_packet);
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
	boost::shared_ptr<std::vector<uint8_t>> packet_received = boost::shared_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(packet));
	auto lambda = [this](boost::shared_ptr<std::vector<uint8_t>> packet_received) {
		BOOST_LOG_TRIVIAL(trace) << "Receiving packet in a new thread";
		this->handler->handleMessage(packet_received);
	};

#if BOOST_VERSION >= 106600
	boost::asio::post(*t_pool, boost::bind<void>(lambda, packet_received));
#else
	boost::thread receive_thread(lambda, packet_received);
	receive_thread.detach();
#endif
	return true;
}

} /* namespace connectionmanager */

