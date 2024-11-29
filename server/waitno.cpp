#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <memory>

using boost::asio::ip::tcp;

bool waitno(std::shared_ptr<tcp::socket> him) {
    boost::system::error_code error;
    std::string message = "Do you want to wait or seek connection?\nPress 'w' to wait or 'c' to connect:\n";
    boost::asio::write(*him, boost::asio::buffer(message), error);

    if (error) {
        std::cerr << "Error writing to socket: " << error.message() << std::endl;
        return false;
    }

    char buffer[1];  // We only expect a single character response
    std::size_t len = him->read_some(boost::asio::buffer(buffer), error);

    if (error) {
        std::cerr << "Error reading from socket: " << error.message() << std::endl;
        return false;
    }

    std::cout << buffer[0] << std::endl;

    if (buffer[0] == 'w') {
        return false;
    } else if (buffer[0] == 'c') {
        return true;
    } else {
        return waitno(him);  // Call recursively if the input is invalid
    }
}

