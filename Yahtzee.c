#include "Yahtzee.h"

void run_app(void)
{
	main_menu();
}

void main_menu(void)
{
	int option = 0, *scores, maxScore = 0, maxI = 0;
	unsigned int players = 1;
	
	char line[50];

	do {
		print_yahtzee_header();
		printf(" Welcome to Yahtzee semi-Interactive video game, for only $199.99 MSRP.\n If you paid less for this game, please send the difference in cash to Stephenson South Rm 1225.\n\n");
		printf(" 1. View rules\n");
		printf(" 2. Play Yahtzee\n");
		printf(" 3. Exit (may crash computer and light power strip on fire)\n");
		printf(" Enter choice: ");
		
		fgets(line, 50, stdin);
		if (NULL != strtok(line, "\n"))
			option = atoi(line);

		system("cls");
	} while (option < 1 || option > 3);

	switch (option)
	{
	case 1:
		print_game_rules();
		system("pause");
		system("cls");
		main_menu();
		break;
	case 2:
		//Gets # players
		do {
			puts("How many people want to play?");
			fgets(line, 50, stdin);
			if (NULL != strtok(line, "\n"))
				players = abs(atoi(line));
			system("cls");
		} while (players < 1);

		printf("Playing with %d number of players\n", players);
		system("pause");
		system("cls");

		//Allocates memory for required number of players
		scores = (int *)malloc(sizeof(int) * players);

		//Gets players' scores by letting them play the game
		for (int i = 0; i < players; i++)
		{
			printf("It's Player %d's turn.\n", i + 1);
			system("pause");
			system("cls");
			scores[i] = run_yahtzee();
		}

		for (int i = 0; i < players; i++)
		{
			printf("Player %d's score: %d", i + 1, scores[i]);
		}
		puts("");
		
		maxScore = scores[0];
		maxI = 0;

		for (int i; i < players; i++)
		{
			if (scores[i] > maxScore)
			{
				maxScore = scores[i];
				maxI = i;
			}
		}

		printf("The winner is Player %d, with a whopping score of %d\n", maxI + 1, scores[maxI]);

		puts("Want to play again?");
		fgets(line, 50, stdin);
		strtok(line, "\n");

		if (tolower(line[0]) == 'y')
			main_menu();
		//if (p1_score > p2_score)
		//{
		//	printf("Player 1 wins! Better luck next time Player 2.\n");
		//}
		//else if (p1_score < p2_score)
		//{
		//	printf("Take that Player 1! Player 2 is the champion forever.\n");
		//}
		//else
		//{
		//	printf("Either you two tied or I'm a crappy programmer! Good work!\n");
		//}
		break;
	case 3:
		puts("You don't even have the guts to play Yahtzee?");
		break;
	}
}

void print_yahtzee_header(void)
{
	FILE *infile;
	char line[50];
	infile = fopen(INFILE, "r");

	while (!feof(infile))
	{
		fgets(line, 50, infile);
		fputs(line, stdout);
	}

	puts("");
	puts("");
	fclose(infile);
}

void print_game_rules()
{
	printf("In Yahtzee, you roll a set of five dice three times and then choose a scoring category.\nThe categories are as follows:\n\n1. Sum of Ones\n2. Sum of Twos\n3. Sum of Threes\n4. Sum of Fours\n5. Sum of Fives\n6. Sum of Sixes\nIf your subtotal from the first six categories is greater than 63, you are awarded a 35 point bonus.\n\n7. Three of a Kind - Condition: Three dice with the same value - Points: Sum of all dice values\n8. Four of a Kind - Condition: Four dice with the same value - Points: Sum of all dice values\n9. Full House - Condition: One pair and three of a kind - Points: 25\n10. Small straight - Condition: At least four dice have consecutive values - Points: 30\n11. Large straight - Condition: All five dice have consecutive values - Points: 40\n12. Yahtzee - Condition: All five dice have the same value - Points: 50 (100 BONUS POINTS for more than one)\n13. Chance - No special condition - Points: Sum of all dice values\n");
}

int run_yahtzee(void)
{
	int option = 0, score = 0, round = 1, num_rolls = 0, *dice_array[6] = { 0,1,1,1,2,2 }, *hold_dice_array[6] = { 0 }, *dice_values_array[7] = { 0 }, *score_card_array[14] = { 0 }, *options_used[14] = { 0 };
	char line[50];
	srand((unsigned int)time(NULL));

	while (game_over(options_used) != GAME_OVER)
	{
		system("cls");
		//Rolls dice
		while (num_rolls < 3)
		{
			roll_dice(dice_array, hold_dice_array, num_rolls, round, score_card_array, options_used); //jeez this is a mess
			//Now it's the next roll
			++num_rolls;
		}
		//Now that final dice are specified, asks about scoring
		system("cls");
		print_scoring_categories(options_used, score_card_array);
		printf("Round: %d. Score: %d.\n\nRoll %d:", round, calc_score(score_card_array), 3);
		print_dice(dice_array, num_rolls);
		printf("\nWhat scoring category do you want to choose? ");
		do {
			fgets(line, 50, stdin);
			strtok(line, "\n");
			option = atoi(line);
		} while ((option < 1) || (option > 13) || (options_used[option] == USED));
		
		//Creates array containing dice values rather than dice rolls to use for easier scoring
		count_dice_values(dice_array, dice_values_array);
		update_score_card(dice_values_array, options_used, score_card_array, dice_array, option);
		num_rolls = 0;
		++round;
		system("pause");
	}
	score = calc_score(score_card_array);
	system("cls");
	print_scoring_categories(options_used, score_card_array);
	printf("Your final score was %d\n", score);
	system("pause");
	return score;
}

void randomize_dice(int *dice_array, int *hold_dice_array)
{
	for (int index = 0; index < NUM_DICE; ++index)
	{
		if (!hold_dice_array[index + 1])
		{
			dice_array[index + 1] = rand() % 6 + 1;
		}
	}
	reset_hold_dice_array(hold_dice_array, NOT_HELD);
}

void roll_dice(int *dice_array, int *hold_dice_array, int num_rolls, int round, int *score_card_array, int *options_used) 
{
	printf("Rolling");
	for (int r = 0; r < 3; ++r)
	{
		_sleep(333);
		printf(".");
	}
	printf("\n");
	//Rolls dice unless they were held in the previous roll
	randomize_dice(dice_array, hold_dice_array);
	//If it's the first two rolls, asks which dice should be held for the subsequent roll. After the third roll completes no more holding
	if (num_rolls < 2)
	{
		//Clears screen, leaving with only new dice values
		system("cls");
		print_scoring_categories(options_used, score_card_array);
		printf("Round: %d. Score: %d.\n\nRoll %d:", round, calc_score(score_card_array), num_rolls + 1);
		print_dice(dice_array, num_rolls);
		ask_hold(hold_dice_array);
	}
}

void ask_hold(int *hold_dice_array)

{
	int i = 0, num = 0;
	char line[50], *token[7];

	////Input validation loop. Get number of dice to hold. Min 0 Max 5
	//do {
	//	printf("\nHow many dice would you like to hold? ");
	//	scanf("%d", &num_hold);
	//} while ((num_hold < 0) || (num_hold > 5));

	////If holding all, doesn't need to ask which
	//if (num_hold == 5)
	//{
	//	reset_hold_dice_array(hold_dice_array, HELD);
	//}

	//Asks which dice, iterates through the loop for the number of dice held in previous step

	puts("Which dice do you want to hold?");
	fgets(line, 50, stdin);

	if (line[0] != '\n')
	{
		strtok(line, "\n");

		token[i++] = strtok(line, " ");
		while ((token[i++] = strtok(NULL, " ")) != NULL && i < 7);

		i--;
		
		for (int hold_iteration = 0; hold_iteration < i; hold_iteration++)
		{
			num = atoi(token[hold_iteration]);
			if (num > 0 && num < 6)
				hold_dice_array[num] = HELD;
		}
	}
}

void reset_hold_dice_array(int *hold_dice_array, int reset_number)
{
	for (int die = 0; die < NUM_DICE; ++die)
	{
		hold_dice_array[die+1] = reset_number;
	}
}

void count_dice_values(int *dice_array, int *dice_values_array)
{
	for (int index = 0; index < NUM_DICE_VALUES; ++index)
	{
		dice_values_array[index + 1] = 0;
	}
	for (int index = 0; index < NUM_DICE; ++index)
	{
		dice_values_array[dice_array[index+1]] += 1;
	}
}

void print_dice(int *dice_array, int num_rolls)
{
	int row = 0, column = 0;
	if (num_rolls > 2)
	{
		num_rolls = 2;
	}

	//New dice graphics engine
	printf("\n\n"DICE_HEADER);
	for (row = 0; row < DICE_HEIGHT; row++)
	{
		for (column = 0; column < NUM_DICE; ++column)
		{
			printDie(dice_array[column + 1], row);
			printf("  ");
		}
		puts("");
	}
}

void print_scoring_categories(int *options_used, int *score_card_array)
{
	printf("SCORE CARD:\n");
	for (int index = 1; index < (NUM_CATEGORIES+1); ++index)
	{
		switch (index)
		{
		case SUM_ONES:
			printf("1. Sum of Ones\t\t");
			break;
		case SUM_TWOS:
			printf("2. Sum of Twos\t\t");
			break;
		case SUM_THREES:
			printf("3. Sum of Threes\t");
			break;
		case SUM_FOURS:
			printf("4. Sum of Fours\t\t");
			break;
		case SUM_FIVES:
			printf("5. Sum of Fives\t\t");
			break;
		case SUM_SIXES:
			printf("6. Sum of Sixes\t\t");
			break;
		case THREE_OF_A_KIND:
			printf("SUBTOTAL: %d", calc_subtotal(score_card_array));
			if (calc_subtotal(score_card_array) >= 63)
			{
				printf(" (BONUS!) + 35 points");
			}
			printf("\n7. Three of a Kind\t");
			break;
		case FOUR_OF_A_KIND:
			printf("8. Four of a Kind\t");
			break;
		case FULL_HOUSE:
			printf("9. Full House\t\t");
			break;
		case SMALL_STRAIGHT:
			printf("10. Small straight\t");
			break;
		case LARGE_STRAIGHT:
			printf("11. Large straight\t");
			break;
		case YAHTZEE:
			printf("12. Yahtzee!\t\t");
			break;
		case CHANCE:
			printf("13. Chance\t\t");
			break;
		}
		if (options_used[index] == USED) 
		{
			printf(" (USED)");
			printf(" Score = %d\n", score_card_array[index]);
		}
		else
		{
			printf("\n");
		}
	}
	printf("\n");
}

int game_over(int *options_used)
{
	int option_iteration = 0;
	while (option_iteration < 13)
	{
		if (options_used[option_iteration + 1] == USED)
		{
			++option_iteration;
		}
		else
		{
			return CONTINUE;
		}

		if (option_iteration == 13)
		{
			return GAME_OVER;
		}
	}
}

int check_if_used(int *options_used, int option)
{
	if (options_used[option])
	{
		return USED; 
	}
	else
	{
		return UNUSED;
	}
}

void update_score_card(int *dice_values_array, int *options_used, int *score_card_array, int *dice_array, int option)
{
	int original_score = 0, final_score = 0;
	original_score = calc_score(score_card_array);
	switch (option)
	{
	//SUMS WORK
	case SUM_ONES:
	case SUM_TWOS:
	case SUM_THREES:
	case SUM_FOURS:
	case SUM_FIVES:
	case SUM_SIXES:
		score_card_array[option] = dice_values_array[option] * option;
		options_used[option] = USED;
		break;
	//THREE OF A KIND
	case THREE_OF_A_KIND:
		if (check_if_of_a_kind(dice_values_array, 3))
		{
			score_card_array[THREE_OF_A_KIND] = add_all_dice(dice_array);
		}
		options_used[THREE_OF_A_KIND] = USED;
		break;
	//FOUR OF A KIND
	case FOUR_OF_A_KIND:
		if (check_if_of_a_kind(dice_values_array, 4))
		{
			score_card_array[FOUR_OF_A_KIND] = add_all_dice(dice_array);
		}
		options_used[FOUR_OF_A_KIND] = USED;
		break;
	//FULL HOUSE WORKS
	case FULL_HOUSE:
		if (check_if_full_house(dice_values_array))
		{
			score_card_array[FULL_HOUSE] = 25;
		}
		options_used[FULL_HOUSE] = USED;
		break;
	case SMALL_STRAIGHT:
		if (check_if_straight(dice_values_array, 4))
		{
			score_card_array[SMALL_STRAIGHT] = 30;
		}
		options_used[SMALL_STRAIGHT] = USED;
		break;
	case LARGE_STRAIGHT:
		if (check_if_straight(dice_values_array, 5))
		{
			score_card_array[LARGE_STRAIGHT] = 40;
		}
		options_used[LARGE_STRAIGHT] = USED;
		break;
	//YAHTZEE WORKS
	case YAHTZEE:
		if (check_if_of_a_kind(dice_values_array, 5))
		{
			if (options_used[YAHTZEE] == UNUSED)
			{
				score_card_array[YAHTZEE] = 50;
				options_used[YAHTZEE] = CAN_STILL_YAHTZEE;
			}
			else if (options_used[YAHTZEE] == CAN_STILL_YAHTZEE)
			{
				score_card_array[YAHTZEE] += 100;
			}
			else
			{
				printf("Error you screwed up Ryan.");
				system("pause");
			}
		}
		else
		{
			options_used[YAHTZEE] = USED;
		}
		break;
	//CHANCE WORKS
	case CHANCE:
		score_card_array[CHANCE] = add_all_dice(dice_array);
		options_used[CHANCE] = USED;
		break;
	}
	final_score = calc_score(score_card_array);
	printf("You earned %d points!\n", (final_score - original_score));
}

int add_all_dice(int *dice_array)
{
	int dice_total = 0;
	for (int index = 0; index < NUM_DICE; ++index)
	{
		dice_total += dice_array[index+1];
	}
	return dice_total;
}

int calc_subtotal (int *score_card_array)
{
	int subtotal = 0;
	for (int category_iteration = 1; category_iteration < (NUM_SUBTOTAL_CATEGORIES + 1); ++category_iteration)
	{
		subtotal += score_card_array[category_iteration];
	}
	return subtotal;
}

int calc_score(int *score_card_array)
{
	int score = 0;
	for (int category_iteration = 1; category_iteration < (NUM_CATEGORIES+1); ++category_iteration)
	{
		score += score_card_array[category_iteration];
	}
	if (calc_subtotal(score_card_array) >= 63)
	{
		score += 35;
	}
	return score;
}

int check_if_of_a_kind(int *dice_values_array, int num_of_a_kind)
{
	for (int value_iteration = 0; value_iteration < NUM_DICE_VALUES; ++value_iteration)
	{
		//DEBUG
		//printf("%d", dice_values_array[value_iteration + 1]);
		if (dice_values_array[value_iteration + 1] >= num_of_a_kind)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int check_if_full_house(int *dice_values_array)
{
	for (int value_iteration = 0; value_iteration < NUM_DICE_VALUES; ++value_iteration)
	{
		if (dice_values_array[value_iteration + 1] == 3)
		{
			for (value_iteration = 0; value_iteration < NUM_DICE_VALUES; ++value_iteration)
			{
				if (dice_values_array[value_iteration + 1] == 2)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

int check_if_straight(int *dice_values_array, int required_in_a_row)
{
	int num_in_a_row = 0;
	for (int value_iteration = 0; value_iteration < NUM_DICE_VALUES; ++value_iteration)
	{
		//DEBUG
		//printf("%d", dice_values_array[value_iteration + 1]);
		if (dice_values_array[value_iteration + 1] >= 1)
		{
			num_in_a_row++;
		}
		else
		{
			if (num_in_a_row < required_in_a_row)
			{
				num_in_a_row = 0;
			}
		}
	}
	//DEBUG
	//printf("NUM in a ROW: %d\n", num_in_a_row);
	if (num_in_a_row >= required_in_a_row)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

//new and improved, suckers
void printDie(int diceNum, int diceRow)
{
	//This could be refactored to a scary degree

	//Prints top and bottom edges
	if (diceRow == 1 || diceRow == 5)
		printf(EDGE);

	else
	{
		switch (diceNum)
		{
		case 1: //#1
			switch (diceRow)
			{
			case 2: printf(EMPTY); break;
			case 3: printf(ONE_MIDDLE); break;
			case 4: printf(EMPTY); break;
			}
			break;
		case 2: //#2
			switch (diceRow)
			{
			case 2: printf(EMPTY); break;
			case 3: printf(TWO); break;
			case 4: printf(EMPTY); break;
			}
			break;
		case 3: //#3
			switch (diceRow)
			{
			case 2: printf(ONE_RIGHT); break;
			case 3: printf(ONE_MIDDLE); break;
			case 4: printf(ONE_LEFT); break;
			}
			break;
		case 4: //#4
			switch (diceRow)
			{
			case 2: printf(TWO); break;
			case 3:	printf(EMPTY); break;
			case 4: printf(TWO); break;
			}
			break;
		case 5: //#5
			switch (diceRow)
			{
			case 2: printf(TWO); break;
			case 3:	printf(ONE_MIDDLE); break;
			case 4: printf(TWO); break;
			}
			break;
		case 6: //#6
			switch (diceRow)
			{
			case 2: printf(TWO); break;
			case 3: printf(TWO); break;
			case 4: printf(TWO); break;
			}
			break;
		}
	}
}

//STUBBED DOESN'T WORK YET DON'T USE
void saveHighScore(int maxScore)
{
	FILE *outfile = NULL;
	char line[20][5], name[20][5];
	int score[5];

	outfile = fopen(OUTFILE, "r");

	for (int i = 0; i < 5; i++)
		fgets(line[i], 20, outfile);

	//no error checking because file is probably good. Someone screwed it up that's their fault
	for (int i = 0; i < 5; i++)
	{
		strcpy(name[i], strtok(line, "\t"));
		score[i] = atoi(strtok(NULL, "\t"));
	}

	//maxScore belongs on the list, but where does it go?
	if (maxScore > score[4])
	{
		for (int i = 0; i < 4; i++)
		{

		}
	}
}