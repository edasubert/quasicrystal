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
#include <list>
#include <string>

typedef alphaSet numberType;
typedef polygon<numberType> windowType;

int main (int argc, char* argv[])
{ 
  
  std::string folder = argv[2];
  std::string fileName = argv[3];
  
  
  
  
  
  numberType winSize;
  
  
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile(argv[1]);
  
  myfile >> winSize;
  getline(myfile, line);
  
  
  
  // initialize
  //windowType win( winSize );
  windowType win = polygon<numberType>::octagon(winSize);
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
  
  
  std::cout << "Loading data: " << std::endl << std::flush;
  
  std::list<std::string> inputData; 
  
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
  
  std::cout << "strings read: " << inputData.size() << std::endl << std::flush; 
  
  inputData.sort();
  inputData.unique();
  
  std::cout << "strings after unique: " << inputData.size() << std::endl << std::flush; 
  
  
  for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  {
    CvoronoiCell<numberType> voronoi;
    voronoi.load(*it);
    
    if (voronoi.CarrierSet->size() < 3)
    {
      continue;
    }
    
    for (std::list<Cpoint<numberType> >::iterator ot = voronoi.CarrierSet->begin(); ot != voronoi.CarrierSet->end();)
    {
      if (*ot == origin)
      {
        ot = voronoi.CarrierSet->removePoint(ot);
      }
      else
      {
        ++ot;
      }
    }
    
    
    voronoi.setDescription(*it);
    voronoi.CarrierSet->setPackingR();
    voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    voronoi.setCenter(origin);
    voronoi.construct();
    cells.push_back(voronoi);
  }
  
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  cells.unique();
  
  std::cout << "cells after unique: " << cells.size() << std::endl << std::endl << std::flush; 
  
  std::string fillColor = "#689F38";
  std::string strokeColor = "#263238";
  std::ostringstream convert;
  convert << (0.1)*winSize;
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
  
  
  int count;
  
  
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
    }
    else
    {
      it = cells.erase(it); 
      //++it;
      std::cout << "WHAT IS GOING ON?!?!?!" << std::endl << std::flush;
    }
  }
  
  std::cout << "cells size: " << cells.size() << std::endl << std::endl << std::flush;
  std::cout << "Dealing with overlap" << std::endl << std::flush;
  
  //// deal with overlap
  cells.sort();
  std::list<CvoronoiCell<numberType> > selection;
  
  selection.push_back(*cells.begin());
  
  count = 0;
  int printCount = 0;
  bool check;
  
  for (std::list<CvoronoiCell<numberType> >::iterator ot = ++cells.begin(); ot != cells.end(); ++ot)
  {
    std::list<windowType> toCut;
    // gather cutting material
    for ( std::list<CvoronoiCell<numberType> >::iterator it = selection.begin(); it != selection.end(); ++it )
    {
      windowType intersection = windowParts[it->getDescription()];
      intersection.intersect(&windowParts[ot->getDescription()]);
      
      if ((!intersection.empty()) && (ot->size() > it->size()))
      {
        toCut.push_back(windowParts[it->getDescription()]);
        std::cout << "CUT cut cut" << std::endl << std::flush;
      }
    }
    
    //toCut.sort();
    //toCut.reverse();
    
    // do some cutting
    for ( std::list<windowType>::iterator it = toCut.begin(); it != toCut.end(); ++it )
    {
      windowType intersection = *it;
      intersection.intersect(&windowParts[ot->getDescription()]);
      diff(windowParts[ot->getDescription()], intersection);
      
      if (windowParts[ot->getDescription()].empty())
        break;
    }
    
    // decide
    if (!windowParts[ot->getDescription()].empty())
    {
      selection.push_back(*ot);
    }
    
    // print progress
    ++count;
    ++printCount;
    if (500*printCount > cells.size())
    {
      std::cout << "processed " << count << "/" << cells.size() << " <=> " << 100*count/cells.size() << "%" << "\t| " << "selection size: " << selection.size() << std::endl << std::flush;
      printCount = 0;
    }
  }
  
  std::cout << "selection size: " << selection.size() << std::endl << std::endl << std::flush;
  
  //cells = selection;
  
  std::cout << "Output" << std::endl << std::flush; 
  
  
  
  
  // OUTPUT
  std::ostringstream tmp;
  
  
  std::ostringstream tmp02;
  tmp02 << folder << '/' << fileName << "_overlay";
  printFile(tmp02, winSize);
  tmp02 << ".svg";
  std::ofstream overlayfile ( tmp02.str().c_str() );
  
  overlayfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  overlayfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
  overlayfile << "<g transform=\"scale(17,-17)\">" << std::endl;
  
  count = 1;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    // construct tiles
    it->setColor(fillColor, strokeColor, strokeWidth);
    it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    it->Center.setColor(fillColor, strokeColor, strokeWidth);
    
    Cpoint<numberType> middle = it->middle();
    Cpoint<numberType> middleDomain = it->middleDomain();
    
    middle.setColor(fillColor, "#FF1744", strokeWidth);
    middleDomain.setColor(fillColor, "#2979FF", strokeWidth);
    
    std::ostringstream oss;
    oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30*8.4 << " " << -30*8.4 << " " << 60*8.4 << " " << 60*8.4 << "\">\n" << std::endl;
    
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
    middle.svg(myfile);
    middleDomain.svg(myfile);
    
    it->svg(overlayfile);
    it->CarrierSet->svg(overlayfile);
    it->Center.svg(overlayfile);
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    
    ++count;
  }
  
  overlayfile << "</g>" << std::endl;
  overlayfile << "</svg>";
  
  overlayfile.close();
  
  
  
  tmp02.clear();
  tmp02.str(std::string());
  tmp02 << folder << '/' << fileName << "_";
  printFile(tmp02, winSize);
  tmp02 << ".svg";
  std::ofstream windowfile ( tmp02.str().c_str() );
  
  windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  windowfile << "<svg width=\"" << 5000 << "\" height=\"" << 5000 << "\" viewBox=\"" << -1.1*winSize << " " << -1.1*winSize << " " << 2.2*winSize << " " << 2.2*winSize << "\">\n" << std::endl;
  windowfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
  //windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
  win.svg(windowfile);
  
  // OUTPUT
  count = 1;
  {
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      windowfile << "<g>" << std::endl;
      // window parts
      windowType intersect = windowParts[it->getDescription()];
      windowfile << "<!--";
      windowfile << it->CarrierSet->getDescription();
      windowfile << "-->" << std::endl;
      intersect.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
      //intersect.intersect(&windowParts[cells.begin()->getDescription()]);
      intersect.svg(windowfile);
      
      // tiles
      windowfile << "<svg x=\"" << static_cast<double>(intersect.centerX())-0.03*winSize << "\" y=\"" << static_cast<double>(intersect.centerY())-0.03*winSize << "\" width=\"" << 0.06*winSize << "\" height=\"" << 0.06*winSize << "\" viewBox=\"" << -2*coveringR << " " << -2*coveringR << " " << 4*coveringR << " " << 4*coveringR << "\">\n" << std::endl;
      
      
      it->setColor(fillColor, "#ffffff", borderstrokeWidth);
      it->CarrierSet->setColor(fillColor, "#ffffff", borderstrokeWidth);
      it->svg(windowfile);
      it->CarrierSet->svg(windowfile);
      
      it->setColor(fillColor, strokeColor, strokeWidth);
      it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      it->Center.setColor(fillColor, strokeColor, strokeWidth);
      it->svg(windowfile);
      it->CarrierSet->svg(windowfile);
      it->Center.svg(windowfile);
      
      windowfile << "</svg>" << std::endl;
      windowfile << "</g>" << std::endl;
    }
  }
  //windowfile << "</g>" << std::endl;
  windowfile << "</svg>" << std::endl;
  windowfile.close();
  
  
  return 0;
}
