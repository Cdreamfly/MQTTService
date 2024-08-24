#include "MqttService.h"
#include "service/ServiceTemp.hpp"
#include "service/Login.h"

MqttService &MqttService::getInstance() {
	static MqttService service;
	return service;
}

MqttService::MqttService() {
	m_fn.insert({
		EnMsgType::LOGIN_MSG, [&](const json &js)-> std::string {
			cm::service::CReQuest<cm::service::Login, cm::service::LoginResult> res = js;
			return res.do_fail_success();
		}
	});
}


MsgHandler MqttService::getHandler(const EnMsgType msgId) {
	if (m_fn.find(msgId) != m_fn.end()) {
		return m_fn[msgId];
	}
	return [=](const json &js) -> std::string {
		return js.dump();
	};
}
