#pragma once

#include <iostream>
#include <vector>

class Tree {
private:
    struct Node {
        Node(int _id): id(_id) {}
        int id;
        Node* child = nullptr;
        Node* brother = nullptr;
    };
    Node* head = nullptr;

public:
    Tree() = default;
    ~Tree() {
        this->delete_tree(this->head);
    }
    std::vector<int> get_all_nodes(Node* node=nullptr) {
        std::vector<int> result;
        if (node == nullptr)
            node = this->head;
        this->all_nodes(node, result);
        return result;
    }
    std::vector<int> get_path_to(int& id, Node* node=nullptr) {
        std::vector<int> path;
        if (node == nullptr)
            node = this->head;
        this->find_path(node, id, path);
        return path;
    }
    bool contains(int& id) const {
        Node* tmp = this->head;
        while(tmp != nullptr){
            if(tmp->id == id)
                break;
            else if(id > tmp->id)
                tmp = tmp->brother;
            else if(id < tmp->id)
                tmp = tmp->brother;
        }
        return tmp != nullptr;
    }

    void insert(int& id){
        if(this->head == nullptr){
            this->head = new Node(id);
            return;
        }
        Node* tmp = this->head;
        while(tmp != nullptr) {
            if(tmp->id == id)
                break;
            else if(id < tmp->id){
                if(tmp->child == nullptr){
                    tmp->child = new Node(id);
                    break;
                }
                tmp = tmp->child;
            }

            else if(id > tmp->id){
                if(tmp->brother == nullptr){
                    tmp->brother = new Node(id);
                    break;
                }
                tmp = tmp->brother;
            }
        }
    }
    void erase(int& id) {
        Node* prev = nullptr;
        Node* tmp = this->head;
        while(tmp != nullptr){

            if (id == tmp->id) {
                if (prev == nullptr) {
                    this->head = nullptr;
                }
                else {
                    if (prev->child == tmp) {
                        prev->child = nullptr;
                    } else {
                        prev->brother = nullptr;
                    }
                }
                delete_tree(tmp);
                tmp = nullptr;
            } 
            else if(id < tmp->id) {
                prev = tmp;
                tmp = tmp->child;
            } 
            else if(id > tmp->id) {
                prev = tmp;
                tmp = tmp->brother;
            }
        }
    }
    private:
    void all_nodes(Node* node, std::vector<int>& vec) const{
        if(node == nullptr)
            return;
        this->all_nodes(node->child, vec);
        vec.push_back(node->id);
        this->all_nodes(node->brother, vec);
    }

    void find_path(Node* node, int& id, std::vector<int>& path) {
        while(node != nullptr){
        path.push_back(node->id);

        if(node->id == id)
            break;
        else if(id > node->id)
            node = node->brother;
        else if(id < node->id)
            node = node->brother;
        }
    }
    
    void delete_tree(Node* node) {
        if(node == nullptr) 
            return;
        delete_tree(node->brother);
        delete_tree(node->child);
        delete node;
        node = nullptr;
    }
    friend class TestBitTree;
};

