#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigchild_handler(int signum) {
    // 处理 SIGCHLD 信号
    int status;
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    int pid;
     while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        //确保deletejob原子操作
       
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
     printf("原子删除\n");
     sigprocmask(SIG_SETMASK, &prev_all, NULL);
     }
      printf("over111\n");
     exit(0);
}

int main() {
       sigset_t mask_all, mask_one, prev_one;
    sigfillset(&mask_all);

    //maskone 只对SIGCHLD
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGCHLD);
    pid_t pid;

    // 设置 SIGCHLD 信号处理程序
    signal(SIGCHLD, sigchild_handler);

    pid = fork(); // 创建子进程

    //父进程阻塞 SIGCHLD
    sigprocmask(SIG_BLOCK, &mask_one, &prev_one); /* Block SIGCHLD */
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        sigprocmask(SIG_SETMASK, &prev_one, NULL);
        printf("子进程开始执行...\n");
        sleep(5); // 子进程睡眠 5 秒
        printf("子进程结束执行\n");
        exit(EXIT_SUCCESS); // 子进程退出
    } else {
        // 父进程
        sigprocmask(SIG_BLOCK, &mask_all, NULL);
        printf("father add job...\n");
        sigprocmask(SIG_SETMASK, &prev_one, NULL); 
        pause(); // 父进程暂停等待 SIGCHLD 信号
        printf("父进程结束\n");
        exit(EXIT_SUCCESS); // 父进程退出
    }

    return 0;
}
