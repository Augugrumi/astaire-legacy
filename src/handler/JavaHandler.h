/*
 * JavaHandler.h
 *
 *  Created on: Aug 31, 2018
 *      Author: zanna
 */

#ifndef HANDLER_JAVAHANDLER_H_
#define HANDLER_JAVAHANDLER_H_

#include "Handler.h"
#include <jni.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>

namespace handler {

class JavaHandler : public Handler {
private:
	uint8_t* execute_java(const std::string& class_file_path, const std::string& class_name,
			const std::string& method_name, uint8_t pkt [], int pkt_size);
public:
	void handleMessage(boost::shared_ptr<std::vector<uint8_t>> packet) const;
	virtual ~JavaHandler();
};

} /* namespace connectionmanager */

#endif /* HANDLER_JAVAHANDLER_H_ */
