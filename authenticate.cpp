
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string authenticate(std::shared_ptr<tcp::socket> incoming_conn, std::vector<std::string>& cid) {
    std::string getid;
    boost::system::error_code error;
    bool ok = false;

    std::fstream new_user;
    new_user.open("user.txt", std::ios::app);

    if (!new_user.is_open()) {
        std::cerr << "Failed to open user.txt" << std::endl;
        return "";
    }

    // Loop for user authentication
    while (!ok) {
        boost::asio::write(*incoming_conn, boost::asio::buffer("Enter a unique name: "));

        std::vector<char> buffer(1024);  // Ensuring enough space for the incoming data
        size_t len = incoming_conn->read_some(boost::asio::buffer(buffer), error);

        if (error) {
            std::cerr << "Error reading: " << error.message() << std::endl;
            return "";
        }

        getid.assign(buffer.begin(), buffer.begin() + len);  // Convert buffer to string

        // Check if the id is unique
        ok = true;
        for (const auto& id : cid) {
            if (id == getid) {
                ok = false;
                break;
            }
        }

        if (ok) {
            new_user << "user:" << getid << std::endl;  // Save the new user to the file
        } else {
            boost::asio::write(*incoming_conn, boost::asio::buffer("Name already taken. Please try again.\n"));
        }
    }

    cid.push_back(getid);
    return getid;
}

