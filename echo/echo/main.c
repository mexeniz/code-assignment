//
//  main.c
//  echo
//
//  Created by pawissakan.ch on 6/10/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef void (*myEcho)(char * str);

void echo1(char *str){
    printf("echo1:%s\n",str);
}
void echo2(char *str){
    printf("echo2:%s\n",str);
}


int main(int argc, const char * argv[]) {
    
    myEcho echo;
    
    //Test Echo 1
    printf("Test Echo 1: ");
    echo = &echo1 ;
    char* str = (char*) malloc(sizeof(char)*50);
    scanf("%s",str);
    echo(str);
    
    //Test Echo 2
    printf("Test Echo 2: ");
    echo = &echo2 ;
    scanf("%s",str);
    echo(str);
    
    
    return 0;
}
