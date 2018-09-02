/*
 * Handler.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef HANDLER_HANDLER_H_
#define HANDLER_HANDLER_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "utils/JsonUtils.h"

namespace handler {

/*
 * Class that handle message received
 */
class Handler {
protected:
	utils::JsonUtils::JsonWrapper* config;
public:
	Handler(const std::string& config_path);
	virtual void handleMessage(boost::shared_ptr<std::vector<uint8_t>> packet) const =0;
	virtual ~Handler();
};

} /* namespace handler */

#endif /* HANDLER_HANDLER_H_ */
