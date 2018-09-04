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
#include <viface/viface.hpp>
#include <tins/tins.h>
#include <boost/log/trivial.hpp>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/types.h>

#include "connection/ConnectionManager.h"
#include "utils/JsonUtils.h"
#include "utils/HandlerUtils.h"

typedef boost::shared_ptr<connection::ConnectionManager> SmartConn;

int main(int argc, const char* argv[])
{

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

    int c ;
	opterr = 0;
	while ((c = getopt(argc, (char **)argv, "c:")) != -1) {
		switch(c) {
			case 'c':
				if(optarg) {
					path = optarg;
				}
				break;
		}
	}

    SmartConn connection = SmartConn(
    		new connection::ConnectionManager("vnf0", "127.0.0.1")
    );

    handler::Handler* h = utils::HandlerUtils::getHandlerByLanguageName(
    		utils::JsonUtils::JsonWrapper(path).getField(utils::JsonUtils::LAUNGUAGE), path);
	connection->setHandler(h);
	connection->start();

    return 0;
}
