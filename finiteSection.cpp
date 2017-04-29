#include "SUPPORT/window2.h"
#include "SUPPORT/generate3.h"
#include "SUPPORT/geometricObject2.h"
#include "SUPPORT/betaSet.h"
#include "SUPPORT/alphaSet.h"

#include "config.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <iterator> 
#include <iomanip>

#define _ERROR_
//#define _SVG_
#define _OUTPUT_

struct division
{
	numberType x;
	numberType y;
	
	numberType a1;
	numberType a2;
	
	numberType b;
	
	numberType a10;
	numberType a20;
	
	numberType b0;
	
	bool operator<(division const &other) { 
        return x < other.x;
    }
    
    bool operator==(division const &other) { 
        return ( (x == other.x) && (y == other.y) );
    }
};

int main( int argc, char ** argv )
{	
	int n = atoi( argv[1] );
	
	std::list<division> D;
	std::list<division> Db;
	
	division cache;
  
  //bool beta = true;
  //bool alpha = false;
	
  window<numberType> G0;
  window<numberType> G1;
  window<numberType> G2;
  window<numberType> G3;
  
  if (numberType::rotateN() == 12)
  {
    G0 = window<numberType>(numberType::get( 4, -1));
    G1 = window<numberType>(numberType::get(-7,  2));
    G2 = window<numberType>(numberType::get(-3,  1));
    G3 = window<numberType>(numberType::get( 1,  0));
  }
  if (numberType::rotateN() == 8)
  {
    G0 = window<numberType>(numberType::get(1, 0));
    G1 = window<numberType>(numberType::get(3,-1)*numberType::get(0,1));
    G2 = window<numberType>(numberType::get(0, 1));
  }
	
	cache.x  = G0.l();
	cache.y  = G1.l();
	cache.a1 = numberType::get(0,0);//G0.a();
	cache.a2 = G1.a();
	cache.b  = G1.b();
	cache.a10 = numberType::get(0,0);//G0.a();
	cache.a20 = G1.a();
	cache.b0  = G1.b();
	D.push_back(cache);
	
	cache.x  = G1.l();
	cache.y  = G2.l();
	cache.a1 = G1.a();
	cache.a2 = G2.a();
	cache.b  = G2.b();
	cache.a10 = G1.a();
	cache.a20 = G2.a();
	cache.b0  = G2.b();
	D.push_back(cache);
	
  if (numberType::rotateN() == 12)
  {
    cache.x  = G2.l();
    cache.y  = G3.l();
    cache.a1 = G2.a();
    cache.a2 = G3.a();
    cache.b  = G3.b();
    cache.a10 = G2.a();
    cache.a20 = G3.a();
    cache.b0  = G3.b();
    D.push_back(cache);
  }
	
	numberType u,v,w;
	
	Db = D;
	
#ifndef _SVG_
	std::cout << "<svg width=\"500\" height=\"500\" viewBox=\"-1 -13 14 14\">" << std::endl;
	//std::cout << "<rect x=\"-1\" y=\"-13\" width=\"100%\" height=\"100%\" fill=\"white\" />" << std::endl;
	std::cout << "<g transform=\"scale(1,-1)\">" << std::endl;
	std::cout << "<line x1=\"0em\" y1=\"0em\" x2=\"1em\" y2=\"0em\" stroke-width=\"0.005em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
	std::cout << "<line x1=\"1em\" y1=\"0em\" x2=\"1em\" y2=\"1em\" stroke-width=\"0.005em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
	std::cout << "<line x1=\"0em\" y1=\"0em\" x2=\"1em\" y2=\"1em\" stroke-width=\"0.005em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
	
	std::cout << std::endl;
	
		for ( std::list<division>::iterator it = D.begin(); it != D.end(); ++it )
		{	
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"0em\" x2=\"" << it->x << "em\" y2=\"" << it->x << "em\" stroke-width=\"0.0025em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
			std::cout << std::endl;
			
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->a10 << "em\" x2=\"" << it->y << "em\" y2=\"" << it->a20 << "em\" stroke-width=\"0.01em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->b0  << "em\" x2=\"" << it->y << "em\" y2=\"" << it->b0 << "em\" stroke-width=\"0.01em\" style=\"stroke:rgb(0,0,0);\" />" << std::endl;
			std::cout << std::endl;
		}
	std::cout << std::endl;
#endif
	
// SEARCH LOOP =========================================================
	// a ---------------------------------------------------------------
	for ( int o = 1; o != n; ++o )
	{
		std::list<division> Dnew;
		
		for ( std::list<division>::iterator it = D.begin(); it != D.end(); ++it )
		{
			if ( it->x == it->y )
				continue;
			
			numberType avgXY = ( it->x + it->y )/2;
			window<numberType> win( avgXY );
			numberType avgA = ( it->a1 + it->a2 )/2;
			
			u = win.step( avgA ) + ( it->x - it->y )/2;
			v = win.step( avgA ) - ( it->x - it->y )/2;
			
#ifndef _ERROR_
print( std::cout, avgA );
std::cout << std::endl;
//std::cout << win.Lchar() << " " << win.L() << std::endl;
//std::cout << win.Mchar() << " " << win.M() << std::endl;
//std::cout << win.Schar() << " " << win.S() << std::endl;

std::cout << u << " < " << it->b0 << " < " << v;
#endif

			if ( ( u < it->b0 ) && ( it->b0 < v ) ) 
			{

#ifndef _ERROR_
std::cout << " split" << std::endl;
#endif

				cache.x  = it->x;
				cache.y  = it->x + it->b0 - u;
				cache.a1 = u;
				cache.a2 = it->b0;
				cache.b  = it->b0;
				
				cache.a10 = it->a10;
				cache.a20 = it->a10 + it->b0 - u;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
				
				cache.x  = it->x + it->b0 - u;
				cache.y  = it->y;
				cache.a1 = it->b0;
				cache.a2 = v;
				cache.b  = it->b0;
				
				cache.a10 = it->a10 + it->b0 - u;
				cache.a20 = it->a20;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
			}
			else
			{

#ifndef _ERROR_
std::cout << std::endl;
#endif

				cache.x  = it->x;
				cache.y  = it->y;
				cache.a1 = u;
				cache.a2 = v;
				cache.b  = it->b0;
				
				cache.a10 = it->a10;
				cache.a20 = it->a20;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
			}
			
		}
		
		D = Dnew;
		//D.sort();
		
#ifndef _ERROR_
std::cout << std::endl;
#endif
		
#ifndef _SVG_
		for ( std::list<division>::iterator it = D.begin(); it != D.end(); ++it )
		{	
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->a1 << "em\" x2=\"" << it->y << "em\" y2=\"" << it->a2 << "em\" stroke-width=\"0.01em\" style=\"stroke:#F44336;stroke-opacity:" << static_cast<float>(o)/n << "\" />" << std::endl;
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->b0 << "em\" x2=\"" << it->y << "em\" y2=\"" << it->b0 << "em\" stroke-width=\"0.01em\" style=\"stroke:#03A9F4;stroke-opacity:" << static_cast<float>(o)/n << "\" />" << std::endl;
		}
#endif
	
	}
	
	Db = D;
	
	// b ---------------------------------------------------------------
	for ( int o = 1; o != n+1; ++o )
	{
		std::list<division> Dnew;
		
		for ( std::list<division>::iterator it = Db.begin(); it != Db.end(); ++it )
		{
			if ( it->x == it->y )
				continue;
			
			numberType avgXY = ( it->x + it->y )/2;
			window<numberType> win( avgXY );
			
			w = win.step( it->b );
			
#ifndef _ERROR_
print( std::cout, it->b );
std::cout << std::endl;
//std::cout << win.Lchar() << " " << win.L() << std::endl;
//std::cout << win.Mchar() << " " << win.M() << std::endl;
//std::cout << win.Schar() << " " << win.S() << std::endl;

std::cout << it->a10 << " < " << w << " < " << it->a20;
#endif

			if ( ( it->a10 < w ) && ( w < it->a20 ) ) 
			{

#ifndef _ERROR_
std::cout << " split" << std::endl;
#endif

				cache.x  = it->x;
				cache.y  = it->x + w - it->a10;
				cache.a1 = it->a10;
				cache.a2 = w;
				cache.b  = w;
				
				cache.a10 = it->a10;
				cache.a20 = w;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
				
				cache.x  = it->x + w - it->a10;
				cache.y  = it->y;
				cache.a1 = w;
				cache.a2 = it->a2;
				cache.b  = w;
				
				cache.a10 = w;
				cache.a20 = it->a20;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
#ifndef _ERROR_
std::cout << it->x << " " << it->b - v + it->y << " " << it->y << std::endl;
#endif
			}
			else
			{

#ifndef _ERROR_
std::cout << std::endl;
#endif

				cache.x  = it->x;
				cache.y  = it->y;
				cache.a1 = it->a1;
				cache.a2 = it->a2;
				cache.b  = w;
				
				cache.a10 = it->a10;
				cache.a20 = it->a20;
				cache.b0 = it->b0;
				Dnew.push_back(cache);
			}
			
		}
		
		Db = Dnew;
		
#ifndef _ERROR_
std::cout << std::endl;
#endif
		
#ifndef _SVG_
		for ( std::list<division>::iterator it = Db.begin(); it != Db.end(); ++it )
		{	
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->a10 << "em\" x2=\"" << it->y << "em\" y2=\"" << it->a20 << "em\" stroke-width=\"0.01em\" style=\"stroke:#4CAF50;stroke-opacity:" << static_cast<float>(o)/n << "\" />" << std::endl;
			std::cout << "<line x1=\"" << it->x << "em\" y1=\"" << it->b << "em\" x2=\"" << it->y << "em\" y2=\"" << it->b << "em\" stroke-width=\"0.01em\" style=\"stroke:#3F51B5;stroke-opacity:" << static_cast<float>(o)/n << "\" />" << std::endl;
		}
#endif
	
	}
	
	
	
#ifndef _SVG_
	std::cout << "</g></svg>" << std::endl;
#endif

#ifndef _OUTPUT_
	std::cout << "D dump: " << D.size() << std::endl;
	for ( std::list<division>::iterator it = D.begin(); it != D.end(); ++it )
	{
		//if ( it->x == it->y )
			//	continue;
		
		print( std::cout, it->x );
		std::cout << '\t';
		print( std::cout, it->y );
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for ( std::list<division>::iterator it = Db.begin(); it != Db.end(); ++it )
	{
		//if ( it->x == it->y )
			//	continue;
		
		print( std::cout, it->x );
		std::cout << '\t';
		print( std::cout, it->y );
		std::cout << std::endl;
	}
	std::cout << std::endl;
  for ( std::list<division>::iterator it = Db.begin(); it != Db.end(); ++it )
	{
		//if ( it->x == it->y )
			//	continue;
		
		print( std::cout, (it->x+it->y)*numberType::get(1,0,2) );
		std::cout << std::endl;
    print( std::cout, it->y );
    std::cout << std::endl;
	}
	std::cout << std::endl;
#endif

	return 0;
}
