#ifndef WEBSOCKET_SERVICE
#define WEBSOCKET_SERVICE
#include <http_message.h>
#include <gameWorld/world.cpp>

class WebsocketService;

static const std::string HTTP1_1 = "HTTP/1.1";

class WebsocketService {
 public:
    WebsocketService(World& world) : state_{0}, world_(world) {}
    ~WebsocketService() {};
    std::string on_message(const std::string& message,
                           const std::string& ip);

 private:
    HttpResponse on_http_message(const HttpRequest& request,
                                 const std::string& ip);
    std::string on_websocket_message(const std::string& request) {};

    // Read data from databse
    // For read_file
    HttpResponse on_file_url(const std::string& url) const;
    std::string file_content(const std::string& filename) const;

 private:
    // 0 - http
    // 1 - ws handshake
    // 2 - ws
    int state_;

    World& world_;
};



#endif  // HTTP_MESSAGE
