#include "SUPPORT/generate2.h"
#include "SUPPORT/window.h"
#include "SUPPORT/geometricObject2.h"
#include "SUPPORT/betaSet.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#define _ERROR_

// creates diagram of "any" window

typedef betaSet numberType;

int main( int argc, char ** argv )
{
  std::cout << "GENERAL QUASICRYSTAL GENERATOR" << std::endl << std::flush;
  std::cout << "------------------------------" << std::endl << std::flush;
  
  // intervel of the quasicrystal
  numberType x1(-150,0);
  numberType x2( 150,0);
  numberType y1(-45,0);
  numberType y2( 45,0);
  
  int winId = 1;
  
  // window definition
  numberType winSize;
  while (std::cin >> winSize)
  {
    Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
    
    
    std::string folder = argv[1];
    std::string fileName = argv[2];
    std::string fillColor = "none";
    std::string strokeColor = "rgb(25,25,25)";
    std::ostringstream convert;
    convert << 0.1/winSize;
    std::string strokeWidth = convert.str();
    
    
    //Cpoint<numberType> awayFromZero( numberType::get(120,10), numberType::get(244,20) );
    
    //rhombus win( winSize, winSize );
    rhombus win( winSize );
    win.center( origin );
    
    // control with hyperquasicrystal
    rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
    
    // control with hypoquasicrystal
    rhombus *insc = dynamic_cast<rhombus*> ( win.inscribed() );
    
    
    betaSet coveringR = numberType::get(161, -43)*insc->large();
    
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
    
    std::ostringstream cache;
    
    std::cout << "L: " << insc->large() << std::endl;
    
    //std::cout << "points of quasicrystal: " << voronoi->CarrierSet->size() << std::endl << std::flush;
    
    for ( std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it )
    {
      CvoronoiCell<numberType> voronoi;
      
      voronoi.setCenter(origin);
      voronoi.setColor( fillColor, strokeColor, strokeWidth );
      
      //Cpoint<numberType> center = *it;
      
      *voronoi.CarrierSet = delone;
      *voronoi.CarrierSet-= *it;
      voronoi.CarrierSet->filterDistanceOrigin(numberType::get(2, 0)*coveringR);
      
      //*voronoi->CarrierSet-=center;
      //voronoi->CarrierSet->filterDistanceOrigin(numberType::get(2,0)*coveringR);
      //voronoi->CarrierSet->sortByDistance();
      
      //*voronoi->CarrierSet+=awayFromZero;
      
      voronoi.construct();
      *voronoi.Cell+= *it;
      
      //*voronoi->Cell+=center;
      //*voronoi->Cell-=awayFromZero;
      //voronoi->colorify();
      voronoi.svg( cache );
    }
    
    std::ostringstream oss;
    oss << folder << '/' << fileName << "_" << winId << "_";
    printFile(oss, winSize);
    oss << ".svg";
        
    std::ofstream myfile ( oss.str().c_str() );
    
    myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
    myfile << "<svg width=\"" << x2-x1 << "\" height=\"" << y2-y1 << "\" viewBox=\"" << -(x2-x1)/2 << " " << -(y2-y1)/2 << " " << x2-x1 << " " << y2-y1 << "\">\n" << std::endl;
    myfile << "<rect x=\"-50%\" y=\"-50%\" width=\"100%\" height=\"100%\" style=\"stroke:" << strokeColor << ";stroke-width:" << (y2-y1)/200 << ";fill:none;\" />" << std::endl;
    myfile << "<g transform=\"scale(1.2,-1.2)\">" << std::endl;
    
    myfile << cache.str().c_str() << std::endl;
    
    myfile << "<!-- hypercrystal -->" << std::endl;
    hyper.svg( myfile );
    myfile << std::endl;
    
    myfile << "<!-- quasicrystal -->" << std::endl;
    delone.svg( myfile );
    myfile << std::endl;
    
    myfile << "<!-- hypocrystal -->" << std::endl;
    //hypo.svg( myfile );
    myfile << std::endl;
    
    
    //win.setColor( "none", "#000000", "0.0006" );
    //circ->setColor( "none", "#000000", "0.0006" );
    
    //win.svg(myfile);
    //circ->svg(myfile);
    
    myfile << "</g>" << std::endl;
    myfile << "</svg>";
    
    myfile.close();
    
    ++winId;
  }
}
