// Топология: дерево общего вида; один управляющий узел;
// чтобы добавить новый вычислительный узел к управляющему, необходимо выполнить команду: create id -1.

// Набор команд: локальный целочисленный словарь

// Тип проверки доступности узлов: heartbit time

#include <zmq.h>
#include <signal.h>
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <vector>
#include <cstring>
// #include "server.hpp"
// #include "server_topology.hpp"

using namespace std;

void print_commands() {
    std::cout << "===================================" << std::endl
    << "create [id]" << std::endl
    << "remove [id]" << std::endl
    << "exec [id] [name] [value]" // Сохранение значения 
    << "exec [id] [name]"; // Загрузка значения
    // id, value - целочисленное
    // name - строка формата [A-Za-z0-9]
    std::cout
    << "heartbeat [time (ms)]" << std::endl
    << "print commands" << std::endl
    << "exit" << std::endl
    << "===================================" << std::endl;
}

int main(int argc, char** argv) {

    void* context = zmq_ctx_new();
	cout << "Client starting…" << endl;;

    void* request_socket = zmq_socket(context, ZMQ_REQ);
	zmq_connect(request_socket, "tcp://localhost:4040");

    int count = 0;

    while(true) {

        char msg_text[] = "hello";
        zmq_msg_t req;
        zmq_msg_init_data(&req, msg_text, sizeof(msg_text), NULL, NULL);
		cout << "Client: \n Sending: " << msg_text << " " << count << endl;
		zmq_msg_send(&req, request_socket, 0);
		zmq_msg_close(&req);

		zmq_msg_t reply;
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, request_socket, 0);

	    // cout << "Received: " << zmq_msg_data(&reply) << " " << count << endl;
        char *rp = static_cast<char*>(zmq_msg_data(&reply));
		cout << "Client: \n Received: "  << rp << endl; 

		zmq_msg_close(&reply);
		count++;
    }

    // We never get here though.
	zmq_close(request_socket);
	zmq_ctx_destroy(context);

    return 0;
}