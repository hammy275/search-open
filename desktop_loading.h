
#ifndef DESKTOP_FINDING
#define DESKTOP_FINDING

#include <vector>
#include <mutex>

#include "Desktop.h"

void get_all_desktops();
extern std::vector<Desktop*> all_desktops;
void clean_entries();
extern std::mutex desktop_lock;

#endif //DESKTOP_FINDING
