# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

typedef enum faciem {LEFT, RIGHT, CENTER, PASS} faces;
faces die[] = {LEFT, RIGHT, CENTER, PASS, PASS, PASS};
const char *names[] = {"Whoopi", "Dale", "Rosie", "Jimmie", "Barbara", "Kyle", "Raven", "Tony", "Jenny", "Clint" };

int main(void)
{
	int bank[] = {3,3,3,3,3,3,3,3,3,3};
	int pot, seed, players, playersTurn = 0, winner; 
	bool endGame = false;
	
	printf("Random seed: ");
	scanf("%d", &seed);
	srand(seed);
	
	printf("How many players? ");
	scanf("%d", &players);
	
	while (!endGame) 
	{		
		int playersCurrentMoney = bank[playersTurn], amountOfRolls;
		
		if(playersCurrentMoney > 0) 
		{
			printf("%s rolls... ", names[playersTurn]);
		}

		for (amountOfRolls = 0; amountOfRolls < playersCurrentMoney && amountOfRolls < 3; amountOfRolls++)
		{
			int roll = rand() % 6;
			if (die[roll] == LEFT && playersTurn != 0) 
			{
				bank[playersTurn]--;
				bank[playersTurn - 1]++;
				printf("gives $1 to %s ", names[playersTurn -1]);
			}
			
			else if (die[roll] == LEFT && playersTurn == 0) 
			{
				bank[playersTurn]--; 
				bank[players - 1]++;
				printf("gives $1 to %s ", names[players - 1]);
			}		
		
			else if (die[roll] == RIGHT && playersTurn != (players - 1))
			{
				bank[playersTurn]--;
				bank[playersTurn + 1]++; 
				printf("gives $1 to %s ", names[playersTurn + 1]);
			}

			else if (die[roll] == RIGHT && playersTurn == (players - 1))
			{
				bank[playersTurn]--; 
				bank[0]++;
				printf("gives $1 to %s ", names[0]);
			}
			
			else if (die[roll] == CENTER )
			{
				pot++;
				bank[playersTurn]--;
				printf("puts $1 in the pot ");
			}
			else 
			{
				printf("gets a pass ");
			}
		}
		if (amountOfRolls > 0)
		{
			printf("\n");
		}
	
		if (playersTurn < (players - 1)) 
		{ 
			playersTurn++;
		
		}

		else if (playersTurn ==  (players - 1))
		{
			playersTurn = 0;
		
		}
		
		int j;	
		for (j = 0; j < players; j++ )
		{
			if(bank[j] + pot == (players * 3)) 
			{
				winner = j;
				endGame = true;
			}	
		}	
	}

	printf("%s wins the $%d pot with $%d left in the bank!", names[winner], pot, bank[winner]);
	printf("\n");
	
	return 0;
}
