cmake_minimum_required(VERSION 3.15)
project(mqtt_server CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(mqtt_server src/main.cpp)
find_package(PahoMqttCpp)
find_package(nlohmann_json)
target_link_libraries(${PROJECT_NAME} PahoMqttCpp::paho-mqttpp3-static nlohmann_json::nlohmann_json)

install(TARGETS mqtt_server DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
