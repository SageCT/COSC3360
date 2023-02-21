#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[25];
        char    readbuffer[80];

        pipe(fd);
        
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
		close(fd[0]);
		close(1);
		dup(fd[1]);
                /* Send "string" through the output side of pipe */
                strcpy(string,"Hello_from_the_child");
		printf("%s",string);
                exit(0);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                close(fd[1]);
		close(0);
		dup(fd[0]);
                /* Read in a string from the pipe */
		scanf("%s",readbuffer);
                printf("Received string: %s\n", readbuffer);
        }
        
        return(0);
}
