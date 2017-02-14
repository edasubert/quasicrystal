#ifndef _WINDOW_
#define _WINDOW_

#include <cstring>
#include <iostream>
#include <cstdio>
#include "geometricObject2.h"
#include "betaSet.h"

typedef betaSet numberType;


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





class window2D {
  public:
    virtual bool in( Cpoint<numberType > star ) const = 0;
    virtual void intersect( window2D* win );
    virtual void intersect( Cpoint<numberType > center );
    virtual void svg( std::ostream& out ) const;
    virtual window2D* inscribed() const;
    virtual window2D* circumscribed() const;
    virtual void center( Cpoint<numberType > center );
    virtual bool empty();
};

class rhombus : public window2D {
  protected:
    numberType  m_height;
    numberType  m_width;
    numberType  m_x; // left buttom corner
    numberType  m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    rhombus();
    rhombus( numberType  width );
    rhombus( numberType  width, numberType  height );
    rhombus( numberType  width, numberType  height, numberType  x ,numberType  y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool operator == ( const rhombus &Input )const;
    
    bool in( Cpoint<numberType > star )const;
    void intersect( rhombus* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<numberType > center );
    
    window Xwindow()const;
    window Ywindow()const;
    numberType  large()const;
    
    bool empty();
    void flip(int mode);
    
    numberType  centerX()const;
    numberType  centerY()const;
    numberType  width()const;
    numberType  height()const;
    
    friend bool diff(rhombus& larger, rhombus smaller);
};

class circle : public window2D {
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
    
    bool in( Cpoint<numberType > star )const;
    void intersect( circle* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<numberType > center );
    void emptyIntersectionList();
    
    bool empty();
    
    numberType  centerX()const;
    numberType  centerY()const;
    
    void createPolygon();
    
    friend bool diff(circle& larger, circle smaller);
};

class dodecagonTip : public window2D {
  protected:
    numberType  m_a;
    numberType  m_x; // center
    numberType  m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    dodecagonTip();
    dodecagonTip( numberType  a );
    dodecagonTip( numberType  a, numberType  x ,numberType  y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<numberType > star )const;
    void intersect( dodecagonTip* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<numberType > center );
    
    bool empty();
};

class eduard : public window2D {
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
    void intersect( eduard* win );
    void intersect( Cpoint<numberType > center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<numberType > center );
    
    bool empty();
};
#endif
