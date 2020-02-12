#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE

#include <unordered_map>
#include <vector>

class HttpMessage {
 public: 
    virtual ~HttpMessage() {};
    virtual std::string generate_message() const = 0;
    std::string get_header (const std::string& key) const;
    void save_header(const std::string& header_line);
    void save_header(const std::string& header_key, const std::string& header_value);

 protected:
    std::vector<std::string> split(const std::string& src,
                                   const std::string& delimiter,
                                   int max_length = -1);
    std::unordered_map<std::string, std::string> headers_;

};

class HttpRequest : public HttpMessage {
 public:
    explicit HttpRequest(const std::string& request);
    HttpRequest(const std::string& method, const std::string& url, const std::string& version,
                const std::string& headers, const std::string& body);
    virtual std::string generate_message() const override;
    const std::string& method() const {return method_;}
    const std::string& url() const {return url_;}
    const std::string& version() const {return version_;}
    const std::string& body() const {return body_;}
    const std::string& ip() const {return ip_;}

 private:
    void construct(const std::string& method, const std::string& url, const std::string& version,
                   const std::string& headers, const std::string& body);

 private:
    std::string method_;
    std::string url_;
    std::string version_;
    std::string body_;
    std::string ip_;
};

class HttpResponse : public HttpMessage {
 public:
    explicit HttpResponse(const std::string& response);
    HttpResponse(const std::string& version, const std::string& status, const std::string& reason,
                 const std::string& headers, const std::string& body);
    virtual std::string generate_message() const override;
    const std::string& version() const {return version_;}
    const std::string& status() const {return status_;}
    const std::string& reason() const {return reason_;}
    const std::string& body() const {return body_;}

 private:
    void construct(const std::string& version, const std::string& status, const std::string& reason,
                   const std::string& headers, const std::string& body);

 private:
    std::string version_;
    std::string status_;
    std::string reason_;
    std::string body_;
};


#endif  // HTTP_MESSAGE
