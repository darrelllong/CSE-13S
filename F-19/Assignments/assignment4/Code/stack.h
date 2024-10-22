// Author: DDEL
# ifndef _STACK_H_DL
# define _STACK_H_DL

# include <stdint.h>
# include <stdbool.h>

typedef struct stack
{
	char name;
	int top;
	int capacity; 
	int *items;
} stack;

stack *new_stack(int, char);

int pop(stack *);

void push(stack *, int);

// extern int if_empty(stack *);

// void print_stack(stack *, stack *, stack *, int);
void print_stack(stack *);

static inline void del_stack(stack *s) { free(s->items); free(s); return; }

static inline bool empty_stack(stack *s) { return s->top == 0; }

# endif
