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
#include <chrono>

#define _ERROR_

// generates all tiles for "any" window

typedef betaSet numberType;
typedef rhombus windowType;

int main( int argc, char ** argv )
{
  // time measuring
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  
  
  // window definition
  numberType winSize(-10, 3, 2);
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  
  std::ostringstream convert;
  convert << 0.1/winSize;
  std::string strokeWidth = convert.str();
  
  convert.str("");
  convert.clear();
  
  std::string windowfillColor = "none";
  std::string windowstrokeColor = "rgb(25,25,25)";
  convert << 0.001/winSize;
  std::string windowstrokeWidth = convert.str();
  
  //rhombus win( winSize, winSize );
  //rhombus win( winSize );
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
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  }
  while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  
  std::list<std::string> lang = language( insc->Xwindow(), circ->Xwindow(), wordLength );
  
  // construct delone sets with 1 0 marks
  std::list<CdeloneSet10<numberType> > delones;
  
  for ( std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it )
  {
    for ( std::list<std::string>::iterator ot = lang.begin(); ot != lang.end(); ++ot )
    {
      //std::cout << *it << std::endl << std::flush;
      //std::cout << *ot << std::endl << std::endl << std::flush;
      CdeloneSet10<numberType> delone = quasicrystal2D10(circ->Xwindow(), *it, *ot);
      
      delone.setPackingR();
      delone.setCoveringR(numberType::get(2, 0)*coveringR);
      delone.setDescription(*it+' '+*ot);
      
      //std::cout << delone.size() << " ";
      //delone.filterDistanceOrigin(numberType::get(2, 0)*coveringR);
      delone.sortByDistance();
      //delone.filterByVoronoi();
      //std::cout << delone.size() << std::endl;
      delones.push_back(delone);
      //delone.svg(std::cout);
    }
    // FILTER DUPLICATES
    delones.sort();
    delones.unique();
  }
  
  
  
  //CREATE SUPERSET (use 1 0 marks) & CREATE VORONOI CELLS
   
  //std::list<CvoronoiCell<numberType> > cells;
  std::list<std::string> cells;
  int cellCount = 0;
   
  for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
  {
    it->filterPotentialByWindow(win);
    
    while (it->isPotential()) // there is an element with '0' => split into two and append
    {
      //std::cout << "there is potential!" << std::endl << std::flush;
      
      Cpoint<numberType> cache = it->popPotential();
      
      CdeloneSet10<numberType> delone = *it;
      
      delone << cache;
      //std::cout << "\t WE HAVE FOUND IT" << std::endl << std::flush;
      //std::cout << "\t \t " << delone.size() << " - " << delone.sizePotential() << std::endl << std::flush;
      //std::cout << "\t add with 1" << std::endl << std::flush;
      //std::cout << delone.size() << " - " << delone.sizePotential() << " ";
      //delone.filterByVoronoi();
      //std::cout << delone.size() << " - " << delone.sizePotential() << std::endl;
      delones.push_back(delone);
    }
    
    CvoronoiCell<numberType> voronoi;
    
    *(voronoi.CarrierSet) = *it;
    voronoi.CarrierSet->sortByDistance();
    voronoi.CarrierSet->setPackingR();
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    
    std::ostringstream toString;
    toString << voronoi.save();
    
    cells.push_back( toString.str() );
    
    delones.erase(it);
    
    // FILTER DUPLICATES
    cells.sort();
    cells.unique();
    
    delones.sort();
    delones.unique();
  }
  
  std::ofstream output(argv[1]);
  
  for ( std::list<std::string>::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    output << *it << std::endl;
  }
  output.close();
  
  // time measuring
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  std::cout << "microseconds: " << duration << " (" << std::round(duration/100000.0)/10.0 << "s)" << std::endl;
  
  return 0;
}
