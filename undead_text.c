#include "game.h"
#include "game_io.h"
#include "toolbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief main : Do the game and play the game.
 */

int main(int argc, char* argv[]){
    printf("ceci est un test\n");
    game g = File(argc,argv); //Modifié de game g = Initialisation ();
    //game g = random_game(10,10); //game aléaoire
    bool b;
    int coups = 0;
    int x, y;
    char m;
    while (is_game_over(g) != true){
        int r;
        display(g);
        b = false;
        while(b==false){
            printf("le format est le suivant : <x> <y> <G|V|Z|S>\n");
            r = scanf("%d %d %c", &x, &y, &m);
            clearBuffer();
            if (r==3){
                if (m == 'Z' && isOkay(x,y,g)){
                    b=true;
                    b=add_monster(g,ZOMBIE,x,y);

                }
                if (m == 'G' && isOkay(x,y,g)){
                    b=true;
                    b=add_monster(g,GHOST,x,y);

                }
                if (m == 'V' && isOkay(x,y,g)){
                    b=true;
                    b=add_monster(g,VAMPIRE,x,y);

                }
                if (m == 'S' && isOkay(x,y,g)){
                    b=true;
                    b=add_monster(g,SPIRIT,x,y);
                }
                else if(isOkay(x,y,g)== false){
                    printf("mauvaise saisie\n");
                }
            }
            else if (r == EOF){
                goto fin_de_fichier;
            }

        }

        if(isOkay(x,y,g)==true){
            coups++;
            printf("\nCoup joué n° %d: x = %d y = %d Monstre: %c\n\n", coups, x, y, m);
        }


    }
    display(g);
    printf(" Bravo c'est gagné!\n\n");
fin_de_fichier:
    delete_game(g);
    return EXIT_SUCCESS;

}



/*commentaire blabballablablablalbballabglbalab
cat soluce.txt | ./undead_text

add_mirror(Game, 1, 0, 0);
  add_mirror(Game, 1, 3, 0);
  add_mirror(Game, 1, 2, 1);
  add_mirror(Game, 0, 3, 1);
  add_mirror(Game, 1, 0, 2);
  add_mirror(Game, 1, 0, 3);
  add_mirror(Game, 0, 3, 3);*/

/* @verbatim
 |   Z:5 V:2 G:2   |   Z:5 V:2 G:2   |
 |                 |                 |
 |     0 3 3 0     |     0 3 3 0     |
 |                 |                 |
 |  3  \     /  2  |  3  z / V \ z 2  |
 |  3  \        3  |  3  v g _ s s 3  |
 |  2      \ /  0  |  2  / g | z \ 0  |
 |  0  \     \  0  |  0  z / z v s 0  |
 |                 |                 |
 |     0 3 2 3     |     0 3 2 3     |
 |                 |                 |
 |     start       |     solution    |*/




