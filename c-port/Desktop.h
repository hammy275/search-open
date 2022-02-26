#include <string>

#ifndef DESKTOP_H
#define DESKTOP_H


class Desktop {
    public:
        std::string id;
        std::string name;
        std::string comment;
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
