#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <memory>

using boost::asio::ip::tcp;

class TCPClient {
public:
    TCPClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : io_context_(io_context), socket_(io_context) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint /*endpoint*/) {
                if (!ec) {
                    std::cout << "Connected to the server." << std::endl;
                    startRead();
                } else {
                    std::cerr << "Failed to connect: " << ec.message() << std::endl;
                }
            });
    }

    void sendMessage(const std::string& message) {
        boost::asio::post(io_context_,
            [this, message]() {
                auto buffer = std::make_shared<std::string>(message);
                boost::asio::async_write(socket_, boost::asio::buffer(*buffer),
                    [this, buffer](boost::system::error_code ec, std::size_t /*length*/) {
                        if (ec) {
                            std::cerr << "Failed to send message: " << ec.message() << std::endl;
                        }
                    });
            });
    }

private:
    void startRead() {
        auto buffer = std::make_shared<std::vector<char>>(1024);
        socket_.async_read_some(boost::asio::buffer(*buffer),
            [this, buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string message(buffer->data(), bytes_transferred);
                    std::cout << "Received: " << message << std::endl;
                    startRead();
                } else {
                    std::cerr << "Error on receive: " << ec.message() << std::endl;
                    socket_.close();
                }
            });
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        TCPClient client(io_context, "127.0.0.1", "12345");

        std::thread t([&io_context]() { io_context.run(); });

        std::string message;
        while (true) {
            std::getline(std::cin, message);
            client.sendMessage(message);
        }

        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

