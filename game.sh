#!/bin/bash  
g++ -c Centipede_Game.cpp
g++ Centipede_Game.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
./sfml-app
