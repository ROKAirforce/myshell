#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN_LINE    30
#define LEN_HOSTNAME	30

int main(void)
{
    char commandn[MAX_LEN_LINE];
//    char *args[] = {commandn, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    char hostname[LEN_HOSTNAME + 1];
    memset(hostname, 0x00, sizeof(hostname));
    gethostname(hostname, LEN_HOSTNAME);
    
    while (true) {
        char *s;
        int len;
        
        printf("%s@%s$ ", getpwuid(getuid())->pw_name, hostname);
        s = fgets(commandn, MAX_LEN_LINE, stdin);
        
        char *command = strtok(commandn, ";");      // ; 를 기준으로 문자열을 자름, 포인터 반환

        while (command != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
        {
            char *args[] = {command, NULL};
            if (s == NULL) {
                fprintf(stderr, "fgets failed\n");
                exit(1);
            }
        
            len = strlen(command);
            
            printf("%d\n", len);
            if (command[len - 1] == '\n') {
                command[len - 1] = '\0'; 
            }
            
            printf("[%s]\n", command);
            
            if (strcmp(command, "exit") == 0)   //exit 입력시 shell 종료
                return 0;
          
            pid = fork();
            if (pid < 0) {
                fprintf(stderr, "fork failed\n");
                exit(1);
            } 
            if (pid != 0) {  /* parent */
                cpid = waitpid(pid, &status, 0);
                if (cpid != pid) {
                    fprintf(stderr, "waitpid failed\n");        
                }
                printf("Child process terminated\n");
                if (WIFEXITED(status)) {
                    printf("Exit status is %d\n", WEXITSTATUS(status)); 
                }
            }
            else {  /* child */
                ret = execve(args[0], args, NULL);
                if (ret < 0) {
                    fprintf(stderr, "execve failed\n");   
                    return 1;
                }
            }
            command = strtok(NULL, ";");      // 다음 문자열을 잘라서 포인터를 반환
        }
        
    }
    return 0;
}
