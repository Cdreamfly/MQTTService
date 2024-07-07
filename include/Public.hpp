#pragma once

enum class EnMsgType {
	UNKNOWN_MSG = 0,      // 未知消息
	LOGIN_MSG,
};

enum class ErrCode {
	SUCCESS = 0,    //正确
	FAILURE = 1,    //失败
};
namespace Agreement {
	constexpr char MSG_ID[] = "msgId";
}