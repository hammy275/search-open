
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>

#include "search.h"

// The below define the types of searches and the multiplier said search should have
#define SEARCH_NAME 3
#define SEARCH_KEYWORDS 2
#define SEARCH_COMMENT 1

std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops> result;

int get_priority(std::string query, std::string other, int multiplier) {
    if (query.empty() || other.empty()) {
        return 0;
    }
    if (query.find(other) != std::string::npos || other.find(query) != std::string::npos) {
        return 1 * multiplier;
    }
    return 0;
}

void do_search(std::string query, std::vector<Desktop*> entries, int search_type) {
    for (int i = 0; i < entries.size(); i++) {
        std::string to_check;
        int multiplier = search_type;
        if (search_type == SEARCH_NAME) {
            to_check = entries.at(i)->name;
        } else if (search_type == SEARCH_COMMENT) {
            to_check = entries.at(i)->comment;
        } else if (search_type == SEARCH_KEYWORDS) {
            // Although keywords is technically a ;-separated list, treating it as a giant string works plenty well
            to_check = entries.at(i)->keywords;
        } else {
            std::cout << "Invalid search type specified!" << std::endl;
            return;
        }
        std::transform(to_check.begin(), to_check.end(), to_check.begin(), ::tolower);
        entries.at(i)->priority = get_priority(query, to_check, multiplier);
        if (entries.at(i)->priority != 0) {
            result.push(entries.at(i));
        }
    }

}

void clean_results() {
    int size = result.size();
    for (int i = 0; i < size; i++) {
        result.pop();
    }
}


void search(std::string query, std::vector<Desktop*> entries) {
    // Make lowercase
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    clean_results();

    // Do searches from least to most impact
    do_search(query, entries, SEARCH_COMMENT);
    do_search(query, entries, SEARCH_KEYWORDS);
    do_search(query, entries, SEARCH_NAME);
}
