#ifndef board_h
#define board_h

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "dictionary.h"
using namespace std;

typedef unsigned int index;
typedef map<string, pair<size_t, vector<string>>> suggestionMap;


/**
 * The Board class contains all the information from a crossword puzzle, including the contents of each position of the board and the
 * words that are written on the board(and the cell where each word starts).
 * The class contains methods to write and remove words from the board, store and load boards from text files and other necessary methods to
 * manage the creation of a crosswoard puzzle.
 */
class Board
{
public:

	//---------- FRIEND FUNCTIONS ---------- 

	/**
	 * Compares two boards to see if they are equal. They are defined as equal if both wordMaps are the same or both boardVectors are equal.
	 *
	 * @return		true if the boards are considered equal, false otherwise
	 */
	friend bool operator==(const Board& left, const Board& right);

	/**
	 * Compares the wordMaps of two boards to assess if they are equal. 
	 *
	 * @param	left	reference to a board
	 * @param	right	reference to a board
	 * @return			true if both board have the same wordMap, false otherwise
	 */
	friend bool equalWordMaps(const Board & left, const Board & right);

	/**
	 * Compares the wordVectors of two boards to assess if they are equal.
	 *
	 * @param	left	reference to a board
	 * @param	right	reference to a board
	 * @return			true if both board have the same wordVector, false otherwise
	 */
	friend bool equalVectors(const Board & left,const Board & right);

	/**
	 * Build, for the given words of a board, two vectors containing the suggestions(synonyms taken from the dictionary
	 * for the horizontal and vertical words. The lines have the following format: word- synonym
	 *
	 * @param	hSuggestions	reference to the horizontal suggestions
	 * @param	vSuggestions	reference to the vertical suggestions
	 * @param	board			reference to the board containing the words
	 * @param	dictionary		reference to the dictionary associated with the board
	 */

	friend void buildGameSuggestions(suggestionMap& hSuggestions, suggestionMap& vSuggestions, Board& board, Dictionary& dictionary);
	
	//---------- CONSTRUCTORS ---------- 

	/**
	 * Default constructor of the class Board, initializes an empty board with 0x0 dimensions.
	 */
	Board();

	/**
	 * Board constructor: initializes an empty board with the given height and width.
	 *
	 * @param	vSize	number of lines
	 * @param	hSize	number of columns
	 */
	Board(unsigned int vSize, unsigned int hSize);

	/**
	 * Board constructor: loads a board from a file stream.
	 *
	 * @param	inputFile	file stream containing the board
	 */
	Board(fstream& inputFile);


	//---------- PUBLIC MEMBER FUNCTIONS ---------- 

	/**
	 * Outputs to the given stream the current board. The user can choose if the axis are shown.
	 *
	 * @param	outputStream	stream where the board will be "shown"(default = cout)
	 * @param	showAxis		true if the the top and left axis should be shown, false otherwise(default = true)
	 */
	void showBoard(ostream& outputStream = cout, bool showAxis = true);

	/**
	 * Writes to the given file the current state of the game. It is stored in the following manner:
	 *  -Name of the dictionary file; empty line; current board(without the axis); empty line; list of coordenates/words
	 *
	 * @param	outputFile		file stream where the gamestate will be stored
	 * @param	dictionaryName	name of the dictionary used to create the board
	 */
	void storeGameState(ostream& outputFile, string dictionaryName);

	/**
	 * Load the current game state from the inputFile to the internal data structures. The inputFile should have the specified format.
	 * This function assumes that the first line as already been read.
	 *
	 * @param	inputFile	file stream containing the desired gamestate
	 * @return				true if the file is loaded successfuly
	 */
	bool loadGameState(istream& inputFile);

	/**
	 * Inserts the given word in the board, starting at the coordenates (coordV,coordH) (alpha-alpha coordenate system), horizontally
	 * or vertically depending on the value of param "orientation" (V or H). The difference between this function and the writeWord function
	 * is that this one checks to see if the word can legally be writen on the board. This function also adds the word and the corresponding
	 * coordenates to the wordMapping map.
	 *
	 * @param	word		the word to be written on the board
	 * @param	coordV		vertical coordenate of the starting position
	 * @param	coordH		horizontal coordenate of the starting position
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word
	 * @return				0 if the word was successfully written on the board. 1 if some part of the word is out of bounds. 2 if the word
	 *						wrongly overlaps another word. 3 if some part of the word hits a blackbox. 4 if
	 *						starts right next to another word. 5 if the word already appears in the board.
	 *						Only in case 0 the word is written on the board.
	 */
	int insertWord(string coords, string word);

	/**
	 * Removes the given word from the board and from the wordMap if there is a valid word in the given position.
	 *
	 * @param	coords	starting coordenates and orientation(format DdO) of the word to be removed
	 * @return			true if a word is removed, false if the given position does not contain a word
	 */
	bool removeWord(string coords, bool removeBlackBoxes = true);

	/**
	 * Removes all the words in the current board (replaces the letters with '.')
	 */
	void removeAllWords();

	/**
	 * Checks if the board is full. The board is considered full if no cells contain the char '.'.
	 *
	 * @return	true if the board is full, false otherwise
	 */
	bool isFull();

	/**
	 * Fills all the empty positions with blackboxes.
	 */
	void fillEmpty();

	/**
	 * Scans the content of the board in the given position generating a wildcard(empty positions are represented by '?').
	 *
	 * @param	coords	position and orientation that is supposed to be scanned
	 * @return			string consisting of the wildcard
	 */
	string generateWildcard(string coords);

	/**
	 * Returns the word corresponding to a given set of coordenates
	 *
	 * @param	coords	coordenates of the word
	 * @return	word corresponding to the given coordenates
	 */
	string wordMapSearch(string coords);

	/**
	 * Scans the content of the board in the given position returning the word that is in there.
	 *
	 * @param	coords	position and orientation that is supposed to be scanned
	 * @return			string consisting of the wildcard
	 */
	string scanPosition(string coords);


	/**
	 * Checks if a word starts at the given position.
	 *
	 * @param	coords	position and orientation to check
	 * @return			true if a word exists at that position, false otherwise
	*/
	bool posValid(string coords);

private:

	//---------- PRIVATE MEMBER FUNCTIONS ---------- 


	/**
	 * Outputs to the given stream the current word Map(used when saving the current state of the game to a file).
	 *
	 * @param	outputStream	stream where the wordMap will be "shown"
	 */
	void showWordMap(ostream& outputStream);

	/**
	 * Writes the given word in the board, starting at the coordenates(coordV, coordH) (alpha - alpha coordenate system), horizontally
	 * or vertically depending on the value of param "orientation" (V or H), not caring if the word correctly fits the desired position.
	 *
	 * @param	word		the word to be written on the board
	 * @param	coordV		vertical coordenate of the starting position
	 * @param	coordH		horizontal coordenate of the starting position
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word
	 */
	void writeWord(string word, char  coordV, char coordH, char orientation);

	/**
	 * Goes through every word in the wordMapping map and rewrites them in their given positions in the board
	 */
	void reloadWords();

	/**
	 * Writes blackboxes('#') on the cells before and after a word(the word is represented by its starting X and Y position, size and
	 * orientation).
	 *
	 * @param	wordSize	size of the word
	 * @param	coordY		line index
	 * @param	coordX		column index
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word
	 */
	void insertBlackCells(int wordSize, index coordY, index coordX, char orientation);

	/**
	 * Removes the blackboxes('#') on the cells before and after a word(the word is represented by its starting X and Y position, size and
	 * orientation).
	 *
	 * @param	wordSize	size of the word
	 * @param	coordY		line index
	 * @param	coordX		column index
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word
	 */
	void removeBlackCells(int wordSize, index coordY, index coordX, char orientation);

	/**
	 * Searches the wordMapping map for the given word.
	 *
	 * @param	word	word to be searched
	 * @return			true if the word exists in the wordMap, false otherwise
	 */
	bool searchWord(string word);

	/**
	 * Returns the char that occupies the specified position (line, column) on the board.
	 *
	 * @param	line	line index [0, lineNumber-1]
	 * @param	column	column index [0, columnNumber - 1]
	 * @return			char that occupies the given position
	 */
	const char at(index line, index column);

	/**
	 * Changes the char at the given position(line, column) on the board to the given char.
	 *
	 * @param	line	line index, [0, lineNumber-1]
	 * @param	column	column index, [0, columnNumber - 1]
	 */
	void set(index line, index column, char letter);

	/**
	 * Receives a given char and the returns the index corresponding to that letter ('A' or 'a' corresponds to 0 and so on...).
	 *
	 * @param	letter	char to be converted
	 * @return			corresponding index
	 */
	index CoordChange(char letter);

	/**
	 * Checks if the given position (numeric coordenates) is within the bounds of the board.
	 *
	 * @param	line	line index
	 * @param	column	column index
	 * @return			true if the cell is within bounds, false otherwise
	 */
	bool isInBounds(index line, index column);

	/**
	 * Checks to see if the given position in the board is empty. Empty positions on the board contain a '.'.
	 *
	 * @param	line	line index
	 * @param	column	column index
	 * @return			true if the cell is empty, false otherwise
	 */
	bool isEmpty(index line, index column);

	/**
	 * Checks to see if the given position in the board is a blackbox.
	 * Blackboxes are represented by '#'.
	 *
	 * @param	line	line index
	 * @param	column	column index
	 * @return			true if the cell is blackbox, false otherwise
	 */
	bool isBlackBox(index line, index column);

	/**
	 * Changes the given word to all upper-case letters.
	 *
	 * @param	word	word to be changed
	 * @return			uppercase version of the word
	 */
	string allCaps(string word);

	/**
	 * Extracts the coordenates and orientation from a string to individual chars.
	 *
	 * @param	position	word to be changed
	 * @param	coordY		line index, passed by reference
	 * @param	coordX		column index, passed by reference
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word, passed by reference
	 */
	void extractCoordenates(string position, char & coordV, char & coordH, char & orientation);

	/**
	 * Increases the number of columns in the board by one.
	 */
	void addColumn();

	/**
	 * Increases the number of lines in the board by one.
	 */
	void addLine();

	/**
	 * Checks to see if a given word can successfuly be written in the desired position.
	 *
	 * @param	word		word to be written
	 * @param	coordY		line index
	 * @param	coordX		column index
	 * @param	orientation	'H' or 'V' corresponding to horizontal or vertical orientation of the word
	 * @return				0 if the word was successfully written on the board. 1 if some part of the word is out of bounds. 2 if the word
	 *						wrongly overlaps another word. 3 if some part of the word hits a blackbox. 4 if the word starts right next to another word.
	 */
	int isClear(string word, index coordY, index coordX, char orientation);

	



	//---------- PRIVATE CLASS ATTRIBUTES ---------- 

	unsigned int lineNumber;			// number of lines of the board
	unsigned int columnNumber;			// number of columns of the board
	vector<vector<char>> boardVector;	// bidimensional vector of chars representing the contents of the board
	map<string, string> wordMapping;	// map containing the words that are written on the board and the position where they are written
};



#endif 
