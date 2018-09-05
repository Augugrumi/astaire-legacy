/*
 * HelloWorldHandler.cpp
 *
 *  Created on: Sep 4, 2018
 *      Author: zanna
 */

#include "HelloWorldHandler.h"

namespace handler {

HelloWorldHandler::HelloWorldHandler() : handler::Handler(utils::JsonUtils::DEFAULT_CONFIG_PATH) {}

void HelloWorldHandler::handleMessage(boost::shared_ptr<std::vector<uint8_t>> packet) const {
	BOOST_LOG_TRIVIAL(debug) << "HelloWorldHandler has started";
	BOOST_LOG_TRIVIAL(debug) << packet;
	BOOST_LOG_TRIVIAL(debug) << packet->size();

	for (std::vector<uint8_t>::const_iterator it = packet->cbegin(); it!=packet->cend(); it++) {
		BOOST_LOG_TRIVIAL(trace) << *it;
	}
	BOOST_LOG_TRIVIAL(debug) << "fine";
}

HelloWorldHandler::~HelloWorldHandler() {}

} /* namespace handler */
