#include <websocket_service.h>
#include <set>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#define MEM_FN(x)          boost::bind(&self_type::x, shared_from_this()) 
#define MEM_FN1(x, y)      boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x, y, z)   boost::bind(&self_type::x, shared_from_this(), y, z)


using namespace boost::asio;

class Connection;
typedef boost::shared_ptr<Connection> client_ptr;
typedef std::set<client_ptr> client_set;

World world;
client_set clients;
io_service service;
ip::tcp::acceptor acc(service, ip::tcp::endpoint(ip::tcp::v4(), 2001));


class Connection : public boost::enable_shared_from_this<Connection> {
 private:
    typedef Connection self_type;
    // Make constructor private to avoid allocate on the stack
    Connection() : sock_{service}, started_{false}, timer_{service},
                   update_timer_{service}, web_service_{world} {}

 public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<Connection> ptr;
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    static ptr get_shared_ptr() {
        ptr res(new Connection);
        return res;
    }

    void start() {
        started_ = true;
        clients.insert(shared_from_this());
        last_keepalive_ = boost::posix_time::microsec_clock::local_time();
        ip_ = sock_.remote_endpoint().address().to_string();
        do_read();
        update_world();
    }

    void stop() {
        if (!started_) {
            return;
        }
        started_ = false;
        sock_.close();

        ptr self = shared_from_this();
        client_set::iterator it = std::find(clients.begin(), clients.end(), self);
        clients.erase(it);
    }

    bool get_started() const {
        return started_;
    }

    ip::tcp::socket& sock() {
        return sock_;
    }

 private:
    void process_request(std::string& msg) {
        // std::cout << "Receive " << msg << std::endl;
        do_write(web_service_.on_message(msg, ip_));
    }

    void check_alive() {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        if ((now - last_keepalive_).total_milliseconds() > 5000) {
            std::cout << "stopping - no keeplive in time" << std::endl;
            stop();
        } else  {
            std::cout << "Good still alive" << std::endl;
        }
        last_keepalive_ = boost::posix_time::microsec_clock::local_time();
    }

    void keepalive() {
        timer_.expires_from_now(boost::posix_time::millisec(5000));
        timer_.async_wait(MEM_FN(check_alive));
    }

    void on_read(const error_code& err, size_t bytes) {
        if (err) {
            stop();
        }
        if (!started_) {
            return;
        }
        std::string msg(read_buffer_, bytes);
        process_request(msg);
    }

    void on_write(const error_code& err, size_t bytes) {
        stop();
        // do_read();
    }

    void do_read() {
        async_read(sock_, buffer(read_buffer_),
                   MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
        // keepalive();
    }

    void update_world() {
        world.update(0.01);
        update_timer_.expires_from_now(boost::posix_time::millisec(100));
        update_timer_.async_wait(MEM_FN(update_world));
    }

    void do_write(const std::string& msg) {
        if (!started_) {
            return;
        }
        // std::cout << "Writing: " << msg << std::endl;
        // copy to extend the lifetime
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
                                MEM_FN2(on_write, _1, _2));
    }

    size_t read_complete(const boost::system::error_code& err, size_t bytes) {
        if (err) {
            return 0;
        }
        if (bytes < 4) {
            return 4 - bytes;
        }

        bool over;
        std::string method(read_buffer_, read_buffer_ + 4);
        if (method== "GET ") {
            over = read_buffer_[bytes - 1] == '\n' && read_buffer_[bytes - 2] == '\r' &&
                        read_buffer_[bytes - 3] == '\n' && read_buffer_[bytes - 4] == '\r';
            return over? 0 : 1;
        } else {
            over = read_buffer_[bytes - 1] == '\n' && read_buffer_[bytes - 2] == '\r' &&
                        read_buffer_[bytes - 3] == '}';
            return over? 0 : 1;
        }

    }

 private:
    ip::tcp::socket sock_;
    std::string ip_;
    enum { max_msg = 65536 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;

    // Keep alive
    deadline_timer timer_;
    boost::posix_time::ptime last_keepalive_;

    // Game Related
    std::string username_;
    deadline_timer update_timer_;
    WebsocketService web_service_;
};


void handle_accept(Connection::ptr client, const boost::system::error_code& err) {
    client->start();
    Connection::ptr new_client = Connection::get_shared_ptr();
    acc.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}

int main(int argc, char** argv) {
    Connection::ptr client = Connection::get_shared_ptr();
    acc.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
    service.run();
}
