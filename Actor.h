
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class AliveActor;

class Actor : public GraphObject {

public:

	Actor(StudentWorld* world, int imageID, int startX, int startY, int dir = none,int health =0, double size = 1.0)
		:GraphObject(imageID, startX, startY, dir, size), m_world(world), m_health(health),isFilled(false), extraGoodie(false), restoreGoodie(false), ammoGoodie(false),m_mconsensus(0),m_rconsensus(0) {}

	virtual void doSomething() = 0;

	// How many hit points does this actor have left?
	virtual double getHitPoints() const { return m_health; }

	//can actor be hurt 
	virtual bool isDamageable() { return false; }

	//is actor alive
	virtual bool isAlive() { if (m_health <= 0)return false;else return true; }

	void healFull() { m_health = 20;return; }

	// Mark this actor as dead
	void setDead() { m_health = 0;setVisible(false);return; }

	void setAlive() { m_health = 1; setVisible(true);return; }

	// Reduce this actor's hit points
	void decHitPoints(int amt) { m_health -= amt;return; }

	// Cause this Actor to sustain damageAmt hit points of damage.
	virtual void damage(int damageAmt) { decHitPoints(damageAmt);return; }

	//tries to kill the actor 
	virtual bool tryToBeKilled(int damageAmt) {
		damage(damageAmt);
		if (isAlive())return true;
		else setDead();return false;
	}

	// is this a pit? 
	
	virtual bool collectMarbles() const { return false; }

	void changeFill(bool x) { isFilled = x;return; }

	bool checkFill() { return isFilled; }

	//is this an exit? 

	virtual bool isExit() const { return false; }

	// is this a crystal? 

	virtual bool isCrystal() const { return false; }

	//is this a pickupable item?

	virtual bool isPickup() const { return false; }

	//is this item a goodie

	virtual bool isAmmo()const { return false; }
	virtual bool isRestore()const { return false; }
	virtual bool isExtra()const { return false; }
	

	//stops peas 
	virtual bool stopsPea() const { return true; };

	//blocks movement
	virtual bool blockMove()const { return true; }

	// Can this actor be pushed by a to location x,y?
	virtual bool bePushedBy(AliveActor* a) { return false; }

	//returns the world of the actor 
	StudentWorld* getWorld() { return m_world; }

	//is it a thief? 

	virtual bool isMeanThief() const { return false; }
	virtual bool isThief()const { return false; }

	//is it a factory?
	
	virtual bool isFactory() const { return false; }

	//increases consensus 

	void inc_rconsensus() { m_rconsensus++;return; }
	void inc_mconsensus() { m_mconsensus++; return; }

	int return_mc() { return m_mconsensus; }
	int return_rc() { return m_rconsensus; }


	void returnR() { restoreGoodie = true;return; }
	void returnE() { extraGoodie = true;return; }
	void returnA() { ammoGoodie = true;return; }

	bool stolenR()const { return restoreGoodie; }
	bool stolenE()const { return extraGoodie; }
	bool stolenA()const { return ammoGoodie; }

	//is player?

	virtual bool isPlayer()const { return false; }

private:

	//health 
	int m_health; 

	//is pit filled 
	bool isFilled; 

	//thief bots and readding them after it dies 
	bool extraGoodie;
	bool restoreGoodie;
	bool ammoGoodie;

	//amount of thief bots in a 3x3 area;
	
	int m_rconsensus;
	int m_mconsensus;

	//current world 
	StudentWorld* m_world;

};

class AliveActor : public Actor {

public:

	AliveActor(StudentWorld* world, int imageID, int startX, int startY, int direction, int health = 0) :
		Actor(world, imageID, startX, startY, direction,health, 1.0) {}

	
	// shoot peas 
	void peaAttack(); 
	
	//is the alive actor the player
	virtual bool canPushMarbles() const = 0;

	//can the alive actor make a valid move
	virtual bool validMove(); 

	//plays the hurt and die sounds 
	virtual void hurtSound() = 0; 
	virtual void dieSound() = 0;

	//alive actors are damageable
	
	virtual bool isDamageable() { return true; }

};

class Robot : public AliveActor {

public:

	Robot(StudentWorld* world, int imageID, int startX, int startY, int dir = none,int health = 0 ):
		AliveActor(world,imageID,startX,startY,dir,health),m_ticks(0),c_ticks(0),alrShot(false){}

	virtual void dieSound();
	virtual void hurtSound();
	virtual bool canPushMarbles()const { return false; }

	//sets the amount of ticks the robot can move
	void setCount();
	int tickCount() const { return m_ticks; }
	void incTick() { c_ticks+=1; return; }
	void setCTick(int x) { c_ticks = x;return; }
	int currentTick() const { return c_ticks; }
	bool shootAlready() {return alrShot;}
	void changeShot(bool x) { alrShot = x;return; }

private:

	int m_ticks;
	int c_ticks; 
	bool alrShot; 

};

class RageBot : public Robot {

public:

	RageBot(StudentWorld* world, int startX, int startY,int dir):
		Robot(world,IID_RAGEBOT,startX,startY,dir,10){}

	virtual void doSomething();  
	virtual bool tryToBeKilled(int damageAmt);
	void rvalidMove(); 
	

};

class ThiefBot : public Robot {

public:

	ThiefBot(StudentWorld* world, int startX, int startY,int health, int imageID) :
		Robot(world, imageID, startX, startY, right, health),randDistance(randInt(1,6)),stoleGoodie(false),moved(0) {}

	virtual void doSomething(); 
	virtual bool tryToBeKilled(int damageAmt);
	void stealing(bool x) { stoleGoodie = x;return; }

	//returns if it already stole item
	bool isGoodieStolen()const {return stoleGoodie;}

	virtual bool isThief()const { return true; }


	void baseDoSomething(); 

private: 

	int randDistance; 
	int moved;
	bool stoleGoodie; 



};

class MeanThiefBot : public ThiefBot {

public:

	MeanThiefBot(StudentWorld* world, int startX, int startY) :
		ThiefBot(world, startX, startY, 8, IID_MEAN_THIEFBOT) {}

	virtual void doSomething();
	virtual bool tryToBeKilled(int damageAmt);
	virtual bool isMeanThief() const { return true; }
};

class Avatar :public AliveActor {

public:

	Avatar(StudentWorld* world, int startX, int startY) :
		AliveActor(world, IID_PLAYER, startX, startY, right, 20), m_pAmnt(20) {}

	//actions 
	virtual void doSomething();
	void shootPea();

	//when attacked
	virtual void hurtSound();
	virtual void dieSound();

	//can push the marble
	virtual bool canPushMarbles()const { return true; }

	//when the player gets hurt 

	virtual bool tryToBeKilled(int damageAmt);
	int ammoCount() const { return m_pAmnt; }
	void ammoFill() { m_pAmnt = 20;return; }


	//is player
	virtual bool isPlayer()const { return true; }

private:

	int m_pAmnt;


};

class Marble : public Actor {

public:

	Marble(StudentWorld* world, int startX, int startY) :
		Actor(world, IID_MARBLE, startX, startY, none, 10,1) {}

	virtual bool bePushedBy(AliveActor* a) { if (a->canPushMarbles())return true;else return false; }
	virtual bool isDamageable(){ return true; }
	virtual void doSomething() { return; }
	

};

class wall : public Actor {

public:

	wall(StudentWorld* world, int startX, int startY) :
		Actor(world, IID_WALL, startX, startY, none,1, 1.0) {}

	// does nothing 
	virtual void doSomething() { return; }
	virtual bool bePushedBy(AliveActor* a, int x, int y) { return true; }

};

class Pit : public Actor {

public:

	Pit(StudentWorld* world, int startX, int startY):
		Actor(world,IID_PIT,startX,startY,none,1,1.0){}

	virtual void doSomething(); 
	virtual bool blockMove()const { return true; }
	virtual bool stopsPea() const { return false; }
	virtual bool collectMarbles()const { return true;  }

};

class Exit :public Actor {

public:

	Exit(StudentWorld* world, int startX, int startY):
		Actor(world, IID_EXIT, startX, startY, none, 1, 1.0),reveal(false),finish(false) {
		setVisible(false);
	}

	virtual void doSomething();
	virtual bool blockMove() const { return false; }
	virtual bool isExit() { return true; }

private:
	bool reveal; 
	bool finish; 


};

class Pea : public Actor {

public:

	Pea(StudentWorld* world, int startX, int startY, int shootDir) :
		Actor(world, IID_PEA, startX, startY, shootDir,1, 1), c_Dir(shootDir) {}


	virtual void doSomething();
	virtual bool blockMove()const { return true; }
	virtual bool stopsPea()const { return false; }
	


private:

	int c_Dir;


};

class PickUp : public Actor {

public: 

	PickUp(StudentWorld* world, int startX, int startY,int imageID,int score):
		Actor(world,imageID,startX,startY,none,1,1.0),g_score(score){}

	bool baseDoSomething(); 
	virtual bool isPickup() const { return true; }
	int giveScore() {return g_score;}
	virtual bool stopsPea() const { return false; }
	virtual bool blockMove() const { return false; }

private: 

	int g_score; 

};

class Crystal : public PickUp {

public:

	Crystal(StudentWorld* world, int startX, int startY) :
		PickUp(world, startX, startY, IID_CRYSTAL, 50) {}

	virtual void doSomething(); 
	virtual bool isCrystal() const { return true; }

};

class ExtraLifeGoodie : public PickUp {

public:

	ExtraLifeGoodie(StudentWorld* world, int startX, int startY):
		PickUp(world ,startX, startY, IID_EXTRA_LIFE,1000){}

	virtual bool isExtra() const { return true;  }
	virtual void doSomething();


};

class RestoreGoodie : public PickUp {

public:

	RestoreGoodie(StudentWorld* world, int startX, int startY):
		PickUp(world,startX,startY,IID_RESTORE_HEALTH,500){}

	virtual bool isRestore()const { return true; }
	virtual void doSomething(); 

};

class AmmoGoodie :public PickUp {

public:

	AmmoGoodie(StudentWorld* world , int startX, int startY):
		PickUp(world, startX, startY, IID_AMMO,100){}

	virtual void doSomething(); 
	virtual bool isAmmo()const { return true; }
};


class ThiefFactory :public Actor {

public:

	ThiefFactory(StudentWorld* world, int startX, int startY, int bot = 0) :
		Actor(world,IID_ROBOT_FACTORY,startX,startY,none,2,1.0),m_type(bot){}

	virtual void doSomething(); 
	virtual bool isFactory() const { return true; }

private: 

	int m_type; 
};

#endif // ACTOR_H_
