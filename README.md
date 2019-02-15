# PacmanAI
The project contains 3 monsters: Red, Turquoise and Green, one yellow Pacman and about 300 coins distributed over the game board. 
<br/>
<br/>
<br/>
## Game Board
The game board is 600x600 px, each wall's thickness is 5 px, except the letters 'H' and 'S' that are thicker. Every group of pawns (Pacman and monsters) has its own maze matrix to track visited cells. 
<br/>
<br/>
<br/>
## Coins
The coins distribution made by a BFS algorithm, which checks for every coin that Pacman can eat it, by checking the closeness of each coin to the wall (coin wouldn't be placed in a distance less than 10 pixels from a wall). There are 301 coins on the board. 
<br/>
<br/>
<br/>
## Monsters
Three monsters start their way in the center 'pool', as in the original game. All the monsters search their path within A* algorithm, if one of them found its target, then it's ready to move, anyway every monster will start moving after 300 iterations of its A* algorithm. After achieving its target point the monster will search again for a path to the updated target's location (whether it's Pacman or a coin). 
The turquoise monster (in the middle) starts its movement first of the three. After it's ready to move (by the terms explained above), it will perform move every 2 CPU clocks (counter in the idle method). The green monster will move every 4 CPU clocks, and the red will move every 6 CPU clocks. Red and turquoise monsters' target is the Pacman itself, and the green's target is the coin that Pacman is moving towards. 
<br/>
<br/>
<br/>
## Pacman
It starts with searching the nearest coin to its location when considering the distance of the coin from the nearest monster to Pacman (the distance between the nearest monster and the coin should be over 200), that number isn't really calculated as the definition to compromise the nearest coin to Pacman and the farther distance from monster isn't defined well, and can cause more problems in the Pacman’s A* search. 
After the nearest coin is set, Pacman will start A* search for a path to that coin (the A* algorithm of Pacman is different from monster's). Pacman is allowed to move only straight (up, down, left, right) it's not allowed to move diagonal, while monsters can, so the A* algorithm is processing the possible path accordingly (not as we did in class). If it found a path to the coin, then it's ready to move, and it will move every 2 CPU clocks (as the turquoise monster). If it didn’t find a path to coin, it will be ready to move after 600 A* iterations and the will search for a new coin to reach. 
<br/>
<br/>
<br/>
## Board View

<img src="https://github.com/HadarPur/PacmanAI/blob/master/PacmanBoard.png" width="500" height="500" />
