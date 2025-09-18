#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <atomic>
#include <vector>
#include <array>
#include <unordered_map>
#include <stdexcept>
#include "../include/ThreadManager.h"
#include "../include/Request.h"
#include "../include/Router.h"
#include "../include/HttpServer.h"





int main() {
    
    HttpServer server{};
    server.start();
}