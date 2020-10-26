// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include "toolbox.h"
#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"

/* **************************************************************** */

#define FONT "image/Arial.ttf"
#define FONTSIZE 70
#define BACKGROUND_IMG "image/fond.png"
#define BACKGROUND2_IMG "image/fond.png"
#define HELP_IMG "image/help.png"
#define ZOMBIE_IMG "image/zombie.png"
#define SPIRIT_IMG "image/spirit.png"
#define VAMPIRE_IMG "image/vampire.png"
#define GHOST_IMG "image/ghost.png"
#define HMIRROR_IMG "image/hmirror.png"
#define VMIRROR_IMG "image/vmirror.png"
#define MIRROR_IMG "image/mirror.png"
#define ANTIMIRROR_IMG "image/antimirror.png"
#define BOUTON_IMG "image/bouton_template.png"

/* **************************************************************** */

struct Env_t {
    game g;
    char * base_game;
    SDL_Texture * zombie;
    SDL_Texture * spirit;
    SDL_Texture * vampire;
    SDL_Texture * ghost;
    SDL_Texture * text;
    SDL_Texture * background;
    SDL_Texture * background2;
    SDL_Texture * help;
    SDL_Texture * hmirror;
    SDL_Texture * vmirror;
    SDL_Texture * antimirror;
    SDL_Texture * mirror;
    SDL_Texture * defaut;
    bouton * boutons; //is the list that contain all buttons that we'll interact with
    int total_buttons; //used to go through the list of buttons
    content currentMonster; //the monster selected
    int startingWidth; //the original dimentions used to scale images and buttons
    int startingHeight; // ...
    int currentWidth; //...
    int currentHeight; // ...
    stage etape; // the current state we're in (in the menu, playing, geting help)
    /* PUT YOUR VARIABLES HERE */

    coup * coups;
    int tailleCoups;
    int coupnum;

};

/* **************************************************************** */
void setBouton(int x, int y, int l, int h, action act,bouton * button,bool pic){
    button->x=x;
    button->startingX=x;
    button->y=y;
    button->startingY=y;
    button->width=l;
    button->startingWidth=l;
    button->height=h;
    button->startingHeight=h;
    button->act=act;
    button->pictured =pic;
    button->text=(char *)malloc(16*sizeof(char));
}
void copy(char* dest, char * src){
    int i = 0;
    for(i =0;src[i]!='\0';i++){
        dest[i]=src[i];
    }
    dest[i]=src[i];
}

void delButton(Env * env){
    for(int i = 0; i<env->total_buttons;i++){

        free(env->boutons[i].text);

    }
    free(env->boutons);
}
void load_playground(Env * env,SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    env->boutons=(bouton*)malloc(8*sizeof(bouton));
    setBouton(0,h*0.860,w*0.25,h*0.125,SELECT_ZOMBIE,&(env->boutons[0]),false);
    setBouton(w/4,h*0.860,w/4,h/8,SELECT_GHOST,&(env->boutons[1]),false);
    setBouton(2*w/4,h*0.860,w/4,h/8,SELECT_VAMPIRE,&(env->boutons[2]),false);
    setBouton(3*w/4,h*0.860,w/4,h/8,SELECT_SPIRIT,&(env->boutons[3]),false);
    setBouton(w/8,7*h/32,(w-(w/4)),((9*h)/16),PUT_MONSTER,&(env->boutons[4]),true);

    setBouton(0,h*0.025,3*w/9,h/9,DO_SOLVE,&(env->boutons[5]),false);
    copy(env->boutons[5].text,"SOLUTION");

    setBouton(6*w/9,h*0.025,3*w/9,h/9,DO_MENU,&(env->boutons[6]),false);
    copy(env->boutons[6].text,"MENU");

    setBouton(3*w/9,h*0.025,3*w/9,h/9,UNDO,&(env->boutons[7]),false);
    copy(env->boutons[7].text,"UNDO");

    sprintf(env->boutons[0].text,"%d",required_nb_monsters((env->g),ZOMBIE));
    sprintf(env->boutons[1].text,"%d",required_nb_monsters((env->g),GHOST));
    sprintf(env->boutons[2].text,"%d",required_nb_monsters((env->g),VAMPIRE));
    sprintf(env->boutons[3].text,"%d",required_nb_monsters((env->g),SPIRIT));
    env->total_buttons=8;

}

void load_menu(Env * env,SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    restart_game(env->g);
    env->background = env-> background2;
    env->boutons=(bouton*)malloc(4*sizeof(bouton));
    setBouton(w*0.25,h*0.2,w*0.5,h*0.1,DO_PLAY,&(env->boutons[0]),false);
    copy(env->boutons[0].text,"PLAY");

    setBouton(w*0.25,h*0.4,w*0.5,h*0.1,DO_LOAD,&(env->boutons[1]),false);
    copy(env->boutons[1].text,"LOAD");

    setBouton(w*0.25,h*0.6,w*0.5,h*0.1,DO_HELP,&(env->boutons[2]),false);
    copy(env->boutons[2].text,"HOW TO PLAY");

    setBouton(w*0.25,h*0.8,w*0.5,h*0.1,DO_QUIT,&(env->boutons[3]),false);
    copy(env->boutons[3].text,"QUIT");
    env->total_buttons=4;
}

void load_help(Env * env,SDL_Window* win){
    //juste une image et un bouton
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    env->background = env->help;
    env->boutons=(bouton*)malloc(sizeof(bouton));
    setBouton(w*0.25,h*0.8,w*0.5,h*0.1,DO_MENU,&(env->boutons[0]),false);
    copy(env->boutons[0].text,"RETOUR");
    env->total_buttons=1;
}

void load_gameover(Env * env,SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    env->boutons=(bouton*)malloc(3 * sizeof(bouton));
    setBouton(w*0.25,h*0.8,w*0.5,h*0.1,DO_LOAD,&(env->boutons[0]),false);
    copy(env->boutons[0].text,"ANOTHER ONE");
    setBouton(w*0.25,h*0.9,w*0.5,h*0.1,DO_MENU,&(env->boutons[1]),false);
    copy(env->boutons[1].text,"BACK TO MENU");
    setBouton(w/8,7*h/32,(w-(w/4)),((9*h)/16),PUT_MONSTER,&(env->boutons[2]),true);
    env->total_buttons=3;
}

void load_loadgame(Env * env,SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    env->boutons=(bouton*)malloc(3 * sizeof(bouton));
    setBouton(w*0.25,h*0.4,w*0.5,h*0.1,DO_SELECT,&(env->boutons[0]),false);
    copy(env->boutons[0].text,env->base_game);
    setBouton(w*0.25,h*0.6,w*0.5,h*0.1,DO_SELECT,&(env->boutons[1]),false);
    copy(env->boutons[1].text,"RANDOM");
    setBouton(w*0.25,h*0.8,w*0.5,h*0.1,DO_MENU,&(env->boutons[2]),false);
    copy(env->boutons[2].text,"BACK TO MENU");
    env->total_buttons=3;
}




Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[])
{
    Env * env = malloc(sizeof(struct Env_t));

    PRINT("Choose with mouse.\n");
    PRINT("Press ESC to quit. Enjoy this SDL2 tm1gundead Project!\n");
    /* get current window size */

    env->etape = MENU; // we start playing in the menu
    int w, h;

    SDL_GetWindowSize(win, &w, &h);
    env->startingHeight=h;//we get the starting dimentionss
    env->startingWidth=w;
    env->currentHeight=h;
    env->currentWidth=w;
    if(argc==1){
        env->g=load_game("game0");
        env->base_game="game0";
    }
    else{
        env->g=load_game(argv[1]);
        env->base_game=argv[1];
    }
    /* init positions */
    load_menu(env,win);

    // L'objectif ici étant de tester l'affichage des 8 images


    /* init background texture from PNG image */

    //voir si le chemin d'accès aux images est correct

    env->background = IMG_LoadTexture(ren, BACKGROUND_IMG);
    if(!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND_IMG);

    /* init texture from PNG images */
    env->background2 = IMG_LoadTexture(ren, BACKGROUND2_IMG);
    if(!env->background2) ERROR("IMG_LoadTexture: %s\n", BACKGROUND2_IMG);
    env->help = IMG_LoadTexture(ren, HELP_IMG);
    if(!env->help) ERROR("IMG_LoadTexture: %s\n", HELP_IMG);
    env->zombie = IMG_LoadTexture(ren, ZOMBIE_IMG);
    if(!env->zombie) ERROR("IMG_LoadTexture: %s\n", ZOMBIE_IMG);
    env->spirit = IMG_LoadTexture(ren, SPIRIT_IMG);
    if(!env->spirit) ERROR("IMG_LoadTexture: %s\n", SPIRIT_IMG);
    env->vampire = IMG_LoadTexture(ren, VAMPIRE_IMG);
    if(!env->vampire) ERROR("IMG_LoadTexture: %s\n", VAMPIRE_IMG);
    env->ghost = IMG_LoadTexture(ren, GHOST_IMG);
    if(!env->ghost) ERROR("IMG_LoadTexture: %s\n", GHOST_IMG);
    env->hmirror = IMG_LoadTexture(ren, HMIRROR_IMG);
    if(!env->hmirror) ERROR("IMG_LoadTexture: %s\n", HMIRROR_IMG);
    env->vmirror = IMG_LoadTexture(ren, VMIRROR_IMG);
    if(!env->vmirror) ERROR("IMG_LoadTexture: %s\n", VMIRROR_IMG);
    env->antimirror = IMG_LoadTexture(ren, ANTIMIRROR_IMG);
    if(!env->antimirror) ERROR("IMG_LoadTexture: %s\n", ANTIMIRROR_IMG);
    env->mirror = IMG_LoadTexture(ren, MIRROR_IMG);
    if(!env->mirror) ERROR("IMG_LoadTexture: %s\n", MIRROR_IMG);
    env->defaut = IMG_LoadTexture(ren,BOUTON_IMG);

    /* init text texture using Arial font */


    env->coupnum=-1;
    env->tailleCoups=32;
    env->coups=(coup *)malloc((env->tailleCoups)*sizeof(coup));
    return env;
}


void actualizeButton(bouton * button,Env * env,SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    button->x= (float)button->startingX * (float)w/env->startingWidth;
    button->y= (float)button->startingY * (float)h/env->startingHeight;
    button->width= (float)button->startingWidth * (float)w/env->startingWidth;
    button->height= (float)button->startingHeight * (float)h/env->startingHeight;
    int delta;
    if(button->width > button->height && button->act==PUT_MONSTER){
        delta = button->width - button->height;
        button->width=button->height;
        button->x=button->x + (float)delta/2;
    }
    else if(button->pictured==true){
        delta = button->height - button->width;
        button->height=button->width;
        button->y=button->y +(float) delta/2;
    }
}



/* **************************************************************** */
/*ATTENTION COPIER / COLLER DE DEMO.C*/



void render(SDL_Window* win, SDL_Renderer* ren, Env * env)
{

    SDL_Color color = { 0, 0, 255, 255 }; /*WHITE color in RGBA */
    TTF_Font * font = TTF_OpenFont(FONT, FONTSIZE);
    if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
    SDL_Surface * surf;

    SDL_Rect rect;
    /* get current window size */
    int w, h;
    SDL_GetWindowSize(win, &w, &h);

    /* render background texture */
    SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */



    for(int i=0;i<env->total_buttons;i++){
        
        rect.x=env->boutons[i].x;
        rect.y=env->boutons[i].y;
        rect.w=env->boutons[i].width;
        rect.h=env->boutons[i].height;
        surf=TTF_RenderText_Blended(font, env->boutons[i].text, color);
        env->text = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);

        /*affichage des boutons*/
        if(env->boutons[i].act==SELECT_GHOST){
            SDL_RenderCopy(ren,env->ghost,NULL,&rect);
            SDL_RenderCopy(ren,env->text,NULL,&rect);
        }
        else if(env->boutons[i].act==SELECT_VAMPIRE){
            SDL_RenderCopy(ren,env->vampire,NULL,&rect);
            SDL_RenderCopy(ren,env->text,NULL,&rect);
        }
        else if(env->boutons[i].act==SELECT_ZOMBIE){
            SDL_RenderCopy(ren,env->zombie,NULL,&rect);
            SDL_RenderCopy(ren,env->text,NULL,&rect);
        }
        else if(env->boutons[i].act==SELECT_SPIRIT){
            SDL_RenderCopy(ren,env->spirit,NULL,&rect);
            SDL_RenderCopy(ren,env->text,NULL,&rect);
        }
        else if((env->boutons[i].act==PUT_MONSTER) ){
            for(int m=0;m<game_width(env->g);m++){
                for(int n=0; n< game_height(env->g);n++){

                    rect.w=(float)env->boutons[i].width/game_width(env->g);
                    rect.h=(float)env->boutons[i].height/game_height(env->g);
                    rect.x=(float)env->boutons[i].x+(m * rect.w);
                    rect.y=(float)env->boutons[i].y+((game_height(env->g)-n-1)*rect.h);

                    content contenu = get_content(env->g,m,n);
                    if(contenu==EMPTY){

                    }
                    if(contenu==ZOMBIE){
                        SDL_RenderCopy(ren,env->zombie,NULL,&rect);
                    }
                    else if(contenu==GHOST){
                        SDL_RenderCopy(ren,env->ghost,NULL,&rect);
                    }
                    else if(contenu==VAMPIRE){
                        SDL_RenderCopy(ren,env->vampire,NULL,&rect);
                    }
                    else if(contenu==SPIRIT){
                        SDL_RenderCopy(ren,env->spirit,NULL,&rect);
                    }
                    else if(contenu==MIRROR){
                        SDL_RenderCopy(ren,env->mirror,NULL,&rect);
                    }
                    else if(contenu==ANTIMIRROR){
                        SDL_RenderCopy(ren,env->antimirror,NULL,&rect);
                    }
                    else if(contenu==VMIRROR){
                        SDL_RenderCopy(ren,env->vmirror,NULL,&rect);
                    }
                    else if(contenu==HMIRROR){
                        SDL_RenderCopy(ren,env->hmirror,NULL,&rect);
                    }
                }
            }
            SDL_Rect nb_seen;
            SDL_Color color = { 120, 0, 255, 255 }; /*WHITE color in RGBA */
            char * rqseen = (char*)malloc(4*sizeof(char));
            for(int dir=0;dir<NB_DIR;dir++){
                if(dir==N){
                    for(int w=0;w<game_width(env->g);w++){
                        sprintf(rqseen,"%d",required_nb_seen(env->g,dir,w));
                        surf=TTF_RenderText_Blended(font,rqseen , color);
                        env->text = SDL_CreateTextureFromSurface(ren, surf);///////////////
                        nb_seen.w=env->boutons[i].width/game_width(env->g);
                        nb_seen.h=env->boutons[i].height/game_height(env->g);
                        nb_seen.x=env->boutons[i].x + w*nb_seen.w;
                        nb_seen.y=env->boutons[i].y - h/8;
                        SDL_RenderCopy(ren,env->text,NULL,&nb_seen);

                        SDL_DestroyTexture(env->text);
                        SDL_FreeSurface(surf);
                    }
                }
                else if(dir==S){
                    for(int w=0;w<game_width(env->g);w++){
                        sprintf(rqseen,"%d",required_nb_seen(env->g,dir,w));
                        surf=TTF_RenderText_Blended(font,rqseen , color);
                        env->text = SDL_CreateTextureFromSurface(ren, surf);///////////////
                        nb_seen.w=env->boutons[i].width/game_width(env->g);
                        nb_seen.h=env->boutons[i].height/game_height(env->g);
                        nb_seen.x=env->boutons[i].x + w*nb_seen.w;
                        nb_seen.y=env->boutons[i].y + env->boutons[i].height;
                        SDL_RenderCopy(ren,env->text,NULL,&nb_seen);

                        SDL_DestroyTexture(env->text);
                        SDL_FreeSurface(surf);

                    }
                }
                else if(dir==E){
                    for(int w=0;w<game_height(env->g);w++){
                        sprintf(rqseen,"%d",required_nb_seen(env->g,dir,w));
                        surf=TTF_RenderText_Blended(font,rqseen , color);
                        env->text = SDL_CreateTextureFromSurface(ren, surf);///////////////
                        nb_seen.w=env->boutons[i].width/game_width(env->g);
                        nb_seen.h=env->boutons[i].height/game_height(env->g);
                        nb_seen.x=env->boutons[i].x + env->boutons[i].width;
                        nb_seen.y=env->boutons[i].y + env->boutons[i].height - (w+1)*nb_seen.h;
                        SDL_RenderCopy(ren,env->text,NULL,&nb_seen);

                        SDL_DestroyTexture(env->text);
                        SDL_FreeSurface(surf);

                    }
                }
                else if(dir==W){
                    for(int z=0;z<game_height(env->g);z++){
                        sprintf(rqseen,"%d",required_nb_seen(env->g,dir,z));
                        surf=TTF_RenderText_Blended(font,rqseen , color);
                        env->text = SDL_CreateTextureFromSurface(ren, surf);///////////////
                        nb_seen.w=env->boutons[i].width/game_width(env->g);
                        nb_seen.h=env->boutons[i].height/game_height(env->g);
                        nb_seen.x=env->boutons[i].x - w/8;

                        nb_seen.y=env->boutons[i].y + env->boutons[i].height - (z+1)*nb_seen.h;
                        SDL_RenderCopy(ren,env->text,NULL,&nb_seen);

                        SDL_DestroyTexture(env->text);
                        SDL_FreeSurface(surf);

                    }
                }
            }
            free(rqseen);
        }
        else if(env->boutons[i].pictured==false ){
            surf=TTF_RenderText_Blended(font,env->boutons[i].text , color);
            env->text = SDL_CreateTextureFromSurface(ren, surf);

            SDL_RenderCopy(ren,env->defaut,NULL,&rect);
            SDL_RenderCopy(ren,env->text,NULL,&rect);

            SDL_DestroyTexture(env->text);
            SDL_FreeSurface(surf);
        }
        else{
            SDL_RenderCopy(ren,env->defaut,NULL,&rect);
        }
        if(is_game_over(env->g)){
            SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(ren, 0, 0, w-2, 0);
            SDL_RenderDrawLine(ren, w-2, 0, w-2, h-2);
            SDL_RenderDrawLine(ren, w-2, h, 0, h-2);
            SDL_RenderDrawLine(ren, 0, h-2, 0, 0);
        }
    }

    TTF_CloseFont(font);

}

/* **************************************************************** */
bool is_pushed(int x,int y,bouton button){

    if(x<button.x || y<button.y){

        return false;
    }

    if(x<(button.x+button.width)&& y<(button.y+button.height)){

        return true;
    }

    return false;
}

void add_coup(coup * coups,content monstre, int x, int y,Env * env){
    if(env->coupnum>=env->tailleCoups){
        env->tailleCoups=env->tailleCoups*2;
        coups=realloc(coups,(env->tailleCoups)*sizeof(coup));
    }
    printf("add %d\n",monstre);
    coups[env->coupnum].monster=monstre;
    coups[env->coupnum].posx=x;
    coups[env->coupnum].posy=y;
    env->coupnum++;
}

void undo(coup * coups, Env * env){
    printf("coup %d\n",env->coupnum);
    if(env->coupnum>=0){
        printf("enleve %d\n",coups[env->coupnum-1].monster);
        add_monster(env->g,coups[env->coupnum-1].monster,coups[env->coupnum-1].posx,coups[env->coupnum-1].posy);
        env->coupnum--;
    }
}

void putMonster(Env* env,int x ,int y,int windows_width,int windows_height,bouton button){

    int gameWidth=game_width(env->g);
    int gameHeight=game_height(env->g);
    int alpha=button.width;
    int beta=button.height;

    int mousX = x- button.x;
    int mousY = y-button.y;
    int tailleCase;
    if(gameHeight>gameWidth){
        tailleCase=(float)alpha/gameHeight;
    }
    else{
        tailleCase=(float)beta/gameWidth;
    }

    int posX=mousX/tailleCase;
    int posY=mousY/tailleCase;
    posY=game_height(env->g)-posY-1;



    add_monster(env->g,env->currentMonster,(int)posX,(int)posY);
    add_coup(env->coups,env->currentMonster,(int)posX,(int)posY,env);

    if(is_game_over(env->g)){
        env->etape=GAME_OVER;
    }
}

game select_game(char * game){
    if(compare_string("RANDOM",game))
        return random_game(4,4);
    else{

        return load_game(game);
    }
}

void click(SDL_Window* win, SDL_Renderer* ren,int x, int y,bouton* list,int size,Env* env,int width,int height){
    for(int i=0;i<env->total_buttons;i++){
        int k =0;
        unsigned int l=0;
        bool drop = false;
        int tab[4]={0,0,0,0};
        char * c = "solution";

        content *** grid;
        function fun = FIND_ONE;
        if(is_pushed(x,y,list[i])){

            switch(list[i].act){
            case SELECT_GHOST:env->currentMonster=GHOST;break;
            case SELECT_SPIRIT:env->currentMonster=SPIRIT;break;
            case SELECT_VAMPIRE:env->currentMonster=VAMPIRE;break;
            case SELECT_ZOMBIE:env->currentMonster=ZOMBIE;break;
            case PUT_MONSTER:putMonster(env,x,y,width,height,list[i]);break;
            case DO_QUIT: clean(win,ren,env);exit(EXIT_SUCCESS);break;
            case DO_SOLVE:grid = deduction(env->g);solveur(&fun,env->g,c,&k,&l,&drop,grid,tab);display(env->g);delete_content_tab(grid,game_width(env->g),game_height(env->g));env->etape=GAME_OVER;break;break;
            case DO_PLAY:env->etape=GAME;break;
            case DO_LOAD:env->etape=LOAD;break;
            case DO_HELP:env->etape=HELP;break;
            case DO_MENU:env->etape=MENU;break;
            case DO_SELECT:delete_game(env->g);env->g=select_game(list[i].text);env->etape=GAME;break;
            case DO_RANDOM:delete_game(env->g);env->g=random_game(4,4);env->etape=GAME;break;
            case UNDO:undo(env->coups,env);break;
            default:break;
            }

            break;
        }

    }
}




bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e){

    /*int w=game_width(env->g); //les colonnes et lignes de la game
  int h=game_height(env->g);*/
    int width,height;     //taille
    SDL_GetWindowSize(win, &width, &height);
    for(int i=0;i<env->total_buttons;i++){
        actualizeButton(&(env->boutons[i]),env,win);
    }
    /* generic events */
    if (e->type == SDL_QUIT) {
        return true;
    }

    /* Android events */
#ifdef __ANDROID__
    else if (e->type == SDL_FINGERDOWN) {

        SDL_Point mouse;
        int x,y;
        stage currStage=env->etape;
        if(e->type == SDL_FINGERDOWN){

            x=e->tfinger.x;
            y=e->tfinger.y;
            click(win,ren,x,y,env->boutons,env->total_buttons,env,width,height);
            if(env->etape!=currStage){
                free(env->boutons);

                switch(env->etape){
                case GAME:load_playground(env,win);break;
                case MENU:load_menu(env,win);break;
                case HELP:load_help(env,win);break;
                case LOAD:load_loadgame(env,win);break;
                case GAME_OVER: load_gameover(env,win);break;
                default:break;
                }

            }
        }
    }
#else
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse;
        int x,y;
        stage currStage=env->etape;
        if(e->type==SDL_MOUSEBUTTONDOWN){
            SDL_GetMouseState(&mouse.x, &mouse.y);
            x=mouse.x;
            y=mouse.y;
            click(win,ren,x,y,env->boutons,env->total_buttons,env,width,height);

            if(env->etape!=currStage){
                delButton(env);

                switch(env->etape){
                case GAME:load_playground(env,win);break;
                case MENU:load_menu(env,win);break;
                case HELP:load_help(env,win);break;
                case LOAD:load_loadgame(env,win);break;
                case GAME_OVER: load_gameover(env,win);break;
                default:break;
                }

            }
        }
    }
#endif

    return false; /* don't quit */
}



/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env){
    SDL_DestroyTexture(env->zombie);
    SDL_DestroyTexture(env->spirit);
    SDL_DestroyTexture(env->vampire);
    SDL_DestroyTexture(env->ghost);
    SDL_DestroyTexture(env->text);
    SDL_DestroyTexture(env->background);
    SDL_DestroyTexture(env->hmirror);
    SDL_DestroyTexture(env->vmirror);
    SDL_DestroyTexture(env->antimirror);
    SDL_DestroyTexture(env->mirror);
    SDL_DestroyTexture(env->defaut);
    delete_game(env->g);
    delButton(env);

    //free((env->coups)); cause problème

    free(env);
}

/* **************************************************************** */
