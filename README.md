
1. Introduction
   
This project is a simple CLI implementation of the classic Dots and Boxes game. It is built entirely using the C programming language. The main aim of the game is for players to take turns drawing lines between adjacent dots on a 4x5 grid.  A player claims a box by completing its fourth side. Each box claimed equates to a single point and at the end of the game, i.e. when all the boxes are completed, the player with the most boxes, i.e. points wins the game; then players can either opt to start a new game or stop playing. 
The system is configured to run and automatically launch inside a Linux virtual machine. 

3. Design Strategy
The game uses a multi-file C programming language approach. Providing specialized methods with dedicated files they live in. The three files used in the program are:
•	main.c : This is the entry point of the program and the brain of the game
•	game.c: Handles the flow of the game, logic and the any interactions with the player. This is the file that does the heaviest lifting. 
•	board.c: This manages the state of the board after any move made by a player and/or at any point in time.
•	Header files (game.h, board.h): Houses the function prototypes of their respective .c files.
The program has been divided up in this manner to promote code readability, reusability, and to make debugging easier especially when using GDB to target specific files and functions.
Whilst each section of the program is vital to the overall correctness, by far the most important is the game loop that is housed in the game.c file. It models a workflow that is as follows:
a.	The board is displayed at the start of the game
b.	An input is requested and collected from the player
c.	This move is validated according to the game rules
d.	If invalid, requests an input from the user again
e.	If valid updates the board
f.	Scores are recalculated

4. Data Structures
The board is modeled using the most efficient data structure possible for this scenario, a 2D array. We deemed it to be the most efficient as it allows O(1) access to any edge or box (index property). It checks for completed squares most efficiently and it is possible to have a clear separation between edges and boxes. The 2D arrays used are below:
•	Horizontal lines: char hLines[ROWS][COLS-1];
•	Vertical lines: char vLines[ROWS-1][COLS];
•	Completed boxes: char boxes[ROWS-1][COLS-1];
Explanation
•	hLines stores horizontal edges between dots (nodes)
•	vLines stores vertical edges
•	boxes stores ownership of completed boxes (e.g., 'A' or 'B')

5. Game Logic
The game operates turn-by-turn between two players.
Core mechanics:
•	Players input two sets of coordinates, each representing dots adjacent to each other 
•	The program validates whether the move is legal (dotsAjacent?)
•	If yes and the move completes a box then:
o	The player gets a point and the boxes array is updated.
o	The player gets another turn
•	Otherwise, the other player gets their turn.
The game ends when all twenty boxes have been filled.

6. Debugging Strategy
5.1 Using GDB
We mainly used GDB to trace the program’s execution and identify logical errors.
We set breakpoints for example in StartGame, step through the code using the next command, and we inspect each variable for semantic errors. 
This helped us verify that we were using the correct input handling methods, we were properly updating the board and that player turns were accurately being switched.

5.2 Using Valgrind
We used valgrind to prevent any detect memory leaks. The program was executed using:
valgrind ./game
We initially faced no issues with memory, this result was however not surprising as the program primarily uses static arrays, this makes memory management controlled and predictable

6. System Integration (Virtual Machine Setup)
To make the Dots and Boxes game autostart in Ubuntu, we added a command sequence to the user’s shell startup file. Specifically, we edited the “.profile” file to include commands that navigate to the project directory, compile the game using“make”, and then execute it. We ensured the directory path was correctly quoted to handle spaces and added a safeguard (|| exit) in case the directory did not exist. This setup allows the game to automatically launch when the Ubuntu session starts, providing a seamless experience without requiring manual navigation or execution each time.

7. Testing
The game was tested in the following ways: 
1.	Firstly, we tested random dots, and this was the following results: 
	We tried connecting dots 0 1 (row, column) and 0 2 and there was an error and it showed that as an invalid move, which we had to debug. 
2.	Secondly, we typed in points without separating rows and columns with a space which returned an invalid move, as we had programmed the game to do. 
3.	We carried out an alpha test by playing a complete game and these were the observations
	The player turns were carried out accurately with turns repeating for a certain player after they complete a box and switching when they don’t.
	Invalid moves were picked up and the player that did the invalid move was given another chance.
	Box ownerships are correct and the player who completes a box has their initial printed inside of the box. 
	The game scores are kept accurately and displayed at the end of the game 
	The winner is correctly declared. 
4.	With the results of the alpha test, a beta test seemed appropriate and the participants confirmed the observations made in the alpha test.

8. Version Control
The version control systems used in this program are Git and Github. 
We used Git to locally hold our commits, first doing the initial commit, committing the structure of the game, the board and then finally the logic of the game. This was to make sure that each update had a backup.  
The commits were pushed and pulled to github to remotely save the project and collaborate effectively.

