/*
 * HandlerUtils.h
 *
 *  Created on: Sep 2, 2018
 *      Author: zanna
 */

#ifndef UTILS_HANDLERUTILS_H_
#define UTILS_HANDLERUTILS_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include "config.h"
#include "handler/Handler.h"
#include "handler/JavaHandler.h"
#include "handler/HelloWorldHandler.h"

namespace utils {

class HandlerUtils {
public:
	static handler::Handler* getHandlerByLanguageName(const std::string & language, const std::string & config_file);
};

} /* namespace utils */

#endif /* UTILS_HANDLERUTILS_H_ */
