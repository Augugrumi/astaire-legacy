/*
 * ConnectionManager.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef CONNECTION_CONNECTIONMANAGER_H_
#define CONNECTION_CONNECTIONMANAGER_H_

#include <vector>
#include <stdlib.h>
#include <string>

namespace connectionmanager {

/* Class that provides methods to accept and send packets
 * It must creating the interface and turning it down
 * `start` method must turn up the interface
 */

class ConnectionManager {
public:
	ConnectionManager();
	void setupInterface(string, string);
	void send(vector<uint8_t>);
	void setMessageHandler();
	void start();
	virtual ~ConnectionManager();

private:
	void receive();
};

} /* namespace connectionmanager */

#endif /* CONNECTION_CONNECTIONMANAGER_H_ */
