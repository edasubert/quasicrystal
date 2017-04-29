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

struct bookmark
{
	windowType intersection;
  std::list<windowType>::iterator last;
};

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
  
  
  std::cout << "DRAWING TILES AND WINDOW FROM A LIST OF CANDIDATES" << std::endl << std::flush;
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  std::string fileName = argv[1];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile(filenameMore);
  
  myfile >> winSize;
  getline(myfile, line);
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
  
  std::cout << "window size: ";
  print(std::cout, winSize);
  std::cout << std::endl;
  
  
  // input finite
  std::cout << "Loading \"finite\" data" << std::endl;
  
  std::list<CvoronoiCell<numberType> > cellsFinite;
  
  std::ifstream myfileFinite(filenameFinite);
  
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
    
    voronoi.setDescription(*it);
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    cellsFinite.push_back(voronoi);
  }
  
  std::cout << "finite cells: " << cellsFinite.size() << std::endl << std::flush; 
  
  cellsFinite.sort();
  
  // select largest size from finite
  numberType largest = cellsFinite.rbegin()->size();
  std::cout << "largest: " << largest << " ";
  print(std::cout, largest);
  std::cout << std::endl;
  
  
  // input
  std::cout << std::endl << "Loading data: " << std::endl << std::flush;
  
  inputData.clear();
  
  if (myfile.is_open())
  {
    while ( getline(myfile, line) )
    {
      if ((line.size() > 0) && (line[0] != '#'))
      {
        inputData.push_back(line);
      }
    }
    myfile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 
  
  inputData.sort();
  inputData.unique();
  
  std::cout << "unique strings read: " << inputData.size() << std::endl << std::flush; 
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
    
    voronoi.setDescription(*it);
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    if (voronoi.size() <= largest)
    {
      // CUT WINDOW SECTIONS -----------------------------------------------
      voronoi.CarrierSet->sortClockwise();
      windowType intersect = win;
      for (std::list<Cpoint<numberType> >::iterator ot = voronoi.CarrierSet->begin(); ot != voronoi.CarrierSet->end(); ++ot)
      {
        intersect.intersect((origin-*ot).star());
      }
      if (!intersect.empty())
      {
        cells.push_back(voronoi);
      }
      //else
      //{
        //std::cout << "EMPTY" << std::endl;
      //}
    }
  }
  
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::string strokeWidth = const_strokeWidth(winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  
  // JOIN CELLS AND CELLS FINITE
  //std::cout << "Join finite" << std::endl << std::flush;
  //cells.insert(cells.begin(), cellsFinite.begin(), cellsFinite.end());
  //
  //std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  //cells.sort();
  //cells.unique();
  //std::cout << "cells after unique: " << cells.size() << std::endl << std::endl << std::flush; 
  
  
  long int count;
  
  std::cout << "Cutting window sections" << std::endl << std::flush;
  // CUT WINDOW SECTIONS -----------------------------------------------
  std::map<std::string, windowType> windowParts;
  
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); )
  {
    it->CarrierSet->sortClockwise();
    windowType intersect = win;
    for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
    {
      intersect.intersect((origin-*ot).star());
    }
    if (!intersect.empty())
    {
      windowParts[it->getDescription()] = intersect;
      ++it;
      //std::cout << "not empty" << std::endl << std::flush;
    }
    else
    {
      it = cells.erase(it); 
      //++it;
      //std::cout << "WHAT IS GOING ON?!?!?!" << std::endl << std::flush;
    }
  }
  
  std::cout << "cells size: " << cells.size() << std::endl << std::endl << std::flush;
  
  //std::cout << "filter by unionizing until full window" << std::endl << std::flush;
  //cells.sort();
  //std::list<CvoronoiCell<numberType> > selection;
  //std::list<windowType> selectionWindow;
  //windowType fullWindow = windowParts[cells.begin()->getDescription()];
  //selection.push_back(*cells.begin());
  //selectionWindow.push_back(windowParts[cells.begin()->getDescription()]);
  
  //for (std::list<CvoronoiCell<numberType> >::iterator it = ++cells.begin(); it != cells.end(); ++it)
  //{
    //windowType intersect = fullWindow;
    //intersect.intersect(&windowParts[it->getDescription()]);
    
    //if (intersect.size() == windowParts[it->getDescription()].size())
    //{
      //std::cout << "continue" << std::endl;
      //continue;
    //}
    
    //selection.push_front(*it);
    //selectionWindow.push_back(windowParts[it->getDescription()]);
    
    //selectionWindow.sort();
    //selectionWindow.reverse();
    
    //for (std::list<windowType>::iterator ot = selectionWindow.begin(); ot != selectionWindow.end();)
    //{
      ////std::cout << "unite!" << std::endl;
      //if (unite(fullWindow, *ot))
      //{
        //ot = selectionWindow.erase(ot);
      //}
      //else
      //{
        //++ot;
      //}
    //}
    //std::cout << fullWindow.size() << "/" << win.size() << std::endl;
    //if (fullWindow.size() >= win.size())
    //{
      //break;
    //}
  //}
  
  //if (cells.size() > 0)
  //{
    
    //std::cout << "filter by intersection" << std::endl << std::flush;
    //cells.sort();
    //std::list<CvoronoiCell<numberType> > selection;
    //selection.push_back(*cells.begin());
    
    //for (std::list<CvoronoiCell<numberType> >::iterator it = ++cells.begin(); it != cells.end(); ++it)
    //{
      //bool flag = true;
      //for (std::list<CvoronoiCell<numberType> >::iterator ot = selection.begin(); ot != selection.end();++ot)
      //{
        //windowType intersect = windowParts[it->getDescription()];
        //intersect.intersect(&windowParts[ot->getDescription()]);
        
        //if (intersect.size() == windowParts[it->getDescription()].size())
        //{
          //flag = false;        
          //break;
        //}
      //}
      
      //if (flag)
      //{
        //selection.push_front(*it);
      //}
    //}
    
    //std::cout << "selection size: " << selection.size() << std::endl << std::endl << std::flush;
    
    //cells = selection;
  
  //}
  
  if (win.getName() != "circle")
  {
  
  // dealing with overlap - inclusion-exclusion principle
  std::cout << "Dealing with overlap" << std::endl << std::flush;
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end();)
  {
    std::list<windowType> intersecting;
    std::list<bookmark> working;
    std::list<bookmark> working_next;
    
    bookmark cache;
    
    // first inclusion
    for (std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot)
    {
      if (it->size() > ot->size())
      {
        windowType intersect = windowParts[it->getDescription()];
        intersect.intersect(&windowParts[ot->getDescription()]);
        
        if (!intersect.empty())
        {
          // check if it is not already present
          bool unique = true;
          for (std::list<windowType>::iterator ut = intersecting.begin(); ut != intersecting.end(); ++ut)
          {
            windowType intersect02 = intersect;
            intersect02.intersect(&*ut);
            
            if (intersect02.size() == intersect.size())
            {
              unique = false;
              break;
            }
          }
          if (unique)
          {
            intersecting.push_back(intersect);
            cache.intersection = intersect;
            cache.last = --intersecting.end();
            working.push_back(cache);
          }
        }
      }
    }
    
    numberType area;
    numberType sign(1,0);
    
    //calculate area of union of working
    while (working.size() > 0)
    {
      //std::cout << working.size() << std::endl;
      
      // inclusion/exclusion
      for (std::list<bookmark>::iterator ot = working.begin(); ot != working.end(); ++ot)
      {
        area+= sign*ot->intersection.size();
      }
      
      // prepair intersection for next run
      for (std::list<bookmark>::iterator ot = working.begin(); ot != working.end(); ++ot)
      {
        for (std::list<windowType>::iterator ut = ++ot->last; ut != intersecting.end(); ++ut)
        {
          windowType intersect = ot->intersection;
          intersect.intersect(&*ut);
          if (!intersect.empty())
          {
            cache.intersection = intersect;
            cache.last = ut;
            working_next.push_back(cache);
          }
        }
      }
      
      sign = sign*numberType::get(-1,0);
      working = working_next;
      working_next.clear();
    }
    
    if (area > windowParts[it->getDescription()].size())
    {
      std::cout << "size comparison: " << area << " " << windowParts[it->getDescription()].size() << std::endl;
    }
    
    if (area == windowParts[it->getDescription()].size())
    {
      it = cells.erase(it);
    }
    else
    {
      ++it;
      //std::cout << "keep, cells size: " << cells.size() << std::endl;
    }
  }
  
  std::cout << "cells size: " << cells.size() << std::endl << std::endl << std::flush;
  
  }
  
  std::cout << "Output" << std::endl << std::flush; 
  
  
  
  
  // OUTPUT
  cells.sort();
  cells.reverse();
  
  
  
  std::ostringstream tmp;
  
  
  std::ostringstream tmp02;
  //tmp02 << folder << '/' << fileName << "_overlay";
  //printFile(tmp02, winSize);
  //tmp02 << ".svg";
  //std::ofstream overlayfile ( tmp02.str().c_str() );
  
  //overlayfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  //overlayfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
  //overlayfile << "<g transform=\"scale(17,-17)\">" << std::endl;
  
  count = 0;
  //for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  //{
    ////for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
    ////{
      ////std::cout << ((it!=ot) && (windowParts[it->getDescription()] == windowParts[ot->getDescription()])) << std::endl;
    ////}
    //++count;
    
    //// construct tiles
    //it->setColor(fillColor, strokeColor, strokeWidth);
    //it->colorify();
    //it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    //it->Center.setColor(fillColor, strokeColor, strokeWidth);
    
    //Cpoint<numberType> middle = it->middle();
    //Cpoint<numberType> middleDomain = it->middleDomain();
    
    //middle.setColor(fillColor, "#FF1744", strokeWidth);
    //middleDomain.setColor(fillColor, "#2979FF", strokeWidth);
    
    //std::ostringstream oss;
    //oss << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
    //std::ofstream myfile ( oss.str().c_str() );
    
    //myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    //myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
    
    //myfile << "<!--" << std::endl;
    ////it->var_dump( myfile );
    //myfile << it->Cell->size() << " " << it->size() << " " << atan2(it->middle().getX(), it->middle().getY()) << std::endl;
    //myfile << "-->" << std::endl;
    
    ////myfile << "<text x=\"" << -30*8.4 + 10 << "\" y=\"" << -30*8.4 + 90<< "\" style=\"font-size:100;font-family:Latin Modern Roman\">" << std::endl;
    ////myfile << count << std::endl;
    ////myfile << "</text>" << std::endl;
    
    //myfile << "<g transform=\"scale(17,-17)\">" << std::endl;
    
    ////myfile << "<circle cx=\"0\" cy=\"0\" r=\"" << L << "\" fill=\"#555555\" />\n" << std::endl;
    ////myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    
    //it->svg(myfile);
    //it->CarrierSet->svg(myfile);
    //it->Center.svg(myfile);
    //middle.svg(myfile);
    //middleDomain.svg(myfile);
    
    ////it->svg(overlayfile);
    ////it->CarrierSet->svg(overlayfile);
    ////it->Center.svg(overlayfile);
    
    //myfile << "</g>" << std::endl;
    //myfile << "</svg>";
    
    //myfile.close();
  //}
  
  //overlayfile << "</g>" << std::endl;
  //overlayfile << "</svg>";
  
  //overlayfile.close();
  
  
  
  tmp02.clear();
  tmp02.str(std::string());
  tmp02 << fileName << "_" << winSize << "_(";
  printFile(tmp02, winSize);
  tmp02 << ").svg";
  std::ofstream windowfile ( tmp02.str().c_str() );
  
  windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  windowfile << "<svg width=\"" << 5000 << "\" height=\"" << 5000 << "\" viewBox=\"" << -1.1*winSize << " " << -1.1*winSize << " " << 2.2*winSize << " " << 2.2*winSize << "\">\n" << std::endl;
  windowfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
  //windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
  win.svg(windowfile);
  
  // OUTPUT
  
  std::ostringstream tmp03;
  tmp03 << filenamePrefix << "list" << filenameSuffix;
  
  // write to file
  std::ofstream output(tmp03.str().c_str());
  cells.sort();
  cells.unique();
  cells.reverse();
  
  print(output, winSize);
  output << std::endl;
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    output << it->save() << std::endl;
  }
  output.close();
  
  // cells & window
  count = 1;
  {
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      ++count;
      
      
      it->setColor(fillColor, "#ffffff", borderstrokeWidth);
      it->colorify();
      it->CarrierSet->setColor(fillColor, "#ffffff", borderstrokeWidth);
      
      windowfile << "<g>" << std::endl;
      // window parts
      windowType intersect = windowParts[it->getDescription()];
      windowfile << "<!--";
      windowfile << it->CarrierSet->getDescription();
      windowfile << "-->" << std::endl;
      intersect.setColor(it->getFillColor(), windowstrokeColor, windowstrokeWidth);
      //intersect.intersect(&windowParts[cells.begin()->getDescription()]);
      intersect.svg(windowfile);
      
      // tiles
      //windowfile << "<svg x=\"" << static_cast<double>(intersect.centerX())-0.03*winSize << "\" y=\"" << static_cast<double>(intersect.centerY())-0.03*winSize << "\" width=\"" << 0.06*winSize << "\" height=\"" << 0.06*winSize << "\" viewBox=\"" << -2*coveringR << " " << -2*coveringR << " " << 4*coveringR << " " << 4*coveringR << "\">\n" << std::endl;
      
      
      //it->svg(windowfile);
      //it->CarrierSet->svg(windowfile);
      
      //it->setColor(fillColor, strokeColor, strokeWidth);
      //it->colorify();
      //it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      //it->Center.setColor(fillColor, strokeColor, strokeWidth);
      //it->svg(windowfile);
      //it->CarrierSet->svg(windowfile);
      //it->Center.svg(windowfile);
      
      //windowfile << "</svg>" << std::endl;
      windowfile << "</g>" << std::endl;
    }
  }
  //windowfile << "</g>" << std::endl;
  windowfile << "</svg>" << std::endl;
  windowfile.close();
  
  delete circ;
  delete insc;
  
  return 0;
}
