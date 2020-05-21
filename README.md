# Battleships
 A battleships game manager and player
 
 ## Rules
 - The board file (see `example-board.sboard`) contains ships in different sizes for both players
 - The game manager gives each player a board containing only their ships
 - Each turn a player attacks a position on the board and gets the result of the attack from the manager
 - If a player hits/sinks an enemy ship they get another turn
 - The game ends once one player has destroyed all enemy ships
 
 ## Writing a player
 - Implement the player interface (see `IBattleshipGameAlgo.h`)
 - The manager will dynamically load the first two `*.dll` players it finds
 
The game will be visually displayed in real time unless `-quiet` is passed as an argument:

![](https://raw.githubusercontent.com/ron828/Battleships/master/Screenshots/running-game.gif)
