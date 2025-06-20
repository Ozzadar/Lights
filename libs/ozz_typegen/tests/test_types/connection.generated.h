
/**
    * Generated by OZZ Typegen
    * This file is auto-generated, do not edit manually.
    * To regenerate, run the typegen tool with the appropriate definition file.
    * Definition file: 
*/
#pragma once
#include <ozz_binary/serializable.h>
#include <string>
#include <vector>


struct LoginRequest {
    std::string Username;
    std::string Password;
};


enum class ConnectionStatus {
    Connected,
    Disconnected,
    Connecting,
    Error,
};


struct ConnectionState {
    ConnectionStatus Status;
    std::string ErrorMessage;
    std::vector<std::string > Players;
};
