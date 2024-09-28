cmake_minimum_required(VERSION 3.15)
project(z2m_testapp CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(z2m_testapp src/main.cpp)
find_package(PahoMqttCpp)
find_package(nlohmann_json)
target_link_libraries(${PROJECT_NAME} PahoMqttCpp::paho-mqttpp3-static nlohmann_json::nlohmann_json)

install(TARGETS z2m_testapp DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
