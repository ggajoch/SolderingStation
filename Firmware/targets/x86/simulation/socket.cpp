#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "../unitTests/HALmock.h"

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

tcp::socket *uart_socket = nullptr;

void socket_write(const char * buffer) {
    if (uart_socket != nullptr) {
        boost::asio::write(*uart_socket, boost::asio::buffer(buffer, strlen(buffer)));
    }
}

void network_thread() {
    try {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));

        for (;;) {
            tcp::socket sock(io_service);
            acceptor.accept(sock);

            uart_socket = &sock;

            while(true) {
                char data[100];

                boost::system::error_code error;
                size_t length = sock.read_some(boost::asio::buffer(data, 100), error);
                if (error == boost::asio::error::eof) {
                    break; // Connection closed cleanly by peer.
                } else if (error) {
                    break;
//                    throw boost::system::system_error(error); // Some other error.
                }

                data[length] = '\0';

                HAL::Com::handler(data);
            }

            uart_socket = nullptr;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
