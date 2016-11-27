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

#define LIMIT 2

typedef betaSet numberType;
typedef circle windowType;

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
  numberType winSize(-10, 3, 2);
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
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  } while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  
  wordLength = 6;
  
  
  /* Obtain number of tasks and task ID */
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  
  std::cout << std::string(4, ' ') << "MPI task " << taskid << " has started..." << std::endl;
  
  nodes = numtasks-1;
  nodeid = taskid-1;
  
  
  if (taskid != MASTER) // NODE ----------------------------------------
  {
    do 
    {
      data.clear();
      
      MPI_Probe(MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      
      MPI_Recv(cache, length, MPI_CHAR, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      data.push_back( std::string(cache, length) );
      delete [] cache;
      
      while (status.MPI_TAG == 0)
      {
        MPI_Probe(MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &length);
        char *cache = new char[length];
        
        MPI_Recv(cache, length, MPI_CHAR, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        data.push_back( std::string(cache, length) );
        delete [] cache;
      }
      
      std::cout << "  node " << taskid << " data received" << std::endl;
      
      std::list<std::string> list;
      if (status.MPI_TAG == 1)
      {
        for (std::list<std::string>::iterator it = data.begin(); it != data.end(); ++it)
        {
          // process data ------------------------------------------------
          std::string word1 = it->substr(0, it->length()/2);
          std::string word2 = it->substr(it->length()/2);
          CdeloneSet10<numberType> delone = quasicrystal2D10(circ->Xwindow(), word1, word2);
          
          delone.setPackingR();
          delone.setCoveringR(numberType::get(2, 0)*coveringR);
          delone.setDescription(word1+word2);
          
          delone.sortByDistance();
          
          std::list<CdeloneSet10<numberType> > delones;
          std::list<CvoronoiCell<numberType> > cells;
          delones.push_back(delone);
          
          for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
          {
            it->filterPotentialByWindow(win);
            
            while (it->isPotential()) 
            {
              Cpoint<numberType> cache = it->popPotential();
              
              delone = *it;
              delone << cache;
              delones.push_back(delone);
            }
            
            CvoronoiCell<numberType> voronoi;
            
            *(voronoi.CarrierSet) = *it;
            voronoi.CarrierSet->sortByDistance();
            voronoi.CarrierSet->setPackingR();
            voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
            voronoi.setCenter(origin);
            voronoi.construct();
            voronoi.filterSet();
            
            cells.push_back( voronoi );
            
            delones.erase(it);
            
            delones.sort();
            delones.unique();
          }
          
          cells.sort();
          cells.unique();
          
          for (std::list<CvoronoiCell<numberType> >::iterator it = cells.begin(); it != cells.end(); ++it)
          {
            list.push_back(it->save());
          }
          
          list.sort();
          list.unique();
          // end ---------------------------------------------------------
        }
        std::cout << "  node " << taskid << " sending back" << std::endl;
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
    } while (status.MPI_TAG != 2); 
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
      // send data
      for ( int ot = 0; ot < LIMIT-1; ++ot)
      {
        send_buffer = *(iterator++);
        MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, it+1, 0, MPI_COMM_WORLD);
      }
      send_buffer = *(iterator++);
      MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, it+1, 1, MPI_COMM_WORLD);
    }
    
    // gather data while processing
    std::cout << "distribute the rest on demand" << std::endl;
    while (iterator != data.end())
    {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      buffer = std::string(cache, length);
      delete [] cache;
      
      // save result
      //res.push_back(buffer);
      
      while (status.MPI_TAG == 0)
      {
        MPI_Probe(status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
        MPI_Get_count(&status, MPI_CHAR, &length);
        char *cache = new char[length];
        MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        buffer = std::string(cache, length);
        delete [] cache;
        
        // save result
        //res.push_back(buffer);
      }
      
      // send more data
      for ( int ot = 0; ot < LIMIT-1; ++ot)
      {
        if (iterator != --data.end())
        {
          send_buffer = *(iterator++);
          MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        }
      }
      if (iterator != data.end())
      {
        send_buffer = *(iterator++);
        MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
      }
      
      std::cout << "processed: " << res.size() << std::endl;
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
      
      //std::cout << std::string(4, ' ') << "MASTER received from: " << status.MPI_SOURCE << " - " << buffer << std::endl;
      
      send_buffer = std::to_string(0);
      
      rc = MPI_Send(send_buffer.c_str(), send_buffer.length(), MPI_CHAR, it+1, 2, MPI_COMM_WORLD);
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
