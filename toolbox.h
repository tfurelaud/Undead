#include <stdbool.h>
#include "game.h"
#include "game_alea.h"




/*  it is easier to use a type for each functionality of the game  */
typedef enum function_e {FIND_ONE,NB_SOL,FIND_ALL} function;
typedef unsigned int uint;

/**
 * @brief Initialisation : Initializes a game grid. We initialize the number of all the monsters, we fill the cases of the game grid and the labels.
 * @return A variable of the type game which is our empty game grid(so without the monsters but with the mirrors, labels, ....).
 **/

game Initialisation(void);


/**
 * @brief File
 * @param argc
 * @param argv
 * @return
 */

game File(int argc, char* argv[]);


/**
 * @brief printSpaces : Print a space between the labels and the '|'.
 * @param a game g.
 */

void printSpaces(cgame g);

/**
 * @brief display : print the game grid with all its elements (labels, nb monsters, monsters,...) as and when we progress in the game. (labels, nb de monstres, monstres, ...) au fur et à mesure de l'avancement de la partie.
 * @param a game g.
 */

int display(cgame g);


/**
 * @brief clearBuffer : clear the scanf after its utilisation when we had withdraw a character with scanf.
 */

void clearBuffer(void);


/**
 * @brief isOkay : Return true if the param of coordinate(x,y) is in the game grid or false if not.
 * @param width coordinate of the requested position.
 * @param height coordinate of the requested position.
 * @param a game g.
 * @return a boolean.
 */

bool isOkay(int x,int y,cgame g);

/** fonctions du solveur **/

/**
 * @brief is_mirror : test if a case which is selectionned is a mirror(antimirror,mirror,Hmirror or Vmirror) or not.
 * @param contenu : content tested.
 * @return a boolean.
 */

bool is_mirror(content contenu);

/**
 * @brief concat : concat 2 characters chain.
 * @param a first chain.
 * @param a second chain.
 * @return a chain.
 */

char* concat(const char *s1, const char *s2);


/**
 * @brief name_generator : Generates a solveur name for a game.
 * @param the name of the game.
 * @param the number of the solution.
 * @return name of the solver file.
 */

char * name_generator(char * prefixe, int n);


/**
 * @brief shuffle
 * @param ordre
 *
 * is not implemented yet
 * It will (maybe) sposed to shuffle a list of content type, for the solver.
 */
void shuffle(content * ordre);


/**
 * @brief solveur : solve a game grid.
 * @param game configuration.
 * @param a game.
 * @param name of the grid game.
 * @param number of the solution.
 * @param position on the grid.
 * @param boolean if a solution was found.
 * @param content list to use recursivly.
 * @param nbm is the list that contain the current number
 *
 * The main function of the solving part, it tests all possibilities for each place (predeterminated with the parameter ordre) recurcively,
 * it also checks at each step if the path its taking is possible or not.
 */

void solveur(function * fonction, game g,char * prefix_file ,int * sol_num, unsigned int * pos, bool * drop_it,content *** ordre,int * nbm);


/**
 * @brief compare_string : compare 2 characters lists.
 * @param string1
 * @param string2
 * used in string_to_func.
 * @return if the two strings are matching each other
 *
 * It will be checking if the input of the user corresspond to any parameter of the programm.
 */

bool compare_string(const char * string1,const char * string2);


/**
 * @brief string_to_func : verify if input is correct.
 * @param string the string to analyse
 * @return fonctionnality of the solver.
 *
 * it returns the function type associated to the user input
 */

function string_to_func(char * string);

/**
 * @brief is_valid is used to verify at each step,if the move chosen is possible to actualy do
 * @param g, the game in question
 * @param nb_z, the number of zombies
 * @param nb_s, ""
 * @param nb_v, ""
 * @param nb_g, ""
 * @param monster, the monster added
 * @param pos, the current position
 * @return if yes or no we can continue through the recursion
 */
bool is_valid(game g,int * nbm,content monster,unsigned int * pos);

/** solveur expérimental **/
/**
 * @brief label_corresponding is used in is valid to check a label associated to a frame and a direction
 * @param g, the game
 * @param pos, the position
 * @param dir, the direction we want to look at
 * @return if the current nb_seen is plosible torward a certain dirction acording to required_nb_seen
 */
bool label_corresponding(game g,unsigned int * pos,direction dir);

/**
 * @brief return_verif returns if current_nb_seen of a label is equal or below the required_nb_seen
 * @param g, the game
 * @param x, abciss
 * @param y, ordonee
 * @return , boolean
 */
bool return_verif(game g,int * x,int * y);

/**
 * @brief mirrorEncounter2 apply the needed modifications to all parameters when a mirror is encontered, given the type of mirror
 * @param vx, speed horizontaly
 * @param vy, speed verticly
 * @param mirrorType, type of mirror
 * @param seenMirror, if we encountered a mirror already
 * @param back, if we are going backward (due to HMIRROR & VMIRROR)
 * @param infinite_loop , if we are stuck in an infinite loop, used to break free
 */
void mirrorEncounter2(int * vx, int * vy, content mirrorType,bool * seenMirror,bool * back,bool * infinite_loop);

/**
 * @brief deduction ,returns the matrix of all possible content in each frame of the game, before the solver starts solving
 * @param g, the game
 * @return a table of list of content
 */
content *** deduction(game g);

/**
 * @brief apply, it apply modification to the grille (table of lists) given its parameters, example: if a label indicates 0, all zombies concerned in the grille of possibilities will be removed
 * @param grille, the grid of possible content
 * @param taille, the length of the path concerned
 * @param back if we went back
 * @param nb_seen, the label
 * @param x, position
 * @param y, position
 * @param seenMirror, if we have seen a mirror yet
 * @param nb_seen_arr, the oposite label
 */
void apply(content *** grille,int taille,bool back,int nb_seen, int x, int y,bool * seenMirror);

/**
 * @brief touchMirror, says if we touched a mirror
 * @param mirroir, type of mirror
 * @param vx, spped x
 * @param vy, speed y
 * @return if we effectively encountered a mirror
 */
bool touchMirror(content * mirroir,int * vx,int * vy);

/**
 * @brief parkour, go through the game and modify the possibilities of each frame in the path
 * @param game, the game
 * @param side, the side we starts
 * @param pos, the position in the side in particular
 * @param tab, the table of possibilities to modify
 */
void parkour(game game, direction side, int pos,content *** tab);

/**
 * @brief set_list_tominus_one sets all elements of list to -1
 * @param l, the list
 * @param n, the length
 */
void set_list_tominus_one(int * l,int n);

/**
 * @brief delete_content_tab, free the allocated memory for the matrix
 * @param tab, the matrix in question
 * @param width, its width
 * @param height, its height
 */
void delete_content_tab(content *** tab,uint width,uint height);


/**
 * @brief suprimer_occurence, delete a monster in a list
 * @param liste
 * @param monster
 *
 * it is used to modify the matrix of possibilities ( a table of lists)
 *
 */
void suprimer_occurence(content * liste, content monster);



