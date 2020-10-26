#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "game.h"

/**
 * @brief rand_a_b : fonction which is giving a random number between a and b.
 * @param a an int.
 * @param b an int.
 * @param length
 * @return
 */

int* rand_a_b(int a,int b,int length){
    int i = 0;
    int* nombre_aleatoire=(int*)malloc(sizeof(int)*length);
    srand(time(NULL)); // initialisation de rand
    for(i=0; i<length; i++){
        nombre_aleatoire[i] = rand()%(b-a)+a;
    }
    return nombre_aleatoire;
}

void rand_list(content * t){
    int z[4];
    int pzi[4];
    int* nb_aleat=rand_a_b(0,80,4);
    int cmp=0;
    int size=4;
    int a=0; //pour z
    int b=0;//pour pzi
    for(int i=0;i<size;i++){
        if(t[i]==-1){
            pzi[b]=-1;
            b++;
        }else{
            z[a]=t[i];
            a++;
        }
    }
    for(int i=0;i<a;i++){
        cmp=z[i];
        z[i]=z[nb_aleat[i]%a];
        z[nb_aleat[i]%a]=cmp;
    }
    for(int i=0;i<b;i++){
        z[a+i]=pzi[i];
    }
    free(nb_aleat);
    for(int i=0;i<size;i++){
        t[i]=z[i];
    }
}
/**
 * @brief random_game : return a random game with random paramters expected height and width.
 * @param width : the width of the random game.
 * @param height :  tes height of the random game.
 * @return a game.
 */

game random_game (int width, int height){
    	struct game_s * g;
	g = new_game_ext(width,height);
	if(g==NULL)
		exit(EXIT_FAILURE);
	int ghost = 0;
	int zombie = 0;
	int vampire = 0;
	int spirit = 0;
        int* x = rand_a_b(1,8,width*height);
	for (int i=0; i<height;i++){
		for(int j=0; j<width; j++){
                    // Quivant les nombres tiré on met quelques chose dans la case [i],[j].
			if(x[i*height+j]==1){
				add_monster(g,GHOST,j,i);
				ghost ++;
			}
			if(x[i*height+j]==2){
				add_monster(g,ZOMBIE,j,i);
				zombie ++;
			}
			if(x[i*height+j]==3){
				add_monster(g,VAMPIRE,j,i);
				vampire ++;
			}
			if(x[i*height+j]==4){
				add_monster(g,SPIRIT,j,i);
				spirit ++;
			}
			if(x[i*height+j]==5){
				add_mirror_ext(g,MIRROR,j,i);
				}
			if(x[i*height+j]==6){
				add_mirror_ext(g,ANTIMIRROR,j,i);
				}
			if(x[i*height+j]==7){
				add_mirror_ext(g,HMIRROR,j,i);
			}
			if(x[i*height+j]==8){
				add_mirror_ext(g,VMIRROR,j,i);
			}
		}
	}
        free(x);
	set_required_nb_monsters(g,GHOST,ghost);
	set_required_nb_monsters(g,VAMPIRE,vampire);
	set_required_nb_monsters(g,ZOMBIE,zombie);
	set_required_nb_monsters(g,SPIRIT,spirit);
	for (int i=0; i<4 ;i++){
		if(i==0){
			for(int j=0;j<width;j++){
				set_required_nb_seen(g,N,j,current_nb_seen(g,N,j));
			}
		}
		if(i==1){
			for(int j=0;j<width;j++){
				set_required_nb_seen(g,S,j,current_nb_seen(g,S,j));
			}
		}			
		if(i==2){
			for(int j=0;j<height;j++){
				set_required_nb_seen(g,E,j,current_nb_seen(g,E,j));
			}
		}
		if(i==3){
			for(int j=0;j<height;j++){
				set_required_nb_seen(g,W,j,current_nb_seen(g,W,j));
			}
		}
	}
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
                        if(get_content(g,j,i)==ZOMBIE||get_content(g,j,i)==VAMPIRE||get_content(g,j,i)==SPIRIT||get_content(g,j,i)==GHOST){ // Si la case contient un monstre alors on remplace le monstre par EMPTY.
				add_monster(g,EMPTY,j,i);
			}
		}
	}
	return g;
}
		

