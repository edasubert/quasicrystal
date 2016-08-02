#include "generate2.h"
#include "geometricObject2.h"
#include "betaSet.h"
#include "window.h"

#include <iostream>
#include <map>
#include <iterator>

#define _ERROR_
#define _IMG_

std::list<betaSet> quasicrystal1D( window win, betaSet x, betaSet y, betaSet start ) // start for legacy reason
{
  #ifndef _ERROR_
  std::cout << "Quasicrystal1D" << std::endl;
  #endif
  
  start = win.seed();
  
  while ( start > x ) // get before x
  {
    start = win.stepBack( start.star() ).star();
  }
  
  while ( start < x ) // get just after x (first point of quasicrystal)
  {
    start = win.step( start.star() ).star();
  }
  
  std::list<betaSet> quasicrystal;
  
  while ( start < y )
  {
    quasicrystal.push_back( start );
    start = win.step( start.star() ).star();
  }
  
  return quasicrystal;
}

CdeloneSet<betaSet> quasicrystal2D( std::list<betaSet> &x, std::list<betaSet> &y )
{
  CdeloneSet<betaSet> delone;
  delone.setDescription("2D quasicrystal parallelogram");
  //delone.setColor( fillColor, strokeColor, strokeWidth );
  
  for ( std::list<betaSet>::iterator it = x.begin(); it != x.end(); ++it )
  {
    for ( std::list<betaSet>::iterator ot = y.begin(); ot != y.end(); ++ot )
    {
      delone << transformBeta( Cpoint<betaSet>( *it, *ot ) );
    }
  }
  
  return delone;
}

CdeloneSet<betaSet> quasicrystal2D( window win, std::string &x, std::string &y )
{
  CdeloneSet<betaSet> delone;
  Cpoint<betaSet> point;
  delone.setDescription("2D quasicrystal parallelogram with 1 0 marks");
  
  betaSet xAxisMove(0,0);
  std::string::iterator it = x.begin();
  
  for (int i = 0; i < ceil(x.size()/2); ++i)
  {
    xAxisMove-= win.char2space(*(it++++));
  }
  
  betaSet yAxisMove(0,0);
  it = y.begin();
  
  for (int i = 0; i < ceil(y.size()/2); ++i)
  {
    yAxisMove-= win.char2space(*(it++++));
  }
  
  betaSet xAxis = xAxisMove;
  betaSet yAxis = yAxisMove;
  
  for ( std::string::iterator it = x.begin(); it != x.end(); ++++it )
  {
    xAxis+= win.char2space(*it);
    for ( std::string::iterator ot = y.begin(); ot != y.end(); ++++ot )
    {
      yAxis+= win.char2space(*ot);
      point.setX(xAxis);
      point.setY(yAxis);
      point.setDescription("0");
      if (*(std::next(it)) == '1' && *(std::next(ot)) == '1')
      {
        point.setDescription("1");
      }
      delone << transformBeta( point );
    }
    yAxis = yAxisMove;
  }
  
  return delone;
}

CdeloneSet10<betaSet> quasicrystal2D10( window win, std::string &x, std::string &y )
{
  CdeloneSet10<betaSet> delone;
  Cpoint<betaSet> point;
  delone.setDescription("2D quasicrystal parallelogram with 1 0 separation");
  
  betaSet xAxisMove(0,0);
  std::string::iterator it = x.begin();
  
  for (int i = 0; i < ceil(x.size()/2); ++i)
  {
    xAxisMove-= win.char2space(*(it++++));
  }
  
  betaSet yAxisMove(0,0);
  it = y.begin();
  
  for (int i = 0; i < ceil(y.size()/2); ++i)
  {
    yAxisMove-= win.char2space(*(it++++));
  }
  
  betaSet xAxis = xAxisMove;
  betaSet yAxis = yAxisMove;
  
  for ( std::string::iterator it = x.begin(); it != x.end(); ++++it )
  {
    xAxis+= win.char2space(*it);
    for ( std::string::iterator ot = y.begin(); ot != y.end(); ++++ot )
    {
      yAxis+= win.char2space(*ot);
      point.setX(xAxis);
      point.setY(yAxis);
      point.setDescription("0");
      if (*(std::next(it)) == '1' && *(std::next(ot)) == '1')
      {
        delone << transformBeta(point);
      }
      else
      {
        delone.addPotential(transformBeta(point));
      }
    }
    yAxis = yAxisMove;
  }
  
  return delone;
}


CdeloneSet<betaSet> quasicrystal2D( window2D &win, betaSet x1, betaSet x2, betaSet y1, betaSet y2 )
{
  rhombus *circ = dynamic_cast<rhombus*> ( win.circumscribed() );
  
  std::list<betaSet> Xquasi = quasicrystal1D( circ->Xwindow(), betaSet::get(-1,1)*x1, betaSet::get(-2,1)*x2 + (x2-x1), betaSet::get(0, 0) );
  std::list<betaSet> Yquasi = quasicrystal1D( circ->Ywindow(), 2*y1, 2*y2, betaSet::get(0, 0) );
  
  CdeloneSet<betaSet> delone;
  for ( std::list<betaSet>::iterator it = Xquasi.begin(); it != Xquasi.end(); ++it )
  {
    for ( std::list<betaSet>::iterator ot = Yquasi.begin(); ot != Yquasi.end(); ++ot )
    {
      Cpoint<betaSet> candidate( *it, *ot );
      candidate = transformBeta( candidate );
      
      if ( ( candidate.getX() >= x1 ) && ( candidate.getX() <= x2 ) && ( candidate.getY() >= y1 ) && ( candidate.getY() <= y2 ) )
      {
        if ( win.in( candidate.star() ) )
        {
          delone << ( candidate );
        }
      }
    }
  }
  delete circ;
  
  return delone;
}



Cpoint<betaSet> transformBeta( const Cpoint<betaSet> &point ) //transforms point in alpha base to real base
{
  Cpoint<betaSet> Output( point );
  Output.setX(point.getX() - (point.getY()*betaSet::get(-2,1,2)));
  Output.setY(point.getY()*betaSet::get(1,0,2));
  return Output;
}

// Language generator
std::list<std::string> language( window win, int n )
{
  std::list<betaSet> delimiter;
  std::map <betaSet, std::string> language;
  
  delimiter.push_back( win.c() );
  delimiter.push_back( win.a() );
  delimiter.push_back( win.b() );
  delimiter.push_back( win.d() );
  
  language[win.c()] = win.Schar();
  language[win.a()] = win.Lchar();
  language[win.b()] = win.Mchar();
  
  #ifndef _IMG_
  std::cout << "<svg height=\"" << 100*(n+2) << "\" width=\"" << 100*(win.l()) << "\" viewBox=\"0 -1 " << win.l() << " " << n+2 << "\">" << std::endl;
  #endif
  
  for ( int i = 2; i <= n; ++i )
  {
    #ifndef _ERROR_
    std::cout << i << ": " << std::endl;
    for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      std::cout << language[ *it ] << '\t';
    }
    std::cout << std::endl;
    #endif
    std::list<betaSet> new_delimiter;
    std::map <betaSet, std::string> new_language;
    
    for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      betaSet image = win.step( *it );
      betaSet length = *std::next( it, 1 ) - *it;
      std::string word = language[*it];
      
      #ifndef _IMG_
      std::cout << "<polygon points=\"" << *it << "," << i-2 << " " << image << "," << i-1 << " " << image + length << "," << i-1 << " " << *it + length << "," << i-2 << " " <<  "\" fill=\"" << "rgb(255,0,0)" << "\" stroke=\"" << "rgb(30,30,30)" << "\" stroke-width=\"0\" opacity=\"0.3\" />" << std::endl;
      std::cout << "<line x1=\"" << *it << "\" y1=\"" << i-2 << "\" x2=\"" << image << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,255,0)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      std::cout << "<line x1=\"" << *it + length << "\" y1=\"" << i-2 << "\" x2=\"" << image + length << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,0,255)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      
      std::cout << "<line x1=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y1=\"" << i-2 + betaSet::get(0,1,17) + *it*betaSet::get(1,0,5) << "\" x2=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y2=\"" << i-2 + betaSet::get(0,1,17) << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.0003 << "\" opacity=\"1\" />" << std::endl;
      std::cout << "<text x=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y=\"" << i-2 + betaSet::get(0,1,17) + *it*betaSet::get(1,0,5) << "\" fill=\"black\" style=\"font-family:Arial;font-size:0.005\">" << word << "</text>" << std::endl;
      #endif
      
      if ( ( image < win.a() ) && ( win.b() < image + length ) )
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Schar();
        
        new_delimiter.push_back( win.a() );
        new_language[win.a()] = word + win.Lchar();
        
        new_delimiter.push_back( win.b() );
        new_language[win.b()] = word + win.Mchar();
      }
      else if ( ( image < win.b() ) && ( win.b() < image + length ) )
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Lchar();
        
        new_delimiter.push_back( win.b() );
        new_language[win.b()] = word + win.Mchar();
      }
      else if ( ( image < win.a() ) && ( win.a() < image + length ) )
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Schar();
        
        new_delimiter.push_back( win.a() );
        new_language[win.a()] = word + win.Lchar();
      }
      else if ( ( image + length <= win.a() ) )
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Schar();
      }
      else if ( ( image + length <= win.b() ) )
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Lchar();
      }
      else
      {
        new_delimiter.push_back( image );
        new_language[image] = word + win.Mchar();
      }
    }
    
    new_delimiter.push_back( *delimiter.rbegin() );
    
    delimiter = new_delimiter;
    language = new_language;
    delimiter.sort();
    delimiter.unique();
    
    #ifndef _IMG_
    std::cout << "<line x1=\"0\" y1=\"" << n-1 << "\" x2=\"" << win.l() << "\" y2=\"" << n-1 << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.01 << "\" />" << std::endl;
    std::cout << "<circle cx=\"" << win.a() << "\" cy=\"" << n-1 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<circle cx=\"" << win.b() << "\" cy=\"" << n-1 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<line x1=\"0\" y1=\"" << i - 2 << "\" x2=\"" << win.l() << "\" y2=\"" << i - 2 << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.01 << "\" />" << std::endl;
    std::cout << "<circle cx=\"" << win.a() << "\" cy=\"" << i-2 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<circle cx=\"" << win.b() << "\" cy=\"" << i-2 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    #endif
  }
  
  #ifndef _ERROR_
  std::cout << n << ": " << std::endl;
  for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    std::cout << language[ *it ] << '\t';
  }
  std::cout << std::endl;
  #endif
  
  #ifndef _IMG_
  std::cout << "</svg>" << std::endl;
  #endif
  
  std::list<std::string> languageList;
  
  for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    languageList.push_back( language[ *it ] );
  }
  
  return languageList;
}


// Language generator for hypercrystal and hypocrystal -------------------------------------------------------------------------------------------------------

std::list<std::string> language( window hypowin, window hyperwin, int n )
{
  std::list<betaSet> delimiter;
  std::map <betaSet, std::string> language;
  
  std::string tmp(1, hyperwin.Schar());
  addSplitForHypowin( hypowin, hyperwin.c(), hyperwin.a() - hyperwin.c(), tmp, &delimiter, &language );
  tmp.assign(1, hyperwin.Lchar());
  addSplitForHypowin( hypowin, hyperwin.a(), hyperwin.b() - hyperwin.a(), tmp, &delimiter, &language );
  tmp.assign(1, hyperwin.Mchar());
  addSplitForHypowin( hypowin, hyperwin.b(), hyperwin.d() - hyperwin.b(), tmp, &delimiter, &language );
  delimiter.push_back( hyperwin.d() );
  
  #ifndef _IMG_
  std::cout << "<svg height=\"" << 100*(n+2) << "\" width=\"" << 100*(hyperwin.l()) << "\" viewBox=\"0 -1 " << hyperwin.l() << " " << n+2 << "\">" << std::endl;
  #endif
  
  for ( int i = 2; i <= n; ++i )
  {
    #ifndef _ERROR_
    std::cout << i << ": " << std::endl;
    for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      std::cout << language[ *it ] << '\t';
    }
    std::cout << std::endl;
    #endif
    std::list<betaSet> new_delimiter;
    std::map <betaSet, std::string> new_language;
    
    for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      betaSet image = hyperwin.step( *it );
      betaSet length = *std::next( it, 1 ) - *it;
      std::string word = language[*it];
      
      #ifndef _IMG_
      std::cout << "<polygon points=\"" << *it << "," << i-2 << " " << image << "," << i-1 << " " << image + length << "," << i-1 << " " << *it + length << "," << i-2 << " " <<  "\" fill=\"" << "rgb(255,0,0)" << "\" stroke=\"" << "rgb(30,30,30)" << "\" stroke-width=\"0\" opacity=\"0.3\" />" << std::endl;
      std::cout << "<line x1=\"" << *it << "\" y1=\"" << i-2 << "\" x2=\"" << image << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,255,0)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      std::cout << "<line x1=\"" << *it + length << "\" y1=\"" << i-2 << "\" x2=\"" << image + length << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,0,255)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      
      std::cout << "<line x1=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y1=\"" << i-2 + betaSet::get(0,1,17) + *it*betaSet::get(1,0,5) << "\" x2=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y2=\"" << i-2 + betaSet::get(0,1,17) << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.0003 << "\" opacity=\"1\" />" << std::endl;
      std::cout << "<text x=\"" << *it + (image - *it)*betaSet::get(0,1,17) << "\" y=\"" << i-2 + betaSet::get(0,1,17) + *it*betaSet::get(1,0,5) << "\" fill=\"black\" style=\"font-family:Arial;font-size:0.005\">" << word << "</text>" << std::endl;
      #endif
      
      if ( ( image < hyperwin.a() ) && ( hyperwin.b() < image + length ) )
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Schar(), &new_delimiter, &new_language );
        
        addSplitForHypowin( hypowin, hyperwin.a(), length, word + hyperwin.Lchar(), &new_delimiter, &new_language );
        
        addSplitForHypowin( hypowin, hyperwin.b(), length, word + hyperwin.Mchar(), &new_delimiter, &new_language );
      }
      else if ( ( image < hyperwin.b() ) && ( hyperwin.b() < image + length ) )
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Lchar(), &new_delimiter, &new_language );
        
        addSplitForHypowin( hypowin, hyperwin.b(), length, word + hyperwin.Mchar(), &new_delimiter, &new_language );
      }
      else if ( ( image < hyperwin.a() ) && ( hyperwin.a() < image + length ) )
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Schar(), &new_delimiter, &new_language );
        
        addSplitForHypowin( hypowin, hyperwin.a(), length, word + hyperwin.Lchar(), &new_delimiter, &new_language );
      }
      else if ( ( image + length <= hyperwin.a() ) )
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Schar(), &new_delimiter, &new_language );
      }
      else if ( ( image + length <= hyperwin.b() ) )
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Lchar(), &new_delimiter, &new_language );
      }
      else
      {
        addSplitForHypowin( hypowin, image,        length, word + hyperwin.Mchar(), &new_delimiter, &new_language );
      }
    }
    
    new_delimiter.push_back( *delimiter.rbegin() );
    
    delimiter = new_delimiter;
    language = new_language;
    delimiter.sort();
    delimiter.unique();
    
    #ifndef _IMG_
    std::cout << "<line x1=\"0\" y1=\"" << n-1 << "\" x2=\"" << hyperwin.l() << "\" y2=\"" << n-1 << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.01 << "\" />" << std::endl;
    std::cout << "<circle cx=\"" << hyperwin.a() << "\" cy=\"" << n-1 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<circle cx=\"" << hyperwin.b() << "\" cy=\"" << n-1 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<line x1=\"0\" y1=\"" << i - 2 << "\" x2=\"" << hyperwin.l() << "\" y2=\"" << i - 2 << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.01 << "\" />" << std::endl;
    std::cout << "<circle cx=\"" << hyperwin.a() << "\" cy=\"" << i-2 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    std::cout << "<circle cx=\"" << hyperwin.b() << "\" cy=\"" << i-2 << "\" r=\"0.01\" stroke=\"rgb(30,30,30)\" stroke-width=\"0\" fill=\"rgb(30,30,30)\" />" << std::endl;
    #endif
  }
  
  #ifndef _ERROR_
  std::cout << n << ": " << std::endl;
  for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    std::cout << language[ *it ] << '\t';
  }
  std::cout << std::endl;
  #endif
  
  #ifndef _IMG_
  std::cout << "</svg>" << std::endl;
  #endif
  
  std::list<std::string> languageList;
  
  for ( std::list<betaSet>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    languageList.push_back( language[ *it ] );
  }
  
  return languageList;
}

// part of code that splits new subintervals by the hypowindow
void addSplitForHypowin( window hypowin, betaSet start, betaSet length, std::string word, std::list<betaSet>* delimiter, std::map <betaSet, std::string>* language )
{
  if ( start + length <= hypowin.c() ) // whole outside (before)
  {
    delimiter->push_back( start );
    (*language)[start] = word + '0';
  }
  else if ( hypowin.d() <= start ) // whole outside (after)
  {
    delimiter->push_back( start );
    (*language)[start] = word + '0';
  }
  else if ( (start < hypowin.c()) && (start+length < hypowin.d()) ) // split by c
  {
    delimiter->push_back( start );
    (*language)[start] = word + '0';
    
    delimiter->push_back( hypowin.c() );
    (*language)[hypowin.c()] = word + '1';
  }
  else if ( (start < hypowin.c()) && (hypowin.d() < start+length) ) // split by c and d
  {
    delimiter->push_back( start );
    (*language)[start] = word + '0';
    
    delimiter->push_back( hypowin.c() );
    (*language)[hypowin.c()] = word + '1';
    
    delimiter->push_back( hypowin.d() );
    (*language)[hypowin.d()] = word + '0';
  }
  else if ( (hypowin.c() < start) && (hypowin.d() < start+length) ) // split by d
  {
    delimiter->push_back( start );
    (*language)[start] = word + '1';
    
    delimiter->push_back( hypowin.d() );
    (*language)[hypowin.d()] = word + '0';
  }
  else if ( (hypowin.c() <= start) && (start+length <= hypowin.d()) ) // whole inside
  {
    delimiter->push_back( start );
    (*language)[start] = word + '1';
  }
  else
  {
    #ifndef _ERROR_
    std::cout << "TROUBLE! " << std::endl;
    std::cout << "hypowin C: " << hypowin.c() << " " << "hypowin D: " << hypowin.d() << " " << "C: " << start << " " << "D: " << start+length << " " << std::endl;
    #endif
  }
}



// filter by window ----------------------------------------------------
bool fitToWindow( window2D* win, CdeloneSet<betaSet> delone )
{ 
  delone = delone.star();
  for (std::list<Cpoint<betaSet> >::iterator it = delone.begin(); it != delone.end(); ++it)
  {
    win->intersect(*it);
  }
  
  return !win->empty();
}




betaSet minWord( std::list<std::string> lang, window win )
{
  betaSet length(0,0);
  for (std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it)
  {
    betaSet tmp(0,0);
    for (std::string::iterator ot = it->begin(); ot != it->end(); ++ot)
    {
      tmp+= win.char2space(*ot);
    }
    
    if ((tmp < length) || (length == betaSet::get(0,0)))
    {
      length = tmp;
    }
  }
  return length;
}
