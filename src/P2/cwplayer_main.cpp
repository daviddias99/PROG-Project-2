#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <conio.h>
#include "console_color.h"
#include "board.h"
#include "dictionary.h"
#include "player.h"
#include "aux_functions.h"
using namespace std;



int main()
{
	const unsigned int MAX_SIZE = 26;
	const long long MAX_STREAM_SIZE = numeric_limits<streamsize>::max();

	suggestionMap hSuggestions;
	suggestionMap vSuggestions;

	Dictionary dictionary;
	Board board_MAIN;
	Board board_PLAYER;
	Player currentPlayer = Player();

	string boardFileName;
	bool stopCicle = false;

	srand((unsigned int)time(NULL));

	while (!stopCicle)
	{
		cout << "CROSSWORDS PUZZLE PLAYER" << endl;
		cout << "====================================================" << endl << endl;

		cout << "INSTRUCTIONS:" << endl;
		cout << "----------------------------------------------------" << endl << endl;
		cout << "Welcome to the game of CROSSWORDS.  " << endl;
		cout << "The objective of this game is to fill a board by guessing words from its synonyms. " << endl;
		cout << "Try to guess all of the words as fast as you can using as less clues as possible. " << endl;
		cout << endl;
		cout << "- While you're playing crosswords: " << endl;
		cout << endl;
		cout << "1. You'll be given one synonym for each word on the board, accompanied by the position and direction the word is written on the board. " << endl;
		cout << endl;
		cout << "2. Choose the position of the board where you want to modify. " << endl;
		cout << "   The position must be entered in the LcD format:" << endl;
		cout << "  * L - line (uppercase)" << endl;
		cout << "  * c - column (lowercase)" << endl;
		cout << "  * D - direction (H - horizontal, V - vertical) (uppercase)" << endl;
		cout << endl;
		cout << "3. After you choose the position, you can do 3 things: " << endl;
		cout << "  * enter your guess" << endl;
		cout << "  * enter - to remove a guess you think is wrong" << endl;
		cout << "  * enter ? if you need an extra clue" << endl;
		cout << endl;
		cout << "4. Whenever you think you've guessed all the words, enter CTRL+Z." << endl;
		cout << "  * if you have made some mistake, you can keep trying until you get everything right" << endl;
		cout << "  * if you have guessed all the words correctly, your information will be stored" << endl;
		cout << endl;
		cout << "GOOD LUCK!" << endl << endl << endl;


		cout << "OPTIONS:" << endl;
		cout << "1- Start game" << endl;
		cout << "0- Exit" << endl << endl;

		// ask user for his option
		int userChoice = getInputNumber("Option ? ", "Invalid option. ", 0, 1);

		cout << endl;

		if (userChoice == 0)
		{
			cout << "Goodbye." << endl;
			return 0;
		}
		else if (userChoice == 1)
		{
			// get all the existent save files into a map
			map<int, string> saveFiles;
			getUsedSaveFiles(saveFiles);

			if (saveFiles.size() == 0)
			{
				cout << "Sorry. There are no available boards to play." << endl << endl;
				continue;
			}

			cout << "----------------------------------------------------" << endl;
			cout << "CROSSWORDS GAME" << endl;
			cout << "----------------------------------------------------" << endl;

			currentPlayer.startGame();


			// show existent save files
			cout << "Existing save files: " << endl;
			for (map<int, string>::iterator it = saveFiles.begin(); it != saveFiles.end(); it++)
			{
				cout << it->second << endl;
			}


			// get number of board save file
			int boardN;

			cout << endl << "Number of board save file you wish to open ? ";
			while (!(cin >> boardN) || saveFiles.find(boardN) == saveFiles.end())
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

				cout << "Save file doesn't exist. " << endl << endl;
				cout << "Number of board save file ? ";
			}

			// ignore what's left in the buffer because getline() will be used later
			cin.ignore(MAX_STREAM_SIZE, '\n');

			// get the name of the save file from the number of the save file
			stringstream name;
			name << 'b' << setfill('0') << setw(3) << boardN << ".txt";
			boardFileName = name.str();


			ifstream boardFile;		// stream containing the board file
			string dictionaryName;	// name of the dictionary

			boardFile.open(boardFileName);		// opens board file
			getline(boardFile, dictionaryName);	// extracts dictionary name from the first line

			// if the word extraction from the given dictionary is successful, loads the rest of the board to the "board" object
			if (dictionary.extractWords(dictionaryName))
			{
				board_MAIN.loadGameState(boardFile);
			}
			else
			{
				cout << endl << "Unable to find the dictionary associated with this file." << endl << endl;

				continue;
			}

			if (!board_MAIN.isFull())
			{
				cout << "ERROR: The board associated with this file is not valid. Exiting..." << endl;
				return 0;
			}
		}


		// setup the player board. The player board is a copy of the "main board" but without any words
		board_PLAYER = board_MAIN;
		board_PLAYER.removeAllWords();

		// fill the hSuggestions and vSuggestions vectors with strings containing the position and a synonym of a word on the board
		buildGameSuggestions(hSuggestions, vSuggestions, board_MAIN, dictionary); 

		cout << endl;

		// get the player ID
		string playerName;

		playerName = getName("Player nickname ? ", "Invalid nickname.");

		currentPlayer.setPlayerName(playerName);


		while (true)	// breaks when the user enters CTRL+Z
		{
			cout << endl;
			board_PLAYER.showBoard();
			cout << endl;
			showGameSuggestions(hSuggestions, vSuggestions);


			string pos; // position of a board cell in the format LcD(line, column, direction)

			
			// get coordenates from the user: if getCoordenates returns false, means that the user entered CTRL-Z and breaks the program		
			if (!getCoordenates(pos)) 
			{
				bool equalBoards = (board_MAIN == board_PLAYER); 	// evaluate if the boards are equal
				bool tryAgain; 

				if (equalBoards)
				{
					currentPlayer.endGame(); // endgame to stop the timer on the player object

					int secondsToComplete = (int)currentPlayer.getTimeToComplete();	// total seconds to finish the game
					int minutesToComplete = secondsToComplete / 60;					// number of minutes to finish the game
					int secondsLeft = secondsToComplete - 60 * minutesToComplete;	//Number of seconds remaining to finish the game
					tryAgain = false;

					cout << "Congratulations, you've finished the game in " << minutesToComplete << " minutes and " << secondsLeft << " seconds and using " << currentPlayer.getCluesAsked() << " clues ..." << endl;
					

					// build the output file name

					string outputFileName = boardFileName.substr(0, 4);
					outputFileName += "_p";
					outputFileName += ".txt";

					cout << "Saving player stats to " << outputFileName << endl << endl;

					// save the player stats to the output file in the form: "PlayerName|TimetoComplete(seconds)|NumberofCluesAsked
					ofstream outputFile;

					outputFile.open(outputFileName, ios::out | ios::app); // setup the output file. ios::app makes it so that the new information is appended to the file
					outputFile << currentPlayer.getPlayerName() << "|" << currentPlayer.getTimeToComplete() << "|" << currentPlayer.getCluesAsked() << endl;
					outputFile.close();
				}
				else
				{
					// get decision from the user
					cout << endl << "Sorry, the given board is not correct! Keep trying ( Y = yes / N = no ) ? ";
					char userInput;
					cin >> userInput;

					while (!(toupper(userInput) == 'Y' || toupper(userInput) == 'N' || cin.peek() != '\n') || cin.eof())
					{
						if (cin.eof())
							cin.clear();
						else
						{
							cin.clear();
							cin.ignore(MAX_STREAM_SIZE, '\n');
						}

						cout << "Invalid answer. " << endl << endl;
						cout << "Keep trying ( Y = yes / N = no ) ? ";
						cin >> userInput;
					}

					cin.ignore(MAX_STREAM_SIZE, '\n'); // ignore what's left in the buffer

					if (toupper(userInput) == 'Y') // if the user typed yes he may try to continue solving the puzzle
						tryAgain = true; 
					else if (toupper(userInput) == 'N') // if the user typed no the game will end
					{
						tryAgain = false;
						cout << "Better luck next time! " << endl;
					}
				}
				

				stopCicle = true;

				if (!tryAgain)
				{
					break;
				}

				continue;

			} 


			cout << "Word ( - = remove / ? = help )  ? ";
			string wordInput;
			cin >> wordInput;


			if (wordInput == "-")
			{
				if (!board_PLAYER.removeWord(pos,false))
					cout << "That position does not contain a valid word. " << endl;
			}
			else if (wordInput == "?") // this will change the synonym shown in the list at the current position to a new random one. (this may cause the same synonym to be shown)
			{
				
				bool isValidPos = board_MAIN.posValid(pos); // check if a word starts at those coordenates
				bool wasChanged;

				if (isValidPos)
				{
					if (pos.at(2) == 'H')
					{
						string word = board_MAIN.wordMapSearch(pos);
						wasChanged = changeSuggestion(hSuggestions, pos, dictionary, word);
					}
					else
					{
						string word = board_MAIN.wordMapSearch(pos);
						wasChanged = changeSuggestion(vSuggestions, pos, dictionary, word);
					}

					if (wasChanged == 0)
					{
						string word = board_MAIN.wordMapSearch(pos);
						currentPlayer.increaseClueCounter();
					}
					else if (wasChanged == 1)
					{
						cout << "No more synonyms available." << endl;
					}
				}
				else
				{
					cout << "There is no word starting at that position..." << endl;
				}

				
			}
			else if (cin.eof())
			{
				cin.clear();
				cout << "Invalid input." << endl;
			}
			else if (!dictionary.wordExists(wordInput))
			{
				cout << "The word \"" << wordInput << "\" doesn't exist on the dictionary. " << endl;
			}
			else if (wordInput.size() != board_MAIN.wordMapSearch(pos).size())
			{
				cout << "The word \"" << wordInput << "\" does not have the correct size!" << endl;
			}
			else
			{
				switch (board_PLAYER.insertWord(pos, wordInput))
				{
				case 1: cout << "The word doesn't fit the board in the desired position. " << endl; break;
				case 2: cout << "Invalid overlap. " << endl; break;
				case 3: cout << "The word hits a blackbox. " << endl; break;
				case 4: cout << "The word starts right next to another word. " << endl; break;
				case 5: cout << "The word \"" << wordInput << "\" is already on the board. " << endl; break;
				}
			}

		}

	}


	return 0;
}


