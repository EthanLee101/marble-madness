#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), finishCheck(false), bonus(1000)
{
    m_player = nullptr;
    m_actors.clear();
}

bool StudentWorld::validPos(double x, double y) {

    for (Actor* a : m_actors) {

        if (x == a->getX() && y == a->getY() && a->blockMove() &&a->isAlive()) {
            return false;
        }

    }

    return true;
}

bool StudentWorld::validPeaPos(double x, double y) {

    for (Actor* a : m_actors) {

        if (x == a->getX() && y == a->getY() && a->stopsPea()&& a->isAlive()) {
          
            return false;
        }

    }

    return true;

}

bool StudentWorld::validActorPos(double x, double y) {

    for (Actor* a : m_actors) {

        if (x == a->getX() && y == a->getY() && (a->blockMove() && a->isAlive() || a->isPickup())) {
            return false;
        }

    }

    return true;

}

bool StudentWorld::validPickUp(double x, double y) {

    for (Actor* a : m_actors) {

        if (a->getX() == x && a->getY() == y && a->isAmmo()&&a->isAlive()) {
            a->returnA();
            a->setDead();
            return true;
        }else if (a->getX() == x && a->getY() == y && a->isExtra()&& a->isAlive()) {
            a->returnE();
            a->setDead();
            return true;
        }
        else if (a->getX() == x && a->getY() == y && a->isRestore()&& a->isAlive()) {
            a->returnR(); 
            a->setDead();
            return true; 
        }

    }
    return false;

}

void StudentWorld :: returnSteal(double x, double y) {
   
    for (Actor* a : m_actors) {

        if (a->stolenR()) {
            a->moveTo(x, y); 
            a->setAlive();
        }
        else if (a->stolenE()) {
            a->moveTo(x, y);
            a->setAlive();
        }
        else if (a->stolenA()) {
            a->moveTo(x, y);
            a->setAlive();

        }


    }
    return; 

}

bool StudentWorld::attacked(double x, double y) {

    for (Actor* a : m_actors) {

        if (x == a->getX() && y == a->getY() && a->isDamageable()&&a->stopsPea()&&a->isAlive()) {
            a->tryToBeKilled(2);
            return true;
        }


    }

    if (findPlayer()->getX() == x && findPlayer()->getY() == y) {
        m_player->tryToBeKilled(2);
        return true;
    }

    return false;
    
}


bool StudentWorld::acceptsMarble(double x, double y) {

    for (Actor* a : m_actors) {

        if (a->getX() == x && a->getY()== y && a->collectMarbles()) {

            a->changeFill(true);
            return true;
        }

    }

    return false; 

}

bool StudentWorld::checkCrystals() {

    for (Actor* a : m_actors) {

        if (a->isCrystal() && a->isAlive())return false;

    }

    return true; 

}

bool StudentWorld::clearShot(double x1, double y1,double x2, double y2 ,int dir) {

   
   
        if (x1 == x2 && dir == 90 && y1 < y2) { // same column dir up 

            while (y1 != y2) {
                y1++;
                if (!validPeaPos(x2, y1))return false;
            }
            return true;

        }
        else if (x1 == x2 && dir == 270 && y1 > y2)//same column dir down 
        {
            while (y1 != y2) {
                y1--;
                if (!validPeaPos(x2, y1))return false;
            }
            return true;
        }
        else if (y1 == y2 && dir == 180 && x1> x2) {

            while (x1 != x2) {
                x1--;
                if (!validPeaPos(x1, y2))return false;
            }
            return true; 
        }
        else if (y1 == y2 && dir == 0&&x1 < x2) {

            while (x1 != x2) {
                x1++;
                if (!validPeaPos(x1, y2))return false;
            }
            return true; 
        }
        return false;
    
}

void StudentWorld::validPush(double x, double y) {

   //if the next move is a marble check if the marble can be moved using valid pos. 
    //if valid pos returns true we move the marble to the next adjacent spot and m_player replaces the marble. 

    int dir = m_player->getDirection(); 
    
    for (Actor* a : m_actors) 
    {
        int i = a->getX();
        int j = a->getY();

        if (a->bePushedBy(m_player)&& x == i && y == j) 
        {
            switch (dir) 
            {

            case 90: // up 
                if (acceptsMarble(x, y+1)) {
                    a->setDead();
                    m_player->moveTo(x, y);
                }
                else if (validActorPos(x, y + 1)) {
                    a->moveTo(x, y + 1); 
                    m_player->moveTo(x, y); 
                }
                break; 
            case 270://down
                if (acceptsMarble(x, y-1)) {
                    a->setDead();
                    m_player->moveTo(x, y);
                }
                else if (validActorPos(x, y - 1) ) {
                    a->moveTo(x, y - 1);
                    m_player->moveTo(x, y);
                }
                break; 
            case 180://left
                if (acceptsMarble(x - 1, y)) {
                    a->setDead();
                    m_player->moveTo(x, y);
                }
                else if (validActorPos(x-1, y )) {
                    a->moveTo(x-1, y);
                    m_player->moveTo(x, y);
                }
                break; 
            case 0://right
                if (acceptsMarble(x + 1, y)) {
                    a->setDead(); 
                    m_player->moveTo(x, y);
                }
                else if (validActorPos(x +1, y)) {
                    a->moveTo(x + 1, y);
                    m_player->moveTo(x, y);
                }
                break; 

             }

        }
        
    }
    
}


// add actors 

bool StudentWorld::addActor(Actor* a, int x, int y) {
    if (validPos(x, y)) {
        m_actors.push_back(a);
        return true;
    }
    return false;

}
bool StudentWorld::addPlayer(int x, int y) {
    if (validPos(x, y)) {
        m_player = new Avatar(this, x, y);
        return true;
    }
    return false;
}

bool StudentWorld::addPea(int x, int y,int dir) {

     m_actors.push_back(new Pea(this, x, y, dir));
    return true;
  

}

bool StudentWorld::addThiefBot(int x, int y) {

    m_actors.push_back(new ThiefBot(this, x, y,5,IID_THIEFBOT));return true;
   

    return false; 
}

bool StudentWorld::addMeanThiefBot(int x, int y) {

    m_actors.push_back(new MeanThiefBot(this, x, y));return true;

    return false; 

}
//end add actors

bool StudentWorld::thiefConsensus(double x, double y) {


    int countT = 0;

    for (Actor* a : m_actors)
    {
        

        if (a->getX() == x && a->getY() == y && a->isThief())return false; 

        int minX = x - 3;
        int maxX = x + 3; 
        int minY = y - 3;
        int maxY = y + 3;

        if (a->getX() >= minX && a->getX() <= maxX && a->getY() >= minY && a->getY() <= maxY && a->isThief())countT++;
        
        
     
    }

    

    if (countT < 3) return true;

    return false;
 
}

int StudentWorld::init()
{
    setFinish(false);
    Level Lev(assetPath());
    string curLevel = "level0" + to_string(getLevel()) + ".txt";
    Level::LoadResult result = Lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found)
        return -1;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success) {

        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                Level::MazeEntry item = Lev.getContentsOf(i, j);
                switch (item) {
                case Level::player:
                    addPlayer(i, j);
                    break;
                case Level::wall:
                    addActor(new wall(this, i, j), i, j);
                    break;
                case Level::marble:
                    addActor(new Marble(this, i, j), i, j);
                    break;
                case Level::pit:
                    addActor(new Pit(this, i, j), i, j);
                    break;
                case Level::crystal:
                    addActor(new Crystal(this, i, j), i, j);
                    break;
                case Level::exit:
                    addActor(new Exit(this, i, j), i, j);
                    break;
                case::Level::extra_life:
                    addActor(new ExtraLifeGoodie(this, i, j), i, j);
                    break;
                case::Level::restore_health:
                    addActor(new RestoreGoodie(this, i, j), i, j); 
                    break;
                case::Level::ammo:
                    addActor(new AmmoGoodie(this, i, j), i, j); 
                    break;
                case::Level::horiz_ragebot:
                    addActor(new RageBot(this, i, j, 0),i,j);
                    break;
                case::Level::vert_ragebot:
                    addActor(new RageBot(this, i, j, 270), i, j);
                    break;
                case::Level::thiefbot_factory:
                    addActor(new ThiefFactory(this, i, j, 1), i, j); 
                    break;
                case::Level::mean_thiefbot_factory:
                    addActor(new ThiefFactory(this, i, j, 2), i, j);
                    break;
                default:
                    break;
                }
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
  
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    string m_score = "Score:  " + to_string(getScore()); m_score += "  ";

    string m_level = "Level:  " + to_string(getLevel()); m_level += "  ";

    string m_lives = "Lives: " + to_string(getLives()); m_lives += "  ";

    int p_health = m_player->getHitPoints() / 20 * 100; 

    string t_health = "Health:  " + to_string(p_health);

    t_health += "%  "; 

    string ammoNum = "Ammo:  " + to_string(m_player->ammoCount()); ammoNum += "  ";

    string bonusAmt = "Bonus: " + to_string(bonus); bonusAmt += "  "; 

    string final = m_score + m_level + m_lives + t_health + ammoNum + bonusAmt; 

    // text at the top; 

   if(m_player->isAlive())setGameStatText(final);

    if (bonus <= 0) { bonus = 0;}else  bonus--; 

    if(m_player->isAlive())m_player->doSomething();

    for (Actor* a : m_actors) {

        if (a->isAlive())a->doSomething();
        else a->setVisible(false); 
       
   
    }

    if (finishCheck)return GWSTATUS_FINISHED_LEVEL; 

    if (!m_player->isAlive())return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a : m_actors)
        delete a;

    m_actors.clear();
    delete m_player;
    m_player = nullptr;

}

StudentWorld::~StudentWorld() {
    cleanUp();
}

