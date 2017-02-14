#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>


// generates all tiles for "any" window

typedef alphaSet numberType;
typedef rhombus<numberType> windowType;

int main( int argc, char ** argv )
{
  
  std::cout << "GENERAL TILE GENERATOR" << std::endl << std::flush;
  std::cout << "----------------------" << std::endl << std::flush;
  
  
  // window definition
  numberType winSize(1, 0, 4);
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  
  std::string folder = argv[1];
  std::string fileName = argv[2];
  std::string fillColor = "none";
  std::string strokeColor = "rgb(25,25,25)";
  std::ostringstream convert;
  convert << 0.1/winSize;
  std::string strokeWidth = convert.str();
  
  convert.str("");
  convert.clear();
  
  std::string windowfillColor = "none";
  std::string windowstrokeColor = "rgb(25,25,25)";
  convert << 0.001/winSize;
  std::string windowstrokeWidth = convert.str();
  
  windowType win( winSize );
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
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  }
  while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  
  //wordLength = 10;
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  std::cout << "hypocrystal:  ";
  print( std::cout, insc->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  std::cout << "L: " << L << std::endl << std::flush;
  std::cout << "S: " << S << std::endl << std::flush;
  
  std::cout << "covering radius < : " << coveringR << std::endl << std::flush;
  std::cout << "length to cover: " << lengthToCover << std::endl << std::flush;
  std::cout << "word length: " << wordLength << std::endl << std::flush;
  std::cout << "minimal sequence length: " << minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) << std::endl << std::flush;
  
  std::list<std::string> lang = language( insc->Xwindow(), circ->Xwindow(), wordLength );
  
  std::cout << "language size: " << lang.size() << std::endl << std::flush;
  
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
    
    std::cout << std::distance(lang.begin(), it)*lang.size() << "/" << lang.size()*lang.size() << std::endl << std::flush;
  }
  std::cout << "Delones first generated: " << delones.size() << std::endl;
  
  
  
  //CREATE SUPERSET (use 1 0 marks) & CREATE VORONOI CELLS
   
  std::list<CvoronoiCell<numberType> > cells;
  int cellCount = 0;
   
  for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
  {
    //std::cout << "loop" << std::endl;
    std::cout << cells.size() << " - " << delones.size() << std::endl << std::flush;
    
    //it->filterPotentialByWindow(win);
    
    while (it->isPotential()) // there is an element with '0' => split into two and append
    {
      std::cout << "there is potential! " << it->sizePotential() << std::endl << std::flush;
      
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
    
    if (cellCount < cells.size())
    {
      std::ostringstream tmp;
      tmp << folder << '/' << "words";
      std::ofstream wordfile ( tmp.str().c_str() );
      
      std::ostringstream tmp02;
      tmp02 << folder << '/' << "window";
      std::ofstream windowfile ( tmp02.str().c_str() );
      
      windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
      windowfile << "<svg width=\"2000\" height=\"2000\" viewBox=\"" << -1*winSize << " " << -1*winSize << " " << 2*winSize << " " << 2*winSize << "\">\n" << std::endl;
      windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
      win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
      win.svg(windowfile);
      
      cellCount = cells.size();
      // OUTPUT
      int count = 1;
      for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
      {
        wordfile << it->CarrierSet->getDescription() << std::endl;
        
        std::ostringstream oss;
        oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
        std::ofstream myfile ( oss.str().c_str() );
        
        oss.str(""); oss.clear();
        oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count << "_window.svg";
        std::ofstream tileWindowfile ( oss.str().c_str() );
        
        myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
        myfile << "<svg width=\"1000\" height=\"1000\" viewBox=\"" << -30*coveringR << " " << -30*coveringR << " " << 60*coveringR << " " << 60*coveringR << "\">\n" << std::endl;
        
        myfile << "<!--" << std::endl;
        //it->var_dump( myfile );
        myfile << it->Cell->size() << " " << it->size() << " " << atan2(it->middle().getX(), it->middle().getY()) << std::endl;
        myfile << "-->" << std::endl;
        
        myfile << "<g transform=\"scale(17,-17)\">" << std::endl;
        
        //myfile << "<circle cx=\"0\" cy=\"0\" r=\"" << L << "\" fill=\"#555555\" />\n" << std::endl;
        //myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
        
        it->svg(myfile);
        it->CarrierSet->svg(myfile);
        Cpoint<numberType> middle = it->middle();
        middle.setColor("#64DD17", "#64DD17", strokeWidth);
        middle.svg(myfile);
        
        Cpoint<numberType> middleDomain = it->middleDomain();
        middleDomain.setColor("#00C853", "#00C853", strokeWidth);
        middleDomain.svg(myfile);
        
        myfile << "</g>" << std::endl;
        myfile << "</svg>";
        
        myfile.close();
        
        // window parts
        windowType intersect = win;
        for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
        {
          windowType moving = win;
          moving.center(ot->star());
          intersect.intersect(&moving);
        }
        windowfile << "<!--";
        windowfile << it->CarrierSet->getDescription();
        windowfile << "-->" << std::endl;
        intersect.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
        //intersect.flip(2);
        intersect.svg(windowfile);
        
        
        
        tileWindowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
        tileWindowfile << "<svg width=\"2000\" height=\"2000\" viewBox=\"" << -1*winSize << " " << -1*winSize << " " << 2*winSize << " " << 2*winSize << "\">\n" << std::endl;
        tileWindowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
        win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
        win.svg(tileWindowfile);
        intersect.svg(tileWindowfile);
        //it->CarrierSet->star().svg(tileWindowfile);
        tileWindowfile << "</g>" << std::endl;
        tileWindowfile << "</svg>";
        tileWindowfile.close();
        
        ++count;
      }
      
      wordfile.close();
      
      windowfile << "</g>" << std::endl;
      windowfile << "</svg>";
      windowfile.close();
    }
  }
  std::cout << "Voronoi cells after duplicate filter: " << cells.size() << std::endl;
}
