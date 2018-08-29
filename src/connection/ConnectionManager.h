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
#include <viface/viface.hpp>

#include "handler/Handler.h"

namespace connectionmanager {

/* Class that provides methods to accept and send packets
 * It must create the interface and turn it down
 * `start` method must turn up the interface
 */

class ConnectionManager {

private:
	Handler handler;
	viface::VIface iface;
	void receive();

public:
	ConnectionManager();
	void setupInterface(std::string interface_name, std::string ip);
	void send(std::vector<uint8_t> raw_packet);
	void setMessageHandler(Handler& handler);
	void start();
	virtual ~ConnectionManager();
};

} /* namespace connectionmanager */

#endif /* CONNECTION_CONNECTIONMANAGER_H_ */
