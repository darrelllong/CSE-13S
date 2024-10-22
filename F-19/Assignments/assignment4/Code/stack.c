// Author: DDEL
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# include "stack.h"

stack *new_stack(int size, char peg)
{
	struct stack *s = (struct stack*)malloc(sizeof(struct stack));

	s->name = peg;
	s->capacity = size;
	s->top = 0;
	s->items = (int*) malloc(sizeof(int) * size);

	return s;
}

int pop(stack *s)
{
	if (s->top > 0)
	{
		s->top -= 1;
		return s->items[s->top];
	}
	else
	{
		printf("Failed to pop off stack.\n");
		return -1;
	}
}

void push(stack *s, int i)
{
	if (s->top == s->capacity)
	{
		s->capacity *= 2;
		s->items = (int *) realloc(s->items, s->capacity * sizeof(int));
	}
	if (s && s->items)
	{
		s->items[s->top] = i;
		s->top += 1;
	}
	return;
}

void print_stack(stack *s)
{
	printf("%c | ", s->name);
	
	for (int i = 0; i < s->top; i++)
	{
		printf("%d ", s->items[i]);
	}
	
	
	printf("\n");
	return;
}
