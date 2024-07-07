#include <iostream>
#include <MqttManage.h>
#include <thread>
#include "Mqtt.h"

void test_mqtt(const std::string &host, const int port)
{
    const MQTTClient mqtt_client(host, port);

    if (!mqtt_client.set_credentials("admin", "admin"))
        std::cout << "Error setting credentials" << std::endl;

    if (!mqtt_client.connect())
        std::cout << "Error connecting" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.subscribe("IOT/intranet/client/request/test/SC2212233160026"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.publish_message("IOT/intranet/server/report/test/SC2212233160026", "Hola mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1000));

    if (!mqtt_client.unsubscribe("topic"))
        std::cout << "Error unsubscribing from topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Adios mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test_mqtt_certs(const std::string &host, const int port)
{
    const MQTTClient mqtt_client(host, port);

    mqtt_client.set_auth_method(AUTH_METHOD::CERTS);

    if (!mqtt_client.set_credentials("usuario", "usuario"))
        std::cout << "Error setting credentials" << std::endl;

    if (!mqtt_client.set_cafile("../my_root_ca.pem"))
        std::cout << "Error setting CA key" << std::endl;

    if (!mqtt_client.connect())
        std::cout << "Error connecting" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.subscribe("IOT/intranet/client/request/test/SC2212233160026"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.subscribe("topic"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.publish_message("IOT/intranet/server/report/test/SC2212233160026", "Hola mundo"))
        std::cout << "Error publishing msg" << std::endl;

    /*while(1)
    {
        if (!mqtt_client.publish_message("IOT/intranet/server/report/test/SC2212233160026", "Hola mundo"))
            std::cout << "Error publishing msg" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }*/
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (!mqtt_client.unsubscribe("topic"))
        std::cout << "Error unsubscribing from topic" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (!mqtt_client.reconnect())
        std::cout << "Error unsubscribing from topic" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (!mqtt_client.publish_message("topic", "Adios mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test_mqtt_psk(const std::string &host, const int port)
{
    const MQTTClient mqtt_client(host, port);

    mqtt_client.set_auth_method(AUTH_METHOD::PSK);

    if (!mqtt_client.set_psk("usuario", "70617373776f726473757065726368756e6761"))
        std::cout << "Error setting PSK key" << std::endl;

    if (!mqtt_client.connect())
        std::cout << "Error connecting" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.subscribe("topic"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Hola mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.unsubscribe("topic"))
        std::cout << "Error unsubscribing from topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Adios mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void mqtt() {
    const MqttManage mqtt_manage("127.0.0.1", 1883);
    mqtt_manage.start();
}

int main(int argc, char **argv)
{
    mqtt();
    std::cout << "**** Attemping to test MQTT stuff... ****\n\n";

    //test_mqtt_psk("localhost", 8883);
    //test_mqtt_certs("192.168.20.104", 8883);
    //test_mqtt("192.168.20.104", 1883);
    std::cout << "Test finished" << std::endl;

    return 0;
}