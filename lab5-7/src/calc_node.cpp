#include <iostream>
#include <map>
#include <sys/time.h>
#include <string>
#include <sstream>
#include "tree.hpp"
#include "zmq_funcs.hpp"
#include "calc_node.hpp"

using namespace std;

int main(int argc, char *argv[]){
    if (argc < 4){
        // printf("Usage: ./main childID parentPort parentID\n");
        cout << "Wrong calc_node input format!" << endl;
        exit(-1);
    }

    CalcNode node(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    
    while(true){

        string message;
        string command;
        message = receive_request(node.parent);
        istringstream request(message);
        request >> command;
        if(command == "pid") {
            string answer = to_string(getpid());
            make_request(node.parent, answer);
        }
        else if(command == "ping"){
            int child;
            request >> child;
            string answer = node.Ping(child);
            make_request(node.parent, answer);
        }
        else if(command == "create"){
            int child;
            request >> child;
            string answer = node.createChild(child);
            make_request(node.parent, answer);
        }
        else if(command == "send"){
            int child;
            string str;
            request >> child;
            getline(request, str);
            string answer = node.sendStr(str, child);
            make_request(node.parent, answer);
        }
        else if(command == "exec"){
            string str;
            getline(request, str);
            string answer = node.exec(str);
            make_request(node.parent, answer);
        }
        else if(command == "remove"){
            string answer = node.removeElem();
            make_request(node.parent, answer);
            unbind(node.parent, node.parent_port);
            node.parent.close();
            break;
        } 
        else if(command == "clear"){
            int child;
            request >> child;
            string answer = node.clearChild(child);
            make_request(node.parent, answer);
        }
    }

    sleep(1);
    return 0;
}