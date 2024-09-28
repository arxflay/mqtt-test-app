//#include "z2m_testapp.h"
//#include <vector>
//#include <string>
#include <mqtt/client.h>
#include <mqtt/callback.h>
#include <mqtt/connect_options.h>
#include <chrono>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <fstream>
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
        cfgJson["username"] = "gardenbro";
        cfgJson["password"] = "paladin";
        cfgFile << cfgJson;
        cfgFile.close();
    }
    else
        cfgJson = nlohmann::json::parse(cfgFile);

    cfgFile.close();
    Config cfg = cfgJson.get<Config>();
    mqtt::client cli(cfg.serverAddress, "69");

    auto connOpts = mqtt::connect_options_builder()
        .keep_alive_interval(std::chrono::seconds(20))
        .clean_session()
        .v3()
        .user_name(cfg.username)
        .password(cfg.password)
        .finalize();

    try {
        cli.connect(connOpts);

        // First use a message pointer.
        cli.subscribe("TestTopic");
        while(true)
        {
            std::cout << cli.consume_message()->get_payload() << std::endl;
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
