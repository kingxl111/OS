// Топология: дерево общего вида; один управляющий узел;
// чтобы добавить новый вычислительный узел к управляющему, необходимо выполнить команду: create id -1.

// Набор команд: локальный целочисленный словарь

// Тип проверки доступности узлов: heartbit time

#include <iostream>
#include <limits>
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <zmq.hpp>
#include <map>
#include "tree.hpp"
#include "calc_node.hpp"

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

vector<int> child_id;

int main(){
    string command;
    CalcNode node(1, -1, -1);
    Tree tree;
    create(&tree, 1);
    cout << "tree has been created" << endl;
    string answer;
    int parent_id;
    while(cin >> command) { 

        if(command == "print_commands") {
            print_commands();
            continue;
        }

        if(command == "create"){
            int child;
            cin >> child;
            if(exist(&tree, child)) {
                cout << "Error: child already exists!\n";
            }
            else {
                cin >> parent_id;
                child_id.push_back(child);
                while(true) {
                    if(parent_id == node.id){
                        cout << "parent_id == node_id" << endl;
                        answer = node.createChild(child);
                        add_node(&tree, child, find_path(&tree, parent_id));
                        break;
                    }
                    else {
                        string message = "create " + to_string(child);
                        answer = node.sendStr(message, parent_id);
                        if(answer == "Error: id is not found"){
                            not_available(&tree, parent_id);
                        }
                        else {
                            add_node(&tree, child, find_path(&tree, parent_id));
                            break;
                        }
                    }
                }
                cout << answer << endl;
            }
        } 
        else if (command == "heartbit"){
            int time;
            cin >> time;
            for (int i = 0; i <= 2; ++i) {
                for (int cur_child = 0; cur_child < child_id.size(); ++cur_child) {
                    if(!exist(&tree, child_id[cur_child])){
                        cout << "Error: child doesn't exist!" << endl;
                    }
                    else if(node.first_child_id == child_id[cur_child] || node.brother_id == child_id[cur_child]){
                        answer = node.Ping(child_id[cur_child]);
                        cout << "(" << child_id[cur_child] << ") => " << answer << endl;
                    }
                    else {
                        string message = "ping " + to_string(child_id[cur_child]);
                        answer = node.sendStr(message, child_id[cur_child]);
                        if(answer == "Error: id is not found"){
                            answer = "OK: 0";
                        }
                        cout << "(" << child_id[cur_child] << ") => " << answer << endl;
                    }
                }
                usleep(time);
            }
        } 
        else if(command == "exec"){
            string str;
            int child;
            cin >> child;
            getline(cin, str);
            if(!exist(&tree, child)){
                cout << "Error: child doesn't exist!" << endl;
            }
            else {
                string message = "exec " + str;
                answer = node.sendStr(message, child);
                cout << answer << endl;
            }
        }
        else if(command == "remove"){
            int child;
            cin >> child;
            string message = "remove";
            if(!exist(&tree, child)){
                cout << "Error: child doesn't exist!" << endl;
            }
            else {
                answer = node.sendStr(message, child);
                child_id.erase(remove(child_id.begin(), child_id.end(), child), child_id.end());
                if(answer != "Error: id is not found"){
                    pop_node(&tree, find_path(&tree, child));
                    if(child == node.first_child_id){
                        unbind(node.first_child, node.first_child_port);
                        node.first_child_id = -2;
                        answer = "OK";
                    }
                    else if(child == node.brother_id){
                        node.brother_id = -2;
                        unbind(node.brother, node.brother_port);
                        answer = "OK";
                    }
                    else{
                        message = "clear " + to_string(child);
                        answer = node.sendStr(message, stoi(answer));
                    }
                    cout << answer << endl;
                }
            }
        } 
        else if (command == "kill") {
            node.removeElem();
            pop_node(&tree, "");
            child_id.clear();
            break;
        }

        else if (command == "exit") {
            exit(0);
            cout << "See u" << endl;
        }

        else {
            cout << "Wrong command format!" << endl;
        }
    }
    return 0;
}