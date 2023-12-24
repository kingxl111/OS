#pragma once
#include <iostream>
#include <set>
#include <string>

typedef struct node node;

struct node {
    int id;
    std::set<int> ex_ids;
    node *child;
    node *next_brother;
};

typedef node* Tree;

void create(Tree *t, int root_id);
Tree* get_node(Tree *t, char *path);
bool add_node(Tree *t, int id, char* path);
void pop_node(Tree *t, char *path);
void print_Tree_tab(Tree t);
bool exist(Tree *t, int id);
