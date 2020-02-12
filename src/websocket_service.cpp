#include <websocket_service.h>

#include <fstream>
#include <streambuf>
#include <iostream>
#include <json/json.hpp>


std::string WebsocketService::on_message(const std::string& message,
                                         const std::string& ip) {
    if (state_ == 0) {
        HttpRequest req(message);
        HttpResponse res = on_http_message(req, ip);
        return res.generate_message();
    } 
}

HttpResponse WebsocketService::on_http_message(const HttpRequest& req, const std::string& ip) {
    world_.add_player(ip);
    std::string url = req.url();
    if (req.method() == "GET") {
        if (url == "/" || url == "/gameView.js" || url == "/inputController.js") {
            return on_file_url(url);
        } else if (url == "/data") {
            std::string body = world_.get_game_state();
            return HttpResponse(HTTP1_1, "200", "OK", "", body);
        } else {
            return HttpResponse(HTTP1_1, "404", "Not Found", "", "");
        }
    } else if (req.method() == "POST") {
        if (url == "/data") {
            world_.parse_user_control(req.body(), ip);
            return HttpResponse(HTTP1_1, "200", "OK", "", "Move");
        }
    } 

    return HttpResponse(HTTP1_1, "404", "Not Found", "", "");
}


HttpResponse WebsocketService::on_file_url(const std::string& url) const {
    std::string filename;
    filename = url.substr(1);
    if (filename.empty()) {
        filename = "index.html";
    }
    HttpResponse res(HTTP1_1, "200", "OK", "", file_content(filename));
    res.save_header("Content-Length", std::to_string(res.body().length()));
    return res;
}


std::string WebsocketService::file_content(const std::string& filename) const {
    std::ifstream ifs("../../static/" + filename);
    return std::string((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
}


// int main() {
//     std::ifstream ifs("index.html");
//     std::cout << std::string((std::istreambuf_iterator<char>(ifs)),
//                         std::istreambuf_iterator<char>()) << std::endl;
// }
