#include <string>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include "desktop_loading.h"
#include "Desktop.h"

std::string desktop_folders_toplevel[3] = {"/usr/share/applications/", "/usr/local/share/applications/",
                                           get_home_desktops_path().append(".local/share/applications/")};
std::vector<Desktop*> all_desktops;

std::string get_home_desktops_path() {
    /**
     * Gets ~/.local/share/applications/
     */
    char* home_dir = getenv("HOME");
    if (home_dir == NULL) {
        std::cout << "$HOME not declared!" << std::endl;
        exit(1);
    }
    std::string home = std::string(home_dir);
    if (home.back() != '/') {
        home.append("/");
    }
    return home;
}

void add_desktops_with_subdirs(std::string path) {

    DIR *dir = opendir(path.c_str());
    struct dirent *entry;
    if (dir == NULL) {
        return; // Bail if we can't open a directory
    }
    entry = readdir(dir);
    while (entry != NULL) {
        std::string subpath = path + std::string(entry->d_name);
        // Check that it isn't . or .. and that it ends with .desktop
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            subpath.find_last_of(".desktop") != subpath.size() + 1) {
            struct stat s;
            if (stat(subpath.c_str(), &s) == 0) {
                if (s.st_mode & S_IFDIR) {
                    add_desktops_with_subdirs(subpath + "/"); // Add .desktop if we have a directory
                } else if (s.st_mode & S_IFREG) {
                    // Add the .desktop if it's a .desktop file
                    Desktop *desktop = new Desktop(subpath);
                    if (desktop->is_valid) {
                        all_desktops.push_back(desktop);
                    } else {
                        delete desktop;
                    }

                }
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);

}

void get_all_desktops() {
    /**
     * Adds all .desktop files to all_desktops
     * Is not automatically called in this file!
     */
    for (int i = 0; i < 3; i++) {
        add_desktops_with_subdirs(desktop_folders_toplevel[i]);
    }
}

void clean_entries() {
    int size = all_desktops.size();
    for (int i = 0; i < size; i++) {
        delete all_desktops.at(i);
    }
}