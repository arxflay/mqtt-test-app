#include <mqtt/client.h>
#include <mqtt/callback.h>
#include <mqtt/connect_options.h>
#include <chrono>
#include <mqtt/message.h>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unistd.h>

struct Config
{
    std::string serverAddress;
    std::string username;
    std::string password;
};

void from_json(const nlohmann::json &jsonObject, Config &config) 
{
    config.serverAddress = jsonObject.at("serverAddress").get<std::string>();
    config.username = jsonObject.at("username").get<std::string>();
    config.password = jsonObject.at("password").get<std::string>();
}

int main() {
    char exePath[512]{};
    auto x = readlink("/proc/self/exe", exePath, sizeof(exePath));
    std::ifstream cfgFile(std::string(exePath) + "/config.json");
    nlohmann::json cfgJson;
    if (!cfgFile.is_open())
    {
        std::ofstream cfgFile("config.json");
        cfgJson["serverAddress"] = "mqtt://172.17.0.1";
        cfgJson["username"] = "gardengal";
        cfgJson["password"] = "paladin";
        cfgFile << cfgJson;
        cfgFile.close();
    }
    else
        cfgJson = nlohmann::json::parse(cfgFile);

    cfgFile.close();
    Config cfg = cfgJson.get<Config>();

    mqtt::client cli(cfg.serverAddress, "70");

    auto connOpts = mqtt::connect_options_builder()
        .keep_alive_interval(std::chrono::seconds(40))
        .clean_session()
        .v3()
        .user_name(cfg.username)
        .password(cfg.password)
        .finalize();

    try {
        cli.connect(connOpts);
        int messageId = 0;
        {
            mqtt::message_ptr pubmsg = mqtt::make_message("TestTopic", R"(
{
    "messageId":)" + std::to_string(messageId++) + R"(
})"
            );
            std::cout << pubmsg->get_payload().c_str() << std::endl;
            pubmsg->set_qos(2);
            pubmsg->set_retained(true);
            cli.publish(pubmsg);
        }
        while(true)
        {
            mqtt::message_ptr pubmsg = mqtt::make_message("TestTopic", R"(
{
    "messageId":)" + std::to_string(messageId++) + R"(
})"
            );
            std::cout << pubmsg->get_payload().c_str() << std::endl;
            pubmsg->set_qos(2);
            cli.publish(pubmsg);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        // Disconnect
        cli.disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
            << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }

    return 0;
}