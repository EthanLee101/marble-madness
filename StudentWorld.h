#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include"Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();

	virtual ~StudentWorld();
	virtual void cleanUp();


	//helper functions

	bool validPos(double x, double y); // for  alive actor movement
	bool validActorPos(double x , double y);
	bool validPickUp(double x, double y);  // for theif bots 
	void returnSteal(double x, double y); 
	bool attacked(double x, double y); //for pea damage
	bool validPeaPos(double x, double y); 
	bool clearShot(double x1, double y1, double x2, double y2, int dir); 
	void validPush(double x,double y); //pushing marbles by player
	bool acceptsMarble(double x, double y);//accepting marbles 
	bool checkCrystals(); // checks if level is completed yet 
	void setFinish(bool x) { finishCheck = x; } // level implementation		
	bool thiefConsensus(double x, double y);
	

	//add actors 

	bool addActor(Actor*, int x, int y); 
	bool addPlayer(int x, int y);
	bool addPea(int x, int y, int dir); 
	bool addThiefBot(int x, int y); 
	bool addMeanThiefBot(int x, int y); 
	

	//accessors

	Avatar* findPlayer() { return m_player; }
	int bonusReturn() { return bonus; }


private:
	vector<Actor*> m_actors;
	Avatar* m_player;
	bool finishCheck; 
	int bonus; 

	
};

#endif // STUDENTWORLD_H_
