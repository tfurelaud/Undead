#include <stdlib.h>
#include <stdio.h>
#include "game_io.h"
#include "string.h"

/**
 * @brief read_next_line :
 * @param f : a file.
 */

char* read_next_line(FILE * f){
     	char* line =(char*)malloc(10*sizeof(char*));
        if(line==NULL){
            return NULL;
        }
        int start=ftell(f);
        int  i;
        if(fgets(line,50,f)!=NULL){
            i = ftell(f)-start;
            if(line[i-1]=='\n'){
                line[i-1]='\0';
            }
            return line;
        }
       	free(line);
        return NULL;
    }

/**
 * @brief fromCharToInt : convert a char chain in int and place it into a tab.
 * @param char chain.
 * @return tab.
 */

int* fromCharToInt(char* chaine){
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

/**
 * @brief load_game : read informations given in file to create a game.
 * @param filename : a file.
 * @return a game.
 */

game load_game(char* filename){

    FILE* f = fopen(filename,"r");
	if(f==NULL){
        printf("error2\n");
		exit(EXIT_FAILURE);
	}
    char* line=read_next_line(f);
    int* tab=fromCharToInt(line);//on récupère width et height
    int width = tab[0];
    int height = tab[1];
    free(line);//on doit free avant de read_next_line sinon mem leak
	line=read_next_line(f);
    free(tab);
	tab=fromCharToInt(line);//on récupère le nombre requis pour chaque type de monstre
	int nb_vampire=tab[0];
	int nb_ghost=tab[1];
	int nb_zombie=tab[2];
	int nb_spirit=tab[3];
	int** labels=(int **)malloc(NB_DIR*sizeof(int*));
	for (int i=0;i<NB_DIR;i++){
        free(line);
		line=read_next_line(f);
        free(tab);
		tab=fromCharToInt(line);//on récupère les labels
		if(i==0||i==1){
			labels[i]=(int*)malloc(sizeof(int)*width);
			for(int j=0;j<width;j++){
				labels[i][j]=tab[j];
			}
		}
		if(i==2||i==3){
			labels[i]=(int*)malloc(sizeof(int)*height);
			for(int j=0;j<height;j++){
				labels[i][j]=tab[j];
			}
		}
	}
    content * board=(content *) malloc((width*height)*sizeof(content));
    for(int i=height-1;i>=0;i--){
        free(line);
		line=read_next_line(f);
		for(int j=0;j<width;j++){
            if(line[j*2] == '/'){
                board[i*width +j]=MIRROR;}
            else if(line[j*2]=='\\'){
                board[i*width +j]=ANTIMIRROR;}
            else if(line[j*2]=='-'){
                board[i*width +j]=HMIRROR;}
            else if(line[j*2]=='|'){
                board[i*width +j]=VMIRROR;}
            else if(line[j*2]=='.'){
                board[i*width +j]=EMPTY;}
            else if(line[j*2]=='V'){
                board[i*width +j]=VAMPIRE;}
            else if(line[j*2]=='Z'){
                board[i*width +j]=ZOMBIE;}
            else if(line[j*2]=='S'){
                board[i*width +j]=SPIRIT;}
            else if(line[j*2]=='G'){
                board[i*width +j]=GHOST;}
		}
    }
	game g=setup_new_game_ext(width, height, labels, board, nb_ghost, nb_vampire, nb_zombie, nb_spirit);
	for(int i=0;i<NB_DIR;i++){
		free(labels[i]);
	}
    free(labels);
    free(board);
	fclose(f);
    free(line);
    free(tab);
	return g;
}

/**
 * @brief whatContent : what is in the case.
 * @param type
 * @return a char.
 */

char whatContent(content type){
	if(type==VAMPIRE)
		return 'V';
	if(type==SPIRIT)
		return 'S';
	if(type==GHOST)
		return 'G';
	if(type==ZOMBIE)
		return 'Z';
	if(type==MIRROR)
		return '/';
	if(type==ANTIMIRROR)
		return '\\';
	if(type==HMIRROR)
		return '-';
	if(type==VMIRROR)
		return '|';
	if(type==ZOMBIE)
		return 'Z';
	if(type==SPIRIT)
		return 'S';
	if(type==GHOST)
		return 'G';
	if(type==VAMPIRE)
		return 'V';
	if(type==EMPTY)
		return '.';
	exit(EXIT_FAILURE);
}

/**
 * @brief save_game
 * @param g
 * @param filename
 */

void save_game(cgame g, char* filename){
	FILE* f=fopen(filename,"w");
	if(f==NULL){
        printf("errore\n");
		exit(EXIT_FAILURE);
	}
	int width=game_width(g);
	int height=game_height(g);
    fprintf(f,"%d %d\n",width,height);
	int spirit=required_nb_monsters(g,SPIRIT);
	int zombie=required_nb_monsters(g,ZOMBIE);
	int ghost=required_nb_monsters(g,GHOST);
	int vampire=required_nb_monsters(g,VAMPIRE);
    fprintf(f,"%d %d %d %d\n",vampire,ghost,zombie,spirit);

    for (int i=0;i<NB_DIR;i++){//labels
		if(i==0||i==1){
			for(int j=0;j<width;j++){
                fprintf(f,"%d",required_nb_seen(g,i,j));
                if(j<width-1){
                    fprintf(f," ");
                }
			}
            fprintf(f,"\n");
        }
		if(i==2||i==3){
            for(int j=0;j<height;j++){
                fprintf(f,"%d",required_nb_seen(g,i,j));
                if(j<height-1){
                    fprintf(f," ");
                }
            }
            fprintf(f,"\n");
        }
    }
    for(int i=height-1;i>=0;i--){
		for(int j=0;j<width;j++){
            if(j!=0){
                fprintf(f," ");
            }
            fprintf(f,"%c",whatContent(get_content(g,j,i)));

		}
        fprintf(f,"\n");
    }
    fclose(f);
    printf("fichier de sauvegarde %s créé.\n",filename);
}
