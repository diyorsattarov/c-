// Client.h
#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <utilities/utilities.h>

using boost::asio::ip::tcp;

class Client {
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    std::string server_response_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::mutex mtx;
    std::condition_variable cv;
    bool connected = false;

public:
    Client(boost::asio::io_service& io_service, const std::string& host, const std::string& port);

    const std::string& server_response() const;

    void notifyConnected() {
        std::unique_lock<std::mutex> lock(mtx);
        connected = true;
        cv.notify_one();
    }

    void waitForConnection() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return connected; });
    }

private:
    void doConnect(const tcp::resolver::results_type& endpoints);
    void doRead();
};

#endif //CLIENT_H
