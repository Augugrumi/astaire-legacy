/*
 * HandlerUtils.cpp
 *
 *  Created on: Sep 2, 2018
 *      Author: zanna
 */

#include "HandlerUtils.h"

namespace utils {

handler::Handler* HandlerUtils::getHandlerByLanguageName(const std::string & language, const std::string & config_file) {
	if (boost::iequals("java", language)) {
		return new handler::JavaHandler(config_file);
	} else {
		BOOST_LOG_TRIVIAL(fatal) << PACKAGE_NAME << " language not recognized. Exiting";
		exit(1);
	}
}

} /* namespace utils */
