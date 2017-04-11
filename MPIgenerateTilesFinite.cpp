#define _ERROR_
#define _IMG_

#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"
#include "SUPPORT/geometricObject2_no11.h"
#include "SUPPORT/delone10.h"
#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"


#include "config.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <list>
#include <mpi.h>
#include <string>
//#include <chrono>

#define MASTER 0        /* task ID of master task */

int main (int argc, char* argv[])
{
  
  // time measuring
  //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  
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
  numberType winSize = const_winSize;
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  
  windowType win = getWindow(winSize);
  win.center( origin );
  
  // hyperquasicrystal
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  circ->center(origin);
  
  numberType S = circ->Xwindow().Small();
  
  // size of rhumbus circumscribed to covering radius disc
  //numberType lengthToCover = numberType::get(8, 0)*coveringR;
  numberType lengthToCover = numberType::circumscribedRhombusToCircle()*numberType::get(2, 0)*covering;
  
  numberType coveringR = covering;
  
  CvoronoiCell<numberType>::large = covering;
  
  // find out the word length by testing
  int wordLength = 1;
  do
  {
    ++wordLength;
  } while ( minWord(language(circ->Xwindow(), wordLength), circ->Xwindow()) < lengthToCover );
  --wordLength;
  //wordLength = 4;
  
  /* Obtain number of tasks and task ID */
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  
  std::cout << std::string(4, ' ') << "MPI task " << taskid << " has started..." << std::endl;
  
  nodes = numtasks-1;
  nodeid = taskid-1;
  
  
  if (taskid != MASTER) // NODE ----------------------------------------
  {
    // input finite
    std::string line;
    
    std::cout << "Loading \"finite\" data" << std::endl;

    std::list<CdeloneSet<numberType> > delonesFinite;

    std::ifstream myfileFinite(argv[2]);

    std::list<std::string> inputData; 

    if (myfileFinite.is_open())
    {
      while ( getline(myfileFinite, line) )
      {
        if ((line.size() > 0) && (line[0] != '#'))
        {
          inputData.push_back(line);
        }
      }
      myfileFinite.close();
    }
    else std::cout << "Unable to open file" << std::endl; 

    std::cout << "strings read: " << inputData.size() << std::endl << std::flush; 

    inputData.sort();
    inputData.unique();

    for (std::list<std::string>::iterator it = inputData.begin(); it != inputData.end(); ++it)
    {
      CvoronoiCell<numberType> voronoi;
      voronoi.load(*it);
      
      if (voronoi.CarrierSet->size() < 3)
      {
        continue;
      }
      
      delonesFinite.push_back(*(voronoi.CarrierSet));
    }

    std::cout << "finite cells: " << delonesFinite.size() << std::endl << std::flush; 

    delonesFinite.sort();

    
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
        
        CdeloneSet<numberType> tmp = quasicrystal2D(circ->Xwindow(), word1, word2);
        
        //std::cout << "full size: " << tmp.size() << std::endl;
        
        std::list<CdeloneSet10<numberType> > delones;
        std::list<CvoronoiCell<numberType> > cells;
        
        // append finite delones by potential
        for (std::list<CdeloneSet<numberType> >::iterator it = delonesFinite.begin(); it != delonesFinite.end(); ++it)
        {
          // test whether finite's carrier set is among potential
          CdeloneSet<numberType> testSet;
          testSet << *it;
          testSet << tmp;
          
          if (testSet.size() > tmp.size())
          {
            continue;
          }
          
          CdeloneSet10<numberType> delone;
          
          delone << *it;
          delone.addPotential(tmp.getPoints());
          
          delone.setPackingR();
          delone.setCoveringR(coveringR);
          
          delone.sortPotentialByDistance();
          delone.sortByDistance();
          
          delone.filterDistanceOrigin(numberType::get(2, 0)*coveringR);
          delone.removeOrigin();
          
          delone.filterPotentialByWindow(win);
          
          CvoronoiCell<numberType> voronoi;
            
          *(voronoi.CarrierSet) = delone;
          
          voronoi.CarrierSet->sort();
          voronoi.CarrierSet->unique();
          
          voronoi.CarrierSet->sortByDistance();
          voronoi.CarrierSet->setPackingR();
          voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
          voronoi.setCenter(origin);
          voronoi.construct();
          voronoi.filterSet();
          
          //std::cout << delone.sizePotential() << "\t";
          std::list<Cpoint<numberType> > potential;
          potential = delone.getPotential();
          
          voronoi.filterSetPotential(&potential);
          delone.clearPotential();
          delone.addPotential(potential);
          //std::cout << delone.sizePotential() << std::endl;
          
          delones.push_back(delone);
          
          
    int count = 100*taskid;
    for ( std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); ++it )
    {
      ++count;
      
      it->setColor("#000000", "#000000", "0.1");
      
      std::ostringstream oss;
      oss << "output/tile/tile" << std::setfill('0') << std::setw(3) << count << ".svg";// << " " << it->size();
      std::ofstream myfile ( oss.str().c_str() );
      
      myfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n" << std::endl;
      myfile << "<svg width=\"3000\" height=\"3000\" viewBox=\"" << -30/5 << " " << -30/5 << " " << 60/5 << " " << 60/5 << "\">\n" << std::endl;
      
      it->svg(myfile);
      
      myfile << "</svg>";
      
      myfile.close();
    }
          
          
        }
        
        for (std::list<CdeloneSet10<numberType> >::iterator it = delones.begin(); it != delones.end(); it = delones.begin())
        {
          //std::cout << "SIZE POTENTIAL: " << it->sizePotential() << std::endl;
          while (it->isPotential()) 
          {
            Cpoint<numberType> cache = it->popPotential();
            
            CdeloneSet10<numberType> delone;
            
            delone = *it;
            delone << cache;
            delone.sortPotentialByDistance();
            delone.filterPotentialByWindow(win);
            
            CvoronoiCell<numberType> voronoi;
            
            *(voronoi.CarrierSet) = delone;
            
            voronoi.CarrierSet->sort();
            voronoi.CarrierSet->unique();
            
            voronoi.CarrierSet->sortByDistance();
            voronoi.CarrierSet->setPackingR();
            voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
            voronoi.setCenter(origin);
            voronoi.construct();
            voronoi.filterSet();
            
            //std::cout << delone.sizePotential() << "\t";
            std::list<Cpoint<numberType> > potential;
            potential = delone.getPotential();
            
            voronoi.filterSetPotential(&potential);
            delone.clearPotential();
            delone.addPotential(potential);
            //std::cout << delone.sizePotential() << std::endl;
            
            {
              cells.push_back(voronoi);
              //std::cout << "tile!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            }
          
            delones.push_back(delone);
          }
          
          delones.erase(it);
          
          delones.sort();
          delones.unique();
          
          cells.sort();
          cells.unique();
          
          //std::cout << "  node " << taskid << " delones: " << delones.size() << '\t' << "cells: " << cells.size() << std::endl << std::flush;
        }
        
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
      
          MPI_Send(&send_buffer[0], send_buffer.length(), MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
        }
        
        send_buffer = *list.rbegin();
      
        MPI_Send(&send_buffer[0], send_buffer.length(), MPI_CHAR, MASTER, 1, MPI_COMM_WORLD);
      }
    } while (status.MPI_TAG == 0); 
  }
  else // MASTER -------------------------------------------------------
  {
    // setup
    std::ostringstream convert;
    convert << argv[1] << "_" << win.getName() << "_" << winSize << "_(";
    printFile(convert, winSize);
    convert << ")_" << res.size();
    
    std::string filename = convert.str().c_str();
    
    {
      std::ofstream output(filename.c_str());
      print(output, winSize);
      output << std::endl;
      output.close();
    }
    
    // create data -----------------------------------------------------
    std::cout << "Load data" << std::endl;
    
    std::list<std::string> lang = language(circ->Xwindow(), wordLength);
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
    
    int count = 0;
    // send initial load of data
    for (int it = 0; it < nodes; ++it)
    {
      send_buffer = *(iterator++);
      
      MPI_Send(&send_buffer[0], send_buffer.length(), MPI_CHAR, it+1, 0, MPI_COMM_WORLD);
      count++;
    }
    
    // gather data while processing
    int count_print;
    std::cout << "distribute the rest on demand" << std::endl;
    while (iterator != data.end())
    {
      //std::cout << "waiting" << std::endl;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      MPI_Get_count(&status, MPI_CHAR, &length);
      char *cache = new char[length];
      MPI_Recv(cache, length, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      buffer = std::string(cache, length);
      delete [] cache;
      
      // save result
      res.push_back(buffer);
      //std::cout << "finished: " << res.size() << std::endl;
      
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
        //std::cout << "finished: " << res.size() << std::endl;
      }
      
      std::cout << std::string(4, ' ') << "MASTER received from: " << status.MPI_SOURCE << std::endl;
      
      send_buffer = *(iterator++);
      
      MPI_Send(&send_buffer[0], send_buffer.length(), MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
      
      count++;
      count_print++;
      //if (count_print > data.size()/100.)
      {
        count_print = 0;
        std::cout << "processed " << count << "/" << data.size() << " <=> " << 100*count/data.size() << "%" << std::endl;
      }
      
      res.sort();
      res.unique();
      
      // write to file
      std::ofstream output(filename.c_str(), std::ios::app);
      
      for (std::list<std::string>::iterator it = res.begin(); it != res.end(); ++it)
      {
        output << *it << std::endl;
      }
      output.close();
      
      res.clear();
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
      //std::cout << "finished: " << res.size() << std::endl;
      
      //std::cout << std::string(4, ' ') << "MASTER received from: " << status.MPI_SOURCE << " - " << buffer << std::endl;
      
      send_buffer = "0";
      
      rc = MPI_Send(&send_buffer[0], send_buffer.length(), MPI_CHAR, it+1, 1, MPI_COMM_WORLD);
    }
    
    res.sort();
    res.unique();
    
    // write to file
    std::ofstream output(filename.c_str(), std::ios::app);
    
    for (std::list<std::string>::iterator it = res.begin(); it != res.end(); ++it)
    {
      output << *it << std::endl;
    }
    output.close();
  }
  
  std::cout << std::string(4, ' ') << "... MPI task " << taskid << " is closing" << std::endl;
  MPI_Finalize();
  
  // time measuring
  //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  //auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  //std::cout << "microseconds: " << duration << " (" << std::round(duration/100000.0)/10.0 << "s)" << std::endl;
  
  return 0;
}
