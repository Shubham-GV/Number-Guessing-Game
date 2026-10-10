/*
 * Number Guessing Game in C
 *
 * The computer picks a secret number between 1 and 100.
 * You get 10 attempts to guess it, with "too high" / "too low" hints.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define MIN_NUMBER   1
#define MAX_NUMBER   100
#define MAX_ATTEMPTS 10

/* Return codes for read_int() */
#define READ_OK       1
#define READ_INVALID  0
#define READ_EOF     -1

/*
 * Reads one line from the keyboard and converts it to an int.
 * Using fgets + strtol is safer than scanf, because typing letters
 * will not leave the program stuck in an infinite loop.
 */
static int read_int(const char *prompt, int *value)
{
    char buffer[64];
    char *end;
    long number;

    printf("%s", prompt);

    if (fgets(buffer, sizeof buffer, stdin) == NULL) {
        return READ_EOF;
    }

    /* Line was longer than the buffer: throw away the rest of it */
    if (strchr(buffer, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard */
        }
        return READ_INVALID;
    }

    errno = 0;
    number = strtol(buffer, &end, 10);

    if (end == buffer) {
        return READ_INVALID;              /* no digits at all */
    }
    while (*end == ' ' || *end == '\t') {
        end++;                            /* allow trailing spaces */
    }
    if (*end != '\n' && *end != '\0') {
        return READ_INVALID;              /* stuff like "12abc" or "3.5" */
    }
    if (errno == ERANGE || number < INT_MIN || number > INT_MAX) {
        return READ_INVALID;
    }

    *value = (int)number;
    return READ_OK;
}

/* Returns a random number from MIN_NUMBER to MAX_NUMBER (inclusive) */
static int random_number(void)
{
    return MIN_NUMBER + rand() % (MAX_NUMBER - MIN_NUMBER + 1);
}

/*
 * Plays one full round.
 * Returns 1 if the round finished normally, 0 if input ended (Ctrl+D / Ctrl+Z).
 * best_score is updated when the player wins with fewer attempts than before.
 */
static int play_round(int *best_score)
{
    int secret = random_number();
    int attempts = 0;
    int guess;

    printf("\nI'm thinking of a number between %d and %d.\n", MIN_NUMBER, MAX_NUMBER);
    printf("You have %d attempts. Good luck!\n\n", MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS) {
        char prompt[64];
        int status;

        snprintf(prompt, sizeof prompt, "Attempt %d/%d - your guess: ",
                 attempts + 1, MAX_ATTEMPTS);
        status = read_int(prompt, &guess);

        if (status == READ_EOF) {
            printf("\n");
            return 0;
        }
        if (status == READ_INVALID || guess < MIN_NUMBER || guess > MAX_NUMBER) {
            printf("Please enter a whole number between %d and %d.\n",
                   MIN_NUMBER, MAX_NUMBER);
            continue;                     /* invalid input does not use an attempt */
        }

        attempts++;

        if (guess == secret) {
            printf("Correct! You got it in %d %s.\n",
                   attempts, attempts == 1 ? "attempt" : "attempts");
            if (*best_score == 0 || attempts < *best_score) {
                *best_score = attempts;
                printf("New best score!\n");
            }
            return 1;
        }

        if (guess < secret) {
            printf("%d is too low. Try higher!\n", guess);
        } else {
            printf("%d is too high. Try lower!\n", guess);
        }
    }

    printf("\nGame over! The number was %d.\n", secret);
    return 1;
}

/* Asks "Play again?" and returns 1 for yes, 0 for no / end of input */
static int ask_play_again(void)
{
    char buffer[16];

    while (1) {
        printf("\nPlay again? (y/n): ");

        if (fgets(buffer, sizeof buffer, stdin) == NULL) {
            printf("\n");
            return 0;
        }
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* discard */
            }
        }

        if (tolower((unsigned char)buffer[0]) == 'y') return 1;
        if (tolower((unsigned char)buffer[0]) == 'n') return 0;

        printf("Please type y or n.\n");
    }
}

int main(void)
{
    int best_score = 0;                   /* 0 means "no score yet" */

    srand((unsigned int)time(NULL));

    printf("=== Number Guessing Game ===\n");

    do {
        if (!play_round(&best_score)) {
            break;
        }
        if (best_score > 0) {
            printf("Best score this session: %d\n", best_score);
        }
    } while (ask_play_again());

    printf("\nThanks for playing!\n");
    return 0;
}
