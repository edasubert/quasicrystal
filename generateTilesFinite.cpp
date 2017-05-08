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
  ++wordLength;
  //wordLength+= 10;
  
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  
  // file setup
  std::ofstream output(filenameMore.c_str());
  
  print(output, winSize);
  output << std::endl;
  
  output.close();
  
  // input finite
  std::string line;
  
  std::cout << "Loading \"finite\" data" << std::endl;

  std::list<CdeloneSet<numberType> > delonesFinite;
  std::list<CvoronoiCell<numberType> > voronoiFinite;

  std::ifstream myfileFinite(filenameFinite.c_str());

  std::list<std::string> inputData; 

  if (myfileFinite.is_open())
  {
    while ( getline(myfileFinite, line) )
    {
      if ((line.size() > 0) && (line[0] != '#'))
      {
        inputData.push_back(line);
      }
    }
    myfileFinite.close();
  }
  else std::cout << "Unable to open file" << std::endl; 

  std::cout << "strings read: " << inputData.size() << std::endl << std::flush; 
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
    
    delonesFinite.push_back(*(voronoi.CarrierSet));
    voronoiFinite.push_back(voronoi);
  }

  std::cout << "finite cells: " << voronoiFinite.size() << std::endl << std::flush; 
  
  
  // create data -----------------------------------------------------
  std::cout << "Load data" << std::endl;
  
  std::list<std::string> res;
  
  std::list<CdeloneSet<numberType> > potentialSet;
  
  std::list<std::string> lang = language(circ->Xwindow(), wordLength);
  for ( std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it )
  {
    for ( std::list<std::string>::iterator ot = lang.begin(); ot != lang.end(); ++ot )
    {
      potentialSet.push_back(quasicrystal2D(circ->Xwindow(), *it, *ot));
    }
  }
  
  std::cout << "tasks: " << potentialSet.size() << std::endl;
  std::cout << "word length: " << wordLength << std::endl;
  
  // computation

  std::list<CdeloneSet10<numberType> > delones;
  std::list<std::string> cells;
  std::list<std::string> cellsFinite;
  
  
  voronoiFinite.sort();
  voronoiFinite.reverse();
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = voronoiFinite.begin(); it != voronoiFinite.end(); ++it)
  {
    // add finite to result
    cells.push_back(it->save());
    // for checking
    cellsFinite.push_back(it->save());
  }
  
  int count = 0;
  int countPrint = 0;
  int countFinite = 0;
  
  // append finite delones by potential ---------------------------------------------
  for (std::list<CvoronoiCell<numberType> >::iterator it = voronoiFinite.begin(); it != voronoiFinite.end(); ++it)
  {
    std::cout << "loop: " << count << "\t" << "delones: " << delones.size() << '\t' << "cells: " << cells.size() << std::endl << std::flush;
    //std::cout << it->save() << std::endl;
    // merge tasks
    CdeloneSet<numberType> tmp;
    
    for (std::list<CdeloneSet<numberType> >::iterator ot = potentialSet.begin(); ot != potentialSet.end(); ++ot)
    {
      // test whether finite's carrier set is among potential
      CdeloneSet<numberType> testSet;
      testSet << *ot;
      testSet << *(it->CarrierSet);
      
      if (!(testSet.size() > ot->size()))
      {
        tmp << *ot;
      }
    }
    //std::cout << "tmp size: " << tmp.size() << std::endl;
    
    
    CdeloneSet10<numberType> delone;
    
    delone << *(it->CarrierSet);
    delone.addPotential(tmp.getPoints());
    
    delone.setCoveringR(CvoronoiCell<numberType>::large);
    
    delone.sortPotentialByDistance();
    delone.sortByDistance();
    
    //delone.filterDistanceOrigin(numberType::get(2, 0)*coveringR);
    delone.removeOrigin();
    
    delone.filterPotentialByWindow(win);
    
    CvoronoiCell<numberType> voronoi;
      
    *(voronoi.CarrierSet) = delone;
    
    voronoi.CarrierSet->sort();
    voronoi.CarrierSet->unique();
    
    voronoi.CarrierSet->sortByDistance();
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    std::cout << delone.sizePotential() << "\t";
    std::list<Cpoint<numberType> > potential;
    potential = delone.getPotential();
    
    voronoi.filterSetPotential(&potential);
    delone.clearPotential();
    delone.addPotential(potential);
    std::cout << delone.sizePotential() << std::endl;
    
    
    
    std::cout << ++countFinite << "/" << voronoiFinite.size() << " finite cell \t" << "potential: " << delone.sizePotential() << std::endl;
    
    delones.push_front(delone);
    
    
                      //origin.setColor("#000000", "#0000F0", "0.1");
                      //delone.setColor("#000000", "#000000", "0.1");
                      //voronoi.setColor("#4CAF50", "#546E7A", "0.1");
                      //voronoi.CarrierSet->setColor("#F4511E", "#F4511E", "0.16");
                      //it->setColor("#00E676", "#00E676", "0.05");
                      //voronoi.colorify();
                      
                      //std::ostringstream oss;
                      //oss << "output/tile/tile" << std::setfill('0') << std::setw(3) << countFinite << ".svg";// << " " << delone.size();
                      //std::ofstream myfile ( oss.str().c_str() );
                      
                      //myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
                      //myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30/5 << " " << -30/5 << " " << 60/5 << " " << 60/5 << "\">\n" << std::endl;
                      
                      //voronoi.svg(myfile);
                      //voronoi.CarrierSet->svg(myfile);
                      //delone.svg(myfile);
                      //origin.svg(myfile);
                      ////it->svg(myfile);
                      
                      //myfile << "</svg>";
                      
                      //myfile.close();
    //continue;
    for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
    {
      ++countPrint;
      if (countPrint%5000 == 0)
      {
        std::cout << "loop: " << count << "\t" << "delones: " << delones.size() << '\t' << "cells: " << cells.size() << std::endl << std::flush;
      }
      
      CvoronoiCell<numberType> voronoiBase;
        
      *(voronoiBase.CarrierSet) = *it;
      voronoiBase.CarrierSet->sortByDistance();
      voronoiBase.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
      voronoiBase.setCenter(origin);
      voronoiBase.construct();
      voronoiBase.filterSet();
      
      
      
      //std::cout << "SIZE POTENTIAL: " << it->sizePotential() << std::endl;
      
      it->sortPotentialByDistance();
      
      while (it->isPotential()) 
      {
        ++count;
        Cpoint<numberType> cache = it->popPotential();
        
        CdeloneSet10<numberType> delone;
        
        delone = *it;
        delone << cache;
        delone.sortPotentialByDistance();
        delone.filterPotentialByWindow(win);
        
        CvoronoiCell<numberType> voronoi;
        
        *(voronoi.CarrierSet) = delone;
        
        voronoi.CarrierSet->sort();
        voronoi.CarrierSet->unique();
        
        voronoi.CarrierSet->sortByDistance();
        voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
        voronoi.setCenter(origin);
        voronoi.construct();
        voronoi.filterSet();
        
        std::list<std::string>::iterator ot;
        for (ot = cellsFinite.begin(); ot != cellsFinite.end(); ++ot)
        {
          if (voronoi.save().compare(*ot) == 0)
          {
            break;
          }
        }
        if (ot != cellsFinite.end()) // this cell was or will be cut
        {
          //std::cout << voronoi.save() << std::endl << "||" << *ot << "||" << std::endl << voronoiBase.save() << std::endl << std::endl;
          continue;
        }
        
        //std::cout << delone.sizePotential() << "\t";
        std::list<Cpoint<numberType> > potential;
        potential = delone.getPotential();
        
        voronoi.filterSetPotential(&potential);
        delone.clearPotential();
        delone.clear();
        delone << *voronoi.CarrierSet;
        delone.addPotential(potential);
        //delone.sortPotentialByDistance();
        
        //std::cout << delone.sizePotential() << std::endl;
        
        cells.push_back(voronoi.save());
        
        delones.push_back(delone);
      }
      
      delones.erase(it);
      
      if (cells.size() > 1000000)
      {
        cells.sort();
        cells.unique();
        
        std::cout << "cells after unique: " << cells.size() << std::endl << std::flush;
        
        // write to file
        std::ofstream output(filenameMore.c_str(), std::ios::app);
        
        for (std::list<std::string>::iterator it = cells.begin(); it != cells.end(); ++it)
        {
          output << *it << std::endl;
        }
        output.close();
        
        cells.clear();
        cells = cellsFinite;
      }
    }
  }
    if (cells.size() > 0)
    {
      cells.sort();
      cells.unique();
      
      std::cout << "cells after unique: " << cells.size() << std::endl << std::flush;
      
      // write to file
      std::ofstream output(filenameMore.c_str(), std::ios::app);
      
      for (std::list<std::string>::iterator it = cells.begin(); it != cells.end(); ++it)
      {
        output << *it << std::endl;
      }
      output.close();
      
      cells.clear();
    }
  
  
  delete circ;
  
  
  return 0;
}
