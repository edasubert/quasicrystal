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
#include <algorithm>
#include <map>

// generates all tiles for "any" window

typedef alphaSet numberType;
typedef circle<numberType> windowType;

int main( int argc, char ** argv )
{
  
  std::cout << "GENERAL TILE GENERATOR" << std::endl << std::flush;
  std::cout << "----------------------" << std::endl << std::flush;
  
  
  // window definition
  numberType winSize;
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  int winId = 1;
  //while (std::cin >> winSize)
  {
    winSize = numberType::get(1,0,1)/numberType::get(0,1,1);
    winSize = numberType::get(3, -1, 1);
    
    
    std::string folder = argv[1];
    std::string fileName = argv[2];
    std::string fillColor = "none";
    std::string strokeColor = "rgb(25,25,25)";
    std::ostringstream convert;
    convert << 0.24/winSize;
    std::string strokeWidth = convert.str();
    
    convert.str("");
    convert.clear();
    
    std::string windowfillColor = "none";
    std::string windowstrokeColor = "rgb(25,25,25)";
    convert << 0.001*winSize;
    std::string windowstrokeWidth = convert.str();
    
    convert.str("");
    convert.clear();
    
    convert << 0.6/winSize;
    std::string borderstrokeWidth = convert.str();
    
    //rhombus win( winSize, winSize );
    //rhombus win( winSize );
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
    
    //int wordLength = lengthToCover/S +1;
    
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
     
    std::list<CvoronoiCell<numberType> > cells;
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
      voronoi.setDescription(it->getDescription());
      
      cells.push_back( voronoi );
      
      delones.erase(it);
      
      // FILTER DUPLICATES
      cells.sort();
      cells.unique();
      
      delones.sort();
      delones.unique();
    }
    
    // CUT WINDOW SECTIONS
    std::map<std::string, windowType> windowParts;
    
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      windowType intersect = win;
      for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
      {
        windowType moving = win;
        moving.center(origin-ot->star());
        intersect.intersect(&moving);
      }
      windowParts[it->getDescription()] = intersect;
    }
    
    // deal with overlap
    cells.sort();
    std::map<std::string, std::list<windowType> > toCut;
    
    bool check;
    for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
    {
      for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
      {
        if (it == ot) continue;
        
        windowType intersection = windowParts[it->getDescription()];
        intersection.intersect(&windowParts[ot->getDescription()]);
        if (!intersection.empty())
        {
          if (it->size() > ot->size())
          {
            toCut[it->getDescription()].push_back(intersection);//windowParts[ot->getDescription()]);
            //check+= diff(windowParts[it->getDescription()], intersection);
          }
          else
          {
            toCut[ot->getDescription()].push_back(intersection);//windowParts[it->getDescription()]);
            //check+= diff(windowParts[ot->getDescription()], intersection);
          }
        }
      }
    }
    
    std::cout << windowParts.size() << std::endl;
    
    int count = 1;
    for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
    {
      //std::ostringstream oss;
      //oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count++ << ".svg";// << " " << it->size();
      //std::ofstream myfile ( oss.str().c_str() );
      
      //myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
      //myfile << "<svg width=\"2800\" height=\"840\" viewBox=\"" << -1*winSize << " " << -0.3*winSize << " " << 2*winSize << " " << 0.6*winSize << "\">\n" << std::endl;
      //myfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
      //myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
      
      for ( std::list<windowType>::iterator it = toCut[ot->getDescription()].begin(); it != toCut[ot->getDescription()].end(); ++it )
      {
        //it->setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
        //it->svg(myfile);
        windowType intersection = *it;
        intersection.intersect(&windowParts[ot->getDescription()]);
        //intersection.setColor("#FF5722", windowstrokeColor, windowstrokeWidth);
        //intersection.svg(myfile);
        diff(windowParts[ot->getDescription()], intersection);
      }
      //windowParts[ot->getDescription()].setColor("#03A9F4", windowstrokeColor, windowstrokeWidth);
      //windowParts[ot->getDescription()].svg(myfile);
      
      //myfile << "</g>" << std::endl;
      //myfile << "</svg>" << std::endl;
      //myfile.close();
    }
    
    
    std::cout << "Voronoi cells after duplicate filter: " << cells.size() << std::endl;
    
    std::ostringstream tmp02;
    tmp02 << folder << '/' << "window_" << winId++ << "_";
    printFile(tmp02, winSize);
    tmp02 << ".svg";
    std::ofstream windowfile ( tmp02.str().c_str() );
    
    windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    windowfile << "<svg width=\"" << 4000 << "\" height=\"" << 4000 << "\" viewBox=\"" << -1*winSize << " " << -1*winSize << " " << 2*winSize << " " << 2*winSize << "\">\n" << std::endl;
    windowfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
    windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
    win.svg(windowfile);
    
    // OUTPUT
    count = 1;
    {
      for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
      { 
        // window parts
        windowType intersect = windowParts[it->getDescription()];
        windowfile << "<!--";
        windowfile << it->CarrierSet->getDescription();
        windowfile << "-->" << std::endl;
        intersect.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
        //intersect.flip(2);
        intersect.svg(windowfile);
        
        // tiles
        windowfile << "<svg x=\"" << static_cast<double>(intersect.centerX())-0.03*winSize << "\" y=\"" << static_cast<double>(intersect.centerY())-0.03*winSize << "\" width=\"" << 0.06*winSize << "\" height=\"" << 0.06*winSize << "\" viewBox=\"" << -2*coveringR << " " << -2*coveringR << " " << 4*coveringR << " " << 4*coveringR << "\">\n" << std::endl;
        
        
        it->setColor(fillColor, "#ffffff", borderstrokeWidth);
        it->CarrierSet->setColor(fillColor, "#ffffff", borderstrokeWidth);
        it->svg(windowfile);
        it->CarrierSet->svg(windowfile);
        
        it->setColor(fillColor, strokeColor, strokeWidth);
        it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
        it->svg(windowfile);
        it->CarrierSet->svg(windowfile);
        
        windowfile << "</svg>" << std::endl;
      }
    }
    windowfile << "</g>" << std::endl;
    windowfile << "</svg>" << std::endl;
    windowfile.close();
  }
}
