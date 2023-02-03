#include "CGI.hpp"

CGI::CGI() {}

CGI::~CGI() {}


//MEMBER FUNCTIONS
int CGI::create_response(const request_handler &request, std::string &response) {
	int fd[2];

	if (pipe(fd) == -1)
		return (error_catched("pipe failed!"));
	switch (fork()) {
		case -1:
			close(fd[0]);
			close(fd[1]);
			return (error_catched("fork failed!"));
		case 0:
			child_process(fd, request);
	}
	close(fd[1]);
	if (parent_process() != EXIT_SUCCESS){
		close(fd[0]);
		return INTERNAL_SERVER_ERROR;
	}
	write_response(fd[0], response);
	if (response.empty())
		return (error_catched("EMPTY RESPONSE!"));
	return EXIT_SUCCESS;
}

void	CGI::child_process(int *fd, const request_handler &request) {
	close(fd[0]);
	dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
	close(fd[1]);

	char* environment[request.env.size() + 1];
	for (size_t i = 0; i < request.env.size(); i++)
		environment[i] = const_cast<char*>(request.env[i].c_str());
	environment[request.env.size()] = NULL;

	char *arguments[3];
	arguments[0] = const_cast<char*>(request.interpreter.c_str());
	arguments[1] = const_cast<char*>(request.file_path.c_str());
	arguments[2] = NULL;

	if (execve(PHP_PATH, arguments, environment) == -1)
		exit(EXIT_FAILURE);
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

void CGI::write_response(int fd, std::string &response) {
	char			buffer[1000];
	long long 		bytes = 1;
	std::stringstream 		body_len;

	while (bytes > 0) {
		bytes = read(fd, buffer, sizeof(buffer));
		response += std::string(buffer, bytes);
	}
	close(fd);
	body_len << response.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + response;
}

int	CGI::error_catched(const char* message) {
	std::cerr << message << '\n';
	return INTERNAL_SERVER_ERROR;
}