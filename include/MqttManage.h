#pragma once

#include <Mqtt.h>
#include <string>

class MqttManage {
public:
	MqttManage(const std::string &, int);

	void start() const;

private:
	MQTTClient client_;
};
