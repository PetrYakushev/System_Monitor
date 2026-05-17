#pragma once

#include "crow.h"
#include <memory>
#include "../include/dto/SystemInfo.hpp"
#include "../monitors/Monitor.hpp"

class WebServer {
public:
    explicit WebServer(int port);

    void run();

private:
    int port_;
    crow::SimpleApp app_;

    Monitor monitor_;

    void setupRoutes();
};