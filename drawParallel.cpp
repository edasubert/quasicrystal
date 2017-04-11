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
  std::cout << "DRAWING TILES AND WINDOW FROM A LIST OF CANDIDATES" << std::endl << std::flush;
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  std::string fileName = argv[2];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile(argv[1]);
  
  myfile >> winSize;
  getline(myfile, line);
  
  
  
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
  
  numberType coveringR = numberType::coveringR()*L;
  
  // size of rhumbus circumscribed to covering radius disc
  numberType lengthToCover = numberType::get(8, 0)*coveringR;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
  
  
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
  
  std::cout << "strings read: " << inputData.size() << std::endl << std::flush; 
  
  inputData.sort();
  inputData.unique();
  
  std::cout << "strings after unique: " << inputData.size() << std::endl << std::flush; 
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
    
    voronoi.setDescription(*it);
    voronoi.CarrierSet->setPackingR();
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    cells.push_back(voronoi);
  }
  
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  cells.unique();
  
  std::cout << "cells after unique: " << cells.size() << std::endl << std::endl << std::flush; 
  
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::string strokeWidth = const_strokeWidth(winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  
  
  std::cout << "Output" << std::endl << std::flush; 
  
  // OUTPUT
  cells.sort();
  cells.reverse();
  
  
  
  std::ostringstream tmp;
  
  int count = 0;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    //for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
    //{
      //std::cout << ((it!=ot) && (windowParts[it->getDescription()] == windowParts[ot->getDescription()])) << std::endl;
    //}
    ++count;
    
    // construct tiles
    it->setColor(fillColor, strokeColor, strokeWidth);
    it->colorify();
    it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    it->Center.setColor(fillColor, strokeColor, strokeWidth);
    
    Cpoint<numberType> middle = it->middle();
    Cpoint<numberType> middleDomain = it->middleDomain();
    
    middle.setColor(fillColor, "#FF1744", strokeWidth);
    middleDomain.setColor(fillColor, "#2979FF", strokeWidth);
    
    std::ostringstream oss;
    oss << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
    
    myfile << "<!--" << std::endl;
    //it->var_dump( myfile );
    myfile << it->Cell->size() << " " << it->size() << " " << atan2(it->middle().getX(), it->middle().getY()) << std::endl;
    myfile << "-->" << std::endl;
    
    //myfile << "<text x=\"" << -30*8.4 + 10 << "\" y=\"" << -30*8.4 + 90<< "\" style=\"font-size:100;font-family:Latin Modern Roman\">" << std::endl;
    //myfile << count << std::endl;
    //myfile << "</text>" << std::endl;
    
    myfile << "<g transform=\"scale(17,-17)\">" << std::endl;
    
    //myfile << "<circle cx=\"0\" cy=\"0\" r=\"" << L << "\" fill=\"#555555\" />\n" << std::endl;
    //myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    
    it->svg(myfile);
    it->CarrierSet->svg(myfile);
    it->Center.svg(myfile);
    middle.svg(myfile);
    middleDomain.svg(myfile);
    
    //it->svg(overlayfile);
    //it->CarrierSet->svg(overlayfile);
    //it->Center.svg(overlayfile);
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
  }
  
  //overlayfile << "</g>" << std::endl;
  //overlayfile << "</svg>";
  
  //overlayfile.close();
  
  
  delete circ;
  delete insc;
  
  
  return 0;
}
