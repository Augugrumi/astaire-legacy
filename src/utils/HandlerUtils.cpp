/*
 * HandlerUtils.cpp
 *
 *  Created on: Sep 2, 2018
 *      Author: zanna
 */

#include "HandlerUtils.h"

namespace utils {

handler::Handler* HandlerUtils::getHandlerByLanguageName(const std::string & language, const std::string & config_file) {
#if ENABLE_JAVA
	if (boost::iequals("java", language)) {
		return new handler::JavaHandler(config_file);
	}
#endif
	//BOOST_LOG_TRIVIAL(fatal) << PACKAGE_NAME << " doesn't have " << language << " bindings yet. Exiting";
	//exit(1);
  BOOST_LOG_TRIVIAL(debug) << "Returning dummy handler";
  return new handler::HelloWorldHandler();
}

} /* namespace utils */
