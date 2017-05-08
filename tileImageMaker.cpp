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
  
  
  //std::string fileName = argv[1];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  winSize = const_winSize;
  
  
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
  
  // CYAN DROPLET section (triangle tile)
  //std::string cell = "(-1-1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/2,(-1-1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1";
  // GREEN TRIANGLE section (droplet tile)
  //std::string cell = "(-1-3*alpha)/2,(-1-1*alpha)/2 (0-1*alpha)/1,(-1-1*alpha)/1 (0+0*alpha)/1,(-1-1*alpha)/1 (1+1*alpha)/2,(-1-1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1 (0-1*alpha)/1,(0+1*alpha)/1 (-1-3*alpha)/2,(-1+1*alpha)/2 ";
  
  //std::string cell = "(-1+0*alpha)/1,(-1+0*alpha)/1 (-1+1*alpha)/2,(-1-1*alpha)/2 (-1+1*alpha)/1,(0+0*alpha)/1 (-1+1*alpha)/2,(-1+1*alpha)/2 (-1+0*alpha)/1,(0+0*alpha)/1 ";
  //std::string cell = "(-1+1*alpha)/2,(-1-1*alpha)/2 (-1+1*alpha)/2,(-1+1*alpha)/2 (-1+0*alpha)/1,(0+0*alpha)/1 ";
  
  //std::string cell = "(-1-1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/2,(-1-1*alpha)/2 (1+3*alpha)/2,(1+1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1 (-1-3*alpha)/2,(1+1*alpha)/2 ";
  std::string cell = "(2-1*beta)/2,(-1+0*beta)/2 (-1+0*beta)/2,(2-1*beta)/2 (0+0*beta)/1,(-1+0*beta)/1 (1+0*beta)/2,(2-1*beta)/2 (-2+1*beta)/2,(-1+0*beta)/2 (1+0*beta)/1,(0+0*beta)/1 (-2+1*beta)/2,(1+0*beta)/2 (1+0*beta)/2,(-2+1*beta)/2 (0+0*beta)/1,(1+0*beta)/1 (-1+0*beta)/2,(-2+1*beta)/2 (2-1*beta)/2,(1+0*beta)/2 (-1+0*beta)/1,(0+0*beta)/1 ";
  
  CvoronoiCell<numberType> voronoi;
  voronoi.load(cell);
  
  voronoi.setDescription(cell);
  voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
  voronoi.setCenter(origin);
  voronoi.construct();
  voronoi.filterSet();
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = "#000000";
  std::string strokeWidth = const_strokeWidth(numberType::get(1,0,90)*winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = "#000000";
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  voronoi.CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
  voronoi.setColor(fillColor, strokeColor, strokeWidth);
  voronoi.colorify();
  
  std::cout << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
  std::cout << "<svg width=\"" << 5000 << "\" height=\"" << 5000 << "\" viewBox=\"" << -3.1*winSize << " " << -3.1*winSize << " " << 6.2*winSize << " " << 6.2*winSize << "\">" << std::endl;
  std::cout << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
  
  
  windowType intersect = getWindow(winSize);
  intersect.setColor("none", windowstrokeColor, windowstrokeWidth);
  intersect.svg(std::cout);
  
  for (std::list<Cpoint<numberType> >::iterator ot = voronoi.CarrierSet->begin(); ot != voronoi.CarrierSet->end(); ++ot)
  {
    intersect.intersect((origin-*ot).star());
    windowType window = getWindow(winSize);
    window.setColor("none", windowstrokeColor, windowstrokeWidth);
    window.center((origin-*ot).star());
    window.svg(std::cout);
    
    Cpoint<numberType> center = ((origin-*ot).star());
    center.setColor(fillColor, strokeColor, strokeWidth);
    center.svg(std::cout);
  }
  
  intersect.setColor(voronoi.getFillColor(), windowstrokeColor, windowstrokeWidth);
  intersect.svg(std::cout);
  
  std::cout << "<svg x=\"" << static_cast<double>(intersect.centerX())-0.03*winSize << "\" y=\"" << static_cast<double>(intersect.centerY())-0.03*winSize << "\" width=\"" << 0.06*winSize << "\" height=\"" << 0.06*winSize << "\" viewBox=\"" << -2*coveringR << " " << -2*coveringR << " " << 4*coveringR << " " << 4*coveringR << "\">\n" << std::endl;
  voronoi.svg(std::cout);
  voronoi.CarrierSet->svg(std::cout);
  std::cout << "</svg>" << std::endl;
  
  
  std::cout << "</svg>" << std::endl;
  
  
  delete circ;
  delete insc;
  
  return 0;
}
