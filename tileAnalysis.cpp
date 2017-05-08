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
  
  
  std::string line;
  numberType input;
  
  // singular file
  std::string singularFilename = "closed/singular";
  std::ifstream singularFile(singularFilename);
  std::list<numberType> singularSizes;
  std::list<numberType> singularSizesUsed;
  if (singularFile.is_open())
  {
    while (singularFile)
    {
      singularFile >> input;
      singularSizes.push_back(input);
    }
    singularFile.close();
  }
  else std::cout << "Unable to open file" << std::endl; 
  
  singularSizes.sort();
  singularSizes.unique();
  std::cout << "singular sizes size: " << singularSizes.size() << std::endl;
  
  std::cout << "ANALYZE TILE BEGINNING AND ENDING" << std::endl << std::flush;
  std::cout << "--------------------------------------------------" << std::endl << std::flush;
  
  //std::string fileName = argv[1];
  
  numberType winSize;
  Cpoint<numberType> origin(numberType::get(0,0), numberType::get(0,0));
  
  
  // input
  std::list<CvoronoiCell<numberType> > cells;
  std::list<CvoronoiCell<numberType> > cellsFull;
  
  std::ifstream myfile(filenameList);
  std::ifstream fileFull(filenamePrefix + "list_concat");
  
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
  
  //inputData.clear();
  //std::string cell = "(-1-1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/2,(-1-1*alpha)/2 (1+3*alpha)/2,(1+1*alpha)/2 (0+0*alpha)/1,(0+1*alpha)/1 (-1-3*alpha)/2,(1+1*alpha)/2 ";
  //std::string cell = "(0-1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(-1-1*alpha)/2 (1+1*alpha)/1,(0+0*alpha)/1 (1+1*alpha)/2,(1+3*alpha)/2 (0-1*alpha)/1,(0+1*alpha)/1";
  //std::string cell = "(-1+0*alpha)/1,(0+0*alpha)/1 (-3+1*alpha)/2,(1-1*alpha)/2 (-1+1*alpha)/2,(-3+1*alpha)/2 (-1+1*alpha)/2,(-1+1*alpha)/2 (-3+1*alpha)/2,(-1+1*alpha)/2 ";
  //inputData.push_back(cell);
  
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
  
  
  
  
  inputData.clear();
  
  if (fileFull.is_open())
  {
    while ( getline(fileFull, line) )
    {
      if ((line.size() > 0) && (line[0] != '#'))
      {
        inputData.push_back(line);
      }
    }
    fileFull.close();
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
    //*voronoi.CarrierSet = *voronoi.CarrierSet * (numberType::get(1,0)/numberType::get(0,1));
    voronoi.setCenter(origin);
    voronoi.construct();
    voronoi.filterSet();
    
    cellsFull.push_back(voronoi); 
  }
  
  std::cout << "cells full: " << cellsFull.size() << std::endl << std::flush; 
  
  cellsFull.sort();
  
  
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
    // check for zero density in the given window
    {
      windowType intersect = getWindow(winSize);
      for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
      {
        intersect.intersect((origin-*ot).star());
      }
      if (intersect.zeroDensity())
      {
        singularSizes.push_back(winSize);
        singularSizes.sort();
        singularSizes.unique();
        singularSizesUsed.push_back(winSize);
        singularSizesUsed.sort();
        singularSizesUsed.unique();
        //std::cout << "zero density --------------------------------------------------------------------------------------------------------------------------" << std::endl;
        continue;
      }
    }
    //check for zero density in already gathered windows
    {
      bool check = false;
      for (std::list<numberType>::iterator ut = singularSizes.begin(); ut != singularSizes.end(); ++ut)
      {
        windowType intersect = getWindow(*ut);
        for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
        {
          intersect.intersect((origin-*ot).star());
        }
        if (intersect.zeroDensity())
        {
          //std::cout << "present for duty" << std::endl;
          check = true;
          singularSizesUsed.push_back(*ut);
          singularSizesUsed.sort();
          singularSizesUsed.unique();
        }
      }
      if (check)
      {
        continue;
      }
    }
    
    // check whether tile starts in (1,beta]
    {
      windowType intersect = getWindow(numberType::get(1,0));
      for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
      {
        intersect.intersect((origin-*ot).star());
      }
      if (intersect.size() > numberType::get(0,0))
      {
        continue;
      }
    }
    
    //std::cout << std::endl << std::endl << std::endl << it->save() << std::endl;
    // omega_1
    numberType L = numberType::get(1,0);
    numberType R = numberType::get(0,1);
    
    numberType y3;
    numberType x3;
    numberType y2;
    numberType x2;
    numberType y1;
    numberType x1;
    numberType parts = numberType::get(40,0);
    numberType a;
    numberType d;
    numberType k;
    
    bool check;
    
    //do
    {
    
      int count = 0;
      
      for (numberType ut = numberType::get(0,0); ut <= parts; ut+= numberType::get(0,1).star().abs())
      {
        x1 = L+(R-L)*ut/parts;
        
        //std::cout << x1 << "\t" << std::flush;
        //print(std::cout, x1);
        //std::cout << std::endl;
        
        windowType intersect = getWindow(x1);
        
        for (std::list<Cpoint<numberType> >::iterator ot = it->CarrierSet->begin(); ot != it->CarrierSet->end(); ++ot)
        {
          intersect.intersect((origin-*ot).star());
        }
        
        //std::cout << x1 << "\t" << intersect.size() << std::endl;
        
        if (intersect.size() == numberType::get(0,0))
          continue;
        
        y1 = intersect.size();
        
        
        if (count > 4)
          break;
        
        x3 = x2;
        y3 = y2;
        x2 = x1;
        y2 = y1;
        
        ++count;
      }
      
      a = ((y1-y2)/(x1-x2)-(y1-y3)/(x1-x3))/(x2-x3);
      d = numberType::get(1,0,2)*((y1-y2)/((x1-x2)*a)-(x1+x2));
      k = y1-a*(x1+d)*(x1+d);
      
      parts+= numberType::get(1,0,3);
      //std::cout << parts << std::endl << std::flush;
      check = false;
      
      if (k == numberType::get(0,0))
      {
        //std::cout << "pass" << std::endl;
        //std::cout << "******************************* MAGIC ***************************************" << std::endl;
        //print(std::cout, d*numberType::get(-1,0));
        //std::cout << std::endl << d*numberType::get(-1,0) << std::endl;
        singularSizes.push_back(d*numberType::get(-1,0));
        singularSizes.sort();
        singularSizes.unique();
        singularSizesUsed.push_back(d*numberType::get(-1,0));
        singularSizesUsed.sort();
        singularSizesUsed.unique();
      }
      else
      {
        for (std::list<numberType>::iterator ot = singularSizes.begin(); ot != singularSizes.end(); ++ot)
        {
          if ((*ot+d)*(*ot+d)+(k/a)==numberType::get(0,0))
          {
            check = true;
            singularSizesUsed.push_back(*ot);
            singularSizesUsed.sort();
            singularSizesUsed.unique();
          }
        }
        if (!check)
        {
          std::cout << "TROUBLE!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << std::flush;
          std::cout << "sqrt(";
          print(std::cout, k*numberType::get(-1,0)/a);
          std::cout << ")+";
          print(std::cout, d*numberType::get(-1,0));
          std::cout << std::endl;
          std::cout << sqrt(-1.0*k/(1.0*a))-1.0*d << std::endl;
        }
        else
        {
          //std::cout << "pass" << std::endl;
        }
      }
    
    } //while ((k != numberType::get(0,0)) && !check);
    
    //it->colorify();
    //std::cout << it->getFillColor() << std::endl;
    //std::cout << "----------------------------------------------------------------------------" << std::endl;
  
  
    //std::cout << "omega_2 *****************************************************************" << std::endl;
    //// omega_2 **************************************************************************************************************************************
    
    //L = numberType::get(1,0);
    //R = numberType::get(0,1)*numberType::get(0,1);
    //parts = numberType::get(80,0);
    
    //a = numberType::get(0,0);
    //d = numberType::get(0,0);
    //k = numberType::get(0,0);
    ////do 
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
          ////std::cout << it->size() << " > " << ot->size() << std::endl;
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
        
        ////std::cout << x << "\t" << intersection.size() << "\t" << area << "\t" << intersection.size()-area << std::endl;
        
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
      //d = numberType::get(1,0,2)*((y1-y2)/((x1-x2)*a)-(x1+x2));
      //k = y1-a*(x1+d)*(x1+d);
      
      //std::cout << a << " ";
      //parts+= numberType::get(1,0);
      //print(std::cout, parts);
      //std::cout << std::endl << std::flush;
      
    //} //while (cplusd*cplusd-numberType::get(4,0)*ctimesd != numberType::get(0,0));
    
    
    
    
    
    //if (k == numberType::get(0,0))
    //{
      //singularSizes.push_back(d*numberType::get(-1,0));
      //singularSizes.sort();
      //singularSizes.unique();
    //}
    //else
    //{
      //bool check = false;
      //for (std::list<numberType>::iterator ot = singularSizes.begin(); ot != singularSizes.end(); ++ot)
      //{
        //if ((*ot+d)*(*ot+d)+(k/a)==numberType::get(0,0))
        //{
          //check = true;
        //}
      //}
      //if (!check)
      //{
        //std::cout << "TROUBLE!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << std::flush;
        //std::cout << "sqrt(";
        //print(std::cout, k*numberType::get(-1,0)/a);
        //std::cout << ")+";
        //print(std::cout, d*numberType::get(-1,0));
        //std::cout << std::endl;
        //std::cout << sqrt(-1.0*k/(1.0*a))-1.0*d << std::endl;
        //std::cout << "sqrt(";
        //print(std::cout, k*numberType::get(1,0)/a);
        //std::cout << ")+";
        //print(std::cout, d*numberType::get(-1,0));
        //std::cout << std::endl;
        //std::cout << sqrt(1.0*k/(1.0*a))-1.0*d << std::endl;
      //}
      //else
      //{
        //std::cout << "WE GOT IT :-D" << std::endl;
      //}
    //}
    
  }
  
  std::cout << "singular sizes size: " << singularSizes.size() << std::endl;
  
  std::ofstream output(singularFilename);
  
  for (std::list<numberType>::iterator it = singularSizes.begin(); it != singularSizes.end(); ++it)
  {
    print(output, *it);
    output << std::endl;
  }
  
  output.close();
  
  for (std::list<numberType>::iterator it = singularSizesUsed.begin(); it != singularSizesUsed.end(); ++it)
  {
    print(std::cout, *it);
    std::cout << std::endl;
    
  }
  
  
  delete circ;
  delete insc;
  
  return 0;
}
