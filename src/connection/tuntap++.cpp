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

void tun::read_from_socket(unsigned short int port) const {
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
	memset(&remote, 0, remotelen);
	if ((net_fd = accept(sock_fd, (struct sockaddr*)&remote, &remotelen)) < 0) {
		BOOST_LOG_TRIVIAL(fatal) << "Failure accepting incoming connection";
		exit(1);
	}

	BOOST_LOG_TRIVIAL(info) << "Connection established with " << inet_ntoa(remote.sin_addr);

	while(true) {

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
