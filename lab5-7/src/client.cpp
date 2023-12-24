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

vector<int> treee;

int main(){
    string command;
    CalcNode node(-1, -1, -1);
    Tree tree;
    create(&tree, 1);
    string answer;
    while(cin >> command) { 

        if(command == "print_commands") {
            print_commands();
        }

        if(command == "create"){
            int child;
            cin >> child;
            if(exist(&tree, child)){
                cout << "Error: child already existed!\n";
            }
            else{
                treee.push_back(child);
                while(true){
                    int idParent = tree.findId();
                    if(idParent == node.id){
                        answer = node.createChild(child);
                        tree.addElem(child, idParent);
                        break;
                    }
                    else{
                        string message = "create " + to_string(child);
                        answer = node.sendStr(message, idParent);
                        if(answer == "Error: id is not found"){
                            tree.notAvailable(idParent);
                        }
                        else{
                            tree.addElem(child, idParent);
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
                for (int childC = 0; childC < treee.size(); ++childC) {
                    if(!tree.exist(treee[childC])){
                        cout << "Error: child is not existed!\n";
                    }
                    else if(node.leftId == treee[childC] || node.rightId == treee[childC]){
                        answer = node.Ping(treee[childC]);
                        cout << "(" << treee[childC] << ") => " << answer << endl;
                    }
                    else{
                        string message = "ping " + to_string(treee[childC]);
                        answer = node.sendStr(message, treee[childC]);
                        if(answer == "Error: id is not found"){
                            answer = "OK: 0";
                        }
                        cout << "(" << treee[childC] << ") => " << answer << endl;
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
                cout << "Error: child is not existed!\n";
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
                cout << "Error: child is not existed!\n";
            }
            else {
                answer = node.sendStr(message, child);
                treee.erase(remove(treee.begin(), treee.end(), child), treee.end());
                if(answer != "Error: id is not found"){
                    tree.Remove(child);
                    if(child == node.leftId){
                        unbind(node.left, node.leftPort);
                        node.leftId = -2;
                        answer = "OK";
                    }
                    else if(child == node.rightId){
                        node.rightId = -2;
                        unbind(node.right, node.rightPort);
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
            tree.Remove(tree.findId());
            treee.clear();
            break;
        }
    }
    return 0;
}