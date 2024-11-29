#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <vector>

using boost::asio::ip::tcp;

std::string otherend(std::vector<std::string>& cli, std::shared_ptr<tcp::socket> incom_conn) {
    std::string getid;
    boost::system::error_code error;
    
    // Write the prompt asking who the user wants to talk to
    boost::asio::write(*incom_conn, boost::asio::buffer("Whom do you want to talk to?\n"));

    // Send the list of clients
    for (const auto& client : cli) {
        boost::asio::write(*incom_conn, boost::asio::buffer("\n" + client));
    }

    // Read the client's choice
    std::vector<char> buffer(1024);  // A buffer to read the incoming data
    while (true) {
        size_t len = incom_conn->read_some(boost::asio::buffer(buffer), error);
        if (error) {
            std::cerr << "Error reading: " << error.message() << std::endl;
            return "";
        }
        getid.assign(buffer.begin(), buffer.begin() + len);  // Convert buffer to string
        
        // Check if the client exists in the list
        bool found = false;
        for (const auto& client : cli) {
            if (client == getid) {
                found = true;
                break;
            }
        }

        // If found, break the loop
        if (found) {
            break;
        } else {
            boost::asio::write(*incom_conn, boost::asio::buffer("Client not found, please try again.\n"));
        }
    }

    // Confirm the choice and return the selected client
    boost::asio::write(*incom_conn, boost::asio::buffer("Ok, you are now connected.\n"));
    return getid;
}

