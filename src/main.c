//
// Created by AymenRaouf on 10/12/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define B1 1
#define B2 1
#define LEN 12

int sem1, sem2;

int main() {

    //creating shared memory
    key_t cle1 = ftok("/home/.",'A');
    key_t cle2 = ftok("/home/.",'B');

    if(cle1 == -1 || cle2 == -1) exit(-1);
    int adr_mem1, adr_mem2, taille = 4096;
    adr_mem1 = shmget(cle1, taille, IPC_CREAT | 0666);
    adr_mem2 = shmget(cle2, taille, IPC_CREAT | 0666);
    if(adr_mem1 == -1 || adr_mem2 == -1){
        exit(-3);
    }

    char adr_string1[LEN], adr_string2[LEN];
    sprintf(adr_string1, "%d", adr_mem1);
    sprintf(adr_string2, "%d", adr_mem2);
    int *mem1, *mem2;
    mem1 = (int *)shmat(adr_mem1,NULL,0);
    mem2 = (int *)shmat(adr_mem2,NULL,0);
    if(mem1 == NULL || mem2 == NULL){
        exit(-4);
    }
    //=======
    //creating semaphores
    sem1 = semget(cle1, 1, IPC_CREAT|0666);
    sem2 = semget(cle2, 1, IPC_CREAT|0666);
    if(sem1 == -1 || sem2 == -1){
        return (-5);
    }
    char sem_string1[LEN], sem_string2[LEN];
    sprintf(sem_string1, "%d", sem1);
    sprintf(sem_string2, "%d", sem2);
    semctl(sem1, B1, SETVAL, 1);
    semctl(sem2, B2, SETVAL, 1);
    //=======

    //ccreating other processes and using exec function to load the producers and the consumer
    char *args1[]={"./p1", adr_string1, sem_string1, NULL};//for the producer p1
    char *args2[]={"./p2", adr_string2, sem_string2, NULL};//for the consumer p2
    char *args3[]={"./p3", adr_string1, sem_string1, adr_string2, sem_string2, NULL};
    int p1,p2,p3;
    p1 = fork();
    if(p1 == 0){//first son
        execvp(args1[0], args1);//program p1
    }
    else{
        p2 = fork();
        if(p2 == 0){//second son
            execvp(args2[0], args2);//program p2
        }
        else{
            p3 = fork();
            if(p3 == 0){//third son
                execvp(args3[0], args3);//program p3
            }
        }
    }

    //========

    //waiting for the processes to finish
    while(wait(NULL)!=-1);
    //========

    //liberer la memoire
    shmdt(mem1);
    shmdt(mem2);
    shmctl(adr_mem1, IPC_RMID, 0);
    shmctl(adr_mem2, IPC_RMID, 0);
    //========

    printf("main finished \n");

    return 0;
}