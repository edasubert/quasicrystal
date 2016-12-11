#include "SUPPORT/generate2.h"
#include "SUPPORT/window.h"
#include "SUPPORT/geometricObject2.h"
#include "SUPPORT/betaSet.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

#define _ERROR_

// creates diagram of "any" window

typedef betaSet numberType;
typedef circle windowType;

int main( int argc, char ** argv )
{
  std::cout << "LIMITED WINDOW DIVISION GENERATOR" << std::endl << std::flush;
  std::cout << "---------------------------------" << std::endl << std::flush;
  
  // intervel of the quasicrystal
  numberType x(300,0);
  
  
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  // window definition
  numberType winSize;
  //while (std::cin >> winSize)
  
  winSize = numberType::get(1,0);
  
  std::string folder = argv[1];
  std::string fileName = argv[2];
  std::string fillColor = "none";
  std::string strokeColor = "rgb(25,25,25)";
  std::ostringstream convert;
  convert << 0.24/winSize;
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
  
  
  //rhombus win( winSize, winSize );
  windowType win( winSize );
  win.center( origin );
  
  // control with hyperquasicrystal
  rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
  
  // control with hypoquasicrystal
  rhombus *insc = dynamic_cast<rhombus*> ( win.inscribed() );
  
  
  numberType x1 = -x;//numberType::get(-3, 0)*insc->large();
  numberType x2 = x;
  numberType y1 = -x;//numberType::get(-3, 0)*insc->large();
  numberType y2 = x;
  
  
  betaSet coveringR = numberType::get(161, -43)*insc->large();
  
  std::cout << "x: " << x << std::endl << std::flush;
  std::cout << "Onto generation ..." << std::endl << std::flush;
  
  CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
  delone.setDescription("Deloneovská množina bodů");
  delone.setColor( fillColor, strokeColor, strokeWidth );
  //*voronoi->CarrierSet = delone;
  
  CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
  hyper.setColor( fillColor, "#2196F3", strokeWidth );
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CdeloneSet<numberType> hypo = quasicrystal2D( *insc, x1, x2, y1, y2 );
  hypo.setColor( fillColor, "#D50000", strokeWidth );
  
  std::cout << "hypocrystal:  ";
  print( std::cout, insc->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CvoronoiCell<numberType>::large = numberType::get(3,0)*coveringR;
  
  delone.setPackingR();
  delone.setCoveringR(CvoronoiCell<numberType>::large);
  
  std::cout << "L: " << insc->large() << std::endl;
  
  //std::cout << "points of quasicrystal: " << voronoi->CarrierSet->size() << std::endl << std::flush;
  
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
    voronoi.setColor( fillColor, strokeColor, strokeWidth );
    
    *voronoi.CarrierSet = delone;
    *voronoi.CarrierSet-= *it;
    voronoi.CarrierSet->filterDistanceOrigin(numberType::get(2, 0)*coveringR);
    
    voronoi.construct();
    voronoi.filterSet();
    
    
    convert.str("");
    convert.clear();
    
    print(convert, it->getX());
    print(convert, it->getY());
    voronoi.setDescription(convert.str());
    
    cells.push_back(voronoi);
    
    cells.sort();
    cells.unique();
  }
  
  std::cout << "found unique cells: " << cells.size() << std::endl;
  
  
  
  std::ostringstream tmp02;
  tmp02 << folder << '/' << fileName << "_";
  printFile(tmp02, winSize);
  tmp02 << "-" << floor(x);
  
  // write to file
  std::ofstream output(tmp02.str().c_str());
  cells.sort();
  cells.unique();
  
  print(output, winSize);
  output << std::endl;
  
  for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    output << it->save() << std::endl;
  }
  output.close();
  
  
  
  //// CUT WINDOW SECTIONS
  //std::map<std::string, windowType> windowParts;
  
  //for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
  //{
    //windowType intersect = win;
    //for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
    //{
      //windowType moving = win;
      //moving.center(origin-ot->star());
      //intersect.intersect(&moving);
    //}
    //windowParts[it->getDescription()] = intersect;
  //}
  
  //// deal with overlap
  //cells.sort();
  //std::map<std::string, std::list<windowType> > toCut;
  
  //bool check;
  //for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
  //{
    //for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    //{
      //if (it == ot) continue;
      
      //windowType intersection = windowParts[it->getDescription()];
      //intersection.intersect(&windowParts[ot->getDescription()]);
      //if (!intersection.empty())
      //{
        //if (it->size() > ot->size())
        //{
          //toCut[it->getDescription()].push_back(intersection);//windowParts[ot->getDescription()]);
          ////check+= diff(windowParts[it->getDescription()], intersection);
        //}
        //else
        //{
          //toCut[ot->getDescription()].push_back(intersection);//windowParts[it->getDescription()]);
          ////check+= diff(windowParts[ot->getDescription()], intersection);
        //}
      //}
    //}
  //}
  
  //std::cout << windowParts.size() << std::endl;
  
  //int count = 1;
  //for ( std::list<CvoronoiCell<numberType> >::iterator ot = cells.begin(); ot != cells.end(); ++ot )
  //{
    ////std::ostringstream oss;
    ////oss << folder << '/' << fileName << std::setfill('0') << std::setw(3) << count++ << ".svg";// << " " << it->size();
    ////std::ofstream myfile ( oss.str().c_str() );
    
    ////myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    ////myfile << "<svg width=\"2800\" height=\"840\" viewBox=\"" << -1*winSize << " " << -0.3*winSize << " " << 2*winSize << " " << 0.6*winSize << "\">\n" << std::endl;
    ////myfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
    ////myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    
    //for ( std::list<windowType>::iterator it = toCut[ot->getDescription()].begin(); it != toCut[ot->getDescription()].end(); ++it )
    //{
      ////it->setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
      ////it->svg(myfile);
      //windowType intersection = *it;
      //intersection.intersect(&windowParts[ot->getDescription()]);
      ////intersection.setColor("#FF5722", windowstrokeColor, windowstrokeWidth);
      ////intersection.svg(myfile);
      //diff(windowParts[ot->getDescription()], intersection);
    //}
    ////windowParts[ot->getDescription()].setColor("#03A9F4", windowstrokeColor, windowstrokeWidth);
    ////windowParts[ot->getDescription()].svg(myfile);
    
    ////myfile << "</g>" << std::endl;
    ////myfile << "</svg>" << std::endl;
    ////myfile.close();
  //}
  
  //std::ostringstream tmp02;
  //tmp02 << folder << '/' << fileName << "_";
  //printFile(tmp02, winSize);
  //tmp02 << "_" << x;
  //tmp02 << ".svg";
  //std::ofstream windowfile ( tmp02.str().c_str() );
  
  //windowfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  //windowfile << "<svg width=\"" << 5000 << "\" height=\"" << 5000 << "\" viewBox=\"" << -1.1*winSize << " " << -1.1*winSize << " " << 2.2*winSize << " " << 2.2*winSize << "\">\n" << std::endl;
  //windowfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
  //windowfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  //win.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
  //win.svg(windowfile);
  
  //// OUTPUT
  //count = 1;
  //{
    //for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it )
    //{ 
      //// window parts
      //windowType intersect = windowParts[it->getDescription()];
      //windowfile << "<!--";
      //windowfile << it->CarrierSet->getDescription();
      //windowfile << "-->" << std::endl;
      //intersect.setColor(windowfillColor, windowstrokeColor, windowstrokeWidth);
      ////intersect.flip(2);
      //intersect.svg(windowfile);
      
      //// tiles
      //windowfile << "<svg x=\"" << static_cast<double>(intersect.centerX())-0.03*winSize << "\" y=\"" << static_cast<double>(intersect.centerY())-0.03*winSize << "\" width=\"" << 0.06*winSize << "\" height=\"" << 0.06*winSize << "\" viewBox=\"" << -2*coveringR << " " << -2*coveringR << " " << 4*coveringR << " " << 4*coveringR << "\">\n" << std::endl;
      
      
      //it->setColor(fillColor, "#ffffff", borderstrokeWidth);
      //it->CarrierSet->setColor(fillColor, "#ffffff", borderstrokeWidth);
      //it->svg(windowfile);
      //it->CarrierSet->svg(windowfile);
      
      //it->setColor(fillColor, strokeColor, strokeWidth);
      //it->CarrierSet->setColor(fillColor, strokeColor, strokeWidth);
      //it->svg(windowfile);
      //it->CarrierSet->svg(windowfile);
      
      //windowfile << "</svg>" << std::endl;
    //}
  //}
  //windowfile << "</g>" << std::endl;
  //windowfile << "</svg>" << std::endl;
  //windowfile.close();
  
}
