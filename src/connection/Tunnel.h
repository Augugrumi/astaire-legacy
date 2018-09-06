/*
 * Tunnel.h
 *
 *  Created on: Sep 6, 2018
 *      Author: zanna
 */

#ifndef SRC_CONNECTION_TUNNEL_H_
#define SRC_CONNECTION_TUNNEL_H_

#include <tuntap++.hh>
#include <boost/log/trivial.hpp>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <functional>
#include <stdio.h>
#include <cstring>

namespace connection {

class Tunnel : public tuntap::tun {
private:
	int cread(int fd, char *buf, int n) const;
	int cwrite(int fd, char *buf, int n) const;
	int read_n(int fd, char *buf, int n) const;

public:
	Tunnel();
	virtual ~Tunnel();
	void read_from_socket(unsigned short int port, std::function<void (char*, int)> callback) const;
};

} /* namespace connection */

#endif /* SRC_CONNECTION_TUNNEL_H_ */
