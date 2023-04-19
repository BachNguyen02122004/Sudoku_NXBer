#ifndef DRAW_H
#define DRAW_H

#include "Function.h"
#include "LTexture.h"
#include "heart.h"
#include "CellData.h"
#include "Area.h"
extern int sel_x ;
extern int sel_y ;
extern LTexture digTxtrs [9] ;
extern SudokuBoard ans, su;
extern CellData data [9][9] ;
extern mang heart;

void drawGrid ();
void drawSelection ();

void drawInvalidCells ();
void drawText () ;
#endif