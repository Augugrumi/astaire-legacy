/*
 * Handler.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef HANDLER_HANDLER_H_
#define HANDLER_HANDLER_H_

#include <vector>
#include <boost/shared_ptr.hpp>

namespace handler {

/*
 * Class that handle message received
 */
class Handler {
public:
	Handler();
	virtual ~Handler();
	void handleMessage(boost::shared_ptr<std::vector<uint8_t>> packet) const;
};

} /* namespace handler */

#endif /* HANDLER_HANDLER_H_ */
