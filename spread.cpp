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



// creates seed files for spreading
int main( int argc, char ** argv )
{
  // INPUT
  numberType const_winSize;
  numberType covering;
  numberType largestTile;
  std::string filenameSuffix;
  std::string filenamePrefix;
  std::string filenameFinite;
  std::string filenameMore;
  
  std::cin >> const_winSize;
  std::cin >> covering;
  std::cin >> largestTile;
  std::cin >> filenamePrefix;
  
  filenameSuffix = "_concat";
  
  
  std::string fileName = argv[1];
  
  windowType win = getWindow(numberType::get(1,0));
  
  int steps = 200;
  
  for (int i = 0; i != steps+1; ++i)
  {
    numberType winSize = numberType::get(1,0,1)/numberType::get(0,1,1) + (numberType::get(1,0,1)-numberType::get(1,0,1)/numberType::get(0,1,1))*numberType::get(i,0,steps);
    
    // write to file properties
    std::ostringstream tmp;
    tmp << fileName << "_" << win.getName() << "_" << winSize << "_(";
    printFile(tmp, winSize);
    tmp << ")";
    
    std::ofstream output(tmp.str().c_str());
    
    print(output, winSize);
    output << std::endl;
    print(output, covering);
    output << std::endl;
    print(output, largestTile);
    output << std::endl;
    output << filenamePrefix << std::endl;
    output << filenameSuffix << std::endl;
    
    output.close();
  }
  
  
  
  return 0;
}
