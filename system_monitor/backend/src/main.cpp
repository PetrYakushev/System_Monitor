#include <iostream>
#include "web/WebServer.hpp"

int main() {
    try {
        WebServer server(8080);
        server.run();
    } 
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}