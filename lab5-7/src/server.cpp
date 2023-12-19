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

int main(int argc, char** argv) {

    void* context = zmq_ctx_new();

    // Сокет для ответа
    void* respond = zmq_socket(context, ZMQ_REP);

    // Привязываем сокет respond к 4040 порту 
    zmq_bind(respond, "tcp://*:4040");
	cout << "Starting server…" << endl;

    while(true) {

        zmq_msg_t request;
		zmq_msg_init(&request);
		zmq_msg_recv(&request, respond, 0);
        char* rq = static_cast<char*>(zmq_msg_data(&request));
		cout << "Server: \n Request: "  << rq << endl; 

		zmq_msg_close(&request);
		sleep(1); // sleep one second
        
        char reply_text[] = " world!"; 
		zmq_msg_t reply;
        zmq_msg_init_data(&reply, reply_text, sizeof(reply_text), NULL, NULL);

		zmq_msg_send(&reply, respond, 0);
		zmq_msg_close(&reply);
    }
    zmq_close(respond);
	zmq_ctx_destroy(context);


    return 0;
}