
#ifndef DESKTOP_FINDING
#define DESKTOP_FINDING

#include <vector>

#include "Desktop.h"

std::string get_home_desktops_path();
extern std::string desktop_folders_toplevel[3];

void get_all_desktops();
extern std::vector<Desktop*> all_desktops;
void clean_entries();

#endif //DESKTOP_FINDING
