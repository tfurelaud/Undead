# Undead #

This game is a reflexion game developped in C. 

![Undead Snapshot](Capture.png?raw=true "snapshot")

As you can see, the SDL design part is not finished yet.

## Rules 

To win the game you have to add monsters where they have to be. 

Zombies count for 1 points whatever the position. 

Vampires count for 1 point if they are before a mirror.

Ghosts count for 1 point if they are after a mirror.

Spirits count for 0 point. 

A line begins with a number and follows the direction of the mirrors that are on the line. At the end of the line, you should have the same number by adding the score of each monster on that line, as the number you read at the start of the row.

You will notice that a line is not the same in both directions.


## Librairies needed

For this game you'll need : sdl2-dev 

```
  sudo apt-get install libsdl2-dev
```

## Compilation and execution

To compile and execute the game :

```
  mkdir build
```
```
  cd build
```
```
  cmake ..
```
```
  make
```
```
  cd ..
```
```
  ./build/undead_"the version you want to execute" (ex : ./build/sdl_text)
```
