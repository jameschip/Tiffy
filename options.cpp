#include <getopt.h>
#include <iostream>

#include "options.hpp"

bool new_project = false;
bool print_version = false;

void processOptions( const int ac, char **av ) {

    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"new_project", required_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while (optind < ac)
    {
        auto option_index = 0;
        // auto c = getopt_long(ac, av, "hdplri:", long_options, &option_index);
        int c;
        if ((c = getopt_long(ac, av, "nv", long_options, &option_index)) != -1) {
            switch (c)
            { 
            case 'v':
                print_version = true;
                break;
            
            case 'n':
                new_project = true;
                break;
                
            case '?':
                std::cout << "Unknown option: " << c << std::endl;
                exit(0);
                break;

            default:
                abort();
            }
        }
    }

}