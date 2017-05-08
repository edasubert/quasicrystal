#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

#include "config.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <list>
#include <string>


int main (int argc, char* argv[])
{ 
  
  // INPUT
  numberType const_winSize;
  numberType covering;
  numberType largestTile;
  std::string filenameSuffix;
  std::string filenamePrefix;
  std::string filenameFinite;
  std::string filenameMore;
  std::string filenameList;
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  std::cin >> filenamePrefix;
  std::cin >> filenameSuffix;
  
  filenameFinite = filenamePrefix + "finite" + filenameSuffix;
  filenameMore = filenamePrefix + "more" + filenameSuffix;
  filenameList = filenamePrefix + "list" + filenameSuffix;
  
  
  std::string line;
  numberType input;
  
  // singular square file
  std::string singularSquareFilename = "closed/singularSQUARE";
  std::ifstream singularSquareFile(singularSquareFilename);
  std::list<numberType> singularSizesSqrt;
  if (singularSquareFile.is_open())
  {
    while (singularSquareFile)
    {
      singularSquareFile >> input;
      singularSizesSqrt.push_back(input);
    }
    singularSquareFile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 
  
  singularSizesSqrt.sort();
  singularSizesSqrt.unique();
  
  // singular file
  std::string singularFilename = "closed/singular";
  std::ifstream singularFile(singularFilename);
  std::list<numberType> singularSizes;
  std::list<numberType> singularSizesUsed;
  if (singularFile.is_open())
  {
    while (singularFile)
    {
      singularFile >> input;
      singularSizes.push_back(input);
    }
    singularFile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 
  
  singularSizes.sort();
  singularSizes.unique();
  std::cout << "singular sizes size: " << singularSizes.size() << std::endl;
  
  std::cout << "ANALYZE TILE BEGINNING FOR CIRCLE" << std::endl << std::flush;
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  //std::string fileName = argv[1];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  std::list<CvoronoiCell<numberType> > cellsFull;
  
  std::ifstream myfile(filenameList);
  
  myfile >> winSize;
  getline(myfile, line);
  //winSize = const_winSize;
  
  
  // initialize
  //windowType win( winSize );
  windowType win = getWindow(winSize);
  win.center( origin );
  
  // hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  
  // hypoquasicrystal
  rhombus<numberType> *insc = dynamic_cast<rhombus<numberType>*> ( win.inscribed() );
  
  numberType S = circ->Xwindow().Small();
  numberType L = insc->Xwindow().Large();
  
  numberType coveringR = covering;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  std::list<std::string> inputData; 
  
  // input
  std::cout << std::endl << "Loading data: " << std::endl << std::flush;
  
  inputData.clear();
  
  if (myfile.is_open())
  {
    while ( getline(myfile, line) )
    {
      if ((line.size() > 0) && (line[0] != '#'))
      {
        inputData.push_back(line);
      }
    }
    myfile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 
  
  inputData.sort();
  inputData.unique();
  
  //inputData.clear();
  //std::string cell = "(-1-1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/2,(-1-1*alpha)/2 (1+3*alpha)/2,(1+1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1 (-1-3*alpha)/2,(1+1*alpha)/2 ";
  //std::string cell = "(0-1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(1+3*alpha)/2 (0-1*alpha)/1,(0+1*alpha)/1";
  //std::string cell = "(-1+0*alpha)/1,(0+0*alpha)/1 (-3+1*alpha)/2,(1-1*alpha)/2 (-1+1*alpha)/2,(-3+1*alpha)/2 (-1+1*alpha)/2,(-1+1*alpha)/2 (-3+1*alpha)/2,(-1+1*alpha)/2 ";
  //inputData.push_back(cell);
  
  std::cout << "unique strings read: " << inputData.size() << std::endl << std::flush; 
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
     
    voronoi.setDescription(*it);
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    //*voronoi.CarrierSet = *voronoi.CarrierSet * (numberType::get(1,0)/numberType::get(0,1));
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    cells.push_back(voronoi); 
  }
  
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  
  
  std::list<std::string> failed;
  std::list<numberType> success;
  
  
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::string strokeWidth = const_strokeWidth(winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  std::cout << "Start of the window sections" << std::endl << std::flush;
  
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    numberType max;
    
    for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
    {
      for (std::list<Cpoint<numberType> >::iterator ut = it->CarrierSet->begin(); ut != it->CarrierSet->end(); ++ut)
      {
        if (max < euklid2((origin-*ot).star(), (origin-*ut).star()))
        {
          max = euklid2((origin-*ot).star(), (origin-*ut).star());
        }
      }
    }
    
    max/= numberType::get(4,0);
    
    if (max < numberType::get(1,0))
    {
      continue;
    }
    
    singularSizesSqrt.push_back(max);
  }
  
  singularSizesSqrt.sort();
  singularSizesSqrt.unique();
  
  for (std::list<numberType>::iterator it = singularSizesSqrt.begin(); it != singularSizesSqrt.end(); ++it)
  {
    //print(std::cout, *it);
    //std::cout << std::endl;
    
    numberType error(1,0,1000);
    
    bool check = false;
    for (std::list<numberType>::iterator ot = singularSizes.begin(); ot != singularSizes.end(); ++ot)
    {
      if (*ot*(*ot) == *it)
      {
        check = true;
      }
      
      if ((*it - (*ot*(*ot))).abs() < error)
      {
        check = true;
      }
    }
    if (check)
    {
      continue;
    }
    
    numberType approx;
    
    while (!(((*it-approx*approx).abs() < error) && (*it < approx*approx)))
    {
      if (approx*approx < *it)
      {
        approx+= numberType::get(0,1,2);
      }
      else if (approx*approx > *it)
      {
        approx-= numberType::get(1,0,2);
      }
      
      //std::cout << sqrt(*it) << "\t" << approx  << "\t" << *it-approx*approx << std::endl;
    }
    
    std::cout << std::endl;
    
    std::cout << sqrt(*it) << "\t";
    print(std::cout, approx);
    std::cout << "\t" << *it-approx*approx << std::endl;
    
    singularSizes.push_back(approx);
    singularSizesUsed.push_back(approx);
  }
  
  singularSizes.sort();
  singularSizes.unique();
  singularSizesUsed.sort();
  singularSizesUsed.unique();
  
  singularSizesSqrt.sort();
  singularSizesSqrt.unique();
  
  std::cout << "singular sizes size: " << singularSizes.size() << std::endl;
  
  std::ofstream output(singularFilename);
  
  for (std::list<numberType>::iterator it = singularSizes.begin(); it != singularSizes.end(); ++it)
  {
    print(output, *it);
    output << std::endl;
    
    std::cout << *it << "\t";
    print(std::cout, *it);
    std::cout << std::endl;
  }
  
  output.close();
  
  //for (std::list<numberType>::iterator it = singularSizesUsed.begin(); it != singularSizesUsed.end(); ++it)
  //{
  //  std::cout << *it << "\t";
  //  print(std::cout, *it);
  //  std::cout << std::endl;
  //}
  
  std::cout << std::endl;
  
  output.open(singularSquareFilename);
  
  for (std::list<numberType>::iterator it = singularSizesSqrt.begin(); it != singularSizesSqrt.end(); ++it)
  {
    print(output, *it);
    output << std::endl;
    
    std::cout << sqrt(*it) << "\t";
    print(std::cout, *it);
    std::cout << std::endl;
  }
  
  output.close();
  
  
  delete circ;
  delete insc;
  
  return 0;
}
