
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>

#include "search.h"
#include "desktop_loading.h"

// The below define the types of searches and the multiplier said search should have
#define SEARCH_NAME 4
#define SEARCH_KEYWORDS 3
#define SEARCH_EXEC 2
#define SEARCH_COMMENT 1

std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops> result;

float get_priority(std::string query, std::string other, int multiplier) {
    if (query.empty() || other.empty()) {
        return 0;
    }

    if (query.compare(other) == 0) {
        return 1024 * multiplier; // Strings are equal, give an absurdly large value
    }

    if (other.find(query) != std::string::npos) {
        return multiplier * 2.0 * query.length(); // We find "query" inside "other"
    }

    // This block of code sees how many characters match between both strings with some leeway on the index
    int matching_chars = 0;

    std::string smaller;
    std::string larger;
    if (query.length() < other.length()) {
        smaller = query;
        larger = other;
    } else {
        smaller = other;
        larger = query;
    }

    for (int c = 0; c < smaller.length(); c++) {
        for (int i = -3; i <= 3; i++) {
            int index = c + i;
            if (index < 0) {
                index = 0;
            } else if (index >= smaller.length()) {
                index = smaller.length() - 1;
            }

            if (index >= larger.length()) {
                break;
            }

            if (smaller.at(index) == larger.at(c)) {
                matching_chars++;
                break;
            }
        }
    }

    if (matching_chars > smaller.length() / 1.5) {
        return matching_chars * multiplier; // Characters found, but half in comparison to finding the full query inside
    }


    if (query.find(other) != std::string::npos) {
        return multiplier * 0.1;
    }
    return 0;
}

void do_search(std::string query, std::vector<Desktop*> *entries, int search_type) {
    for (int i = 0; i < entries->size(); i++) {
        std::string to_check;
        int multiplier = search_type;
        if (search_type == SEARCH_NAME) {
            to_check = entries->at(i)->name;
        } else if (search_type == SEARCH_COMMENT) {
            to_check = entries->at(i)->comment;
        } else if (search_type == SEARCH_KEYWORDS) {
            // Although keywords is technically a ;-separated list, treating it as a giant string works plenty well
            to_check = entries->at(i)->keywords;
        } else if (search_type == SEARCH_EXEC) {
            to_check = entries->at(i)->exec;
        } else {
            std::cout << "Invalid search type specified!" << std::endl;
            return;
        }
        std::transform(to_check.begin(), to_check.end(), to_check.begin(), ::tolower);
        entries->at(i)->priority = get_priority(query, to_check, multiplier);
        if (entries->at(i)->priority != 0) {
            result.push(entries->at(i));
            // Remove an already-found desktop so we don't search for it again
            entries->erase(entries->begin() + i);
            // i-- to counteract so when the i++ of the for-loop runs, we don't skip a .desktop
            i--;
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

    // Do searches from most to least important
    desktop_lock.lock();
    std::vector<Desktop*> copy = entries;  // Make a copy since entries are delete to prevent dupes
    desktop_lock.unlock();
    do_search(query, &copy, SEARCH_NAME);
    do_search(query, &copy, SEARCH_KEYWORDS);
    do_search(query, &copy, SEARCH_EXEC);
    do_search(query, &copy, SEARCH_COMMENT);
}
