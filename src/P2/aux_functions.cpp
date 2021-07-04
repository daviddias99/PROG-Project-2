#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <conio.h>
#include "board.h"
#include "dictionary.h"
#include "aux_functions.h"
using namespace std;



string getName(string msg, string msgErr)
{
	string name;
	const long long MAX_STREAM_SIZE = numeric_limits<streamsize>::max();

	cout << msg;
	getline(cin, name);

	while (cin.eof())
	{
		if (cin.eof())
			cin.clear();
		else
			cin.ignore(MAX_STREAM_SIZE, '\n');

		cout << msgErr << endl << endl;
		cout << msg;
		getline(cin, name);
	}

	return name;
}


void showVector(vector<string> vec)
{
	if (vec.size() > 0)
	{
		cout << vec.at(0);

		for (size_t i = 1; i < vec.size(); i++)
		{
			cout << "; " << vec.at(i);
		}
	}
}


bool validCoordenates(string& coords)
{
	if (cin.eof())
		return false;
	else
	{
		if (coords.length() == 3)
			return ((coords.at(0) == toupper(coords.at(0))) && isalpha(coords.at(0)) && isalpha(coords.at(1))
				&& (coords.at(1) != toupper(coords.at(1))) && ((coords.at(2) == 'V') || (coords.at(2) == 'H'))
				&& (coords.length() == 3));
		else
			return false;
	}
}


bool getCoordenates(string& coordsInput)
{
	const long long MAX_STREAM_SIZE = numeric_limits<streamsize>::max();
	cout << "Position ( LCD / CTRL-Z = stop )  ? ";
	cin >> coordsInput;
	cin.ignore(MAX_STREAM_SIZE, '\n');

	while (!(validCoordenates(coordsInput))) // repeat while the string is not in the valid format
	{
		if (cin.eof()) // if CTRL-Z is entered, return false to finish the game
		{
			cin.clear();
			return false;
		}
		cout << "Invalid position. " << endl;
		cout << "Position ( LCD / CTRL-Z = stop )  ? ";
		cin >> coordsInput;
		cin.ignore(MAX_STREAM_SIZE, '\n');
	}


	return true;
}


int randomBetween(int n1, int n2)
{
	if (n1 < n2)
		return n1 + rand() % (n2 - n1 + 1);
	else
		return n2 + rand() % (n1 - n2 + 1);
}


void showSuggestions(vector<string>& vec, int numberOfSuggestions)
{
	if ((size_t)numberOfSuggestions > vec.size())
	{
		numberOfSuggestions = -1;
	}

	vector<string> vecCopy = vec;
	string tempSuggestion;

	if (numberOfSuggestions == -1)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			cout << ". " << vec.at(i) << endl;
		}
	}
	else
	{
		for (size_t i = 0; i < (size_t)numberOfSuggestions; i++)
		{
			size_t randomIndex = randomBetween(0, (int) vecCopy.size() - 1);
			
			vector<string>::iterator vI = vecCopy.begin() + randomIndex;
			tempSuggestion = vecCopy.at(randomIndex);
			vecCopy.erase(vI);
		
			cout << ". " << tempSuggestion << endl;
		}
	}

}


bool vectorSearch(vector<string>& vector, string word)
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


int getInputNumber(string question, string errorMsg, int limInf, int limSup)
{
	int number;
	const long long MAX_STREAM_SIZE = numeric_limits<streamsize>::max();

	cout << question;

	while (!(cin >> number) || number < limInf || number > limSup)
	{
		if (cin.eof())
		{
			cin.clear();
		}
		else
		{
			cin.clear();
			cin.ignore(MAX_STREAM_SIZE, '\n');
		}

		cout << errorMsg << endl << endl;
		cout << question;
	}

	cin.ignore(MAX_STREAM_SIZE, '\n'); // ignore what's left in the buffer so it doesn't conflict with getline() function

	return number;
}


void getUsedSaveFiles(map<int, string>& saveFiles)
{
	ifstream testFile;
	bool alreadyExists;

	for (int i = 0; i < 1000; i++)
	{
		string fileName;
		stringstream name;
		name << 'b' << setfill('0') << setw(3) << i << ".txt";
		fileName = name.str();

		testFile.open(fileName);
		alreadyExists = testFile.is_open(); // if the file opens in reading mode it means that the file already exists

		if (alreadyExists)
		{
			saveFiles.insert(pair<int, string>(i, fileName));
			testFile.close();
		}
	}
}


void buildGameSuggestions(suggestionMap& hSuggestions, suggestionMap& vSuggestions, Board& board, Dictionary& dictionary)
{
	for (map<string, string>::iterator it = board.wordMapping.begin(); it != board.wordMapping.end(); it++)
	{
		if (it->first.at(2) == 'H')
		{

			string word = it->second;
			size_t suggestionsShown = 1; //the number of suggestions shown at the start is 1
			string simpCoord = it->first.substr(0, 2); //the coordinates are only the vertical and horizontal letters
			vector<string> synVec = dictionary.getSynonyms(word); //vector that contains the synonyms of the word
			pair<size_t, vector<string>> synPair(suggestionsShown, synVec);
			pair<string, pair<size_t, vector<string>>> tempPair(simpCoord,synPair);

			hSuggestions.insert(tempPair);
		}
		else
		{
			string word = it->second;
			size_t suggestionsShown = 1;  //the number of suggestions shown at the start is 1
			string simpCoord = it->first.substr(0, 2); //the coordinates are only the vertical and horizontal letters
			vector<string> synVec = dictionary.getSynonyms(word); //vector that contains the synonyms of the word
			pair<size_t, vector<string>> synPair(suggestionsShown, synVec);
			pair<string, pair<size_t, vector<string>>> tempPair(simpCoord, synPair);

			vSuggestions.insert(tempPair);
		}
	}


}


void showGameSuggestions(suggestionMap& hSuggestions, suggestionMap& vSuggestions)
{
	cout << "Horizontal: " << endl << endl;

	for (suggestionMap::iterator it = hSuggestions.begin(); it != hSuggestions.end(); it++)
	{
		cout << it->first << ": "; // (coordenates) :

		//show words followed by a ',', unless it is the last synonynm shown
		for (size_t i = 0; i < it->second.first; i++)
		{
			if (i != it->second.first - 1)
			{
				cout << it->second.second.at(i) << ", ";
			}
			else
			{
				cout << it->second.second.at(i) << endl;
			}
			
		}

	}
	cout << endl;

	

	cout << "Vertical: " << endl << endl;

	for (suggestionMap::iterator it = vSuggestions.begin(); it != vSuggestions.end(); it++)
	{
		cout << it->first << ": ";// (coordenates) :

		//show words followed by a ',', unless it is the last synonynm shown
		for (size_t i = 0; i < it->second.first; i++)
		{
			if (i != it->second.first - 1)
			{
				cout << it->second.second.at(i) << ", ";
			}
			else
			{
				cout << it->second.second.at(i) << endl;
			}

		}

	}

	cout << endl;
}


int changeSuggestion(suggestionMap& mapSuggestions, string coords, Dictionary& dictionary, string word)
{
	string xy = coords.substr(0, 2);
	
	for (suggestionMap::iterator it = mapSuggestions.begin(); it != mapSuggestions.end(); it++)
	{
		if (it->first == xy)
		{
			if (it->second.first != it->second.second.size())
			{
				it->second.first++; //increase the shown suggestion counter
				return 0;
			}
			else
				return 1;  //if all the synonyms of the word are being shown return 1
		}

	}

	return 2; //if there is no word at the chosen coordinates, return 2

}
