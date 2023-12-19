#pragma once

#include <zmq.h>
#include <unistd.h>
#include <iostream>
#include <string>

void make_request(void* socket, char msg_text[]) {
    zmq_msg_t req;
    zmq_msg_init_data(&req, msg_text, sizeof(msg_text), NULL, NULL);
    std::cout << "Client: \n Sending: " << msg_text << " " << std::endl;
    zmq_msg_send(&req, socket, 0);
    zmq_msg_close(&req);
}

void recieve_request(void* socket, char reply_text[]) {
    zmq_msg_t request;
    zmq_msg_init(&request);
    zmq_msg_recv(&request, socket, 0);
    char* rq = static_cast<char*>(zmq_msg_data(&request));
    std::cout << "Server: \n Request: "  << rq << std::endl;
    reply_text = rq; 
    zmq_msg_close(&request);
}

