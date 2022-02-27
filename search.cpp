
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>

#include "search.h"

#define SEARCH_NAME 0
#define SEARCH_COMMENT 1

std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops> result;

int get_priority(std::string query, std::string other) {
    if (query.empty() || other.empty()) {
        return 0;
    }
    if (query.find(other) != std::string::npos || other.find(query) != std::string::npos) {
        return 1;
    }
    return 0;
}

void do_search(std::string query, std::vector<Desktop*> entries, int search_type) {
    for (int i = 0; i < entries.size(); i++) {
        std::string to_check;
        if (search_type == SEARCH_NAME) {
            to_check = entries.at(i)->name;
        } else if (search_type == SEARCH_COMMENT) {
            to_check = entries.at(i)->comment;
        } else {
            std::cout << "Invalid search type specified!" << std::endl;
            return;
        }
        std::transform(to_check.begin(), to_check.end(), to_check.begin(), ::tolower);
        entries.at(i)->priority = get_priority(query, to_check);
        if (entries.at(i)->priority != 0) {
            result.push(entries.at(i));
        }
    }

}

void clean_results() {
    for (int i = 0; i < result.size(); i++) {
        result.pop();
    }
}


void search(std::string query, std::vector<Desktop*> entries) {
    // Make lowercase
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    clean_results();

    do_search(query, entries, SEARCH_NAME);
    do_search(query, entries, SEARCH_COMMENT);
}
