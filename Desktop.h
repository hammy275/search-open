#include <string>

#ifndef DESKTOP_H
#define DESKTOP_H

#define ICON_TYPE_NAME 0
#define ICON_TYPE_FILE 1


class Desktop {
    public:
        std::string id;
        std::string name;
        std::string comment;
        std::string icon;
        int icon_type = ICON_TYPE_NAME;
        int is_valid = 0;
        int priority = 0;
        explicit Desktop(std::string file_path);
};

struct CompareDesktops {
    bool operator()(Desktop* const & d1, Desktop* const & d2) {
        return d1->priority < d2->priority;
    }
};


#endif //DESKTOP_H
