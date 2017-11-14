#ifndef SOLDERINGSTATION_SOCKET_H
#define SOLDERINGSTATION_SOCKET_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

extern tcp::socket *uart_socket;

void socket_write(const char *);
void network_thread();

#endif //SOLDERINGSTATION_SOCKET_H
