#pragma once
#ifndef YAHTZEE_H
#define YAHTZEE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DICE_HEADER "    1    " "      2    " "      3    " "      4    " "      5    "
#define EDGE		"*********"
#define ONE_RIGHT	"*     O *"
#define ONE_MIDDLE	"*   O   *"
#define ONE_LEFT	"* O     *"
#define TWO			"* O   O *"
#define EMPTY		"*       *"

#define SUM_ONES 1
#define SUM_TWOS 2
#define SUM_THREES 3
#define SUM_FOURS 4
#define SUM_FIVES 5
#define SUM_SIXES 6
#define THREE_OF_A_KIND 7
#define FOUR_OF_A_KIND 8
#define FULL_HOUSE 9
#define SMALL_STRAIGHT 10
#define LARGE_STRAIGHT 11
#define YAHTZEE 12
#define CHANCE 13

#define INFILE "header.txt"
#define OUTFILE "highscore.txt"

#define FIRST_YAHTZEE 0
#define CAN_STILL_YAHTZEE -1
#define UNUSED 0
#define USED 1
#define ALREADY_USED 1

#define FALSE 0
#define TRUE 1

#define CONTINUE 0
#define GAME_OVER 1

#define NOT_HELD 0
#define HELD 1
#define ALREADY_HELD 1

#define NUM_DICE 5
#define NUM_DICE_VALUES 6
#define DICE_HEIGHT 6
#define NUM_SUBTOTAL_CATEGORIES 6
#define NUM_CATEGORIES 13

void run_app(void);
void main_menu(void);
void print_yahtzee_header(void);
void print_game_rules(void);
int run_yahtzee(void);
void roll_dice(int *dice_array, int *hold_dice_array, int num_rolls, int round, int *score_card_array, int *options_used);
void ask_hold(int *hold_dice_array);
void print_scoring_categories(int *options_used, int *score_card_array);
void update_score_card(int *dice_values_array, int *options_used, int *score_card_array, int *dice_array, int option);
void randomize_dice(int *dice_array, int *hold_dice_array);
void print_dice(int *dice_array, int num_rolls);
void count_dice_values(int *dice_array, int *dice_values_array);
void reset_hold_dice_array(int *hold_dice_array, int reset_number);
int game_over(int *options_used);
int check_if_used(int *options_used, int option);
int add_all_dice(int *dice_array);
int calc_subtotal(int *score_card_array);
int calc_score(int *score_card_array);
int check_if_of_a_kind(int *dice_values_array, int num_of_a_kind);
int check_if_full_house(int *dice_values_array);
int check_if_straight(int *dice_values_array, int required_in_a_row);
void printDie(int diceNum, int diceRow);

#endif
