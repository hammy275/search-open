#include <iostream>
#include <fstream>
#include <string>

#include "Desktop.h"

Desktop::Desktop(std::string file_path) {
    std::ifstream file;
    file.open(file_path);

    std::string content;
    while (file.good()) {
        std::getline(file, content);
        if (content.rfind("Name=", 0) == 0) {
            // If line starts with Name=
            this->name = content.substr(5);
        } else if (content.rfind("Comment=", 0) == 0) {
            // If line starts with Comment=
            this->comment = content.substr(8);
        }
    }

    if (!this->name.empty() && !this->comment.empty()) {
        // Remove .desktop from end
        this->id = file_path.substr(0, file_path.size() - 8);
        // Remove rest of file path from beginning
        this->id = this->id.substr(this->id.find_last_of('/') + 1);
        this->is_valid = 1;
    }
}