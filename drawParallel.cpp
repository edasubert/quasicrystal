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
  std::string filenameList;
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  std::cin >> filenamePrefix;
  std::cin >> filenameSuffix;
  
  filenameFinite = filenamePrefix + "finite" + filenameSuffix;
  filenameMore = filenamePrefix + "more" + filenameSuffix;
  filenameList = filenamePrefix + "list" + filenameSuffix;
  
  
  //std::cout << "DRAWING TILES AND WINDOW FROM A LIST OF CANDIDATES" << std::endl << std::flush;
  //std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  std::string fileName = argv[1];
  
  numberType winSize = const_winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  // initialize
  //windowType win( winSize );
  windowType win = getWindow(winSize);
  win.center( origin );
  
  numberType coveringR = covering;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
  
  //std::cout << "window size: ";
  //print(std::cout, winSize);
  //std::cout << std::endl << std::flush;
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile;
  if (win.getName() == "circle")
  {
    myfile.open(filenameFinite);
  }
  else
  {
    myfile.open(filenameList);
  }
  
  getline(myfile, line);
  winSize = const_winSize;
  
  //std::cout << std::endl << "Loading data: " << std::endl << std::flush;
  
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
  
  inputData.sort();
  inputData.unique();
  
  //std::cout << "unique strings read: " << inputData.size() << std::endl << std::flush; 
  
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
    
    cells.push_back(voronoi);
  }
  
  //std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::string strokeWidth = const_strokeWidth(numberType::get(1,0)/winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  std::string windowstrokeWidthZero = const_windowstrokeWidth(numberType::get(2,0)*winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  
  long int count;
  
  //std::cout << "Cutting window sections" << std::endl << std::flush;
  // CUT WINDOW SECTIONS -----------------------------------------------
  std::map<std::string, windowType> windowParts;
  
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    it->CarrierSet->sortClockwise();
    windowType intersect = win;
    for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
    {
      intersect.intersect((origin-*ot).star());
    }
    windowParts[it->save()] = intersect;
  }
  
  //std::cout << "cells size: " << cells.size() << std::endl << std::endl << std::flush;
  
  // OUTPUT ------------------------------------------------------------
  cells.sort();
  cells.reverse();
  
  std::list<CvoronoiCell<numberType> > cellsDNA;
  
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    cellsDNA.push_back(it->dna());
  }
  
  cellsDNA.sort();
  cellsDNA.unique();
  
  std::ostringstream tmp;
  
  std::ostringstream tmp02;
  
  
  std::cout << "\\begin{figure}[h!]" << std::endl << "\\centering" << std::endl;
  std::cout << "\\includegraphics[width=1\\textwidth]{img/results/circle8/circle8" << "_" << std::floor(100000.0*winSize) << "_(";
  printFile(std::cout, winSize);
  std::cout << ")" << ".pdf}" << std::endl;
  
  count = 0;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cellsDNA.begin(); it != cellsDNA.end(); ++it )
  {
    ++count;
    
    // construct tiles
    it->setColor(fillColor, strokeColor, strokeWidth);
    it->colorify();
    it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    it->Center.setColor(fillColor, strokeColor, strokeWidth);
    
    Cpoint<numberType> middle = it->middle();
    Cpoint<numberType> middleDomain = it->middleDomain();
    
    middle.setColor(fillColor, "#FF1744", strokeWidth);
    middleDomain.setColor(fillColor, "#2979FF", strokeWidth);
    
    
    std::ostringstream oss;
    oss << fileName << "_" << std::floor(100000.0*winSize) << "_(";
    printFile(oss, winSize);
    oss << ")_" << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
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
    
    myfile << "<g transform=\"scale(140,-140)\">" << std::endl;
    
    it->svg(myfile);
    it->CarrierSet->svg(myfile);
    it->Center.svg(myfile);
    //middle.svg(myfile);
    //middleDomain.svg(myfile);
    
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    
    std::cout << "\\includegraphics[width=0.12\\textwidth]{img/results/circle8/circle8" << "_" << std::floor(100000.0*winSize) << "_(";
    printFile(std::cout, winSize);
    std::cout << ")_" << std::setfill('0') << std::setw(3) << count << ".pdf}" << std::endl;
  }
  
  std::cout << "\\end{figure}" << std::endl << std::endl;
  
  std::cout << "\\begin{figure}[h!]" << std::endl << "\\centering" << std::endl;
  std::cout << "\\includegraphics[width=1\\textwidth]{img/results/octagon/quasi_circle" << "_" << std::floor(100000.0*winSize) << "_(";
  printFile(std::cout, winSize);
  std::cout << ")" << ".pdf}" << std::endl;
  
  std::cout << "\\end{figure}" << std::endl << std::endl;
  
  
  // OUTPUT WINDOW -----------------------------------------------------
  count = 0;
  
  tmp02.clear();
  tmp02.str(std::string());
  tmp02 << fileName << "_" << std::floor(100000.0*winSize) << "_(";
  printFile(tmp02, winSize);
  tmp02 << ").svg";
  std::ofstream windowfile ( tmp02.str().c_str() );
  
  windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  windowfile << "<svg width=\"" << 2200 << "\" height=\"" << 2200 << "\" viewBox=\"" << -1.01*winSize << " " << -1.01*winSize << " " << 2.02*winSize << " " << 2.02*winSize << "\">\n" << std::endl;
  //windowfile << "<svg width=\"" << 2200 << "\" height=\"" << 1000 << "\" viewBox=\"" << -0.05*winSize << " " << -0.4*winSize << " " << 1.1*winSize << " " << 0.45*winSize << "\">\n" << std::endl; //-------------------octagon
  //windowfile << "<svg width=\"" << 2200 << "\" height=\"" << 800 << "\" viewBox=\"" << -0.05*winSize << " " << -0.35*winSize << " " << 1.1*winSize << " " << 0.4*winSize << "\">\n" << std::endl; //-------------------dodecagon
  //windowfile << "<rect x=\"-100%\" y=\"-100%\" width=\"200%\" height=\"200%\" fill=\"white\" />" << std::endl;
  //windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  windowfile << "<defs>" << std::endl;
  //windowfile << "<mask id=\"mask\" x=\"0\" y=\"0\" width=\"100%\" height=\"100%\">" << std::endl;
  //windowfile << "<rect x=\"-10\" y=\"-10\" width=\"10\" height=\"10\" style=\"fill:#000000;\" />" << std::endl;
  //windowfile << "<g transform=\"scale(" << winSize << ")\">" << std::endl;
  //windowfile << "<polygon points=\"0,0 1,0 0.8535,-0.3535 \" style=\"fill:#ffffff;stroke-width:0\" fill-opacity=\"1\" />" << std::endl; //-------------------octagon
  ////windowfile << "<polygon points=\"0,0 1,0 0.933,-0.25 \" style=\"fill:#ffffff;stroke-width:0\" fill-opacity=\"1\" />" << std::endl; //-------------------dodecagon
  //windowfile << "</g>" << std::endl;
  //windowfile << "</mask>" << std::endl;
  //
  //windowfile << "<mask id=\"mask02\" x=\"0\" y=\"0\" width=\"100%\" height=\"100%\">" << std::endl;
  //windowfile << "<rect x=\"-10\" y=\"-10\" width=\"10\" height=\"10\" style=\"fill:#000000;\" />" << std::endl;
  //windowfile << "<g transform=\"scale(" << winSize << ")\">" << std::endl;
  //windowfile << "<polygon points=\"0,0 1,0 0.8535,-0.3535\" style=\"fill:#ffffff;stroke:#ffffff;stroke-width:" << 0.06*largestTile*winSize/(4*coveringR) << "\" fill-opacity=\"1\" stroke-linecap=\"round\" stroke-linejoin=\"miter\" stroke-miterlimit=\"100\" />" << std::endl; //-------------------octagon
  ////windowfile << "<polygon points=\"0,0 1,0 0.933,-0.25 \" style=\"fill:#ffffff;stroke-width:0\" fill-opacity=\"1\" />" << std::endl; //-------------------dodecagon
  //windowfile << "</g>" << std::endl;
  //windowfile << "</mask>" << std::endl;
  
  count = 1;
  {
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      ++count;
      
      it->setColor(fillColor, strokeColor, strokeWidth);
      it->colorify();
      
      windowfile << "<pattern id=\"pattern" << count << "\" x=\"" << (-1.4)*winSize << "\" y=\"" << (-1.4)*winSize << "\" width=\"" << 2.8*winSize << "\" height=\"" << 2.8*winSize << "\" patternUnits=\"userSpaceOnUse\" patternTransform=\"scale(0.01)\">";
      windowfile << "<g transform=\"translate(" << (1.4)*winSize << "," << (1.4)*winSize << ")\">";
      windowfile << "<g transform=\"scale(" << 0.92*winSize*winSize << ")\">";
      windowfile << "<rect x=\"" << (-2.0)*winSize*winSize << "\" y=\"" << (-2.0)*winSize*winSize << "\" width=\"" << 4.0*winSize*winSize << "\" height=\"" << 4.0*winSize*winSize << "\" style=\"fill:" << it->getFillColor() << "\" />";
      it->setColor("none", strokeColor, strokeWidth);
      it->svg(windowfile);
      windowfile << "</g>";
      windowfile << "</g>";
      windowfile << "</pattern>";
      
    }
  }
  windowfile << "</defs>" << std::endl;
  
  
  // cells & window
  count = 1;
  {
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      ++count;
      
      it->setColor(fillColor, strokeColor, strokeWidth);
      it->colorify();
      it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      
      // window parts
      windowType intersect = windowParts[it->getDescription()];
      //if ((intersect.zeroDensity()) && (intersect.zeroDensitySize() == numberType::get(0,0)))
      //{
      //  windowfile << "<g mask=\"url(#mask02)\">" << std::endl;
      //}
      //else
      //{
      //  windowfile << "<g mask=\"url(#mask)\">" << std::endl;
      //}
      
      if (intersect.zeroDensity())
      {
        intersect.setColor(it->getFillColor(), windowstrokeColor, windowstrokeWidthZero);
      }
      else
      {
        intersect.setColor("url(#pattern" + std::to_string(count) + ")", windowstrokeColor, windowstrokeWidth);
      }
      
      
      intersect.svg(windowfile);
      //windowfile << "</g>" << std::endl;
    }
    for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    {
      ++count;
      
      it->setColor(fillColor, strokeColor, strokeWidth);
      it->colorify();
      it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      
      windowType intersect = windowParts[it->getDescription()];
      
      if (intersect.zeroDensity())
      {
        // tiles
        //windowfile << "<g mask=\"url(#mask02)\">" << std::endl;
        windowfile << "<g transform=\"translate(" << static_cast<double>(intersect.centerX()) << "," << static_cast<double>(intersect.centerY()) << ")\">" << std::endl;
        windowfile << "<g transform=\"scale(" << 0.008*winSize*winSize << "," << 0.008*winSize*winSize << ")\">" << std::endl;
        
        it->svg(windowfile);
        //it->CarrierSet->svg(windowfile);
        
        windowfile << "</g>" << std::endl;
        windowfile << "</g>" << std::endl;
        //windowfile << "</g>" << std::endl;
      }
    }
    //for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    //{
      //++count;
      
      //it->setColor(fillColor, strokeColor, strokeWidth);
      //it->colorify();
      //it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      
      //windowfile << "<g transform=\"translate(" << 0.14*winSize << "," << -0.24*winSize << ")\">" << std::endl;
      //windowfile << "<g transform=\"scale(" << 0.14 << ")\">" << std::endl;
      
      //// window parts
      //windowType intersect = windowParts[it->getDescription()];
      //intersect.setColor(it->getFillColor(), windowstrokeColor, windowstrokeWidthZero);
      //intersect.svg(windowfile);
      
      //windowfile << "</g>" << std::endl;
      //windowfile << "</g>" << std::endl;
    //}
  }
  
  //windowfile << "</g>" << std::endl;
  windowfile << "</svg>" << std::endl;
  windowfile.close();
  
  return 0;
}
