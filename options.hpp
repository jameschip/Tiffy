
#ifndef TIFFY_OPTIONS_HPP
#define TIFFY_OPTIONS_HPP

struct tiffy_options {
    bool newProject = false;
};

void processOptions(const int ac, char **av, tiffy_options & out);

#endif