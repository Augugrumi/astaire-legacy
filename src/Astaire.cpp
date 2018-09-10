/*
 ============================================================================
 Name        : Astaire.cpp
 Author      : Marco Zanella, Davide Polonio
 Version     :
 Copyright   : GPLv3+
 Description : Hello World in C++,
 ============================================================================
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#if JSONCPP_INCLUDE_PATH_FIX
#include <jsoncpp/json/json.h>
#else
#include <json/json.h>
#endif
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/types.h>

#include "connection/ConnectionManager.h"
#include "utils/JsonUtils.h"
#include "utils/HandlerUtils.h"

#include <csignal>

typedef boost::shared_ptr<connection::ConnectionManager> SmartConn;

void signal_handler(int signal)
{
   switch (signal) {
       case SIGINT:
           exit(1);
           break;
   }
}

int main(int argc, const char* argv[])
{

	signal(SIGINT, signal_handler);

	BOOST_LOG_TRIVIAL(info) << "Spinning up " << PACKAGE_STRING << "...";

#if BOOST_VERSION >= 106600
	BOOST_LOG_TRIVIAL(debug) << "Using a version of boost newer then 1.66!";
#endif

    // Returned is uid_t, that's an unsigned int
    if (getuid() != 0) {
    	BOOST_LOG_TRIVIAL(fatal) << PACKAGE_NAME << " must run as root. Exiting";
    	exit(1);
    }

    std::string path = utils::JsonUtils::DEFAULT_CONFIG_PATH;
    unsigned short int port = 55555; // FIXME parse from command line

    int c ;
	opterr = 0;
	while ((c = getopt(argc, (char **)argv, "c:p:")) != -1) {
		switch(c) {
			case 'c':
				if(optarg) {
					path = optarg;
				}
				break;
			case 'p':
				if(optarg) {
					port = atoi(optarg);
				}
				break;
		}
	}

  const std::string interface_name = std::string("vnf0");
  const std::string ip_address = std::string("198.0.0.1");
  const int subnet_mask = 16;

  SmartConn connection = SmartConn(
                                   new connection::ConnectionManager(interface_name,
                                                                     ip_address,
                                                                     subnet_mask)
                                   );

    handler::Handler* h = utils::HandlerUtils::getHandlerByLanguageName(
    		utils::JsonUtils::JsonWrapper(path).getField(utils::JsonUtils::LAUNGUAGE), path);
	connection->setHandler(h);
	BOOST_LOG_TRIVIAL(debug) << "Bringing interface " << interface_name << " up...";
	connection->start();
	BOOST_LOG_TRIVIAL(debug) << "------------START RECEIVING-------------------";
	connection->listen(port);
	BOOST_LOG_TRIVIAL(debug) << "----------------------------------------------";
	return 0;
}
