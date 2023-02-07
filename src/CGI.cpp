#include "CGI.hpp"

CGI::CGI() : _response_fd(fileno(tmpfile())) {}

CGI::~CGI() {}


//MEMBER FUNCTIONS
int CGI::create_response(const request_handler &request, std::string &response) {
    if (_response_fd < 0)
        return INTERNAL_SERVER_ERROR;
	switch (fork()) {
		case -1:
			return (error_catched("fork failed!"));
		case 0:
			child_process(request);
	}
	if (parent_process() != EXIT_SUCCESS){
		return INTERNAL_SERVER_ERROR;
	}
	if (write_response(response) != EXIT_SUCCESS)
		return	(error_catched("Reading from pipe failed!"));
	if (response.empty())
		return (error_catched("EMPTY RESPONSE!"));
	return EXIT_SUCCESS;
}

void	CGI::child_process(const request_handler &request) {
	char* environment[request.env.size() + 1];
	for (size_t i = 0; i < request.env.size(); i++)
		environment[i] = const_cast<char*>(request.env[i].c_str());
	environment[request.env.size()] = NULL;

	char *arguments[3];
	arguments[0] = const_cast<char*>(request.interpreter.c_str());
	arguments[1] = const_cast<char*>(request.file_path.c_str());
	arguments[2] = NULL;

    dup2(_response_fd, STDOUT_FILENO);
    if (request_to_stdin(request) == EXIT_FAILURE)
        exit (EXIT_FAILURE);
	if (execve(arguments[0], arguments, environment) == -1)
		exit(EXIT_FAILURE);
}

int CGI::request_to_stdin(const request_handler& request) {
    int fd = fileno(tmpfile());

	if (fd < 0 || write(fd, request.query.c_str(), request.query.length()) == -1)
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
			return (error_catched("execve failed!"));
	}
	else if (WIFSIGNALED(status))
		return (error_catched("interrupted by signal!"));
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

int	CGI::error_catched(const char* message) {
	std::cerr << message << '\n';
	return INTERNAL_SERVER_ERROR;
}