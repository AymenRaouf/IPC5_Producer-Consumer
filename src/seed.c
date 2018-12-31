//
// Created by AymenRaouf on 10/12/18.
//

#include <stdio.h>
#include <stdlib.h>


int main(){
    FILE *f1, *f2;
    f1 = fopen("f1.txt","w+");
    f2 = fopen("f2.txt","w+");
    int i = 0;
    char a1 = 'a', a2 = 'b';

    while (i<100){
        fwrite(&a1, sizeof(char),1,f1);
        fwrite(&a2, sizeof(char),1,f2);
        i++;
    }

    fclose(f1);
    fclose(f2);

    return 0;
}