#include <http_message.h>

#include <sstream>
#include <iostream>

std::string CLRF = "\r\n";
std::string HTTP = "HTTP";

std::vector<std::string> HttpMessage::split(const std::string& src,
                                            const std::string& delimiter,
                                            int max_length) {
    if (delimiter.empty())
        return std::vector<std::string>();

    std::vector<std::string> res;
    int new_pos{0}, old_pos{0};
    while((new_pos = src.find(delimiter, old_pos)) != std::string::npos) {
        res.push_back(src.substr(old_pos, new_pos - old_pos));
        old_pos = new_pos + delimiter.size();
    }

    if (old_pos < src.size()) {
        res.push_back(src.substr(old_pos));
    }

    if (max_length != -1 && res.size() > max_length) {
        for (int i = max_length; i < res.size(); ++i) {
            res[max_length - 1] += (delimiter + res[i]);
        }
        res.resize(max_length);
    }


    return std::move(res);
}


std::string HttpMessage::get_header(const std::string& key) const {
    auto it = headers_.find(key);
    if (it == headers_.end()) {
        return "";
    }
    return it->second;
}

void HttpMessage::save_header(const std::string& header_line) {
    size_t pos = header_line.find(": ");
    std::string first = header_line.substr(0, pos);
    std::string second = header_line.substr(pos + 2);
    headers_[first] = second;
}

void HttpMessage::save_header(const std::string& header_key, const std::string& header_value) {
    save_header(header_key + ": " + header_value);
}

HttpRequest::HttpRequest(const std::string& method, const std::string& url,
                         const std::string& version, const std::string& headers,
                         const std::string& body)
                         : HttpMessage{} {
    construct(method, url, version, headers, body);
}

HttpRequest::HttpRequest(const std::string& request) : HttpMessage()
{
    std::string line_headers, request_line, headers, body;

    size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos) {
        line_headers = request.substr(0, pos);
        body = request.substr(pos + 4);
    } else {
        line_headers = request;
    }

    pos = line_headers.find("\r\n");
    if (pos != std::string::npos) {
        request_line = line_headers.substr(0, pos);
        headers = line_headers.substr(pos + 2);
    } else {
        request_line = line_headers;
    }

    std::vector<std::string> line_array = split(request_line, " ", 3);
    construct(line_array.at(0), line_array.at(1), line_array.at(2),
              headers, body);

}

void HttpRequest::construct(const std::string& method, const std::string& url,
                            const std::string& version, const std::string& headers,
                            const std::string& body) {
    method_ = method;
    url_ = url;
    version_ = version;
    body_ = body;
    std::vector<std::string> vt = split(headers, "\r\n");
    for (auto&& str : vt) {
        save_header(str);
    }
// #ifndef NDEBUG
//     std::cout << __FUNCTION__ << " method=" << method << " url=" << url << " version=" << version << " headers=" << headers << " body=" << body << std::endl;
// 
// #endif
}

std::string HttpRequest::generate_message() const {
    std::stringstream ss;
    ss << method_ << " " << url_ << " " << version_ << "\r\n";
    for (auto it = headers_.begin(); it != headers_.end(); ++it) {
        ss << it->first << ": " << it->second << "\r\n";
    }

    if (!body_.empty()) {
        ss << "\r\n" << body_;
    }
    return ss.str();
}

HttpResponse::HttpResponse(const std::string& version, const std::string& status,
                           const std::string& reason, const std::string& headers,
                           const std::string& body)
                         : HttpMessage(),
                           version_{version},
                           status_{status},
                           reason_{reason},
                           body_{body} {
    construct(version, status, reason, headers, body);
}

HttpResponse::HttpResponse(const std::string& response) {
    std::string line_headers, status_line, headers, body;

    size_t pos = response.find("\r\n\r\n");
    if (pos != std::string::npos) {
        line_headers = response.substr(0, pos);
        body = response.substr(pos + 4);
    } else {
        line_headers = response;
    }

    pos = line_headers.find("\r\n");
    if (pos != std::string::npos) {
        status_line = line_headers.substr(0, pos);
        headers = line_headers.substr(pos + 2);
    } else {
        status_line = line_headers;
    }

    std::vector<std::string> line_array = split(status_line, " ", 3);
    construct(line_array.at(0), line_array.at(1), line_array.at(2),
              headers, body);
}

void HttpResponse::construct(const std::string& version, const std::string& status,
                           const std::string& reason, const std::string& headers,
                           const std::string& body) {
    version_ = version;
    status_ = status;
    reason_ = reason;
    body_ = body;
    std::vector<std::string> vt = split(headers, "\r\n");
    for (auto&& str : vt) {
        save_header(str);
    }
}

std::string HttpResponse::generate_message() const {
    std::stringstream ss;
    ss << version_ << " " << status_ << " " << reason_ << "\r\n";
    for (auto it = headers_.begin(); it != headers_.end(); ++it) {
        ss << it->first << ": " << it->second << "\r\n";
    }

    if (!body_.empty()) {
        ss << "\r\n" << body_;
    } return ss.str();
}



// int main(int argc, char** argv) {
//     std::string request = "GET /socket.io/?EIO=3&transport=websocket HTTP/1.1\r\n"
//         "Host: 127.0.0.1:8080\r\n"
//         "Connection: Upgrade\r\n"
//         "Pragma: no-cache\r\n"
//         "Cache-Control: no-cache\r\n"
//         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36\r\n"
//         "Upgrade: websocket\r\n"
//         "Origin: http://127.0.0.1:8080\r\n"
//         "Sec-WebSocket-Version: 13\r\n"
//         "Accept-Encoding: gzip, deflate, br\r\n"
//         "Accept-Language: en-US,en;q=0.9\r\n"
//         "Sec-WebSocket-Key: 39CjiNQaO0qkbFYz0FB6HA==\r\n"
//         "Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n"
//         "\r\n"
//         "dafdasfasf\r\n"
//         "dfafdfdfdfaf";
//     HttpRequest hr(request);
//     std::cout << std::endl << std::endl;
//     std::cout << request << std::endl;
//     std::cout << "--------------------------------------------" << std::endl;
//     std::cout << hr.generate_message() << std::endl;
// 
//     std::string response = "HTTP/1.1 101 Switching Protocols\r\n"
//     "Upgrade: websocket\r\n"
//     "Connection: Upgrade\r\n"
//     "Sec-WebSocket-Accept: doCb3D7nFwyQMvWxx2ClQxS4ecs=\r\n"
//     "Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits=15\r\n"
//     "\r\n"
//     "dfsafdfaf\r\n"
//     "dfasfdfsf";
//     HttpResponse hp(response);
//     hp.save_header("key", "value");
//     std::cout << std::endl << std::endl;
//     std::cout << response << std::endl;
//     std::cout << "--------------------------------------------" << std::endl;
//     std::cout << hp.generate_message() << std::endl;
// }
