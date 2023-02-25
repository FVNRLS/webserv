#include "CGI.hpp"

CGI::CGI() {}

CGI::~CGI() {}


//MEMBER FUNCTIONS

/*
 * The function is used to generate a response to a HTTP request by executing the child_process function with requested parameters.
 * It creates a temporary file descriptor, forks a child process to handle the request,
 * and then writes the response data to the response parameter using the write_response function and returns EXIT_SUCCESS.
 * Otherwise, an internal server error is returned.
 * */
int CGI::create_response(const request_handler &request, std::string &response) {
    _response_fd = tmp_fd();
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

/*
 * The function sets up the environment for the script by creating an array of environment variables
 * and setting the appropriate values.
 * It also creates an array of arguments for the script, including the path to the interpreter and the path to the script itself.
 * The function redirects the standard output to the _response_fd file descriptor and duplicates
 * the request data to the standard input using the dup_request_to_stdin function.
 * It also changes the current working directory to the directory of the CGI script if specified in the request.
 * Finally, executes the script using the execve system call.
 * If the call fails, the child process exits with an error.
 * */
void	CGI::child_process(const request_handler &request) {
	char *environment[request.env.size() + 1];
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

/*
 * The function is used to redirect the HTTP request data to the standard input of the CGI script or program.
 * The function creates a temporary file descriptor using the tmp_fd() function and stores it in the fd variable.
 * It writes the HTTP request data from the request.query object to the file descriptor using the write() function.
 * The function then sets the file position to the beginning of the file using the lseek() function.
 * It duplicates the file descriptor to the standard input file descriptor using the dup2() function, and closes the fd.
 * If all operations succeed, the function returns EXIT_SUCCESS.
 * */
int CGI::dup_request_to_stdin(const request_handler& request) {
    int fd = tmp_fd();

	if (fd < 0 || write(fd, request.query.c_str(), request.query.length()) < 0)
       return EXIT_FAILURE;
    lseek(fd, 0, SEEK_SET);
	dup2(fd, STDIN_FILENO);
    close(fd);
	return EXIT_SUCCESS;
}

/*
 * The function is used to wait for the child process created by create_response to complete, and then check the status of the child process.
 * It calls the waitpid system call with a process ID of -1 to wait for any child process to complete.
 * If the child process terminated normally, the function checks the exit status using the WIFEXITED and WEXITSTATUS macros.
 * If the exit status is non-zero, an error is returned indicating that the execve function failed.
 * If the child process was terminated by a signal, the function checks this using the WIFSIGNALED macro,
 * and returns an error indicating that the child process was interrupted by a signal
 * Finally, if the child process completed successfully, the function returns EXIT_SUCCESS.
 * */
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

/*
 * The function is used to write the response data from the _response_fd file descriptor to the response parameter.
 * It sets the file descriptor's offset to the beginning of the file using lseek.
 * Then it reads data from the file descriptor using the read() function in a loop until there is no more data to read.
 * If an error occurs during the read, the function returns EXIT_FAILURE.
 * Otherwise, the function appends the data read from the file descriptor to the response parameter as a string.
 * After reading all the data, the function closes the file descriptor and returns EXIT_SUCCESS.
 * */
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

/* Writes a specified error message to CLI and returns with INTERNAL_SERVER_ERROR */
int	CGI::error(const char* message) {
	std::cerr << message << '\n';
	return INTERNAL_SERVER_ERROR;
}
/* Creates a temporary file descriptor */
int CGI::tmp_fd() {
    FILE*   tmp = tmpfile();

    if (tmp == NULL)
        return -1;
    return fileno(tmp);
}