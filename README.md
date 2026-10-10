# Number Guessing Game (C)

A simple terminal number guessing game written in C. The computer picks a secret number between 1 and 100 and you get 10 attempts to find it.

## Features

- Random secret number from 1 to 100 on every round
- "Too high" / "Too low" hints after each guess
- 10 attempts per round with an attempt counter
- Safe input handling: letters, decimals and out-of-range numbers are rejected without using up an attempt
- Best score tracking across rounds (for the current session)
- "Play again?" option after each round

## Project Structure

```
number-guessing-game-c/
├── main.c       # the whole game
└── README.md
```

## Requirements

A C compiler such as `gcc`.

- **Linux:** `sudo apt install gcc` (usually already installed)
- **macOS:** run `xcode-select --install`
- **Windows:** install [MinGW-w64](https://www.mingw-w64.org/) or use WSL

## Compile and Run

```bash
gcc main.c -o game
./game
```

On Windows (Command Prompt or PowerShell):

```bash
gcc main.c -o game.exe
game.exe
```

To see extra compiler warnings while learning:

```bash
gcc -Wall -Wextra -std=c99 main.c -o game
```

## Example

```
=== Number Guessing Game ===

I'm thinking of a number between 1 and 100.
You have 10 attempts. Good luck!

Attempt 1/10 - your guess: 50
50 is too high. Try lower!
Attempt 2/10 - your guess: 25
25 is too low. Try higher!
Attempt 3/10 - your guess: 37
Correct! You got it in 3 attempts.
New best score!
```

## How It Works

`rand()` (seeded with the current time using `srand`) picks the secret number. Input is read with `fgets` and converted with `strtol`, which is safer than `scanf` because bad input cannot get the program stuck. Each valid guess is compared with the secret number and the game prints a hint until you win or run out of attempts.

## Ideas to Extend

- Add difficulty levels (different ranges and attempt limits)
- Save the best score in a file so it survives after closing the program
- Add a "hot / cold" hint based on how close the guess is
- Split the code into separate `.c` and `.h` files

## License

MIT
