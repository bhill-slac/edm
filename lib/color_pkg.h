//  edm - extensible display manager

//  Copyright (C) 1999 John W. Sinclair

//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef __color_pkg_h
#define __color_pkg_h 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/Protocols.h>

#include "sys_types.h"
#include "avl.h"
#include "thread.h"
#include "gc_pkg.h"
#include "color_pkg.str"

#define COLORINFO_SUCCESS 1
#define COLORINFO_EMPTY 100
#define COLORINFO_FAIL 102
#define COLORINFO_NO_FILE 104
#define COLORINFO_NO_COLOR 106

#define NUM_SPECIAL_COLORS 7
#define NUM_BLINKING_COLORS 8
#define MAX_COLORS 88
#define NUM_COLOR_COLS 11

#define COLORINFO_K_DISCONNECTED 0
#define COLORINFO_K_INVALID 1
#define COLORINFO_K_MINOR 2
#define COLORINFO_K_MAJOR 3
#define COLORINFO_K_NOALARM 4

typedef int (*ruleFuncType)( double v1, double v2 );

typedef struct ruleConditionTag {
  struct ruleConditionTag *flink;
  double value1;
  ruleFuncType ruleFunc1;
  double value2;
  ruleFuncType ruleFunc2;
  ruleFuncType connectingFunc;
  int result;
  char *resultName;
} ruleConditionType, *ruleConditionPtr;

typedef struct ruleTag {
  ruleConditionPtr ruleHead;
  ruleConditionPtr ruleTail;
} ruleType, *rulePtr;

typedef struct colorCacheTag {
  AVL_FIELDS(colorCacheTag)
  unsigned int rgb[3]; // [0]=r, [1]=g, [2]=b
  unsigned int pixel;
  unsigned int blinkRgb[3]; // [0]=r, [1]=g, [2]=b
  unsigned int blinkPixel;
  int index;
  char *name;
  rulePtr rule;
} colorCacheType, *colorCachePtr;

class colorInfoClass {

private:

friend void doColorBlink (
  XtPointer client,
  XtIntervalId *id );

friend void colorFormEventHandler (
  Widget w,
  XtPointer client,
  XEvent *e,
  Boolean *continueToDispatch );

friend class colorButtonClass;

int max_colors, num_blinking_colors, num_color_cols, usingPrivateColorMap;

AVL_HANDLE colorCacheByColorH;
AVL_HANDLE colorCacheByPixelH;
AVL_HANDLE colorCacheByIndexH;
AVL_HANDLE colorCacheByNameH;

Display *display;
int screen;
int depth;
Visual *visual;
Colormap cmap;
unsigned int fg;

/*  unsigned int *colors[MAX_COLORS+NUM_BLINKING_COLORS]; */
/*  unsigned long *blinkingColorCells[NUM_BLINKING_COLORS]; */
/*  XColor *blinkingXColor[NUM_BLINKING_COLORS], */
/*   *offBlinkingXColor[NUM_BLINKING_COLORS]; */

unsigned int *colors, *blinkingColors;
unsigned long *blinkingColorCells;
XColor *blinkingXColor, *offBlinkingXColor;

int special[NUM_SPECIAL_COLORS];
int specialIndex[NUM_SPECIAL_COLORS];
int numColors, blink;

int curIndex, curX, curY;

XtAppContext appCtx;
XtIntervalId incrementTimer;
int incrementTimerValue;

Widget activeWidget;
int *curDestination;

gcClass gc;

Widget shell, rc, mbar, mb1, mb2, form, rc1, rc2, fgpb, bgpb;

// color file processing
static const int MAX_LINE_SIZE = 255;

static const int GET_1ST_NONWS_CHAR = 1;
static const int GET_TIL_END_OF_TOKEN = 2;
static const int GET_TIL_END_OF_QUOTE = 3;
static const int GET_TIL_END_OF_SPECIAL = 4;

static const int GET_FIRST_TOKEN = 1;
static const int GET_NUM_COLUMNS = 2;
static const int GET_MAX = 3;
static const int GET_RULE = 4;
static const int GET_RULE_CONDITION = 5;
static const int GET_RULE_CONNECTOR = 6;
static const int GET_RULE_INDEX = 7;
static const int GET_COLOR = 8;
static const int GET_ALARM_PARAMS = 9;

int readFile, tokenState, parseIndex, parseLine, tokenFirst, tokenLast,
 tokenNext, gotToken, colorIndex;
char parseBuf[MAX_LINE_SIZE+1], parseToken[MAX_LINE_SIZE+1];
FILE *parseFile;

int maxColor, numPaletteCols;

public:

static const int SUCCESS = 1;
static const int FAIL = 0;

int change;

int colorWindowIsOpen;

colorInfoClass::colorInfoClass ( void );   // constructor

colorInfoClass::~colorInfoClass ( void );   // destructor

int colorInfoClass::colorChanged ( void ) {
int i;
  i = change;
  change = 0;
  return i;
}

int colorInfoClass::ver3InitFromFile (
  FILE *f,
  XtAppContext app,
  Display *d,
  Widget top,
  char *fileName );

int colorInfoClass::initFromFile (
  XtAppContext app,
  Display *d,
  Widget top,
  char *fileName );

int colorInfoClass::openColorWindow( void );

int colorInfoClass::closeColorWindow( void );

unsigned int colorInfoClass::getFg( void );

void colorInfoClass::setCurDestination( int *ptr );

int *colorInfoClass::getCurDestination( void );

int colorInfoClass::setActiveWidget( Widget w );

Widget colorInfoClass::getActiveWidget( void );

Widget colorInfoClass::createColorButton(
  Widget parent,
  Arg args[],
  int num_args );

int colorInfoClass::getRGB (
  unsigned int pixel,
  int *r,
  int *g,
  int *b );

int colorInfoClass::setRGB (
  int r,
  int g,
  int b,
  unsigned int *pixel );

int colorInfoClass::getIndex (
  unsigned int pixel,
  int *index );

int colorInfoClass::setIndex (
  int index,
  unsigned int *pixel );

int colorInfoClass::getSpecialColor (
  int index );

int colorInfoClass::getSpecialIndex (
  int index );

Colormap colorInfoClass::getColorMap ( void );

int colorInfoClass::setCurIndexByPixel (
  unsigned int pixel );

int colorInfoClass::setCurIndex (
  int index );

int colorInfoClass::canDiscardPixel (
  unsigned int pixel );

unsigned int colorInfoClass::getPixelByIndex (
  int index );

unsigned int colorInfoClass::pix ( // same as getPixelByIndex
  int index );

int colorInfoClass::pixIndex (
  unsigned int pixel );

void colorInfoClass::initParseEngine (
  FILE *f );

void colorInfoClass::parseError (
 char *msg );

int colorInfoClass::getToken (
 char toke[255+1] );

};

#endif
