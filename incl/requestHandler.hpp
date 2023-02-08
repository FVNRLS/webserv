#pragma once

#include "Socket.hpp"

struct request_handler { //todo: delete item from map
	Socket															socket;  // done
	std::string 												raw; 			// done
	std::string 												method;		// done
	std::string													url;			// done
	std::string 												file_path;
	bool 																body_received;  // done
	bool 																head_received;  // done
	int 																status;  // done
	size_t 															body_length;  // not in header parser
	size_t 															head_length;  // ?????
	std::vector<std::string>						env;
	std::string													interpreter; 
	std::string													query; // done
	std::vector<std::string>						cookies; //
};
