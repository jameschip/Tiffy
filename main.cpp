#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

#include "lizard.hpp"
#include "options.hpp"

#define CONTENT_DIR     "/content"
#define MEDIA_DIR       "/media"
#define LAYOUT_SOURCE   "/tiffy_layout.html"
#define STYLE_SOURCE    "/tiffy_layout.css"
#define BUILD_FILE      "/.tiffy_build"
#define CONTENT_TAG     "{{content}}"


namespace fs = std::filesystem;


struct PageParts {
    std::string top;
    std::string bottom;
};


bool hasEnding ( std::string const &fullString, std::string const &ending);
int handleLayoutFile( const std::string & path, PageParts & pp );
void parseLayoutPage( const std::string &file, PageParts & pp );
std::string getFileContents( const std::string & fp );
void createOutputFile( const std::string &path, const std::string &content );
std::string parseLizardFile( const std::string &path );
std::string getOutputName(const std::string & file );
void cleanLastBuild( const std::string & path );


int main(int argc, char** argv) {

    const std::string path = fs::current_path();
    
    tiffy_options topt;

    processOptions( argc, argv, topt );

    // Do new project setup.
    if ( topt.newProject == true ) {
        std::cout << "Giving a go at creating new project!" << std::endl;
        fs::create_directory( path + CONTENT_DIR );
        std::ofstream file_s; 
        file_s.open( path + CONTENT_DIR + STYLE_SOURCE );
        file_s.close();
        file_s.open( path + CONTENT_DIR + LAYOUT_SOURCE );
        file_s.close();
        fs::create_directory( path + MEDIA_DIR );
        std::cout << "Done!" << std::endl;
        return 1;
    }

    PageParts pp;
    
    cleanLastBuild( path );

    if ( fs::exists ( path + STYLE_SOURCE ) ) {
        fs::remove( path + STYLE_SOURCE );
    }

    fs::copy( path + CONTENT_DIR + STYLE_SOURCE, path + STYLE_SOURCE);

    if  (fs::exists( path + LAYOUT_SOURCE ) ){
        fs::remove( path + LAYOUT_SOURCE );
    }

    if  ( handleLayoutFile( path + CONTENT_DIR + LAYOUT_SOURCE, pp ) == 1 ) {
        return 1;
    }

    std::ofstream b_file;

    b_file.open( path + BUILD_FILE ); 

    for (const auto & entry : fs::directory_iterator(path + CONTENT_DIR )) {
        
        std::string infile = entry.path().u8string();
        
        if ( hasEnding(infile, ".liz") ) {
            std::string outfile = getOutputName( infile );
            b_file << outfile << std::endl;
            std::string content = parseLizardFile( infile );
            createOutputFile( path + outfile, pp.top + content + pp.bottom );
        }

    }

    b_file.close();

    return 0;

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


std::string getFileContents( const std::string & fp) {

    std::ifstream t( fp );
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();

}


void createOutputFile(const std::string &path, const std::string &content) {

    std::cout  << "Creating -> " + path << std::endl;
    std::ofstream out( path );
    out << content;
    out.close();

}


std::string parseLizardFile( const std::string &path) {

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


void parseLayoutPage (const std::string &file, PageParts & pp ) {

    std::string content = getFileContents( file );
    auto start = 0U;
    auto end = content.find(CONTENT_TAG);
    pp.top = content.substr(start, end - start);
    start = end + 11;
    end = content.find(CONTENT_TAG, start);
    pp.bottom = content.substr(start, end);

}

void cleanLastBuild( const std::string & path ) {

    if ( fs::exists ( path + BUILD_FILE ) ) {

        std::cout << "Castigating the last build!" << std::endl;

        std::ifstream infile( path + BUILD_FILE );
        std::string line;

        while ( std::getline( infile, line ) ) {
            if ( fs::exists( path + "/" + line ) ) {
                fs::remove( path + "/" + line );
            }
        }

        fs::remove( path + BUILD_FILE );

    }

}
