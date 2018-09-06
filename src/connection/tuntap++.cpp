#include "tuntap++.h"

namespace tuntap {

tun::tun() : _dev { tuntap_init() }, _physical_dev(0) {
	tuntap_start(_dev, TUNTAP_MODE_TUNNEL, TUNTAP_ID_ANY);
}

tun::~tun() {
	tuntap_destroy(_dev);
}

tun::tun(tun &&t) : _dev(nullptr), _physical_dev(0) {
	std::swap(t._dev, this->_dev);
}

void tun::release() {
	tuntap_release(_dev);
}

std::string tun::name() const {
	return tuntap_get_ifname(_dev);
}

void tun::name(std::string const &s) {
	tuntap_set_ifname(_dev, s.c_str());
}

t_tun tun::native_handle() const {
	return TUNTAP_GET_FD(this->_dev);
}

void tun::up() {
	tuntap_up(_dev);
}

void tun::down() {
	tuntap_down(_dev);
}

int tun::mtu() const {
	return tuntap_get_mtu(_dev);
}

void tun::mtu(int m) {
	tuntap_set_mtu(_dev, m);
}

void tun::ip(std::string const &s, int netmask) {
	tuntap_set_ip(_dev, s.c_str(), netmask);
}

void tun::nonblocking(bool b) {
	tuntap_set_nonblocking(_dev, int(b));
}

// TODO move to a better place
//******************************************************
int cread(int fd, char *buf, int n){

  int nread;

  if((nread=read(fd, buf, n)) < 0){
    perror("Reading data");
    exit(1);
  }
  return nread;
}

int read_n(int fd, char *buf, int n) {

  int nread, left = n;

  while(left > 0) {
	BOOST_LOG_TRIVIAL(fatal) << "read_n::nread: " << nread;
    if ((nread = cread(fd, buf, left)) == 0){
      return 0 ;
    }else {
      left -= nread;
      buf += nread;
    }
  }
  return n;
}

int cwrite(int fd, char *buf, int n){

  int nwrite;

  if((nwrite=write(fd, buf, n)) < 0){
	  BOOST_LOG_TRIVIAL(fatal) << "Failure on data writing";
    exit(1);
  }
  return nwrite;
}
//******************************************************

void tun::read_from_socket(unsigned short int port, std::function<void (char*, int)> callback) const {
	BOOST_LOG_TRIVIAL(trace)<< "Initialize reading packages from socket on port: " << port;

	int sock_fd, net_fd, optval = 1; // Socket file descriptor
	struct sockaddr_in physical, remote;
	socklen_t remotelen;

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
		int maxfd = (_dev->tun_fd > net_fd)?_dev-> tun_fd : net_fd;

		BOOST_LOG_TRIVIAL(info) << "maxfd:" << maxfd << " _dev: " << _dev->tun_fd << " net: " << net_fd;

		while(true) {
			int ret;
			fd_set rd_set;
			int nread, nwrite;
			char buffer[mtu()];
			uint16_t plength;

			BOOST_LOG_TRIVIAL(info) << " after initialization";

			FD_ZERO(&rd_set);
			FD_SET(_dev->tun_fd, &rd_set); FD_SET(net_fd, &rd_set);

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

			if(FD_ISSET(_dev->tun_fd, &rd_set)) {
				BOOST_LOG_TRIVIAL(debug) << "Reading data from buffer...";

				nread = cread(_dev->tun_fd, buffer, mtu());

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

				callback(buffer, nread);

				//nwrite = cwrite(net_fd, buffer, nread);
				BOOST_LOG_TRIVIAL(debug) << "4";
			}

		}
	}
}

tap::tap() :
		_dev { tuntap_init() } {
	tuntap_start(_dev, TUNTAP_MODE_ETHERNET, TUNTAP_ID_ANY);
}

tap::~tap() {
	tuntap_destroy(_dev);
}

tap::tap(tap &&t) :
		_dev(nullptr) {
	std::swap(t._dev, this->_dev);
}

void tap::release() {
	tuntap_release(_dev);
}

std::string tap::name() const {
	return tuntap_get_ifname(_dev);
}

void tap::name(std::string const &s) {
	tuntap_set_ifname(_dev, s.c_str());
}

std::string tap::hwaddr() const {
	return tuntap_get_hwaddr(_dev);
}

void tap::hwaddr(std::string const &s) {
	tuntap_set_hwaddr(_dev, s.c_str());
}

t_tun tap::native_handle() const {
	return TUNTAP_GET_FD(this->_dev);
}

void tap::up() {
	tuntap_up(_dev);
}

void tap::down() {
	tuntap_down(_dev);
}

int tap::mtu() const {
	return tuntap_get_mtu(_dev);
}

void tap::mtu(int m) {
	tuntap_set_mtu(_dev, m);
}

void tap::ip(std::string const &s, int netmask) {
	tuntap_set_ip(_dev, s.c_str(), netmask);
}

void tap::nonblocking(bool b) {
	tuntap_set_nonblocking(_dev, int(b));
}

} /* tuntap */
