# Text-based dungeon escape - by Kyle Parker

## Description
A simple text-based dungeon escape. Move between rooms, collect 2 keys,
avoid trap rooms, then use keys in the Treasure Room to exit and win.
Entering trap rooms causes an immediate loss.

## How to compile & run:
g++ -std=c++17 *.cpp -o game && ./game

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

## Classes created
- Player
- Room

## How this project uses OOP
- All logic is in the respective bundle.
- `Player` is an actor which has attirbutes that can be get/set. There is no logic within; simple assignment and returns.
- `Room` is a record so all info is read-only after init.
- `Game` contains all the business logic and handles user interaction.
- `Phrases` acts as a mock strings file. It allows for simple phrase modifications and support for multi-language. Not everything is supported, but many phrases are. For future use, simply extend Phrases and use the newly derived class as the init. `Phrases *p; p = new MyLang(); p->getPhrase("kMY_KEY");`

### Pillars of OOP
- Encapsulation - data and methods are binded to a single name and closely-related.
- Abstraction - Ease of use through functions so the handling of data is hidden behind the scenes.
- Inheritance - EnglishPhrases specializes Phrases and makes it useful. Before it would always return the id as there is no data stored in the default Phrases class.
- Polyphorhism - Not used; this program is too simple and would require jumping through hoops.

If you have more questions, please reach out. I can explain anything if needed.

