/*
 * Kent hack enough 2019
 * old_western.cpp
 * Damian Kouvaris
 * 9/28/2019
 * contains all function definitions for project
 */

#include "old_western.hpp"

//def ctor for loc
location::location(){
  x = 0;
  y = 0;
}

//xcord ctor for loc
location::location(int xcord):location(){
  x = xcord;
}

//REQ: 'F' 'B' 'L' 'R'
//ENS: location change 1-5 unit in cardinal direction
void location::move(char dirn, int dist){
  if(dist < 0 || dist > 5)
    dist = 5;
    
  switch(dirn){

  case 'F':
    x = x + dist;
    break;

  case 'B':
    x = x - dist;
    break;

  case 'L':
    y = y + dist;
    break;

  case 'R':
    y = y - dist;
    break;

  default:
    break;
  }
}

double location::getDistance(location enemy){
  double result;
  result = (pow((x - enemy.x),2) +pow((y - enemy.y),2));
  return sqrt(result);
}

//def ctor for wep
weapon::weapon(){
  type = 'r';
  ammo = REV_AMMO;
  loaded = true;
  for(int i = 0; i < 8; ++i)
    accuracy[i] = REV_ACC[i];
  //accuracy = {0, 0.4, 0.2, 0.33, 0.25, 0.4, 0.5};
}

//chart ctor for wep(allows lr or cg)
weapon::weapon(char choice):weapon(){
  type = choice; 
  if(choice == 'l'){
    ammo = LR_AMMO;
    for(int i = 0; i < 8; ++i)
      accuracy[i] = LR_ACC[i];
    //accuracy = {0, 3.6, 1.4, 0.53, 0.1, 0.1, 0.1}; 
  }
  if(choice == 'c'){
    ammo = CARAVAN_AMMO;
    for(int i = 0; i < 8; ++i)
      accuracy[i] = CG_ACC[i];
    //accuracy = {0, 1.6, 1.2, 1.06, 0.9, 0.8, 0.66};
  }
}

//should expend 1 ammo for a chance to hit
//if no ammo should not fire, expend a turn
void weapon::fire(){
  if(loaded){
    ammo = ammo -1;
    if(ammo == 0)
      loaded = false;
  }
}

//loads weapon, regardless of current ammo. expends turn.
void weapon::load(){
  if(type == 'l')
    ammo = LR_AMMO;
  if(type == 'r')
    ammo = REV_AMMO;
  if(type == 'c')
    ammo = CARAVAN_AMMO;

  loaded = true;
}

//checks if loaded is t/f
bool weapon::checkLoad()const{
  return loaded;
}

//return weapon accuracy
double weapon::getAccuracy(int i)const{
  return accuracy[i];
}

//returns weapon type
char weapon::checkType()const{
  return type;
}

//def ctor for computer player
player::player(){
  health = 100;
  loc = location(10);
  took_aim = false;
  has_initiative = false;
  agile_target = false;
  gun = weapon();
  const char cn[7] = "Bandit";
  for(int i = 0; i < 7; ++i)
    name[i] = cn[i];
  name[7] = 0;
}

//name ctor for human player
player::player(const char pn[],char& gunType, int pos){
  health = 100; 
  loc = location(pos);
  took_aim = false;
  has_initiative = false;
  agile_target = false;
  gun = weapon(gunType);
  int i = 0;
  while(pn[i]!=0){
    name[i] = pn[i];
    ++i;
  }
  name[i] = 0;
}

//Who's turn is it anyways?
void player::your_turn(){
  has_initiative = true;
}

//Checks turn
bool player::checkTurn()const{
  return has_initiative;
}

//aim method for player, takes a turn but improves accuracy
void player::aim(player enemy){
  took_aim = true;
  std::cout << name << " steadies their aim for a critical shot" << std::endl;

  agile_target = false;
  has_initiative = false;
}

//move player, expend turn must be F,B,L,R
 void player::move(char dir, int dist, player enemy){
  if(dir != 'F'&& dir != 'B'&& dir != 'L'&& dir != 'R')
    std::cout << name <<" walked in a circle for some reason..." << std::endl;
  else{
    loc.move(dir,dist);
    std::cout << name << " took " << dist << " steps ";
    if(dir == 'F')
      std::cout << "forwards." << std::endl;
    if(dir == 'B')
      std::cout << "backwards." << std::endl;
    if(dir == 'L')
      std::cout << "to the right." << std::endl;
    if(dir == 'R')
      std::cout << "to the left." << std::endl;  
  }
  std::cout << name <<"'s swift movements make them harder to hit" << std::endl;

  took_aim = false;
  agile_target = true;
  has_initiative = false;
}

//fires weapon, expends turn
void player::shoot(player& enemy){
  if(enemy.loc.getDistance(loc) > 0){
    if(gun.checkLoad()){
      gun.fire();
      if(checkHit(enemy)){
        enemy.applyDamage(gun);
        std::cout<< name <<" landed a shot on their opponent." <<std::endl<<std::endl;
      }  
    } 
  
    else
      std::cout << name <<"'s gun is not loaded, they couldn't shoot" << std::endl<<std::endl;
  }
  else{
    enemy.applyDamage();
    std::cout << name << " clubbed their opponent with their weapon." << std::endl<<std::endl;
  }

  took_aim = false;
  agile_target = false;
  has_initiative = false;
}

//reload weapon, expend turn
void player::reload(player enemy){
  gun.load();
  std::cout << name <<"'s weapon is fully loaded" << std::endl<<std::endl;

  took_aim = false;
  agile_target = false;
  has_initiative = false;
}

//checks the players gun for ammo
void player::checkGun()const{
  if(gun.checkLoad())
    std::cout <<name << "'s gun is still loaded" << std::endl<<std::endl;
  else
    std::cout << name <<"'s gun is empty" << std::endl<<std::endl;
}

//checks the computers gun for ammo
bool player::compCheckGun()const{
  return gun.checkLoad();
}

//checks if a d20 roll is greater than acc*dist
bool player::checkHit(player enemy)const{
  bool hit = false;
  double dist = enemy.loc.getDistance(loc);
  int dmod = dist/5;
  double hitCheck = dist*gun.getAccuracy(dmod);
  double roll = rand()%20+1;
  if(took_aim)
    roll = roll + 10;
  if(enemy.checkDodge())
    roll = roll - 5;
  hit = ( roll > hitCheck);


  std::cout << __FUNCTION__ << " " << __LINE__ << " " << roll << " " << dist
            << " " << dmod << " " << hitCheck  << std::endl;
  return hit;
}

//apply damage to other player
void player::applyDamage(weapon gun){
  char gunType = gun.checkType();
  //  std::cout << gunType << '\n'; //debug  
  if(gunType == 'l')
    health = (health - 50);

  if(gunType == 'r')
    health = (health - 40);

  if(gunType == 'c')
    health = (health - 100);
}

//apply melee damage to other player
void player::applyDamage(){
  health = (health - 100);
}


//check agility
 bool player::checkDodge()const{
   return agile_target;
 }

//returns players hp
int player::getHealth()const{
  return health;
}

//checks if player is still alive
bool player::stillAlive()const{
   return (health > 0);
}


//runs single player
void singleplayer(){

  std::cout << "OLD WESTERN" << '\n' << '\n';

  std::cout << "Welcome to town partner, you're the new sheriff around here"       <<'\n' << 
    "and you've already got a bandit here to challenge you. The"        <<'\n' <<
    "rules and duels are simple around here. You'll both walk 10"       <<'\n' <<
    "paces away and then you'll take turns shooting at each other"      <<'\n' <<
    "until one of you drops. Of course you can move around or"          <<'\n' <<
    "steady your aim but those will cost you your turn too. Oh and"     <<'\n' <<
    "be careful not to fire an empty weapon or walk around in a circle" <<'\n' <<std::endl;

  char play = 'y';
  char pname[NAME_SIZE];
  std::cout << "Enter your name(NO_SPACES!): " << std::endl;
  std::cin >> pname;

    
  char action,direction;
  int steps;

  //////////////////////LOOP 1 BEGINS 
  while(play == 'y'){
    
    std::cout << 
      "Choose your weapon sheriff " <<pname << " . You can take"     << '\n' <<
      "the accurate long rifle (l), it packs a decent punch but"     << '\n' <<
      "needs to be reloaded after each shot. We have a two round"    << '\n' <<
      "caravan gun (c) that will devastate that bandit if you"       << '\n' <<
      "manage to land a shot. Or you can take your trusty"           << '\n' <<
      "revolver(r) six rounds with reliable accuracy and damage."    << '\n' <<std::endl;

    char choice,ch;
    std::cin >> choice;
    player bandit;
    player sheriff(pname,choice,-10);
    sheriff.your_turn();
    std::cout << 
      "Great Sheriff, alls that's left is to walk to ten" << '\n' <<
      "paces and begin the duel. By the way, the bandits" << '\n' << 
      "have a strange sense of honor around these parts"  << '\n' <<
      "and will always allow you the first move"          << '\n' << std::endl;

    //LOOP 2 BEGINS
    while((sheriff.stillAlive()) && (bandit.stillAlive())){     
      std::cout << "Bandits HP is: " << bandit.getHealth() << std::endl;
      std::cout << "Sheriffs HP is: " << sheriff.getHealth() << '\n' << std::endl;

      if(sheriff.checkTurn()){
	std::cout << 
	  "It's your move Sheriff, you can shoot(s), move(m), aim(a)," << '\n' <<
	  "reload(r), or check your gun(g)without losing your turn if" << '\n' <<
	  "you havn't been keeping track of your shots."               << '\n' <<std::endl;
	std::cin>> action;
	switch(action){

	case 'm':
	  std::cout << 
	    "You can take up to five steps, which direction would"               << '\n' << 
	    "you like to move? forward(F), backwards(B), left(L), right(R): " << std::endl;
	  std::cin >> direction;
	  std::cout << "How many steps (1-5): " << std::endl;
	  std::cin >> steps; 
	  sheriff.move(direction, steps, bandit);
	  break;

	case 's':
	  sheriff.shoot(bandit);
	  break;

	case 'a':
	  sheriff.aim(bandit);
	  break;

	case 'r':
	  sheriff.reload(bandit);
	  break;

	case 'g':
	  sheriff.checkGun();
	  break;

	default:
	  std::cout << "Don't be a coward sheriff, you have to do something" << std::endl;
	  break;
	}
      }
      else {
	std::cout << "It's the bandits turn now, and he looks hungry for blood" << std::endl;
	if(bandit.compCheckGun()) 
	  bandit.shoot(sheriff);
	else
	  bandit.reload(sheriff);
	sheriff.your_turn();
      }
    }//LOOP 2 ENDS

    if(sheriff.stillAlive()){
      std::cout << 
	"You did it sheriff, but now's not the time to" << '\n' <<
	"celebrate! I can already see another bandit" << '\n' <<
	"heading for town. Are you still up to the challenge? (y/n): " << std::endl;
      std::cin >> play;
    }
    else{
      std::cout << 
	"Looks like " << pname << " wasn't cut out for this"          << '\n' <<
	"line of work. Maybe the next sheriff will have better luck." << '\n' << 
	"GAME OVER!"                                                  << '\n' << std::endl;
      play = 'n';
    }
  }//LOOP 1 ENDS

}


//runs multiplayer
void multiplayer(){
  std::cout << "OLD WESTERN:IT TAKES TWO TO TANGO EDITION" << '\n' << '\n';

  std::cout << "Alright partners, you know the law for feuds around here." <<'\n' << 
    "If you want to kill each other that's fine, "                         <<'\n' <<
    "but you gotta do it the legal way. You'll both walk 10"               <<'\n' <<
    "paces away and then you'll take turns shooting at each other"         <<'\n' <<
    "until one of you drops. Of course you can both move around or"        <<'\n' <<
    "steady your aim but those will cost you your turn too. Oh and"        <<'\n' <<
    "be careful not to fire an empty weapon or walk around in a circle"    <<'\n' <<std::endl;

  char play = 'y';
  char p1name[NAME_SIZE];
  char p2name[NAME_SIZE];
  std::cout << "Enter your name player one(NO_SPACES!): " << std::endl;
  std::cin >> p1name;
  std::cout << "Enter your name player two(NO_SPACES!): " << std::endl;
  std::cin >> p2name;
    
  char action,direction;
  int steps;

  //////////////////////LOOP 1 BEGINS 
  while(play == 'y'){
    char p1choice, p2choice;
    
    std::cout << 
      "Choose your weapon " <<p1name << " . You can take"    << '\n' <<
      "the long rifle (l) [Damage:50 Ammo:1 Range:Far]"      << '\n' <<
      "the caravan gun (c)[Damage:100 Ammo:2 Range:Close] "  << '\n' <<
      "or the revolver(r) [Damage:40 Ammo:6 Range:Average]." << '\n' <<std::endl;
    std::cin >> p1choice;

    std::cout << 
      "Choose your weapon " <<p2name << " . You can take"    << '\n' <<
      "the long rifle (l) [Damage:50 Ammo:1 Range:Far]"      << '\n' <<
      "the caravan gun (c)[Damage:100 Ammo:2 Range:Close] "  << '\n' <<
      "or the revolver(r) [Damage:40 Ammo:6 Range:Average]." << '\n' <<std::endl;
    std::cin >> p2choice;

    player player1(p1name, p1choice, -10);
    player player2(p2name, p2choice, 10);

    std::cout << 
      "Alright you two. " << p1name << " walk 10 paces backwards, and " << '\n' << p2name <<
      "walk 10 paces forwards and begin the duel."                      << '\n' << 
      "I'll flip a coin to see who goes first."                         << '\n' << std::endl;

    int firstTurn = (rand()%2 + 1);

    if(firstTurn == 1){
      std::cout << p1name <<" gets the first move" << std::endl;
      player1.your_turn();
    }
    else{
      std::cout << p2name <<" gets the first move" << std::endl;
      player2.your_turn();
    }


    //LOOP 2 BEGINS
    while((player1.stillAlive()) && (player2.stillAlive())){     
      std::cout << p1name << "'s HP is: " << player1.getHealth() << std::endl;
      std::cout << p2name << "'s HP is: " << player2.getHealth() << '\n' << std::endl;

      if(player1.checkTurn()){
	std::cout << 
	  "It's your move " << p1name <<", you can shoot(s), move(m), aim(a), reload(r)" << '\n' <<
	  "or check your gun(g)if you havn't been keeping track of your shots." << '\n' <<std::endl;
	std::cin>> action;
	switch(action){

	case 'm':
	  std::cout << 
	    "You can take up to five steps, which direction would"               << '\n' << 
	    "you like to move? forward(F), backwards(B), left(L), right(R): " << std::endl;
	  std::cin >> direction;
	  std::cout << "How many steps (1-5): " << std::endl;
	  std::cin >> steps; 
	  player1.move(direction, steps, player2);
	  break;

	case 's':
	  player1.shoot(player2);
	  break;

	case 'a':
	  player1.aim(player2);
	  break;

	case 'r':
	  player1.reload(player2);
	  break;

	case 'g':
	  player1.checkGun();
	  break;

	default:
	  std::cout << "Don't be a coward, you have to do something" << std::endl;
	  break;
	}
	player2.your_turn();
      }
      else {
	std::cout << 
	  "It's your move " << p2name <<", you can shoot(s), move(m), aim(a), reload(r) " << '\n' <<
	  "or check your gun(g)if you havn't been keeping track of your shots." << '\n' <<std::endl;
	std::cin>> action;
	switch(action){

	case 'm':
	  std::cout << 
	    "You can take up to five steps, which direction would"               << '\n' << 
	    "you like to move? forward(B), backwards(F), left(R), right(L): " << std::endl;
	  std::cin >> direction;
	  std::cout << "How many steps (1-5): " << std::endl;
	  std::cin >> steps; 
	  player2.move(direction, steps, player1);
	  break;

	case 's':
	  player2.shoot(player1);
	  break;

	case 'a':
	  player2.aim(player1);
	  break;

	case 'r':
	  player2.reload(player1);
	  break;

	case 'g':
	  player2.checkGun();
	  break;

	default:
	  std::cout << "Don't be a coward, you have to do something" << std::endl;
	  break;
	}
	player1.your_turn();  
      }
    }//LOOP 2 ENDS

    if(player1.stillAlive()){
      std::cout << 
	"Guess you'll live another day " << p1name       << '\n' <<
	"Does anybody else want to a duel today?(y/n): " << std::endl;
      std::cin >> play;
    }
    else{
      std::cout << 
        "Looks like " << p2name << " can postpone their funeral" << '\n' <<
	"Does anybody else want to duel today?(y/n): "           << '\n' << std::endl;
      std::cin >> play;
    }
  }//LOOP 1 ENDS

}

