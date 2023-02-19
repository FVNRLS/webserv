#include "CGI.hpp"

CGI::CGI() {}

CGI::~CGI() {}


//MEMBER FUNCTIONS
/*
	The function creates a response for a CGI request.
	It takes a reference to a request_handler object and a reference to a string that will hold the response.
	It first creates a temporary file descriptor to store the response.
	If the temporary file descriptor creation fails, the function returns an error.
	It then forks a child process to handle the request.
	If the fork fails, the function returns an error.
	If the fork is successful, the child process is executed using the child_process function.
	The parent process waits for the child process to finish executing.
	If the child process returns an error, the function returns an internal server error.
	If the child process completes successfully, the response is written to the string reference passed in as a parameter.
	If writing to the string fails, the function returns an error.
	Finally, the function returns EXIT_SUCCESS if everything was successful.
 * */
int CGI::create_response(const request_handler &request, std::string &response) {
    _response_fd = tmpfilefd();
    if (_response_fd < 0)
        return error("tmpfile creation failed!");
	switch (fork()) {
		case -1:
			return error("fork failed!");
		case 0:
			child_process(request);
	}
	if (parent_process() != EXIT_SUCCESS){
		return INTERNAL_SERVER_ERROR;
	}
	if (write_response(response) != EXIT_SUCCESS)
		return error("Reading from pipe failed!");
	return EXIT_SUCCESS;
}

void	CGI::child_process(const request_handler &request) {
	char* environment[request.env.size() + 1];
	for (size_t i = 0; i < request.env.size(); i++)
		environment[i] = const_cast<char*>(request.env[i].c_str());
	environment[request.env.size()] = NULL;

	char *arguments[3];
	arguments[0] = const_cast<char*>(request.interpreter.c_str());
	arguments[1] = realpath(request.file_path.c_str(), NULL);
	arguments[2] = NULL;

    dup2(_response_fd, STDOUT_FILENO);
    if (dup_request_to_stdin(request) == EXIT_FAILURE)
        exit(error("tmpfile creation failed!"));
    if (!request.cgi_path.empty() && chdir(request.cgi_path.c_str()) == -1)
        exit(error("chdir failed!"));
    if (execve(arguments[0], arguments, environment) == -1)
		exit(error("execve failed!"));
}

int CGI::dup_request_to_stdin(const request_handler& request) {
    int fd = tmpfilefd();

	if (fd < 0 || write(fd, request.query.c_str(), request.query.length()) < 0)
       return EXIT_FAILURE;
    lseek(fd, 0, SEEK_SET);
	dup2(fd, STDIN_FILENO);
    close(fd);
	return EXIT_SUCCESS;
}

int	CGI::parent_process() {
	int status;

	waitpid(-1, &status, 0);
	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status))
			return (error("execve failed!"));
	}
	else if (WIFSIGNALED(status))
		return error("interrupted by signal!");
	return EXIT_SUCCESS;
}

int CGI::write_response(std::string &response) {
	char			buffer[1000];
	long long 		bytes = 1;

    lseek(_response_fd, 0, SEEK_SET);
	while (bytes > 0) {
		bytes = read(_response_fd, buffer, sizeof(buffer));
		if (bytes < 0) {
			close(_response_fd);
			return EXIT_FAILURE;
		}
		else
			response += std::string(buffer, bytes);
	}
	close(_response_fd);
	return	EXIT_SUCCESS;
}

int	CGI::error(const char* message) {
	std::cerr << message << '\n';
	return INTERNAL_SERVER_ERROR;
}

int CGI::tmpfilefd() {
    FILE*   tmp = tmpfile();

    if (tmp == NULL)
        return -1;
    return fileno(tmp);
}