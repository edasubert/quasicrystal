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
  std::string filenameList;
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  std::cin >> filenamePrefix;
  std::cin >> filenameSuffix;
  
  filenameFinite = filenamePrefix + "finite" + filenameSuffix;
  filenameMore = filenamePrefix + "more" + filenameSuffix;
  filenameList = filenamePrefix + "list" + filenameSuffix;
  
  
  std::cout << "ANALYZE TILE BEGINNING AND ENDING" << std::endl << std::flush;
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  //std::string fileName = argv[1];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  //std::list<CvoronoiCell<numberType> > cellsFull;
  
  std::string line;
  std::ifstream myfile(filenameList);
  //std::ifstream fileFull(filenamePrefix + "list_concat");
  
  myfile >> winSize;
  getline(myfile, line);
  //winSize = const_winSize;
  
  
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
  
  
  std::list<std::string> inputData; 
  
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
  
  inputData.clear();
  //std::string cell = "(-1-1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/2,(-1-1*alpha)/2 (1+3*alpha)/2,(1+1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1 (-1-3*alpha)/2,(1+1*alpha)/2 ";
  //std::string cell = "(0-1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(1+3*alpha)/2 (0-1*alpha)/1,(0+1*alpha)/1";
  std::string cell = "(-1+0*alpha)/1,(0+0*alpha)/1 (-3+1*alpha)/2,(1-1*alpha)/2 (-1+1*alpha)/2,(-3+1*alpha)/2 (-1+1*alpha)/2,(-1+1*alpha)/2 (-3+1*alpha)/2,(-1+1*alpha)/2 ";
  inputData.push_back(cell);
  
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
    //*voronoi.CarrierSet = *voronoi.CarrierSet * (numberType::get(1,0)/numberType::get(0,1));
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    cells.push_back(voronoi); 
  }
  
  std::cout << "cells: " << cells.size() << std::endl << std::flush; 
  
  cells.sort();
  
  
  
  
  //inputData.clear();
  
  //if (fileFull.is_open())
  //{
    //while ( getline(fileFull, line) )
    //{
      //if ((line.size() > 0) && (line[0] != '#'))
      //{
        //inputData.push_back(line);
      //}
    //}
    //fileFull.close();
  //}
  //else std::cout << "Unable to open file" << std::endl; 
  
  //inputData.sort();
  //inputData.unique();
  
  //std::cout << "unique strings read: " << inputData.size() << std::endl << std::flush; 
  
  //for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
  //{
    //CvoronoiCell<numberType> voronoi;
    //voronoi.load(*it);
    
    //if (voronoi.CarrierSet->size() < 3)
    //{
      //continue;
    //}
     
    //voronoi.setDescription(*it);
    //voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
    ////*voronoi.CarrierSet = *voronoi.CarrierSet * (numberType::get(1,0)/numberType::get(0,1));
    //voronoi.setCenter(origin);
    //voronoi.construct();
    //voronoi.filterSet();
    
    //cellsFull.push_back(voronoi); 
  //}
  
  //std::cout << "cells full: " << cellsFull.size() << std::endl << std::flush; 
  
  //cellsFull.sort();
  
  
  std::list<std::string> failed;
  std::list<numberType> success;
  
  
  
  std::string fillColor = const_fillColor;
  std::string strokeColor = const_strokeColor;
  std::string strokeWidth = const_strokeWidth(winSize);
  
  std::string windowfillColor = const_windowfillColor;
  std::string windowstrokeColor = const_windowstrokeColor;
  std::string windowstrokeWidth = const_windowstrokeWidth(winSize);
  
  std::ostringstream convert;
  convert << 0.6/winSize;
  std::string borderstrokeWidth = convert.str();
  
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  std::cout << "Start of the window sections" << std::endl << std::flush;
  
  for ( std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
  {
    
    // omega_1
    numberType L = numberType::get(1,0);
    numberType R = numberType::get(0,1)*numberType::get(0,1);
    
    numberType y3;
    numberType x3;
    numberType y2;
    numberType x2;
    numberType y1;
    numberType x1;
    numberType parts = numberType::get(2,10);
    numberType a;
    numberType d;
    numberType c;
    numberType cplusd;
    numberType ctimesd;
    
    //do 
    {
      int count = 0;
      
      for (numberType ut = numberType::get(0,0); ut <= parts; ut+= numberType::get(1,0)/numberType::get(0,3))
      {
        x1 = L+(R-L)*ut/parts;
        
        std::cout << x1 << "\t" << std::flush;
        print(std::cout, x1);
        
        windowType intersect = getWindow(x1);
        
        for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
        {
          intersect.intersect((origin-*ot).star());
        }
        
        //std::cout << x1 << "\t" << intersect.size() << std::endl;
        
        if (intersect.size() == numberType::get(0,0))
          continue;
        
        y1 = intersect.size();
        
        
        if (count > 3)
          break;
        
        x3 = x2;
        y3 = y2;
        x2 = x1;
        y2 = y1;
        
        ++count;
      }
      
      a = ((y1-y2)/(x1-x2)-(y1-y3)/(x1-x3))/(x2-x3);
        
      cplusd = (y1-y2)/((x1-x2)*a)-(x1+x2);
      ctimesd = y1/a - x1*x1-x1*cplusd;
      
      d = numberType::get(1,0,2)*((y1-y2)/((x1-x2)*a)-(x1+x2));
      
      
      parts+= numberType::get(1,0)/numberType::get(0,1);
      print(std::cout, parts);
      std::cout << std::endl << std::flush;
      
    } //while (cplusd*cplusd-numberType::get(4,0)*ctimesd != numberType::get(0,0));
    
    std::cout << "\t";
    std::cout << y1 << "\t";
    print(std::cout, y1);
    std::cout << "\t";
    std::cout << x1 << "\t";
    print(std::cout, x2);
    std::cout << "\t";
    std::cout << y1 << "\t";
    print(std::cout, y2);
    std::cout << "\t";
    std::cout << x1 << "\t";
    print(std::cout, x3);
    std::cout << "\t";
    std::cout << y1 << "\t";
    print(std::cout, y3);
    std::cout << "\t";
    std::cout << std::endl;
    print(std::cout, a);
    std::cout << "\t";
    print(std::cout, cplusd);
    std::cout << "\t";
    print(std::cout, ctimesd);
    std::cout << std::endl;
    std::cout << "\t x1:";
    print(std::cout, a*(x1*x1+x1*cplusd+ctimesd)-y1);
    print(std::cout, a*(x1+d)*(x1+d)-y1);
    std::cout << std::endl;
    std::cout << "\t x2:";
    print(std::cout, a*(x2*x2+x2*cplusd+ctimesd)-y2);
    print(std::cout, a*(x2+d)*(x2+d)-y2);
    std::cout << std::endl;
    std::cout << "\t x3:";
    print(std::cout, a*(x3*x3+x3*cplusd+ctimesd)-y3);
    print(std::cout, a*(x3+d)*(x3+d)-y3);
    std::cout << std::endl;
    
    if (cplusd*cplusd-numberType::get(4,0)*ctimesd == numberType::get(0,0))
    {
      std::cout << "******************************* MAGIC ***************************************" << std::endl;
      print(std::cout, cplusd/numberType::get(-2,0));
      std::cout << std::endl << cplusd/numberType::get(-2,0) << std::endl;
      success.push_back(cplusd/numberType::get(-2,0));
    }
    else
    {
      std::cout << -(1.0*cplusd+sqrt(1.0*cplusd*cplusd-4.0*ctimesd))/2.0 << std::endl;
      std::cout << -(1.0*cplusd-sqrt(1.0*cplusd*cplusd-4.0*ctimesd))/2.0 << std::endl;
      failed.push_back(it->save());
    }
    
    it->colorify();
    std::cout << it->getFillColor() << std::endl;
    std::cout << "----------------------------------------------------------------------------" << std::endl;
  
  
    
    //// omega_2 **************************************************************************************************************************************
    
    //L = numberType::get(1,0)/numberType::get(0,1);
    //R = numberType::get(0,1)*numberType::get(0,1);
    //parts = numberType::get(101,0);
    
    //do 
    //{
      //int count = 1;
      //for (numberType ut = numberType::get(0,0); ut <= parts; ut+= numberType::get(3,-1))
      //{
        //numberType x = L+(R-L)*ut/parts;
        //windowType testWin = getWindow(x);
        
        //// CUT WINDOW SECTIONS -----------------------------------------------
        //windowType intersection;
        //{
          //it->CarrierSet->sortClockwise();
          //windowType intersect = testWin;
          //for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
          //{
            //intersect.intersect((origin-*ot).star());
          //}
          //intersection = intersect;
        //}
        
        ////std::cout << "intersection size: " << intersection.size() << std::endl;
        
        //std::map<std::string, windowType> windowParts;
        
        //for ( std::list<CvoronoiCell<numberType> >::iterator ut = cellsFull.begin(); ut != cellsFull.end(); ++ut)
        //{
          //ut->CarrierSet->sortClockwise();
          //windowType intersect = testWin;
          //for (std::list<Cpoint<numberType> >::iterator ot = ut->CarrierSet->begin(); ot != ut->CarrierSet->end(); ++ot)
          //{
            //intersect.intersect((origin-*ot).star());
          //}
          //if (!intersect.empty())
          //{
            //windowParts[ut->getDescription()] = intersect;
          //}
        //}
        
        //std::list<windowType> intersecting;
        //std::list<bookmark> working;
        //std::list<bookmark> working_next;
        
        //bookmark cache;
        
        //// first inclusion
        //for (std::list<CvoronoiCell<numberType> >::iterator ot = cellsFull.begin(); ot != cellsFull.end(); ++ot)
        //{
          //if (it->size() > ot->size())
          //{
            //windowType intersect = intersection;
            //intersect.intersect(&windowParts[ot->getDescription()]);
            
            //if (!intersect.empty())
            //{
              //// check if it is not already present
              //bool unique = true;
              //for (std::list<windowType>::iterator ut = intersecting.begin(); ut != intersecting.end(); ++ut)
              //{
                //windowType intersect02 = intersect;
                //intersect02.intersect(&*ut);
                
                //if (intersect02.size() == intersect.size())
                //{
                  //unique = false;
                  //break;
                //}
              //}
              //if (unique)
              //{
                //intersecting.push_back(intersect);
                //cache.intersection = intersect;
                //cache.last = --intersecting.end();
                //working.push_back(cache);
              //}
            //}
          //}
        //}
          
        //numberType area;
        //numberType sign(1,0);
        
        ////calculate area of union of working
        //while (working.size() > 0)
        //{
          ////std::cout << working.size() << std::endl;
          
          //// inclusion/exclusion
          //for (std::list<bookmark>::iterator ot = working.begin(); ot != working.end(); ++ot)
          //{
            //area+= sign*ot->intersection.size();
          //}
          
          //// prepair intersection for next run
          //for (std::list<bookmark>::iterator ot = working.begin(); ot != working.end(); ++ot)
          //{
            //for (std::list<windowType>::iterator ut = ++ot->last; ut != intersecting.end(); ++ut)
            //{
              //windowType intersect = ot->intersection;
              //intersect.intersect(&*ut);
              //if (!intersect.empty())
              //{
                //cache.intersection = intersect;
                //cache.last = ut;
                //working_next.push_back(cache);
              //}
            //}
          //}
          
          //sign = sign*numberType::get(-1,0);
          //working = working_next;
          //working_next.clear();
        //}
        
        //std::cout << x << "\t" << intersection.size() << "\t" << area << "\t" << intersection.size()-area << "\t" << a*(x1*x1+x1*cplusd+ctimesd) << std::endl;
        
        //if (area == numberType::get(0,0))
          //continue;
        
        //if (intersection.size() == area)
        ////if (count > 3)
          //break;
        
        //y1 = (intersection.size()-area);
        //x1 = x;
        
        //x3 = x2;
        //y3 = y2;
        //x2 = x1;
        //y2 = y1;
        
        //++count;
        
        ////std::cout << "difference size:   " << intersection.size()-area << std::endl;
      //}
      
      //a = ((y1-y2)/(x1-x2)-(y1-y3)/(x1-x3))/(x2-x3);
      
      //cplusd = (y1-y2)/((x1-x2)*a)-(x1+x2);
      //ctimesd = y1/a - x1*x1-x1*cplusd;
      
      //d = numberType::get(1,0,2)*((y1-y2)/((x1-x2)*a)-(x1+x2));
      
      //std::cout << a << " ";
      //parts+= numberType::get(1,0);
      //print(std::cout, parts);
      //std::cout << std::endl << std::flush;
      
    //} while (cplusd*cplusd-numberType::get(4,0)*ctimesd != numberType::get(0,0));
    
    
    
    
    
    //if (cplusd*cplusd-numberType::get(4,0)*ctimesd == numberType::get(0,0))
    //{
      //std::cout << "******************************* MAGIC ***************************************" << std::endl;
      //print(std::cout, cplusd/numberType::get(-2,0)/numberType::get(0,1));
      //std::cout << std::endl << cplusd/numberType::get(-2,0)/numberType::get(0,1) << std::endl;
    //}
    //else
    //{
      //std::cout << -(1.0*cplusd+sqrt(1.0*cplusd*cplusd-4.0*ctimesd))/2.0 << std::endl;
      //std::cout << -(1.0*cplusd-sqrt(1.0*cplusd*cplusd-4.0*ctimesd))/2.0 << std::endl;
    //}
    
  }
  
  success.sort();
  success.unique();
  
  for (std::list<numberType>::iterator it = success.begin(); it != success.end(); ++it)
  {
    print(std::cout, *it);
    std::cout << std::endl;
  }
  
  for (std::list<std::string>::iterator it = failed.begin(); it != failed.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
  
  delete circ;
  delete insc;
  
  return 0;
}
