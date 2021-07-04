#ifndef aux_functions
#define aux_functions

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <conio.h>
#include "board.h"
#include "dictionary.h"

typedef map<string, pair<size_t, vector<string>>> suggestionMap;


/**
 * Asks the user for the name (file or player)
 *
 * @param	msg		message to show to the user
 * @return			string representing the name
 */
string getName(string msg, string msgErr);

/**
 * Displays all the elements from a vector separated by ';'.
 *
 * @param	vec		vector of strings we want to display
 */
void showVector(vector<string> vec);

/**
 * Validates the string containing the coordenates and direction of a word in a board.
 *
 * @param	coords	string containing the position
 * @return			true if the string is in the correct format, false otherwise
 */
bool validCoordenates(string& coords);

/**
 * Gets the position of a the first cell of a word from the user.
 *
 * @param	coords	string containing the position
 * @return			false if the user enters CTRL-Z to finish the game
 */
bool getCoordenates(string& coordsInput);

/**
 * Asks the user for a number between two limits.
 *
 * @param	question	message to be displayed when asking for the number
 * @param	errorMsg	message to be displayed if the input is invalid
 * @param	limInf		lowest valid number
 * @param	limSup		highest valid number
 * @return				user entered number
 */
int getInputNumber(string question, string errorMsg, int limInf, int limSup);

/**
 * Gives a random integer between two given numbers(don't forget to seed the RNG in the main function).
 *
 * @param	n1	bound
 * @param	n2	other bound
 * @return		random between n1 and n2 (n1 and n2 included)
 */
int randomBetween(int n1, int n2);

/**
 * Fills a map(key = number of save file; value = name of save file) with the existent save files.
 *
 * @param	saveFiles	map to be altered
 */
void getUsedSaveFiles(map<int, string>& saveFiles);

/**
 * Chooses a certain ammount of words, randomly, from the "pool" of suggestions and prints them out one per line, with a dot before
 * If numerOfSuggestions is -1 all the available suggestions are shown
 *
 * @param	vec						vector containing the pool of suggestions
 * @param numberOfSuggestions		number of suggestions to be shown to the user
 */
void showSuggestions(vector<string>& vec, int numberOfSuggestions);

/**
 * Show to the screen the horizontal and vertical suggestions
 *
 * @param	hSuggestions	reference to the horizontal suggestions
 * @param	vSuggestions	reference to the vertical suggestions
 */
void showGameSuggestions(suggestionMap& hSuggestions, suggestionMap& vSuggestions);

/**
 * Increase the number of synonyms shown for a given word. Return 0 if the count was changed, 1 if the count wasn't changed
 * (this happens when all the synonyms are already being shown), and 2 if a word doesn't start there.
 *
 * @param	mapSuggestions		the map of suggestions
 * @param	coords				coordenates of the word in the board
 * @param	dictionary			reference to the dictionary associated with the board
 * @param	word				word that belongs in that position
 * @return						true if the count was increase, false otherwise
 */
int changeSuggestion(suggestionMap& mapSuggestions, string coords, Dictionary& dictionary, string word);

/**
 * Build, for the given words of a board, two suggestion maps for the horizontal and vertical words.
 * A suggestion map consists of a pair with the coordenates of the word on the left, and a pair on the
 * right that has the number of suggestions to be shown on the left and the vector if synonyms of that
 * word on the right. 
 *
 * @param	hSuggestions	reference to the horizontal suggestions
 * @param	vSuggestions	reference to the vertical suggestions
 * @param	board			reference to the board containing the words
 * @param	dictionary		reference to the dictionary associated with the board
 */
void buildGameSuggestions(suggestionMap& hSuggestions, suggestionMap& vSuggestions, Board& board, Dictionary& dictionary);

/**
 * Finds if a certain word exists in the given vector
 *
 * @param vector	vector of strings
 * @param word	word to be seached in the vector
 * @return		true if the word is present in the vector, false otherwise
 */
bool vectorSearch(vector<string>& vector, string word);


#endif 

