# Text Based Game - by Kyle Parker

 [!WARNING]
 The test.sh file required modification. `g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o game > compile_output.txt 2>&1`
 is insufficent and not respect the seperation of C++ files. It was corrected to:
 `g++ -std=c++17 -Wall -Wextra -pedantic *.cpp -o game > compile_output.txt 2>&1`

## Description
A simple text-based dungeon escape. Move between rooms, collect 2 keys (from the Hallway and a chest in the Armory),
avoid the Trap Room, then use keys in the Treasure Room to exit and win. Entering the Trap Room causes an immediate loss.

## How to compile & run:
g++ *.cpp -o game && ./game

## Valid commands
- go <direction>
- look
- take key
- open chest
- use key
- exit
- inventory (or inv)
- help
- quit (or q)
