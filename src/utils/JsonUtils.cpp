/*
 * JsonUtils.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: zanna
 */

#include "JsonUtils.h"

namespace utils {

const std::string JsonUtils::DEFAULT_CONFIG_PATH="./conf.json";
const std::string JsonUtils::LAUNGUAGE="language";
const std::string JsonUtils::FILE_PATH="file_path";
const std::string JsonUtils::CLASS_NAME="class_name";
const std::string JsonUtils::METHOD="method_name";

JsonUtils::JsonWrapper::JsonWrapper(const std::string& path) {
	std::ifstream ifs(const_cast<char *>(path.c_str()));
	Json::Reader reader;
	reader.parse(ifs, this->obj);
}

std::string JsonUtils::JsonWrapper::getField(const std::string& field_name) const {
	return obj[field_name].asString();
}


} /* namespace connectionmanager */
