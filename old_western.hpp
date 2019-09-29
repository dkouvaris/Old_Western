/*
 * Kent hack enough 2019
 * old_western.hpp
 * Damian Kouvaris
 * 9/28/2019
 */

#ifndef OLD_WESTERN_HPP
#define OLD_WESTERN_HPP

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

const int NAME_SIZE = 8, ACC_SIZE = 7;
const int REV_AMMO = 6, LR_AMMO = 1, CARAVAN_AMMO = 2;
const double REV_ACC[7] = {0.2, 0.4, 0.36, 0.33, 0.32, 0.4, 0.5};
const double LR_ACC[7] = {0, 126, 140, 120, 60, 60, 60};
const double CG_ACC[7] = {1.0, 1.2, 1.1, 1.06, 0.9, 0.85, 0.83};


//////////////
//CLASS INV: Will always stary with y=0, but x and y can be any int
//
class location{
public:
  location(); //sets to 0,0
  location(int);
  void move(char,int);
  double getDistance(location);

private:
  int x; //x coord
  int y; //y coord
};

/////////////
//CLASS INV: Three choices, longrifle, revolver, caravan
//           Each has a prefered range with accuracy falloffs
//
class weapon{
public:
  weapon(); //default to rev
  weapon(char); //l, r, c
  void fire();
  void load();
  bool checkLoad()const;
  char checkType()const;
  double getAccuracy(int)const;
private:
  char type;
  int ammo; //in wep
  bool loaded;
  double accuracy[ACC_SIZE]; //falloff was merged into this
  

};
//////////////
//CLASS INV: Contains player's stats and actions
//

class player{
public:
  player(); //default settings for computer player
  player(const char[], char&, int); //choose weapon and set human player distance
  void your_turn(); //passive
  bool checkTurn()const; //passive
  void  aim(player); //active
  void  move(char,int,player); //active
  void  shoot(player&); //active
  void  reload(player); //active
  void checkGun()const; //active
  bool compCheckGun()const; //passive
  bool checkHit(player)const; //passive
  void applyDamage(); //passive
  void applyDamage(weapon); //passive
  int getHealth()const; //passive
  bool stillAlive()const; //passive
  bool checkDodge()const;
private:
  int health;
  bool took_aim;
  bool agile_target;
  location loc;//class
  weapon gun;//class
  bool has_initiative;
  char name[NAME_SIZE];
};

void singleplayer();
void multiplayer();


#endif
