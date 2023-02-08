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
	// std::vector<std::string>						cookies; //
	int																	cookie; // by default 1;
};


// fir request form session comes in, no cookies
// 		use logs in > we create cookies; send the key back to the browser as a cookie

// when user logs out -> send command to delete this particular key
