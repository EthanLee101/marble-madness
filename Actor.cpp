#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool AliveActor::validMove() {

	int dir = getDirection(); 
	switch (dir) {
	case up:
		if(isPlayer())getWorld()->validPush(getX(), getY() + 1); 
		if (getWorld()->validPos(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			return true; 
		}
		break;
	case down:
		if (isPlayer())getWorld()->validPush(getX(), getY() - 1);
		if (getWorld()->validPos(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		break;
	case left:
		if (isPlayer())getWorld()->validPush(getX()-1, getY());
		if (getWorld()->validPos(getX() - 1, getY())) {
			moveTo(getX() - 1, getY());
			return true;
		}
		break;
	case right:
		if (isPlayer())getWorld()->validPush(getX() + 1, getY());
		if (getWorld()->validPos(getX() + 1, getY())) {
			moveTo(getX() + 1, getY());
			return true;
		}
		break; 
	default:
		return false;
		break;
	}
	
	return false; 
}

void AliveActor :: peaAttack() {

	int dir = getDirection();
	int x = getX();
	int y = getY();

	switch (dir) {

	case up:
		getWorld()->addPea(x, y + 1,up);
		break;
	case down:
		getWorld()->addPea(x, y - 1,down);
		break;
	case left:
		getWorld()->addPea(x - 1, y,left);
		break;
	case right:
		getWorld()->addPea(x + 1, y,right);
		break;
	default:
		break;

	}

}

void Robot::setCount() {

	m_ticks = (28 - (getWorld()->getLevel())) / 4;
	return; 

}

void RageBot::rvalidMove() {

	int dir = getDirection(); 
	if (!validMove()) {
		switch (dir) {
		case up:
			setDirection(down);
			break;
		case down:
			setDirection(up);
			break;
		case left:
			setDirection(right);
			break;
		case right:
			setDirection(left);
			break;
		default:
			break; 


		}


	}
	return; 

}


void RageBot::doSomething() {

	int dir = getDirection();

	int cx = getX();
	int cy = getY();

	int px = getWorld()->findPlayer()->getX();
	int py = getWorld()->findPlayer()->getY();

	setCount(); 
	
	if (!isAlive())return; 

	if (currentTick() == tickCount()) {
		
		if (getWorld()->clearShot(cx, cy, px, py, dir) && !shootAlready()) {
			peaAttack();
			changeShot(true);
			return;
		}
		else if (getWorld()->clearShot(cx, cy, px, py, dir))
		{
			if (currentTick() >= tickCount()) { setCTick(0);changeShot(false); }
			return;
		}
		else
		{
			rvalidMove();
			if (currentTick() >= tickCount()) { setCTick(0);changeShot(false); }
			return;
		}
	}
	else {
		incTick(); 
	}
	
	return;
}

bool RageBot::tryToBeKilled(int damageAmt) {
	
		damage(damageAmt);
		hurtSound(); 
		if (isAlive())return false;
		else { dieSound();setDead();getWorld()->increaseScore(100);return true; }
	
}

void ThiefBot::baseDoSomething() {

	if (moved < randDistance && validMove()) {
		if (currentTick() >= tickCount()) { setCTick(0); }
		moved++;
		return;
	}
	else {
		if (currentTick() >= tickCount()) { setCTick(0); }
		moved = 0;
		int rand = randInt(1, 4);
		int dir = 0;

		switch (rand) {
		case 1:
			dir = up;
			break;
		case 2:
			dir = down;
			break;
		case 3:
			dir = left;
			break;
		case 4:
			dir = right;
			break;
		default:
			break;
		}

		switch (dir) {
		case up:
			if (!validMove()) {
				setDirection(left);
			}
			else if (!validMove()) {
				setDirection(down);
			}
			else if (!validMove()) {
				setDirection(right);
			}
			else if (!validMove()) {
				return;
			}
			break;
		case down:
			if (!validMove()) {
				setDirection(right);
			}
			else if (!validMove()) {
				setDirection(left);
			}
			else if (!validMove()) {
				setDirection(up);
			}
			else if (!validMove()) {
				return;
			}
			break;
		case left:
			if (!validMove()) {
				setDirection(down);
			}
			else if (!validMove()) {
				setDirection(up);
			}
			else if (!validMove()) {
				setDirection(right);
			}
			else if (!validMove()) {
				return;
			}
			break;
		case right:
			if (!validMove()) {
				setDirection(up);
			}
			else if (!validMove()) {
				setDirection(left);
			}
			else if (!validMove()) {
				setDirection(down);
			}
			else if (!validMove()) {
				return;
			}
			break;
		default:
			break;
		}


	}

}
void ThiefBot::doSomething() {

	setCount();

	int cx = getX();
	int cy = getY();

	if (!isAlive())return;

	if (currentTick() == tickCount()) 
	{

		
		if (!isGoodieStolen()&& getWorld()->validPickUp(cx,cy)) //!isGoodieStolen()&&getWorld()->validPickUp(cx, cy)
		{
		stealing(true);
		getWorld()->playSound(SOUND_ROBOT_MUNCH); 
		}
		else 
		{
				baseDoSomething(); 
				if (currentTick() >= tickCount()) { setCTick(0); }
		}


	}
	else 
	{

		incTick();
		return; 
		
	}

	return;

}

bool ThiefBot::tryToBeKilled(int damageAmt) {

	damage(damageAmt);
	hurtSound();
	if (isAlive())return false;
	else { dieSound();setDead();getWorld()->increaseScore(10);getWorld()->returnSteal(getX(),getY());return true; }

}

bool MeanThiefBot::tryToBeKilled(int damageAmt) {
	damage(damageAmt);
	hurtSound();
	if (isAlive())return false;
	else { dieSound();setDead();getWorld()->increaseScore(20);getWorld()->returnSteal(getX(), getY());return true; }
}

void MeanThiefBot::doSomething() {

	setCount();

	int dir = getDirection();

	int px = getWorld()->findPlayer()->getX();
	int py = getWorld()->findPlayer()->getY();

	int cx = getX();
	int cy = getY();

	if (!isAlive())return;

	if (currentTick() == tickCount())
	{

		if (getWorld()->clearShot(cx, cy, px, py, dir) && !shootAlready()) {
			peaAttack();
			changeShot(true);
			return;
		}
		else if (getWorld()->clearShot(cx, cy, px, py, dir))
		{
			if (currentTick() >= tickCount()) { setCTick(0);changeShot(false); }
			return;

		}else if (!isGoodieStolen() && getWorld()->validPickUp(cx, cy)) //!isGoodieStolen()&&getWorld()->validPickUp(cx, cy)
		{
			stealing(true);
		}
		else
		{
			baseDoSomething();
			if (currentTick() >= tickCount()) { setCTick(0); }
		}


	}
	else
	{

		incTick();
		return;

	}

	return;

}

 void Robot::hurtSound() { getWorld()->playSound(SOUND_ROBOT_IMPACT);return; }

 void Robot::dieSound() { getWorld()->playSound(SOUND_ROBOT_DIE);return; }

 void Avatar::hurtSound() { getWorld()->playSound(SOUND_PLAYER_IMPACT);return; }

 void Avatar::dieSound() { getWorld()->playSound(SOUND_PLAYER_DIE);return; }


bool Avatar::tryToBeKilled(int damageAmt) {

		damage(damageAmt);
		hurtSound();
		if (isAlive())return false;
		else { dieSound();setDead();getWorld()->decLives();return true; }
}


void Avatar::shootPea() {

	if (m_pAmnt > 0) {
		peaAttack(); 
		getWorld()->playSound(SOUND_PLAYER_FIRE);
		m_pAmnt--;
		 
	}

}


void Avatar::doSomething() {

	if (!isAlive())return;

	int c;

	if (getWorld()->getKey(c)) {

		switch (c) {

		case KEY_PRESS_LEFT:
			setDirection(left);
			validMove();
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			validMove();
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			validMove();
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			validMove();
			break;
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_SPACE:
			shootPea();
			break;
		default:
			break;

		}

	}

	return;

}

void Pea::doSomething() {



		if (!isAlive())return; // checks if Pea if still valid 

		double x = getX();
		double y = getY();

		// checks if the Pea should damage other actors or be removed 
		if (getWorld()->attacked(x, y))setDead();
		else if (!getWorld()->validPeaPos(x, y))setDead();

		//if Pea passes the tests then it continues moving 

		switch (c_Dir) {

		case up:
			moveTo(x, y + 1);
			break;
		case down:
			moveTo(x, y - 1);
			break;
		case left:
			moveTo(x-1, y);
			break;
		case right:
			moveTo(x + 1, y);
			break;
		default:
			break;

		}

	

}

void Pit::doSomething() {

	if (!isAlive())return; 

	if (checkFill())setDead();
	
	return; 

}

void Exit::doSomething() {

	

	int px = getWorld()->findPlayer()->getX();
	int py = getWorld()->findPlayer()->getY();

	if (getWorld()->checkCrystals()&&!reveal) {

		setVisible(true); 
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		reveal = true; 
	}

	if (px == getX() && py == getY() && isVisible() &&!finish) {

		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000 + getWorld()->bonusReturn());
		getWorld()->setFinish(true); 
		finish = true; 
		

	}
	

}

bool PickUp::baseDoSomething() {

	if (!isAlive())return false;

	int px = getWorld()->findPlayer()->getX();
	int py = getWorld()->findPlayer()->getY();

	if (px == getX() && py == getY()) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		setDead();
		getWorld()->increaseScore(giveScore());
		return true;
	}
	return false;
}
void Crystal::doSomething() {

	baseDoSomething(); 

}

void ExtraLifeGoodie::doSomething() {

	if (baseDoSomething()) {
		getWorld()->incLives(); 
	}

}

void RestoreGoodie::doSomething() {

	if (baseDoSomething()) {
		getWorld()->findPlayer()->healFull();
	}
}

void AmmoGoodie::doSomething() {

	if (baseDoSomething()) {

		getWorld()->findPlayer()->ammoFill(); 

	}
}

void ThiefFactory::doSomething() {

	int cx = getX(); 
	int cy = getY(); 

	
		

			if (m_type == 1) {
				if (getWorld()->thiefConsensus(cx, cy)) 
				{
					if (randInt(1, 50) == 1) {
					getWorld()->addThiefBot(cx, cy);
					getWorld()->playSound(SOUND_ROBOT_BORN);
					return;
					}
					
				}
			}
			
			if (m_type == 2) {
				if(getWorld()->thiefConsensus(cx, cy))
				{
					if (randInt(1, 50) == 1) {
					getWorld()->addMeanThiefBot(cx, cy);
					getWorld()->playSound(SOUND_ROBOT_BORN);
					return;
					}
					
				}
			}

		
	

	return; 

}