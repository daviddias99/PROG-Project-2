#ifndef player_h
#define player_h

#include <string>
#include <ctime>

using namespace std;

typedef unsigned int uint;

/**
* The Player class contains information about the player(designed for the cwplayer program), such as the player Name,
* the number of clues he/she asked during the game and the time he took to finish. It contains methods to get and set
* the player name, the number of clues asked and to start/stop the tracking of the time the player took to finish the 
* game.
 */

class Player
{
public:

	//---------- CONSTRUCTORS ---------- 

	/**
	* Creates a new Player, inicializing the timeStart variable with the current time, the number of cluesAsked with 0 and the playerName with
	* "Default player name"
	*/
	Player();

	/**
	* Creates a new Player, inicializing the timeStart variable with the current time, the number of cluesAsked with 0 and the playerName with
	* the string given by the user
	*
	* @param	playerName	name to initiliaze the class playerName variable
	*/
	Player(string playerName);


	//---------- PUBLIC MEMBER FUNCTIONS ---------- 

	/**
	* Returns the name of the player
	*
	* @return	name of the player associated with the instance of the class
	*/
	string getPlayerName();

	/**
	* Returns the time it took the player to finish the game. If that time isn't defined yet
	* the function "ends" the game and returns the time.
	*
	* @return	time, in seconds, the player took to finish the game
	*/
	time_t getTimeToComplete();

	/**
	* Returns the number of clues the player asked for in the current game
	*
	* @return	number of clues asked by the player
	*/
	uint getCluesAsked();

	/**
	* Changes the player name to the string given by the user
	*
	* @param playerName	name to change the playerName attribute to
	*/
	void setPlayerName(string playerName);

	/**
	* Increment the clue counter
	*/
	void increaseClueCounter();

	/**
	* Start the game clock (set the timeStart attribute to the current time)
	*/
	void startGame();

	/**
	* End the game clock (set the timeFinish attribute to the current time and calculate the timeToComplete variable)
	*/
	void endGame();


private:

	//---------- PRIVATE ATTRIBUTES ---------- 

	string playerName; //Name of the player
	
	uint cluesAsked; //Number of clues the player has asked

	time_t timeStart; //Timestamp of the time the player started the game
	time_t timeFinish; //Timestamp of the time the player finished the game
	time_t timeToComplete = -1; //Number of seconds the player took to finish the game

};

#endif


