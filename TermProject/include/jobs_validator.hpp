#pragma once

#include <iostream>
#include <vector>
#include "jobs.hpp"

using graph = std::vector<std::vector<IJob*> >;

bool only_connectivity_component(graph& g);
bool without_cycles(graph &g, vector<IJob*> Jobs, vector<IJob*> &finish_jobs);
std::vector<IJob*> start_component(graph &g, vector<IJob*> Jobs);
std::vector<IJob*> finish_component(graph &g, vector<IJob*> Jobs);
