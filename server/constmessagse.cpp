#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>
#include <string>

using boost::asio::ip::tcp;
void startmg(std::string client, std::string endpt, std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap);

void startmsg(std::string client, std::string endpt, std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap);

void contmessage(std::string client, std::string endpt, std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap) {
    std::vector<char> getid(1024);
    boost::system::error_code error;
    std::size_t len = clidmap[client]->read_some(boost::asio::buffer(getid), error);

    if (!error) {
        boost::asio::write(*clidmap[endpt], boost::asio::buffer("you are connected with " + client), error);
        if (!error) {
            startmsg(client, endpt, clidmap);
            startmg(client, endpt, clidmap);
        } else {
            std::cerr << "Error on write: " << error.message() << std::endl;
        }
    } else {
        std::cerr << "Error on read: " << error.message() << std::endl;
    }
}

void startmsg(std::string client, std::string endpt, std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap) {
    auto buffer = std::make_shared<std::vector<char>>(1024);
    auto buffer1 = std::make_shared<std::vector<char>>(1024);
    auto s1 = clidmap[client];
    auto s2 = clidmap[endpt];

    s1->async_read_some(boost::asio::buffer(*buffer),
        [buffer, s1, s2,client,endpt,clidmap](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::string message(buffer->data(), bytes_transferred);
                boost::system::error_code write_error;
                boost::asio::write(*s2, boost::asio::buffer("recieved:"+message), write_error);
                if (write_error) {
                    std::cerr << "Error on write: " << write_error.message() << std::endl;
                }
            }             startmsg(client, endpt, clidmap);

             
        });
}
void startmg(std::string client, std::string endpt, std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clidmap) {
    auto buffer = std::make_shared<std::vector<char>>(1024);
    auto buffer1 = std::make_shared<std::vector<char>>(1024);
    auto s1 = clidmap[client];
    auto s2 = clidmap[endpt];

    s2->async_read_some(boost::asio::buffer(*buffer1),
        [buffer1, s1, s2,client,endpt,clidmap](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::string message(buffer1->data(), bytes_transferred);
                boost::system::error_code write_error;
                boost::asio::write(*s1, boost::asio::buffer("recieved"+message), write_error);
                if (write_error) {
                    std::cerr << "Error on write: " << write_error.message() << std::endl;
                }

            }            startmg(client, endpt, clidmap);
 
        });

}

