/*
 * ConnectionManager.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef CONNECTION_CONNECTIONMANAGER_H_
#define CONNECTION_CONNECTIONMANAGER_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <viface/viface.hpp>

#include "handler/Handler.h"

namespace connectionmanager {

/* Class that provides methods to accept and send packets
 * It must create the interface and turn it down
 * `start` method must turn up the interface
 */

class ConnectionManager {

private:
	boost::shared_ptr<handler::Handler> handler;
	boost::shared_ptr< viface::VIface > iface;
	void receive() const;
	void receive_method() const;

public:
	ConnectionManager();
	void setupInterface(std::string interface_name, std::string ip);
	void send(std::vector<uint8_t> &raw_packet) const;
	void setMessageHandler(handler::Handler* handler);
	void start();
	virtual ~ConnectionManager();
};

} /* namespace connectionmanager */

#endif /* CONNECTION_CONNECTIONMANAGER_H_ */
