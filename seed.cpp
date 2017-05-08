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
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>



// creates seed files for generateTilesFromQuasi
int main( int argc, char ** argv )
{
  numberType input;
  
  // singular file
  std::string singularFilename = "closed/singular";
  std::ifstream singularFile(singularFilename);
  std::list<numberType> singularSizes;
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
  
  windowType win = getWindow(numberType::get(1,0));
  
  std::string fileName = argv[1];
  
  int steps = 20;
  
  
  
  for (std::list<numberType>::iterator it = ++singularSizes.begin(), itold = singularSizes.begin(); it != singularSizes.end(); ++it, ++itold)
  //for (std::list<numberType>::iterator it = singularSizes.begin(); it != singularSizes.end(); ++it)
  //for (numberType it = numberType::get(1,0); it <= numberType::get(0,1); it+= (numberType::get(0,1)-numberType::get(1,0))/numberType::get(10,0))
  {
    //numberType winSize = numberType::get(-1,2,3);//numberType::get(1,0,1)/numberType::get(0,1,1) + (numberType::get(1,0,1)-numberType::get(1,0,1)/numberType::get(0,1,1))*numberType::get(i,0,steps);
    numberType winSize = (*it+*itold)/numberType::get(2,0);
    //numberType winSize = *it;
    //numberType winSize = it;
    numberType placeholder(0, 0);
    
    // write to file properties
    std::ostringstream tmp;
    tmp << fileName << "_" << win.getName() << "_" << winSize << "_(";
    printFile(tmp, winSize);
    tmp << ")";
    
    std::ofstream output(tmp.str().c_str());
    
    print(output, winSize);
    output << std::endl;
    print(output, placeholder);
    output << std::endl;
    print(output, placeholder);
    output << std::endl;
    
    output.close();
  }
  
  
  
  return 0;
}
