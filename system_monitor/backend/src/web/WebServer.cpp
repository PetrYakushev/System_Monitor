#include "WebServer.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

WebServer::WebServer(int port) : port_(port)
{
    setupRoutes();
}

void WebServer::setupRoutes()
{
    CROW_ROUTE(app_, "/")([]() {
        string path = string(FRONTEND_PATH) + "/index.html";

        ifstream file(path);

        if (!file.is_open()) {
            return crow::response(404, "index.html not found");
        }

        stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html; charset=utf-8");
        return res;
    });

    CROW_ROUTE(app_, "/style.css")([]() {
        string path = std::string(FRONTEND_PATH) + "/style.css";

        ifstream file(path);

        if (!file.is_open()) {
            return crow::response(404, "style.css not found");
        }

        stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/css");
        return res;
    });

    CROW_ROUTE(app_, "/dist/<string>")([](const crow::request&, string filename) {
        string path = string(FRONTEND_PATH) + "/dist/" + filename;

        ifstream file(path, ios::binary);

        if (!file.is_open()) {
            return crow::response(404, "File not found: " + filename);
        }

        stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header(
            "Content-Type",
            "application/javascript"
        );

        return res;
    });

    CROW_ROUTE(app_, "/api/system")([this]() {
        SystemInfo info = monitor_.collect();

        nlohmann::json json = info;

        crow::response res(json.dump(4));
        res.set_header("Content-Type", "application/json");
        return res;
    });

    CROW_ROUTE(app_, "/health")([]() {
        return crow::response("OK");
    });
}

void WebServer::run()
{
    cout << "Server running on http://localhost:" << port_ << endl;

    app_.port(port_).multithreaded().run();
}