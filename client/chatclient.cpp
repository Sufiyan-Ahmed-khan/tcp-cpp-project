#include </home/sufiyanahmedkhan/projects/chatclient.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
using boost::asio::ip::tcp;

void send_messages(tcp::socket &socket) {
  try {
    std::string message;
    while (true) {
      std::getline(std::cin, message);
      if (message == "exit") {
        break;
      }
      message += "\n";
      boost::system::error_code error;
      boost::asio::write(socket, boost::asio::buffer(message), error);
      if (error) {
        std::cerr << "Error sending message: " << error.message() << std::endl;
        break;
      }

      save(message);
    }
  } catch (std::exception &e) {
    std::cerr << "Exception in send_messages: " << e.what() << std::endl;
  }
}

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    std::thread sender_thread(send_messages, std::ref(socket));
    sender_thread.join();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
