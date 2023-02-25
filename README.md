
# webserv
webserv is a C++ HTTP server that provides basic functionalities for serving web pages, handling HTTP requests and responses, and managing sockets.

## Installation
1. Clone the repository: git clone https://github.com/FVNRLS/webserv
2. Build the project: make

## Running the server
To run the server, execute the following command in the terminal:
<br>`./webserv config_file`
<br><br>Alternatively execute the following command to run the server from default configuration file:
<br>`make run`

## Dependencies
- C++98 
- Make 
- CMake (optional, for building the project)
- python3 
- php

## Setup

1. Verify if some packages has been already installed by running the following commands in the terminal:
- gcc --version
- make --version
- cmake --version
- python3 --version
- php --version

2. install missing packages.


### Setup on Mac (with brew)
- brew update 
- brew install gcc
- brew install make
- brew install cmake 
- brew install python 
- brew install php

### Setup on Arch
- sudo pacman -Syu
- sudo pacman -S gcc
- sudo pacman -S make
- sudo pacman -S cmake
- sudo pacman -S python
- sudo pacman -S php

## Usage
webserv requires a configuration file as an argument in order to run. The configuration file specifies the ports and roots for the server.
pick a browser of your choice and visit the server's website by entering the address specified in the ``*.conf`` file (standard is localhost)
<br>Project was extensively tested with Google Chrome and Chromium, it may be that other browsers behave differently in certain cases.

## Configuration file
The configuration file is a plain text file that specifies the ports and roots for the server. 
<br>The file should contain one or more server blocks. 
Each server block begins with the server keyword and contains directives like in following example:

<details>
  <summary>Click to expand server configuration</summary>

    server {
    server_name                     TurkmenianRacoons;
    ip_address                      127.0.0.1;
    port                            6969;
    root                            html/;
    allowed_methods                 GET PUT POST DELETE;
    index                           index.html;
    max_client_body_size            ;   
    redirection                     google = https://www.google.com/;
    
        location /hering {
            allowed_methods	        GET POST DELETE;
            allowed_scripts         py = /usr/bin/python;
            allowed_scripts         php = /usr/bin/php;
            index                   start.py;
            cgi_path                ;
            directory_listing       true;
        }
    
        location /khan {
            root                    html/cgi/;
            allowed_methods         GET POST;
            allowed_scripts         py = /usr/bin/python;
            allowed_scripts         php = /usr/bin/php;
            index                   start.py;
            cgi_path                ;
            directory_listing       true;
        }
    
        location /hollandic_people {
            root                    html/cgi/;
            allowed_methods         GET POST;
            allowed_scripts         py = /usr/bin/python;
            allowed_scripts         php = /usr/bin/php;
            index                   start.py;
            cgi_path                ;
            directory_listing       true;
        }
    }
</details>


## Contributing
Contributions are welcome! If you find a bug or have an idea for a new feature, please create an issue or a pull request.
