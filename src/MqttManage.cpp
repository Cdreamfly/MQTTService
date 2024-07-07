#include <iostream>
#include <thread>

#include "MqttService.h"
#include "MqttManage.h"
#include "Public.hpp"
#include "json.hpp"

using json = nlohmann::json;

MqttManage::MqttManage(const std::string &host, const int port) : client_(host, port) {
	client_.setConnectionCallback([&](const int rc) {
		std::cout << "connect rc:" << rc << std::endl;
		if (!client_.subscribe("subscrib_topic")) {
			std::cout << "Error subscribing to topic" << std::endl;
		}
		if (!client_.publish_message("public_topic", "start hello")) {
			std::cout << "Error publishing msg" << std::endl;
		}
	});
	client_.setMessageCallback([&](const std::string &msg) {
		nlohmann::json js;
		auto type = EnMsgType::UNKNOWN_MSG;
		try {
			js = json::parse(msg);
		} catch (const json::exception &e) {
			std::cout << e.what() << std::endl;
		}
		try {
			type = js.at(Agreement::MSG_ID).get<EnMsgType>();
		} catch (const json::exception &e) {
			std::cout << e.what() << std::endl;
		}
		if (!client_.publish_message("public_topic", MqttService::getInstance().getHandler(type)(js))) {
			std::cout << "publish_message err!" << std::endl;
		}
	});
}

void MqttManage::start() const {
	if (!client_.connect()) {
		std::cout << "connect err!" << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
	while (true) {
		if (client_.publish_message("public_topic", "hello")) {
			std::cout << "Error publishing hello" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}
