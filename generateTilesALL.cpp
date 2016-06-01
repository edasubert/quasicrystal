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
#include <algorithm>

#define _ERROR_

// generates all tiles for "any" window

typedef betaSet numberType;
typedef rhombus windowType;

int main( int argc, char ** argv )
{
  
  std::cout << "GENERAL TILE GENERATOR" << std::endl << std::flush;
  std::cout << "----------------------" << std::endl << std::flush;
  
  int wordLength = 5;
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  std::list<CdeloneSet10<numberType> > delones;
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string folder = argv[1];
  std::string fileName = argv[2];
  
  // window definition
  numberType winSize;
  while (std::cin >> winSize)
  {
    std::string fillColor = "none";
    std::string strokeColor = "rgb(25,25,25)";
    std::ostringstream convert;
    convert << 0.1/0.25;
    std::string strokeWidth = convert.str();
    
    convert.str("");
    convert.clear();
    
    std::string windowfillColor = "none";
    std::string windowstrokeColor = "rgb(25,25,25)";
    convert << 0.001/0.25;
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
    
    CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
    
    std::cout << "hypercrystal: ";
    print( std::cout, circ->Xwindow().l() );
    std::cout << std::endl << std::flush;
    
    std::cout << "hypocrystal:  ";
    print( std::cout, insc->Xwindow().l() );
    std::cout << std::endl << std::flush;
    
    std::cout << "L: " << L << std::endl << std::flush;
    std::cout << "S: " << S << std::endl << std::flush;
    
    std::cout << "covering radius < : " << coveringR << std::endl << std::flush;
    std::cout << "word length: " << wordLength << std::endl << std::flush;
    
    std::list<std::string> lang = language( insc->Xwindow(), circ->Xwindow(), wordLength );
    
    std::cout << "language size: " << lang.size() << std::endl << std::flush;
    
    // construct delone sets with 1 0 marks
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
        delone.filterDistanceOrigin(numberType::get(2, 0)*coveringR);
        delone.sortByDistance();
        //delone.filterByVoronoi();
        //std::cout << delone.size() << std::endl;
        delones.push_back(delone);
      }
      // FILTER DUPLICATES
      delones.sort();
      delones.unique();
      
      std::cout << std::distance(lang.begin(), it)*lang.size() << "/" << lang.size()*lang.size() << std::endl << std::flush;
    }
    std::cout << "Delones first generated: " << delones.size() << std::endl;
    
    
    
    //CREATE SUPERSET (use 1 0 marks) & CREATE VORONOI CELLS
    int cellCount = 0;
     
    for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
    {
      //std::cout << "loop" << std::endl;
      std::cout << cells.size() << " - " << delones.size() << std::endl << std::flush;
      
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
      voronoi.setColor(fillColor, strokeColor, strokeWidth);
      voronoi.CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      voronoi.construct();
      voronoi.filterSet();
      
      cells.push_back( voronoi );
      
      delones.erase(it);
      
      // FILTER DUPLICATES
      cells.sort();
      cells.unique();
      
      delones.sort();
      delones.unique();
    }
    std::cout << "Voronoi cells after duplicate filter: " << cells.size() << std::endl;
  }  
  
  // flipping filter
  std::list<CvoronoiCell<numberType> > cellsFlip = cells;
  cells.clear();
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cellsFlip.begin(); it != cellsFlip.end(); ++it )
  {
    CvoronoiCell<betaSet> voronoi1R = *it;
    CvoronoiCell<betaSet> voronoi2R = *it;
    CvoronoiCell<betaSet> voronoi3R = *it;
    
    voronoi1R.Cell->transform( betaSet::get( -2, 1, 2 ), betaSet::get( -1, 0, 2 ), betaSet::get( -1, 0, 2 ), betaSet::get( 2, -1, 2 ) );
    voronoi1R.CarrierSet->transform( betaSet::get( -2, 1, 2 ), betaSet::get( -1, 0, 2 ), betaSet::get( -1, 0, 2 ), betaSet::get( 2, -1, 2 ) );
    voronoi2R.Cell->transform( betaSet::get( 2, -1, 2 ), betaSet::get( 1, 0, 2 ), betaSet::get( 1, 0, 2 ), betaSet::get( -2, 1, 2 ) );
    voronoi2R.CarrierSet->transform( betaSet::get( 2, -1, 2 ), betaSet::get( 1, 0, 2 ), betaSet::get( 1, 0, 2 ), betaSet::get( -2, 1, 2 ) );
    voronoi3R.Cell->transform( betaSet::get( -1, 0, 1 ), betaSet::get( 0, 0, 1 ), betaSet::get( 0, 0, 1 ), betaSet::get( -1, 0, 1 ) );
    voronoi3R.CarrierSet->transform( betaSet::get( -1, 0, 1 ), betaSet::get( 0, 0, 1 ), betaSet::get( 0, 0, 1 ), betaSet::get( -1, 0, 1 ) );
    
    if ((std::find(cells.begin(), cells.end(), voronoi1R)==cells.end()) && (std::find(cells.begin(), cells.end(), voronoi2R)==cells.end()) && (std::find(cells.begin(), cells.end(), voronoi3R)==cells.end()))
    {
      cells.push_back(*it);
    }
  }
  
  cells.sort();
  cells.reverse();
  
  std::cout << "Voronoi cells after flip filter: " << cells.size() << std::endl;
  
  // OUTPUT
  std::ostringstream tmp;
  tmp << folder << '/' << "words";
  std::ofstream wordfile ( tmp.str().c_str() );
  
  int count = 1;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
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
    
    myfile << "<text x=\"" << -30*8.4 + 10 << "\" y=\"" << -30*8.4 + 90<< "\" style=\"font-size:100;font-family:Latin Modern Roman\">" << std::endl;
    myfile << count << std::endl;
    myfile << "</text>" << std::endl;
    
    myfile << "<g transform=\"scale(17,-17)\">" << std::endl;
    
    //myfile << "<circle cx=\"0\" cy=\"0\" r=\"" << L << "\" fill=\"#555555\" />\n" << std::endl;
    //myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    
    it->svg(myfile);
    it->CarrierSet->svg(myfile);
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    
    ++count;
  }
  
  wordfile.close();
}
