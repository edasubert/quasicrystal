#ifndef _WINDOW_
#define _WINDOW_

#include <cstring>
#include <iostream>
#include <cstdio>
#include "geometricObject2.h"
#include "betaSet.h"

//typedef betaSet numberType;

template <typename numberType>
class window {
    protected:
        numberType  m_c, m_d; // borders of window interval
        numberType  m_a, m_b; // dividors of diferent spaces
        int m_k;
        numberType  m_L, m_M, m_S; // sizes of spaces
        char m_Lchar, m_Mchar, m_Schar; // character representation of spaces
        
        void assign();
    public:
        window();
        window( numberType  l );
        window( numberType  c, numberType  d );
        
        void assign(numberType c, numberType d, numberType a, numberType b, numberType L, numberType M, numberType S, char Lchar, char Mchar, char Schar, int k);
        
        numberType  c() const;
        numberType  d() const;
        numberType  a() const;
        numberType  b() const;
        numberType  l() const;
        int k() const;
        
        // S,M,L does not mean small, medium, large but name third, first and second semi-interval
        numberType  S() const;
        numberType  M() const;
        numberType  L() const;
        
        numberType  Small() const;
        numberType  Large() const;
        
        char Schar() const;
        char Mchar() const;
        char Lchar() const;
        
        numberType  char2space( char letter ) const;
        
        numberType  step( numberType  xStar, bool leftLimit = false );
        numberType  stepBack( numberType  xStar );
        
        void var_dump( std::ostream& out ) const;
        
        numberType  seed() const; // returns point of quasicrystal
};




template <typename numberType>
class window2D {
  public:
    virtual bool in( Cpoint<numberType > star );
    virtual void intersect( window2D* win );
    virtual void intersect( Cpoint<numberType > center );
    virtual void svg( std::ostream& out );
    virtual window2D* inscribed();
    virtual window2D* circumscribed();
    virtual void center( Cpoint<numberType > center );
    virtual bool empty();
};

template <typename numberType>
class rhombus : public window2D<numberType> 
{
  protected:
    numberType  m_height;
    numberType  m_width;
    numberType  m_x; // left buttom corner
    numberType  m_y;
    
    numberType  backup_m_height;
    numberType  backup_m_width;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    rhombus();
    rhombus( numberType  width );
    rhombus( numberType  width, numberType  height );
    rhombus( numberType  width, numberType  height, numberType  x ,numberType  y );
    void operator = (const rhombus<numberType>& rhombus);
    void svg( std::ostream& out );
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool operator == ( const rhombus<numberType> &Input );
    
    bool in( Cpoint<numberType > star );
    void intersect( rhombus<numberType>* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus<numberType>* inscribed();
    rhombus<numberType>* circumscribed();
    
    void center( Cpoint<numberType > center );
    
    window<numberType> Xwindow()const;
    window<numberType> Ywindow()const;
    numberType  large()const;
    
    bool empty();
    void flip(int mode);
    
    numberType  centerX()const;
    numberType  centerY()const;
    numberType  width()const;
    numberType  height()const;
    numberType size();
    
    template <typename number>
    friend bool diff(rhombus<number>& larger, rhombus<number> smaller);
};

template <typename numberType>
class circle : public window2D<numberType> 
{
  protected:
    numberType  m_R;
    numberType  m_x; // center
    numberType  m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
    
    std::list<circle> intersectionList;
    CpointSet<double> polygon;
    std::list<int> sweep; // svg sweep-flag
  public:
    circle();
    circle( numberType  R );
    circle( numberType  R, numberType  x ,numberType  y );
    void svg( std::ostream& out );
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<numberType > star );
    void intersect( circle<numberType>* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus<numberType>* inscribed();
    rhombus<numberType>* circumscribed();
    
    void center( Cpoint<numberType > center );
    void emptyIntersectionList();
    
    bool empty();
    
    double  centerX();
    double  centerY();
    
    void createPolygon();
    
    template <typename number>
    friend bool diff(circle<number>& larger, circle<number> smaller);
};

template <typename numberType>
class polygon : public window2D<numberType> 
{
  protected:
    Cpoint<numberType> m_center;
    CpointSet<numberType> m_vert;
    CpointSet<numberType> backup_vert; // for simple intersection after original polygon has been altered
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    polygon();
    polygon(CpointSet<numberType> vert);
    polygon(CpointSet<numberType> vert, Cpoint<numberType> center);
    void operator = (const polygon<numberType>& polygon);
    void svg( std::ostream& out );
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<numberType> star );
    bool inClose( Cpoint<numberType> star );
    void intersect( polygon<numberType>* win );
    void intersect( Cpoint<numberType> center );
    
    rhombus<numberType>* inscribed();
    rhombus<numberType>* circumscribed();
    
    void center( Cpoint<numberType> center );
    
    bool empty();
    
    double  centerX();
    double  centerY();
    
    numberType size();
    bool operator < (polygon<numberType> &compare);
    
    template <typename number>
    friend bool diff(polygon<number>& larger, polygon<number> smaller);
    
    polygon<numberType> static octagon(numberType size);
    polygon<numberType> static rhombic(numberType size);
};

template <typename numberType>
class eduard : public window2D<numberType> 
{
  protected:
    numberType  m_height;
    numberType  m_width;
    numberType  m_x; // right buttom corner
    numberType  m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    eduard();
    eduard( numberType  width, numberType  height );
    eduard( numberType  width, numberType  height, numberType  x ,numberType  y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<numberType > star )const;
    void intersect( eduard<numberType>* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus<numberType>* inscribed() const;
    rhombus<numberType>* circumscribed() const;
    
    void center( Cpoint<numberType > center );
    
    bool empty();
};








// IMPLEMENTATION ------------------------------------------------------


// window1D ------------------------------------------------------------------------------------------------
template <typename numberType>
window<numberType>::window()
{
  m_c = numberType ::get( 0, 0 );
  m_d = numberType ::get( 1, 0 );
  m_k = 0;
  
  m_L = numberType ::get( 0, 0 );
  m_M = numberType ::get( 0, 1 );
  m_S = numberType ::get( -1, 1 );
  
  m_Lchar = '0';
  m_Mchar = 'D';
  m_Schar = 'E';
  
  m_a = numberType ::get( -3, 1 );
  m_b = numberType ::get( -3, 1 );
}

template <typename numberType>
window<numberType>::window( numberType  l )
{
  m_c = numberType ::get( 0, 0 );
  m_d = l;
  
  assign();
}

template <typename numberType>
window<numberType>::window( numberType  c, numberType  d )
{
  m_c = c;
  m_d = d;
  
  assign();
}

template <typename numberType>
void window<numberType>::assign()
{
  numberType  tmp = numberType ::get( 1, 0 );
  m_k = 0;
  
  while ( tmp*m_d - tmp*m_c <= numberType::get( 1, 0 )/numberType::get( 0, 1 ) ) {
    tmp*= numberType ::get( 0, 1 );
    m_k++;
  }
  while ( tmp*m_d - tmp*m_c > numberType::get( 1, 0 ) ) {
    tmp/= numberType::get( 0, 1 ); 
    m_k--;
  }
  
  // assign various values
  m_L = numberType::assign_L(tmp*m_c, tmp*m_d, m_k)*tmp;
  m_M = numberType::assign_M(tmp*m_c, tmp*m_d, m_k)*tmp;
  m_S = numberType::assign_S(tmp*m_c, tmp*m_d, m_k)*tmp;
  
  m_Lchar = numberType::assign_Lchar(tmp*m_c, tmp*m_d, m_k);
  m_Mchar = numberType::assign_Mchar(tmp*m_c, tmp*m_d, m_k);
  m_Schar = numberType::assign_Schar(tmp*m_c, tmp*m_d, m_k);
  
  m_a = numberType::assign_a(tmp*m_c, tmp*m_d, m_k)/tmp;
  m_b = numberType::assign_b(tmp*m_c, tmp*m_d, m_k)/tmp;
}

template <typename numberType>
void window<numberType>::assign(numberType c, numberType d, numberType a, numberType b, numberType L, numberType M, numberType S, char Lchar, char Mchar, char Schar, int k)
{
  m_a = a;
  m_b = b;
  m_c = c;
  m_d = d;
  
  m_L = L;
  m_M = M;
  m_S = S;
  
  m_Lchar = Lchar;
  m_Mchar = Mchar;
  m_Schar = Schar;
  
  m_k = k;
}

template <typename numberType>
numberType  window<numberType>::L() const
{
  return m_L;
}

template <typename numberType>
numberType  window<numberType>::M() const
{
  return m_M;
}

template <typename numberType>
numberType  window<numberType>::S() const
{
  return m_S;
}

template <typename numberType>
numberType  window<numberType>::Small() const
{
  if ( ( m_S < m_M ) && ( ( m_S < m_L ) || ( m_L == numberType ::get(0,0) ) ) )
  {
    return m_S;
  }
  else if ( ( m_M < m_L ) || ( m_L == numberType ::get(0,0) ) )
  {
    return m_M;
  }
  return m_L;
}

template <typename numberType>
numberType  window<numberType>::Large() const
{
  if ( ( m_L > m_M ) && ( m_L > m_S ) )
  {
    return m_L;
  }
  else if ( m_M > m_S )
  {
    return m_M;
  }
  return m_S;
}

template <typename numberType>
char  window<numberType>::Lchar() const
{
  return m_Lchar;
}

template <typename numberType>
char  window<numberType>::Mchar() const
{
  return m_Mchar;
}

template <typename numberType>
char  window<numberType>::Schar() const
{
  return m_Schar;
}

template <typename numberType>
numberType window<numberType>::c() const
{
  return m_c;
}

template <typename numberType>
numberType window<numberType>::d() const
{
  return m_d;
}

template <typename numberType>
numberType window<numberType>::a() const
{
  return m_a;
}

template <typename numberType>
numberType window<numberType>::b() const
{
  return m_b;
}

template <typename numberType>
numberType window<numberType>::l() const
{
  return m_d-m_c;
}

template <typename numberType>
int window<numberType>::k() const
{
  return m_k;
}

template <typename numberType>
numberType window<numberType>::char2space( char letter ) const
{
  if ( letter == m_Lchar )
  {
    return m_L;
  }
  else if ( letter == m_Mchar )
  {
    return m_M;
  }
  else if ( letter == m_Schar )
  {
    return m_S;
  }
  else
  {
    return numberType ::get( 0, 0 );
  }
}

template <typename numberType>
numberType window<numberType>::step(numberType  xStar, bool leftLimit)
{
  if ( ( m_c <= xStar && xStar < m_a ) || ( ( xStar == m_a ) && leftLimit ) )
  {
    return xStar + m_S.star();
  }
  else if ( ( m_a <= xStar && xStar < m_b ) || ( ( xStar == m_b ) && leftLimit ) )
  {
    return xStar + m_L.star();
  }
  else if ( ( m_b <= xStar && xStar < m_d ) || ( ( xStar == m_d ) && leftLimit ) )
  {
    return xStar + m_M.star();
  }
}

template <typename numberType>
numberType window<numberType>::stepBack(numberType  xStar)
{  
  if ( (m_c + m_S.star() <= xStar && xStar < m_a + m_S.star()) )
  {
    return xStar - m_S.star();
  }
  else if ( (m_a + m_L.star() <= xStar && xStar < m_b + m_L.star()) )
  {
    return xStar - m_L.star();
  }
  else if ( ( m_b + m_M.star() <= xStar && xStar < m_d + m_M.star()) )
  {
    return xStar - m_M.star();
  }
  
  std::cout << "NOOOOOOOOOO " << m_k << std::endl;
  print(std::cout, xStar);
  std::cout << " c: ";
  print(std::cout, m_c-m_c);
  std::cout << " a: ";
  print(std::cout, m_a-m_c);
  std::cout << " b: ";
  print(std::cout, m_b-m_c);
  std::cout << " d: ";
  print(std::cout, m_d-m_c);
  std::cout << " c+S: ";
  print(std::cout, m_c-m_c + m_S.star());
  std::cout << " a+S: ";
  print(std::cout, m_a-m_c + m_S.star());
  std::cout << " a+L: ";
  print(std::cout, m_a-m_c + m_L.star());
  std::cout << " b+L: ";
  print(std::cout, m_b-m_c + m_L.star());
  std::cout << " b+M: ";
  print(std::cout, m_b-m_c + m_M.star());
  std::cout << " d+M: ";
  print(std::cout, m_d-m_c + m_M.star());
  std::cout << " S: ";
  print(std::cout, m_S);
  std::cout << " L: ";
  print(std::cout, m_L);
  std::cout << " M: ";
  print(std::cout, m_M);
  std::cout << std::endl;
}

template <typename numberType>
void window<numberType>::var_dump( std::ostream& out ) const
{
  out << "WWWWWWWWWWWWWWWWWWWW" << std::endl;
  
  out << "c = ";
  print( out, m_c );
  out << std::endl;
  
  out << "a = ";
  print( out, m_a );
  out << std::endl;
  
  out << "b = ";
  print( out, m_b );
  out << std::endl;
  
  out << "d = ";
  print( out, m_d );
  out << std::endl;
  
  out << "k = ";
  out << m_k;
  out << std::endl;out << std::endl;
  
  out << m_Schar << ": ";
  print( out, m_S );
  out << std::endl;
  
  out << m_Mchar << ": ";
  print( out, m_M );
  out << std::endl;
  
  out << m_Lchar << ": ";
  print( out, m_L );
  out << std::endl;
  
  out << "WWWWWWWWWWWWWWWWWWWW" << std::endl;
}

template <typename numberType>
numberType window<numberType>::seed() const
{
  return ((m_c + m_d)*numberType::get(1,0,2)).star();
}




// window2D ------------------------------------------------------------------------------------------------
template <typename numberType>
void window2D<numberType>::intersect( window2D* win ) {}

template <typename numberType>
void window2D<numberType>::intersect( Cpoint<numberType> center ) {}

template <typename numberType>
void window2D<numberType>::svg( std::ostream& out ) {}

template <typename numberType>
window2D<numberType>* window2D<numberType>::inscribed() {}

template <typename numberType>
window2D<numberType>* window2D<numberType>::circumscribed() {}

template <typename numberType>
void window2D<numberType>::center( Cpoint<numberType> center ) {}

template <typename numberType>
bool window2D<numberType>::empty() {}

// rhombus -------------------------------------------------------------------------------------------------
template <typename numberType>
rhombus<numberType>::rhombus()
{
  m_width = numberType::get( 1, 0 );
  m_height = numberType::get( 1, 0 );
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
  
  backup_m_width = m_width;
  backup_m_height = m_height;
}

template <typename numberType>
rhombus<numberType>::rhombus( numberType width )
{
  m_width = width;
  m_height = width;
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
  
  backup_m_width = m_width;
  backup_m_height = m_height;
}

template <typename numberType>
rhombus<numberType>::rhombus( numberType width, numberType height )
{
  m_width = width;
  m_height = height;
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
  
  backup_m_width = m_width;
  backup_m_height = m_height;
}

template <typename numberType>
rhombus<numberType>::rhombus( numberType width, numberType height, numberType x ,numberType y )
{
  m_width = width;
  m_height = height;
  m_x = x;
  m_y = y;
  
  backup_m_width = m_width;
  backup_m_height = m_height;
}

template <typename numberType>
void rhombus<numberType>::operator = (const rhombus<numberType>& rhombus)
{
  m_width = rhombus.m_width;
  m_height = rhombus.m_height;
  m_x = rhombus.m_x;
  m_y = rhombus.m_y;
  
  backup_m_width = rhombus.m_width;
  backup_m_height = rhombus.m_height;
}

template <typename numberType>
bool rhombus<numberType>::operator == ( const rhombus &Input )
{
  return ((m_x == Input.m_x) && (m_y == Input.m_y) && (m_width == Input.m_width) && (m_height == Input.m_height));
}

template <typename numberType>
bool rhombus<numberType>::in(Cpoint<numberType> star)
{
  numberType tmp = numberType::windowA()*numberType::windowD() - numberType::windowB()*numberType::windowC(); // denominator of inverse transform
  
  return ( ( m_x <= (numberType::windowD()/tmp*star.getX() - numberType::windowB()/tmp*star.getY()) ) && \
           ( (numberType::windowD()/tmp*star.getX() - numberType::windowB()/tmp*star.getY()) < m_x + m_width ) && \
           ( m_y <= (numberType::windowA()/tmp*star.getY() - numberType::windowC()/tmp*star.getX()) ) && \
           ( (numberType::windowA()/tmp*star.getY() - numberType::windowC()/tmp*star.getX()) < m_y + m_height ) 
      );
}

template <typename numberType>
void rhombus<numberType>::intersect( rhombus* win )
{
  numberType x;
  numberType y;
  numberType W;
  numberType H;
  
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
  
  if ((W < numberType::get(0,0)) || (H < numberType::get(0,0)))
  {
    W = numberType::get(0,0);
    H = numberType::get(0,0);
  }
  
  m_x = x;
  m_y = y;
  m_width = W;
  m_height = H;
}

template <typename numberType>
void rhombus<numberType>::intersect( Cpoint<numberType> center )
{
  rhombus moving = rhombus(backup_m_width, backup_m_height);
  moving.center(center);
  this->intersect(&moving);
}

template <typename numberType>
void rhombus<numberType>::svg( std::ostream& out )
{
  out << "<!--" << std::endl;
  out << "x:\t\t\t";
  print(out, m_x);
  out << std::endl;
  out << "y:\t\t\t";
  print(out, m_y);
  out << std::endl;
  out << "width:\t";
  print(out, m_width);
  out << std::endl;
  out << "height:\t";
  print(out, m_height);
  out << std::endl;
  out << "-->" << std::endl;
  out << "<polygon points=\"";
  out << (numberType::windowA()*m_x           + numberType::windowB()*m_y)            << "," << ( numberType::windowC()*m_x           + numberType::windowD()*m_y ) << " ";
  out << (numberType::windowA()*(m_x+m_width) + numberType::windowB()*m_y)            << "," << ( numberType::windowC()*(m_x+m_width) + numberType::windowD()*m_y ) << " ";
  out << (numberType::windowA()*(m_x+m_width) + numberType::windowB()*(m_y+m_height)) << "," << ( numberType::windowC()*(m_x+m_width) + numberType::windowD()*( m_y+m_height ) ) << " ";
  out << (numberType::windowA()*m_x           + numberType::windowB()*(m_y+m_height)) << "," << ( numberType::windowC()*m_x           + numberType::windowD()*( m_y+m_height ) ) << " ";
  out << "\" style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << "\" fill-opacity=\"0.5\" />" << std::endl;
}

template <typename numberType>
void rhombus<numberType>::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

template <typename numberType>
rhombus<numberType>* rhombus<numberType>::inscribed()
{
  rhombus* result;
  if ( m_width < m_height )
  {
    result = new rhombus<numberType>( m_width, m_width, m_x, m_y + (m_width - m_height)*numberType::get( 1, 0, 2 ) );
  }
  else
  {
    result = new rhombus<numberType>( m_height*numberType::get( 4, 0, 5 ), m_height*numberType::get( 4, 0, 5 ), m_x + (m_height - m_width)*numberType::get( 1, 0, 2 ), m_y );
  }
  
  return result;
}

template <typename numberType>
rhombus<numberType>* rhombus<numberType>::circumscribed()
{
  rhombus* result;
  if ( m_width > m_height )
  {
    result = new rhombus<numberType>(m_width, m_width, m_x, m_y + (m_height - m_width)*numberType::get( 1, 0, 2 ) );
  }
  else
  {
    result = new rhombus<numberType>(m_height*numberType::get( 5, 0, 4 ), m_height*numberType::get( 5, 0, 4 ), m_x + (m_width - m_height)*numberType::get( 1, 0, 2 ), m_y );
  }
  
  return result;
}

template <typename numberType>
void rhombus<numberType>::center( Cpoint<numberType> center )
{
  //numberType x = center.getX()*numberType::get( 1, 0 ) + center.getY()*numberType::get( 2, -1 );
  //numberType y = center.getY()*numberType::get( 2, 0 );
  
  numberType tmp = numberType::windowA()*numberType::windowD() - numberType::windowB()*numberType::windowC(); // denominator of inverse transform
  
  numberType x = numberType::windowD()/tmp*center.getX() - numberType::windowB()/tmp*center.getY();
  numberType y = numberType::windowA()/tmp*center.getY() - numberType::windowC()/tmp*center.getX();
  
  m_x = x - m_width*numberType::get( 1, 0, 2 );
  m_y = y - m_height*numberType::get( 1, 0, 2 );
}

template <typename numberType>
window<numberType> rhombus<numberType>::Xwindow() const
{
  return window<numberType>( m_x, m_x + m_width );
}

template <typename numberType>
window<numberType> rhombus<numberType>::Ywindow() const
{
  return window<numberType>( m_y, m_y + m_height );
}

template <typename numberType>
numberType rhombus<numberType>::large() const
{
  window<numberType> Xwin( m_width );
  window<numberType> Ywin( m_height );
  
  return ( Xwin.Large() > Ywin.Large() ) ? Xwin.Large() : Ywin.Large();
}

template <typename numberType>
bool rhombus<numberType>::empty() 
{
  return (m_width == numberType::get( 0, 0 )) || (m_height == numberType::get( 0, 0 ));
}

template <typename numberType>
void rhombus<numberType>::flip(int mode)
{
  if (mode == 0 || mode == 2)
  {
    numberType x = m_x;
    numberType y = m_y;
    numberType width = m_width;
    numberType height = m_height;
    
    m_x = y;
    m_y = x;
    m_width = height;
    m_height = width;
  }
  
  if (mode == 1 || mode == 2)
  {
    numberType x = m_x;
    numberType y = m_y;
    numberType width = m_width;
    numberType height = m_height;
    
    m_x = numberType::get(0,0)-y-height;
    m_y = numberType::get(0,0)-x-width;
    m_width = height;
    m_height = width;
  }
}

template <typename numberType>
numberType rhombus<numberType>::centerX()const
{
  return numberType::windowA()*(m_x+numberType::get( 1, 0, 2 )*m_width) + numberType::windowB()*( m_y+numberType::get( 1, 0, 2 )*m_height );
}

template <typename numberType>
numberType rhombus<numberType>::centerY()const
{
  return numberType::windowC()*(m_x+numberType::get( 1, 0, 2 )*m_width) + numberType::windowD()*( m_y+numberType::get( 1, 0, 2 )*m_height );
}

template <typename numberType>
numberType rhombus<numberType>::width()const
{
  return  m_width;
}

template <typename numberType>
numberType rhombus<numberType>::height()const
{
  return  m_height;
}

template <typename numberType>
numberType rhombus<numberType>::size()
{
  return (numberType::windowA()*m_width+numberType::windowB()*m_height)*(numberType::windowC()*m_width+numberType::windowD()*m_height);
}

template <typename numberType>
bool diff(rhombus<numberType>& larger, rhombus<numberType> smaller)
{
  if (larger == smaller)
  {
    larger.m_width = numberType::get(0,0);
    larger.m_height = numberType::get(0,0);
    return true;
  }
  
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

template <typename numberType>
circle<numberType>::circle()
{
  m_R = numberType::get( 1, 0 );
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
  intersectionList.push_front(*this);
}

template <typename numberType>
circle<numberType>::circle( numberType R )
{
  m_R = R;
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
  intersectionList.push_front(*this);
}

template <typename numberType>
circle<numberType>::circle( numberType R, numberType x ,numberType y )
{
  m_R = R;
  m_x = x;
  m_y = y;
  intersectionList.push_front(*this);
}

template <typename numberType>
void circle<numberType>::svg( std::ostream& out )
{
  if (intersectionList.size() == 1)
  {
    out << "<circle cx=\"" << m_x << "\" cy=\"" << m_y << "\" r=\"" << m_R << "\" " << "style=\"fill:none;stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << ";stroke-opacity:1; opacity:0.3;\" />" << std::endl;
    return;
  }
  
  std::list<int>::iterator ot = sweep.begin();
  
  out << "<path d=\" M" << polygon.begin()->getX() << "," << polygon.begin()->getY();
  for (std::list<Cpoint<double> >::iterator it = ++polygon.begin(); it != polygon.end(); ++it)
  {
    out << " A" << m_R << " " << m_R << ", 0, 0," << *ot++ << ", " << it->getX() << " " << it->getY();
  }
  out << " A" << m_R << " " << m_R << ", 0, 0," << *ot << ", " << polygon.begin()->getX() << " " << polygon.begin()->getY();
  out << " \" style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << ";stroke-opacity:1; opacity:0.3;\" />" << std::endl; // light blue, green

}

template <typename numberType>
void circle<numberType>::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

template <typename numberType>
bool circle<numberType>::in( Cpoint<numberType> star )
{
  for (typename std::list<circle<numberType> >::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it )
  {
    if ( euklid2( star, Cpoint<numberType>( it->m_x, it->m_y ) ) >= m_R*m_R )
    {
      return false;
    }
  }
  return true;
}

template <typename numberType>
void circle<numberType>::intersect( circle* win )
{
  if (m_R == win->m_R)
  {
    for (typename std::list<circle<numberType> >::iterator it = win->intersectionList.begin(); it != win->intersectionList.end(); ++it)
    {
      bool flag = true;
      for (typename std::list<circle<numberType> >::iterator ot = intersectionList.begin(); ot != intersectionList.end(); ++ot)
      {
        if ((ot->m_x == it->m_x) && (ot->m_y == it->m_y))
        {
          flag = false;
        }
      }
      if (flag)
      {
        intersectionList.push_back(*it);
      }
    }
  }
  createPolygon();
}

template <typename numberType>
void circle<numberType>::intersect( Cpoint<numberType> center )
{
  circle moving = *this;
  moving.emptyIntersectionList();
  moving.center(center);
  this->intersect(&moving);
}

template <typename numberType>
rhombus<numberType>* circle<numberType>::inscribed()
{
  numberType size = m_R*numberType::inscribedRhombusToCircle();;
  
  rhombus<numberType>* insc = new rhombus<numberType>( size, size, m_x, m_y );
  insc->center( Cpoint<numberType>( m_x, m_y ) );
  return insc;
}

template <typename numberType>
rhombus<numberType>* circle<numberType>::circumscribed()
{
  numberType size = m_R*numberType::circumscribedRhombusToCircle();
  
  rhombus<numberType>* circ = new rhombus<numberType>( size, size, m_x, m_y );
  circ->center( Cpoint<numberType>( m_x, m_y ) );
  return circ;
}

template <typename numberType>
void circle<numberType>::center( Cpoint<numberType> center )
{
  m_x = center.getX();
  m_y = center.getY();
  intersectionList.begin()->m_x = center.getX();
  intersectionList.begin()->m_y = center.getY();
}

template <typename numberType>
void circle<numberType>::emptyIntersectionList()
{
  intersectionList.clear();
  intersectionList.push_front(*this);
}

template <typename numberType>
bool circle<numberType>::empty() 
{
  if (intersectionList.size() == 1)
  {
    return false;
  }
  
  return polygon.size() < 2;
}

template <typename numberType>
double circle<numberType>::centerX()
{
  if (intersectionList.size() == 1)
  {
    return m_x;
  }
  
  double x = 0;
  for (std::list<Cpoint<double> >::iterator it = polygon.begin(); it != polygon.end(); ++it)
  {
    x+= it->getX();
  }
  
  return x/polygon.size();
}

template <typename numberType>
double circle<numberType>::centerY()
{
  if (intersectionList.size() == 1)
  {
    return m_y;
  }
  
  double y = 0;
  for (std::list<Cpoint<double> >::iterator it = polygon.begin(); it != polygon.end(); ++it)
  {
    y+= it->getY();
  }
  
  return y/polygon.size();
}


template <typename numberType>
void circle<numberType>::createPolygon()
{
  polygon.clear();
  
  for (typename std::list<circle<numberType> >::const_iterator it = intersectionList.begin(); it != intersectionList.end(); ++it)
  {
    double angle01 = 0; //start
    double angle02 = 4*M_PI; // interval   <--- deliberately larger 
    
    double count = 1;
    
    for (typename std::list<circle<numberType> >::const_iterator ot = intersectionList.begin(); ot != intersectionList.end(); ++ot)
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


template <typename numberType>
bool diff(circle<numberType>& larger, circle<numberType> smaller)
{
  CpointSet<double> cut = smaller.polygon;
  bool flag = false;
  
  // prepair cut points
  for (std::list<Cpoint<double> >::iterator it = cut.begin(); it != cut.end();)
  {
    std::list<Cpoint<double> >::iterator ot; //= std::find(larger.polygon.begin(), larger.polygon.end(), *it);
    for (ot = larger.polygon.begin(); ot != larger.polygon.end(); ot++)
    {
      if (*ot == *it)
      {
        break;
      }
    }
    
    if (ot != larger.polygon.end())
    {
      it = cut.removePoint(it);
      larger.polygon.removePoint(ot);
      flag = true;
    }
    else
    {
      ++it;
    }
  }
  
  // cut
  for (std::list<Cpoint<double> >::iterator it = cut.begin(); it != cut.end(); ++it)
  {
    larger.polygon.addPoint(*it);
  }
  larger.polygon.sortClockwise();
  
  return flag;
}


// POLYGON -------------------------------------------------------------
template <typename numberType>
polygon<numberType>::polygon()
{
  
}

template <typename numberType>
polygon<numberType>::polygon(CpointSet<numberType> vert)
{
  m_vert = vert;
  m_vert.sortClockwise();
  for (typename std::list<Cpoint<numberType> >::iterator it = m_vert.begin(); it != m_vert.end(); ++it)
  {
    m_center.set(m_center.getX() + it->getX(), m_center.getY() + it->getY());
  }
  m_center.set(m_center.getX()/numberType::get(m_vert.size(),0), m_center.getY()/numberType::get(m_vert.size(),0));
  
  backup_vert = m_vert;
}

template <typename numberType>
polygon<numberType>::polygon(CpointSet<numberType> vert, Cpoint<numberType> center)
{
  m_vert = vert;
  m_center = center;
  
  m_vert.sortClockwise();
  
  backup_vert = m_vert;
}

template <typename numberType>
void polygon<numberType>::operator = (const polygon<numberType> &polygon)
{
  this->m_vert = polygon.m_vert;
  this->m_center = polygon.m_center;
  this->backup_vert = polygon.backup_vert;
}

template <typename numberType>
void polygon<numberType>::svg( std::ostream& out )
{
  out << "<polygon points=\"";
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(); it != m_vert.end(); ++it)
  {
    out << it->getX() << "," << it->getY() << " ";
  }
  out << "\" style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << "\"/>" << std::endl;
}

template <typename numberType>
void polygon<numberType>::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

template <typename numberType>
bool polygon<numberType>::in( Cpoint<numberType> star )
{
  bool c = false;
  // sending horizontal rays, flipping c on boundary intersection: http://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(), ot = --m_vert.end(); it != m_vert.end(); ot = it++) 
  {
    // check border
    if ( (max(it->getX(), ot->getX()) >= star.getX()) && (min(it->getX(), ot->getX()) <= star.getX()) && (max(it->getY(), ot->getY()) >= star.getY()) && (min(it->getY(), ot->getY()) <= star.getY()) \
      && ( (ot->getX() - it->getX()) * (star.getY() - it->getY()) == (star.getX() - it->getX()) * (ot->getY() - it->getY()) ) \
       )
    {
      return false;
    }
    
    if ( ((it->getY() > star.getY()) != (ot->getY() > star.getY()) ) \
      && ( (star.getX()-it->getX()) < ((ot->getX()-it->getX()) * (star.getY()-it->getY()) / (ot->getY()-it->getY())) ) \
       )
    {
      c = !c;
    }
  }
  
  return c;
}


template <typename numberType>
bool polygon<numberType>::inClose( Cpoint<numberType> star )
{
  bool c = false;
  // sending horizontal rays, flipping c on boundary intersection: http://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(), ot = --m_vert.end(); it != m_vert.end(); ot = it++) 
  {
    // check border
    if ( (max(it->getX(), ot->getX()) >= star.getX()) && (min(it->getX(), ot->getX()) <= star.getX()) && (max(it->getY(), ot->getY()) >= star.getY()) && (min(it->getY(), ot->getY()) <= star.getY()) \
      && ( (ot->getX() - it->getX()) * (star.getY() - it->getY()) == (star.getX() - it->getX()) * (ot->getY() - it->getY()) ) \
       )
    {
      return true;
    }
    
    if ( ((it->getY() > star.getY()) != (ot->getY() > star.getY()) ) \
      && ( (star.getX()-it->getX()) < ((ot->getX()-it->getX()) * (star.getY()-it->getY()) / (ot->getY()-it->getY())) ) \
       )
    {
      c = !c;
    }
  }
  
  return c;
}


template <typename numberType>
void polygon<numberType>::intersect( polygon* win )
{
  // assume one intersecting area
  CpointSet<numberType> new_vert;
  
  for (typename std::list<Cpoint<numberType> >::const_iterator ot = win->m_vert.begin(); ot != win->m_vert.end(); ++ot) 
  {
    if (this->inClose(*ot))
    {
      new_vert.addPoint(*ot);
    }
  }
  
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(), itold = --m_vert.end(); it != m_vert.end(); itold = it++) 
  {
    if (win->inClose(*it))
    {
      new_vert.addPoint(*it);
    }
    
    if (win->inClose(*it) != win->in(*itold))
    {
      // search for two points dividing this edge
      for (typename std::list<Cpoint<numberType> >::const_iterator ot = win->m_vert.begin(), otold = --win->m_vert.end(); ot != win->m_vert.end(); otold = ot++) 
      {
        if ( ( sign( ((it->getX()-ot->getX())*(otold->getY()-ot->getY())-(it->getY()-ot->getY())*(otold->getX()-ot->getX())) ) \
            != sign( ((itold->getX()-ot->getX())*(otold->getY()-ot->getY())-(itold->getY()-ot->getY())*(otold->getX()-ot->getX())) ) \
             ) \
          && ( sign( ((ot->getX()-it->getX())*(itold->getY()-it->getY())-(ot->getY()-it->getY())*(itold->getX()-it->getX())) ) \
            != sign( ((otold->getX()-it->getX())*(itold->getY()-it->getY())-(otold->getY()-it->getY())*(itold->getX()-it->getX())) ) \
             ) \
           )
        {
          // add intersection of edges to new vertexes https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
          numberType a = it->getX()*itold->getY()-it->getY()*itold->getX();
          numberType b = ot->getX()*otold->getY()-ot->getY()*otold->getX();
          numberType c = (it->getX()-itold->getX())*(ot->getY()-otold->getY()) - (it->getY()-itold->getY())*(ot->getX()-otold->getX()); //denominator
          Cpoint<numberType> vertex;
          vertex.setX((a*(ot->getX()-otold->getX())-b*(it->getX()-itold->getX()))/c);
          vertex.setY((a*(ot->getY()-otold->getY())-b*(it->getY()-itold->getY()))/c);
          new_vert.addPoint(vertex);
        }
      }
    }
  }
  new_vert.sort();
  new_vert.unique();
  new_vert.sortClockwise();
  
  m_center = Cpoint<numberType>(0,0);
  for (typename std::list<Cpoint<numberType> >::const_iterator it = new_vert.begin(); it != new_vert.end(); ++it) 
  {
    m_center.set(m_center.getX() + it->getX(), m_center.getY() + it->getY());
  }
  m_center.set(m_center.getX()/numberType::get(new_vert.size(),0), m_center.getY()/numberType::get(new_vert.size(),0));
  
  m_vert = new_vert;
}

template <typename numberType>
void polygon<numberType>::intersect( Cpoint<numberType> center )
{
  polygon<numberType> moving(backup_vert);
  moving.center(center);
  this->intersect(&moving);
}

template <typename numberType>
rhombus<numberType>* polygon<numberType>::inscribed()
{
  numberType size = max((m_vert.begin()->getX()-m_center.getX()).abs(), (m_vert.begin()->getY()-m_center.getY()).abs());
  
  // radius of inscribed circle
  for (typename std::list<Cpoint<numberType> >::const_iterator it = ++m_vert.begin(); it != m_vert.end(); ++it)
  {
    size = min(max((it->getX()-m_center.getX()).abs(), (it->getY()-m_center.getY()).abs()), size);
  }
  size = size*numberType::inscribedRhombusToCircle();
  
  rhombus<numberType>* insc = new rhombus<numberType>(size, size);
  insc->center(m_center);
  return insc;
}

template <typename numberType>
rhombus<numberType>* polygon<numberType>::circumscribed()
{
  numberType size;
  
  // radius of circumscribed circle
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(); it != m_vert.end(); ++it)
  {
      size = max((it->getX()-m_center.getX()).abs() + (it->getY()-m_center.getY()).abs(), size);
  }
  
  size = size*numberType::circumscribedRhombusToCircle();
  
  rhombus<numberType>* circ = new rhombus<numberType>(size, size);
  circ->center(m_center);
  return circ;
}

template <typename numberType>
void polygon<numberType>::center( Cpoint<numberType> center )
{
  for (typename std::list<Cpoint<numberType> >::iterator it = m_vert.begin(); it != m_vert.end(); ++it)
  {
    it->setX(it->getX() - m_center.getX() + center.getX());
    it->setY(it->getY() - m_center.getY() + center.getY());
  }
  m_center = center;
}

template <typename numberType>
bool polygon<numberType>::empty() 
{
  numberType area;
  
  if (m_vert.size() < 3)
  {
    return true;
  }
  
  for (typename std::list<Cpoint<numberType> >::const_iterator it = m_vert.begin(), itold = --m_vert.end(); it != m_vert.end(); itold = it++) 
  {
    area+= itold->getX()*it->getY() - it->getX()*itold->getY();
  }
  
  return abs(area) == 0;
}

template <typename numberType>
double polygon<numberType>::centerX()
{
  return m_center.getX();
}

template <typename numberType>
double polygon<numberType>::centerY()
{
  return m_center.getY();
}

template <typename numberType>
numberType polygon<numberType>::size()
{
  
  numberType area = numberType::get(0,0);
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = m_vert.begin(), itold = --m_vert.end(); it != m_vert.end(); itold = it++ )
  {
    area+= itold->getX()*it->getY() - it->getX()*itold->getY();
  }
  
  return area.abs()*numberType::get(1,0,2);
}

template <typename numberType>
bool polygon<numberType>::operator < (polygon<numberType> &compare)
{
  return this->size() < compare.size();
}

template <typename numberType>
bool diff(polygon<numberType>& larger, polygon<numberType> smaller)
{
  //std::cout << smaller.m_vert.size() << std::endl << std::flush;
  // assume one area
  CpointSet<numberType> new_vert;
  
  // start larger outside smaller
  {
    Cpoint<numberType> tmp = *larger.m_vert.begin();
    while (smaller.inClose(*(--larger.m_vert.end())))
    {
      larger.m_vert.push_back(*larger.m_vert.begin());
      larger.m_vert.removePoint(larger.m_vert.begin());
      
      if (tmp == *larger.m_vert.begin())
      {
        larger.m_vert = new_vert; // empty
        return true;
      }
    }
  }
  
  // add points of smaller to the bigger
  for (typename std::list<Cpoint<numberType> >::iterator it = larger.m_vert.begin(), itold = --larger.m_vert.end(); it != larger.m_vert.end(); itold = it++) 
  {
    new_vert.push_back(*itold);
    for (typename std::list<Cpoint<numberType> >::iterator ot = smaller.m_vert.begin(); ot != smaller.m_vert.end(); ++ot) 
    {
      // add points on edge it--itold
      if (((ot->getY()-it->getY())*(itold->getX()-it->getX()) == (ot->getX()-it->getX())*(itold->getY()-it->getY())) \
        && (max(it->getX(),itold->getX()) > ot->getX()) && (min(it->getX(),itold->getX()) < ot->getX()) \
        && (max(it->getY(),itold->getY()) > ot->getY()) && (min(it->getY(),itold->getY()) < ot->getY()) \
         )
      {
        new_vert.push_back(*ot);
      }
    }
  }
  
  larger.m_vert = new_vert;
  new_vert.clear();
  
  larger.m_vert.push_back(*larger.m_vert.begin());
  larger.m_vert.removePoint(larger.m_vert.begin());
  
  typename std::list<Cpoint<numberType> >::iterator ot = smaller.m_vert.end();
  
  for (typename std::list<Cpoint<numberType> >::iterator it = larger.m_vert.begin(), itold = --larger.m_vert.end(); it != larger.m_vert.end(); itold = it++) 
  {
    std::cout << "FOR " << std::distance(larger.m_vert.begin(), it) << std::endl << std::flush;
    if (ot == smaller.m_vert.end())
    {
      new_vert.push_back(*itold);
    }
    
    // entering the smaller
    if ((smaller.inClose(*it)) && (ot == smaller.m_vert.end()))
    {
      std::cout << "IF" << std::endl << std::flush;
      // search for point of smaller on the edge itold--it; make it begin
      ot = smaller.m_vert.begin();
      Cpoint<numberType> tmp = *ot;
      while (*ot != *it)
      {
        std::cout << "WHILE smaller" << std::endl << std::flush;
        smaller.m_vert.push_back(*ot);
        ot = smaller.m_vert.removePoint(ot); //if there is no such point, there is troble elsewhere
        if (*ot == tmp)
        {
          std::cout << "OH NOOOO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
          return false;
        }
      }
    }
    
    std::cout << "test: " << (!smaller.inClose(*it)) << (ot != smaller.m_vert.end()) << std::endl << std::flush;
    if ((!smaller.inClose(*it)) && (ot != smaller.m_vert.end()))
    {
      std::cout << "IF add smaller" << std::endl << std::flush;
      // fill in points of smaller, counterclockwise
      new_vert.push_back(*ot);
      typename std::list<Cpoint<numberType> >::reverse_iterator ut = smaller.m_vert.rbegin();
      while (*ut != *itold)
      {
        std::cout << "add" << std::endl << std::flush;
        new_vert.push_back(*ut);
        ++ut;
      }
      new_vert.push_back(*ut);
      
      ot = smaller.m_vert.end();
    }
    
    std::cout << "larger vert size: " << larger.m_vert.size() << std::endl << std::flush;
    std::cout << "new vert size: " << new_vert.size() << std::endl << std::flush;
  }
  
  new_vert.unique();
  
  larger.m_center = Cpoint<numberType>(0,0);
  for (typename std::list<Cpoint<numberType> >::const_iterator it = new_vert.begin(); it != new_vert.end(); ++it) 
  {
    larger.m_center.set(larger.m_center.getX() + it->getX(), larger.m_center.getY() + it->getY());
  }
  larger.m_center.set(larger.m_center.getX()/numberType::get(new_vert.size(),0), larger.m_center.getY()/numberType::get(new_vert.size(),0));
  
  larger.m_vert = new_vert;
  std::cout << "DIFF DONE" << std::endl << std::flush;
}






template <typename numberType>
polygon<numberType> polygon<numberType>::octagon(numberType size)
{
  CpointSet<numberType> vert;
  vert.addPoint(Cpoint<numberType>(size*numberType::get(1,0,1),  size*numberType::get(0,0,1)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(-1,1,2), size*numberType::get(-1,1,2)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(0,0,1),  size*numberType::get(1,0,1)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(1,-1,2), size*numberType::get(-1,1,2)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(-1,0,1), size*numberType::get(0,0,1)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(1,-1,2), size*numberType::get(1,-1,2)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(0,0,1),  size*numberType::get(-1,0,1)));
  vert.addPoint(Cpoint<numberType>(size*numberType::get(-1,1,2), size*numberType::get(1,-1,2)));
  
  return polygon<numberType>(vert);
}

template <typename numberType>
polygon<numberType> polygon<numberType>::rhombic(numberType size)
{
  CpointSet<numberType> vert;
  vert.push_back(Cpoint<numberType>(numberType::get(0,0), numberType::get(0,0)));
  vert.push_back(Cpoint<numberType>(numberType::windowA()*size, numberType::windowC()*size));
  vert.push_back(Cpoint<numberType>(numberType::windowA()*size + numberType::windowB()*size, numberType::windowC()*size + numberType::windowD()*size));
  vert.push_back(Cpoint<numberType>(numberType::windowB()*size, numberType::windowD()*size));
  
  return polygon<numberType>(vert);
}


// EDUARD -----------------------------------------------------------
template <typename numberType>
eduard<numberType>::eduard()
{
  m_width  = numberType::get( 1, 0 );
  m_height = numberType::get( 1, 0 );
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
}

template <typename numberType>
eduard<numberType>::eduard( numberType width, numberType height )
{
  m_width  = width;
  m_height = height;
  m_x = numberType::get( 0, 0 );
  m_y = numberType::get( 0, 0 );
}

template <typename numberType>
eduard<numberType>::eduard( numberType width, numberType height, numberType x ,numberType y )
{
  m_width  = width;
  m_height = height;
  m_x = x;
  m_y = y;
}

template <typename numberType>
void eduard<numberType>::svg( std::ostream& out ) const
{
  out << "<polygon points=\"";
  out << m_x << "," << m_y << " ";
  out << m_x - m_width << "," << m_y << " ";
  out << m_x - m_width << "," << m_y + m_height << " ";
  out << m_x << "," << m_y + m_height << " ";
  out << m_x << "," << m_y + numberType::get(4,0,5)*m_height << " ";
  out << m_x - numberType::get(2,0,3)*m_width << "," << m_y + numberType::get(4,0,5)*m_height << " ";
  out << m_x - numberType::get(2,0,3)*m_width << "," << m_y + numberType::get(3,0,5)*m_height << " ";
  out << m_x << "," << m_y + numberType::get(3,0,5)*m_height << " ";
  out << m_x << "," << m_y + numberType::get(2,0,5)*m_height << " ";
  out << m_x - numberType::get(2,0,3)*m_width << "," << m_y + numberType::get(2,0,5)*m_height << " ";
  out << m_x - numberType::get(2,0,3)*m_width << "," << m_y + numberType::get(1,0,5)*m_height << " ";
  out << m_x << "," << m_y + numberType::get(1,0,5)*m_height << "\"";
  out << " style=\"fill:" << m_fillColor << ";stroke:" << m_strokeColor << ";stroke-width:" << m_strokeWidth << "\" />" << std::endl;
}

template <typename numberType>
void eduard<numberType>::setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth )
{
  m_fillColor = fillColor;
  m_strokeColor = strokeColor;
  m_strokeWidth = strokeWidth;
}

template <typename numberType>
bool eduard<numberType>::in( Cpoint<numberType> star )const
{
  numberType x = star.getX();
  numberType y = star.getY();
  
  return !(((m_x - numberType::get(2,0,3)*m_width < x) && (x < m_x)) && (((m_y + numberType::get(1,0,5)*m_height < y) && (y < m_y + numberType::get(2,0,5)*m_height)) || ((m_y + numberType::get(3,0,5)*m_height < y) && (y < m_y + numberType::get(4,0,5)*m_height)))) && (((m_x - m_width < x) && (x < m_x)) && ((m_y + m_height > y) && (y > m_y))) ;
  return (((m_x - m_width < x) && (x < m_x)) && ((m_y + m_height > y) && (y > m_y)));
}

template <typename numberType>
void eduard<numberType>::intersect( eduard* win )
{
  
}

template <typename numberType>
void eduard<numberType>::intersect( Cpoint<numberType> center )
{
  eduard moving = *this;
  moving.center(center);
  this->intersect(&moving);
}

template <typename numberType>
rhombus<numberType>* eduard<numberType>::inscribed() const
{
  numberType size = numberType::get( -3, 1, 4 )*m_width;
  
  rhombus<numberType>* insc = new rhombus<numberType>( size, size, m_x, m_y );
  insc->center( Cpoint<numberType>( m_x, m_y ) );
  return insc;
}

template <typename numberType>
rhombus<numberType>* eduard<numberType>::circumscribed() const
{
  numberType size = numberType::get( -2, 1 )*m_height + m_width;
  
  if (numberType::get( 2, 0 )*m_height > numberType::get( -2, 1 )*m_height + m_width)
  {
    size = numberType::get( 2, 0 )*m_height;
  }
  
  rhombus<numberType>* circ = new rhombus<numberType>( size, size );
  circ->center(Cpoint<numberType>(m_x - numberType::get(1,0,2)*m_width, m_y + numberType::get(1,0,2)*m_height));
  return circ;
}

template <typename numberType>
void eduard<numberType>::center( Cpoint<numberType> center )
{
  m_x = center.getX() + numberType::get(1,0,2)*m_width;
  m_y = center.getY() - numberType::get(1,0,2)*m_height;
}

template <typename numberType>
bool eduard<numberType>::empty() 
{
  
}








#endif
