/*
 * HelloWorldHandler.h
 *
 *  Created on: Sep 4, 2018
 *      Author: zanna
 */

#ifndef SRC_HANDLER_HELLOWORLDHANDLER_H_
#define SRC_HANDLER_HELLOWORLDHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/log/trivial.hpp>
#include "handler/Handler.h"
#include "utils/JsonUtils.h"

namespace handler {

class HelloWorldHandler : public Handler {
public:
	HelloWorldHandler();
	virtual void handleMessage(boost::shared_ptr<std::vector<uint8_t>> packet) const;
	virtual ~HelloWorldHandler();
};

} /* namespace handler */

#endif /* SRC_HANDLER_HELLOWORLDHANDLER_H_ */
