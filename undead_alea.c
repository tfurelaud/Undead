#include <stdio.h>
#include <stdlib.h>
#include "game_io.h"
#include "toolbox.h"
#include "string.h"

int* fromcharToInt(char* chaine){
       	int* conversion=(int*)malloc(sizeof(int)*10);
        if(conversion==NULL){

            return NULL;
        }
        char* a=strtok(chaine," ");
	int i=0;
        while(a!=NULL){
        	int b=strtol(a,&a,10);
		if(*a=='\0'){
             	conversion[i]=b;
                i++;
            }else{
               	free(conversion);
                return NULL;
            }
            a=strtok(NULL," ");
        }
        return conversion;
    }

int main(int argc, char* argv[]){
	if(argc!=3){
        	printf("Erreur de synthaxe :/\n");
        	printf("voici le paterne: ./undead_alea <width> <height>\n");
        	exit(EXIT_FAILURE);
    	}
	int* width=fromcharToInt(argv[1]);
	int* height=fromcharToInt(argv[2]);
	game g=random_game(width[0],height[0]);
	save_game(g,"alea");
	return 0;
}
	

