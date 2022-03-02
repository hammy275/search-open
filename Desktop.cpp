#include <iostream>
#include <fstream>
#include <string>

#include "Desktop.h"

Desktop::Desktop(std::string file_path) {
    bool has_name = false;
    bool has_comment = false;
    bool has_icon = false;
    bool has_keywords = false;
    std::ifstream file;
    file.open(file_path);

    std::string content;
    while (file.good() && (!has_name || !has_comment || !has_icon || !has_keywords)) {
        std::getline(file, content);
        if (!has_name && content.rfind("Name=", 0) == 0) {
            // If line starts with Name=
            this->name = content.substr(5);
            has_name = true;
        } else if (!has_comment && content.rfind("Comment=", 0) == 0) {
            // If line starts with Comment=
            this->comment = content.substr(8);
            has_comment = true;
        } else if (!has_icon && content.rfind("Icon=", 0) == 0) {
            // If line starts with Icon=
            this->icon = content.substr(5);
            has_icon = true;
        } else if (!has_keywords && content.rfind("Keywords=", 0) == 0) {
            this->keywords = content.substr(9);
        }
    }

    file.close();

    if (!this->name.empty() && !this->comment.empty()) {
        // Remove .desktop from end
        this->path = file_path;
        file.open(this->icon);
        if (file) {
            this->icon_type = ICON_TYPE_FILE;
        }
        this->is_valid = 1;
    }
}