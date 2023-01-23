cmake_minimum_required(VERSION 3.22)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

include_directories(./incl)

add_executable(webserver
        incl/defines.hpp

        src/main.cpp
        incl/main.hpp

        src/Config.cpp
        incl/Config.hpp

        src/ConfigParser.cpp
        incl/ConfigParser.hpp

        src/error_handler.cpp
        incl/error_handler.hpp

        src/Socket.cpp
        incl/Socket.hpp

        src/CLI.cpp
        incl/CLI.hpp

        src/Server.cpp
        incl/Server.hpp

        #TOOLS
        src/tools.cpp
        incl/tools.hpp

        )
