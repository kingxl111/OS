#pragma once

#include <iostream>
#include <set>
#include <string>


class node {
public:
    int id;
    node *child;
    node *next_brother;
    bool is_avaliable;
    std::set<int> ex_ids;
};

typedef node* Tree;

void create(Tree *t, int root_id);
Tree* get_node(Tree *t, char *path);
bool add_node(Tree *t, int id, char* path);
int pop_node(Tree *t, char *path);
void print_Tree_tab(Tree t);
bool exist(Tree *t, int id);
char* find_path(Tree* t, int node_id);
void not_available(Tree* t, int node_id);