#include <string>
#include <iostream>
#include <unistd.h>

#include "desktop_loading.h"
#include "search.h"

int main() {
    /**
     * Entrypoint
     */
    get_all_desktops();
    auto search_res = std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops>();
    search("intellij", all_desktops);
    return 0;
}

