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
		return FORBIDDEN;
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

	char** argv = create_arguments();
	char** envp = create_environment();
	if (execve(PHP_PATH, argv, envp) == -1)
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

char**		CGI::create_environment() {
	char**	env = (char**)malloc(2);

	env[0] = getenv("PATH");
	std::cerr << "ENV ===> " << env[0] << '\n';
	env[1] = NULL;
	return env;
}

char**		CGI::create_arguments() {
	char **argv = (char**)malloc(3);

	argv[0] = PHP_PATH;
	std::cerr << "ARGV ===> " << argv[0] << '\n';
	argv[1] = CGI_SCRIPT_PATH;
	std::cerr << "ARGV ===> " << argv[1] << '\n';
	argv[2] = NULL;
	return argv;
}



int	CGI::error_catched(const char* message) {
	std::cerr << message << '\n';
	return FORBIDDEN;
}