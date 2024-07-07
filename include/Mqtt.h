#pragma once

#include <memory>

#include "Callbacks.hpp"

enum class AUTH_METHOD {
	PLAIN,
	PSK,
	CERTS,
};

class MQTTClient {
public:
	explicit MQTTClient(const std::string &,
	                    int port = DEFAULT_PORT,
	                    int keepalive = DEFAULT_KEEP_ALIVE);

	~MQTTClient();

	void set_auth_method(AUTH_METHOD) const;

	[[nodiscard]] bool set_credentials(const std::string &, const std::string &) const;

	[[nodiscard]] bool set_cafile(const std::string &) const;

	[[nodiscard]] bool set_psk(const std::string &, const std::string &) const;

	[[nodiscard]] bool connect() const;

	[[nodiscard]] bool reconnect() const;

	[[nodiscard]] bool publish_message(const std::string &, const std::string &) const;

	[[nodiscard]] bool subscribe(const std::string &) const;

	void resubscribe() const;

	[[nodiscard]] bool unsubscribe(const std::string &) const;

	void setMessageCallback(const cm::MessageCallback &) const;

	void setConnectionCallback(const cm::ConnectionCallback &) const;

private:
	class MQTTClientImpl;
	using MQTTClientImplPtr = std::unique_ptr<MQTTClientImpl>;
	MQTTClientImplPtr impl_;
	static constexpr unsigned int DEFAULT_PORT = 1883;
	static constexpr unsigned int DEFAULT_KEEP_ALIVE = 60;
};
