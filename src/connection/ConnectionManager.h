/*
 * ConnectionManager.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */
#pragma once
#ifndef CONNECTION_CONNECTIONMANAGER_H_
#define CONNECTION_CONNECTIONMANAGER_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <thread>
#include <chrono>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#if BOOST_VERSION > 106600
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#endif

#include "handler/Handler.h"
#include "tuntap++.h"

namespace connection {

/* Class that provides methods to accept and send packets
 * It must create the interface and turn it down
 * `start` method must turn up the interface
 */

class ConnectionManager {

private:
	boost::shared_ptr<handler::Handler> handler;
	boost::shared_ptr<tuntap::tun> tun;
	void receive(char* p, int size) const;
#if BOOST_VERSION >= 106600
	boost::shared_ptr<boost::asio::thread_pool> t_pool;
#endif

public:
	ConnectionManager(const std::string&, const std::string&, int);
    void setHandler(handler::Handler* handler);
	void send(std::vector<uint8_t> &) const;
    bool listen(unsigned short int&) const;
	void start();
	virtual ~ConnectionManager();
};

} /* namespace connectionmanager */

#endif /* CONNECTION_CONNECTIONMANAGER_H_ */
