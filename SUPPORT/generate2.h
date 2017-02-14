#ifndef _GENERATE2_
#define _GENERATE2_

#include <iostream>
#include <cmath>

#include "geometricObject2.h"
#include "betaSet.h"
#include "window.h"
#include "delone10.h"


std::list<betaSet> quasicrystal1D( window win, betaSet x, betaSet y, betaSet start );

CdeloneSet<betaSet> quasicrystal2D( std::list<betaSet> &x, std::list<betaSet> &y );
CdeloneSet<betaSet> quasicrystal2D( window win, std::string &x, std::string &y );
CdeloneSet10<betaSet> quasicrystal2D10( window win, std::string &x, std::string &y );

CdeloneSet<betaSet> quasicrystal2D( window2D &win, betaSet x1, betaSet x2, betaSet y1, betaSet y2 );

Cpoint<betaSet> transformBeta( const Cpoint<betaSet> &point );

std::list<std::string> language( window win, int n );

std::list<std::string> language( window hypowin, window hyperwin, int n );
void addSplitForHypowin( window hypowin, betaSet start, betaSet length, std::string word, std::list<betaSet>* delimiter, std::map <betaSet, std::string>* language );

bool fitToWindow( window2D* win, CdeloneSet<betaSet> delone );



betaSet minWord( std::list<std::string> lang, window win );

#endif
