#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fds[2];
    pipe(fds);
    pid_t pid1 = fork();
    if (pid1 < 0) {
        return 1;
    } else if (pid1 > 0) {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            return 1;
        } else if (pid2 > 0) {
            close(fds[0]);
            char buffer[1024];
            sprintf(buffer, "%i", pid2);
            write(fds[1], buffer, 1024);
            int status;
            while (waitpid(-1, &status, 0) > 0);
        } else {
            close(fds[0]);
            close(fds[1]);
            int i = 0;
            while (true) {
                printf("%i \n", i++);
                fflush(stdout);
                sleep(1);
            }
        }
    } else {
        close(fds[1]);
        char buffer[1024];
        while (read(fds[0], buffer, 1024) <= 0);
        sleep(10);
        pid_t pid = atoi(buffer);
        kill(pid, SIGTERM);
    }

    return 0;
}