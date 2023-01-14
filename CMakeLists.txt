cmake_minimum_required(VERSION 3.22)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

include_directories(./incl)

add_executable(webserver
        src/main.cpp

        src/Config.cpp
        incl/Config.hpp

        src/Client.cpp
        incl/Client.hpp

        incl/main.hpp
        src/ConfigParser.cpp
        incl/ConfigParser.hpp

        src/error_handler.cpp
        incl/error_handler.hpp

        #TOOLS
        incl/tools.hpp
        src/tools/trim.cpp
        src/tools/split.cpp
        incl/Server.hpp src/Server.cpp)
