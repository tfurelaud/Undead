#include <stdio.h>
#include <stdlib.h>
#include "game.h"

typedef short unsigned int suint;

/*The structure of a game*/
struct game_s{
    int game_width;
    int game_height;
    int nb_ghost;
    int nb_vampire;
    int nb_zombie;
    int nb_spirit;
    content ** board;
    int ** labels;
};



game new_game() {
    return new_game_ext(4,4);//use old parameters on the new function for retrocompatibility
}

game setup_new_game(int **labels, content * board, int required_nb_ghosts, int required_nb_vampires, int required_nb_zombies){
    return setup_new_game_ext(4,4,labels,board,required_nb_ghosts,required_nb_vampires,required_nb_zombies,0);//use old parameters on the new function for retrocompatibility
}

void add_mirror(game game, int dir, int col, int line){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    switch(dir){
    case 0:add_mirror_ext(game,MIRROR,col,line);break;
    case 1:add_mirror_ext(game,ANTIMIRROR,col,line);break;

    }
}

void add_mirror_ext(game game, content mirror_type, int col, int line){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    game->board[col][line]=mirror_type;
}

void set_required_nb_seen(game game, direction side, int pos, int value){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    (*game).labels[side][pos]=value;
}

void set_required_nb_monsters(game game, content monster, int value){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    switch (monster){
    case ZOMBIE : game->nb_zombie = value;break;
    case GHOST : game->nb_ghost = value;break;
    case VAMPIRE : game->nb_vampire = value;break;
    case SPIRIT : game ->nb_spirit = value;break;
    default:exit(EXIT_FAILURE);fprintf(stderr,"unexpected content\n");break;
    }
}

game copy_game (cgame g_src){
    if(g_src == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    game g = new_game_ext(game_width(g_src),game_height(g_src));
    for(int i=0;i<NB_DIR;i++){//copy all the labels to the copy
        if(i<2){
            for(int j=0;j<game_width(g);j++){
                (*g).labels[i][j]= g_src->labels[i][j];
            }
        }
        else{
            for(int j=0;j<game_height(g);j++){
                (*g).labels[i][j]= g_src->labels[i][j];
            }
        }

    }
    for(int i=0;i<game_width(g);i++){//copy all the content of the board to the copy
        for(int j=0;j<game_height(g);j++){
            (*g).board[i][j]=g_src->board[i][j];
        }
    }
    (*g).game_height=g_src->game_height;//we've just copyed each cell of the game g_src into an empty game g
    (*g).game_width=g_src->game_width;
    (*g).nb_ghost=g_src->nb_ghost;
    (*g).nb_vampire=g_src->nb_vampire;
    (*g).nb_zombie=g_src->nb_zombie;
    (*g).nb_spirit=g_src->nb_spirit;

    return g;

}

void delete_game (game g){
    if(g == NULL){
        fprintf(stderr,"pointeur dejà null\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<game_width(g);i++){
        free (((*g).board)[i]);//we free in first all elements of the g->board
    }
    for(int i =0; i<NB_DIR;i++){
        free (((*g).labels)[i]);//then all elements of the g->labels
    }
    free ((*g).board);//and finally we free the double pointers of board and labels
    free ((*g).labels);
    free (g);
}

int required_nb_seen(cgame game, direction side, int pos){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    return ((*game).labels)[side][pos];
}

content get_content(cgame game, int col, int line){
    if(game==NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    return game->board[col][line];
}

int required_nb_monsters(cgame game, content monster){
    if(game==NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    switch(monster){
    case ZOMBIE : return game->nb_zombie;break;
    case VAMPIRE: return game->nb_vampire;break;
    case GHOST  : return game->nb_ghost;break;
    case SPIRIT : return game->nb_spirit;break;
    default:return -1;break;
    }
}

bool is_game_over (cgame g){
    if(g==NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    if (current_nb_monsters(g,ZOMBIE)!= required_nb_monsters(g,ZOMBIE) ||current_nb_monsters(g,VAMPIRE)!= required_nb_monsters(g,VAMPIRE) ||current_nb_monsters(g,GHOST)!= required_nb_monsters(g,GHOST) || current_nb_monsters(g,SPIRIT)!= required_nb_monsters(g,SPIRIT)){
        return false;
    }
    bool res = true ;
    for(int i = 0;i<NB_DIR;i++){
        if(i<2){
            for(int j=0;j<(g->game_width);j++){
                if(g->labels[i][j]!=current_nb_seen(g,i,j)){//we test if at each position [i][j] if the current_nb_seen is equal to the label
                    return false;
                }
            }
        }else{
            for(int j=0;j<(g->game_height);j++){
                if(g->labels[i][j]!=current_nb_seen(g,i,j)){//we test if at each position [i][j] if the current_nb_seen is equal to the label
                    return false;
                }
            }
        }

    }

    return res;
}

void restart_game(game g){
    if(g == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    content k;
    for(int i=0;i<g->game_width;i++){
        for(int j=0;j<g->game_height;j++){
            k=get_content(g,i,j);
            if(k!=MIRROR && k!= ANTIMIRROR && k != VMIRROR && k != HMIRROR)
                g->board[i][j]=EMPTY;//we set every cells of the board to empty, excluding all different mirror
        }
    }
}

bool add_monster(game game, content monster, int col, int line){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    content k = get_content(game,col,line);
    if(k!=MIRROR && k != ANTIMIRROR && k != VMIRROR && k != HMIRROR){
        if((*game).board[col][line]!=monster){
        (*game).board[col][line]=monster;
        }
        else{
            (*game).board[col][line]=EMPTY;
        }
        return true;
    }
    else{return false;}
}

/** This function is used in current_nb_seen to tell what to do in case of a mirror encounter **
 * It takes in parameter pointer vx , pointer vy , the type of mirror, and a pointer seenMirror
 * it modify vx vy and seenMirror, given the mirrortype and vx and vy themselves
 * **/
void mirrorEncounter(short int * vx,short int * vy, content mirrorType,bool * seenMirror){
    if(mirrorType==MIRROR){
        *seenMirror=true;
        if(*vx!=0){
            *vy=*vx;
            *vx=0;
        }else{
            *vx=*vy;
            *vy=0;
        }
    }else if(mirrorType==ANTIMIRROR){
        *seenMirror=true;
        if(*vx!=0){

            *vy=-(*vx);
            *vx=0;
        }else{
            *vx=-(*vy);
            *vy=0;
        }
    }else if(mirrorType==HMIRROR){
        if(*vx==0){
            *seenMirror=true;
        }
        if(vy!=0){
            *vy=-(*vy);
        }
    }else if(mirrorType==VMIRROR){
        if(*vy==0){
            *seenMirror=true;
        }
        if(*vx!=0){
            *vx=-(*vx);
        }
    }

}


int current_nb_seen(cgame game, direction side, int pos){
    if(game == NULL){/**test if pointer given is null**/
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    short int nb_seen = 0;//counts the number of monters seen, will be returned later

    //to count the number of monster seen from a position, we go through the grid, from a starting position (x & y), until we got out of it
    short int x=0;// x and y will be our coordinates into the grid
    short int y=0;// "
    bool * seenMirror=(bool*)malloc(sizeof(bool)); //current state of the light ray, to count either vampires or ghosts
    *seenMirror=false;
    short int * vy = (short int *)malloc(sizeof(short int));	// vx and vy will be the vectors allowing us to move into the grid, changing if a mirror is encountered
    short int * vx = (short int *)malloc(sizeof(short int));
    *vx=0;
    *vy=0;
    /*switch initializing x and y following the given position and side*/
    switch (side){
        case N : y=game_height(game)-1;x=pos;*vy=-1;break;
        case S : y=0;x=pos;*vy=1;break;
        case E : x=game_width(game)-1;y=pos;*vx=-1;break;
        case W : x=0;y=pos;*vx=1;break;
        default : exit(EXIT_FAILURE);break;
    }
    content caseGame = EMPTY;//caseGame will contain the content at the current position for comparing purpose
    /*the loop allowing us to go through the grid until we exede its limits*/
    while((x>-1 && x<(game->game_width))&&(y>-1 && y < (game->game_height))){
        caseGame = get_content(game,x,y);//catch the current content
        switch (caseGame) {//to process what to do given the diferent kind  of content encontered
            case MIRROR :mirrorEncounter(vx,vy,MIRROR,seenMirror);break;
            case ANTIMIRROR : mirrorEncounter(vx,vy,ANTIMIRROR,seenMirror);break;
            case HMIRROR : mirrorEncounter(vx,vy,HMIRROR,seenMirror); break;
            case VMIRROR :   mirrorEncounter(vx,vy,VMIRROR,seenMirror); break;
            case EMPTY :break;
            case ZOMBIE: nb_seen++;break;
            case GHOST:if(*seenMirror==true){nb_seen++;};break;
            case VAMPIRE:if(*seenMirror==false){nb_seen++;};break;
            case SPIRIT :break;
        }
        x += *vx;//we update the positions x and y according to the vectors vx and vy
        y += *vy;//
    }
    free(seenMirror);
    free(vx);
    free(vy);
    return nb_seen;//we return the number of monsters encountered

}

int current_nb_monsters(cgame game, content monster){
    if(game == NULL){
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    //we simply go through the board with a double loop, checking for monsters.
    int count = 0;
    for(int i=0;i<game->game_width;i++){
        for(int j=0;j<game->game_height;j++){
            if (get_content(game,i,j)==monster)
                count++;
        }
    }

    return count;
}

//*

game new_game_ext(int width, int height){
    struct game_s * g;
    g=(struct game_s *)malloc(sizeof(struct game_s));//allocating and initializing all elements of the structure
    (*g).game_height=height;
    (*g).game_width=width;
    (*g).nb_ghost=0;
    (*g).nb_vampire=0;
    (*g).nb_zombie=0;
    (*g).nb_spirit=0;
    (*g).labels=(int **)malloc(NB_DIR*sizeof(int*));
    (*g).board=(content **)malloc((g->game_width)*sizeof(content *));//we allocate memory for board

    for(int i=0;i<NB_DIR;i++){
        if(i<2){
            (*g).labels[i]=(int *)malloc((g->game_width)*sizeof(int));
            for(int j=0;j<(g->game_width);j++){
                (*g).labels[i][j]=0;
            }
        }
        else{
            (*g).labels[i]=(int *)malloc((g->game_height)*sizeof(int));
            for(int j=0;j<(g->game_height);j++){
                (*g).labels[i][j]=0;
            }
        }
    }
    for(int i=0;i<(g->game_width);i++){
        (*g).board[i]=(content *)malloc((g->game_height)*sizeof(content));
        for(int j=0;j<(g->game_height);j++){
            (*g).board[i][j]=0;
        }
    }
    if(g==NULL){
        fprintf(stderr,"not enough memory\n");
        exit(EXIT_FAILURE);
    }
    return g;
}

/**
 * @brief setup_new_game_ext :
 * @param is the width of the game grid
 * @param is the height of the game grid
 * @param the tab that containt the actual labels of the game.
 * @param pointeur to the tab that containt the actual content of the game.
 * @param number of ghosts required.
 * @param number of vampires required.
 * @param number of zombies required.
 * @param number of spirits required.
 * @return the game.
 */

game setup_new_game_ext(int width, int height, int *labels[], content * board,int required_nb_ghosts, int required_nb_vampires, int required_nb_zombies, int required_nb_spirits){
    if( board==NULL || labels==NULL){/**test if pointer given is null**/
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    game g = new_game_ext(width,height);
    for(int i=0;i<NB_DIR;i++){

        if(i<2){
            for(int j=0;j<width;j++){
                g->labels[i][j]=labels[i][j];
            }
        }

        else{
            for(int j=0;j<height;j++){
                g->labels[i][j]=labels[i][j];
            }
        }
    }
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            g->board[j][i]=board[(i*width)+j];
        }
    }
    (*g).game_height=height;
    (*g).game_width=width;
    (*g).nb_ghost=required_nb_ghosts;
    (*g).nb_vampire=required_nb_vampires;
    (*g).nb_zombie=required_nb_zombies;
    (*g).nb_spirit=required_nb_spirits;

    return g;
}

/**
 * @brief game_height : the height of the game grid
 * @param a game g
 * @return the height of the grid.
 */

int game_height(cgame g){
    if(g == NULL){/**test if pointer given is null**/
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    return g->game_height;
}

/**
 * @brief game_width : the width of the game grid.
 * @param a game g.
 * @return the width of the grid.
 */

int game_width(cgame g){
    if(g == NULL){/**test if pointer given is null**/
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    return g->game_width;
}
