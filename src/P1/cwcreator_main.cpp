#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "board.h"
#include "dictionary.h"
#include "console_color.h"
#include "aux_functions.h"
using namespace std;




int main()
{
	const unsigned int MAX_SIZE = 26;
	const long long MAX_STREAM_SIZE = numeric_limits<streamsize>::max();

	Dictionary dictionary;
	Board board;
	string boardFileName;
	bool stopCicle = false;		// becomes true when a board is saved(or discarted) after the user enters ctrl+z

	cout << "CROSSWORDS PUZZLE CREATOR" << endl;
	cout << "====================================================" << endl << endl;

	cout << "INSTRUCTIONS:" << endl;
	cout << "----------------------------------------------------" << endl << endl;
	cout << "- First, choose if you want to start a new board(option 1) or resume an existing one(option 2). Enter 0 if you wish to exit." << endl;
	cout << "  * If you're creating a new board you'll need to specify the size of the board and the dictionary you wish to use.\n    (press enter if you wish to use the default dictionary)" << endl;
	cout << "  * It is only possible to resume an existing board if the dictionary being used in its creation is still available." << endl;
	cout << endl;
	cout << "- While you're building the board:" << endl;
	cout << endl;
	cout << "1. Enter the position you wish to modify(position must be in the LcD format)." << endl;
	cout << "  * L - line (uppercase)" << endl;
	cout << "  * c - column (lowercase)" << endl;
	cout << "  * D - direction (H - horizontal, V - vertical) (uppercase)" << endl;
	cout << endl;
	cout << "2. You now have four choices:" << endl;
	cout << "  * enter a word you wish to write" << endl;
	cout << "  * enter - to remove a word that is written starting in the chosen position" << endl;
	cout << "  * enter + to check if a \"randomly\" formed word belongs on the dictionary, and if so, make it part of the board" << endl;
	cout << "  * enter ? if you need suggestions of words" << endl;
	cout << endl;
	cout << "3. Whenever you're ready to finish, enter CTRL+Z while you're being asked for the position to finish and save the board." << endl;
	cout << "  * enter F if the board is completed (filling all the remaining . with #)" << endl;
	cout << "  * enter L if you want to save the board as it is to continue it later" << endl;
	cout << "  * enter D if you want to discard the board" << endl;
	cout << endl << endl;



	while (!stopCicle)
	{
		cout << "OPTIONS:" << endl;
		cout << "1- Create puzzle" << endl;
		cout << "2- Resume puzzle" << endl;
		cout << "0- Exit" << endl << endl;

		// ask user for his option
		int userChoice = getInputNumber("Option ? ", "Invalid option. ", 0, 2);

		cout << endl;

		if (userChoice == 0)
		{
			cout << "Goodbye." << endl;
			return 0;
		}
		else if (userChoice == 1)
		{
			cout << "----------------------------------------------------" << endl;
			cout << "CREATE PUZZLE" << endl;
			cout << "----------------------------------------------------" << endl;

			const string DEFAULT_DICTIONARY = "synonyms.txt";
			string dictionaryFileName;

			// gets dictionary name from user and extracts the words from it
			dictionaryFileName = getName("Dictionary name ? (Press enter to use the default dictionary) ", "Invalid file name.");
			// if user doesn't give a dictionary file name, use the default dictionary
			if (dictionaryFileName == "") dictionaryFileName = DEFAULT_DICTIONARY;
			

			while (!dictionary.extractWords(dictionaryFileName))
			{
				cout << "Invalid dictionary file. " << endl << endl;
				dictionaryFileName = getName("Dictionary name ? (Press enter to use the default dictionary) ", "Invalid file name.");
				if (dictionaryFileName == "") dictionaryFileName = DEFAULT_DICTIONARY;
			}

			int lineNumber, columnNumber;

			cout << endl << "Board size ? " << endl;

			// get number of lines
			lineNumber = getInputNumber("Lines: ", "Invalid number of lines. ", 0, MAX_SIZE);

			// get number of columns
			columnNumber = getInputNumber("Columns: ", "Invalid number of columns. ", 0, MAX_SIZE);


			board = Board(lineNumber, columnNumber);
		}
		else if (userChoice == 2)
		{
			// get all the existent save files into a map
			map<int, string> saveFiles;
			getUsedSaveFiles(saveFiles);

			if (saveFiles.size() == 0)
			{
				cout << "There aren't any board files available to resume. " << endl << endl << endl;
				continue;
			}
			else
			{
				cout << "----------------------------------------------------" << endl;
				cout << "RESUME PUZZLE" << endl;
				cout << "----------------------------------------------------" << endl;


				// show existent save files
				cout << "Existing save files: " << endl;
				for (map<int, string>::iterator it = saveFiles.begin(); it != saveFiles.end(); it++)
					cout << it->second << endl;



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
					board.loadGameState(boardFile);
				}
				else
				{
					cout << endl << "Unable to find the dictionary associated with this file." << endl << endl << endl;

					continue;
				}
			}
		}



		while (true)	// breaks when the user enters CTRL+Z
		{
			cout << endl;
			board.showBoard();
			cout << endl;

			string pos; // position of a board cell in the format LcD(line, column, direction)

			// get coordenates from the user: if getCoordenates returns false, means that the user entered CTRL-Z and breaks the program
			if (!getCoordenates(pos))
			{
				// get decision from the user
				cout << endl << "Is the board finished or do you wish to continue it later ( F = finished / L = later / D = discard ) ? ";
				char userInput;
				cin >> userInput;

				while (!(toupper(userInput) == 'L' || toupper(userInput) == 'F' || toupper(userInput) == 'D') || cin.peek() != '\n')
				{
					if (cin.eof())
						cin.clear();
					else
					{
						cin.clear();
						cin.ignore(MAX_STREAM_SIZE, '\n');
					}

					cout << "Invalid answer. " << endl << endl;
					cout << "Is the board finished or do you wish to continue it later ( F = finished / L = later / D = discard ) ? ";
					cin >> userInput;
				}

				cin.ignore(MAX_STREAM_SIZE, '\n'); // ignore what's left in the buffer


				if (!(toupper(userInput) == 'D'))
				{
					if (toupper(userInput) == 'F')
						board.fillEmpty(); // if the board is finished, fill the remaining '.' with '#'


					bool alreadyExists;
					ofstream saveFile;
					string fileName; // name of the file where the board will be saved

					// if the user choice is 2, it means that the user is resuming a board, so it is stored in the same file
					// if not, then the file is saved sequentially in the next free save file
					if (userChoice != 2)
					{
						ifstream testFile;	// used to open files and see if they already exist

						// get next "free" save file(saving is made in a sequential fashion)
						for (int i = 1; i < 1000; i++)
						{
							stringstream name;
							name << 'b' << setfill('0') << setw(3) << i << ".txt";
							fileName = name.str();

							testFile.open(fileName);
							alreadyExists = testFile.is_open(); // if the file opens in reading mode it means that the file already exists

							// if the file doesn't exist, then it means that it is the one where the board will be saved
							if (!alreadyExists)
								break;
							else
							{
								testFile.close();
								if (i == 999)
									cout << "All valid save files are occupied. Save file 'b999.txt' overwritten. ";
							}
						}

						boardFileName = fileName;
					}

					// opens the file and stores the board in it
					saveFile.open(boardFileName);
					board.storeGameState(saveFile, dictionary.getDicName());
					saveFile.close();

					cout << "Board file saved as '" << boardFileName << "'. " << endl;

				}

				// after the file is saved the program stops
				stopCicle = true;
				break;
			}

			cout << "Word ( - = remove / ? = help / + = check if the word is valid )  ? ";
			string wordInput;
			cin >> wordInput;


			if (wordInput == "-")
			{
				if (!board.removeWord(pos))
					cout << "That position does not contain a valid word. " << endl;
			}
			else if (wordInput == "?")
			{
				vector<string> suggestions;
				int numberOfSuggestions;

				suggestions = dictionary.suggestions(board.generateWildcard(pos));


				if (suggestions.size() > 0)
				{
					string msg = "How many suggestions would you like to see(type -1 to see every suggestion available) ? ";
					string msg_err = "Not a valid number of suggestions. ";

					cout << endl << "There are " << suggestions.size() << " valid words that fit the given position." << endl;

					numberOfSuggestions = getInputNumber(msg, msg_err, -1, suggestions.size());

					if (numberOfSuggestions != 0)
					{
						cout << endl << " Suggestions: " << endl;
						showSuggestions(suggestions, numberOfSuggestions);
					}
				}
				else
					cout << endl << "There are no valid words in the dictionary for the given position." << endl;

			}
			else if (wordInput == "+")
			{
				string word;

				word = board.scanPosition(pos);

				if (dictionary.wordExists(word))
				{
					cout << "The word \"" << word << "\" is a valid word from the dictionary." << endl;
					board.insertWord(pos, word);
				}
				else
				{
					cout << "The word in the given position is not a valid word from the dictionary." << endl;
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
			else
			{
				switch (board.insertWord(pos, wordInput))
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



