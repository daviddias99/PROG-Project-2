#include "player.h"



Player::Player()
{
	this->timeStart = time(NULL);
	this->cluesAsked = 0;
	this->playerName = "Default player name";
}


Player::Player(string playerName)
{
	this->timeStart = time(NULL);
	this->cluesAsked = 0;
	this->playerName = playerName;
}



string Player::getPlayerName()
{
	return this->playerName;
}



time_t Player::getTimeToComplete()
{
	if (this->timeToComplete== -1)
	{
		this->endGame();
	}

	return this->timeToComplete;
}


uint Player::getCluesAsked()
{
	return this->cluesAsked;
}


void Player::setPlayerName(string playerName)
{
	this->playerName = playerName;
}



void Player::increaseClueCounter()
{
	this->cluesAsked++;
}



void Player::startGame()
{
	this->timeStart = time(NULL);
}


void Player::endGame()
{
	this->timeFinish = time(NULL);
	this->timeToComplete =  this->timeFinish - this->timeStart;
}
