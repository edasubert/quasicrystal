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
#include <list>
#include <string>

typedef betaSet numberType;
typedef circle windowType;

int main (int argc, char* argv[])
{ 
  
  std::string folder = argv[2];
  std::string fileName = argv[3];
  
  
  
  
  
  numberType winSize;
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  
  std::string line;
  std::ifstream myfile(argv[1]);
  
  myfile >> winSize;
  getline(myfile, line);
  
  if (myfile.is_open())
  {
    while ( getline(myfile, line) )
    {
      CvoronoiCell<numberType> voronoi;
      voronoi.load(line);
      voronoi.setDescription(line);
      cells.push_back(voronoi);
    }
    myfile.close();
  }
  else std::cout << "Unable to open file"; 
  
  
  std::string fillColor = "#689F38";
  std::string strokeColor = "#263238";
  std::ostringstream convert;
  convert << (0.3)*winSize;
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
  
  
  
  // initialize
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
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
  
  
  
  // OUTPUT
  std::ostringstream tmp;
  
  int count = 1;
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  {
    // construct tiles
    it->CarrierSet->setPackingR();
    it->CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    it->setCenter(origin);
    it->setColor(fillColor, strokeColor, strokeWidth);
    it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
    it->Center.setColor(fillColor, strokeColor, strokeWidth);
    it->construct();
    
    
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
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    
    ++count;
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
  
  std::cout << "window parts: " << windowParts.size() << std::endl;
  
  count = 1;
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
  
  std::ostringstream tmp02;
  tmp02 << folder << '/' << fileName << "_";
  printFile(tmp02, winSize);
  tmp02 << ".svg";
  std::ofstream windowfile ( tmp02.str().c_str() );
  
  windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  windowfile << "<svg width=\"" << 5000 << "\" height=\"" << 5000 << "\" viewBox=\"" << -1.1*winSize << " " << -1.1*winSize << " " << 2.2*winSize << " " << 2.2*winSize << "\">\n" << std::endl;
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
  
  
  return 0;
}
