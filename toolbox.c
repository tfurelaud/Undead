#include "game.h"
#include "game_io.h"
#include "toolbox.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


game Initialisation(void){
    /*
    int nb_ghosts = 2;
    int nb_vampires = 3;
    int nb_zombies = 5;
    int nb_spirits = 3;
    content board []={EMPTY, VMIRROR, EMPTY, EMPTY, EMPTY, MIRROR, EMPTY, VMIRROR, EMPTY, ANTIMIRROR, EMPTY, EMPTY, HMIRROR, EMPTY, EMPTY, EMPTY, MIRROR, EMPTY, ANTIMIRROR, EMPTY}; //Remplissage de toutes les cases du tableau de jeu.
    int North_labels [] = {2, 1, 1, 1, 1};
    int South_labels [] = {4, 1, 2, 4, 2};
    int East_labels [] = {3, 1, 1, 1};
    int West_labels [] = {2, 1, 1, 1};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};

    game Game = setup_new_game_ext(5,4,labels, board, nb_ghosts, nb_vampires ,nb_zombies,nb_spirits);*/
    game Game = random_game(4,4);
    //game Game = load_game("putain");

    return(Game);
}


game File(int argc, char* argv[]){
    game Game;
    if(argc==1){
        Game = Initialisation();
    }
    else if(argc==2){
        printf("zuqdqjbd \n");
        Game = load_game(argv[1]);

    }
    else{
        fprintf(stderr,"Too much arguments given");
        exit(EXIT_FAILURE);
    }
    return(Game);
}

void printSpaces(cgame g){
    for(int i = 4;i<game_width(g);i++){
        printf(" ");
    }
}

int display(cgame g){
    int height = game_height(g);
    int width = game_width(g);
    /*ligne 1*/
    printf("| ");
    printSpaces(g);
    printf("Z:%d V:%d G:%d S:%d ",required_nb_monsters(g,ZOMBIE),required_nb_monsters(g,VAMPIRE),required_nb_monsters(g,GHOST),required_nb_monsters(g,SPIRIT));
    printSpaces(g);
    printf("|\n");
    /*ligne2*/
    printf("|                 ");
    printSpaces(g);
    printf(" |\n");
    /*ligne3*/
    printf("|     ");
    for(int i = 0; i<game_width(g);i++){
        printf("%d ",required_nb_seen(g,N,i));
    }
    printf("    |\n");
    /*ligne4*/
    printf("|                  ");
    printSpaces(g);
    printf("|\n");
    /*ligne 5 à ligne (height-4)*/
    for(int i = (height-1); i>=0; i=i-1){
        printf("|  %d  ", required_nb_seen(g, W, i));
        for( int j = 0; j<width; j++){

            if (get_content(g,j,i)==EMPTY){
                printf("  ");
            }
            else if (get_content(g,j,i)==MIRROR){
                printf("/ ");
            }
            else if (get_content(g,j,i)==ANTIMIRROR){
                printf("\\ ");
            }
            else if (get_content(g,j,i)==ZOMBIE){
                printf("Z ");
            }
            else if (get_content(g,j,i)==SPIRIT){
                printf("S ");
            }
            else if (get_content(g,j,i)==VAMPIRE){
                printf("V ");
            }
            else if (get_content(g,j,i)==GHOST){
                printf("G ");
            }
            else if (get_content(g,j,i)==VMIRROR){
                printf("| ");
            }
            else if (get_content(g,j,i)==HMIRROR){
                printf("- ");
            }

        }
        printf(" %d  |\n", required_nb_seen(g, E, i));
    }
    /*ligne n-3*/
    printf("|                  ");
    printSpaces(g);
    printf("|\n");
    /*ligne n-2*/
    printf("|     ");
    for(int i = 0; i<game_width(g);i++){
        printf("%d ",required_nb_seen(g,S,i));
    }
    printf("    |\n");
    /*ligne n-1*/
    printf("|                  ");
    printSpaces(g);
    printf("|\n");
    /*ligne n*/
    printf("|");
    printSpaces(g);
           printf("      start      ");
    printSpaces(g);
   printf("|\n\n\n\n");
    return EXIT_SUCCESS;
}

void clearBuffer(void) {
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
}

bool isOkay(int x,int y,cgame g){
    if((x>=game_width(g) || y>=game_height(g)||(x<0 || y<0))){
        return false;
    }
    else{
        return true;
    }

}

/** fonctions du solveur **/

bool is_mirror(content contenu){
    return (contenu==MIRROR || contenu==ANTIMIRROR || contenu==HMIRROR || contenu==VMIRROR);
}

char* concat(const char *s1, const char *s2){
    char *result =(char*) malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char * name_generator(char * prefixe, int n){
    char *v1 = concat(prefixe,".sol");

    if(n!=0){
        char str[16];
        sprintf(str, "%d", n);
        char*v2=v1;
        v1=concat(v1,str);
        free(v2);
    }
    return v1;
}

void shuffle(content * ordre){
    return;
}

void solveur(function * fonction, game g,char * prefix_file ,int * sol_num,unsigned int * pos, bool * drop_it,content *** ordre,int * nbm){
    if(*drop_it){
        display(g);
        return;
    }

    //display(g);
    if(nbm[0]==required_nb_monsters(g,GHOST) && nbm[1]==required_nb_monsters(g,VAMPIRE)&&nbm[2]==required_nb_monsters(g,ZOMBIE)&&nbm[3]==required_nb_monsters(g,SPIRIT)){//is_game over is heavy in complexity (to check all monsters we go through the grid each time
        if(is_game_over(g)){    //we check at each recursion if the game is over

            char * name_file = name_generator(prefix_file,*sol_num);


            switch(*fonction){  //if so, we do acording to the fonctionality,
                case FIND_ONE:*sol_num=*sol_num+1;save_game(g,name_file);*drop_it=true;free(name_file);return;break;   //-->we increment sol_num and save the game and drop out of the recursions
                case NB_SOL:*sol_num=*sol_num+1;free(name_file);return;break;   //-->we just increment sol_num and drop out of the recursions
                case FIND_ALL:*sol_num=*sol_num+1;save_game(g,name_file);free(name_file);return;break; //-->we increment sol_num, save the game and go on.
            }
            free(name_file);
        }
    }
    if(*pos>=(game_height(g)*game_width(g))){   // to be sure if we did not went too far
        return;
    }

    uint x = *pos%game_width(g);//we compute the position x and y for ulterior uses
    uint y = *pos/game_width(g);

    content contenu = get_content(g,x,y);
    if(is_mirror(contenu)){//if the frame we're on is a mirror, we go for an other recursion
            *pos=*pos+1;
            solveur(fonction,g,prefix_file,sol_num,pos,drop_it,ordre,nbm);
            *pos=*pos-1;
            return;
    }

    for(int i =0; i<4;i++){     //now we test all the possibilities in the list (int the matrix) at x and y
        switch(ordre[x][y][i]){
            case ZOMBIE: nbm[2]++;add_monster(g,ZOMBIE,x,y);if(is_valid(g,nbm,ZOMBIE,pos)){*pos=*pos+1;solveur(fonction,g,prefix_file,sol_num,pos,drop_it,ordre,nbm);if(*drop_it){return;};*pos=*pos-1;}add_monster(g,EMPTY,x,y);nbm[2]--;break;
            case VAMPIRE:nbm[1]++;add_monster(g,VAMPIRE,x,y);if (is_valid(g,nbm,VAMPIRE,pos)){*pos=*pos+1;solveur(fonction,g,prefix_file,sol_num,pos,drop_it,ordre,nbm);if(*drop_it){return;};*pos=*pos-1;}add_monster(g,EMPTY,x,y);nbm[1]--;break;
            case GHOST:  nbm[0]++;add_monster(g,GHOST,x,y);if(is_valid(g,nbm,GHOST,pos)){*pos=*pos+1;solveur(fonction,g,prefix_file,sol_num,pos,drop_it,ordre,nbm);if(*drop_it){return;};*pos=*pos-1;}add_monster(g,EMPTY,x,y);nbm[0]--;break;
            case SPIRIT: nbm[3]++;add_monster(g,SPIRIT,x,y);if(is_valid(g,nbm,SPIRIT,pos)){*pos=*pos+1;solveur(fonction,g,prefix_file,sol_num,pos,drop_it,ordre,nbm);if(*drop_it){return;};*pos=*pos-1;}add_monster(g,EMPTY,x,y);nbm[3]--;break;
            default:break;
        }
    }
    return;
}

bool compare_string(const char * string1,const char * string2){
        if( strlen(string1) != strlen(string2) )//just comparing strings
            return false;
        for(int i=0;i<strlen(string1);i++)
            if(string1[i] != string2[i])
                return false;
        return true;
}

function string_to_func(char * string){
    if(compare_string(string,"FIND_ONE")){
        return FIND_ONE;
    }
    else if(compare_string(string,"NB_SOL")){
        return NB_SOL;
    }
    else if(compare_string(string,"FIND_ALL")){
        return FIND_ALL;
    }
    else{
        printf("%s n'existe pas :/\n",string);
        exit(EXIT_FAILURE);
    }
}


/** parties pour un solveur expérimental **/

bool is_valid(game g,int * nbm,content monster,unsigned int * pos){
    switch(monster){//here we check if we did not put a monster over the requirement
        case ZOMBIE : if(nbm[2]>required_nb_monsters(g,ZOMBIE)){return false;}break;
        case VAMPIRE : if(nbm[1]>required_nb_monsters(g,VAMPIRE)){return false;}break;
        case GHOST : if(nbm[0]>required_nb_monsters(g,GHOST)){return false;}break;
        case SPIRIT : if(nbm[3]>required_nb_monsters(g,SPIRIT)){return false;}break;
        default:break;
    }
    for(int i=0;i<NB_DIR;i++){//there we check if the label corresponding to the fram at position pos and direction i, is not over the required_nb_seen
        if(label_corresponding(g,pos,i)==false)
            return false;
    }
    return true;
}

bool label_corresponding(game g,unsigned int * pos,direction dir){
    int x=(*pos)%game_width(g);
    int y=(*pos)/game_height(g);
    int vx =0;
    int vy =0;
    bool fill;
    bool back;
    bool infinite_loop=false;
    content contenu;
    switch(dir){
        case N:vy=1;break;
        case S:vy=-1;break;
        case E:vx=1;break;
        case W:vx=-1;break;
    }
    while(x>=0 && x<game_width(g) && y>=0 && y<game_height(g) && !infinite_loop){// to check if a label is not over the requirement, we have to go to the limits of the game then gess the corresponding label...
        contenu = get_content(g,x,y);
        if(is_mirror(contenu))
            mirrorEncounter2(&vx,&vy,contenu,&fill,&back,&infinite_loop);
        x+=vx;
        y+=vy;
    }
    return return_verif(g,&x,&y);//...with return_verif
}

bool return_verif(game g,int * x,int * y){
    if(*x==-1){//we just verify each case and return the result acording to the correspondant one.
        return required_nb_seen(g,W,*y)>=current_nb_seen(g,W,*y);
    }
    else if(*x==game_width(g)){
        return required_nb_seen(g,E,*y)>=current_nb_seen(g,E,*y);
    }
    else if(*y==game_height(g)){
        return required_nb_seen(g,N,*x)>=current_nb_seen(g,N,*x);
    }
    else if(*y==-1){
        return required_nb_seen(g,S,*x)>=current_nb_seen(g,S,*x);
    }
    return true;
}

void mirrorEncounter2(int * vx, int * vy, content mirrorType,bool * seenMirror,bool * back,bool  * infinite_loop){
    if(mirrorType==MIRROR){//Here we just aply the modifications to the parameters when a mirror is encountered, when it is needed
        *seenMirror=true;
        if(*vx!=0){
            *vy=*vx;
            *vx=0;
        }else{
            *vx=*vy;
            *vy=0;
        }
        return;
    }else if(mirrorType==ANTIMIRROR){
        *seenMirror=true;
        if(*vx!=0){
            *vy=-(*vx);
            *vx=0;
        }else{
            *vx=-(*vy);
            *vy=0;
        }
        return;
    }else if(mirrorType==HMIRROR){
        if(*vx==0){
            if(*back)
                *infinite_loop=true;
            *seenMirror=true;
            *back=true;
            *vy=-(*vy);
        }
        return;
    }else if(mirrorType==VMIRROR){
        if(*vy==0){
            if(*back)
                *infinite_loop=true;
            *back = true;
            *seenMirror=true;
            *vx=-(*vx);
        }
        return;
    }

}

content *** deduction(game g){
    //this initialize the matrix with each list at {ZOMBIE,VAMPIRE,GHOST,SPIRIT}
    content *** tab =(content***)malloc(sizeof(content **)*game_width(g));
    for(int i=0;i<game_width(g);i++){
        tab[i]=(content **) malloc(sizeof(content *)*game_height(g));
        for(int j = 0;j<game_height(g);j++){
            tab[i][j]=(content *)malloc(sizeof(content)*4);
            for(int k=0;k<4;k++){
                switch(k){
                    case 0:tab[i][j][k]=ZOMBIE;break;//by default, all monsters are possible on eachframe
                    case 1:tab[i][j][k]=VAMPIRE;break;
                    case 2:tab[i][j][k]=GHOST;break;
                    case 3:tab[i][j][k]=SPIRIT;break;
                }
            }
            //rand_list(tab[i][j]);//Slows the
        }
    }
    //there we start filtering the content from each position, calling parkour
    int dir=0;
    while(dir<NB_DIR){
        if(dir<2){
            for(int i=0;i<game_width(g);i++){
                parkour(g,dir,i,tab);
            }
        }

        else{
            for(int i=0;i<game_height(g);i++){
                parkour(g,dir,i,tab);
            }
        }

        dir++;
    }
    return tab;
}

//not clearly defined yet
bool label_verification(game g,int pos,direction dir){ //verify a label associated to a specific case, to check if it is possible to put a monster of a specific type (false = no ghosts, true = no vamps
    //printf("i:   %d\n ",dir);
    int x=pos%game_width(g);
    int y=pos/game_height(g);
    int vx =0;
    int vy =0;
    bool fill;
    bool back;
    bool infinite_loop=false;
    content contenu;
    switch(dir){
        case N:vy=1;break;
        case S:vy=-1;break;
        case E:vx=1;break;
        case W:vx=-1;break;
    }
    while(x>=0 && x<game_width(g) && y>=0 && y<game_height(g) && !infinite_loop){
        contenu = get_content(g,x,y);
        if(is_mirror(contenu))
            mirrorEncounter2(&vx,&vy,contenu,&fill,&back,&infinite_loop);
        x+=vx;
        y+=vy;
    }
    return return_verif(g,&x,&y);
}

void apply(content *** grille,int taille,bool back,int nb_seen, int x, int y,bool * seenMirror){
   //given the position x, y and others parameters, we filter the content inside the list in grille

    //this function is used over a list containing positions
    bool can_spirit=true;
    bool can_vampire=true;
    bool can_zombie=true;
    bool can_ghost=true;

    //this is all the conditions listed to filter an element that i have found yet, there is probably more//
    if(*seenMirror==true){
        if(taille==nb_seen){

            can_spirit=false;
            can_vampire=false;

            if(back){
                can_vampire=false;
                can_ghost=false;
                can_spirit=false;
            }
        }
        else if(nb_seen==0){
            can_zombie=false;
            can_ghost=false;

        }
    }
    else if(*seenMirror==false){
        if(taille==nb_seen){
            can_spirit=false;
            can_ghost=false;
        }
        else if(nb_seen==0){
            can_vampire=false;
            can_zombie=false;
        }
    }
    if(nb_seen==0 && taille !=0){
        can_zombie=false;
    }
    if(nb_seen==1 && taille ==1 && !(*seenMirror)){
        can_vampire=false;
    }
    //if()


//    if(taille/2==nb_seen && back){//1
//        can_zombie=false;
//        can_spirit=false;
//    }

    if(back && taille==2 && nb_seen==1){
        can_zombie=false;
        can_spirit=false;
    }
    if(can_ghost==false && can_zombie==false && can_vampire==false && can_spirit==false){//in case we removed every thing, the game is wrong, not yet occured
        printf("Une case est dépourvue de toute possibilité, le jeu est impossible :/\n");
        exit(EXIT_FAILURE);
    }

    //here is the actual removing
    if(can_ghost==false)
        suprimer_occurence(grille[x][y],GHOST);
    if(can_vampire==false)
        suprimer_occurence(grille[x][y],VAMPIRE);
    if(can_zombie==false)
        suprimer_occurence(grille[x][y],ZOMBIE);
    if(can_spirit==false)
        suprimer_occurence(grille[x][y],SPIRIT);
}

bool touchMirror(content * mirroir,int * vx,int * vy){
    if(*mirroir==HMIRROR)
        return *vy!=0;
    else if(*mirroir==VMIRROR)
        return *vx!=0;
    return true;
}

void parkour(game game, direction side, int pos,content *** tab){
    //in this function we go through the game, listing all frame and their position to, then modify the list assiciated in the matrix
    //it is simmilar to previous functions
    if(game == NULL){/**test if pointer given is null**/
        fprintf(stderr,"pointeur null\n");
        exit(EXIT_FAILURE);
    }
    int max=game_width(game)*game_height(game);
    //histoX and histoY lists all the position of all empty frame encontered to then apply modification to these positions
    int histoX[max];
    set_list_tominus_one(histoX,max);
    int histoY[max];
    set_list_tominus_one(histoY,max);
    bool infinite_loop = false;
    int rq_seen = required_nb_seen(game,side,pos);// the number of monster to be seen
    //to count the number of monster seen from a position, we go through the grid, from a starting position (x & y), until we got out of it
    int x=0;// x and y will be our coordinates into the grid
    int y=0;// "
    bool seenMirror; //current state of the light ray, to count either vampires or ghosts
    seenMirror=false;
    int vy;
    int vx;
    vx=0;
    vy=0;
    int n=0;
    /*switch initializing x and y following the given position and side*/
    switch (side){
        case N : y=game_height(game)-1;x=pos;vy=-1;break;
        case S : y=0;x=pos;vy=1;break;
        case E : x=game_width(game)-1;y=pos;vx=-1;break;
        case W : x=0;y=pos;vx=1;break;
        default : exit(EXIT_FAILURE);break;
    }

    content caseGame = EMPTY;//caseGame will contain the content at the current position for comparing purpose
    /*the loop allowing us to go through the grid until we exede its limits*/
    int taille =0;
    bool back =false;
    while((x>-1 && x<(game_width(game)))&&(y>-1 && y < (game_height(game))) && !infinite_loop){
        caseGame = get_content(game,x,y);//catch the current content
        if(touchMirror(&caseGame,&vx,&vy)&&seenMirror==false){//we put -2 at indice taille+n to tel that we have encountered a mirror from now on
            n=taille;
        }

        switch (caseGame) {//to process what to do given the diferent kind  of content encontered
            case MIRROR :mirrorEncounter2(&vx,&vy,MIRROR,&seenMirror,&back,&infinite_loop);break;
            case ANTIMIRROR : mirrorEncounter2(&vx,&vy,ANTIMIRROR,&seenMirror,&back,&infinite_loop);break;
            case HMIRROR :mirrorEncounter2(&vx,&vy,HMIRROR,&seenMirror,&back,&infinite_loop); break;
            case VMIRROR : mirrorEncounter2(&vx,&vy,VMIRROR,&seenMirror,&back,&infinite_loop); break;
            default:break;
        }

        if(!is_mirror(caseGame)){//we list all positions
            histoX[taille+n]=x;
            histoY[taille+n]=y;
            taille++;
        }
        x += vx;//we update the positions x and y according to the vectors vx and vy
        y += vy;//

    }
    seenMirror=false;
    for(int i=0;histoX[i]!=-1;i++){
        if(i==n)
            seenMirror=true;
        apply(tab,taille,back,rq_seen,histoX[i],histoY[i],&seenMirror);
    }
}

void delete_content_tab(content *** tab,uint width,uint height){
    for(int i=0;i<width;i++){
        for(int j =0;j<height;j++){
            free(tab[i][j]);
        }
        free(tab[i]);
    }
    free(tab);
}

void set_list_tominus_one(int * l,int n){
    for(int i = 0; i<n;i++){
        l[i]=-1;
    }
}

void suprimer_occurence(content * liste, content monster){
    for(int i=0;liste[i]!=-1&&i<4;i++){
        if(liste[i]==monster){
            for(int j=i;j<3;j++){
                liste[j]=liste[j+1];
                liste[j+1]=-1;
            }
        }
    }
}
