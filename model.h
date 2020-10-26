// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MODEL_H
#define MODEL_H

#include "game.h"
#include "game_io.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Env_t Env;
typedef enum action_e {SELECT_ZOMBIE,SELECT_SPIRIT,SELECT_GHOST,SELECT_VAMPIRE,PUT_MONSTER,DO_QUIT,DO_SOLVE,DO_PLAY,DO_LOAD,DO_HELP,DO_MENU,DO_SELECT,DO_REPLAY,DO_RANDOM,UNDO} action;
typedef enum stage_e {MENU,HELP,GAME,GAME_OVER,LOAD} stage;
typedef struct bouton_s {
    int startingX;//original coordinate
    int startingY;
    int x;//new coordinate
    int y;
    int startingWidth;//original dimentions
    int startingHeight;
    int width;//current dimentions
    int height;
    action act;
    bool pictured;
    char * text;
}bouton;

typedef struct coup_s{
    int posx;
    int posy;
    content monster;
}coup;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);} while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

#define APP_NAME "SDL2 Demo"
#define SCREEN_WIDTH 270//270
#define SCREEN_HEIGHT 480//480
#define DELAY 100

/* **************************************************************** */



/**
 * @brief setBouton sets a new button
 * @param x its coordinate in x
 * @param y its coordinates in y
 * @param l its width
 * @param h its height
 * @param act what it will do
 * @param button its adress
 * @param pic boolean if there is pic related
 */
void setBouton(int x, int y, int l, int h, action act,bouton * button,bool pic);




/**
 * @brief load_playground setup the game environement (its game, parameters & buttons), sensybli the same for other load...
 * @param env
 * @param win
 */
void load_playground(Env * env,SDL_Window* win);

void load_menu(Env * env,SDL_Window* win);

void load_help(Env * env,SDL_Window* win);

void load_loadgame(Env * env,SDL_Window* win);




/**
 * @brief actualizeButton, adjust size and coordinates of the button if the window changes
 * @param button, the button in question
 * @param env
 * @param win
 */
void actualizeButton(bouton * button,Env * env,SDL_Window* win);




/**
 * @brief is_pushed check if a button is currently pushed according to mouse coordinate
 * @param x
 * @param y
 * @param button
 * @return bool
 */
bool is_pushed(int x,int y,bouton button);



/**
 * @brief putMonster, put a monster in the corresponding coordinates if we clicked in the grid
 * @param env
 * @param x
 * @param y
 * @param windows_width
 * @param windows_height
 * @param button
 */
void putMonster(Env* env,int x ,int y,int windows_width,int windows_height,bouton button);




/**
 * @brief select_game load the game corresponding to the string in the button
 * @param game the string
 * @return
 */
game select_game(char * game);

void copy(char* dest, char * src);

/**
 * @brief click, verify at each click what we clicked on (with above function) among the list of button and do accordding to its action associated with
 * @param win
 * @param ren
 * @param x
 * @param y
 * @param list
 * @param size
 * @param env
 * @param width
 * @param height
 */
void click(SDL_Window* win, SDL_Renderer* ren,int x, int y,bouton* list,int size,Env* env,int width,int height);




Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);
void render(SDL_Window* win, SDL_Renderer* ren, Env * env);
void clean(SDL_Window* win, SDL_Renderer* ren, Env * env);
bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);

/* **************************************************************** */

#endif
