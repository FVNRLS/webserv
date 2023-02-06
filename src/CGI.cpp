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
	if (write_response(fd[0], response) != EXIT_SUCCESS)
		return	(error_catched("Reading from pipe failed!"));
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

    create_tmp_file(request);
	if (execve(arguments[0], arguments, environment) == -1)
		exit(EXIT_FAILURE);
}

int CGI::create_tmp_file(const request_handler& request) {
    int tmpfd = fileno(tmpfile());

    write(tmpfd, request.query.c_str(), request.query.length());
    dup2(tmpfd, STDIN_FILENO);
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



int CGI::write_response(int fd, std::string &response) {
	char			buffer[1000];
	long long 		bytes = 1;

	while (bytes > 0) {
		bytes = read(fd, buffer, sizeof(buffer));
		if (bytes < 0) {
			close(fd);
			return EXIT_FAILURE;
		}
		else
			response += std::string(buffer, bytes);
	}
	close(fd);
	response = RESPONSE_HEADER + toString<size_t>(response.length()) + "\n\n" + response;
	return	EXIT_SUCCESS;
}

int	CGI::error_catched(const char* message) {
	std::cerr << message << '\n';
	return INTERNAL_SERVER_ERROR;
}