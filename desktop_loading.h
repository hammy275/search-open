
#ifndef DESKTOP_FINDING
#define DESKTOP_FINDING

#include <vector>

#include "Desktop.h"

void get_all_desktops();
extern std::vector<Desktop*> all_desktops;
void clean_entries();

#endif //DESKTOP_FINDING
