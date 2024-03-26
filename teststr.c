#include <stdio.h>

int main() {
    char *argv[] = {"bg", NULL};
     char *job_id_str = argv[1];
     if (job_id_str[0]!='%')
     {
            printf("111");
     }
     

    return 0;
}
