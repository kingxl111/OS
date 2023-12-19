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
#include "server.hpp"
// #include "server_topology.hpp"

#define MAX_CMD_SIZE 100
#define MAX_MSG_SIZE 1000

using namespace std;

void print_commands() {
    cout << "===================================" << endl
    << "create [id] [parent_id]" << endl
    << "remove [id]" << endl
    << "exec [id] [name] [value]" // Сохранение значения 
    << "exec [id] [name]"; // Загрузка значения
    // id, value - целочисленное
    // name - строка формата [A-Za-z0-9]
    cout
    << "heartbeat [time (ms)]" << endl
    << "print_commands" << endl
    << "exit" << endl
    << "===================================" << endl;
}

int main(int argc, char** argv) {

    void* context = zmq_ctx_new();
	cout << "Client starting…" << endl;;

    void* main_socket = zmq_socket(context, ZMQ_REQ);
	zmq_connect(main_socket, "tcp://localhost:4040");

    int count = 0;
    char cmd[MAX_CMD_SIZE];
    char msg[MAX_MSG_SIZE];
    char reply[MAX_MSG_SIZE];
    int node_id, parent_id; 
    char name[MAX_CMD_SIZE];
    int value;
    int heartbeat_time;
    string cur_cmd;

    while(true) {
        
        // char msg_text[] = "hello";
        // zmq_msg_t req;
        // zmq_msg_init_data(&req, msg_text, sizeof(msg_text), NULL, NULL);
		// cout << "Client: \n Sending: " << msg_text << " " << count << endl;
		// zmq_msg_send(&req, main_socket, 0);
		// zmq_msg_close(&req);

		// zmq_msg_t reply;
		// zmq_msg_init(&reply);
		// zmq_msg_recv(&reply, main_socket, 0);

        // char *rp = static_cast<char*>(zmq_msg_data(&reply));
		// cout << "Client: \n Received: "  << rp << endl; 

		// zmq_msg_close(&reply);
		// count++;
        ///////////////////////////////////////////////////////////////////////////////

        cout << "Enter command" << endl;
        cin >> cmd;
        

        if (cmd == "print_commands") {
            print_commands();
            continue;
        }

        else if (cmd == "create") {
            cin >> node_id >> parent_id;
            cur_cmd = "create " + to_string(node_id) + " " + to_string(parent_id);
        }

        else if (cmd == "remove") {
            cin >> node_id;
            cout << "Warning: all child nodes will be removed\n Continue [Y/N]?: ";
            string confirm;
            cin >> confirm;
            if(confirm == "Y") {
                cur_cmd = "remove " + to_string(node_id);
            }
            else {
                continue;
            }
                
        }

        else if (cmd == "exec") {
            cin >> node_id;
            cin >> name;
            cur_cmd = "exec " + to_string(node_id) + name;
            // Сохраняем новое значение
            if(cin >> value) {
                cur_cmd = cur_cmd + to_string(value);
            }
        }

        else if (cmd == "heartbeat") {
            cin >> heartbeat_time;
            cur_cmd = "heartbeat " + to_string(heartbeat_time);
        }

        else {
            cout << "Wrong command format! Any requests aborted" << endl;
            continue;
        }

        for (size_t i = 0; i < cur_cmd.size(); ++i) {
            msg[i] = cur_cmd[i];
        }
        make_request(main_socket, msg);
        recieve_request(main_socket, reply);
        cout << reply << endl;
    }

    // We never get here though.
	zmq_close(main_socket);
	zmq_ctx_destroy(context);

    return 0;
}