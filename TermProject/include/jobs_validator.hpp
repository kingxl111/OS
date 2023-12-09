#pragma once

#include <iostream>
#include <vector>

using graph = std::vector<std::vector<int> >;

bool only_connectivity_component(graph &g);
bool without_cycles(graph &g);
bool has_start_component(graph &g);
bool has_finish_component(graph &g);
