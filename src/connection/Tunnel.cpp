/*
 * Tunnel.cpp
 *
 *  Created on: Sep 6, 2018
 *      Author: zanna
 */

#include "Tunnel.h"

namespace connection {

Tunnel::Tunnel() {
	// TODO Auto-generated constructor stub

}

Tunnel::~Tunnel() {
	// TODO Auto-generated destructor stub
}


int Tunnel::cread(int fd, char *buf, int n) const {

  int nread;

  if((nread=read(fd, buf, n)) < 0){
    perror("Reading data");
    exit(1);
  }
  return nread;
}

int Tunnel::read_n(int fd, char *buf, int n) const {

  int nread, left = n;

  while(left > 0) {
	BOOST_LOG_TRIVIAL(trace) << "read_n::nread: " << nread;
    if ((nread = cread(fd, buf, left)) == 0){
      return 0 ;
    }else {
      left -= nread;
      buf += nread;
    }
  }
  return n;
}

int Tunnel::cwrite(int fd, char *buf, int n) const {

  int nwrite;

  if((nwrite=write(fd, buf, n)) < 0){
	  BOOST_LOG_TRIVIAL(fatal) << "Failure on data writing";
    exit(1);
  }
  return nwrite;
}

void Tunnel::read_from_socket(unsigned short int port, std::function<void (char*, int)> callback) const {
	BOOST_LOG_TRIVIAL(trace)<< "Initialize reading packages from socket on port: " << port;

	int sock_fd, net_fd, optval = 1; // Socket file descriptor
	struct sockaddr_in physical, remote;
	socklen_t remotelen;
	int tun_fd = native_handle();

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Failure while opening the socket";
		exit(1);
	}

	if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Failure while setting socket options...";
		exit(1);
	}

	memset(&physical, 0, sizeof(physical));
	physical.sin_family = AF_INET;
	physical.sin_addr.s_addr = htonl(INADDR_ANY);
	physical.sin_port = htons(port);

	if (bind(sock_fd, (struct sockaddr*) &physical, sizeof(physical)) < 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Failure binding socket with port: " << inet_ntoa(remote.sin_addr);
		exit(1);
	}

	if (listen(sock_fd, 5) < 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Failure listening to binded port";
		exit(1);
	}

	/* wait for connection request */
	remotelen = sizeof(remote);

	while(1) {
		memset(&remote, 0, remotelen);
		if ((net_fd = accept(sock_fd, (struct sockaddr*)&remote, &remotelen)) < 0) {
			BOOST_LOG_TRIVIAL(fatal) << "Failure accepting incoming connection";
			exit(1);
		}

		BOOST_LOG_TRIVIAL(info) << "Connection established with " << inet_ntoa(remote.sin_addr);

		// TODO check if mapping tap_fd(simpletun.c) - _dev->tun_fd is correct
		int maxfd = (tun_fd > net_fd)? tun_fd : net_fd;

		BOOST_LOG_TRIVIAL(trace) << "maxfd:" << maxfd << " _dev: " << tun_fd << " net: " << net_fd;

		while(true) {
			int ret;
			fd_set rd_set;
			int nread, nwrite;
			char buffer[mtu()];
			uint16_t plength;

			BOOST_LOG_TRIVIAL(info) << " after initialization";

			FD_ZERO(&rd_set);
			FD_SET(tun_fd, &rd_set); FD_SET(net_fd, &rd_set);

			BOOST_LOG_TRIVIAL(info) << " after FD_SET";

			ret = select(maxfd + 1, &rd_set, NULL, NULL, NULL);

			BOOST_LOG_TRIVIAL(info) << " select";

			if (ret < 0 && errno == EINTR){
			  continue;
			}

			if (ret < 0) {
				BOOST_LOG_TRIVIAL(fatal) << "Failure on select.";
				exit(1);
			}

			if(FD_ISSET(tun_fd, &rd_set)) {
				BOOST_LOG_TRIVIAL(debug) << "Reading data from buffer...";

				nread = cread(tun_fd, buffer, mtu());

				// write length + packet
				plength = htons(nread);
				nwrite = cwrite(net_fd, (char *)&plength, sizeof(plength));
				nwrite = cwrite(net_fd, buffer, nread);

			}

			if(FD_ISSET(net_fd, &rd_set)) {
				BOOST_LOG_TRIVIAL(debug) << "Reading data from network...";

				nread = read_n(net_fd, (char *)&plength, sizeof(plength));

				BOOST_LOG_TRIVIAL(debug) << "nread:" << nread;

				if(nread == 0){
					BOOST_LOG_TRIVIAL(debug) << "No data from network.";
					break;
				}

				nread = recv(net_fd, buffer, mtu(), 0);
				BOOST_LOG_TRIVIAL(debug) << "data:" << buffer;

				BOOST_LOG_TRIVIAL(debug) << "nread:" << nread << " sizeof: " << sizeof(buffer)/sizeof(*buffer);

				callback(buffer, nread+2);

				nwrite = cwrite(net_fd, buffer, nread);
				BOOST_LOG_TRIVIAL(debug) << "4";
			}

		}
	}
}

} /* namespace connection */
