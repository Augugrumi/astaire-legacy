#pragma once
#ifndef LIBTUNTAP_ALY0MA60
#define LIBTUNTAP_ALY0MA60

#include <string>
#include <tuntap.h>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <boost/log/trivial.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <functional>
#include <errno.h>
#include <unistd.h>

namespace tuntap {

class tun
{
 public:
  tun();
  ~tun();
  tun(tun const &) = delete;
  tun & operator = (tun const &) = delete;
  tun(tun &&);

  // Properties
  std::string name() const;
  void name(std::string const &);
  int mtu() const ;
  void mtu(int);
  t_tun native_handle() const;

  // Network
  void up();
  void down();
  void ip(std::string const &presentation, int netmask);

  // System
  void release();
  void nonblocking(bool);

  // I/O
  void read_from_socket(unsigned short int, std::function<void(char*, int)>) const;

 private:
  struct device* _dev; // vnf interface
  struct device* _physical_dev;
};

class tap
{
 public:
  tap();
  ~tap();
  tap(tap const &) = delete;
  tap & operator = (tap const &) = delete;
  tap(tap &&);

  // Properties
  std::string name() const;
  void name(std::string const &);
  std::string hwaddr() const;
  void hwaddr(std::string const &);
  int mtu() const;
  void mtu(int);
  t_tun native_handle() const;

  // Network
  void up();
  void down();
  void ip(std::string const &presentation, int netmask);

  // System
  void release();
  void nonblocking(bool);
 private:
  struct device* _dev;
};

} /* tuntap */


#endif /* end of include guard: LIBTUNTAP_ALY0MA60 */
