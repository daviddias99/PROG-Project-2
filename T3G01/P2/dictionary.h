#ifndef dictionary_h
#define dictionary_h

#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


/**
 * The Dictionary class stores the contents of a synonyms dictionary text file, which consists of words and its synonyms.
 * The class is responsible from extracting only the simple words and its synonyms(that could be expressions) from a text file in the specified format.
 * Each object of the Dictionary class can only be associated with one dictionary text file.
 */
class Dictionary
{
	
public:


	//---------- CONSTRUCTOR ---------- 

	/**
	 * Creates an empty dictionary which is not associated to any dictionary text file.
	 */
	Dictionary();

	//---------- PUBLIC MEMBER FUNCTIONS ---------- 

	/**
	 * Extracts words from a synonyms dictionary file, storing them in a map(key = word; value = string of synonyms).
	 *
	 * @param	fileName	name of the dictionary file
	 * @return				true if the extraction was successful, false otherwise
	 */
	bool extractWords(string fileName);

	/**
	 * Gives the name of the file associated to the dictionary.
	 *
	 * @return	name of the dictionary file
	 */
	string getDicName();

	/**
	 * Checks if a given word belongs on the dictionary(case insensitive).
	 *
	 * @param	word	word we want to check if it belongs on the dictionary
	 * @return			true if the word is on the dictionary
	 */
	bool wordExists(string word);

	/**
	 * Returns a vector containing all the synonyms of a given word(case insensitive).
	 *
	 * @param	word	word whose synonyms we want to know
	 * @return			vector containing the synonyms from a given word
	 */
	vector<string> getSynonyms(string word);

	/**
	 * Randomly chooses a word from the vector of synonyms of a given word(case insensitive).
	 *
	 * @param	word	word whose random synonym we want
	 * @return			string corresponding to a random synonym of the given word
	 */
	string getRndSyn(string word);

	/**
	 * Receives a wildcard and returns a vector with all possible matches for each substring starting in the first letter of the wildcard.
	 * 
	 * @param	wildcard	wildcard containing '?'
	 * @return				vector of strings containing all the matches for the given wildcard
	 */
	vector<string> suggestions(string wildcard);
 


private:

	//---------- PRIVATE MEMBER FUNCTIONS ---------- 

	/**
	 * Modifies a word removing all the spaces from the beginning and end.
	 *
	 * @param	word	word to be normalized, passed by reference
	 */
	void removeSpaces(string& word);
 
	/**
	 * Receives a word and returns it in all capital letters
	 *
	 * @param	word	word to make all caps
	 * @return			word in all caps
	 */
	string allCaps(string word);

	/**
	 * Checks if a string is a simple word(only one word with all alphabetic chars).
	 *
	 * @param	s	string to be validated
	 * @return		true if the string is a valid simple word, false otherwise
	 */
	bool isValidWord(string s);

	/**
	 * Checks if a string is a valid synonym(can be an expression, having ' ' or '-').
	 *
	 * @param	s	string to be validated
	 * @return		true if the string is a valid synonym, false otherwise
	 */
	bool isValidSyn(string s);

	/**
	 * Processes a line from the dictionary, adding the words and the synonyms to the map.
	 *
	 * @param	line	line from the dictionary
	 */
	void processLine(string line);

	/**
	 * Gives a random integer between two given numbers(don't forget to seed the RNG in the main function).
	 *
	 * @param	n1	bound
	 * @param	n2	other bound
	 * @return		random between n1 and n2 (n1 and n2 included)
	 */
	int randomBetween(int n1, int n2);

	/**
	 * Gives all the words present in the dictionary that match the given wildcard.
	 *
	 * @param	wilCard	string corresponding to the wildcard(A ? sign matches any character, except an empty string)
	 * @return			vector containing all the matching words
	 */
	vector<string> getFittingWords(string wildCard);

	//////////////////////////////////////////////////////////////////////////
	// WildcardMatch
	// str - Input string to match
	// strWild - Match mask that may contain wildcards like ? and *
	//
	// A ? sign matches any character, except an empty string.
	// A * sign matches any string inclusive an empty string.
	// Characters are compared caseless.
	//
	// ADAPTED FROM:
	// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
	bool wildcardMatch(const char *str, const char *strWild);

	/**
	 * Finds if a certain word exists in the given vector.
	 *
	 * @param	vector	vector of strings
	 * @param	word	word to be seached in the vector
	 * @return			true if the word is present in the vector, false otherwise
	 */
	bool vectorFind(vector<string>& vector, string word);

	/**
	 * Produces, from a given wildcard, a vector that contains the invalid wildcards.
	 *
	 * @param	wildcard	a wildcard
	 * @result				a vector containing the invalid wildcards
	 */
	vector<string> invalidWildcards(string wildcard);

	//---------- PRIVATE ATTRIBUTES ---------- 

	map<string, vector<string>> wordMap;	// map(key = word; value = string of synonyms)
	string fileName;						// name of the dictionary file used to extract the words
};




#endif
