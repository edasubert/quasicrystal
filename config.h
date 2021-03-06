#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

typedef alphaSet numberType;
//typedef betaSet numberType;
//typedef rhombus<numberType> windowType;
//typedef circle<numberType> windowType;
typedef polygon<numberType> windowType;

windowType getWindow(numberType winSize)
{
  //return polygon<numberType>::rhombic(winSize);
  return polygon<numberType>::octagon(winSize);
  //return polygon<numberType>::dodecagon(winSize);
  //return windowType(winSize);
}

//numberType const const_winSize = numberType::get(1,0,1)/numberType::get(0,1,1) + (numberType::get(1,0,1)-numberType::get(1,0,1)/numberType::get(0,1,1))*numberType::get(2,0,10);
//numberType const const_winSize = numberType::get(-1,1,2);

//numberType covering = numberType::get(1, 3, 2);
//numberType lengthToCover = numberType::get(4, 0)*covering;
//numberType largestTile = numberType::get(4, 11, 4);

// cell colors
std::string const const_fillColor = "#689F38";
std::string const const_strokeColor = "#263238";
std::string const_strokeWidth(numberType winSize)
{
  std::ostringstream convert;
  convert << (0.07)*winSize;
  return convert.str();
}
  
// window colors
std::string const const_windowfillColor = "#111111";
std::string const const_windowstrokeColor = "#37474F";
std::string const_windowstrokeWidth(numberType winSize)
{
  std::ostringstream convert;
  convert << (0.003)*winSize;
  return convert.str();
}


#endif
