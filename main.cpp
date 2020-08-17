#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <chrono>

#include "lizard.hpp"
#include "tiffy.hpp"
#include "options.hpp"

namespace fs = std::filesystem;

bool hasEnding ( std::string const &fullString, std::string const &ending);
std::string getOutputName(const std::string & file );
void buildDirectory( const std::string & dir );


int main(int argc, char** argv) {

    if ( argc > 0 ) {
        processOptions( argc, argv );

        if ( new_project ) {
            create_new_project();
            return 0;
        }

        if ( print_version ) {
            std::cout << "Tiffy version " << VERSION_NUMBER << std::endl;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
	
    cleanLastBuild();
    init_layout();
	createDirs();

	std::ifstream d_file ( ".dir_file" );
	std::string line;

	while ( std::getline( d_file, line ) ) {
        buildDirectory( line );
	}
    
    buildDirectory( "" );

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Build time: " << duration.count() << "ms" << std::endl;
    return 0;

}

void buildDirectory( const std::string & dir ) {
    for ( const auto & entry : fs::directory_iterator( "content/" + dir )) {

        std::string infile = entry.path().u8string();

        if ( hasEnding( infile, ".liz" ) ) {
            std::string outfile = dir + getOutputName( infile );

            if ( outfile[0] == '/' ) {
                outfile.erase( outfile.begin() );
            }

            buildFile( infile, outfile );
        }
    }
}


bool hasEnding (std::string const &fullString, std::string const &ending) {

    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }

}


std::string getOutputName(const std::string & file ) {

    fs::path pathObj(file);

    if ( pathObj.has_stem() ) {
        return  "/" + pathObj.stem().string() + ".html";
    }

    return NULL;

}