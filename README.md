# Tiffy
Tiffy is a simple minimalist static website generator that creates the flow of information on the river. 
Tiffy uses a single html layout file and a single css file to create pages filled with content from the [Lizard](https://github.com/jameschip/Lizard)
files you create.

## Tiffy projects

A Tiffy project can be created in any directory and has a simple file structure.  
  
* /content                     
* /content/tiffy_loyout.html  
* /content/tiffy_layout.css
* /content/index.liz
* /media
* /.tiffy_build
* /.dir_file

An example project can be seen [by looking jameschip.io on github](https://github.com/jameschip/jameschip.github.io)

### The content directory.

The content directory is where all of your .liz files live. It also holds the tiffy_layout.html and tiffy_layout.css files. When you build your website each .liz file in the content directory will be converted into a .html file in the project's root folder, for example; index.liz will be converted into index.html in the root directory. 

### The tiffy_layout.html and tiffy_layout.css files.

The tiffy_layout.html file is the basic layout for every page of your website. It is a regular html file in every way except that it includes a single special ```{{content}}``` tag. When you build your pages the ```{{content}}``` tag will be replaced with the content generated from a .liz file. The tiffy_layout.css file
is included in the header of tiffy_layout.html and will be copied into the projects root folder when you build your project.

### The index.liz file.

This is a single content file. When you build your project, an html file with the same name will be created in the project's root folder. 

### The .dir_file file

This file contains a list of files relative to /content that you have your websites files in. The top line should be / and everything under that should be /dir_name. One directory per line. This is to allow you to split your content into sensible folders in your project such as /games /software and so on.
The output files will still be in the root of the project, not in sub folders, this is just for convenience of editing. [Example from jameschip.io](https://github.com/jameschip/jameschip.github.io/blob/master/.dir_file)

### The .tiffy_build file

After a tiffy build this file will have the relative path of all of the output files in it. It's basically a log file, leave this alone to be on the safe side.

### The media folder.

This is where you should put all of the images and other media that you need for your pages.

## Building the project.

To build your website, navigate to the root of your project and run:  

```tiffy```  
  
The project's root directory will be populated with html files.

