#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

#include "lizard.hpp"

namespace fs = std::filesystem;

#define CONTENT_DIR     "/content"
#define ABOUT_DIR       "/about"
#define SOFTWARE_DIR    "/software"
#define AUDIO_DIR       "/audio"
#define GAME_DIR        "/games"
#define WIKI_DIR        "/wiki"
#define MEDIA_DIR       "/media"
#define BUILD_DIR       "/build"
#define LAYOUT_SOURCE   "/tiffy_layout.html"

#define CONTENT_TAG     "{{content}}"


struct PageParts {
    std::string top;
    std::string bottom;
};

bool hasEnding (std::string const &fullString, std::string const &ending);

int handleLayoutFile( const std::string & path, PageParts & pp);

int cleanBuildDirecotry( const std::string & path);

void parseLayoutPage ( const std::string &file, PageParts & pp );

std::string getCurrentWorkingDir( void );

std::string getFileContents( const std::string & fp);

void createOutputFile( const std::string &path, const std::string &content);

std::string parseSPFile( const std::string &path);

int copyMediaFolder( const std::string & from, const std::string & to );

std::string createOutputFileName(const std::string & file, const std::string & outDir );

int main(int argc, char** argv) {

    std::string path =  getCurrentWorkingDir();
    PageParts pp;
    
    if  ( handleLayoutFile( path + CONTENT_DIR + LAYOUT_SOURCE, pp ) == 1 ) {
        return 1;
    }

    // Remove the old build directory if it exists, we wont be needing it.
    if ( cleanBuildDirecotry( path + BUILD_DIR ) == 1 ) {
        return 1;
    }

    if ( copyMediaFolder(path + CONTENT_DIR + MEDIA_DIR, path + BUILD_DIR) == 1 ) {
        return 1;
    }

    fs::copy( path + CONTENT_DIR + "/style.css", path + BUILD_DIR + "/style.css");

    for (const auto & entry : fs::directory_iterator(path + CONTENT_DIR )) {
        
        std::string infile = entry.path().u8string();
        
        if ( hasEnding(infile, ".liz") ) {
            std::string outfile = createOutputFileName( infile, path +  BUILD_DIR );
            std::string content = parseSPFile( infile );
            createOutputFile( outfile, pp.top + content + pp.bottom );
        }

    }

    return 0;

}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::string createOutputFileName(const std::string & file, const std::string & outDir ) {
    fs::path pathObj(file);
    if ( pathObj.has_stem() ) {
        return outDir + "/" + pathObj.stem().string() + ".html";
    }
    return NULL;
}


std::string getCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    getcwd( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}


std::string getFileContents( const std::string & fp) {
    std::ifstream t( fp );
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}


void createOutputFile(const std::string &path, const std::string &content) {
    std::cout  << "Creating file -> " + path << std::endl;
    std::ofstream out( path );
    out << content;
    out.close();
}


std::string parseSPFile( const std::string &path) {
    std::string content = getFileContents( path );
    return parse( content );
}


int handleLayoutFile( const std::string & path, PageParts & pp) {
    if ( !fs::exists(path) ) {
        std::cout << "Can not find layout file." << std::endl;
        return 1;
    }
    parseLayoutPage(path, pp);
    return 0;
}


int cleanBuildDirecotry( const std::string & path) {
    if ( fs::exists( path ) ) {
        std::cout << "Removing the previous build directory." << std::endl;
        if ( !fs::remove_all( path )) {
            std::cout << "Unable to remove previous build" << std::endl;
            return 1;
        }
    }
    return 0;
}


void parseLayoutPage (const std::string &file, PageParts & pp ) {

    std::string content = getFileContents( file );
    auto start = 0U;
    auto end = content.find(CONTENT_TAG);
    pp.top = content.substr(start, end - start);
    start = end + 11;
    end = content.find(CONTENT_TAG, start);
    pp.bottom = content.substr(start, end);
}


int copyMediaFolder( const std::string & from, const std::string & to ) {
    if ( !fs::exists(from) ) {
        std::cout << "No media directory found" << std::endl;
        return 1;
    }
    if ( !fs::exists(to) ) {
        fs::create_directory( to ); 
    }
    fs::copy(from, to + MEDIA_DIR, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    return 0;
}