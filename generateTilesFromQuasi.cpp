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
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>



// creates diagram of "any" window from finite section
int main( int argc, char ** argv )
{
  // INPUT
  numberType const_winSize;
  numberType covering;
  numberType largestTile;
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  
  
  std::string fileName = argv[1];
  fileName+= "/";
  fileName+= argv[2];
  
  std::string fileNameProp = argv[3];
  
  std::cout << "LIMITED WINDOW DIVISION GENERATOR" << std::endl << std::flush;
  std::cout << "---------------------------------" << std::endl << std::flush;
  
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  // window definition
  numberType winSize = const_winSize;
  //while (std::cin >> winSize)
  
  // interval of the quasicrystal
  //numberType x = numberType::get(60,0)*(numberType::get(22,0,4)-winSize); //polygon
  numberType x = numberType::get(20,0)*(numberType::get(22,0,4)-winSize); //circle8
  
  //rhombus win( winSize, winSize );
  //windowType win( winSize );
  windowType win = getWindow(winSize);
  win.center(origin);
  
  // control with hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> (win.circumscribed());
  
  // control with hypoquasicrystal
  rhombus<numberType> *insc = dynamic_cast<rhombus<numberType>*> (win.inscribed());
  
  
  numberType x1 = -x;
  numberType x2 = x;
  numberType y1 = -x;
  numberType y2 = x;
  
  numberType coveringR = numberType::coveringR()*insc->large();
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  std::cout << "x: " << x << std::endl << std::flush;
  std::cout << "Onto generation ..." << std::endl << std::flush;
  
  CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
  delone.setDescription("Deloneovská množina bodů");
  //*voronoi->CarrierSet = delone;
  
  CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
  
  std::cout << "window: ";
  print( std::cout, winSize );
  std::cout << std::endl << std::flush;
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CvoronoiCell<numberType>::large = numberType::get(2,0)*coveringR;
  
  delone.setPackingR();
  delone.setCoveringR(CvoronoiCell<numberType>::large);
  
  std::cout << "points of quasicrystal (including edge): " << delone.size() << std::endl << std::flush;
  
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
    
    *voronoi.CarrierSet = delone;
    *voronoi.CarrierSet-= *it;
    voronoi.CarrierSet->filterDistanceOrigin(numberType::get(2, 0)*coveringR);
    
    voronoi.construct();
    voronoi.filterSet();
    
    
    //convert.str("");
    //convert.clear();
    //
    //print(convert, it->getX());
    //print(convert, it->getY());
    //voronoi.setDescription(convert.str());
    
    cells.push_back(voronoi);
  }
  cells.sort();
  cells.unique();
  
  std::cout << "found unique cells: " << cells.size() << std::endl;
  
  
  std::ostringstream tmp02;
  tmp02 << fileName << "_" << win.getName() << "_" << winSize << "_(";
  printFile(tmp02, winSize);
  tmp02 << ")";
  
  // write to file
  std::ofstream output(tmp02.str().c_str());
  cells.sort();
  cells.unique();
  
  print(output, winSize);
  output << std::endl;
  
  numberType radius;
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    // get radius
    radius = max(it->radius(), radius);
    
    
    
    output << it->save() << std::endl;
  }
  output.close();
  
  
  // write to file properties
  std::ostringstream tmp03;
  tmp03 << fileNameProp << "_" << win.getName() << "_" << winSize << "_(";
  printFile(tmp03, winSize);
  tmp03 << ")";
  
  output.open(tmp03.str().c_str());
  
  print(output, winSize);
  output << std::endl;
  print(output, radius);
  output << std::endl;
  print(output, cells.rbegin()->size());
  output << std::endl;
  
  output << argv[1] << "/" << std::endl;
  
  output << "_" << win.getName() << "_" << winSize << "_(";
  printFile(output, winSize);
  output << ")" << std::endl;
  
  output.close();
  
  
  
  std::cout << "Covering radius estimate: ";
  print(std::cout, radius);
  std::cout << std::endl;
  
  std::cout << "largest tile size: ";
  print(std::cout, cells.rbegin()->size());
  std::cout << std::endl;
  
  
  delete circ;
  delete insc;
  
  
  return 0;
}
