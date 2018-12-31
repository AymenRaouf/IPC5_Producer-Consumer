//
// Created by AymenRaouf on 10/12/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define B1 1


struct sembuf PB1 = {//P(sB1)
        B1,
        -1,
        0,
};

struct sembuf VB1 = {//V(sB1)
        B1,
        1,
        0,
};


int main(int argc, char *argv[]){

    FILE* f1;
    f1 = fopen("f1.txt","a+");
    int n = 10;
    int sem = atoi(argv[2]);
    if(f1 == NULL) exit(-1);
    int adr_mem = atoi(argv[1]);
    char * mem = (char * )shmat(adr_mem, NULL, 0);
    if(mem == NULL) exit(-1);
    int j = 0;
    while(!feof(f1)){//reading from f1 and writing in the shared memory
        //P(sB1)
        semop(sem, &PB1, 1);
        fread(&mem[j],sizeof(char),1,f1);
        j=(j+1)%n;
        semop(sem, &VB1, 1);
    }
    fclose(f1);
    shmdt(mem);
    printf("p1 finished \n");
    return 0;

}