
#pragma once

#include <iostream>
#include <zmq.hpp>
#include <string>
#include <sstream>
#include "unistd.h"
#include "zmq_funcs.hpp"

using namespace std;

class CalcNode{

private:
    zmq::context_t context;

public:
    zmq::socket_t first_child, brother, parent;
    int id, first_child_id = -2, brother_id = -2, parent_id;
    int first_child_port, brother_port, parent_port;

    CalcNode(int _id, int _parent_port, int _parent_id): id(_id), parent_port(_parent_port), parent_id(_parent_id),
    first_child(context, ZMQ_REQ), brother(context,ZMQ_REQ), parent(context,ZMQ_REP){
        if(_id != -1) {
            bind(parent, parent_port);
        }
    }

    string createChild(int childId){
        int port;
        bool isfirst_child = false;
        if(first_child_id == -2) {
            first_child_port = bind(first_child, childId);
            first_child_id = childId;
            port = first_child_port;
            isfirst_child = true;
        } 
        else if(brother_id == -2){
            brother_port = bind(brother, childId);
            brother_id = childId;
            port = brother_port;
        } 
        else {
            return "Create error";
        }
        int pid = fork();
        if(pid == 0){
            cout << "Launching new calc node..." << endl;
            execl("./calc_node_exe", "./calc_node_exe", to_string(childId).c_str(), to_string(port).c_str(), to_string(id).c_str(), (char*)NULL);
            cout << "Execl error" << endl;
            exit(1);
        }
        else {
            string pidChild;
            try {
                if(isfirst_child) {
                    first_child.setsockopt(ZMQ_SNDTIMEO, 3000);
                    make_request(first_child, "pid");
                    pidChild = receive_request(first_child);
                }
                else {
                    brother.setsockopt(ZMQ_SNDTIMEO, 3000);
                    make_request(brother, "pid");
                    pidChild = receive_request(brother);
                }
                return "OK: " + pidChild;
            }
            catch(int){
                return "Error: can't connect to child";
            }
        }
    }

    string Ping(int _id){
        string answer = "OK: 0";
        if(_id == id){
            answer = "OK: 1";
            return answer;
        }
        else if(first_child_id == _id){
            string message = "ping " + to_string(_id);
            make_request(first_child, message);
            try{
                message = receive_request(first_child);
                if(message == "OK: 1"){
                    answer = message;
                }
            }
            catch(int){}
        }
        else if(brother_id == _id){
            string message = "ping " + to_string(_id);
            make_request(brother, message);
            try{
                message = receive_request(brother);
                if(message == "OK: 1"){
                    answer = message;
                }
            }
            catch(int){}
        }
        return answer;
    }

    string sendStr(string str, int _id){
        string answer = "Error: id is not found";

        if(first_child_id == -2 && brother_id == -2){
            return answer;
        }
        else if(first_child_id == _id){
            if(Ping(first_child_id) == "OK: 1"){
                make_request(first_child, str);
                try{
                    answer = receive_request(first_child);
                }
                catch(int){}
            }
        }
        else if(brother_id == _id){
            if(Ping(brother_id) == "OK: 1"){
                make_request(brother, str);
                try{
                    answer = receive_request(brother);
                }
                catch(int){}
            }
        }
        else{                
            if(Ping(first_child_id) == "OK: 1"){
                string message = "send " + to_string(_id) + " " + str;
                make_request(first_child, message);
                try{
                    message = receive_request(first_child);
                }
                catch(int){
                    message = "Error: id is not found";
                }
                if(message != "Error: id is not found"){
                    answer = message;
                }
            }
            if(Ping(brother_id) == "OK: 1"){
                string message = "send " + to_string(_id) + " " + str;
                make_request(brother, message);
                try{
                    message = receive_request(brother);
                }
                catch(int){
                    message = "Error: id is not found";
                }
                if(message != "Error: id is not found"){
                    answer = message;
                }
            }
        }
        return answer;
    }

    string exec(string str){
        istringstream numbers(str);
        int sum = 0;
        int count, number;
        numbers >> count;
        for(int i = 0; i < count; i++){
            numbers >> number;
            sum += number;
        }
        string answer = "OK: " + to_string(id) + ": " + to_string(sum);
        return answer;
    }

    string clearChild(int child){
        if(first_child_id == child){
            first_child_id = -2;
            unbind(first_child, first_child_port);
        }
        else{
            brother_id = -2;
            unbind(brother, brother_port);
        }
        return "OK";
    }

    string removeElem(){
        //string answer;
        if(first_child_id != -2){
            if(Ping(first_child_id) == "OK: 1"){
                string message = "remove";
                make_request(first_child, message);
                try{
                    message = receive_request(first_child);
                }
                catch(int) {}
                unbind(first_child, first_child_port);
                first_child.close();
            }
        }

        if(brother_id != -2){
            if(Ping(brother_id) == "OK: 1"){
                string message = "remove";
                make_request(brother, message);
                try {
                    message = receive_request(brother);
                }
                catch(int){}
                unbind(brother, brother_port);
                brother.close();
            }
        }
        return to_string(parent_id);
    }
};