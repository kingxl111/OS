#include <iostream>
#include "tree.hpp"

using namespace std;

void create(Tree *t, int root_id) {
    (*t) = new node;
    (*t)->child = nullptr;
    (*t)->next_brother = nullptr;
    (*t)->id = root_id;
    // (*t)->ex_ids.insert(root_id);
    (*t)->is_avaliable = true;
}

bool exist(Tree *t, int id) {
    if(t == nullptr) {
        return false;
    }
    // cout << "I'm alive yet" << endl;
    
    return (((*t)->ex_ids.find(id)) != (*t)->ex_ids.end());
}

Tree *get_node(Tree *t, char *path) {

    if (path[0] == '\0') {
        return t;
    }
    else if (path[0] == 'c' && (*t) != nullptr) {
        return get_node(&((*t)->child), path + 1);
    }
    else if (path[0] == 'b' && (*t) != nullptr) {
        return get_node(&((*t)->next_brother), path + 1);
    }
    else {
        return nullptr;
    }
}

bool add_node(Tree *t, int id, char *path) {
    Tree *place = get_node(t, path);
    if (place == nullptr) {
        return false;
    }

    node *new_node = static_cast<node*>(malloc(sizeof(node)));
    new_node->next_brother = (*place);
    new_node->id = id;
    new_node->child = nullptr;
    *place = new_node;
    return true;
}

static void remove_node(Tree t) {
    if (t == nullptr) {
        return;
    }
    remove_node(t->child);
    remove_node(t->next_brother);
    free(t);
}

int pop_node(Tree *t, char *path) {
    Tree to_pop = nullptr;
    Tree *prev = nullptr;
    bool is_child = false;   // Является ли удаляемый узел первым ребенком
    bool is_brother = false; // Является ли удаляемый узел чьим-то братом
    if (t == nullptr)
        return -1;
    while (path[0] != '\0') {

        if (path[0] == 'c' && t != nullptr) {
            prev = t;
            t = &((*t)->child);
            is_brother = false;
            is_child = true;
        }
        else if (path[0] == 'b' && t != nullptr) {
            prev = t;
            t = &((*t)->next_brother);
            is_brother = true;
            is_child = false;
        }
        if ((*t) == nullptr || (path[0] != 'b' && path[0] != 'c')) {
            is_brother = false;
            is_child = false;
            return -1; // Обработка случая, когда пользователь ввёл некорректный путь к узлу(или узла по такому пути нет)
        }
        path = path + 1;
    }
    to_pop = (*t);
    // printf("is_child: %d is_brother: %d\n", is_child, is_brother);
    remove_node(to_pop->child);
    if (is_brother) {
        (*prev)->next_brother = to_pop->next_brother;
    }
    else if (is_child) {
        (*prev)->child = to_pop->next_brother;
    }
    int _id = to_pop->id;
    free(to_pop);
    return _id;
}

static string find_path_intern(node* tnode, int node_id, string cur_path) {
    if(tnode == nullptr) {
        return "";
    }
    if(tnode->id == node_id) {
        return cur_path;
    }
    find_path_intern(tnode->child, node_id, cur_path + 'c');
    find_path_intern(tnode->next_brother, node_id, cur_path + 'b');
}

char* find_path(Tree* t, int node_id) {

    string path = find_path_intern((*t)->child, node_id, "");
    char* answer = new char[path.size()];
    for (size_t i = 0; i < path.size(); ++i) {
        answer[i] = path[i];
    }
    
    return answer;
}

static void not_avaliable_intern(node* cur_node, int node_id) {
    if(!cur_node) {
        return;
    }
    if(cur_node->id == node_id) {
        cur_node->is_avaliable = false;
        return;
    }
    not_avaliable_intern(cur_node->child, node_id);
    not_avaliable_intern(cur_node->next_brother, node_id);
}

void not_available(Tree* t, int node_id) {
    if(t == nullptr) {
        return;
    }
    not_avaliable_intern((*t)->child, node_id);
}

// static void print_Tree_tab_intern(Tree t, int depth) {
//     if (t == nullptr) {
//         return;
//     }
//     for (int i = 0; i < depth; ++i) {
//         printf("    ");
//     }
//     char *color;

//     printf("%s (degree: %d)\n", color, count_degree(t->child));
//     print_Tree_tab_intern(t->child, depth + 1);
//     print_Tree_tab_intern(t->next_brother, depth);
// }

// void print_Tree_tab(Tree t)
// {
//     print_Tree_tab_intern(t, 0);
// }

// int Tree_depth(Tree t)
// {
//     if (t == nullptr)
//     {
//         return 0;
//     }
//     return max(Tree_depth(t->next_brother), 1 + Tree_depth(t->child));
// }