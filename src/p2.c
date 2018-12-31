//
// Created by AymenRaouf on 10/12/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define B2 1


struct sembuf PB2 = {//P(sB2)
        B2,
        -1,
        0,
};

struct sembuf VB2 = {//V(sB2)
        B2,
        1,
        0,
};


int main(int argc, char *argv[]){

    FILE* f2;
    f2 = fopen("f2.txt","a+");
    int n = 10;
    int sem = atoi(argv[2]);
    if(f2 == NULL) exit(-1);
    int adr_mem = atoi(argv[1]);
    char * mem = (char * )shmat(adr_mem, NULL, 0);
    if(mem == NULL) exit(-1);
    int j = 0;
    while(!feof(f2)){//reading from f2 and writing in the shared memory
        //P(sB2)
        semop(sem, &PB2, 1);
        fread(&mem[j],sizeof(char),1,f2);
        j=(j+1)%n;
        semop(sem, &VB2, 1);
    }
    fclose(f2);
    shmdt(mem);
    printf("p2 finished \n");
    return 0;

}