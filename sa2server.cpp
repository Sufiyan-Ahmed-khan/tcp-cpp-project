#include <iostream>
#include </home/sufiyanahmedkhan/projects/server/ttt/sample/hyp/msgapp.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <string>
#include<unordered_map>
using boost::asio::ip::tcp;

class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        startAccept();
    }

    // Function to broadcast message to all connected clients
    void broadcastMessage(const std::string& message) {
        for (auto& connection : connections_) {
            if (connection && connection->is_open()) {
                boost::asio::async_write(*connection, boost::asio::buffer(message),
                    [connection](boost::system::error_code ec, std::size_t /*length*/) {
                        if (ec) {
                            std::cerr << "Error on broadcast: " << ec.message() << std::endl;
                            connection->close();
                        }
                    });
            }
        }
    }

private:
    void startAccept() {
        auto new_connection = std::make_shared<tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(*new_connection,
            [this, new_connection](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "New connection accepted." << std::endl;
                    connections_.push_back(new_connection);
                               std::string client=authenticate(new_connection, cid);
                            clidmap[client]=new_connection;
                            bool ornot=waitno(new_connection);
                               if(ornot){
                               std::string endpt= otherend(cid,new_connection);
                               contmessage(client,endpt,clidmap);}
                    //startRead(new_connection);
                }
                startAccept();
            });
    }

    void startRead(std::shared_ptr<tcp::socket> socket) {
        auto buffer = std::make_shared<std::vector<char>>(1024);
        socket->async_read_some(boost::asio::buffer(*buffer),
            [this, socket, buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string message(buffer->data(), bytes_transferred);
                    std::cout << "Received: " << message << std::endl;
                    // Example of broadcasting the received message
                   // broadcastMessage("Server received: " + message);
                    startRead(socket);
                }
                else {
                    std::cerr << "Error on receive: " << ec.message() << std::endl;
                    socket->close();
                }
            });
    }
    std::vector<std::string> cid;
    std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap;
    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<tcp::socket>> connections_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        TCPServer server(io_context, 12345);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

