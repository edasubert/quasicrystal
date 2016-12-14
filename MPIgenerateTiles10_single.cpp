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
#include <mpi.h>
#include <string>
#include <chrono>

#define MASTER 0        /* task ID of master task */

typedef betaSet numberType;
typedef rhombus windowType;

int main (int argc, char* argv[])
{
  // time measuring
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  
  int	taskid;	  
  int numtasks; 
  int nodeid;
  int nodes;
  int rc;
  
  std::string buffer;
  std::string send_buffer;
  int length;
  
  std::list<std::string> res;
  std::list<std::string> data;
  std::list<std::string>::iterator iterator;
  
  MPI_Status status;
  
  
  
  // initialize
  numberType winSize(1, 0, 2);
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  std::string clipTileStr = "(0+1*beta)/2,(-1+2*beta)/2 (0+0*beta)/2,(0+1*beta)/1 (-1+2*beta)/2,(0+1*beta)/2 (0-1*beta)/2,(-1+2*beta)/2 (0+1*beta)/1,(0+0*beta)/4 (1-2*beta)/2,(0+1*beta)/2 (0+0*beta)/4,(0+0*beta)/4 (-1+2*beta)/2,(0-1*beta)/2 (0-1*beta)/1,(0+0*beta)/4 (0+1*beta)/2,(1-2*beta)/2 (1-2*beta)/2,(0-1*beta)/2 (0+0*beta)/2,(0-1*beta)/1 (0-1*beta)/2,(1-2*beta)/2 ";
  
  windowType win( winSize );
  win.center( origin );
  
  // hyperquasicrystal
  //rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
  rhombus *circ = new rhombus(winSize*betaSet::get(4,0,3));
  
  
  // hypoquasicrystal
  //rhombus *insc = dynamic_cast<rhombus*> ( win.inscribed() );
  rhombus *insc = new rhombus(winSize*betaSet::get(2,0,3));
  
  betaSet S = circ->Xwindow().Small();
  betaSet L = insc->Xwindow().Large();
  
  betaSet coveringR = numberType::get(161, -43)*L;
  
  // size of rhumbus circumscribed to covering radius disc
  betaSet lengthToCover = numberType::get(8, 0)*coveringR;
  
  CvoronoiCell<numberType>::large = numberType::get(2, 0)*coveringR;
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  } while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  
  //wordLength = 10;
  
  /* Obtain number of tasks and task ID */
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  
  std::cout << std::string(4, ' ') << "MPI task " << taskid << " has started..." << std::endl;
  
  nodes = numtasks-1;
  nodeid = taskid-1;
  
  
  if (taskid != MASTER) // NODE ----------------------------------------
  {
    CvoronoiCell<numberType> clipTile;
    clipTile.load(clipTileStr);
    
    do 
    {
      MPI_Probe(MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      
      MPI_Recv(cache, length, MPI_CHAR, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      buffer = std::string(cache, length);
      delete [] cache;
      
      //std::cout << std::string(4, ' ') << taskid << ": received " << buffer << " status: " << status.MPI_TAG << std::endl;
      
      //std::cout << "  node " << taskid << " data received" << std::endl;
      
      if (status.MPI_TAG == 0)
      {
        
        
        // process data ------------------------------------------------
        std::string word1 = buffer.substr(0, buffer.length()/2);
        std::string word2 = buffer.substr(buffer.length()/2);
        CdeloneSet10<numberType> delone = quasicrystal2D10(circ->Xwindow(), word1, word2);
        
        delone << *clipTile.CarrierSet;
        
        delone.setPackingR();
        delone.setCoveringR(numberType::get(2, 0)*coveringR);
        delone.setDescription(word1+word2);
        delone.sortPotentialByDistance();
        
        delone.sortByDistance();
        
        std::list<CdeloneSet10<numberType> > delones;
        std::list<CvoronoiCell<numberType> > cells;
        delones.push_back(delone);
        
        for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
        {
          //std::cout << "SIZE POTENTIAL: " << it->sizePotential() << std::endl;
          //it->filterPotentialByWindow(win);
          
          CvoronoiCell<numberType> voronoi;
          
          *(voronoi.CarrierSet) = *it;
          voronoi.CarrierSet->sortByDistance();
          voronoi.CarrierSet->setPackingR();
          voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
          voronoi.setCenter(origin);
          voronoi.construct();
          voronoi.filterSet();
          
          std::list<Cpoint<numberType> > potential;
          potential = it->getPotential();
          voronoi.filterSetPotential( &potential );
          it->clearPotential();
          it->addPotential(potential);
          
          std::cout << "SIZE POTENTIAL: " << it->sizePotential() << '\t' << "SIZE DELONES: " << delones.size() << std::endl;
          
          // deal with potential
          while (it->isPotential()) 
          {
            Cpoint<numberType> cache = it->popPotential();
            
            delone = *it;
            delone << cache;
            delones.push_back(delone);
          }
          
          
          cells.push_back( voronoi );
          
          delones.erase(it);
          
          delones.sort();
          delones.unique();
          
          //std::cout << "  node " << taskid << " delones: " << delones.size() << std::endl;
        }
        
        cells.sort();
        cells.unique();
        
        std::list<std::string> list;
        
        for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
        {
          list.push_back(it->save());
        }
        
        list.sort();
        list.unique();
        // end ---------------------------------------------------------
        
        
        std::cout << "  node " << taskid << " sending back: " << list.size() << std::endl;
        
        for (std::list<std::string>::iterator it = list.begin(); it != --list.end(); ++it)
        {
          send_buffer = *it;
          // return result
          MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
        }
        
        send_buffer = *list.rbegin();
        // return result
        MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, MASTER, 1, MPI_COMM_WORLD);
      }
    } while (status.MPI_TAG == 0); 
  }
  else // MASTER -------------------------------------------------------
  {
    // create data -----------------------------------------------------
    std::cout << "Load data" << std::endl;
    
    std::list<std::string> lang = language( insc->Xwindow(), circ->Xwindow(), wordLength );
    for ( std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it )
    {
      for ( std::list<std::string>::iterator ot = lang.begin(); ot != lang.end(); ++ot )
      {
        data.push_back(*it+*ot);
      }
    }
    
    std::cout << "tasks: " << data.size() << std::endl;
    std::cout << "word length: " << wordLength << std::endl;
    
    iterator = data.begin();
    
    // send initial load of data
    std::cout << "Send initial load of data" << std::endl;
    for (int it = 0; it < nodes; ++it)
    {
      send_buffer = *(iterator++);
      
      //std::cout << std::string(4, ' ') << "MASTER sending: " << iterator << "/168" << std::endl;
      
      MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, it+1, 0, MPI_COMM_WORLD);
    }
    
    // gather data while processing
    std::cout << "distribute the rest on demand" << std::endl;
    while (iterator != data.end())
    {
      std::cout << "waiting" << std::endl;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      buffer = std::string(cache, length);
      delete [] cache;
      
      // save result
      res.push_back(buffer);
      std::cout << "finished: " << res.size() << std::endl;
      
      while (status.MPI_TAG == 0)
      {
        MPI_Probe(status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
        MPI_Get_count(&status, MPI_CHAR, &length);
        char *cache = new char[length];
        MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        buffer = std::string(cache, length);
        delete [] cache;
        
        // save result
        res.push_back(buffer);
        std::cout << "finished: " << res.size() << std::endl;
      }
      
      //std::cout << std::string(4, ' ') << "MASTER received from: " << status.MPI_SOURCE << " sending: " << iterator << std::endl;
      
      send_buffer = *(iterator++);
      
      MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
    
    // terminate processes
    std::cout << "termination" << std::endl;
    for (int it = 0; it < nodes; ++it)
    {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      buffer = std::string(cache, length);
      delete [] cache;
      
      // save result
      res.push_back(buffer);
      std::cout << "finished: " << res.size() << std::endl;
      
      //std::cout << std::string(4, ' ') << "MASTER received from: " << status.MPI_SOURCE << " - " << buffer << std::endl;
      
      send_buffer = std::to_string(0);
      
      rc = MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, it+1, 1, MPI_COMM_WORLD);
    }
    
    
    // write to file
    std::ofstream output(argv[1]);
    res.sort();
    res.unique();
    for (std::list<std::string>::iterator it = res.begin(); it != res.end(); ++it)
    {
      output << *it << std::endl;
    }
    output.close();
  }
  
  std::cout << std::string(4, ' ') << "... MPI task " << taskid << " is closing" << std::endl;
  MPI_Finalize();
  
  // time measuring
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  std::cout << "microseconds: " << duration << " (" << std::round(duration/100000.0)/10.0 << "s)" << std::endl;
  
  return 0;
}
