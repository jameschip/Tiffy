
#include <getopt.h>
#include "options.hpp"

void processOptions(const int ac, char **av, tiffy_options & out) {

    static struct option long_options[] = {
        // {"input", required_argument, 0, 'i'},
        {"new", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while (optind < ac)
    {
        auto option_index = 0;
        // auto c = getopt_long(ac, av, "hdplri:", long_options, &option_index);
        int c;
        if ((c = getopt_long(ac, av, "n", long_options, &option_index)) != -1) {
            switch (c)
            { 
            case 'n':
                out.newProject = true;
                return;
            default:
                return;
            }
        }
    }
}