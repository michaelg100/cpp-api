#pragma once

#include <iostream>
#include <string>

#include "SimpleJSON/json.hpp"
#include "mongocxx/instance.hpp"
//#include "mongodb_handler.h"
#include "served/multiplexer.hpp"
#include "served/net/server.hpp"

namespace lft {

constexpr char kFunctionAEndpoint[] = "/function";
constexpr char kIpAddress[] = "0.0.0.0";
constexpr char kPort[] = "5002";
constexpr int kThreads = 10;
class HttpServer {
 public:
  HttpServer(served::multiplexer multiplexer) : multiplexer(multiplexer) {}

  auto functionA() {
    return [&](served::response &response, const served::request &request) {
      json::JSON request_body = json::JSON::Load(request.body());
      std::string text;
      text = request_body["text"].ToString();
      //const Int size = request_body["size"].ToInt();
      bool successful;
      successful ? served::response::stock_reply(200, response)
                 : served::response::stock_reply(400, response);
    };
  }

  
  void InitializeEndpoints() {
    multiplexer.handle(kFunctionAEndpoint).post(functionA());
  }

  void StartServer() {
    served::net::server server(kIpAddress, kPort, multiplexer);
    std::cout << "Starting server to listen on port 5002..." << std::endl;
    server.run(kThreads);
  }

 private:
  served::multiplexer multiplexer;
};

} // namespace lft
