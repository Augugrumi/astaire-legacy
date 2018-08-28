/*
 * Handler.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zanna
 */

#ifndef HANDLER_HANDLER_H_
#define HANDLER_HANDLER_H_

namespace connectionmanager {

/*
 * Class that handle message received
 */
class Handler {
public:
	Handler();
	virtual ~Handler();
};

} /* namespace connectionmanager */

#endif /* HANDLER_HANDLER_H_ */
