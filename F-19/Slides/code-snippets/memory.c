int main(void) {
  int fib[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
  return 0;
}

void inc_by_ref(int *x) {
  *x = (*x) + 1;
  return;
}

int inc_by_val(int x) {
  return x + 1;
}

int main(void) {
  int x = 5;
  inc_by_ref(&x);
  x = inc_by_val(x);
  return 0;
}

int main(void) {
  int fib[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };

  int *fib_ptr = fib;
  while (fib_ptr < (fib + 10)) {
    *(fib_ptr)++;   // Increment array element.
    fib_ptr++;      // Increment pointer.
  }

  return 0;
}

#include <stdio.h>

int main(void) {
  int a = 42;
  int *ptr_a = &a;
  int *ptr_b = &a;

  // Two pointers can point to the same address.
  printf("The address of pointer A is %p\n", ptr_a);
  printf("The address of pointer B is %p\n", ptr_b);
  return 0;
}

#include <stdio.h>

int main(void) {
  int foo = 0;

  // Print out address of foo.
  printf("Address of foo: %p\n", &foo);
  return 0;
}

#include <stdio.h>

void increment_two_ints(int *a, int *b) {
  *a += 1;
  *b += 1;
  return;
}

int main(void) {
  int x = 3;
  int y = 4;
  increment_two_ints(&x, &y);

  // Now, x is 4 and y is 5.
  printf("The value of x is now: %d\n", x);
  printf("The value of y is now: %d\n", y);
  return 0;
}

#include <stdio.h>

int main(void) {
  int foo = 13;

  // The * denotes that bar is a pointer variable.
  // The address of foo is stored in bar.
  int *bar = &foo;

  // Dereference bar using * to print out value at its stored address.
  printf("The value in the address stored in bar: %d\n", *bar);
  return 0;
}

#include <stdio.h>

int main(void) {
  int age;
  double gpa;

  // Pass age and gpa variables by reference.
  printf("Please enter your age and gpa: ");
  scanf("%d %lf", &age, &gpa);
  return 0;
}

int main(void) {
  int a[2][2];
  int x = matrix_determinant_by_val(a[0][0], a[0][1], a[1][0], a[1][1]);
  int x = matrix_determinant_by_ref(a);
  return 0;
}

#include <stdio.h>

int main(void) {
  char *str = "Hello World!";
  printf("str[0]: %c\n", *str);
  printf("str[1]: %c\n", *(str + 1));
  return 0;
}

#include <stdio.h>

int main(void) {
  char *hello = "Hello World!";
  char *bye = { 'G', 'o', 'o', 'd', 'b', 'y', 'e', 0 };

  // Strings can be indexed.
  printf("hello[3]: %c\n", hello[3]);
  return 0;
}

int main(int argc, char **argv);

#include <stdio.h>

void increment(int *a) {
  *a += 1;
  return;
}

int main(void) {
  // Make func_ptr point at increment().
  void (*func_ptr)(int *) = increment;

  int x = 5;

  // Increment x by invoking increment().
  func_ptr(&x);
  return 0;
}

#include <stdio.h>
#include <math.h>

//
// Numerically computes the derivative of a function f about the point x0.
// Takes a double as an argument and returns a double.
//
double diff(double (*f)(double), double x0) {
  double epsilon = 0.00001;
  double delta_x = f(x0 + epsilon) - f(x0 - epsilon);
  return delta_x / (2 * epsilon);
}

int main(void) {
  for (double x = 0.0; x < (2 * M_PI); x += (M_PI / 16)) {
    printf("x = %6.3f, cos(x) = %6.3f, d/dx(cos(x)) = %6.3f\n"
      i, cos(x), diff(sin, x));
  }

  return 0;
}

#include <stdio.h>

enum { INCREMENT, DECREMENT };

void increment(int *a) {
  *a += 1;
  return;
}

void decrement(int *a) {
  *a -= 1;
  return;
}

int main(void) {
  // func_table is an array of function pointers.
  // func_table[0] is increment().
  // func_table[1] is decrement().
  void (*func_table[])(int *) = { INCREMENT, DECREMENT };

  int x = 5;

  func_table[INCREMENT](&x);  // Increments x to 6.
  func_table[DECREMENT](&x);  // Decrements x back to 5.
  return 0;
}



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//
// Allocates memory for an array of 10 ints.
// Sets each array index to the value of the index.
//
int main(void) {
  int *arr = malloc(10 * sizeof(int));
  assert(n);

  for (int i = 0; i < 10; ++i) {
    arr[i] = i;
  }

  return 0;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//
// Functions like previous malloc() example.
// Takes advantage of zeroed-out array to reach same result.
//
int main(void) {
  int *arr = calloc(10, sizeof(int));
  assert(arr);

  for (int i = 0; i < 10; ++i) {
    arr[i] += i;
  }

  return 0;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int **matrix_create(int n) {
  int **m = (int **)calloc(n, sizeof(int *));
  assert(m);

  for (int i = 0; i < n; ++i) {
    m[i] = (int *)calloc(n, sizeof(int));
    assert(m[i]);
  }

  return m;
}

void matrix_delete(int **m, int n) {
  for (int i = 0; i < n; ++i) {
    free(m[i]);
    m[i] = NULL;
  }

  free(m);
  m = NULL;
  return;
}

int main(void) {
  int n = 10;
  int **m = matrix_create(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%d ", m[i][j]);
    }

    printf("\n");
  }

  matrix_delete(m);
  return 0;
}

#ifndef __STACK_H__
#define __STACK_H__

#include <inttypes.h>
#include <stdbool.h>

#define MIN_STACK 16
#define INVALID   0xdeadd00d

typedef uint32_t item;

typedef struct Stack {
  uint32_t size;
  uint32_t top;
  item *entries;
} Stack;

Stack *stack_create(void);

void stack_delete(Stack *);

item stack_pop(Stack *);

void stack_push(Stack *);

bool stack_empty(Stack *);

#endif

Stack *stack_create(void) {
  Stack *s = (Stack *)malloc(MIN_STACK, sizeof(Stack));
  assert(s);
  s->size = MIN_STACK;
  s->top = 0;
  s->entries = (item *)calloc(MIN_STACK, sizeof(item));
  return s;
}

void stack_push(Stack *s, item i) {
  if (s->top == s->size) {
    s->size *= 2;
    s->entries = (item *)realloc(s->entries, s->size * sizeof(item));
  }

  s->entries[s->top] = i;
  s->top += 1;
  return;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *arr = (int *)calloc(7, sizeof(int));
  assert(arr);

  for (int i = 0; i < 7; ++i) {
    arr[i] = i;
  }

  arr = (int *)realloc(arr, 10 * sizeof(int));
  assert(arr);

  for (int i = 0; i < 10; ++i) {
    printf("arr[i] = %d\n", arr[i]);
  }

  return 0;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int **arr = (int **)calloc(10, sizeof(int *));
  assert(arr);

  for (int i = 0; i < 10; ++i) {
    arr[i] = (int *)calloc(10, sizeof(int));
    assert(arr[i]);
  }

  for (int i = 0; i < 10; ++i) {
    free(arr[i]);
    arr[i] = NULL;
  }

  free(arr);
  arr = NULL;
  return 0;
}

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct NamelessPerson {
  uint32_t height;
  uint32_t age;
} Person;

typedef struct RandomHouse {
  Person **members;
  uint32_t size;
} House;

Person *person_create(uint32_t height, uint32_t age) {
  assert(height > 0 && age > 0);
  Person *p = (Person *)malloc(sizeof(Person));
  assert(p);
  p->height = height;
  p->age = age;
  return p;
}

void person_delete(Person *p) {
  free(p);
  p = NULL;
  return;
}

House *house_create(uint32_t size) {
  assert(size > 0);
  House *h = (House *)malloc(sizeof(House));
  assert(h);
  h->members = (Person **)malloc(size * sizeof(Person *));
  assert(h->members);
  h->size = size;
  return h;
}

void house_delete(House *h) {
  free(h->members);
  h->members = NULL;
  free(h);
  h = NULL;
  return;
}

void house_delete(House *h) {
  for (int i = 0; i < h->size; ++i) {
    person_delete(h->members[i]);
  }

  free(h->members);
  h->members = NULL;
  free(h);
  h = NULL;
  return;
}

int main(void) {
  Person *sabrina = person_create(162, 21);
  Person *tim = person_create(182, 21);
  Person *eugene = person_create(175, 21);
  House *hut = house_create(3);
  hut->members[0] = sabrina;
  hut->members[1] = tim;
  hut->members[2] = eugene;
  house_delete(hut);
  return 0;
}


