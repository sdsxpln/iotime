#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
	printf("Usage: %s program [arg [arg ...]]\n", argv[0]);
	return 1;
    }
    pid_t pid = fork();
    if (pid == 0) {
	execvp(argv[1], argv+1);
	printf("execvp failed\n");
	exit(-1);
    }
    pid_t pid_io = fork();
    if (pid_io == 0) {
	dup2(2, 1);
	char *args[] = {"iostat", "-p", "sdb", "2"};
	execvp("iostat", args);
	printf("execvp failed\n");
	exit(-1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    kill(pid_io, SIGINT);
    int blarg = 0;
    waitpid(pid_io, &blarg, 0);
    return status;
}
// vim:set sw=4 ts=8:
