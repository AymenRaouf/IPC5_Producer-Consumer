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
#define B2 1

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

    FILE* f3;
    int sem1 = atoi(argv[2]);
    int sem2 = atoi(argv[4]);
    int adr_mem1 = atoi(argv[1]);
    int adr_mem2 = atoi(argv[3]);
    f3 = fopen("f3.txt","w+");
    if(f3 == NULL) exit(-1);
    char * mem1 = (char*)shmat(adr_mem1, NULL, 0);
    char * mem2 = (char*)shmat(adr_mem2, NULL, 0);
    int i = 0, j = 0, k = 0;
    while(i<1){
        semop(sem1, &PB1, 1);
        fwrite(&mem1[k], sizeof(char),1,f3);
        puts(&mem1[k]);
        k = ( k + 1 ) % 10;
        semop(sem1, &VB1, 1);
        semop(sem2, &PB2, 1);
        fwrite(&mem2[j], sizeof(char),1,f3);
        puts(&mem2[j]);
        j = ( j + 1 ) % 10;
        semop(sem2, &VB2, 1);
        i++;
    }
    fclose(f3);
    shmdt(mem1);
    shmdt(mem2);
    printf("p3 finished \n");
    return 0;

}