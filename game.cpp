/*
 * Kent hack enough 2019
 * game.cpp
 * Damian Kouvaris
 * 9/28/2019
 */

#include "old_western.hpp"

int main(){
  srand(time(NULL));

  int gameMode = 0;
  char quit = 'n';
  while(quit != 'y'){
    std::cout << "Welcome to old western, how many players are there?(1/2)" << std::endl;
    std::cin >>  gameMode;

    if(gameMode == 1)
      singleplayer();
    if(gameMode == 2)
      multiplayer();

    std::cout << "Would you like to quit?(y/n) " << std::endl;
    std::cin >> quit;
  }

 return 0;
}
