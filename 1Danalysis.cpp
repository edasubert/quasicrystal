#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

// analysis of 1D window

int main( int argc, char ** argv )
{
  std::cout << "ANALYSIS OF 1D WINDOW" << std::endl << std::flush;
  std::cout << "------------------------------" << std::endl << std::flush;
  
  std::string fileName = argv[1];
  
  // window definition
  numberType winSize;

  //winSize = const_winSize;
  
  winSize = (numberType::get(-1,1,2));
  numberType origin(0,0);
  
  window<numberType> win(numberType::get(0,0), winSize);
  //window<numberType> win(winSize*numberType::get(-1,0,2), winSize*numberType::get(1,0,2));
  
  std::cout << "win size: ";
  print(std::cout, winSize);
  std::cout << '\t' << winSize << std::endl;
  std::cout << "Large: " << win.Large() << std::endl;
  std::cout << "L: " << win.Lchar() << "\t";
  print(std::cout, win.L());
  std::cout << "\t" << win.L() << "\t ' " << win.L().star() << std::endl;
  std::cout << "M: " << win.Mchar() << "\t";
  print(std::cout, win.M());
  std::cout << "\t" << win.M() << "\t ' " << win.M().star() << std::endl;
  std::cout << "S: " << win.Schar() << "\t";
  print(std::cout, win.S());
  std::cout << "\t" << win.S() << "\t ' " << win.S().star() << std::endl << std::endl;
  
  std::cout << "c: ";
  print(std::cout, win.c());
  std::cout << "\t" << win.c() << std::endl;
  std::cout << "a: ";
  print(std::cout, win.a());
  std::cout << "\t" << win.a() << std::endl;
  std::cout << "b: ";
  print(std::cout, win.b());
  std::cout << "\t" << win.b() << std::endl;
  std::cout << "d: ";
  print(std::cout, win.d());
  std::cout << "\t" << win.d() << std::endl;
  
  // estimate covering radius
  numberType distanceToCover = numberType::get(2,0)*win.Large();
  
  numberType x1 = numberType::get(0,0);
  numberType x2 = distanceToCover;
  
  // generate superset of all possible finite sections
  int wordLength = 2;
  
  std::list<std::string> lang = language(win, wordLength);
  //print(std::cout, minWord(lang, win));
  //std::cout << "\t" << (minWord(lang, win)<=numberType::get(2,4)) << std::endl;
  
  //lang.push_back("NN");
  //lang.push_back("ML");
  
  std::list<std::list<numberType> > cells;
  
  for (std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it)
  {
    std::list<numberType> tmp;
    tmp.push_back(win.char2space(*it->begin())*numberType(-1,0,2));
    tmp.push_back(win.char2space(*(++it->begin()))*numberType(1,0,2));
    
    cells.push_back(tmp);
  }
  
  std::cout << "------------------------------" << std::endl;
  std::cout << "language" << std::endl;
  
  for (std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
  std::cout << "------------------------------" << std::endl;
  
  std::cout << "cells size: " << cells.size() << std::endl;
  
  //cells.sort();
  
  std::cout << "------------------------------" << std::endl;
  std::cout << "cells" << std::endl;
  
  for (std::list<std::list<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    for (std::list<numberType>::iterator ot = it->begin(); ot != it->end(); ++ot)
    {
      print(std::cout, *ot);
      std::cout << '\t' << *ot << "\t\t";
    }
    std::cout << ((*++it->begin()).abs()+(*it->begin()).abs()) << std::endl;
  }
  std::cout << "------------------------------" << std::endl;
  
  // cut window sections
    // intersection is max of lower boundary and min of upper
  std::list<std::list<numberType> > sections;
  
  for (std::list<std::list<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    std::list<numberType> tmp;
    
    numberType a = winSize*numberType::get(-1,0,2);
    numberType b = winSize*numberType::get( 1,0,2);
    
    for (std::list<numberType>::iterator ot = it->begin(); ot != it->end(); ++ot)
    {
      a = max(a, winSize*numberType::get(-1,0,2) - (*ot*numberType::get(2,0)).star());
      b = min(b, winSize*numberType::get( 1,0,2) - (*ot*numberType::get(2,0)).star());
      print(std::cout, ot->star());
      std::cout << "\t" << ot->star() << std::endl;
      //std::cout << numberType::get(0,0) - (ot->star() + winSize*numberType::get(1,0,2)) << '\t' << numberType::get(0,0) - (ot->star() - winSize*numberType::get(1,0,2)) << std::endl;
    }
    
    tmp.push_back(a);
    tmp.push_back(b);
    sections.push_back(tmp);
  }
  
  std::cout << "------------------------------" << std::endl;
  std::cout << "sections" << std::endl;
  
  for (std::list<std::list<numberType> >::iterator it = sections.begin(); it != sections.end(); ++it)
  {
    for (std::list<numberType>::iterator ot = it->begin(); ot != it->end(); ++ot)
    {
      print(std::cout, *ot);
      std::cout << "\t\t" << *ot << "\t\t\t";
    }
    std::cout << ((*++it->begin())-(*it->begin())) << std::endl;
  }
  std::cout << "------------------------------" << std::endl;
  
  
  // SVG
  std::ofstream myfile(fileName);
  
  int y = 1;
  
  myfile << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
  myfile << "<svg width=\"" << 1920 << "\" height=\"" << 1920*(cells.size()+1)*100/winSize/100 << "\" viewBox=\"" << -winSize/2 << " " << 0 << " " << winSize << " " << (cells.size()+1) << "\">" << std::endl;
  
  myfile << "<line x1=\"" << -winSize/2 << "\" y1=\"" << 0 << "\" x2=\"" << winSize/2 << "\" y2=\"" << 0 << "\" stroke=\"#f33333\" stroke-width=\"0.1\" />" << std::endl;
  for (std::list<std::list<numberType> >::iterator it = sections.begin(); it != sections.end(); ++it)
  {
    myfile << "<line x1=\"" << *it->begin() << "\" y1=\"" << y << "\" x2=\"" << *++it->begin() << "\" y2=\"" << y << "\" stroke=\"#333333\" stroke-width=\"0.1\" />" << std::endl;
    ++y;
    //std::cout << ((*++it->begin()).abs()+(*it->begin()).abs()) << std::endl;
  }
  
  myfile << "</svg>";
  myfile.close();
}
