#pragma once

#include <functional>
#include <string>

#include "json.hpp"
#include "Public.hpp"

using json = nlohmann::json;

using MsgHandler = std::function<std::string(const json &)>;

class MqttService {
public:
	static MqttService &getInstance();

	MsgHandler getHandler(EnMsgType);

	MqttService(const MqttService &) = delete;

	MqttService &operator=(const MqttService &) = delete;

	~MqttService() = default;

private:
	MqttService();

	std::unordered_map<EnMsgType, MsgHandler> m_fn;
};
