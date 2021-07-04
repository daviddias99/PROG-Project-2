#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "dictionary.h"
#include <iostream>
using namespace std;



Dictionary::Dictionary()
{
	this->fileName = "";
}
 

bool Dictionary::extractWords(string fileName)
{
	if (this->fileName != "")
		return false; // dictionary already associated with a dictionary text file
	else
	{
		ifstream wordFile;
		wordFile.open(fileName);

		bool validFile = wordFile.is_open();

		if (validFile)
		{
			string line = "";

			getline(wordFile, line);
			processLine(line);

			while (!wordFile.eof())
			{
				getline(wordFile, line);
				processLine(line);
			}

			this->fileName = fileName;
		}

		wordFile.close();

		return validFile;
	}
}


string Dictionary::getDicName()
{
	return this->fileName;
}


bool Dictionary::wordExists(string word)
{
	return !(wordMap.find(allCaps(word)) == wordMap.end());
}


vector<string> Dictionary::getSynonyms(string word)
{
	return wordMap.at(allCaps(word));
}


string Dictionary::getRndSyn(string word)
{
	vector<string> synonyms = getSynonyms(word);

	return synonyms.at(randomBetween(0, (int)synonyms.size() - 1));
}


vector<string> Dictionary::getFittingWords(string wildCard)
{
	const unsigned int MAX_STR_SIZE = 26;

	vector<string> validWords;


	char cWildPointer[MAX_STR_SIZE + 1];
	char cWordPointer[MAX_STR_SIZE + 1];

	strcpy_s(cWildPointer, wildCard.c_str());

	for (map<string, vector<string>>::const_iterator it = wordMap.begin(); it != wordMap.end(); it++)
	{
		strcpy_s(cWordPointer, it->first.c_str());

		// if the word matches the wildcard, add it to the vector containing the valid words
		if (wildcardMatch(cWordPointer, cWildPointer))
		{
			validWords.push_back(it->first);
		}
	}

	return validWords;
}


void Dictionary::removeSpaces(string &word)
{
	if (word != "")
	{
		while (isspace((unsigned char)word.at(0))) // remove spaces at the beginning of the string
		{
			word.erase(0, 1);
			if (word == "")
				break;
		}
	}

	if (word != "")
	{
		size_t j = word.length() - 1;
		while (isspace((unsigned char)word.at(j))) // remove spaces at the end of the string
		{
			word.erase(j, 1);
			j--;
		}
	}
}


string Dictionary::allCaps(string word)
{
	for (size_t i = 0; i < word.size(); i++)
	{
		word.at(i) = toupper(word.at(i));
	}

	return word;
}


bool Dictionary::isValidWord(string s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (!isalpha(s.at(i)))
			return false;
	}

	return true;
}


bool Dictionary::isValidSyn(string s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (!(isalpha(s.at(i)) || s.at(i) == '-' || s.at(i) == ' '))
			return false;
	}

	return true;
}


void Dictionary::processLine(string line)
{
	if (line != "")
	{
		string currentSyn;
		vector<string> synonyms;

		string word = allCaps(line.substr(0, line.find_first_of(':')));
		line = line.substr(line.find_first_of(':') + 1, line.length() - 1);

		if (isValidWord(word))
		{
			while (line.find(',') != string::npos)
			{
				currentSyn = line.substr(0, line.find_first_of(','));
				line = line.substr(line.find_first_of(',') + 1, line.length() - 1);

				removeSpaces(currentSyn);
				if (currentSyn != "" && isValidSyn(currentSyn))
					synonyms.push_back(allCaps(currentSyn));
			}

			currentSyn = line; // after the last comma, there is still one more synonym
			removeSpaces(currentSyn);
			if (currentSyn != "" && isValidSyn(currentSyn))
				synonyms.push_back(allCaps(currentSyn));

			// inserts a pair to the map(key = word; value = vector containing all synonyms) only if there is at least one synonym
			if (synonyms.size() > 0)
				wordMap.insert(pair<string, vector<string>>(word, synonyms));
		}
	}
}


int Dictionary::randomBetween(int n1, int n2)
{
	if (n1 < n2)
		return n1 + rand() % (n2 - n1 + 1);
	else
		return n2 + rand() % (n1 - n2 + 1);
}


bool Dictionary::vectorFind(vector<string>& vector, string word)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (vector.at(i) == word)
		{
			return true;
		}

	}
	return false;
}


vector<string> Dictionary::invalidWildcards(string wildcard)
{
	vector<string> result;

	for (size_t i = 1; i <= wildcard.length(); i++)
	{
		string currentWildcard = wildcard.substr(0, i);

		if (isalpha(currentWildcard.back()))
		{
			result.push_back(currentWildcard.substr(0, i - 1));
		}
	}

	return result;
}


vector<string> Dictionary::suggestions(string wildcard)
{
	vector<string> result;
	vector<string> invWildcards = invalidWildcards(wildcard);

	for (size_t i = 1; i <= wildcard.length(); i++)
	{
		string currentWildcard = wildcard.substr(0, i);

		if (vectorFind(invWildcards, currentWildcard))
		{
			continue;
		}

		vector<string> currentVector = this->getFittingWords(wildcard.substr(0, i));
		result.insert(result.end(), currentVector.begin(), currentVector.end());
	}

	return result;
}


bool Dictionary::wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

