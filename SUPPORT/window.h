#ifndef _WINDOW_
#define _WINDOW_

#include <cstring>
#include <iostream>
#include <cstdio>
#include "geometricObject2.h"
#include "betaSet.h"


class window2D {
  public:
    virtual bool in( Cpoint<betaSet> star ) const = 0;
    virtual void intersect( window2D* win );
    virtual void intersect( Cpoint<betaSet> center );
    virtual void svg( std::ostream& out ) const;
    virtual window2D* inscribed() const;
    virtual window2D* circumscribed() const;
    virtual void center( Cpoint<betaSet> center );
    virtual bool empty();
};

class rhombus : public window2D {
  protected:
    betaSet m_height;
    betaSet m_width;
    betaSet m_x; // left buttom corner
    betaSet m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    rhombus();
    rhombus( betaSet width );
    rhombus( betaSet width, betaSet height );
    rhombus( betaSet width, betaSet height, betaSet x ,betaSet y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool operator == ( const rhombus &Input )const;
    
    bool in( Cpoint<betaSet> star )const;
    void intersect( rhombus* win );
    void intersect( Cpoint<betaSet> center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<betaSet> center );
    
    window Xwindow()const;
    window Ywindow()const;
    betaSet large()const;
    
    bool empty();
    void flip(int mode);
    
    betaSet centerX()const;
    betaSet centerY()const;
    betaSet width()const;
    betaSet height()const;
    
    friend bool diff(rhombus& larger, rhombus smaller);
};

class circle : public window2D {
  protected:
    betaSet m_R;
    betaSet m_x; // center
    betaSet m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
    
    std::list<circle> intersectionList;
  public:
    circle();
    circle( betaSet R );
    circle( betaSet R, betaSet x ,betaSet y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<betaSet> star )const;
    void intersect( circle* win );
    void intersect( Cpoint<betaSet> center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<betaSet> center );
    void emptyIntersectionList();
    
    bool empty();
};

class dodecagonTip : public window2D {
  protected:
    betaSet m_a;
    betaSet m_x; // center
    betaSet m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    dodecagonTip();
    dodecagonTip( betaSet a );
    dodecagonTip( betaSet a, betaSet x ,betaSet y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<betaSet> star )const;
    void intersect( dodecagonTip* win );
    void intersect( Cpoint<betaSet> center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<betaSet> center );
    
    bool empty();
};

class eduard : public window2D {
  protected:
    betaSet m_height;
    betaSet m_width;
    betaSet m_x; // right buttom corner
    betaSet m_y;
    
    std::string m_fillColor;  // format rgb(R,G,B)
    std::string m_strokeColor;
    std::string m_strokeWidth;
  public:
    eduard();
    eduard( betaSet width, betaSet height );
    eduard( betaSet width, betaSet height, betaSet x ,betaSet y );
    void svg( std::ostream& out ) const;
    void setColor( const std::string fillColor, const std::string strokeColor, const std::string strokeWidth );
    
    bool in( Cpoint<betaSet> star )const;
    void intersect( eduard* win );
    void intersect( Cpoint<betaSet> center );
    
    rhombus* inscribed() const;
    rhombus* circumscribed() const;
    
    void center( Cpoint<betaSet> center );
    
    bool empty();
};
#endif
