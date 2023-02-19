#include "DELETERequest.hpp"

//BASIC CLASS SETUP
DELETERequest::DELETERequest() {}

DELETERequest::~DELETERequest() {}


//MEMBER FUNCTIONS

/*
 * Handles a file deletion request by parsing the filename from the request buffer, checking whether the file exists,
 * attempting to delete the file, and returning the appropriate success or error code depending on the outcome of the deletion operation.

DESCRIPTION:
 * Searches for the position of the string "name=\"" within the request buffer, which is expected to contain the name of the file to be deleted.
 * If this string is not found, the function returns a BAD_REQUEST error code.
 * If the "name=\"" string is found, the function parses the filename by finding the position of the closing double quote.
 * The parsed filename is then concatenated with the path "uploads/" to form the filepath variable.
 *
 * Next, the function checks whether the file at the specified filepath exists using the access() system call, which returns 0 if the file exists, and -1 otherwise.
 * If the file does not exist, the function returns a PAGE_NOT_FOUND error code.
 *
 * If the file exists, the function attempts to delete it using the unlink() system call.
 * If this call fails (i.e., it returns -1), the function returns a FORBIDDEN error code, indicating that the file could not be deleted.
 *
 * If the file is successfully deleted, the function returns an OK status code, indicating that the file was successfully deleted.
 * Overall, this function seems to be designed to
 * */
int DELETERequest::create_response(const request_handler &request) {
	std::string	filepath;

	size_t begin = request.buf.find("name=\"");
	if (begin == std::string::npos)
		return BAD_REQUEST;
	begin += std::strlen("name=\"");
	size_t end = request.buf.find("\"", begin);
	if (end == std::string::npos)
		return BAD_REQUEST;
	filepath = "uploads/" + request.buf.substr(begin, end - begin);

	if (access(filepath.c_str(), F_OK) < 0 )
		return PAGE_NOT_FOUND;
	if (unlink(filepath.c_str()) == -1)
		return FORBIDDEN;
	return OK;
}