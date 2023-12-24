
#pragma once

#include <iostream>
#include <string>
#include <zmq.hpp>

using namespace std;

const int CONTROL_NODE_PORT = 4040;

// Делаем запрос
void make_request(zmq::socket_t &socket, const string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

// Обрабатываем запрос
string receive_request(zmq::socket_t &socket) {
    zmq::message_t message;
    int chars_read;
    try {
        chars_read = (int)socket.recv(&message);
    }
    catch (...) {
        throw -1;
    }

    string received_msg(static_cast<char*>(message.data()), message.size());
    return received_msg;
}

// Привязываем к сокету определенный порт
int bind(zmq::socket_t &socket, int id) {

    int port = CONTROL_NODE_PORT + id;
    string adress = "tcp://127.0.0.1:" + to_string(port);
    socket.bind(adress);

    return port;
}

// Отвязываем порт от сокета
void unbind(zmq::socket_t &socket, int port) {
    string adress = "tcp://127.0.0.1:" + to_string(port);
    socket.unbind(adress);
}

