# Dungeons Of Pain

Dungeons of Pain is a turn-based graphical roguelike game. You pick a class and slaughter your way through rooms filled with enemies. Along the way you will find new items to equip, potions to chug down and spells to cast. The game is not easy and it will take perseverance to defeat Sorr Coldheart in his icy chamber.

When you are done playing, you can also mod the game, adding new enemies, items, classes, spells and levels. I love it when games let players change and mod them how they like, which is why I decided to make this game moddable as well.

## How to play

You can download the game from the releases section. Unpack it and run the executable.

Make sure to read the controls text file to better understand the game.

## How to mod

Game elements are defined through XML files. If you downloaded the game release, you will find these in *rsrcs/xmls*. There are several different files, each for a different aspect of the game. At the beginning of each file is a description of what it is for and how you can alter it.

You mod the game by adding your own elements to XMLs (or by changing/removing existing parts). You can also add new graphics and sound effects and reference them in XML files.

## How to use code

Project was written in C++. You will find all code files in *SOURCE* folder.

Aside from those, the project also uses several libraries. You can find these in their respective folders.

When running, *openal32.dll* is needed for the executable to start.
