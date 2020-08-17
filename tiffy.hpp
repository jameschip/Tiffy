#ifndef TIFFY_HPP
#define TIFFY_HPP

#define VERSION_NUMBER  "2"

void create_new_project( void );
/*
Find and parse the layout file for the pages
*/
void init_layout( void );

/*
Clean all the files from the last build
*/
void cleanLastBuild( void );

/*
Add a line to the buildfile.
*/
void addToBuildFile( const std::string & file );

/*
Create the dirs needed for the build.
*/
void createDirs( void );

/*
Build a liz file and create the apropriate html file.
*/
void buildFile( const std::string & in, const std::string & out );

#endif