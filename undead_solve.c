#include "game.h"
#include "game_io.h"
#include "toolbox.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//test branche


/**
 * @brief main : Do the solve.
 * @param argc
 * @param argv
 */

int main(int argc,char * argv[]){
    if(argc!=4){
        printf("Erreur de synthaxe :/\n");
        printf("voici le paterne: ./undead_solve FIND_ONE|NB_SOL|FIND_ALL <nom_fichier_pb> <prefix_fichier_sol>\n");
        exit(EXIT_FAILURE);
    }
    game the_game;
    char * functionality;
    char * name_file;
    char * prefix_file;
    int nbm[4]={0,0,0,0};
    function fonction;
    unsigned int position = 0;
    bool drop_it = false;
    int sol_num = 0;
    //content ordre[4]={ZOMBIE,VAMPIRE,GHOST,SPIRIT};


    content *** ordre;
    functionality = argv[1];
    name_file = argv[2];
    prefix_file = argv[3];
    fonction = string_to_func(functionality);
    the_game=load_game(name_file);
    ordre = deduction(the_game);
    solveur(&fonction,the_game,prefix_file,&sol_num,&position,&drop_it,ordre,nbm);

    if(fonction==FIND_ONE&&sol_num==0){
        FILE* f=fopen(concat(prefix_file,".sol"),"w");
        fprintf(f,"NO SOLUTION\n");
        fclose(f);
    }
    else if(fonction==NB_SOL){
        FILE* f=fopen(concat(prefix_file,".nbsol"),"w");
        fprintf(f,"%d\n",sol_num);
        fclose(f);
    }

    printf("paramètres: %s, %s, %s\n",functionality,name_file,prefix_file);
    printf("il y a eu %d solutions\n",sol_num);
    if(sol_num==0)
        exit(EXIT_FAILURE);
    delete_content_tab(ordre,game_width(the_game),game_height(the_game));
    delete_game(the_game);
    return EXIT_SUCCESS;
}
