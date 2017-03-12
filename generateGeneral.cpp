#define _ERROR_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"

#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

// creates diagram of "any" window

int main( int argc, char ** argv )
{
  std::cout << "GENERAL QUASICRYSTAL GENERATOR" << std::endl << std::flush;
  std::cout << "------------------------------" << std::endl << std::flush;
  
  // intervel of the quasicrystal
  numberType x( 40,0,1);
  numberType y( 30,0,1);
  
  numberType x1 = numberType::get(-3,0,4)*x;
  numberType x2 = numberType::get( 3,0,4)*x;
  numberType y1 = numberType::get(-3,0,4)*y;
  numberType y2 = numberType::get( 3,0,4)*y;
  
  int winId = 1;
  
  // window definition
  numberType winSize;
  //while (std::cin >> winSize)
  {
    winSize = const_winSize;
    Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
    
    //rhombus win( winSize, winSize );
    //windowType win( winSize );
    //win.center( origin );
    
    windowType win = getWindow(winSize);
    win.center(origin);
    
    // control with hyperquasicrystal
    rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
    
    // control with hypoquasicrystal
    rhombus<numberType> *insc = dynamic_cast<rhombus<numberType>*> ( win.inscribed() );
    
    numberType coveringR = numberType::coveringR()*insc->large();
    
    std::cout << "window size: ";
    print(std::cout, winSize);
    std::cout << std::endl;
    
    std::cout << "hypercrystal: ";
    print( std::cout, circ->Xwindow().l() );
    std::cout << std::endl << std::flush;
    
    std::cout << "hypocrystal:  ";
    print( std::cout, insc->Xwindow().l() );
    std::cout << std::endl << std::flush;
    
    std::cout << "Onto generation ..." << std::endl << std::flush;
    
    CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
    delone.setDescription("Deloneovská množina bodů");
    //*voronoi->CarrierSet = delone;
    std::cout << "quasicrystal size: " << delone.size() << std::endl << std::flush;
    
    CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
    
    //CdeloneSet<numberType> hypo = quasicrystal2D( *insc, x1, x2, y1, y2 );
    
    CvoronoiCell<numberType>::large = numberType::get(5,0)*coveringR;
    
    delone.setPackingR();
    delone.setCoveringR(CvoronoiCell<numberType>::large);
    
    std::ostringstream cache;
    
    std::cout << "L: " << insc->large() << std::endl;
    
    
    std::string folder = argv[1];
    std::string fileName = argv[2];
    std::string fillColor = const_fillColor;
    std::string strokeColor = const_strokeColor;
    std::ostringstream convert;
    convert << 0.06*sqrt((x2-x1)*(y2-y1)/numberType::get(delone.size(),0,1));
    std::string strokeWidth = convert.str();
    
    delone.setColor( fillColor, strokeColor, strokeWidth );
    hyper.setColor( fillColor, "#2196F3", strokeWidth );
    //hypo.setColor( fillColor, "#D50000", strokeWidth );
    
    
    for ( std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it )
    {
      if ((it->getX() < numberType::get(-6,0,10)*x) || (it->getX() > numberType::get(6,0,10)*x) || (it->getY() < numberType::get(-6,0,10)*y) || (it->getY() > numberType::get(6,0,10)*y))
      {
        continue;
      }
      CvoronoiCell<numberType> voronoi;
      
      voronoi.setCenter(origin);
      voronoi.setColor( fillColor, strokeColor, strokeWidth );
      
      *voronoi.CarrierSet = delone;
      *voronoi.CarrierSet-= *it;
      //voronoi.CarrierSet->filterDistanceOrigin(numberType::get(2, 0)*coveringR);
      
      voronoi.CarrierSet->sortByDistance();
      
      voronoi.construct();
      voronoi.colorify();
      *voronoi.Cell+= *it;
      
      voronoi.svg(cache);
    }
    
    std::ostringstream oss;
    oss << folder << '/' << fileName << "_" << winId << "_";
    printFile(oss, winSize);
    oss << ".svg";
        
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"" << 1920 << "\" height=\"" << 1920*y/x << "\" viewBox=\"" << -x/2 << " " << -y/2 << " " << x << " " << y << "\">\n" << std::endl;
    //myfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" style=\"stroke:" << strokeColor << ";stroke-width:" << y/200 << ";fill:none;\" />" << std::endl;
    myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
    
    myfile << cache.str().c_str() << std::endl;
    
    myfile << "<!-- hypercrystal -->" << std::endl;
    //hyper.svg(myfile);
    //hyper.star().svg(myfile);
    myfile << std::endl;
    
    myfile << "<!-- quasicrystal -->" << std::endl;
    delone.svg(myfile);
    //delone.star().svg(myfile);
    myfile << std::endl;
    
    myfile << "<!-- hypocrystal -->" << std::endl;
    //hypo.svg(myfile);
    myfile << std::endl;
    
    
    win.setColor( "none", "#000000", "0.006" );
    circ->setColor( "none", "#000000", "0.006" );
    insc->setColor( "none", "#000000", "0.006" );
    
    //win.svg(myfile);
    //circ->svg(myfile);
    //insc->svg(myfile);
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    ++winId;
  }
}
