#define _ERROR_

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

// creates diagram of "any" window

int main( int argc, char ** argv )
{
  std::cout << "GENERAL QUASICRYSTAL GENERATOR" << std::endl << std::flush;
  std::cout << "------------------------------" << std::endl << std::flush;
  
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
  
  // intervel of the quasicrystal
  numberType x = numberType::get(210,0,1)*numberType::get(3,0,8);
  numberType y = numberType::get(297,0,1)*numberType::get(3,0,8);
  
  numberType x1 = numberType::get(-3,0,4)*x;
  numberType x2 = numberType::get( 3,0,4)*x;
  numberType y1 = numberType::get(-3,0,4)*y;
  numberType y2 = numberType::get( 3,0,4)*y;
  
  
  // window definition
  numberType winSize = const_winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  Cpoint<numberType> translation(numberType::get(500,0), numberType::get(0,0));
  
  windowType win = getWindow(winSize);
  win.center(origin);
  //win.center(translation);
  
  
  // hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  circ->center(origin);
  
  numberType S = circ->Xwindow().Small();
  
  // size of rhumbus circumscribed to covering radius disc
  //numberType lengthToCover = numberType::get(8, 0)*coveringR;
  numberType lengthToCover = numberType::circumscribedRhombusToCircle()*numberType::get(2, 0)*covering;
  
  numberType coveringR = covering;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*covering;
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  
  // file setup
  std::ifstream inputfile;
  
  if (win.getName() == "circle")
  {
    inputfile.open(filenameFinite.c_str());
  }
  else
  {
    inputfile.open(filenameList.c_str());
  }
  // input finite
  std::string line;
  
  std::list<std::string> inputData; 

  if (inputfile.is_open())
  {
    while ( getline(inputfile, line) )
    {
      if ((line.size() > 0) && (line[0] != '#'))
      {
        inputData.push_back(line);
      }
    }
    inputfile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 

  std::cout << "strings read: " << inputData.size() << std::endl << std::flush; 
  
  std::list<CvoronoiCell<numberType> > cells;
  std::map<std::string, windowType> windowParts;
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
    
    voronoi.setDescription(voronoi.save());
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    // CUT WINDOW SECTIONS -----------------------------------------------
    voronoi.CarrierSet->sortClockwise();
    windowType intersect = win;
    for (std::list<Cpoint<numberType> >::iterator ot = voronoi.CarrierSet->begin(); ot != voronoi.CarrierSet->end(); ++ot)
    {
      intersect.intersect((origin-*ot).star());
    }
    windowParts[voronoi.getDescription()] = intersect;
    
    cells.push_back(voronoi);
  }
  
  cells.sort();
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  
  
  std::cout << "window name: " << win.getName() << std::endl;
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  std::cout << "Onto generation ..." << std::endl << std::flush;
  
  CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
  delone.setDescription("Deloneovská množina bodů");
  //*voronoi->CarrierSet = delone;
  std::cout << "quasicrystal size: " << delone.size() << std::endl << std::flush;
  
  CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
  
  CvoronoiCell<numberType>::large = numberType::get(5,0)*coveringR;
  
  delone.setCoveringR(CvoronoiCell<numberType>::large);
  
  std::ostringstream cache;
  
  std::string fileName = argv[1];
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::ostringstream convert;
  convert << 0.06*sqrt((x2-x1)*(y2-y1)/numberType::get(delone.size(),0,1));
  std::string strokeWidth = convert.str();
  
  delone.setColor( fillColor, strokeColor, strokeWidth );
  hyper.setColor( fillColor, "#2196F3", strokeWidth );
  //hypo.setColor( fillColor, "#D50000", strokeWidth );
  
  
  for ( std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it )
  {
    if ((it->getX() < numberType::get(-6,0,10)*x) || (it->getX() > numberType::get(6,0,10)*x) || (it->getY() < numberType::get(-6,0,10)*y) || (it->getY() > numberType::get(6,0,10)*y))
    {
      continue;
    }
    CvoronoiCell<numberType> voronoi;
    
    for (std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot)
    {
      if (windowParts[ot->getDescription()].in(it->star()))
      {
        voronoi = *ot;
        break;
      }
    }
    
    voronoi.setColor( fillColor, strokeColor, strokeWidth );
    voronoi.colorify();
    *voronoi.Cell+= *it;
    
    voronoi.svg(cache);
  }
  
  std::ostringstream oss;
  oss << fileName << "_" << win.getName() << "_" << std::floor(100000.0*winSize) << "_(";
  printFile(oss, winSize);
  oss << ").svg";
      
  std::ofstream myfile ( oss.str().c_str() );
  
  myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  myfile << "<svg width=\"" << 1920 << "\" height=\"" << 1920*y/x << "\" viewBox=\"" << -x/2 << " " << -y/2 << " " << x << " " << y << "\">\n" << std::endl;
  //myfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" style=\"stroke:" << strokeColor << ";stroke-width:" << y/200 << ";fill:none;\" />" << std::endl;
  myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  
  myfile << cache.str().c_str() << std::endl;
  
  myfile << "<!-- hypercrystal -->" << std::endl;
  //hyper.svg(myfile);
  //hyper.star().svg(myfile);
  myfile << std::endl;
  
  myfile << "<!-- quasicrystal -->" << std::endl;
  delone.svg(myfile);
  //delone.star().svg(myfile);
  myfile << std::endl;
  
  myfile << "<!-- hypocrystal -->" << std::endl;
  //hypo.svg(myfile);
  myfile << std::endl;
  
  
  win.setColor( "none", "#000000", "0.006" );
  circ->setColor( "none", "#000000", "0.006" );
  
  //win.svg(myfile);
  //circ->svg(myfile);
  
  myfile << "</g>" << std::endl;
  myfile << "</svg>";
  
  myfile.close();
  
  
  delete circ;
  
  return 0;
}
