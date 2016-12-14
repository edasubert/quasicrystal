#include <cmath>
#include <iostream>
#include <iomanip>
#include <list>
#include <cstdlib>

#include "betaSet.h"
#include "geometricObject2.h"
#include "window.h"

// window2D ------------------------------------------------------------------------------------------------
void window2D::intersect( window2D* win ) {}
void window2D::intersect( Cpoint<betaSet> center ) {}
void window2D::svg( std::ostream& out ) const {}
window2D* window2D::inscribed() const {}
window2D* window2D::circumscribed() const {}
void window2D::center( Cpoint<betaSet> center ) {}
bool window2D::empty() {}

// rhombus -------------------------------------------------------------------------------------------------
rhombus::rhombus()
{
  m_width = betaSet::get( 1, 0 );
  m_height = betaSet::get( 1, 0 );
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}

rhombus::rhombus( betaSet width )
{
  m_width = width;
  m_height = width;
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}

rhombus::rhombus( betaSet width, betaSet height )
{
  m_width = width;
  m_height = height;
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}
rhombus::rhombus( betaSet width, betaSet height, betaSet x ,betaSet y )
{
  m_width = width;
  m_height = height;
  m_x = x;
  m_y = y;
}

bool rhombus::operator == ( const rhombus &Input )const
{
  return ((m_x == Input.m_x) && (m_y == Input.m_y) && (m_width == Input.m_width) && (m_height == Input.m_height));
}

bool rhombus::in( Cpoint<betaSet> star )const
{
  return ( ( m_x <= ( star.getX() + betaSet::get( 2, -1 )*star.getY() ) ) && \
       ( ( star.getX() + betaSet::get( 2, -1 )*star.getY() ) < m_x + m_width ) && \
       ( m_y <= ( betaSet::get( 2, 0 )*star.getY() ) ) && \
       ( ( betaSet::get( 2, 0 )*star.getY() ) < m_y + m_height ) 
      );
}

void rhombus::intersect( rhombus* win )
{
  betaSet x;
  betaSet y;
  betaSet W;
  betaSet H;
  
  if ( m_x < win->m_x )
    x = win->m_x;
  else
    x = m_x;
    
  if ( m_y < win->m_y )
    y = win->m_y;
  else
    y = m_y;
  
  if ( m_x+m_width < win->m_x+win->m_width )
    W = m_x+m_width - x;
  else
    W = win->m_x+win->m_width - x;
    
  if ( m_y+m_height < win->m_y+win->m_height )
    H = m_y+m_height - y;
  else
    H = win->m_y+win->m_height - y;
  
  if ((W < betaSet::get(0,0)) || (H < betaSet::get(0,0)))
  {
    W = betaSet::get(0,0);
    H = betaSet::get(0,0);
  }
  m_x = x;
  m_y = y;
  m_width = W;
  m_height = H;
}

void rhombus::intersect( Cpoint<betaSet> center )
{
  rhombus moving = *this;
  moving.center(center);
  this->intersect(&moving);
}

void rhombus::svg( std::ostream& out ) const
{
  out << "<polygon points=\"";
  out << ( m_x +      betaSet::get( -2, 1, 2 )*m_y )                  << "," << ( betaSet::get( 1, 0, 2 )*m_y ) << " ";
  out << ( m_x+m_width +  betaSet::get( -2, 1, 2 )*m_y )              << "," << ( betaSet::get( 1, 0, 2 )*m_y ) << " ";
  out << ( m_x+m_width +  betaSet::get( -2, 1, 2 )*( m_y+m_height ) ) << "," << ( betaSet::get( 1, 0, 2 )*( m_y+m_height ) ) << " ";
  out << ( m_x +      betaSet::get( -2, 1, 2 )*( m_y+m_height ) )     << "," << ( betaSet::get( 1, 0, 2 )*( m_y+m_height ) ) << " ";
  out << "\" style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << "\" fill-opacity=\"0.5\" />" << std::endl;
}

void rhombus::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

rhombus* rhombus::inscribed() const
{
  rhombus* result;
  if ( m_width < m_height )
  {
    result = new rhombus( m_width, m_width, m_x, m_y + (m_width - m_height)*betaSet::get( 1, 0, 2 ) );
  }
  else
  {
    result = new rhombus( m_height, m_height, m_x + (m_height - m_width)*betaSet::get( 1, 0, 2 ), m_y );
  }
  
  return result;
}

rhombus* rhombus::circumscribed() const
{
  rhombus* result;
  if ( m_width > m_height )
  {
    result = new rhombus( m_width, m_width, m_x, m_y + (m_height - m_width)*betaSet::get( 1, 0, 2 ) );
  }
  else
  {
    result = new rhombus( m_height, m_height, m_x + (m_width - m_height)*betaSet::get( 1, 0, 2 ), m_y );
  }
  
  return result;
}

void rhombus::center( Cpoint<betaSet> center )
{
  betaSet x = center.getX()*betaSet::get( 1, 0 ) + center.getY()*betaSet::get( 2, -1 );
  betaSet y = center.getY()*betaSet::get( 2, 0 );
  
  m_x = x - m_width*betaSet::get( 1, 0, 2 );
  m_y = y - m_height*betaSet::get( 1, 0, 2 );
}

window rhombus::Xwindow()const
{
  return window( m_x, m_x + m_width );
}

window rhombus::Ywindow()const
{
  return window( m_y, m_y + m_height );
}

betaSet rhombus::large()const
{
  window Xwin( m_width );
  window Ywin( m_height );
  
  return ( Xwin.Large() > Ywin.Large() ) ? Xwin.Large() : Ywin.Large();
}

bool rhombus::empty() 
{
  return (m_width == betaSet::get( 0, 0 )) || (m_height == betaSet::get( 0, 0 ));
}

void rhombus::flip(int mode)
{
  if (mode == 0 || mode == 2)
  {
    betaSet x = m_x;
    betaSet y = m_y;
    betaSet width = m_width;
    betaSet height = m_height;
    
    m_x = y;
    m_y = x;
    m_width = height;
    m_height = width;
  }
  
  if (mode == 1 || mode == 2)
  {
    betaSet x = m_x;
    betaSet y = m_y;
    betaSet width = m_width;
    betaSet height = m_height;
    
    m_x = betaSet::get(0,0)-y-height;
    m_y = betaSet::get(0,0)-x-width;
    m_width = height;
    m_height = width;
  }
}

betaSet rhombus::centerX()const
{
  return m_x+betaSet::get( 1, 0, 2 )*m_width +  betaSet::get( -2, 1, 2 )*( m_y+betaSet::get( 1, 0, 2 )*m_height );
}

betaSet rhombus::centerY()const
{
  return  betaSet::get( 1, 0, 2 )*( m_y+betaSet::get( 1, 0, 2 )*m_height );
}

betaSet rhombus::width()const
{
  return  m_width;
}

betaSet rhombus::height()const
{
  return  m_height;
}

bool diff(rhombus& larger, rhombus smaller)
{
  if ((larger.m_x <= smaller.m_x) && (larger.m_y <= smaller.m_y) && (smaller.m_x < larger.m_x+larger.m_width) && (smaller.m_y < larger.m_y+larger.m_height))
  {
    if ((larger.m_x == smaller.m_x) && (larger.m_y == smaller.m_y)) //same top left corner
    {
      if ((larger.m_width == smaller.m_width) && (larger.m_height > smaller.m_height)) //difference in height
      {
        larger.m_height-= smaller.m_height;
        larger.m_y+= smaller.m_height;
        
        return true;
      }
      if ((larger.m_height == smaller.m_height) && (larger.m_width > smaller.m_width)) //difference in width
      {
        larger.m_width-= smaller.m_width;
        larger.m_x+= smaller.m_width;
        
        return true;
      }
    }
    if ((larger.m_x+larger.m_width == smaller.m_x+smaller.m_width) && (larger.m_y+larger.m_height == smaller.m_y+smaller.m_height)) //same bottom right corner
    {
      if ((larger.m_width == smaller.m_width) && (larger.m_height > smaller.m_height)) //difference in height
      {
        larger.m_height-= smaller.m_height;
        
        return true;
      }
      if ((larger.m_height == smaller.m_height) && (larger.m_width > smaller.m_width)) //difference in width
      {
        larger.m_width-= smaller.m_width;
        
        return true;
      }
    }
  }
  return false;
}

// CIRCLE --------------------------------------------------------------

circle::circle()
{
  m_R = betaSet::get( 1, 0 );
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
  intersectionList.push_front(*this);
}
circle::circle( betaSet R )
{
  m_R = R;
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
  intersectionList.push_front(*this);
}
circle::circle( betaSet R, betaSet x ,betaSet y )
{
  m_R = R;
  m_x = x;
  m_y = y;
  intersectionList.push_front(*this);
}

void circle::svg( std::ostream& out )
{
  if (intersectionList.size() == 1)
  {
    out << "<circle cx=\"" << m_x << "\" cy=\"" << m_y << "\" r=\"" << m_R << "\" " << "style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << ";stroke-opacity:1; opacity:0.1;\" />" << std::endl;
    return;
  }
  
  std::list<int>::iterator ot = sweep.begin();
  
  out << "<path d=\" M" << polygon.begin()->getX() << "," << polygon.begin()->getY();
  for (std::list<Cpoint<double> >::iterator it = ++polygon.begin(); it != polygon.end(); ++it)
  {
    out << " A" << m_R << " " << m_R << ", 0, 0," << *ot++ << ", " << it->getX() << " " << it->getY();
  }
  out << " A" << m_R << " " << m_R << ", 0, 0," << *ot << ", " << polygon.begin()->getX() << " " << polygon.begin()->getY();
  out << " \" style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << ";stroke-opacity:1; opacity:0.1;\" />" << std::endl; // light blue, green

}

void circle::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

bool circle::in( Cpoint<betaSet> star )const
{
  for ( std::list<circle>::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it )
  {
    if ( euklid2( star, Cpoint<betaSet>( it->m_x, it->m_y ) ) > m_R*m_R )
    {
      return false;
    }
  }
  return true;
}

void circle::intersect( circle* win )
{
  if ((m_R == win->m_R) && ((m_x != win->m_x) || (m_y != win->m_y)))
  {
    intersectionList.push_front( *win );
  }
  createPolygon();
}

void circle::intersect( Cpoint<betaSet> center )
{
  circle moving = *this;
  moving.center(center);
  this->intersect(&moving);
}

rhombus* circle::inscribed() const
{
  betaSet size = betaSet::get( 15, -3, 4 )*m_R;
  
  rhombus* insc = new rhombus( size, size, m_x, m_y );
  insc->center( Cpoint<betaSet>( m_x, m_y ) );
  return insc;
}
rhombus* circle::circumscribed() const
{
  betaSet size = m_R*betaSet::get( 4, 0 );
  
  rhombus* circ = new rhombus( size, size, m_x, m_y );
  circ->center( Cpoint<betaSet>( m_x, m_y ) );
  return circ;
}

void circle::center( Cpoint<betaSet> center )
{
  m_x = center.getX();
  m_y = center.getY();
}

void circle::emptyIntersectionList()
{
  intersectionList.clear();
  intersectionList.push_front(*this);
}

bool circle::empty() 
{
  if (intersectionList.size() == 1)
  {
    return 0;
  }
  
  return polygon.size() < 2;
}

betaSet circle::centerX()const
{
  betaSet x = m_x;
  for ( std::list<circle>::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it )
  {
    x+= it->m_x;
  }
  
  return x/betaSet::get(intersectionList.size()+1,0);
}

betaSet circle::centerY()const
{
  betaSet y = m_y;
  for ( std::list<circle>::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it )
  {
    y+= it->m_y;
  }
  
  return y/betaSet::get(intersectionList.size()+1,0);
}


void circle::createPolygon()
{
  polygon.clear();
  
  for (std::list<circle>::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it)
  {
    double angle01 = 0; //start
    double angle02 = 4*M_PI; // interval   <--- deliberately larger 
    
    double count = 1;
    
    for (std::list<circle>::const_iterator ot = intersectionList.begin(); ot != intersectionList.end(); ++ot)
    {
      if (it == ot)
        continue;
      
      double tmp01 = atan2(ot->m_y-it->m_y, ot->m_x-it->m_x);
      double tmp02 = acos(sqrt( (ot->m_x-it->m_x)*(ot->m_x-it->m_x) + (ot->m_y-it->m_y)*(ot->m_y-it->m_y) )/(2*m_R));
      
      double new_angle01 = tmp01 - tmp02;
      double new_angle02 = 2*tmp02;
      
      if (new_angle01 < 0)
      {
        new_angle01+= 2*M_PI;
      }
      
      // a starts first
      double a1, a2, b1, b2;
      if (angle01 < new_angle01)
      {
        a1 = angle01;
        a2 = angle02;
        b1 = new_angle01;
        b2 = new_angle02;
      }
      else
      {
        b1 = angle01;
        b2 = angle02;
        a1 = new_angle01;
        a2 = new_angle02;
      }
      
      //std::cout.precision(3);
      //std::cout << a1 << " " << a1+a2 << "\t" << b1 << " " << b1+b2 << "\t" << std::endl << std::flush;
      
      // solve wrap around
      if ((a1+a2 < b1) && (b1+b2 > 2*M_PI+a1))
      {
        a1+= 2*M_PI;
      }
      
      //std::cout.precision(3);
      //std::cout << a1 << " " << a1+a2 << "\t" << b1 << " " << b1+b2 << "\t" << std::endl << std::flush;
      
      // intersection
      angle01 = std::max(a1,b1);
      angle02 = std::min(a1+a2,b1+b2) - angle01;
      
      if (angle01 > 2*M_PI)
      {
        angle01-= 2*M_PI;
      }
      
      //std::cout.precision(3);
      //std::cout << a1 << " " << a1+a2 << "\t" << b1 << " " << b1+b2 << "\t" << angle01 << " " << angle01+angle02 << std::endl;
      
      //if (angle02 > 0)
      //{
      //out << "<line x1=\"" << it->m_x << "\" y1=\"" << it->m_y << "\" x2=\"" << cos(tmp01)*m_R + it->m_x << "\" y2=\"" << sin(tmp01)*m_R + it->m_y << "\" stroke=\"#607D8B\" stroke-width=\"" << 8*count/1000 << "\" stroke-opacity=\"0.4\" />" << std::endl; // blueGrey
        
      //out << "<path d=\" M" << cos(angle01)*m_R + it->m_x << "," << sin(angle01)*m_R + it->m_y << " A" << m_R << " " << m_R << ", 0, 0,1, " << cos(angle01+angle02)*m_R + it->m_x << " " << sin(angle01+angle02)*m_R + it->m_y << "\" " 
          //<< "fill=\"none\" stroke=\"#FF1744\" stroke-width=\"" << 8*count/1000 << "\" stroke-opacity=\"0.4\" />" << std::endl; // red
      //out << "<path d=\" M" << cos(a1)*m_R + it->m_x << "," << sin(a1)*m_R + it->m_y << " A" << m_R << " " << m_R << ", 0, 0,1, " << cos(a1+a2)*m_R + it->m_x << " " << sin(a1+a2)*m_R + it->m_y << "\" " 
          //<< "fill=\"none\" stroke=\"#FFEB3B\" stroke-width=\"" << 4*count/1000 << "\" stroke-opacity=\"0.4\" />" << std::endl; // yellow
      //out << "<path d=\" M" << cos(b1)*m_R + it->m_x << "," << sin(b1)*m_R + it->m_y << " A" << m_R << " " << m_R << ", 0, 0,1, " << cos(b1+b2)*m_R + it->m_x << " " << sin(b1+b2)*m_R + it->m_y << "\" " 
          //<< "fill=\"none\" stroke=\"#0288D1\" stroke-width=\"" << 4*count/1000 << "\" stroke-opacity=\"0.4\" />" << std::endl; // blue
      //count++;
      //}
    }
    //std::cout << angle01 << " " << angle01+angle02 << std::endl;
    //std::cout << std::endl;
    if (angle02 > 0)
    {
      //out << "<path d=\" M" << cos(angle01)*m_R + it->m_x << "," << sin(angle01)*m_R + it->m_y << " A" << m_R << " " << m_R << ", 0, 0,1, " << cos(angle01+angle02)*m_R + it->m_x << " " << sin(angle01+angle02)*m_R + it->m_y << "\" " 
          //<< "fill=\"none\" stroke=\"#3F51B5\" stroke-width=\"" << m_strokeWidth << "\" />" << std::endl; // indigo
      polygon << Cpoint<double>(cos(angle01+angle02)*m_R + it->m_x, sin(angle01+angle02)*m_R + it->m_y);
      sweep.push_back(1);
    }
  }
  
  polygon.sortClockwise();
}



bool diff(circle& larger, circle smaller)
{
  
  //// establish zero (for double equality)
  //double ZERO = 1;
  //for (std::list<Cpoint<double> >::iterator it = larger.polygon.begin(); it != --larger.polygon.end(); it )
  //{
    //Cpoint<double> tmp = *it++;
    //ZERO = std::min(ZERO, euklid(tmp, *it));
  //}
  //ZERO*= 0.001;
  
  
  //bool samePoint = false;
  //std::list<int>::iterator it_sweep = larger.sweep.begin();
  //std::list<Cpoint<double> >::iterator it;
  //std::list<Cpoint<double> >::iterator ot;
  //// find cutting area
  //for (it = larger.polygon.begin(); it != larger.polygon.end(); ++it)
  //{
    //for (ot = smaller.polygon.begin(); ot != smaller.polygon.end(); ++ot)
    //{
      //if (euklid(*it, *ot) < ZERO)
      //{
        //samePoint = true;
        //break;
      //}
    //}
    
    //if (samePoint)
      //break;
    
    //++it_sweep;
  //}
  
  //// no cut available
  //if (!samePoint)
    //return false;
  
  //// CUT
  //std::list<Cpoint<double> >::iterator first = ot;
  
  //*it_sweep = 0;
  
  //++it;
  //++it_sweep;
  //++ot;
  
  //std::list<Cpoint<double> >::iterator it_first = it;
  //std::list<int>::iterator it_sweep_first = it_sweep;
  
  //while ((euklid(*it, *ot) < ZERO) && (ot != first))
  //{
    //++it;
    //++it_sweep;
    //++ot;
    //std::cout << "loop" << std::endl;
  //}
  
  //// if there is stuff to erase, erase it
  //if (ot == first)
  //{
    //larger.polygon.points->clear();
    //return true;
  //}
  
  //if (it != it_first)
  //{
    //// back up to the last common point
    //--it;
    //--it_sweep;
    //--ot;
    
    //std::cout << "erasing !!!!" << std::endl;
    //larger.polygon.points->erase(it_first, it);
    //larger.sweep.erase(it_sweep_first, it_sweep);
  //}
  //return true;
}

// DODECAGON_TIP -----------------------------------------------------------
dodecagonTip::dodecagonTip()
{
  m_a = betaSet::get( 1, 0 );
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}
dodecagonTip::dodecagonTip( betaSet a )
{
  m_a = a;
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}
dodecagonTip::dodecagonTip( betaSet a, betaSet x ,betaSet y )
{
  m_a = a;
  m_x = x;
  m_y = y;
}

void dodecagonTip::svg( std::ostream& out ) const
{
  
}

void dodecagonTip::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

bool dodecagonTip::in( Cpoint<betaSet> star )const
{
  betaSet x = star.getX();
  betaSet y = star.getY();
  
  if ( x < m_x )
  {
    x = m_x*betaSet::get(2,0) - x;
  }
  
  if ( y < m_y )
  {
    y = m_y*betaSet::get(2,0) - y;
  }
  
  return ( ( y + betaSet::get(0,1)*x <= betaSet::get(0,1)*(m_x+m_a) + m_y     ) \
        && ( y + x                   <= m_x + m_y + betaSet::get(-1,1,2)*m_a  ) \
        && ( betaSet::get(0,1)*y + x <= m_x + betaSet::get(0,1)*( m_y + m_a ) ) );
}

void dodecagonTip::intersect( dodecagonTip* win )
{
  
}

void dodecagonTip::intersect( Cpoint<betaSet> center )
{
  dodecagonTip moving = *this;
  moving.center(center);
  this->intersect(&moving);
}

rhombus* dodecagonTip::inscribed() const
{
  betaSet size = betaSet::get( -3, 1, 4 )*m_a;
  
  rhombus* insc = new rhombus( size, size, m_x, m_y );
  insc->center( Cpoint<betaSet>( m_x, m_y ) );
  return insc;
}
rhombus* dodecagonTip::circumscribed() const
{
  betaSet size = m_a*betaSet::get( 4, 0 );
  
  rhombus* circ = new rhombus( size, size, m_x, m_y );
  circ->center( Cpoint<betaSet>( m_x, m_y ) );
  return circ;
}

void dodecagonTip::center( Cpoint<betaSet> center )
{
  m_x = center.getX();
  m_y = center.getY();
}

bool dodecagonTip::empty() 
{
  
}


// EDUARD -----------------------------------------------------------
eduard::eduard()
{
  m_width  = betaSet::get( 1, 0 );
  m_height = betaSet::get( 1, 0 );
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}
eduard::eduard( betaSet width, betaSet height )
{
  m_width  = width;
  m_height = height;
  m_x = betaSet::get( 0, 0 );
  m_y = betaSet::get( 0, 0 );
}
eduard::eduard( betaSet width, betaSet height, betaSet x ,betaSet y )
{
  m_width  = width;
  m_height = height;
  m_x = x;
  m_y = y;
}

void eduard::svg( std::ostream& out ) const
{
  out << "<polygon points=\"";
  out << m_x << "," << m_y << " ";
  out << m_x - m_width << "," << m_y << " ";
  out << m_x - m_width << "," << m_y + m_height << " ";
  out << m_x << "," << m_y + m_height << " ";
  out << m_x << "," << m_y + betaSet::get(4,0,5)*m_height << " ";
  out << m_x - betaSet::get(2,0,3)*m_width << "," << m_y + betaSet::get(4,0,5)*m_height << " ";
  out << m_x - betaSet::get(2,0,3)*m_width << "," << m_y + betaSet::get(3,0,5)*m_height << " ";
  out << m_x << "," << m_y + betaSet::get(3,0,5)*m_height << " ";
  out << m_x << "," << m_y + betaSet::get(2,0,5)*m_height << " ";
  out << m_x - betaSet::get(2,0,3)*m_width << "," << m_y + betaSet::get(2,0,5)*m_height << " ";
  out << m_x - betaSet::get(2,0,3)*m_width << "," << m_y + betaSet::get(1,0,5)*m_height << " ";
  out << m_x << "," << m_y + betaSet::get(1,0,5)*m_height << "\"";
  out << " style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << "\" />" << std::endl;
}

void eduard::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

bool eduard::in( Cpoint<betaSet> star )const
{
  betaSet x = star.getX();
  betaSet y = star.getY();
  
  return !(((m_x - betaSet::get(2,0,3)*m_width < x) && (x < m_x)) && (((m_y + betaSet::get(1,0,5)*m_height < y) && (y < m_y + betaSet::get(2,0,5)*m_height)) || ((m_y + betaSet::get(3,0,5)*m_height < y) && (y < m_y + betaSet::get(4,0,5)*m_height)))) && (((m_x - m_width < x) && (x < m_x)) && ((m_y + m_height > y) && (y > m_y))) ;
  return (((m_x - m_width < x) && (x < m_x)) && ((m_y + m_height > y) && (y > m_y)));
}

void eduard::intersect( eduard* win )
{
  
}

void eduard::intersect( Cpoint<betaSet> center )
{
  eduard moving = *this;
  moving.center(center);
  this->intersect(&moving);
}

rhombus* eduard::inscribed() const
{
  betaSet size = betaSet::get( -3, 1, 4 )*m_width;
  
  rhombus* insc = new rhombus( size, size, m_x, m_y );
  insc->center( Cpoint<betaSet>( m_x, m_y ) );
  return insc;
}
rhombus* eduard::circumscribed() const
{
  betaSet size = betaSet::get( -2, 1 )*m_height + m_width;
  
  if (betaSet::get( 2, 0 )*m_height > betaSet::get( -2, 1 )*m_height + m_width)
  {
    size = betaSet::get( 2, 0 )*m_height;
  }
  
  rhombus* circ = new rhombus( size, size );
  circ->center(Cpoint<betaSet>(m_x - betaSet::get(1,0,2)*m_width, m_y + betaSet::get(1,0,2)*m_height));
  return circ;
}

void eduard::center( Cpoint<betaSet> center )
{
  m_x = center.getX() + betaSet::get(1,0,2)*m_width;
  m_y = center.getY() - betaSet::get(1,0,2)*m_height;
}

bool eduard::empty() 
{
  
}
