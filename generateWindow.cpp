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
  
  std::string folder = argv[1];
  std::string fileName = argv[2];
  std::string fillColor = "#8BC34A";
  std::string strokeColor = "rgb(25,25,25)";
  std::string strokeWidth = "0.1em";
  
  // intervel of the quasicrystal
  numberType x1(-80,0);
  numberType x2( 80,0);
  numberType y1(-45,0);
  numberType y2( 45,0);
  
  // window definition
  numberType winSize( 4, -1, 4 );
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  //rhombus win( winSize, winSize );
  //rhombus win( winSize );
  circle win( winSize );
  win.center( origin );
  
  // quasicrystal generation
  CvoronoiCell<numberType>* voronoi;
  voronoi = new CvoronoiCell<numberType>();
  
  std::cout << "Onto generation ..." << std::endl << std::flush;
  
  CdeloneSet<numberType> delone = quasicrystal2D( win, x1, x2, y1, y2 );
  delone.setDescription("Deloneovská množina bodů");
  delone.setColor( fillColor, strokeColor, strokeWidth );
  *voronoi->CarrierSet = delone;
  
  // control with hyperquasicrystal
  rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
  CdeloneSet<numberType> hyper = quasicrystal2D( *circ, x1, x2, y1, y2 );
  hyper.setColor( fillColor, "#2196F3", strokeWidth );
  
  std::cout << "hypercrystal: ";
  print( std::cout, circ->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  // control with hypoquasicrystal
  rhombus *insc = dynamic_cast<rhombus*> ( win.inscribed() );
  CdeloneSet<numberType> hypo = quasicrystal2D( *insc, x1, x2, y1, y2 );
  hypo.setColor( fillColor, "#D50000", strokeWidth );
  
  std::cout << "hypocrystal:  ";
  print( std::cout, insc->Xwindow().l() );
  std::cout << std::endl << std::flush;
  
  CvoronoiCell<numberType>::large = insc->large();
  
  delone.setPackingR();
  delone.setCoveringR(CvoronoiCell<numberType>::large);
  
  std::ostringstream cache;
  
  std::cout << "points of quasicrystal: " << voronoi->CarrierSet->size() << std::endl << std::flush;
  
  //for ( std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it )
  //{
    //voronoi->setCenter( *it );
    //voronoi->setColor( fillColor, strokeColor, strokeWidth );
    
    //Cpoint<numberType> center = *it;
    
    //*voronoi->CarrierSet = delone;
    
    //*voronoi->CarrierSet-=center;
    //voronoi->CarrierSet->filterDistanceOrigin(insc->large());
    //voronoi->CarrierSet->sortByDistance();
    //*voronoi->CarrierSet+=center;
    
    //voronoi->construct();
    ////voronoi->colorify();
    //voronoi->svg( cache );
  //}
  
  std::ostringstream oss;
  oss << folder << '/' << fileName;
      
  std::ofstream myfile ( oss.str().c_str() );
  
  myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
  myfile << "<svg width=\"1920\" height=\"1920\" viewBox=\"" << -4*winSize << " " << -4*winSize << " " << 8*winSize << " " << 8*winSize << "\">\n" << std::endl;
  //myfile << "<g transform=\"scale(1,-1)\">" << std::endl;
  
  //myfile << cache.str().c_str() << std::endl;
  
  insc->svg(myfile);
  win.svg(myfile);
  circ->svg(myfile);
  
  
  myfile << "<!-- hypercrystal -->" << std::endl;
  hyper.star().svg( myfile );
  myfile << std::endl;
  
  myfile << "<!-- quasicrystal -->" << std::endl;
  delone.star().svg( myfile );
  myfile << std::endl;
  
  myfile << "<!-- hypocrystal -->" << std::endl;
  hypo.star().svg( myfile );
  myfile << std::endl;
  
  //myfile << "</g>" << std::endl;
  myfile << "</svg>";
  
  myfile.close();
  
  delete voronoi;
}
