#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"
#include "game_alea.h"
#include "toolbox.h"
#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

bool test_get_content(){

    printf("testing get_content()             : ");
    bool res=true;
    game g = new_game();
    assert(g);

    /*
    | Z:1 V:1 G:1 S:1 |
    |                 |
    |     X X X X     |
    |                 |
    |  X  Z   - \  X  |
    |  X  V   / |  X  |
    |  X  G        X  |
    |  X  S        X  |
    |                 |
    |     X X X X     |
    |                 |
*/

    add_monster(g, SPIRIT, 0, 0);
    add_monster(g, GHOST, 0, 1);
    add_monster(g, VAMPIRE, 0, 2);
    add_monster(g, ZOMBIE, 0, 3);

    add_mirror_ext(g, MIRROR, 2, 2);
    add_mirror_ext(g, ANTIMIRROR, 3, 3);
    add_mirror_ext(g, VMIRROR, 3, 2);
    add_mirror_ext(g, HMIRROR, 2, 3);

    if(get_content(g, 0, 0) != SPIRIT) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : SPIRIT : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 0, 1) != GHOST) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : GHOST : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 0, 2) != VAMPIRE) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : VAMPIRE : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 0, 3) != ZOMBIE) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : ZOMBIE : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 2, 2) != MIRROR) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : MIRROR : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 3, 3) != ANTIMIRROR) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : ANTIMIRROR : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 3, 2) != VMIRROR) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : VMIRROR : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 2, 3) != HMIRROR) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : HMIRROR : should have been detected but have not\n");
        res=false;
    }

    if(get_content(g, 3, 0) != EMPTY) {
        if(res==true){
            printf("Basic test on get_content failed\n");
        }
        printf("	|content of value : EMPTY : should have been detected but have not\n");
        res=false;
    }

    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}


void test_delete_game(){// delete_game est testé grâce au valgrind//
    printf("testing delete_game()             : cf valgrind result\n");
}

bool test_required_nb_seen(){
    printf("testing required_nb_seen()        : ");

    bool res=true;
    game g = new_game();
    assert(g);

    if(required_nb_seen(g,N,0)!= 0){
        printf("Basic test on required_nb_seen failed\n");
        printf("	|NORTH labels does not match what was given\n");
        res=false;
    }
    if(required_nb_seen(g,S,0)!= 0){
        printf("Basic test on required_nb_seen failed\n");
        printf("	|SOUTH labels does not match what was given\n");
        res=false;
    }
    if(required_nb_seen(g,E,0)!= 0){
        printf("Basic test on required_nb_seen failed\n");
        printf("	|EAST labels does not match what was given\n");
        res=false;
    }
    if(required_nb_seen(g,W,0)!= 0){
        printf("Basic test on required_nb_seen failed\n");
        printf("	|WEST labels does not match what was given\n");
        res=false;
    }
    if(res==false){
        goto shutitdown;
    }

    /* Map used in test_required_nb_seen function
    | Z:3 V:2 G:2 S:2 | Z:3 V:2 G:2 S:2 |
    |                 |                 |
    |     0 1 3 2     |     0 1 3 2     |
    |                 |                 |
    |  0  \     |  0  |  0  \ V V |  0  |
    |  2  -        2  |  2  - S Z Z  2  |
    |  2      \ /  0  |  2  Z G \ /  0  |
    |  0  \     \  0  |  0  \ S G \  0  |
    |                 |                 |
    |     0 1 2 4     |     0 1 2 4     |
    |                 |                 |
    |     start       |     solution    |
*/

    delete_game(g);
    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 3;
    int nb_spirits = 2;
    content board []={ANTIMIRROR, SPIRIT, GHOST, ANTIMIRROR, ZOMBIE, GHOST, ANTIMIRROR, MIRROR, HMIRROR, SPIRIT, ZOMBIE, ZOMBIE, ANTIMIRROR, VAMPIRE, VAMPIRE, VMIRROR};
    int North_labels [] = {0, 1, 3, 2};
    int South_labels [] = {0, 1, 2, 4};
    int East_labels [] = {0, 0, 2, 0};
    int West_labels [] = {0, 2, 2, 0};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    g = setup_new_game_ext(4, 4, labels, board, nb_ghosts, nb_vampires ,nb_zombies, nb_spirits);

    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,N,i)!=North_labels[i]){
            if(res==true){
                printf("Basic test on required_nb_seen failed\n");
            }
            printf("	|NORTH labels does not match what was given\n");
            res=false;
            goto next;
        }
    }

next:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,S,i)!=South_labels[i]){
            if(res==true){
                printf("Basic test on required_nb_seen failed\n");
            }
            printf("	|SOUTH labels does not match what was given\n");
            res=false;
            goto next2;
        }
    }

next2:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,E,i)!=East_labels[i]){
            if(res==true){
                printf("Basic test on required_nb_seen failed\n");
            }
            printf("	|EAST labels does not match what was given\n");
            res=false;
            goto next3;
        }
    }

next3:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,W,i)!=West_labels[i]){
            if(res==true){
                printf("Basic test on required_nb_seen failed\n");
            }
            printf("	|WEST labels does not match what was given\n");
            res=false;
            goto shutitdown;
        }
    }

shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }
    return res;
}

bool test_required_nb_monsters(){
    printf("testing required_nb_monster()     : ");
    bool res=true;
    game g = new_game();
    assert(g);
    set_required_nb_monsters(g,GHOST,3);
    set_required_nb_monsters(g,VAMPIRE,2);
    set_required_nb_monsters(g,ZOMBIE,4);
    set_required_nb_monsters(g,SPIRIT,1);

    if(required_nb_monsters(g,GHOST)!=3){
        if(res==true){
            printf("Basic test on required_nb_monsters failed\n");
        }
        printf("	|required_nb_monster does not match what was given\n");
        res=false;
    }
    if(required_nb_monsters(g,VAMPIRE)!=2){
        if(res==true){
            printf("Basic test on required_nb_monsters failed\n");
        }
        printf("	|required_nb_monster does not match what was given\n");
        res=false;
    }
    if(required_nb_monsters(g,ZOMBIE)!=4){
        if(res==true){
            printf("Basic test on required_nb_monsters failed\n");
        }
        printf("	|required_nb_monster does not match what was given\n");
        res=false;
    }
    if(required_nb_monsters(g,SPIRIT)!=1){
        if(res==true){
            printf("Basic test on required_nb_monsters failed\n");
        }
        printf("	|required_nb_monster does not match what was given\n");
        res=false;
    }

    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

bool test_restart_game(){
    bool res = true;
    printf("testing restart_game()            : ");
    game g = new_game();
    assert(g);
    add_monster(g, VAMPIRE,0,1);
    add_monster(g, ZOMBIE,2,1);
    add_monster(g, GHOST,1,1);
    add_monster(g, SPIRIT,3,0);
    set_required_nb_monsters(g,GHOST,3);
    set_required_nb_monsters(g,VAMPIRE,2);
    set_required_nb_monsters(g,ZOMBIE,4);
    set_required_nb_monsters(g,SPIRIT,1);

    restart_game(g);
    //we look if the content is resets
    for (int i=0; i<game_width(g);i++){
        for (int j=0; j<game_height(g);j++){
            if(get_content(g,i,j)==ZOMBIE || get_content(g,i,j)==GHOST || get_content(g,i,j)==VAMPIRE || get_content(g,i,j)==SPIRIT){
                if(res==true){
                    printf("Basic test on restart_game failed\n");
                }
                printf("	|monster detected on the grid :%d %d\n",i,j);
                res=false;
                goto shutitdown;
            }
        }
    }
shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

bool test_new_game(){
    bool res = true;
    printf("testing new_game()           	  : ");
    game g=new_game();
    assert(g);
    if(required_nb_monsters(g,VAMPIRE)!=0 || required_nb_monsters(g,GHOST)!=0 || required_nb_monsters(g,ZOMBIE)!=0){
        printf("Basic test on new_game failed\n");
        printf("	|requiered_nb_monster not null\n");
        res=false;
    }

    for(int i=0; i<game_width(g); i++){
        for(int j=0; j<game_height(g); j++){
            if(get_content(g,i,j)!=EMPTY){
                if(res==true){
                    printf("Basic test on new_game failed\n");
                }
                printf("	|content detected on the grid\n");
                goto shutitdown;
                res=false;
            }
        }
    }
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,N,i)!=0){
            if(res==true){
                printf("Basic test on new_game failed\n");
            }
            printf("	|NORTH label not null\n");
            res=false;
            goto next;
        }
    }
next:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,S,i)!=0){
            if(res==true){
                printf("Basic test on new_game failed\n");
            }
            printf("	|SOUTH label not null\n");
            res=false;
            goto next2;
        }
    }
next2:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,E,i)!=0){
            if(res==true){
                printf("Basic test on new_game failed\n");
            }
            printf("	|EAST label not null\n");
            res=false;
            goto next3;
        }
    }
next3:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,W,i)!=0){
            if(res==true){
                printf("Basic test on new_game failed\n");
            }
            printf("	|WEST label not null\n");
            res=false;
            goto shutitdown;
        }
    }
shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

bool test_new_game_ext(){
    bool res = true;
    printf("testing new_game_ext()            : ");
    game g=new_game_ext(4, 4);
    assert(g);
    if(required_nb_monsters(g,VAMPIRE)!=0 || required_nb_monsters(g,GHOST)!=0 || required_nb_monsters(g,ZOMBIE)!=0  ||required_nb_monsters(g,SPIRIT)!=0){
        printf("Basic test on new_game failed\n");
        printf("	|requiered_nb_monster not null\n");
        res=false;
    }

    for(int i=0; i<game_width(g); i++){
        for(int j=0; j<game_height(g); j++){
            if(get_content(g,i,j)!=EMPTY){
                if(res==true){
                    printf("Basic test on new_game_ext failed\n");
                }
                printf("	|content detected on the grid\n");
                goto shutitdown;
                res=false;
            }
        }
    }
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,N,i)!=0){
            if(res==true){
                printf("Basic test on new_game_ext failed\n");
            }
            printf("	|NORTH label not null\n");
            res=false;
            goto next;
        }
    }
next:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,S,i)!=0){
            if(res==true){
                printf("Basic test on new_game_ext failed\n");
            }
            printf("	|SOUTH label not null\n");
            res=false;
            goto next2;
        }
    }
next2:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,E,i)!=0){
            if(res==true){
                printf("Basic test on new_game_ext failed\n");
            }
            printf("	|EAST label not null\n");
            res=false;
            goto next3;
        }
    }
next3:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,W,i)!=0){
            if(res==true){
                printf("Basic test on new_game_ext failed\n");
            }
            printf("	|WEST label not null\n");
            res=false;
            goto shutitdown;
        }
    }
shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

bool test_set_required_nb_monster(){
    bool res = true;
    printf("testing set_required_nb_monster() : ");
    game g=new_game();
    assert(g);
    set_required_nb_monsters(g,ZOMBIE,4);
    set_required_nb_monsters(g,GHOST,6);
    set_required_nb_monsters(g,VAMPIRE,2);
    set_required_nb_monsters(g,SPIRIT,3);

    if(required_nb_monsters(g,ZOMBIE)!=4 || required_nb_monsters(g,VAMPIRE)!= 2 || required_nb_monsters(g,GHOST) != 6 || required_nb_monsters(g,SPIRIT)!=3){
        printf("Basic test on set_required_nb_monster failed\n");
        printf("	|required_nb_monster does not match with what was expected\n\n");
        res=false;
    }
    if(res==true){
        printf("ok.\n");
    }
    delete_game(g);

    return res;
}

bool test_add_mirror(){
    printf("testing add_mirror()              : ");
    bool res=true;
    game g = new_game();
    assert(g);
    add_mirror(g, 0, 1, 2);
    add_mirror(g, 1, 0, 2);
    add_mirror(g, 0, 3, 2);
    if(get_content(g, 1, 2) != MIRROR ||get_content(g, 0, 2) != ANTIMIRROR ||get_content(g, 3, 2) != MIRROR) {
        printf("Basic test on add_mirror failed\n");
        printf("	|content didn't match with what was expected\n\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}


bool test_add_mirror_ext(){
    printf("testing add_mirror_ext()          : ");
    bool res=true;
    game g = new_game();
    assert(g);
    add_mirror_ext(g, MIRROR, 1, 2);
    add_mirror_ext(g, ANTIMIRROR, 0, 2);
    add_mirror_ext(g, VMIRROR, 3, 2);
    add_mirror_ext(g, HMIRROR, 3, 3);
    if(get_content(g, 1, 2) != MIRROR ||get_content(g, 0, 2) != ANTIMIRROR ||get_content(g, 3, 2) != VMIRROR ||get_content(g, 3, 3) != HMIRROR){
        printf("Basic test on add_mirror_ext failed\n");
        printf("	|content didn't match with what was expected\n\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

bool test_current_nb_monsters(){
    printf("testing current_nb_monster()      : ");
    bool res=true;
    game g = new_game();
    assert(g);
    add_monster(g, GHOST, 0, 1);
    add_monster(g, VAMPIRE, 0, 2);
    add_monster(g, ZOMBIE, 0, 3);
    add_monster(g, SPIRIT, 0, 0);

    if(current_nb_monsters(g, GHOST) != 1){
        if(res==true){
            printf("Basic test on current_monsters with ghost failed\n");
        }
        printf("	|current_nb_monsters (GHOST) does not match what was expected\n");
        res=false;
    }
    if(current_nb_monsters(g, VAMPIRE) != 1){
        if(res==true){
            printf("Basic test on current_monsters with vampire failed\n");
        }
        printf("	|current_nb_monsters (VAMPIRE) does not match what was expected\n");
        res=false;
    }
    if(current_nb_monsters(g, ZOMBIE) != 1){
        if(res==true){
            printf("Basic test on current_monsters with zombie failed\n");
        }
        printf("	|current_nb_monsters (ZOMBIE) does not match what was expected\n");
        res=false;
    }
    if(current_nb_monsters(g, SPIRIT) != 1){
        if(res==true){
            printf("Basic test on current_monsters with ghost failed\n");
        }
        printf("	|current_nb_monsters (GHOST) does not match what was expected\n");
        res=false;
    }


    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;



}

bool test_current_nb_seen(){
    printf("testing current_nb_seen()         : ");
    bool res=true;
    game g = new_game();
    assert(g);

    /*
    | Z:1 V:1 G:1 S:1 |
    |                 |
    |     X 0 2 X     |
    |                 |
    |  X  / G Z S  2  |
    |  1        \  0  |
    |  1  G |   V  X  |
    |  X  V   - Z  2  |
    |                 |
    |     X X 0 X     |
    |                 |
*/

    add_monster(g, VAMPIRE, 0, 0);
    add_monster(g, ZOMBIE, 3, 0);
    add_monster(g, GHOST, 0, 1);
    add_monster(g, VAMPIRE, 3, 1);
    add_monster(g, GHOST, 1, 3);
    add_monster(g, ZOMBIE, 2, 3);
    add_monster(g, SPIRIT, 3, 3);

    add_mirror_ext(g, HMIRROR, 2, 0);
    add_mirror_ext(g, VMIRROR, 1, 1);
    add_mirror_ext(g, ANTIMIRROR, 3, 2);
    add_mirror_ext(g, MIRROR, 0, 3);


    if (current_nb_seen(g, S, 2) != 0){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 1 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, E, 0) != 2){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 2 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, E, 2) != 0){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 3 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, E, 3) != 2){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 4 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, N, 2) != 2){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 5 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, N, 1) != 0){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 6 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, W, 2) != 1){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 7 different than what was expected\n");
        res=false;
    }
    if(current_nb_seen(g, W, 1) != 1){
        if(res==true){
            printf("Basic test on current_monsters_seen failed\n");
        }
        printf("	|current_nb_seen 8 different than what was expected\n");
        res=false;
    }

    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}


bool test_is_game_over(){
    printf("testing is_game_over              : ");
    bool res=true;
    int width = 4;
    int height = 4;
    game g = new_game_ext(width, height);
    /* Map used in test_is_game_over function (solution map only)
    | Z:3 V:2 G:2 S:2 | Z:3 V:2 G:2 S:2 |
    |                 |                 |
    |     0 1 3 2     |     0 1 3 2     |
    |                 |                 |
    |  0  \     |  0  |  0  \ V V |  0  |
    |  2  -        2  |  2  - S Z Z  2  |
    |  2      \ /  0  |  2  Z G \ /  0  |
    |  0  \     \  0  |  0  \ S G \  0  |
    |                 |                 |
    |     0 1 2 4     |     0 1 2 4     |
    |                 |                 |
    |     start       |     solution    |
*/

    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 3;
    int nb_spirits = 2;
    content board []={ANTIMIRROR, SPIRIT, GHOST, ANTIMIRROR, ZOMBIE, GHOST, ANTIMIRROR, MIRROR, HMIRROR, SPIRIT, ZOMBIE, ZOMBIE, ANTIMIRROR, VAMPIRE, VAMPIRE, VMIRROR};
    int North_labels [] = {0, 1, 3, 2};
    int South_labels [] = {0, 1, 2, 4};
    int East_labels [] = {0, 0, 2, 0};
    int West_labels [] = {0, 2, 2, 0};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    g = setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires ,nb_zombies, nb_spirits);

    if (is_game_over(g) == true){
        for(int i=0; i<game_width(g); i++){
            if (current_nb_seen(g, N, i) != North_labels[i]){
                if(res==true){
                    printf("Basic test on is_game_over failed\n");
                }
                printf("	|current_nb_seen different than the lables on NORTH[%d]\n",i);
                res=false;
                goto shutitdown;
            }
        }
        for(int i=0; i<game_width(g); i++){
            if (current_nb_seen(g, S, i) != South_labels[i]){
                if(res==true){
                    printf("Basic test on is_game_over failed\n");
                }
                printf("	|current_nb_seen different than the lables on SOUTH[%d]\n",i);
                res=false;
                goto shutitdown;
            }
        }
        for(int i=0; i<game_height(g); i++){
            if (current_nb_seen(g, E, i) != East_labels[i]){
                if(res==true){
                    printf("Basic test on is_game_over failed\n");
                }
                printf("	|current_nb_seen different than the lables on EAST[%d]\n",i);
                res=false;
                goto shutitdown;
            }
        }
        for(int i=0; i<game_height(g); i++){
            if (current_nb_seen(g, W, i) != West_labels[i]){
                if(res==true){
                    printf("Basic test on is_game_over failed\n");
                }
                printf("	|current_nb_seen different than the lables on WEST[%d]\n",i);
                res=false;
                goto shutitdown;
            }
        }
//int current_nb_monsters(cgame game, content monster);
        if(current_nb_monsters(g, SPIRIT) != nb_spirits){
            if(res==true){
                printf("Basic test on current_monsters_seen failed\n");
            }
            printf("	|current_nb_monsters on the grid different than what was expected\n");
            res=false;
        }
        if(current_nb_monsters(g, ZOMBIE) != nb_zombies){
            if(res==true){
                printf("Basic test on current_monsters_seen failed\n");
            }
            printf("	|current_nb_monsters on the grid different than what was expected\n");
            res=false;
        }
        if(current_nb_monsters(g, GHOST) != nb_ghosts){
            if(res==true){
                printf("Basic test on current_monsters_seen failed\n");
            }
            printf("	|current_nb_monsters on the grid different than what was expected\n");
            res=false;
        }
        if(current_nb_monsters(g, VAMPIRE) != nb_vampires){
            if(res==true){
                printf("Basic test on current_monsters_seen failed\n");
            }
            printf("	|current_nb_monsters on the grid different than what was expected\n");
            res=false;
        }

        //faire un test avec set requiered nb monsters et current_nb_monsters

    }else{
        printf("Basic test on is_game_over failed\n");
        printf("	|is_game_over return false instead of true");
        res=false;
        goto shutitdown;
    }

shutitdown:
    if(res==true){
        printf("ok.\n");
    }

    delete_game(g);
    return res;
}



bool test_set_required_nb_seen(){
    bool res = true;
    printf("testing set_required_nb_seen()    : ");
    game g = new_game();
    assert(g);
    set_required_nb_seen(g,N,1,1);
    set_required_nb_seen(g,S,2,2);
    set_required_nb_seen(g,E,3,1);
    set_required_nb_seen(g,W,2,4);

    if(required_nb_seen(g,N,1) != 1 ||required_nb_seen(g,S,2) != 2 || required_nb_seen(g,E,3) != 1 || required_nb_seen(g,W,2) != 4){
        if(res==true){
            printf("Basic test on set_required_nb_seen failed\n");
        }
        printf("	|required_nb_seen did not match what were expected\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}



bool test_copy_game(){
    bool res = true;
    printf("testing copy_game()	          : ");
    game g = new_game();
    assert(g);

    /* Map used in test_copy_game function
| Z:3 V:2 G:2 S:2 |
|                 |
|     0 1 3 2     |
|                 |
|  0  \ V V |  0  |
|  2  - S Z Z  2  |
|  2  Z G \ /  0  |
|  0  \ S G \  0  |
|                 |
|     0 1 2 4     |
|                 |
|     solution    |
*/

    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 3;
    int nb_spirits = 2;

    int width = 5;
    int height = 5;

    content board []={ANTIMIRROR, SPIRIT, GHOST, ANTIMIRROR, ANTIMIRROR, ZOMBIE, GHOST, ANTIMIRROR, MIRROR, ANTIMIRROR, HMIRROR, SPIRIT, ZOMBIE, ZOMBIE, ANTIMIRROR, ANTIMIRROR, VAMPIRE, VAMPIRE, VMIRROR, ANTIMIRROR, SPIRIT, SPIRIT, SPIRIT, SPIRIT, SPIRIT};

    int North_labels [] = {0, 1, 3, 2, 5};
    int South_labels [] = {0, 1, 2, 4, 5};
    int East_labels [] = {0, 0, 2, 0, 5};
    int West_labels [] = {0, 2, 2, 0, 5};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};



    g = setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires ,nb_zombies, nb_spirits);

    game g2 = copy_game(g);

    if (game_width(g) != game_width(g2) || game_height(g) != game_width(g2)){
        if(res==true){
            printf("Basic test on copy_game failed\n");
        }
        printf("	|height or width on the copy are different\n");
        res=false;
    }

    if(required_nb_monsters(g,ZOMBIE)!=required_nb_monsters(g2,ZOMBIE)||required_nb_monsters(g,VAMPIRE)!=required_nb_monsters(g2,VAMPIRE) ||required_nb_monsters(g,GHOST)!=required_nb_monsters(g2,GHOST) || required_nb_monsters(g,SPIRIT)!=required_nb_monsters(g2,SPIRIT)){
        if(res==true){
            printf("Basic test on copy_game failed\n");
        }
        printf("	|required_nb_monsters on the copy are different\n");
        res=false;
    }

    for(int i=0; i<game_width(g); i++){
        for(int j=0; j<game_height(g); j++){
            if(get_content(g2,i,j)!=get_content(g, i, j)){
                if(res==true){
                    printf("Basic test on copy_game failed\n");
                }
                printf("	|different content found on the copy\n");
                goto shutitdown;
                res=false;
            }
        }
    }

    content tab [4] = {N, S, E, W};
    for(int c=0; c<2; c++){
        for(int w=0; w<game_width(g); w++){
            if(required_nb_seen(g,tab[c],w)!=required_nb_seen(g2,tab[c],w)){
                if(res==true){
                    printf("Basic test on copy_game failed\n");
                }
                printf("	|required_nb_seen on the copy are different\n");
                goto shutitdown;
                res=false;
            }
        }
    }

    for(int c=2; c<4; c++){
        for(int h=0; h<game_height(g); h++){
            if(required_nb_seen(g,tab[c],h)!=required_nb_seen(g2,tab[c],h)){
                if(res==true){
                    printf("Basic test on copy_game failed\n");
                }
                printf("	|required_nb_seen on the copy are different\n");
                goto shutitdown;
                res=false;
            }
        }
    }




    delete_game(g);
    delete_game(g2);

shutitdown:
    if(res==true){
        printf("ok.\n");
    }

    return res;

}




bool test_setup_new_game(){
    bool res = true;
    printf("testing setup_new_game()          : ");
    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 5;
    content board []={ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, MIRROR, ANTIMIRROR, EMPTY, EMPTY, EMPTY, ANTIMIRROR, EMPTY, EMPTY, MIRROR};
    int North_labels [] = {0, 3, 3, 0};
    int South_labels [] = {0, 3, 2, 3};
    int East_labels [] = {0, 0, 3, 2};
    int West_labels [] = {0, 2, 3, 3};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    game g=setup_new_game(labels, board, nb_ghosts, nb_vampires ,nb_zombies);
    assert(g);

    for(int i=0;i<(game_width(g)*game_height(g));i++){
        if(get_content(g,i%4,i/4)!=board[i]){
            if(res==true){
                printf("Basic test on setup_new_game failed\n");
            }
            printf("	|content of the grid different than what was given\n");
            res=false;
            goto next;
        }
    }
next:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,N,i)!=North_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game failed\n");
            }
            printf("	|NORTH labels differents than what was given\n");
            res=false;
            goto next1;
        }
    }
next1:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,S,i)!=South_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game failed\n");
            }
            printf("	|SOUTH labels differents than what was given\n");
            res=false;
            goto next2;
        }
    }
next2:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,E,i)!=East_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game failed\n");
            }
            printf("	|EAST labels differents than what was given\n");
            res=false;
            goto next3;
        }
    }
next3:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,W,i)!=West_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game failed\n");
            }
            printf("	|WEST labels differents than what was given\n");
            res=false;
            goto next4;
        }
    }

next4:
    if(required_nb_monsters(g, ZOMBIE) != nb_zombies || required_nb_monsters(g, VAMPIRE) != nb_vampires || required_nb_monsters(g, GHOST) != nb_ghosts){
        if(res==true){
            printf("Basic test on setup_new_game failed\n");
        }
        printf("	|requiered_nb_monsters are wrong\n");
        res=false;
        goto shutitdown;
    }
shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}

/*game setup_new_game_ext(int width, int height, int *labels[], content * board,int required_nb_ghosts, int required_nb_vampires, int required_nb_zombies, int required_nb_spirits){
    game g = new_game_ext(width,height);*/


bool test_setup_new_game_ext(){
    bool res = true;
    printf("testing setup_new_game_ext()      : ");
    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 5;
    int nb_spirits = 4;
    int width = 4;
    int height = 4;
    content board []={ANTIMIRROR, EMPTY, VMIRROR, ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, MIRROR, ANTIMIRROR, EMPTY, HMIRROR, EMPTY, ANTIMIRROR, EMPTY, EMPTY, MIRROR};
    int North_labels [] = {0, 3, 3, 0};
    int South_labels [] = {0, 3, 2, 3};
    int East_labels [] = {0, 0, 3, 2};
    int West_labels [] = {0, 2, 3, 3};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    game g=setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires, nb_zombies, nb_spirits);
    assert(g);

    if(game_height(g)!= height){
        if(res==true){
            printf("Basic test on setup_new_game_ext failed\n");
        }
        printf("	|height different than what was given\n");
        res=false;
        goto before1;
    }
before1:
    if(game_width(g)!= width){
        if(res==true){
            printf("Basic test on setup_new_game_ext failed\n");
        }
        printf("	|width different than what was given\n");
        res=false;
        goto before2;
    }

before2:
    for(int i=0;i<(game_width(g)*game_height(g));i++){
        if(get_content(g,i%4,i/4)!=board[i]){
            if(res==true){
                printf("Basic test on setup_new_game_ext failed\n");
            }
            printf("	|content of the grid different than what was given\n");
            res=false;
            goto next;
        }
    }
next:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,N,i)!=North_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game_ext failed\n");
            }
            printf("	|NORTH labels differents than what was given\n");
            res=false;
            goto next1;
        }
    }
next1:
    for(int i=0;i<game_width(g);i++){
        if(required_nb_seen(g,S,i)!=South_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game_ext failed\n");
            }
            printf("	|SOUTH labels differents than what was given\n");
            res=false;
            goto next2;
        }
    }
next2:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,E,i)!=East_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game_ext failed\n");
            }
            printf("	|EAST labels differents than what was given\n");
            res=false;
            goto next3;
        }
    }
next3:
    for(int i=0;i<game_height(g);i++){
        if(required_nb_seen(g,W,i)!=West_labels[i]){
            if(res==true){
                printf("Basic test on setup_new_game_ext failed\n");
            }
            printf("	|WEST labels differents than what was given\n");
            res=false;
            goto next4;
        }
    }

next4:
    if(required_nb_monsters(g, ZOMBIE) != nb_zombies || required_nb_monsters(g, VAMPIRE) != nb_vampires || required_nb_monsters(g, GHOST) != nb_ghosts || required_nb_monsters(g, SPIRIT) != nb_spirits){
        if(res==true){
            printf("Basic test on setup_new_game_ext failed\n");
        }
        printf("	|requiered_nb_monsters are wrong\n");
        res=false;
        goto shutitdown;
    }
shutitdown:
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }

    return res;
}



bool test_add_monster(){
    bool res = true;
    printf("testing add_monster()             : ");
    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 5;
    int nb_spirits = 4;
    int width = 4;
    int height = 4;
    content board []={ANTIMIRROR, EMPTY, VMIRROR, ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, MIRROR, ANTIMIRROR, EMPTY, HMIRROR, EMPTY, ANTIMIRROR, EMPTY, EMPTY, MIRROR};
    int North_labels [] = {0, 3, 3, 0};
    int South_labels [] = {0, 3, 2, 3};
    int East_labels [] = {0, 0, 3, 2};
    int West_labels [] = {0, 2, 3, 3};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    game g=setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires, nb_zombies, nb_spirits);
    assert(g);

    add_monster(g,ZOMBIE,1,1);
    add_monster(g,VAMPIRE,1,0);
    add_monster(g,GHOST,0,1);
    add_monster(g,SPIRIT,1,3);

    add_monster(g,ZOMBIE,0,0);
    add_monster(g,VAMPIRE,3,1);
    add_monster(g,GHOST,2,0);
    add_monster(g,SPIRIT,2,2);


    if(get_content(g,1,1)!=ZOMBIE || get_content(g,1,0)!=VAMPIRE || get_content(g,0,1)!=GHOST || get_content(g,1,3)!= SPIRIT){
        if(res==true){
            printf("Basic test on add_monster failed\n");
        }
        printf("	|content different than what was given\n");
        res=false;
    }
    if(get_content(g,0,0)!=ANTIMIRROR || get_content(g,3,1)!=MIRROR || get_content(g,2,0)!=VMIRROR || get_content(g,2,2)!=HMIRROR){
        if(res==true){
            printf("Basic test on add_monster failed\n");
        }
        printf("	|mirror or antimirror or vmirror or hmirror shouldn't have been replaced\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }
    return res;

}

/////////////////FONCTIONS à continuer

bool test_game_height(){
    bool res = true;
    printf("testing game_height()             : ");
    int height = 5;
    int width = 3;
    game g=new_game_ext(width, height);
    assert(g);

    if (game_height(g) != height){
        if(res==true){
            printf("Basic test on game_height failed\n");
        }
        printf("	|return different than what was given\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }
    return res;

}

bool test_game_width(){
    bool res = true;
    printf("testing game_width()              : ");
    int height = 5;
    int width = 3;
    game g=new_game_ext(width, height);
    assert(g);

    if (game_width(g) != width){
        if(res==true){
            printf("Basic test on game_width failed\n");
        }
        printf("	|return different than what was given\n");
        res=false;
    }
    delete_game(g);
    if(res==true){
        printf("ok.\n");
    }
    return res;

}

void testfuite(){
    printf("testing if any memory leaks	  : cf Leak summary (if any)\n");
}

//////////// Test fonctions toolbox

bool test_is_okay(){
    bool res = true;
    printf("testing is_okay()	          : ");
    game g = new_game();
    assert(g);

  /* Map used in test_copy_game function
| Z:3 V:2 G:2 S:2 |
|                 |
|     0 1 3 2     |
|                 |
|  0  \ V V |  0  |
|  2  - S Z Z  2  |
|  2  Z G \ /  0  |
|  0  \ S G \  0  |
|                 |
|     0 1 2 4     |
|                 |
|     solution    |
*/

    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 3;
    int nb_spirits = 2;

    int width = 5;
    int height = 5;

    content board []={ANTIMIRROR, SPIRIT, GHOST, ANTIMIRROR, ANTIMIRROR, ZOMBIE, GHOST, ANTIMIRROR, MIRROR, ANTIMIRROR, HMIRROR, SPIRIT, ZOMBIE, ZOMBIE, ANTIMIRROR, ANTIMIRROR, VAMPIRE, VAMPIRE, VMIRROR, ANTIMIRROR, SPIRIT, SPIRIT, SPIRIT, SPIRIT, SPIRIT};

    int North_labels [] = {0, 1, 3, 2, 5};
    int South_labels [] = {0, 1, 2, 4, 5};
    int East_labels [] = {0, 0, 2, 0, 5};
    int West_labels [] = {0, 2, 2, 0, 5};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};

    g = setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires ,nb_zombies, nb_spirits);

    if(isOkay(5, 5, g) != false || isOkay(-1, -1, g) != false || isOkay(2, 2, g)!= true){
        res=false;
    }
    if(res==true){
        printf("ok.\n");
    }else{
        printf("Basic test on isOkay failed\n	|return different than what was given\n");
    }
    return res;
}


bool test_is_mirror(){
    bool res = true;
    printf("testing is_mirror()	          : ");
    content board []={ANTIMIRROR, SPIRIT, GHOST, ANTIMIRROR, ANTIMIRROR, ZOMBIE, GHOST, ANTIMIRROR, MIRROR, ANTIMIRROR, HMIRROR, SPIRIT, ZOMBIE, ZOMBIE, ANTIMIRROR, ANTIMIRROR, VAMPIRE, VAMPIRE, VMIRROR, ANTIMIRROR, SPIRIT, SPIRIT, SPIRIT, SPIRIT, SPIRIT};
    if(is_mirror(board[0])&&is_mirror(board[8])&&is_mirror(board[10])&&is_mirror(board[18])!= true){
        res = false;
        printf("Basic test on is_mirror failed\n	|return different than what was given\n");
    }else{
        if(is_mirror(board[1])&&is_mirror(board[2])&&is_mirror(board[5])&&is_mirror(board[16])!= false){
            printf("Basic test on is_mirror failed\n	|return different than what was given\n");
            res = false;
        }else{
            printf("ok.\n");
        }
    }
    return res;

}

bool test_copy(){
    bool res = true;
    printf("testing copy()    	          : ");
    char * dest = (char *)malloc(16*sizeof(char));
    char * src = "bonjour";
    copy(dest, src);
    if (*dest != *src){
        printf("Basic test on test_copy failed\n	|return different than what was given\n");
        res = false;
    }else{
        printf("ok.\n");
    }
    return res;
}

/*bool is_valid(game g,int * nbm,content monster,unsigned int * pos){
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
}*/


bool test_solveur(){
    bool res = true;
    printf("testing solveur()    	          : ");
    int width = 4;
    int height = 4;
    game g = new_game_ext(width, height);
    /* Map used in test_is_game_over function (solution map only)
    | Z:3 V:2 G:2 S:2 | Z:3 V:2 G:2 S:2 |
    |                 |                 |
    |     0 1 3 2     |     0 1 3 2     |
    |                 |                 |
    |  0  \     |  0  |  0  \ V V |  0  |
    |  2  -        2  |  2  - S Z Z  2  |
    |  2      \ /  0  |  2  Z G \ /  0  |
    |  0  \     \  0  |  0  \ S G \  0  |
    |                 |                 |
    |     0 1 2 4     |     0 1 2 4     |
    |                 |                 |
    |     start       |     solution    |
*/

    int nb_ghosts = 2;
    int nb_vampires = 2;
    int nb_zombies = 3;
    int nb_spirits = 2;
    content board []={ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, EMPTY, EMPTY, ANTIMIRROR, MIRROR, HMIRROR, EMPTY, EMPTY, EMPTY, ANTIMIRROR, EMPTY, EMPTY, VMIRROR};
    int North_labels [] = {0, 1, 3, 2};
    int South_labels [] = {0, 1, 2, 4};
    int East_labels [] = {0, 0, 2, 0};
    int West_labels [] = {0, 2, 2, 0};
    int *labels [] = {North_labels, South_labels, East_labels, West_labels};
    g = setup_new_game_ext(width, height, labels, board, nb_ghosts, nb_vampires ,nb_zombies, nb_spirits);

    /*solveur(&fun,env->g,c,&k,&l,&drop,grid,tab)
    */
    function fun = FIND_ONE;
    char * c = "solution";
    int k =0;
    unsigned int l=0;
    bool drop = false;
    content *** grid;
    grid = deduction(g);
    int tab[4]={0,0,0,0};
    solveur(&fun,g,c,&k,&l,&drop,grid,tab);

    game g_correct = load_game("solution_vrai.sol");
    game g2=load_game("solution.sol");

    for (int i =0; i<width; i++){
        for (int j = 0; j<height; j++){
            if(get_content  (g_correct, i, j) != get_content(g2, i, j)){
                printf("Basic test on test_solveur failed\n	|return different than what was given\n");
                res = false;
                return res;
            }
        }

        /*content get_content(cgame game, int col, int line)*/
    }
    printf("ok.\n");
    return res;

}


////////////Test fonctions game_alea.c




//////////// Main test function


int main(void){

    bool result = true;

    result = test_required_nb_seen() && result;
    result = test_required_nb_monsters() && result;
    result = result && test_set_required_nb_seen();
    result = result && test_copy_game();
    result = result && test_setup_new_game();
    result = result && test_setup_new_game_ext();
    result = result && test_add_monster();
    result = test_set_required_nb_monster() && result;
    result = test_new_game() && result;
    result = test_new_game_ext() && result;
    result = test_add_mirror() && result;
    result = test_add_mirror_ext() && result;
    result = test_restart_game() && result;
    result = test_current_nb_monsters() && result;
    result = test_current_nb_seen() && result;
    result = test_is_game_over() && result;
    result = test_get_content() && result;
    result = result && test_game_width();
    result = result && test_game_height();
    test_delete_game();
    testfuite();
    result = result && test_is_okay();
    result = result && test_is_mirror();
    result = result && test_copy();
    result = result && test_solveur();

    if(result==false){
        fprintf(stderr,"Fonction(s) basique(s) défaillante(s)\n");
        return EXIT_FAILURE;
    }
    printf("Tests successfull\n");
    return EXIT_SUCCESS;
}
