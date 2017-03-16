#ifndef _GENERATE_
#define _GENERATE_

#include <iostream>
#include <cmath>

#include "geometricObject2.h"
#include "betaSet.h"
#include "window2.h"
#include "delone10.h"

template <typename numberType>
std::list<numberType> quasicrystal1D( window<numberType> win, numberType x, numberType y, numberType start )
{
  start = win.seed();
  
  while ( start > x ) // get before x
  {
    start = win.stepBack(start.star()).star();
  }
  
  while ( start < x ) // get just after x (first point of quasicrystal)
  {
    start = win.step(start.star()).star();
  }
  std::list<numberType> quasicrystal;
  
  while ( start < y )
  {
    quasicrystal.push_back(start);
    start = win.step(start.star()).star();
  }
  
  #ifndef _ERROR_
  std::cout << "Quasicrystal1D: " << quasicrystal.size() << std::endl;
  #endif
  
  return quasicrystal;
}



template <typename numberType>
CdeloneSet<numberType> quasicrystal2D( std::list<numberType> &x, std::list<numberType> &y )
{
  CdeloneSet<numberType> delone;
  delone.setDescription("2D quasicrystal parallelogram");
  //delone.setColor( fillColor, strokeColor, strokeWidth );
  
  for (typename std::list<numberType>::iterator it = x.begin(); it != x.end(); ++it )
  {
    for (typename std::list<numberType>::iterator ot = y.begin(); ot != y.end(); ++ot )
    {
      delone << transformBeta( Cpoint<numberType>( *it, *ot ) );
    }
  }
  
  return delone;
}

template <typename numberType>
CdeloneSet<numberType> quasicrystal2D( window<numberType> win, std::string &x, std::string &y )
{
  CdeloneSet<numberType> delone;
  Cpoint<numberType> point;
  delone.setDescription("2D quasicrystal parallelogram");
  
  numberType xAxisMove(0,0);
  std::string::iterator it = x.begin();
  
  for (int i = 0; i < ceil(x.size()/2); ++i)
  {
    xAxisMove-= win.char2space(*(it++++));
  }
  
  numberType yAxisMove(0,0);
  it = y.begin();
  
  for (int i = 0; i < ceil(y.size()/2); ++i)
  {
    yAxisMove-= win.char2space(*(it++++));
  }
  
  numberType xAxis = xAxisMove;
  numberType yAxis = yAxisMove;
  
  for (std::string::iterator it = x.begin(); it != x.end(); ++it )
  {
    xAxis+= win.char2space(*it);
    for (std::string::iterator ot = y.begin(); ot != y.end(); ++ot )
    {
      yAxis+= win.char2space(*ot);
      point.setX(xAxis);
      point.setY(yAxis);
      delone << transformBeta( point );
    }
    yAxis = yAxisMove;
  }
  
  return delone;
}

template <typename numberType>
CdeloneSet10<numberType> quasicrystal2D10( window<numberType> win, std::string &x, std::string &y )
{
  CdeloneSet10<numberType> delone;
  Cpoint<numberType> point;
  delone.setDescription("2D quasicrystal parallelogram with 1 0 separation");
  
  numberType xAxisMove(0,0);
  std::string::iterator it = x.begin();
  
  for (int i = 0; i < ceil(x.size()/2); ++i)
  {
    xAxisMove-= win.char2space(*(it++++));
  }
  
  numberType yAxisMove(0,0);
  it = y.begin();
  
  for (int i = 0; i < ceil(y.size()/2); ++i)
  {
    yAxisMove-= win.char2space(*(it++++));
  }
  
  numberType xAxis = xAxisMove;
  numberType yAxis = yAxisMove;
  
  for (std::string::iterator it = x.begin(); it != x.end(); ++++it )
  {
    xAxis+= win.char2space(*it);
    for (std::string::iterator ot = y.begin(); ot != y.end(); ++++ot )
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


template <typename numberType>
CdeloneSet<numberType> quasicrystal2D( window2D<numberType> &win, numberType x1, numberType x2, numberType y1, numberType y2 )
{
  rhombus<numberType> *circ = dynamic_cast<rhombus<numberType>*> ( win.circumscribed() );
  
  std::list<numberType> Xquasi = quasicrystal1D( circ->Xwindow(), x1 - numberType::windowB()*(y2-y1)/numberType::windowA(), numberType::windowB()*(y2-y1)/numberType::windowA() + (x2-x1), numberType::get(0, 0) );
  std::list<numberType> Yquasi = quasicrystal1D( circ->Ywindow(), y1/numberType::windowD(), y2/numberType::windowD(), numberType::get(0, 0) );
  
  CdeloneSet<numberType> delone;
  for (typename std::list<numberType>::iterator it = Xquasi.begin(); it != Xquasi.end(); ++it )
  {
    for (typename std::list<numberType>::iterator ot = Yquasi.begin(); ot != Yquasi.end(); ++ot )
    {
      Cpoint<numberType> candidate( *it, *ot );
      candidate = transformBeta( candidate );
      
      if ( ( candidate.getX() >= x1 ) && ( candidate.getX() <= x2 ) && ( candidate.getY() >= y1 ) && ( candidate.getY() <= y2 ) )
      {
        if ( win.in( candidate.star() ) )
        {
          delone << candidate;
        }
      }
    }
  }
  delete circ;
  
  return delone;
}

template <typename numberType>
Cpoint<numberType> transformBeta( const Cpoint<numberType> &point )
{
  Cpoint<numberType> Output( point );
  Output.setX(numberType::transformA()*point.getX() + numberType::transformB()*(point.getY()));
  Output.setY(numberType::transformC()*point.getX() + numberType::transformD()*(point.getY()));
  return Output;
}


template <typename numberType>
std::list<std::string> language( window<numberType> win, int n )
{
  std::list<numberType> delimiter;
  std::map <numberType, std::string> language;
  
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
  
  for (int i = 2; i <= n; ++i )
  {
    #ifndef _ERROR_
    std::cout << i << ": " << std::endl;
    for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      std::cout << language[ *it ] << '\t';
    }
    std::cout << std::endl;
    #endif
    std::list<numberType> new_delimiter;
    std::map <numberType, std::string> new_language;
    
    for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      numberType image = win.step( *it );
      numberType length = *std::next( it, 1 ) - *it;
      std::string word = language[*it];
      
      #ifndef _IMG_
      std::cout << "<polygon points=\"" << *it << "," << i-2 << " " << image << "," << i-1 << " " << image + length << "," << i-1 << " " << *it + length << "," << i-2 << " " <<  "\" fill=\"" << "rgb(255,0,0)" << "\" stroke=\"" << "rgb(30,30,30)" << "\" stroke-width=\"0\" opacity=\"0.3\" />" << std::endl;
      std::cout << "<line x1=\"" << *it << "\" y1=\"" << i-2 << "\" x2=\"" << image << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,255,0)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      std::cout << "<line x1=\"" << *it + length << "\" y1=\"" << i-2 << "\" x2=\"" << image + length << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,0,255)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      
      std::cout << "<line x1=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y1=\"" << i-2 + numberType::get(0,1,17) + *it*numberType::get(1,0,5) << "\" x2=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y2=\"" << i-2 + numberType::get(0,1,17) << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.0003 << "\" opacity=\"1\" />" << std::endl;
      std::cout << "<text x=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y=\"" << i-2 + numberType::get(0,1,17) + *it*numberType::get(1,0,5) << "\" fill=\"black\" style=\"font-family:Arial;font-size:0.005\">" << word << "</text>" << std::endl;
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
  for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    std::cout << language[ *it ] << '\t';
  }
  std::cout << std::endl;
  #endif
  
  #ifndef _IMG_
  std::cout << "</svg>" << std::endl;
  #endif
  
  std::list<std::string> languageList;
  
  for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    languageList.push_back( language[ *it ] );
  }
  
  return languageList;
}

template <typename numberType>
std::list<std::string> language( window<numberType> hypowin, window<numberType> hyperwin, int n )
{
  std::list<numberType> delimiter;
  std::map <numberType, std::string> language;
  
  std::string tmp(1, hyperwin.Schar());
  addSplitForHypowin( hypowin, hyperwin.c(), hyperwin.a() - hyperwin.c(), tmp, &delimiter, &language );
  tmp.assign(1, hyperwin.Lchar());
  addSplitForHypowin( hypowin, hyperwin.a(), hyperwin.b() - hyperwin.a(), tmp, &delimiter, &language );
  tmp.assign(1, hyperwin.Mchar());
  addSplitForHypowin( hypowin, hyperwin.b(), hyperwin.d() - hyperwin.b(), tmp, &delimiter, &language );
  delimiter.push_back( hyperwin.d() );
  
  //std::cout << hyperwin.Mchar() << " " << hyperwin.Lchar() << " " << hyperwin.Schar() << " " << std::endl;
  
  #ifndef _IMG_
  std::cout << "<svg height=\"" << 100*(n+2) << "\" width=\"" << 100*(hyperwin.l()) << "\" viewBox=\"0 -1 " << hyperwin.l() << " " << n+2 << "\">" << std::endl;
  #endif
  
  for (int i = 2; i <= n; ++i )
  {
    #ifndef _ERROR_
    std::cout << i << ": " << std::endl;
    for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      std::cout << language[ *it ] << '\t';
    }
    std::cout << std::endl;
    #endif
    
    
    std::list<numberType> new_delimiter;
    std::map <numberType, std::string> new_language;
    
    for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
    {
      numberType image = hyperwin.step( *it );
      numberType length = *std::next( it, 1 ) - *it;
      std::string word = language[*it];
      
      #ifndef _IMG_
      std::cout << "<polygon points=\"" << *it << "," << i-2 << " " << image << "," << i-1 << " " << image + length << "," << i-1 << " " << *it + length << "," << i-2 << " " <<  "\" fill=\"" << "rgb(255,0,0)" << "\" stroke=\"" << "rgb(30,30,30)" << "\" stroke-width=\"0\" opacity=\"0.3\" />" << std::endl;
      std::cout << "<line x1=\"" << *it << "\" y1=\"" << i-2 << "\" x2=\"" << image << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,255,0)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      std::cout << "<line x1=\"" << *it + length << "\" y1=\"" << i-2 << "\" x2=\"" << image + length << "\" y2=\"" << i-1 << "\" stroke=\"" << "rgb(0,0,255)" << "\" stroke-width=\"" << 0.003 << "\" opacity=\"0.6\" />" << std::endl;
      
      std::cout << "<line x1=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y1=\"" << i-2 + numberType::get(0,1,17) + *it*numberType::get(1,0,5) << "\" x2=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y2=\"" << i-2 + numberType::get(0,1,17) << "\" stroke=\"" << "rgb(0,0,0)" << "\" stroke-width=\"" << 0.0003 << "\" opacity=\"1\" />" << std::endl;
      std::cout << "<text x=\"" << *it + (image - *it)*numberType::get(0,1,17) << "\" y=\"" << i-2 + numberType::get(0,1,17) + *it*numberType::get(1,0,5) << "\" fill=\"black\" style=\"font-family:Arial;font-size:0.005\">" << word << "</text>" << std::endl;
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
  for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    std::cout << language[ *it ] << '\t';
  }
  std::cout << std::endl;
  #endif
  
  #ifndef _IMG_
  std::cout << "</svg>" << std::endl;
  #endif
  
  std::list<std::string> languageList;
  
  for (typename std::list<numberType>::iterator it = delimiter.begin(); std::next(it,1) != delimiter.end(); ++it )
  {
    languageList.push_back( language[ *it ] );
  }
  
  return languageList;
}

template <typename numberType>
void addSplitForHypowin( window<numberType> hypowin, numberType start, numberType length, std::string word, std::list<numberType>* delimiter, std::map <numberType, std::string>* language )
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


template <typename numberType>
bool fitToWindow( window2D<numberType>* win, CdeloneSet<numberType> delone )
{ 
  window2D<numberType> windowCheck = *win;
  delone = delone.star();
  for (typename std::list<Cpoint<numberType> >::iterator it = delone.begin(); it != delone.end(); ++it)
  {
    windowCheck.intersect(*it);
  }
  
  return !windowCheck.empty();
}


template <typename numberType>
numberType minWord( std::list<std::string> lang, window<numberType> win )
{
  numberType length(0,0);
  for (std::list<std::string>::iterator it = lang.begin(); it != lang.end(); ++it)
  {
    numberType tmp(0,0);
    for (std::string::iterator ot = it->begin(); ot != it->end(); ++ot)
    {
      tmp+= win.char2space(*ot);
    }
    
    if ((tmp < length) || (length == numberType::get(0,0)))
    {
      length = tmp;
    }
  }
  return length;
}

template <typename numberType>
numberType max(numberType a, numberType b)
{
  return (a>=b)? a : b;
}

template <typename numberType>
numberType min(numberType a, numberType b)
{
  return (a<=b)? a : b;
}

template <typename numberType>
int sign(numberType a)
{
  if ( a > numberType::get(0,0) )
    return 1;
  else if ( a < numberType::get(0,0) )
    return -1;
  else
    return 0;
}

#endif
