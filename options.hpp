
#ifndef TIFFY_OPTIONS_HPP
#define TIFFY_OPTIONS_HPP

#include <string>

struct tiffy_options {
    bool newProject = false;
    bool newFile = false;
    std::string fileName;
};

void processOptions(const int ac, char **av, tiffy_options & out);

#endif