#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>



// creates diagram of "any" window from finite section

typedef alphaSet numberType;
typedef circle<numberType> windowType;

int main( int argc, char ** argv )
{
  std::cout << "LIMITED WINDOW DIVISION GENERATOR" << std::endl << std::flush;
  std::cout << "---------------------------------" << std::endl << std::flush;
  
  // interval of the quasicrystal
  numberType x(20,0);
    
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  // window definition
  numberType winSize;
  //while (std::cin >> winSize)
  
  winSize = numberType::get(1,0);
  
  std::string folder = argv[1];
  std::string fileName = argv[2];
  std::string fillColor = "none";
  std::string strokeColor = "rgb(25,25,25)";
  std::ostringstream convert;
  convert << 0.24/winSize;
  std::string strokeWidth = convert.str();
  
  convert.str("");
  convert.clear();
  
  std::string windowfillColor = "#00BCD4";
  std::string windowstrokeColor = "#37474F";
  convert << 0.003*winSize;
  std::string windowstrokeWidth = convert.str();
  
  convert.str("");
  convert.clear();
  
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  
  //rhombus win( winSize, winSize );
  windowType win( winSize );
  win.center( origin );
  
  // control with hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  
  // control with hypoquasicrystal
  rhombus<numberType> *insc = dynamic_cast<rhombus<numberType>*> ( win.inscribed() );
  
  
  numberType x1 = -x;//numberType::get(-3, 0)*insc->large();
  numberType x2 = x;
  numberType y1 = -x;//numberType::get(-3, 0)*insc->large();
  numberType y2 = x;
  
  
  numberType coveringR = numberType::coveringR()*insc->large();
  
  std::cout << "x: " << x << std::endl << std::flush;
  std::cout << "Onto generation ..." << std::endl << std::flush;
  
  CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
  delone.setDescription("Deloneovská množina bodů");
  delone.setColor( fillColor, strokeColor, strokeWidth );
  //*voronoi->CarrierSet = delone;
  
  CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
  hyper.setColor( fillColor, "#2196F3", strokeWidth );
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CdeloneSet<numberType> hypo = quasicrystal2D( *insc, x1, x2, y1, y2 );
  hypo.setColor( fillColor, "#D50000", strokeWidth );
  
  std::cout << "hypocrystal:  ";
  print( std::cout, insc->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CvoronoiCell<numberType>::large = numberType::get(3,0)*coveringR;
  
  delone.setPackingR();
  delone.setCoveringR(CvoronoiCell<numberType>::large);
  
  std::cout << "L: " << insc->large() << std::endl;
  
  //std::cout << "points of quasicrystal: " << voronoi->CarrierSet->size() << std::endl << std::flush;
  
  std::list<CvoronoiCell<numberType> > cells;
  
  
  // generate tiles
  for ( std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it )
  {
    // skip edge
    if ((it->getX() < x1+insc->large()*numberType::get(2,0)) || (it->getX() > x2-insc->large()*numberType::get(2,0)) || (it->getY() < y1+insc->large()*numberType::get(2,0)) || (it->getY() > y2-insc->large()*numberType::get(2,0)))
    {
      continue;
    }
    
    CvoronoiCell<numberType> voronoi;
    
    voronoi.setCenter(origin);
    voronoi.setColor( fillColor, strokeColor, strokeWidth );
    
    *voronoi.CarrierSet = delone;
    *voronoi.CarrierSet-= *it;
    voronoi.CarrierSet->filterDistanceOrigin(numberType::get(2, 0)*coveringR);
    
    voronoi.construct();
    voronoi.filterSet();
    
    
    convert.str("");
    convert.clear();
    
    print(convert, it->getX());
    print(convert, it->getY());
    voronoi.setDescription(convert.str());
    
    cells.push_back(voronoi);
    
    cells.sort();
    cells.unique();
  }
  
  std::cout << "found unique cells: " << cells.size() << std::endl;
  
  
  
  std::ostringstream tmp02;
  tmp02 << folder << '/' << fileName << "_";
  printFile(tmp02, winSize);
  tmp02 << "-" << floor(x);
  
  // write to file
  std::ofstream output(tmp02.str().c_str());
  cells.sort();
  cells.unique();
  
  print(output, winSize);
  output << std::endl;
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    output << it->save() << std::endl;
  }
  output.close();
}
