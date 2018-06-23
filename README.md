# --- MIEIC-PROG: Project 2 2017/2018 ---

WARNING: FOR CONSULTING THE FULL DEVELOPMENT OF THE PROGRAM SUCH AS COMMITS VISIT  "luispcunha"'s GITHUB PAGE.

AUTHORS

David Luis Dias da Silva, up201705473
Luis Pedro Pereira Lopes Mascarenhas Cunha, up201706736



INTRODUCTION

.These programs were assigned to us as the second project for the class "Programming" in order to apply the knowledge we gained through the semester about the concepts of structured programming, top-down and bottom-up development, object-oriented programming and the programming language C++(library functions, syntax, data-structures,...).
The project is divided in two segments.

PROGRAM 1 - CROSSWORDS CREATOR

.The first program consists of a program that allows the user to create a crosswords puzzle, using the words present in a dictionary file.
.The user is allowed to choose the size of the board(max size is 26x26).
.An empty board is presented to the user so that he can write and remove words, as well as ask for suggestions, in order to be able to create a puzzle board.
.If the user thinks that letters from various words that are written next to each other form a valid word from the dictionary, he can ask to validate it 
by entering '+', and if the word belongs on the dictionary, it will be added as a word written in the dictionary.
.Repeated words are not allowed on the board.
.The user can save the board as a finished puzzle or in the current state to be continued later.
.Board save files are stored in a sequential fashion.
.When resuming a board, the user will be presented by a list of the existent board save files, so that it makes it easier to choose the one they wish to resume.

.For the first program we were asked to develop two classes(dictionary and board). 

 *Dictionary:
  - The dictionary class has method functions that enable it to extract words from a synonyms dictionary text file storing each word and its synonyms in     what we thought was an appropriate data structure(map<string, vector<string>>).
  - It also has public methods to allow access to the words and synonyms in the map.
 
 *Board:
  - The board class stores the board(represented by a bidimensional vector of chars) and the words that are written in it and their position, as well as the
  dictionary file that was used to validate the words.
  - It also contains all the methods we thought necessary to manipulate the board.



PROGRAM 2 - CROSSWORDS PLAYER

.The second program is meant to be a "follow-up" to the first program. In it a player can load a board created by
the cwcreator program and be given an empty board. Then he must try to guess the hidden words with help from synonyms
shown on the screen for each word. 
.During the game, the player can ask for more synonyms to be shown for each word. The number of times he asks for these
clues is tracked and shown to him when he finishes the game.
.When all the words have been filled, the user may press CTRL+Z to check if the board is correctly solved.
.If the board is incorrect, the player is asked if he wants to keep trying to solve the board. Otherwise it is displayed
the number of minutes and seconds the player took to solve the board and the number of "hints" he asked for.
.The information about the player's name, the time in seconds he took to solve the puzzle and the number of clues he asked
for is stored in a text file with the same name of the board and with the appendix "_p".

.For the seconds program, in addition to adding more methods to the existing Dictionary and Board class, we also created
a new class named "Player".

 *Player:
 - The Player class contains information about the player of the crosswords game. It stores the name of the player,
  the number of clues he asked during the solving of the puzzle and information about the time he took to solve it.


COMMENTS ABOUT THE PROJECT

.We feel that the we have produced solid and robust programs that accomplishes the desired functions and that does
them, for the most part, efficiently. Nonetheless we think that there is room for improvement in some areas, namely
in the clarity of the comments for the functions, on the structure of the program itself(class organization,
method naming,etc. ) and on adding of some ways to improve the intuitive nature of the program for the end-user.
