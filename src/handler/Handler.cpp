/*
 * Handler.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: zanna
 */

#include "Handler.h"

namespace handler {

Handler::Handler(const std::string& config_path) : config(new utils::JsonUtils::JsonWrapper(config_path)) {}

Handler::~Handler() {
	delete this->config;
}

}
