#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/geometricObject2.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"


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
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  std::cin >> filenamePrefix;
  std::cin >> filenameSuffix;
  
  filenameFinite = filenamePrefix + "finite" + filenameSuffix;
  filenameMore = filenamePrefix + "more" + filenameSuffix;
  
  std::string fileName = argv[1];
  
  // initialize
  numberType winSize = const_winSize;
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  windowType win = getWindow(winSize);
  win.center( origin );
  
  // hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  circ->center(origin);
  
  numberType S = circ->Xwindow().Small();
  
  // size of rhumbus circumscribed to covering radius disc
  //numberType lengthToCover = numberType::get(8, 0)*coveringR;
  numberType lengthToCover = numberType::circumscribedRhombusToCircle()*numberType::get(2, 0)*covering;
  
  numberType coveringR = covering;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*covering;
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  } while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  
  if ((wordLength%2) == 1)
  {
    ++wordLength;
  }
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = "#000000";
  std::string strokeWidth = const_strokeWidth(winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  
  // file setup
  std::ofstream output(filenameMore.c_str());
  
  print(output, winSize);
  output << std::endl;
  
  output.close();
  
  // create data -----------------------------------------------------
  std::cout << "Load data" << std::endl;
  
  std::list<std::string> res;
  std::list<std::string> data;
  
  
  // language to delone set
  std::list<CdeloneSet<numberType> > potentialSet;
  
  std::list<std::string> lang = language(circ->Xwindow(), wordLength);
  for ( std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it )
  {
    for ( std::list<std::string>::iterator ot = lang.begin(); ot != lang.end(); ++ot )
    {
      potentialSet.push_back(quasicrystal2D(circ->Xwindow(), *it, *ot));
    }
  }
  
  int count = 0;
  for ( std::list<CdeloneSet<numberType> >::iterator it = potentialSet.begin(); it != potentialSet.end(); ++it )
  {
    ++count;
    
    
    origin.setColor(fillColor, "#FF0000", strokeWidth);
    it->setColor(fillColor, strokeColor, strokeWidth);
    
    std::ostringstream oss;
    oss << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
    
    myfile << "<g transform=\"scale(17,-17)\">" << std::endl;
    
    myfile << "<circle cx=\"0\" cy=\"0\" r=\"" << 2*coveringR << "\" fill=\"#000000\" fill-opacity=\"" << 80/255.0 << "\" />\n" << std::endl;
    
    myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    myfile << "<polygon points=\"1.91421,1.20711 -1.91421,1.20711 -2.20711,0.50000 0.00000,-1.70711 2.20711,0.50000 \" style=\"fill:#7c69f0;stroke:#000000;stroke-width:0.06\" />" << std::endl;
    myfile << "<circle cx=\"-1.70711\" cy=\"-1.70711\"  r=\"0.3\" stroke=\"#000000\" stroke-width=\"0\" fill=\"#F00000\" />" << std::endl;
    myfile << "<circle cx=\"1.70711\" cy=\"-1.70711\"   r=\"0.3\" stroke=\"#000000\" stroke-width=\"0\" fill=\"#F00000\" />" << std::endl;
    myfile << "<circle cx=\"4.12132\" cy=\"1.70711\"    r=\"0.3\" stroke=\"#000000\" stroke-width=\"0\" fill=\"#F00000\" />" << std::endl;
    myfile << "<circle cx=\"0.00000\" cy=\"2.41421\"    r=\"0.3\" stroke=\"#000000\" stroke-width=\"0\" fill=\"#F00000\" />" << std::endl;
    myfile << "<circle cx=\"-4.12132\" cy=\"1.70711\"   r=\"0.3\" stroke=\"#000000\" stroke-width=\"0\" fill=\"#F00000\" />" << std::endl;
    myfile << "</g>" << std::endl;
    
    it->svg(myfile);
    //origin.svg(myfile);
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
  }
  
}
