
#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <queue>

#include "Desktop.h"

void search(std::string query, std::vector<Desktop*> entries);
extern std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops> result;
void clean_results();

#endif //SEARCH_H
