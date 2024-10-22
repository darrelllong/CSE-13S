#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct Universe Universe;

Universe *uv_create(int m, int n);

void uv_delete(Universe **u);

int uv_rows(Universe *u);

int uv_cols(Universe *u);

void uv_set_cell(Universe *u, int m , int n);

void uv_clr_cell(Universe *u, int m, int n);

int uv_get_cell(Universe *u, int m, int n);

bool uv_populate(Universe *u, FILE *infile);

int uv_census(Universe *u, int m, int n, bool toroidal);

void uv_print(Universe *u, FILE *outfile);

#endif
