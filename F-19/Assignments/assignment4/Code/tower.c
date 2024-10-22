# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# include "stack.h"

int stack_moves = 0;
int recursive_moves = 0;

int calculate_min_moves(int x)
{
	return (1 << x) - 1;
}

void init_peg(stack *peg, int disks)
{
	for(int i = disks; i >= 1; i--)
	{
		push(peg, i);
	}
}

void recursive_move(int x, char from, char to, char neigh) {
  if (x == 1) // Base case
  {
    printf("Move disk 1 from peg %c to peg %c", from, to);
    recursive_moves++;
    return;
  }
  recursive_move(x - 1, from, neigh, to);
  recursive_moves++;
  printf("\nMove disk %d from peg %c to peg %c\n", x, from, to);
  recursive_move(x - 1, neigh, to, from);
}

void stack_move(int x, stack *from, stack *to, stack *neigh)
{
	int temp1, temp2, temp3;
	
	if (x == 1) // Base case
	{
   		printf("Move disk 1 from peg %c to peg %c", from->name, to->name);
   		temp1 = pop(from);
   		push(to, temp1);
   		stack_moves++;
   		return;
	}

	temp2 = pop(from);
	stack_move(x-1, from, neigh, to);
	push(neigh, temp2);

  	printf("\nMove disk %d from peg %c to peg %c\n", x, from->name, to->name);
	stack_moves++;
  	
	temp3 = pop(neigh);
  	stack_move(x-1, neigh, to, from);
  	push(to, temp3);  
}

int main(int argc, char **argv)
{
	int flag, disks = 5, min_moves, stack_flag = 0, recursive_flag = 0;

  	while ((flag = getopt(argc, argv, "srn:")) != -1) 
  	{
		switch(flag) 
		{
			case 's':
			{
				stack_flag = 1;
				break;
			}
			case 'r':
			{
				recursive_flag = 1;
				break;
			}
			case 'n':
			{
				disks = atoi(optarg);
				break;
			}
		}
	}
	
	if (stack_flag == 1)
	{
		printf("================================\n");
		printf("----------   STACKS   ----------\n");
		printf("================================\n");

		// Declare and init new stacks
		stack *A = new_stack(disks, 'A');
		stack *B = new_stack(disks, 'B');
		stack *C = new_stack(disks, 'C');

		// Push disks onto peg A to start
		init_peg(A, disks);

		// Pegs before playing for debugging
		// print_stack(A);
		// print_stack(B);
		// print_stack(C);

		// Play game
		stack_move(disks, A, B, C);

		// Pegs after playing for debugging
		// printf("\n");
		// print_stack(A);
		// print_stack(B);
		// print_stack(C);	

		// Printing moves and what not
		min_moves = calculate_min_moves(disks);
		printf("\n\nMinimum amount of moves required: %d", min_moves);
		printf("\nNumber of moves used: %d\n\n", stack_moves);
		
		// No quiero memory leaks
		del_stack(A);
		del_stack(B);
		del_stack(C);
	}

	if (recursive_flag == 1)
	{
		printf("================================\n");
		printf("--------   RECURSION   ---------\n");
		printf("================================\n");
		recursive_move(disks, 'A', 'B', 'C');
	 	min_moves = calculate_min_moves(disks);
	  	printf("\n\nMinimum amount of moves required: %d", min_moves);
	  	printf("\nNumber of moves used: %d\n",recursive_moves);
	}

	return 0;
}
