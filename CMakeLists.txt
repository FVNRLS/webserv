cmake_minimum_required(VERSION 3.22)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

include_directories(./incl)

add_executable(webserver
        incl/defines.hpp

        src/main.cpp

        src/Config.cpp
        incl/Config.hpp

        src/ConfigParser.cpp
        incl/ConfigParser.hpp

        src/Socket.cpp
        incl/Socket.hpp

        src/CLI.cpp
        incl/CLI.hpp

        src/Server.cpp
        incl/Server.hpp

        src/ResponseGenerator.cpp
        incl/ResponseGenerator.hpp

        incl/GETRequest.hpp
        src/GETRequest.cpp

        src/POSTRequest.cpp
        incl/POSTRequest.hpp

        incl/request_handler.hpp
        src/request_handler.cpp

        incl/requestParser.hpp
        src/requestParser.cpp

        src/DELETERequest.cpp
        incl/DELETERequest.hpp

        src/Session.cpp
        incl/Session.hpp

        src/CGI.cpp
        incl/CGI.hpp

        src/Env.cpp
        incl/Env.hpp

        #TOOLS
        src/tools.cpp
        incl/tools.hpp
        src/PUTRequest.cpp incl/PUTRequest.hpp)

