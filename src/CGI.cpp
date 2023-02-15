#include "CGI.hpp"

CGI::CGI() {}

CGI::~CGI() {}


//MEMBER FUNCTIONS
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
	if (response.empty())
		return error("EMPTY RESPONSE!");
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
    if (dup_request_to_stdin(request) == EXIT_FAILURE)
        exit(error("tmpfile creation failed!"));
	if (execve(arguments[0], arguments, environment) == -1)
		exit(error("execve failed!"));
}

int CGI::dup_request_to_stdin(const request_handler& request) {
    int fd = tmpfilefd(); // todo check tmpfile() is not NULL

	if (fd < 0 || write(fd, request.query.c_str(), request.query.length())  < 0)
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