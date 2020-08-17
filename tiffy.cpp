#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "tiffy.hpp"
#include "lizard.hpp"

#define CONTENT_DIR     "content/"
#define MEDIA_DIR       "media/"
#define LAYOUT_SOURCE   "content/tiffy_layout.html"
#define STYLE_SOURCE    "content/tiffy_layout.css"
#define INDEX_SOURCE    "content/index.liz"
#define BUILD_FILE      ".tiffy_build"
#define DIR_FILE		".dir_file"
#define CONTENT_TAG     "{{content}}"


std::string getFileContents( const std::string & fp);

namespace fs = std::filesystem;

std::string layout_top;
std::string layout_bottom;

void create_new_project( void ) {
    
    std::cout << "Creating new project!" << std::endl;

    if ( fs::exists( CONTENT_DIR ) ) {
        std::cout << "Content directory already exists!" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::cout << "Creating content dir!" << std::endl;
    fs::create_directories( CONTENT_DIR );

    std::cout << "Creating layout file!" << std::endl;
    std::ofstream layout ( LAYOUT_SOURCE );
    if ( !layout.is_open() ) {
        std::cout << "Error creating layout file!" << std::endl;
        exit( EXIT_FAILURE );
    }
    layout.close();

    std::cout << "Creating style file!" << std::endl;
    std::ofstream style ( STYLE_SOURCE );
    if ( !style.is_open() ) {
        std::cout << "Error creating style file!" << std::endl;
        exit( EXIT_FAILURE );
    }
    style.close();

    std::cout << "Creating index file!" << std::endl;
    std::ofstream ind ( INDEX_SOURCE );
    if ( !ind.is_open() ) {
        std::cout << "Error creating index file!" << std::endl;
        exit( EXIT_FAILURE );
    }
    ind.close();

    if ( fs::exists( MEDIA_DIR ) ) {
        std::cout << "Media directory already exists!" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::cout << "Creating Media dir!" << std::endl;
    fs::create_directories( MEDIA_DIR );

    std::cout << "Creating dir file!" << std::endl;
    std::ofstream dirf ( DIR_FILE );
    if ( !dirf.is_open() ) {
        std::cout << "Error creating dir file!" << std::endl;
        exit( EXIT_FAILURE );
    }
    dirf.close();

}

void init_layout( void ) {

    if ( !fs::exists( CONTENT_DIR ) ) {
        std::cout << "No content directory!" << std::endl;
        exit( EXIT_FAILURE );
    }

    if ( !fs::exists( LAYOUT_SOURCE ) ) {
        std::cout << "No tiffy_layout.html!" << std::endl;
        exit( EXIT_FAILURE );
    }

    if ( !fs::exists( STYLE_SOURCE ) ) {
        std::cout << "No tiffy_style.css!" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::string layout_content = getFileContents( LAYOUT_SOURCE );

    auto start = 0U;
    auto end = layout_content.find(CONTENT_TAG);

    layout_top = layout_content.substr(start, end - start);
    start = end + 11;
    
    end = layout_content.find(CONTENT_TAG, start);
    layout_bottom = layout_content.substr(start, end);

    fs::copy( STYLE_SOURCE, "tiffy_layout.css");

}

void addToBuildFile( const std::string & file ) {
    std::ofstream f ( BUILD_FILE, std::ios_base::app );
    if ( !f.is_open() ) {
        std::cout << "Error writing to build file!" << std::endl;
        exit( EXIT_FAILURE );
    }
    f << file << std::endl;
    f.close();
}

void cleanLastBuild( void ) {

    // Removeall of the files that were buiilt
    if ( fs::exists ( BUILD_FILE ) ) {

        std::cout << "Castigating the last build!" << std::endl;

        std::ifstream infile( BUILD_FILE );

        if ( !infile.is_open() ){
            std::cout << "Could not open build file!" << std::endl;
            exit( EXIT_FAILURE );
        }

        std::string line;

        while ( std::getline( infile, line ) ) {
            if ( fs::exists( line ) ) {
                fs::remove( line );
            }
        }

        infile.close();

        fs::remove( BUILD_FILE );

    }

    // Remove all of the directories that were created for the build
    if ( fs::exists( DIR_FILE ) ) {

        std::ifstream infile( DIR_FILE );

        if ( !infile.is_open() ){
            std::cout << "Could not open dir file!" << std::endl;
            exit( EXIT_FAILURE );
        }

        std::string line;

        while ( std::getline( infile, line ) ) {
            if ( fs::exists( line ) ) {
                fs::remove( line );
            }
        }

        infile.close();
    }

    fs::remove( "tiffy_layout.css" );

}

void createDirs( void ) {
    std::cout << "Creating dirs!" << std::endl;
    if ( fs::exists( DIR_FILE ) ) {

        std::ifstream infile( DIR_FILE );

        if ( !infile.is_open() ){
            std::cout << "Could not open dir file!" << std::endl;
            exit( EXIT_FAILURE );
        }

        std::string line;

        while ( std::getline( infile, line ) ) {
            fs::create_directories( line );
        }

        infile.close();
    }   
}

void buildFile( const std::string & in, const std::string & out ) {
    std::cout << "Building file -> " << in << std::endl;
    std::string in_content = getFileContents( in );
    std::string out_content = parse( in_content );
    
    std::ofstream f ( out );
    if ( !f.is_open() ){
        std::cout << "Unable to creaete file -> " << out << std::endl;
        exit( EXIT_FAILURE );
    }
    f << layout_top << std::endl << out_content << std::endl << layout_bottom << std::endl;
    f.close();
    addToBuildFile( out );
}

std::string getFileContents( const std::string & fp) {

    std::ifstream f( fp );
    if ( !f.is_open() ) {
        std::cout << "Unable to open file -> " << fp << std::endl;
        exit( EXIT_FAILURE );
    }
    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();
    return buffer.str();
}

