/*			
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

/*
    netID: wwu70
    This program contains three functions: set_seed, start_game, and make_guess
    set_seed: 
        initializes a pseudo-random number generator from a seed the user enters 
        with error checking for mis-input
    start_game:
        uses the pseudo-random number generator from set_seed to generate a solution code 
        (a sequence of 4 "randomly chosen" numbers from 1-8) which is kept hidden form the player
    make_guess: 
        gives feedback on user guesses notifying the user how many of their guessed numbers were
        in the same place as the solution code (perfect matches) and
        in a different place than the solution code (misplaced matches)
        with error checking for mis-input
*/
static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int set_seed (const char seed_str[]) {
    int seed;
    char post[2];
    int num_read = sscanf (seed_str, "%d%1s", &seed, post); // retrieve number of items read from sscanf, assign value to num_read
    if (num_read == 0 || num_read == 2) { // if number of items read != 1, print error message
        printf("set_seed: invalid seed\n");
        return 0;
    }
    else { // otherwise, set the seed using the value retrieved by sscanf
        srand(seed);
        return 1;
    }
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void start_game (int* one, int* two, int* three, int* four) {
    /* generate four random numbers between 1-8 to be used as the solution code using rand */
    *one = rand() % 8 + 1;
    *two = rand() % 8 + 1;
    *three = rand() % 8 + 1;
    *four = rand() % 8 + 1;
    /* initialize four solution numbers using static variables at beginning of program */
    solution1 = *one;
    solution2 = *two;
    solution3 = *three;
    solution4 = *four;
    guess_number = 1;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int make_guess (const char guess_str[], int* one, int* two, int* three, int* four) {
    int w, x, y, z;
    int perfect_matches = 0; 
    int misplaced_matches = 0; 
    char post[2];
    int soln[4] = {solution1, solution2, solution3, solution4}; // array for 4 solution values
    int soln_paired[4] = {0, 0, 0, 0}; // array indicating which values of the solution have been paired
    int guess_paired[4] = {0, 0, 0, 0}; // array indicating which values of the guess have been paired
    int num_read = sscanf(guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post);
    if (num_read == 4) { // check if the number of values read was 4
        if ((w >= 1 && w <= 8) && (x >= 1 && x <= 8) && (y >= 1 && y <= 8) && (z >= 1 && z <= 8)) { // if all numbers are between 1-8
            /* dereference pointers to point at values retrieved by sscanf */
            *one = w;
            *two = x;
            *three = y;
            *four = z;
            int guess_idx;
            int guess[4] = {*one, *two, *three, *four};
            for(guess_idx = 0; guess_idx < 4; guess_idx++) { // check perfect matches first
                /* if the numbers in the guess and solution at the current index match */
                if (guess[guess_idx] == soln[guess_idx]) { 
                    guess_paired[guess_idx]++;
                    soln_paired[guess_idx]++; // update both status arrays at the same index
                    perfect_matches++; // increment number of perfect matches
                }
            }
            int sol_idx; 
            for (guess_idx = 0; guess_idx < 4; guess_idx++) { // then check the misplaced matches
                if (guess_paired[guess_idx] == 0) { // if current guess number hasn't been paired
                    /* check entire solution array for unpaired values */
                    for(sol_idx = 0; sol_idx < 4; sol_idx++) { 
                        /* if values match, but current value of solution hasn't been paired yet */
                        if (guess[guess_idx] == soln[sol_idx] && soln_paired[sol_idx] == 0) { 
                            misplaced_matches++; // increment misplaced matches
                            soln_paired[sol_idx]++; // update solution status array
                            break; // check next value in guess array
                        }
                    }
                }
            } 
            printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, perfect_matches, misplaced_matches); // print perfect and misplaced matches
            guess_number++; // increment the guess number
            return 1;
        }
        else { // if some numbers aren't between 1-8, print error message
            printf("make_guess: invalid guess\n");
            return 0;
        }
    }
    else { // if any ASCII characters are entered, print error message
        printf("make_guess: invalid guess\n");
        return 0;
    }
}


