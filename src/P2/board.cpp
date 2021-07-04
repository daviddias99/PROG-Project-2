#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "board.h"
#include "dictionary.h"
#include "console_color.h"
using namespace std;


Board::Board()
{
	this->columnNumber = 0;
	this->lineNumber = 0;
}


Board::Board(unsigned int vSize, unsigned int hSize)
{
	vector<char> line;
	this->columnNumber = hSize;
	this->lineNumber = vSize;

	for (size_t i = 0; i < hSize; i++)
	{
		line.push_back('.');
	}

	for (size_t i = 0; i < vSize; i++)
	{
		this->boardVector.push_back(line);
	}
}


Board::Board(fstream& inputFile)
{
	this->loadGameState(inputFile);
}


void Board::showBoard(ostream& outputStream, bool showAxis)
{
	const unsigned int LOWERCASE_ASCII_BEG = 97;
	const unsigned int UPPERCASE_ASCII_BEG = 65;

	if (showAxis) //Show the alphabetic axis
	{
		outputStream << setw(2) << "";
		for (unsigned int i = 0; i < this->columnNumber; i++)
		{
			setcolor(4, 0);
			outputStream << setw(2) << (char)(i + LOWERCASE_ASCII_BEG);

		}
		outputStream << endl;
	}

	for (unsigned int i = 0; i < this->lineNumber; i++) //Show the word board
	{
		if (showAxis)
		{
			setcolor(4, 0);
			outputStream << (char)(i + UPPERCASE_ASCII_BEG) << " ";
		}

		for (unsigned int j = 0; j < this->columnNumber; j++)
		{
			if (this->isBlackBox(i, j))
			{
				setcolor(0, 15);
				outputStream << " ";
				setcolor(15, 0);
				outputStream << this->at(i, j);
			}
			else
			{
				setcolor(0, 15);
				outputStream << setw(2) << this->at(i, j);
			}

			if (j == (this->columnNumber) - 1)
			{
				setcolor(0, 15);
				outputStream << " ";
				if (&outputStream == &cout)
				{
					setcolor(0, 0);
					outputStream << "|"; // just a trick so that if the user zooms the command prompt the board doesn't look disfigured
				}
			}
		}

		outputStream << endl;
	}

	setcolor(15, 0);
}


void Board::showWordMap(ostream& outputStream)
{
	for (map<string, string>::const_iterator mi = this->wordMapping.begin(); mi != this->wordMapping.end(); mi++)
	{
		outputStream << mi->first << " " << mi->second << endl;
	}
}


void Board::storeGameState(ostream& outputFile, string dictionaryName)
{
	// write the file name
	outputFile << dictionaryName << endl << endl;

	// write the current board
	this->showBoard(outputFile, false);
	outputFile << endl;

	// write the word map
	this->showWordMap(outputFile);
}


bool Board::loadGameState(istream& inputFile)
{
	string tempString;
	getline(inputFile, tempString); //Throw away an empty line

	index i = 0;

	do //Read the board
	{
		getline(inputFile, tempString);

		if (tempString != "")
		{
			this->addLine();

			index k = 0;

			for (index j = 1; j < tempString.length(); j += 2)
			{
				if (i == 0)
				{
					this->addColumn();
				}
				this->set(i, k, tempString.at(j));
				k++;
			}
			i++;
		}

	} while (tempString != "");

	getline(inputFile, tempString);//Throw away an empty line

	while (tempString != "" && !inputFile.eof()) //Read the word Map
	{
		stringstream tempSS(tempString);
		string code;
		string word;

		tempSS >> code;
		tempSS >> word;

		this->wordMapping.insert(pair<string, string>(code, word));
		getline(inputFile, tempString);
	}

	return true;
}


void Board::addColumn()
{
	for (size_t i = 0; i < this->lineNumber; i++)
	{
		this->boardVector.at(i).push_back('.');
	}

	this->columnNumber++;
}


void Board::addLine()
{
	vector<char> line;

	for (size_t i = 0; i < this->columnNumber; i++)
	{
		line.push_back('.');
	}

	this->boardVector.push_back(line);
	this->lineNumber++;

}


const char Board::at(index line, index column)
{
	return this->boardVector.at(line).at(column);
}


void Board::set(index line, index column, char letter)
{
	this->boardVector.at(line).at(column) = letter;

}


int Board::insertWord(string coords, string word)
{
	char coordV, coordH, orientation;

	extractCoordenates(coords, coordV, coordH, orientation);

	// change coordenates from "alphabetic" to numeric
	index coordY = this->CoordChange(coordV); 
	index coordX = this->CoordChange(coordH);

	string code = "";
	code += coordV;
	code += coordH;
	code += orientation;

	string wordCaps = this->allCaps(word);
	int validationCode = this->isClear(wordCaps, coordY, coordX, orientation);

	// check if the word exists
	if (this->searchWord(word))
	{
		return 4;
	}


	if (validationCode == 0)
	{
		this->wordMapping.insert(pair<string, string>(code, wordCaps));
		if (orientation == 'H')
		{
			for (size_t i = 0; i < wordCaps.size(); i++)
			{
				this->set(coordY, coordX + i, wordCaps.at(i));
			}
		}
		else if (orientation == 'V')
		{
			for (size_t i = 0; i < wordCaps.size(); i++)
			{
				this->set(coordY + i, coordX, wordCaps.at(i));
			}
		}

		this->insertBlackCells(word.size(), coordY, coordX, orientation);

		return 0;
	}
	else
		return validationCode;
}


bool Board::removeWord(string coords, bool removeBlackBoxes)
{
	if (wordMapping.find(coords) == wordMapping.end())
		return false;
	else
	{
		string word = allCaps(wordMapping.find(coords)->second);

		char vCoord, hCoord, orientation;

		extractCoordenates(coords, vCoord, hCoord, orientation);

		index coordY = this->CoordChange(vCoord);
		index coordX = this->CoordChange(hCoord);

		if (removeBlackBoxes)
		{
			this->removeBlackCells(word.size(), coordY, coordX, orientation);
		}
		if (orientation == 'H')
		{
			for (index i = coordX; i < coordX + word.size(); i++)
			{
				this->set(coordY, i, '.');
			}
		}

		if (orientation == 'V')
		{
			for (index i = coordY; i < coordY + word.size(); i++)
			{
				this->set(i, coordX, '.');
			}
		}

		this->wordMapping.erase(coords); // delete the word from the wordMap
		this->reloadWords(); // reload the words in the board in order to rewrite letters erased that might  belong to another word

		return true;
	}
}


void Board::writeWord(string word, char coordV, char coordH, char orientation)
{
	// change coordenates from "alphabetic" to numeric
	index coordY = this->CoordChange(coordV);
	index coordX = this->CoordChange(coordH);

	string wordCaps = this->allCaps(word);

	// this is done because deleting a word might remove a BlackCell that also belongs to another word (Context: removeWord and InsertWord)
	this->insertBlackCells(word.size(), coordY, coordX, orientation); 

	if (orientation == 'H')
	{
		for (size_t i = 0; i < wordCaps.size(); i++)
		{
			this->set(coordY, coordX + i, wordCaps.at(i));
		}
	}
	else if (orientation == 'V')
	{
		for (size_t i = 0; i < wordCaps.size(); i++)
		{
			this->set(coordY + i, coordX, wordCaps.at(i));
		}
	}
}


void Board::insertBlackCells(int wordSize, index coordY, index coordX, char orientation)
{
	if (orientation == 'H')
	{
		if (isInBounds(coordY, coordX - 1))
		{
			this->set(coordY, coordX - 1, '#');
		}

		if (isInBounds(coordY, coordX + wordSize))
		{
			this->set(coordY, coordX + wordSize, '#');
		}

	}

	if (orientation == 'V')
	{
		if (isInBounds(coordY - 1, coordX))
		{
			this->set(coordY - 1, coordX, '#');
		}

		if (isInBounds(coordY + wordSize, coordX))
		{
			this->set(coordY + wordSize, coordX, '#');
		}

	}

}


void Board::removeBlackCells(int wordSize, index coordY, index coordX, char orientation)
{
	if (orientation == 'H')
	{
		if (isInBounds(coordY, coordX - 1))
		{
			this->set(coordY, coordX - 1, '.');
		}

		if (isInBounds(coordY, coordX + wordSize))
		{
			this->set(coordY, coordX + wordSize, '.');
		}
	}

	if (orientation == 'V')
	{
		if (isInBounds(coordY - 1, coordX))
		{
			this->set(coordY - 1, coordX, '.');
		}

		if (isInBounds(coordY + wordSize, coordX))
		{
			this->set(coordY + wordSize, coordX, '.');
		}
	}
}


bool Board::searchWord(string word)
{
	return !(wordMapping.find(word) == wordMapping.end());
}


void Board::reloadWords()
{
	for (map<string, string>::const_iterator mi = this->wordMapping.begin(); mi != this->wordMapping.end(); mi++)
	{
		stringstream coords(mi->first);
		char vCoord = coords.get();
		char hCoord = coords.get();
		char orientation = coords.get();

		writeWord(mi->second, vCoord, hCoord, orientation);
	}
}


index Board::CoordChange(char letter)
{
	const int LOWERCASE_ASCII_BEG = 97;
	const int UPPERCASE_ASCII_BEG = 65;

	if ((int)letter < LOWERCASE_ASCII_BEG)
	{
		return (int)letter - UPPERCASE_ASCII_BEG;
	}
	else
	{
		return (int)letter - LOWERCASE_ASCII_BEG;
	}
}


bool Board::isInBounds(index line, index column)
{
	if ((line >= this->lineNumber) || (column >= this->columnNumber))
		return false;
	else
		return true;
}


bool Board::isEmpty(index line, index column)
{
	if (this->at(line, column) == '.')
	{
		return true;
	}
	else
		return false;
}


bool Board::isBlackBox(index line, index column)
{
	if (!this->isInBounds(line, column))
		return false;

	if (this->at(line, column) == '#')
		return true;
	else
		return false;
}


bool Board::isFull()
{

	for (index i = 0; i < this->lineNumber; i++)
	{
		for (index j = 0; j < this->columnNumber; j++)
		{
			if (this->isEmpty(i, j))
			{
				return false;
			}
		}
	}

	return true;
}


void Board::fillEmpty()
{
	for (index i = 0; i < this->lineNumber; i++)
	{
		for (index j = 0; j < this->columnNumber; j++)
		{
			if (this->isEmpty(i, j))
				this->set(i, j, '#');
		}
	}
}


string Board::generateWildcard(string coords)
{
	string result = "";

	char coordV, coordH, orientation;
	extractCoordenates(coords, coordV, coordH, orientation);

	//change coordenates from "alphabetic" to numeric
	index coordY = this->CoordChange(coordV);
	index coordX = this->CoordChange(coordH);

	if (orientation == 'H')
	{
		index i = 0;

		//The function increases the wildcard until it "hits" a blackbox or the edge of the board
		while (!(this->isBlackBox(coordY, coordX + i)) && this->isInBounds(coordY, coordX + i)) 
		{
			if (isalpha(this->at(coordY, coordX + i)))
				result += this->at(coordY, coordX + i);
			else result += '?';

			i++;
		}
	}

	if (orientation == 'V')
	{
		index i = 0;

		//The function increases the wildcard until it "hits" a blackbox or the edge of the board
		while (!(this->isBlackBox(coordY + i, coordX)) && this->isInBounds(coordY + i, coordX))
		{
			if (isalpha(this->at(coordY + i, coordX)))
				result += this->at(coordY + i, coordX);
			else result += '?';

			i++;
		}
	}

	return result;
}


void Board::removeAllWords()
{
	// the original wordMap size is kept because it tells the function how many words it as to remove
	// as it removes a word the wordMap shrinks in size so it always removes the first word

	int originalSize = this->wordMapping.size();

	for (int i = 1; i <= originalSize; i++)
	{
		this->removeWord(this->wordMapping.begin()->first, false);
	}
}


string Board::wordMapSearch(string coords)
{
	return this->wordMapping.at(coords);
}


string Board::scanPosition(string coords)
{
	string result = "";

	char coordV, coordH, orientation;
	extractCoordenates(coords, coordV, coordH, orientation);

	// the function increases the wildcard until it "hits" a blackbox or the edge of the board
	index coordY = this->CoordChange(coordV);
	index coordX = this->CoordChange(coordH);


	if (orientation == 'H')
	{
		index i = 0;

		while (!(this->isBlackBox(coordY, coordX + i)) && !(this->isEmpty(coordY, coordX + i)) && this->isInBounds(coordY, coordX + i))
		{
			result += this->at(coordY, coordX + i);

			i++;
		}
	}

	if (orientation == 'V')
	{
		index i = 0;

		while (!(this->isBlackBox(coordY + i, coordX)) && !(this->isEmpty(coordY + i, coordX)) && this->isInBounds(coordY + i, coordX))
		{
			result += this->at(coordY + i, coordX);

			i++;
		}
	}

	return result;
}


int Board::isClear(string word, index coordY, index coordX, char orientation)
{
	bool result = 0;

	if (orientation == 'H')
	{
		// check if the cell "behind" the word is in bounds
		if (isInBounds(coordY, coordX - 1))
		{
			// if the letter in the cell "behind" the word is a letter, than the word can't be written there
			if (isalpha(this->at(coordY, coordX - 1)))
			{
				return 4;
			}
		}
		// check if the cell "in front of" the word is in bounds
		if (isInBounds(coordY, coordX + word.size()))
		{
			// if the letter in the cell "in front of" the word is a letter, than the word can't be written there
			if (isalpha(this->at(coordY, coordX + word.size())))
			{
				return 4;
			}
		}


		for (index i = coordX; i <= coordX + word.size() - 1; i++)
		{
			// if any part of the word is out-of-bounds it can't be written
			if (!isInBounds(coordY, i))
			{
				return 1;
			}

			// if any part of the word hits a blackbox it can't be written
			if (isBlackBox(coordY, i))
			{
				return 3;
			}

			if (isEmpty(coordY, i))
				continue;
			else if (word.at(i - coordX) != this->at(coordY, i)) // if the current cell isn't empty, the letter must match the current letter of the word														
				return 2;										 // otherwise the word can't be written
		}
	}

	else if (orientation == 'V')
	{
		// check if the cell "behind" the word is in bounds
		if (isInBounds(coordY - 1, coordX))
		{
			// if the letter in the cell "behind" the word is a letter, than the word can't be written there
			if (isalpha(this->at(coordY - 1, coordX)))
			{
				return 4;
			}
		}

		// check if the cell "in front of" the word is in bounds
		if (isInBounds(coordY + word.size(), coordX))
		{
			// if the letter in the cell "in front of" the word is a letter, than the word can't be written there
			if (isalpha(this->at(coordY + word.size(), coordX)))
			{
				return 4;
			}
		}

		for (index i = coordY; i <= coordY + word.size() - 1; i++)
		{
			// if any part of the word is out-of-bounds it can't be written
			if (!isInBounds(i, coordX))
				return 1;

			// if any part of the word hits a blackbox it can't be written
			if (isBlackBox(i, coordX))
			{
				return 3;
			}

			if (isEmpty(i, coordX))
				continue;
			else if (word.at(i - coordY) != this->at(i, coordX)) // if the current cell isn't empty, the letter must match the current letter of the word														
				return 2;										 // otherwise the word can't be written
		}

	}

	return 0;
}

bool Board::posValid(string coords)
{
	if (wordMapping.find(coords) == wordMapping.end())
	{
		return false;
	}
	else return true;

}


string Board::allCaps(string word)
{
	for (size_t i = 0; i < word.size(); i++)
	{
		word.at(i) = toupper(word.at(i));
	}

	return word;
}


void Board::extractCoordenates(string position, char& coordV, char& coordH, char& orientation)
{
	coordV = position.at(0);
	coordH = position.at(1);
	orientation = position.at(2);
}


bool operator==(const Board & left, const Board & right)
{
	if (equalVectors(left, right) || equalWordMaps(left, right))
		return true;
	else
		return false;
}


bool equalWordMaps(const Board & left, const Board & right)
{
	if (left.wordMapping.size() != right.wordMapping.size()) 
	{
		return false;
	}

	map<string, string>::const_iterator mi2 = right.wordMapping.begin();

	for (map<string, string>::const_iterator mi1 = left.wordMapping.begin(); mi1 != left.wordMapping.end(); mi1++)
	{
		if (mi1->second != mi2->second)
		{
			return false;
		}
		mi2++;
	}

	return true;
}


bool equalVectors(const Board & left,const Board & right)
{
	for (size_t i = 0; i < left.lineNumber; i++)
	{
		for (size_t j = 0; j < left.columnNumber; j++)
		{
			if (left.boardVector.at(i).at(j) != right.boardVector.at(i).at(j))
			{
				return false;
			}
		}
	}

	return true;
}