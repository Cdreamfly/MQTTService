#include "mosquittopp.h"
#include "Mqtt.h"

#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

class MQTTClient::MQTTClientImpl final : public mosqpp::mosquittopp {
public:
	MQTTClientImpl(std::string host,
	               const int port,
	               const int keepalive): host_(std::move(host)),
	                                     port_(port),
	                                     keepalive_(keepalive),
	                                     auth_method_(AUTH_METHOD::PLAIN) {
		mosqpp::lib_init();
	}

	~MQTTClientImpl() override {
		std::cout << "~MQTTClientImpl" << std::endl;
		loop_stop(true);
		mosqpp::lib_cleanup();
	}

	void set_auth_method(const AUTH_METHOD method) {
		auth_method_ = method;
	}

	bool set_credentials(const std::string &user, const std::string &password) {
		return username_pw_set(user.c_str(), password.c_str()) == MOSQ_ERR_SUCCESS;
	}

	bool set_cafile(const std::string &cafile_path) {
		if (auth_method_ != AUTH_METHOD::CERTS) {
			throw std::runtime_error("No method allowed for selected authentication method");
		}
		return tls_set(cafile_path.c_str()) == MOSQ_ERR_SUCCESS;
	}

	bool set_psk(const std::string &identity, const std::string &psk) {
		if (auth_method_ != AUTH_METHOD::PSK) {
			throw std::runtime_error("No method allowed for selected authentication method");
		}
		return tls_psk_set(psk.c_str(), identity.c_str()) == MOSQ_ERR_SUCCESS;
	}

	bool connect() {
		if (connect_async(host_.c_str(), port_, keepalive_) == MOSQ_ERR_SUCCESS) {
			loop_start();
			return true;
		}
		return false;
	}

	bool re_connect() {
		return reconnect_async() == MOSQ_ERR_SUCCESS;
	}

	bool publish_message(const std::string &topic, const std::string &msg) {
		return publish(nullptr, topic.c_str(), static_cast<int>(msg.length()), msg.c_str(), 1, false) == MOSQ_ERR_SUCCESS;
	}

	bool subscribe_to_topic(const std::string &topic) {
		if (subscribe(nullptr, topic.c_str()) == MOSQ_ERR_SUCCESS) {
			topics_.push_back(topic);
			return true;
		}
		return false;
	}

	void resubscribe() {
		for (const std::string &topic: topics_) {
			if (subscribe(nullptr, topic.c_str()) != MOSQ_ERR_SUCCESS) {
				std::cout << "MQTTClientImpl - on_subscribe(" << topic << ")" << std::endl;
			}
		}
	}

	bool unsubscribe_from_topic(const std::string &topic) {
	const auto &it = std::find(topics_.begin(), topics_.end(), topic);
		if (it != topics_.end()) {
			topics_.erase(it);
		}
		return unsubscribe(nullptr, topic.c_str()) == MOSQ_ERR_SUCCESS;
	}

	void setMessageCallback(const cm::MessageCallback &cb) { messageCallback_ = cb; }

	void setConnectionCallback(const cm::ConnectionCallback &cb) { connectionCallback_ = cb; }

private:
	void on_connect(const int rc) override {
		std::cout << "MQTTClientImpl - on_connect (" << rc << ")" << std::endl;
		connectionCallback_(rc);
	}

	void on_disconnect(const int rc) override {
		std::cout << "MQTTClientImpl - on_disconnect (" << rc << ")" << std::endl;
	}

	void on_publish(const int mid) override {
		std::cout << "MQTTClientImpl - on_publish (" << mid << ")" << std::endl;
	}

	void on_message(const mosquitto_message *message) override {
		std::cout << "MQTTClientImpl - on_message(" << message->mid
				<< " topic: " << std::string(message->topic) << " - message: "
				<< std::string(static_cast<char *>(message->payload), message->payloadlen) << ")" << std::endl;
		messageCallback_(static_cast<char *>(message->payload));
	}

	void on_subscribe(const int mid, int qos_count, const int *granted_qos) override {
		std::cout << "MQTTClientImpl - on_subscribe(" << mid << ")" << std::endl;
	}

	void on_unsubscribe(const int mid) override {
		std::cout << "MQTTClientImpl - on_unsubscribe(" << mid << ")" << std::endl;
	}

	void on_log(const int level, const char *str) override {
		std::cout << "MQTTClientImpl - on_log (" << level << " " << std::string(str) << ")" << std::endl;
	}

	void on_error() override {
		std::cout << "MQTTClientImpl - on_error ()" << std::endl;
	}

	cm::MessageCallback messageCallback_;
	cm::ConnectionCallback connectionCallback_;

	const std::string host_;
	int port_;
	int keepalive_;
	AUTH_METHOD auth_method_;
	std::vector<std::string> topics_;
};

MQTTClient::MQTTClient(const std::string &host,
                       int port,
                       int keepalive) : impl_(cm::make_unique<MQTTClientImpl>(host, port, keepalive)) {
}

MQTTClient::~MQTTClient() = default;

void MQTTClient::set_auth_method(const AUTH_METHOD method) const {
	impl_->set_auth_method(method);
}

bool MQTTClient::set_credentials(const std::string &user, const std::string &password) const {
	return impl_->set_credentials(user, password);
}

bool MQTTClient::set_psk(const std::string &identity, const std::string &psk) const {
	return impl_->set_psk(identity, psk);
}

bool MQTTClient::set_cafile(const std::string &cafile_path) const {
	return impl_->set_cafile(cafile_path);
}

bool MQTTClient::connect() const {
	return impl_->connect();
}

bool MQTTClient::reconnect() const {
	return impl_->re_connect();
}

bool MQTTClient::publish_message(const std::string &topic, const std::string &msg) const {
	return impl_->publish_message(topic, msg);
}

bool MQTTClient::subscribe(const std::string &topic) const {
	return impl_->subscribe_to_topic(topic);
}

void MQTTClient::resubscribe() const {
	impl_->resubscribe();
}

bool MQTTClient::unsubscribe(const std::string &topic) const {
	return impl_->unsubscribe_from_topic(topic);
}

void MQTTClient::setMessageCallback(const cm::MessageCallback &cb) const {
	impl_->setMessageCallback(cb);
}

void MQTTClient::setConnectionCallback(const cm::ConnectionCallback &cb) const {
	impl_->setConnectionCallback(cb);
}
