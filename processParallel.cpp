#include "SUPPORT/generate2.h"
#include "SUPPORT/window.h"
#include "SUPPORT/geometricObject2.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/betaSet.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <list>
#include <string>

typedef betaSet numberType;
typedef circle windowType;

int main (int argc, char* argv[])
{ 
  
  std::string folder = argv[2];
  std::string fileName = argv[3];
  
  
  
  
  // initialize
  numberType winSize(-10, 3, 2);
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  windowType win( winSize );
  win.center( origin );
  
  // hyperquasicrystal
  rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
  
  // hypoquasicrystal
  rhombus *insc = dynamic_cast<rhombus*> ( win.inscribed() );
  
  betaSet S = circ->Xwindow().Small();
  betaSet L = insc->Xwindow().Large();
  
  betaSet coveringR = numberType::get(161, -43)*L;
  
  // size of rhumbus circumscribed to covering radius disc
  betaSet lengthToCover = numberType::get(8, 0)*coveringR;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
  
  
  
  std::string fillColor = "#689F38";
  std::string strokeColor = "#263238";
  std::ostringstream convert;
  convert << (0.3)*winSize;
  std::string strokeWidth = convert.str();
  
  
  
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile(argv[1]);
  if (myfile.is_open())
  {
    while ( getline(myfile, line) )
    {
      CvoronoiCell<numberType> voronoi;
      voronoi.load(line);
      cells.push_back(voronoi);
    }
    myfile.close();
  }
  else std::cout << "Unable to open file"; 
  
  
  
  
  
  // OUTPUT
  std::ostringstream tmp;
  tmp << folder << '/' << "words";
  std::ofstream wordfile ( tmp.str().c_str() );
  
  int count = 1;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    // construct tiles
    it->CarrierSet->setPackingR();
    it->CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    it->setCenter(origin);
    it->setColor(fillColor, strokeColor, strokeWidth);
    it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    it->Center.setColor(fillColor, strokeColor, strokeWidth);
    it->construct();
    
    
    
    wordfile << it->CarrierSet->getDescription() << std::endl;
    
    std::ostringstream oss;
    oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"1000\" height=\"1000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
    
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
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    
    ++count;
  }
  
  return 0;
}
