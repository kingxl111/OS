#pragma once

#include <iostream>
#include <vector>

using graph = std::vector<std::vector<int> >;

bool only_connectivity_component(graph& g);
bool without_cycles(graph &g);
std::vector<int> start_component(graph &g);
std::vector<int> finish_component(graph &g);
