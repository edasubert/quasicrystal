#ifndef _GEOMETRICoBJECT_
#define _GEOMETRICoBJECT_

#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <cctype>
#include <math.h> 
#include <fstream>
#include <sstream>
#include <functional>


#include "betaSet.h"


//#define _ERROR_
#define _IMG_OUTPUT_


template <typename numberType>
class Cfigure
{
  protected:
    std::string name;
    std::string description;
    
    std::string fillColor;  // format rgb(R,G,B)
    std::string strokeColor;
    std::string strokeWidth;
    
    
  public:
    Cfigure();
    void var_dump( std::ostream& out ) const;
    void setDescription( const std::string input );
    std::string getDescription();
    
    void setColor( const std::string I_fillColor, const std::string I_strokeColor, const std::string I_strokeWidth );
    void setFillColor( const std::string I_fillColor );
    std::string getFillColor();
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    void load( std::istream& in );
};


template <typename numberType>
class Cpoint : public virtual Cfigure<numberType>
{
  protected:
    numberType x;
    numberType y;
  public:
    Cpoint();
    Cpoint( const Cpoint<numberType>& I_Point );
    Cpoint( numberType xCoord, numberType yCoord );
    
    Cpoint& operator = ( const Cpoint& I_Point );
    
    void var_dump( std::ostream& out ) const;
    
    numberType getX() const;
    numberType getY() const;
    
    void setX( numberType xCoord );
    void setY( numberType yCoord );
    
    void set( numberType xCoord, numberType yCoord );
    
    Cpoint<numberType> operator * ( const int multiplicator )const;
    Cpoint<numberType> operator / ( const int divider )const;
    
    bool operator == ( const Cpoint<numberType> &compare ) const;
    bool operator != ( const Cpoint<numberType> &compare ) const;
    bool operator <  ( const Cpoint<numberType> &compare ) const;

    Cpoint<numberType> operator - ( const Cpoint<numberType> &term ) const;
    
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    void load( std::istream& in );
    
    Cpoint<numberType> star()const;
};


template <typename numberType>
class Cvector : public virtual Cpoint<numberType>
{
  public:
    Cvector();
    Cvector( const Cpoint<numberType>& I_Point );
    Cvector( const Cvector& I_Vector );
    Cvector( numberType xCoord, numberType yCoord );
    
    Cvector operator * ( const int multiplicator )const;
    Cvector operator * ( const numberType multiplicator )const;
    Cvector operator / ( const int divider )const;
    Cvector operator + ( const Cpoint<numberType> term )const;
    Cvector operator - ( const Cpoint<numberType> term )const;
    
    Cvector operator * ( const Cvector term )const;
    
    numberType sum()const;
    
    void var_dump( std::ostream& out ) const;
    
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    void load( std::istream& in );
};


template <typename numberType>
class CpointSet : public virtual Cfigure<numberType>
{
  public:
    std::list<Cpoint<numberType> >* points;
    
    CpointSet();
    CpointSet( const CpointSet& I_PointSet );
    ~CpointSet();
    
    void var_dump( std::ostream& out ) const;
    
    void addPoint( Cpoint<numberType> addition );
    void push_front( Cpoint<numberType> addition );
    void push_back( Cpoint<numberType> addition );
    void insert(typename std::list<Cpoint<numberType> >::iterator pos, Cpoint<numberType> addition);
    typename std::list<Cpoint<numberType> >::iterator removePoint( typename std::list<Cpoint<numberType> >::iterator removal );
    
    CpointSet& operator << ( Cpoint<numberType> addition );
    CpointSet& operator << ( CpointSet<numberType> addition );
    CpointSet& operator = ( const CpointSet& I_PointSet );
    
    typename std::list<Cpoint<numberType> >::iterator begin();
    typename std::list<Cpoint<numberType> >::iterator end();
    
    typename std::list<Cpoint<numberType> >::reverse_iterator rbegin();
    typename std::list<Cpoint<numberType> >::reverse_iterator rend();
    
    void transform( const numberType a, const numberType b, const numberType c, const numberType d );
    
    const Cpoint<numberType>& front() const;
    const Cpoint<numberType>& back() const;
    
    void clear();
    int size() const;
    void sort();
    void unique();
    void reverse();
    
    CpointSet<numberType>& operator + ( const Cpoint<numberType>& point );
    CpointSet<numberType>& operator - ( const Cpoint<numberType>& point );
    CpointSet<numberType>& operator * ( const numberType& multiplicator );
    
    CpointSet<numberType>& operator+= ( const Cpoint<numberType>& point );
    CpointSet<numberType>& operator-= ( const Cpoint<numberType>& point );
    
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    std::string save() const;
    void load( std::string in );
    void load( std::istream& in );
    
    CpointSet<numberType> star()const;
    void filterDistanceOrigin(const numberType dist);
    
    void sortClockwise();
};

template <typename numberType>
class CdeloneSet : public CpointSet<numberType>, public virtual Cfigure<numberType>
{
  protected:
    double PackingR;
    double CoveringR;
  public:
    CdeloneSet();
    CdeloneSet( const CdeloneSet<numberType>& I_PointSet );
    
    void var_dump( std::ostream& out ) const;
    
    CdeloneSet<numberType>& operator = ( const CdeloneSet<numberType>& I_DeloneSet );
    
    void setPackingR( double I_PackingR );
    void setCoveringR( double I_CoveringR );
    
    double getPackingR();
    double getCoveringR();
    
    void setPackingR();
    void setCoveringR();
    
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    std::string save() const;
    void load( std::string in );
    void load( std::istream& in );
    
    CdeloneSet<numberType>& operator + ( const Cpoint<numberType>& point );
    CdeloneSet<numberType>& operator - ( const Cpoint<numberType>& point );
    CdeloneSet<numberType>& operator * ( const numberType& multiplicator );
    
    bool operator == ( const CdeloneSet<numberType> &compare ) const;
    bool operator <  ( const CdeloneSet<numberType> &compare ) const;
    
    CdeloneSet<numberType> star()const;
    void filterDistanceOrigin(const numberType dist);
    void sort();
    void sortByDistance();
    
    std::list<Cpoint<numberType> > getPoints()const;
};


template <typename numberType>
class CvoronoiCell : public virtual Cfigure<numberType>
{
  protected: 
    bool constructCut( typename std::list<Cpoint<numberType> >::iterator add );
    bool filterPoint( Cpoint<numberType> add );
    bool filterPointAny( Cpoint<numberType> add );
  public:
    CdeloneSet<numberType> *CarrierSet;
    CpointSet<numberType> *Cell;
    Cpoint<numberType> Center;
  public:
    static numberType large;
    CvoronoiCell();
    CvoronoiCell( const CdeloneSet<numberType>& I_CarrierSet, const Cpoint<numberType> I_Center );
    CvoronoiCell( const CdeloneSet<numberType>& I_CarrierSet );
    CvoronoiCell( const CvoronoiCell<numberType>& I_VoronoiCell);
    ~CvoronoiCell();
    
    
    void setCenter( const Cpoint<numberType> I_Center );
    
    void var_dump( std::ostream& out ) const;
    
    void construct();
    bool constructAdd( const Cpoint<numberType> add );
    void filterSet();
    void filterSetPotential( std::list<Cpoint<numberType> >* points );
    
    CvoronoiCell<numberType>& operator = ( const CvoronoiCell<numberType>& I_VoronoiCell );
    
    bool operator == ( const CvoronoiCell<numberType> &compare ) const;
    bool operator < ( const CvoronoiCell<numberType> &compare ) const;
    
    void svg( std::ostream& out ) const;
    void svg( std::ostream& out, double a, double b, double c, double d ) const;
    
    void save( std::ostream& out ) const;
    std::string save()const;
    void load( std::string in );
    void load( std::istream& in );
    
    double value() const;
    numberType size() const;
    numberType radius() const;
    Cpoint<numberType> middle() const;
    Cpoint<numberType> middleDomain() const;
    void colorify();
    
    CvoronoiCell<numberType> rotate(int n) const;
    CvoronoiCell<numberType> flip() const;
    CvoronoiCell<numberType> flop() const;
    
    std::string dna() const;
    
    CvoronoiCell<numberType> star()const;
};

template <typename numberType>
double euklid( const Cpoint<numberType>& a, const Cpoint<numberType>& b );

template <typename numberType>
double euklid2( const Cpoint<numberType>& a, const Cpoint<numberType>& b );

template <typename numberType>
double euklid2( const Cpoint<numberType>& a );

template <typename numberType>
bool distanceComp( const Cpoint<numberType>& a, const Cpoint<numberType>& b );

template <typename numberType>
bool clockwiseComp( const Cpoint<numberType>& a, const Cpoint<numberType>& b );

template <typename numberType>
bool dnaComp( const Cpoint<numberType>& a, const Cpoint<numberType>& b );


// FIGURE ==============================================================
template <typename numberType>
Cfigure<numberType>::Cfigure()
{
  this->name = "[UNSPECIFIED_FIGURE]";
  description = "[UNSPECIFIED_FIGURE]";
}

template <typename numberType>
void Cfigure<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "====================" << std::endl; 
  out << "Name: " << this->name << std::endl << "Description: " << description << std::endl; 
  out << "====================" << std::endl;
}

template <typename numberType>
void Cfigure<numberType>::setDescription( const std::string input )
{
  this->description = input;
}

template <typename numberType>
std::string Cfigure<numberType>::getDescription()
{
  return description;
}

template <typename numberType>
void Cfigure<numberType>::setColor( const std::string I_fillColor, const std::string I_strokeColor, const std::string I_strokeWidth )
{
  this->fillColor = I_fillColor;
  this->strokeColor = I_strokeColor;
  this->strokeWidth = I_strokeWidth;
}

template <typename numberType>
void Cfigure<numberType>::setFillColor( const std::string I_fillColor )
{
  this->fillColor = I_fillColor;
}

template <typename numberType>
std::string Cfigure<numberType>::getFillColor()
{
  return this->fillColor;
}



// POINT ===============================================================
template <typename numberType>
Cpoint<numberType>::Cpoint()
:Cfigure<numberType>()
{
  this->name = "Point";
  this->description = "";
  
  x = 0;
  y = 0;
  
  this->fillColor = "rgb(30,30,30)";
  this->strokeColor = "rgb(30,30,30)";
  this->strokeWidth = "10";
}

template <typename numberType>
Cpoint<numberType>::Cpoint( const Cpoint<numberType>& I_Point )
{
  this->name = "Point";
  this->description = I_Point.description;
  
  x = I_Point.x;
  y = I_Point.y;
  
  this->fillColor = I_Point.fillColor;
  this->strokeColor = I_Point.strokeColor;
  this->strokeWidth = I_Point.strokeWidth;
}

template <typename numberType>
Cpoint<numberType>::Cpoint( numberType xCoord, numberType yCoord )
{
  this->name = "Point";
  this->description = "";
  
  x = xCoord;
  y = yCoord;
  
  this->fillColor = "rgb(30,30,30)";
  this->strokeColor = "rgb(30,30,30)";
  this->strokeWidth = "10";
}

template <typename numberType>
Cpoint<numberType>& Cpoint<numberType>::operator = ( const Cpoint& I_Point )
{
  this->name = I_Point.name;
  this->description = I_Point.description;
  
  this->x = I_Point.x;
  this->y = I_Point.y;
  
  this->fillColor   = I_Point.fillColor;
  this->strokeColor = I_Point.strokeColor;
  this->strokeWidth = I_Point.strokeWidth;
}

template <typename numberType>
void Cpoint<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "====================" << std::endl; 
  out << "Name: " << this->name << std::endl;
  out << "Coordinates: X = ";
  print( out, x );
  out << " Y = ";
  print( out, y );
  out << std::endl;
  out << "Description: " << this->description << std::endl; 
  out << "====================" << std::endl;
}

template <typename numberType>
numberType Cpoint<numberType>::getX() const
{
  return x;
}

template <typename numberType>
numberType Cpoint<numberType>::getY() const
{
  return y;
}

template <typename numberType>
void Cpoint<numberType>::setX( numberType xCoord )
{
  x = xCoord;
}

template <typename numberType>
void Cpoint<numberType>::setY( numberType yCoord )
{
  y = yCoord;
}

template <typename numberType>
void Cpoint<numberType>::set( numberType xCoord, numberType yCoord )
{
  x = xCoord;
  y = yCoord;
}

template <typename numberType>
Cpoint<numberType> Cpoint<numberType>::operator * ( const int multiplicator )const
{
  Cpoint<numberType> Output;
  Output.x = x*multiplicator;
  Output.y = y*multiplicator;
  
  //Output.x = x.operator*(multiplicator);
  //Output.y = y.operator*(multiplicator);
  
  return Output;
}

template <typename numberType>
Cpoint<numberType> Cpoint<numberType>::operator / ( const int divider )const
{
  Cpoint<numberType> Output;
  Output.x = x/divider;
  Output.y = y/divider;
  
  return Output;
}

template <typename numberType>
bool Cpoint<numberType>::operator == ( const Cpoint<numberType> &compare ) const
{
  return ((x == compare.x) && (y == compare.y) && (this->description.compare(compare.description) == 0));
}

template <typename numberType>
bool Cpoint<numberType>::operator != ( const Cpoint<numberType> &compare ) const
{
  return (( x != compare.x ) || ( y != compare.y ) || (this->description.compare(compare.description) != 0));
}

template <typename numberType>
bool Cpoint<numberType>::operator <  ( const Cpoint<numberType> &compare ) const
{
  return (( x < compare.x ) || (( x == compare.x ) && ( y < compare.y )) || (( x == compare.x ) && ( y == compare.y ) && (this->description.compare(compare.description) < 0)));
}

template <typename numberType>
Cpoint<numberType> Cpoint<numberType>::operator - ( const Cpoint<numberType> &term ) const
{
  Cpoint<numberType> Output( *this );
  Output.x = x - term.x;
  Output.y = y - term.y;
  
  return Output;
}

template <typename numberType>
void Cpoint<numberType>::svg( std::ostream& out ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << this->strokeWidth << "\" stroke=\"" << this->strokeColor << "\" stroke-width=\"" << 0 << "\" fill=\"" << this->strokeColor << "\" />" << std::endl;
}

template <typename numberType>
void Cpoint<numberType>::svg( std::ostream& out, double a, double b, double c, double d ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "<circle cx=\"" << ( a*x + b*y ) << "\" cy=\"" << ( c*x + d*y ) << "\" r=\"" << this->strokeWidth << "\" stroke=\"" << this->strokeColor << "\" stroke-width=\"" << 0 << "\" fill=\"" << this->strokeColor << "\" />" << std::endl;
}

template <typename numberType>
void Cpoint<numberType>::save( std::ostream& out ) const
{
  out << "Cpoint" << std::endl;
  out << this->name << std::endl;
  out << this->description << std::endl;
  out << this->fillColor << std::endl;
  out << this->strokeColor << std::endl;
  out << this->strokeWidth << std::endl;
  print( out, this->x );
  out << std::endl;
  print( out, this->y );
  out << std::endl;
  out << "END" << std::endl;
}

template <typename numberType>
void Cpoint<numberType>::load( std::istream& in )
{
  std::string line;
  getline( in, line );
  if ( line != "Cpoint" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  getline( in, this->name );
  getline( in, this->description );
  getline( in, this->fillColor );
  getline( in, this->strokeColor );
  getline( in, this->strokeWidth );
  
  in >> this->x;
  in >> this->y;
  
  getline( in, line );
}

template <typename numberType>
Cpoint<numberType> Cpoint<numberType>::star() const
{
  numberType tmp = numberType::windowA()*numberType::windowD() - numberType::windowB()*numberType::windowC(); // denominator of inverse transform
  
  numberType X = (numberType::transformD()/tmp*x - numberType::transformB()/tmp*y);
  numberType Y = (numberType::transformA()/tmp*y - numberType::transformC()/tmp*x);
  
  Cpoint<numberType> Output( *this );
  Output.x = numberType::windowA()*X.star() + numberType::windowB()*Y.star();
  Output.y = numberType::windowC()*X.star() + numberType::windowD()*Y.star();
  Output.description = "**STARED**\n" + Output.description;
  
  return Output;
}


// VECTOR ==============================================================
template <typename numberType>
Cvector<numberType>::Cvector()
:Cpoint<numberType>::Cpoint()
{
  this->name = "Vector";
}

template <typename numberType>
Cvector<numberType>::Cvector( const Cvector<numberType>& I_Vector )
:Cpoint<numberType>::Cpoint( I_Vector )
{
  this->name = "Vector";
}

template <typename numberType>
Cvector<numberType>::Cvector( const Cpoint<numberType>& I_Point )
:Cpoint<numberType>::Cpoint( I_Point )
{
  this->name = "Vector";
}

template <typename numberType>
Cvector<numberType>::Cvector( numberType xCoord, numberType yCoord )
:Cpoint<numberType>::Cpoint( xCoord, yCoord )
{
  this->name = "Vector";
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator * ( const int multiplicator )const
{
  Cvector<numberType> Output;
  Output.x = this->x*multiplicator;//x.operator*(multiplicator);
  Output.y = this->y*multiplicator;//y.operator*(multiplicator);
  
  //Output.x = this->x.operator*(multiplicator);
  //Output.y = this->y.operator*(multiplicator);
  
  return Output;
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator * ( const numberType multiplicator )const
{
  Cvector<numberType> Output;
  Output.x = this->x*(multiplicator);
  Output.y = this->y*(multiplicator);
  
  return Output;
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator / ( const int divider )const
{
  Cvector<numberType> Output;
  Output.x = this->x/divider;
  Output.y = this->y/divider;
  
  return Output;
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator + ( const Cpoint<numberType> term )const
{
  Cvector<numberType> Output;
  Output.x = this->x + term.getX();
  Output.y = this->y + term.getY();
  
  return Output;
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator - ( const Cpoint<numberType> term )const
{
  Cvector<numberType> Output;
  Output.x = this->x - term.getX();
  Output.y = this->y - term.getY();
  
  return Output;
}

template <typename numberType>
Cvector<numberType> Cvector<numberType>::operator * ( const Cvector<numberType> term )const
{
  Cvector<numberType> Output;
  Output.x = this->x * term.getX();
  Output.y = this->y * term.getY();
  
  return Output;
}

template <typename numberType>
numberType Cvector<numberType>::sum()const
{
  return this->x+this->y;
}

template <typename numberType>
void Cvector<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "=>=>=>=>=>=>=>=>=>=>" << std::endl; 
  out << "Name: " << this->name << std::endl;
  out << "Coordinates: X = " << this->x << " Y = " << this->y << std::endl;
  out << "Description: " << this->description << std::endl; 
  out << "=>=>=>=>=>=>=>=>=>=>" << std::endl;
}

template <typename numberType>
void Cvector<numberType>::svg( std::ostream& out ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "<line x1=\"0\" y1=\"0\" x2=\"" << this->x << "\" y2=\"" << this->y << "\" marker-end=\"url(#triangle)\"  stroke=\"" << this->strokeColor << "\" stroke-width=\"" << this->strokeWidth << "\" fill=\"" << this->strokeColor << "\" />" << std::endl;
}

template <typename numberType>
void Cvector<numberType>::svg( std::ostream& out, double a, double b, double c, double d ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "<line x1=\"0\" y1=\"0\" x2=\"" << ( a*this->x + b*this->y ) << "\" y2=\"" << ( c*this->x + d*this->y ) << "\" marker-end=\"url(#triangle)\"  stroke=\"" << this->strokeColor << "\" stroke-width=\"" << 0 << "\" fill=\"" << this->strokeColor << "\" />" << std::endl;
}

template <typename numberType>
void Cvector<numberType>::save( std::ostream& out ) const
{
  out << "Cvector" << std::endl;
  out << this->name << std::endl;
  out << this->description << std::endl;
  out << this->fillColor << std::endl;
  out << this->strokeColor << std::endl;
  out << this->strokeWidth << std::endl;
  print( out, this->x );
  out << std::endl;
  print( out, this->y );
  out << std::endl;
  out << "END" << std::endl;
}

template <typename numberType>
void Cvector<numberType>::load( std::istream& in )
{
  std::string line;
  getline( in, line );
  if ( line != "Cvector" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  getline( in, this->name );
  getline( in, this->description );
  getline( in, this->fillColor );
  getline( in, this->strokeColor );
  getline( in, this->strokeWidth );
  
  in >> this->x;
  in >> this->y;
  
  getline( in, line );
}

// POINT SET ===========================================================
template <typename numberType>
CpointSet<numberType>::CpointSet()
{
  this->name = "Point Set";
  this->description = "";
  
  points = new std::list<Cpoint<numberType> >;
}

template <typename numberType>
CpointSet<numberType>::CpointSet( const CpointSet<numberType>& I_PointSet )
{
  this->name = "Point Set";
  this->description = "";
  
  points = new std::list<Cpoint<numberType> >;
  
  this->description = I_PointSet.description;
  *points = *(I_PointSet.points);
    
  this->fillColor = I_PointSet.fillColor;
  this->strokeColor = I_PointSet.strokeColor;
  this->strokeWidth = I_PointSet.strokeWidth;
}

template <typename numberType>
CpointSet<numberType>::~CpointSet()
{
  delete points;
}

template <typename numberType>
void CpointSet<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "*************************" << std::endl; 
  out << "Name: " << this->name << std::endl;
  out << "Description: " << this->description << std::endl; 
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
    {
    it->var_dump( out );
  }
  
  out << "*************************" << std::endl;
}

template <typename numberType>
void CpointSet<numberType>::addPoint( Cpoint<numberType> addition )
{
  points->push_front( addition );
}

template <typename numberType>
void CpointSet<numberType>::push_front( Cpoint<numberType> addition )
{
  points->push_front( addition );
}

template <typename numberType>
void CpointSet<numberType>::push_back( Cpoint<numberType> addition )
{
  points->push_back( addition );
}

template <typename numberType>
void CpointSet<numberType>::insert(typename std::list<Cpoint<numberType> >::iterator pos, Cpoint<numberType> addition)
{
  points->insert(pos, addition);
}

template <typename numberType>
typename std::list<Cpoint<numberType> >::iterator CpointSet<numberType>::removePoint( typename std::list<Cpoint<numberType> >::iterator removal )
{
  return this->points->erase(removal);
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator << ( Cpoint<numberType> addition )
{
  for ( typename std::list< Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    if ( *it == addition )
      return *this;
  }
  
  points->push_front( addition );
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator << ( CpointSet<numberType> addition )
{
  for ( typename std::list< Cpoint<numberType> >::iterator it = addition.begin(); it != addition.end(); ++it )
  {
    *this << *it;
  }
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator = ( const CpointSet<numberType>& I_PointSet )
{
  this->description = I_PointSet.description;
  
  delete points;
  points = new std::list<Cpoint<numberType> >;
  
  *points = *(I_PointSet.points);
  
  this->fillColor = I_PointSet.fillColor;
  this->strokeColor = I_PointSet.strokeColor;
  this->strokeWidth = I_PointSet.strokeWidth;
}

template <typename numberType>
typename std::list<Cpoint<numberType> >::iterator CpointSet<numberType>::begin()
{
  return points->begin();
}

template <typename numberType>
typename std::list<Cpoint<numberType> >::iterator CpointSet<numberType>::end()
{
  return points->end();
}

template <typename numberType>
typename std::list<Cpoint<numberType> >::reverse_iterator CpointSet<numberType>::rbegin()
{
  return points->rbegin();
}

template <typename numberType>
typename std::list<Cpoint<numberType> >::reverse_iterator CpointSet<numberType>::rend()
{
  return points->rend();
}

template <typename numberType>
void CpointSet<numberType>::clear()
{
  points->clear();
}

template <typename numberType>
int CpointSet<numberType>::size() const
{
  return points->size();
}

template <typename numberType>
void CpointSet<numberType>::sort()
{
  points->sort();
}

template <typename numberType>
void CpointSet<numberType>::unique()
{
  points->unique();
}

template <typename numberType>
void CpointSet<numberType>::reverse()
{
  points->reverse();
}

template <typename numberType>
const Cpoint<numberType>& CpointSet<numberType>::front() const
{
  return points->front(); 
}

template <typename numberType>
const Cpoint<numberType>& CpointSet<numberType>::back() const
{
  return points->back(); 
}

template <typename numberType>
void CpointSet<numberType>::transform( const numberType a, const numberType b, const numberType c, const numberType d )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( a*it->getX() + b*it->getY(), c*it->getX() + d*it->getY() );
  }
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator + ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( it->getX() + point.getX(), it->getY() + point.getY() );
  }
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator - ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( it->getX() - point.getX(), it->getY() - point.getY() );
  }
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator * ( const numberType& multiplicator )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( it->getX()*multiplicator, it->getY()*multiplicator );
  }
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator+= ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( it->getX() + point.getX(), it->getY() + point.getY() );
  }
  
  return *this;
}

template <typename numberType>
CpointSet<numberType>& CpointSet<numberType>::operator-= ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->set( it->getX() - point.getX(), it->getY() - point.getY() );
  }
  
  return *this;
}

template <typename numberType>
void CpointSet<numberType>::svg( std::ostream& out ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "/*" << this->description << "*/" << std::endl;
  out << "<g id=\"" << this->name << "\">" << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->setColor( this->fillColor, this->strokeColor, this->strokeWidth );
    it->svg( out );
  }
  out << "<g />" << std::endl;
}

template <typename numberType>
void CpointSet<numberType>::svg( std::ostream& out, double a, double b, double c, double d ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "/*" << this->description << "*/" << std::endl;
  out << "<g id=\"" << this->name << "\">" << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->setColor( this->fillColor, this->strokeColor, this->strokeWidth );
    it->svg( out, a, b, c, d );
  }
  out << "<g />" << std::endl;
}

template <typename numberType>
void CpointSet<numberType>::save( std::ostream& out ) const
{
  out << "CpointSet" << std::endl;
  out << this->name << std::endl;
  out << this->description << std::endl;
  out << this->fillColor << std::endl;
  out << this->strokeColor << std::endl;
  out << this->strokeWidth << std::endl;
  out << "POINTS" << std::endl;
  out << this->points->size() << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    it->save( out );
  }
  out << "END" << std::endl;
}

template <typename numberType>
std::string CpointSet<numberType>::save() const
{
  std::ostringstream str;
  for ( typename std::list<Cpoint<numberType> >::iterator it = points->begin(); it != points->end(); ++it )
  {
    print(str, it->getX());
    str << ",";
    print(str, it->getY());
    str << " ";
  }
  return str.str().c_str();
}

template <typename numberType>
void CpointSet<numberType>::load( std::string in )
{
  std::istringstream iss(in);
  numberType number;
  
  while (iss >> number)
  {
    std::cout << number << std::endl;
  }
}

template <typename numberType>
void CpointSet<numberType>::load( std::istream& in )
{
  std::string line;
  getline( in, line );
  if ( line != "CpointSet" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  getline( in, this->name );
  getline( in, this->description );
  getline( in, this->fillColor );
  getline( in, this->strokeColor );
  getline( in, this->strokeWidth );
  
  getline( in, line );
  if ( line != "POINTS" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  int count;
  in >> count;
  Cpoint<numberType> cache;
  
  for ( int i = 0; i < count; ++i )
  {
    cache.load( in );
    this->points.push_back( cache );
  }
  
  getline( in, line );
}

template <typename numberType>
CpointSet<numberType> CpointSet<numberType>::star() const
{
  CpointSet<numberType> Output( *this );
  for ( typename std::list<Cpoint<numberType> >::iterator it = Output.points->begin(); it != Output.points->end(); ++it )
  {
    *it = it->star();
  }
  Output.description = "**STARED**\n" + Output.description;
  
  return Output;
}

template <typename numberType>
void CpointSet<numberType>::filterDistanceOrigin(const numberType dist)
{
  std::cout << "HEy" << std::endl << std::flush;
  Cpoint<numberType> origin(0, 0);
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); )
  {
    std::cout << std::distance(this->points->begin(), it) << " - " << std::distance(it, this->points->end()) << std::flush << std::endl;
    if (euklid2<numberType>(origin,*it) > dist*dist )
    {
      it = this->removePoint( it );
    }
    else
    {
      ++it;
    }
  }
}


template <typename numberType>
void CpointSet<numberType>::sortClockwise()
{
  Cpoint<numberType> center;
  for (typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it)
  {
    center.set(center.getX()+it->getX(), center.getY()+it->getY());
  }
  //center.set(center.getX()*numberType::get(1,0,this->points->size()), center.getY()*numberType::get(1,0,this->points->size()));
  center.set(center.getX()/static_cast<numberType>(this->points->size()), center.getY()/static_cast<numberType>(this->points->size()));
  *this-= center;
  
  this->points->sort(clockwiseComp<numberType>);
  
  *this+= center;
}

// DELONE SET ==========================================================
template <typename numberType>
CdeloneSet<numberType>::CdeloneSet() : CpointSet<numberType>()
{
  this->name = "Delone Set";
  
  PackingR = 0;
  CoveringR = 0;
}

template <typename numberType>
CdeloneSet<numberType>::CdeloneSet( const CdeloneSet<numberType>& I_DeloneSet ) : CpointSet<numberType>()
{
  this->name = "Delone Set";
  
  this->description = I_DeloneSet.description;
  *(this->points) = *(I_DeloneSet.points);
  PackingR = I_DeloneSet.PackingR;
  CoveringR = I_DeloneSet.CoveringR;
  
  this->fillColor = I_DeloneSet.fillColor;
  this->strokeColor = I_DeloneSet.strokeColor;
  this->strokeWidth = I_DeloneSet.strokeWidth;
}

template <typename numberType>
CdeloneSet<numberType>& CdeloneSet<numberType>::operator = ( const CdeloneSet<numberType>& I_DeloneSet )
{
  this->name = "Delone Set";
  
  this->description = I_DeloneSet.description;
  *(this->points) = *(I_DeloneSet.points);
  PackingR = I_DeloneSet.PackingR;
  CoveringR = I_DeloneSet.CoveringR;
  
  this->fillColor = I_DeloneSet.fillColor;
  this->strokeColor = I_DeloneSet.strokeColor;
  this->strokeWidth = I_DeloneSet.strokeWidth;
}

template <typename numberType>
void CdeloneSet<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "*************************" << std::endl; 
  out << "Name: " << this->name << std::endl;
  out << "Description: " << this->description << std::endl; 
  out << "Packing radius: " << PackingR << std::endl; 
  out << "Covering radius: " << CoveringR << std::endl; 
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
    {
    it->var_dump( out );
  }
  
  out << "*************************" << std::endl;
}

template <typename numberType>
void CdeloneSet<numberType>::setPackingR( double I_PackingR )
{
  PackingR = I_PackingR;
}

template <typename numberType>
void CdeloneSet<numberType>::setCoveringR( double I_CoveringR )
{
  CoveringR = I_CoveringR;
}

template <typename numberType>
double CdeloneSet<numberType>::getPackingR()
{
  return PackingR;
}

template <typename numberType>
double CdeloneSet<numberType>::getCoveringR()
{
  return CoveringR;
}

template <typename numberType>
void CdeloneSet<numberType>::setPackingR()
{
  typename std::list<Cpoint<numberType> >::iterator tmp = this->points->begin();
  
  double radius = euklid2( *(++tmp), *tmp );
  double swap;
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    tmp = it;
    ++tmp;
    
    for ( typename std::list<Cpoint<numberType> >::iterator ot = tmp; ot != this->points->end(); ++ot )
    {
      swap = euklid2( *it, *ot );
      radius = (radius > swap) ? swap : radius;
    }
  }
  
  PackingR = sqrt(radius)/2;
}

template <typename numberType>
void CdeloneSet<numberType>::setCoveringR()
{
  typename std::list<Cpoint<numberType> >::iterator tmp;
  
  double radius = 0;
  double swap;
  double closest;
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    tmp = it;
    ++tmp;
    
    closest = euklid2( *it, *tmp );
    
    for ( typename std::list<Cpoint<numberType> >::iterator ot = tmp; ot != this->points->end(); ++ot )
    {
      swap = euklid2( *it, *ot );
      closest = (closest > swap) ? swap : closest;
    }
    
    radius = (radius < closest) ? closest : radius;
  }
  
  CoveringR = sqrt(radius);
}

template <typename numberType>
void CdeloneSet<numberType>::svg( std::ostream& out ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  //out << "/*" << this->description << "*/" << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->setColor( this->fillColor, this->strokeColor, this->strokeWidth );
    it->svg( out );
  }
}

template <typename numberType>
void CdeloneSet<numberType>::svg( std::ostream& out, double a, double b, double c, double d ) const
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
    {
    it->svg(out, a, b, c, d);
  }
}

template <typename numberType>
void CdeloneSet<numberType>::save( std::ostream& out ) const
{
  out << "CdeloneSet" << std::endl;
  out << this->name << std::endl;
  out << this->description << std::endl;
  out << this->fillColor << std::endl;
  out << this->strokeColor << std::endl;
  out << this->strokeWidth << std::endl;
  out << this->CoveringR << std::endl;
  out << this->PackingR << std::endl;
  out << "POINTS" << std::endl;
  out << this->points->size() << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->save( out );
  }
  out << "END" << std::endl;
}

template <typename numberType>
void CdeloneSet<numberType>::load( std::string in )
{
  this->points->clear();
  
  std::istringstream iss(in);
  numberType a;
  numberType b;
  Cpoint<numberType> point;
  
  while (iss >> a)
  {
    iss >> b;
    point.set(a, b);
    this->points->push_back(point);
  }
}

template <typename numberType>
std::string CdeloneSet<numberType>::save() const
{
  std::ostringstream str;
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    print(str, it->getX());
    str << ",";
    print(str, it->getY());
    str << " ";
  }
  return str.str().c_str();
}

template <typename numberType>
void CdeloneSet<numberType>::load( std::istream& in )
{
  std::string line;
  getline( in, line );
  if ( line != "CdeloneSet" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  getline( in, this->name );
  getline( in, this->description );
  getline( in, this->fillColor );
  getline( in, this->strokeColor );
  getline( in, this->strokeWidth );
  in >> CoveringR;
  in >> PackingR;
  
  getline( in, line );
  if ( line != "POINTS" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  int count;
  in >> count;
  Cpoint<numberType> cache;
  
  for ( int i = 0; i < count; ++i )
  {
    cache.load( in );
    this->points.push_back( cache );
  }
  
  getline( in, line );
}

template <typename numberType>
CdeloneSet<numberType>& CdeloneSet<numberType>::operator + ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->set( it->getX() + point.getX(), it->getY() + point.getY() );
  }
  
  return *this;
}

template <typename numberType>
CdeloneSet<numberType>& CdeloneSet<numberType>::operator - ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->set( it->getX() - point.getX(), it->getY() - point.getY() );
  }
  
  return *this;
}

template <typename numberType>
CdeloneSet<numberType>& CdeloneSet<numberType>::operator * ( const numberType& multiplicator )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->set( it->getX()*multiplicator, it->getY()*multiplicator );
  }
  
  return *this;
}
  
template <typename numberType>
bool CdeloneSet<numberType>::operator == ( const CdeloneSet<numberType> &compare ) const
{
  return *(this->points) == *(compare.points);
}
    
template <typename numberType>
bool CdeloneSet<numberType>::operator < ( const CdeloneSet<numberType> &compare ) const
{
  return *(this->points) < *(compare.points);
}

template <typename numberType>
CdeloneSet<numberType> CdeloneSet<numberType>::star() const
{
  CdeloneSet<numberType> Output( *this );
  for ( typename std::list<Cpoint<numberType> >::iterator it = Output.points->begin(); it != Output.points->end(); ++it )
  {
    *it = it->star();
  }
  Output.description = "**STARED**\n" + Output.description;
  
  return Output;
}


template <typename numberType>
void CdeloneSet<numberType>::filterDistanceOrigin(const numberType dist)
{
  Cpoint<numberType> origin(0, 0);
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); )
  {
    if (euklid2<numberType>(origin,*it) > dist*dist )
    {
      it = this->removePoint( it );
    }
    else
    {
      ++it;
    }
  }
}

template <typename numberType>
void CdeloneSet<numberType>::sort()
{
  this->points->sort();
}

template <typename numberType>
void CdeloneSet<numberType>::sortByDistance()
{
  this->points->sort(distanceComp<numberType>);
}

template <typename numberType>
std::list<Cpoint<numberType> > CdeloneSet<numberType>::getPoints()const
{
  return *(this->points);
}

// VORONOI CELL ========================================================
template <typename numberType>
CvoronoiCell<numberType>::CvoronoiCell()
{
  this->name = "Voronoi cell";
  this->description = "";
  
  CarrierSet = new CdeloneSet<numberType>();
  Cell = new CpointSet<numberType>();
}

template <typename numberType>
CvoronoiCell<numberType>::~CvoronoiCell()
{
  delete CarrierSet;
  delete Cell;
}

template <typename numberType>
CvoronoiCell<numberType>::CvoronoiCell( const CdeloneSet<numberType>& I_CarrierSet, const Cpoint<numberType> I_Center )
{
  this->name = "Voronoi cell";
  this->description = "";
  
  this->CarrierSet = new CdeloneSet<numberType>();
  this->Cell = new CpointSet<numberType>();
  
  *(this->CarrierSet) = I_CarrierSet;
  this->Center = I_Center;
}

template <typename numberType>
CvoronoiCell<numberType>::CvoronoiCell( const CdeloneSet<numberType>& I_CarrierSet )
{
  this->name = "Voronoi cell";
  this->description = "";
  
  this->CarrierSet = new CdeloneSet<numberType>();
  this->Cell = new CpointSet<numberType>();
  
  *(this->CarrierSet) = I_CarrierSet;
}

template <typename numberType>
CvoronoiCell<numberType>::CvoronoiCell( const CvoronoiCell<numberType>& I_VoronoiCell )
{
  this->name = "Voronoi cell";
  this->description = I_VoronoiCell.description;
  
  this->CarrierSet = new CdeloneSet<numberType>();
  this->Cell = new CpointSet<numberType>();
  
  *(this->CarrierSet) = *(I_VoronoiCell.CarrierSet);
  *(this->Cell) = *(I_VoronoiCell.Cell);
  this->Center = I_VoronoiCell.Center;
  
  this->fillColor = I_VoronoiCell.fillColor;
  this->strokeColor = I_VoronoiCell.strokeColor;
  this->strokeWidth = I_VoronoiCell.strokeWidth;
}

template <typename numberType>
void CvoronoiCell<numberType>::setCenter( const Cpoint<numberType> I_Center )
{
  this->Center = I_Center;
}

template <typename numberType>
CvoronoiCell<numberType>& CvoronoiCell<numberType>::operator = ( const CvoronoiCell<numberType>& I_VoronoiCell )
{
  this->name = "Voronoi cell";
  this->description = I_VoronoiCell.description;
  
  delete this->CarrierSet;
  delete this->Cell;
  
  this->CarrierSet = new CdeloneSet<numberType>();
  this->Cell = new CpointSet<numberType>();
  
  *(this->CarrierSet) = *(I_VoronoiCell.CarrierSet);
  *(this->Cell) = *(I_VoronoiCell.Cell);
  this->Center = I_VoronoiCell.Center;
  
  this->fillColor = I_VoronoiCell.fillColor;
  this->strokeColor = I_VoronoiCell.strokeColor;
  this->strokeWidth = I_VoronoiCell.strokeWidth;
}

template <typename numberType>
void CvoronoiCell<numberType>::var_dump( std::ostream& out ) const
{
  out << std::endl << "##############################" << std::endl; 
  out << "Name: " << this->name << std::endl;
  out << "Description: " << this->description << std::endl; 
  out << "Center: " << std::endl; 
  Center.var_dump( out );
  
  out << std::endl << "###############" << std::endl; 
  out << "Cell: " << std::endl; 
  Cell->var_dump( out );
  
  out << std::endl << "###############" << std::endl; 
  out << "Carrier set: " << std::endl; 
  CarrierSet->var_dump( out );
  
  out << std::endl << "##############################" << std::endl; 
}

// constructCut ********************************************************
template <typename numberType>
bool CvoronoiCell<numberType>::constructCut( typename std::list<Cpoint<numberType> >::iterator add )
{
  if ( Cell->size() <= 2 )
  {
    Cell->clear();
  
    // initialization of polygon
    Cpoint<numberType> point( Center.getX() + large, Center.getY() + large );
    point.setDescription( "original" );
    *Cell << point;
    point.set( Center.getX() + large, Center.getY() - large );
    *Cell << point;
    point.set( Center.getX() - large, Center.getY() - large );
    *Cell << point;
    point.set( Center.getX() - large, Center.getY() + large );
    *Cell << point;
  }
  //std::cout << "<g>" << std::endl;
  //this->svg(std::cout);
  
  CpointSet<numberType> newCell = *Cell;
  
  typename std::list<Cpoint<numberType> >::iterator it = add;
  
  if ( ( euklid2( *it, Center ) > CarrierSet->getCoveringR()*CarrierSet->getCoveringR() ) || ( *it == Center ) )
  {
    return false;
  }
  
  // vector from it toward center
  Cvector<numberType> n = Center - *it;
  n.setDescription("n");
  //n.setColor("#ff0000", "#00ff00", "0.4");
  //n.svg(std::cout);
  
  // normal form of axis of center and active point
  Cpoint<numberType> c = n/2 + *it;
  c.setDescription("c");
  //c.setColor("#00ff00", "#0000ff", "0.4");
  //c.svg(std::cout);
  
  numberType d = (n*c).sum(); // constant for cut line
  
  // check for position of Cell and axis
  newCell.clear();
  bool position, positionOld;
  typename std::list<Cpoint<numberType> >::iterator old = --Cell->end();
  position = ((n*(*old)).sum() > d);
  
  bool cut = false;
  
  for (typename std::list<Cpoint<numberType> >::iterator ot = Cell->begin(); ot != Cell->end(); ++ot)
  {
    //print(std::cout, d);
    //std::cout << "\t";
    //print(std::cout, (n*(*old)).sum());
    //std::cout << "\t";
    //print(std::cout, (n*(*ot)).sum());
    //std::cout << std::endl;
    //std::cout << d << "\t" << (n*(*old)).sum() << "\t" << (n*(*ot)).sum() << std::endl;
    //std::cout << "\t\t" << ((n*(*old)).sum()<d) << "\t" << ((n*(*ot)).sum()<d) << std::endl;
    //std::cout << "\t\t" << (static_cast<double>((n*(*old)).sum())< static_cast<double>(d)) << "\t" << (static_cast<double>((n*(*ot)).sum()) < static_cast<double>(d)) << std::endl;
    positionOld = position;
    position = ((n*(*ot)).sum() > d);
    
    if (positionOld != position)  // old and ot are on different sides of cut line
    {
      // vector from old to ot
      Cvector<numberType> u = *ot - *old;
      u.setDescription( "u" );
      //u.setColor("#00ffff", "#00ffff", "0.4");
      //u.svg(std::cout);
      
      numberType t = ((n*(c - (*old))).sum()) / ((u*n).sum()); // ( nx*( cx - old->getX() ) + ny*( cy - old->getY() ) )/( ux*nx + uy*ny );
      
      Cpoint<numberType> newPoint(u*t + *old);
      newPoint.setDescription( "newPoint" );
      //newPoint.setColor("#00ffff", "#00ffff", "0.4");
      //newPoint.svg(std::cout);
      
      newCell << newPoint;
      
      cut = true;//equality && equalityOld;
    }
    
    if ((position) && (*ot != newCell.front())) // ot is on same side of cut line as center
    {
      newCell << *ot;
    }
    
    old = ot;
  }
  
  newCell.reverse();
  
  *Cell = newCell;
  //std::cout << "</g>" << std::endl;
  return cut;
}

template <typename numberType>
void CvoronoiCell<numberType>::construct()
{
  Cell->clear();
  
  for (typename std::list<Cpoint<numberType> >::iterator ot = CarrierSet->begin(); ot != CarrierSet->end(); ++ot)
  {
    constructCut( ot );
  }
}

template <typename numberType>
bool CvoronoiCell<numberType>::constructAdd(const Cpoint<numberType> add)
{
  CarrierSet->addPoint( add );
  
  return constructCut( CarrierSet->begin() );
}

template <typename numberType>
bool CvoronoiCell<numberType>::filterPoint(Cpoint<numberType> add)
{
  // vector from it toward center
  Cvector<numberType> n = Center - add;
  n.setDescription( "n" );
  
  // normal form of axis of center and active point
  Cvector<numberType> c = n/2 + add;
  c.setDescription( "c" );
  
  numberType d = (n*c).sum(); // constant for cut line
  
  typename std::list<Cpoint<numberType> >::iterator old = Cell->end();
  --old;
  
  for (typename std::list<Cpoint<numberType> >::iterator ot = Cell->begin(); ot != Cell->end(); ++ot)
  {
    if ( ( (n*(*ot)).sum() == d ) && ( (n*(*old)).sum() == d ) )
    {
      return false;
    }
    
    old = ot;
  }
  return true;
}

template <typename numberType>
void CvoronoiCell<numberType>::filterSet()
{
  //typename std::list<Cpoint<numberType> >::iterator it;
  
  typename std::list<Cpoint<numberType> >::iterator ot = CarrierSet->begin();
  while ( ot != CarrierSet->end() )
  {
    //it = ot++;
    if ( filterPoint( *ot ) )
    {
      ot = CarrierSet->removePoint( ot );
    }
    else if (*ot == Cpoint<numberType>())
    {
      ot = CarrierSet->removePoint( ot );
    }
    else
    {
      ++ot;
    }
  }
  
  CarrierSet->sortClockwise();
}

template <typename numberType>
bool CvoronoiCell<numberType>::filterPointAny( Cpoint<numberType> add )
{
  // vector from it toward center
  Cvector<numberType> n = Center - add;
  n.setDescription( "n" );
  
  // normal form of axis of center and active point
  Cvector<numberType> c = n/2 + add;
  c.setDescription( "c" );
  
  numberType d = (n*c).sum(); // constant for cut line
  
  typename std::list<Cpoint<numberType> >::iterator old = Cell->end();
  --old;
  
  for ( typename std::list<Cpoint<numberType> >::iterator ot = Cell->begin(); ot != Cell->end(); ++ot )
  {
    if ( ( (n*(*ot)).sum() > d ) != ( (n*(*old)).sum() > d ) )
    {
      Cvector<numberType> u = *ot - *old;
      numberType t = ((n*(c - (*old))).sum()) / ((u*n).sum());
      
      if ((t==numberType::get(1,0)) || (t==numberType::get(0,0)))
      {
        return true;
      }
      return false;
    }
    
    old = ot;
  }
  return true;
}

template <typename numberType>
void CvoronoiCell<numberType>::filterSetPotential( std::list<Cpoint<numberType> >* potentialSet )
{
  typename std::list<Cpoint<numberType> >::iterator ot = potentialSet->begin();
  while ( ot != potentialSet->end() )
  {
    if ( filterPointAny( *ot ) )
    {
      ot = potentialSet->erase( ot );
    }
    else
    {
      ++ot;
    }
  }
  
}

template <typename numberType>
void CvoronoiCell<numberType>::svg( std::ostream& out ) const
{
  
  out << "<!-- " << this->description << " -->" << std::endl;
  out << "<polygon points=\"";
  for ( typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(); it != Cell->end(); ++it )
    {
    out << std::fixed << it->getX() << "," << it->getY() << " ";
  }
  out << "\" style=\"fill:" << this->fillColor << ";stroke:" << this->strokeColor << ";stroke-width:" << this->strokeWidth << "\" />" << std::endl;
}

template <typename numberType>
void CvoronoiCell<numberType>::save( std::ostream& out ) const
{
  out << "CvoronoiCell" << std::endl;
  out << this->name << std::endl;
  out << this->description << std::endl;
  out << this->fillColor << std::endl;
  out << this->strokeColor << std::endl;
  out << this->strokeWidth << std::endl;
  out << "CENTER" << std::endl;
  Center.save( out );
  out << "CARRIER_SET" << std::endl;
  CarrierSet->save( out );
  out << "CELL" << std::endl;
  Cell->save( out );
  out << "END" << std::endl;
}

template <typename numberType>
std::string CvoronoiCell<numberType>::save() const
{
  return CarrierSet->save();
}


template <typename numberType>
void CvoronoiCell<numberType>::load( std::string in )
{
  CarrierSet->load(in);
}

template <typename numberType>
void CvoronoiCell<numberType>::load( std::istream& in )
{
  std::string line;
  getline( in, line );
  if ( line != "CvoronoiCell" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  getline( in, this->name );
  getline( in, this->description );
  getline( in, this->fillColor );
  getline( in, this->strokeColor );
  getline( in, this->strokeWidth );
  
  getline( in, line );
  if ( line != "CENTER" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  this->Center.load( in );
  
  if ( line != "CARRIER_SET" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  this->CarrierSet->load( in );
  
  if ( line != "CELL" )
  {
    std::cout << "Wrong format" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  this->Cell->load( in );
  
  getline( in, line );
}

template <typename numberType>
void CvoronoiCell<numberType>::svg( std::ostream& out, double a, double b, double c, double d ) const
{
  out << "<polygon points=\"";
  for ( typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(); it != Cell->end(); ++it )
    {
    out << a*it->getX() + b*it->getY() << "," << c*it->getX() + d*it->getY() << " ";
  }
  out << "\" fill=\"" << this->fillColor << "\" stroke=\"" << this->strokeColor << "\" stroke-width=\"" << this->strokeWidth << "\" fill-opacity=\"" << 0.6 << "\" />" << std::endl;
}



template <typename numberType>
bool CvoronoiCell<numberType>::operator == ( const CvoronoiCell<numberType> &compare ) const
{
  if ( Cell->size() != compare.Cell->size() )
    return false;
  
  typename std::list<Cpoint<numberType> >::iterator it = compare.Cell->begin();
  
  typename std::list<Cpoint<numberType> >::iterator ot = Cell->begin(); 
  
  bool check = false;
  
  while ( (ot != Cell->end()) && (!check) )
  {
    if ( *it == *ot )
      check = true;
    else
      ++ot;
  }
  
  if ( !check )
  {
    return false;
  }
  
  typename std::list<Cpoint<numberType> >::iterator cache = ot;
  
  for ( it = compare.Cell->begin(); it != compare.Cell->end(); ++it, ++ot )
  {
    if ( ot == Cell->end() )
      ot = Cell->begin();
      
    if ( *ot != *it )
      check = false;
  }
  
  if ( !check )
  {
    ot = ++cache;
    for ( typename std::list<Cpoint<numberType> >::reverse_iterator ut = compare.Cell->rbegin(); ut != compare.Cell->rend(); ++ut, ++ot )
    {
      if ( ot == Cell->end() )
        ot = Cell->begin();
        
      if ( *ot != *ut )
      {
        return false;
      }
    }
  }
  
  return true;
}

template <typename numberType>
bool CvoronoiCell<numberType>::operator < ( const CvoronoiCell<numberType> &compare ) const
{
  // area of cell
  if (size() < compare.size())
  {
    return true;
  }
  
  // number of vertices of cell
  if ((size() == compare.size()) && (Cell->size() < compare.Cell->size()))
  {
    return true;
  }
  
  // distance of mean of cell vertices from origin
  if ((size() == compare.size()) && (Cell->size() == compare.Cell->size()) && (euklid2(middle()) < euklid2(compare.middle())))
  {
    return true;
  }
  
  // distance of mean of carrier set from origin
  if ((size() == compare.size()) && (Cell->size() == compare.Cell->size()) && (euklid2(middle()) == euklid2(compare.middle())) \
                                 && (euklid2(middleDomain()) < euklid2(compare.middleDomain())))
  {
    return true;
  }
  
  
  // angle of mean of cell vertices
  if ((size() == compare.size()) && (Cell->size() == compare.Cell->size()) && (euklid2(middle()) == euklid2(compare.middle())) \
                                 && (euklid2(middleDomain()) == euklid2(compare.middleDomain())) \
                                 && (atan2(middle().getX(), middle().getY()) < atan2(compare.middle().getX(), compare.middle().getY())))
  {
    return true;
  }
  
  // angle of mean of carrier set
  if ((size() == compare.size()) && (Cell->size() == compare.Cell->size()) && (euklid2(middle()) == euklid2(compare.middle())) \
                                 && (euklid2(middleDomain()) == euklid2(compare.middleDomain())) \
                                 && (atan2(middle().getX(), middle().getY()) == atan2(compare.middle().getX(), compare.middle().getY())) \
                                 && (atan2(middleDomain().getX(), middleDomain().getY()) < atan2(compare.middleDomain().getX(), compare.middleDomain().getY())))
  {
    return true;
  }
  
  return false;
  
}


template <typename numberType>
double CvoronoiCell<numberType>::value() const
{
  double value = 0;
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(); it != Cell->end(); ++it )
  {
    value+= Cell->size()*sqrt( fabs( it->getX() - Center.getX() )*fabs( it->getX() - Center.getX() ) + fabs( it->getY() - Center.getY() )*fabs( it->getY() - Center.getY() ) );
  }
  
  return 10000*value;
}

template <typename numberType>
numberType CvoronoiCell<numberType>::size() const
{
  numberType area = numberType::get(0,0);
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(), itold = --Cell->end(); it != Cell->end(); itold = it++ )
  {
    area+= itold->getX()*it->getY() - it->getX()*itold->getY();
  }
  
  return area.abs()*numberType::get(1,0,2);
}

template <typename numberType>
numberType CvoronoiCell<numberType>::radius() const
{
  numberType radius = numberType::get(0,0);
  
  // upper estimate from Manhattan metric
  for (typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(); it != Cell->end(); ++it)
  {
    radius = max(it->getX().abs()+it->getY().abs(), radius);
  }
  
  return radius;
}

template <typename numberType>
Cpoint<numberType> CvoronoiCell<numberType>::middle() const
{
  Cpoint<numberType> middle;
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = Cell->begin(); it != Cell->end(); ++it )
  {
    middle.setX( middle.getX() + it->getX() );
    middle.setY( middle.getY() + it->getY() );
  }
  
  return middle;
}

template <typename numberType>
Cpoint<numberType> CvoronoiCell<numberType>::middleDomain() const
{
  Cpoint<numberType> middle;
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = CarrierSet->begin(); it != CarrierSet->end(); ++it )
  {
    middle.setX( middle.getX() + it->getX() );
    middle.setY( middle.getY() + it->getY() );
  }
  
  return middle;
}

template <typename numberType>
void CvoronoiCell<numberType>::colorify()
{
  std::hash<std::string> str_hash;
  
  CvoronoiCell<numberType> rot = *this;
  
  rot.filterSet();
  
  *rot.Cell-= rot.Center;
  *rot.CarrierSet-= rot.Center;
  
  rot.CarrierSet->sortClockwise();
  
  CvoronoiCell<numberType> min = rot;
  CvoronoiCell<numberType> flip = rot.flip();
  CvoronoiCell<numberType> flop = rot.flop();
  
  for (int i = 1; i < numberType::rotateN(); ++i)
  {
    flip = flip.rotate(1);
    flop = flop.rotate(1);
    rot = rot.rotate(1);
    if (rot < min)
    {
      min = rot;
    }
    if (flip < min)
    {
      min = flip;
    }
    if (flop < min)
    {
      min = flop;
    }
  }
  
  //*this = min;
  
  min.CarrierSet->sortClockwise();
  
  //*this->CarrierSet = *min.CarrierSet;
  //*this->Cell = *min.Cell;
  
  //*min.CarrierSet = *min.CarrierSet*numberType::get(1,0,min.size());
  *min.CarrierSet = *min.CarrierSet*(numberType::get(6,0)/(min.CarrierSet->begin()->getX()+min.CarrierSet->begin()->getY()));
  
  //std::cout << min.save() << std::endl << std::endl;
  
  unsigned int hash = str_hash(min.save());
  
  double h = hash % 2147483647;
  double s = (((hash & 0x00FF00) >> 8) % 124 + 128);
  double v = ((hash & 0x0000FF) % 16 + 240);
  
  h/=2147483647.;
  h*=6.;
  s/=256.;
  v/=256.;
  
  double fract = h - floor(h);
  double P = v*(1. - s);
  double Q = v*(1. - s*fract);
  double T = v*(1. - s*(1. - fract));
  
  double r, g, b;
  
  if (0. <= h && h < 1.)
  {
    r = v;
    g = T;
    b = P;
  }
  else if (1. <= h && h < 2.)
  {
    r = Q;
    g = v;
    b = P;
  }
  else if (2. <= h && h < 3.)
  {
    r = P;
    g = v;
    b = T;
  }
  else if (3. <= h && h < 4.)
  {
    r = P;
    g = Q;
    b = v;
  }
  else if (4. <= h && h < 5.)
  {
    r = T;
    g = P;
    b = v;
  }
  else if (5. <= h && h < 6.)
  {
    r = v;
    g = P;
    b = Q;
  }
  else
  {
    r = 0;
    g = 0;
    b = 0;
  }
  
  r*=256;
  g*=256;
  b*=256;
  
  //std::cout << r << " " << g << " " << b << std::endl;
  
  std::ostringstream color;
  
  color << "#" << std::setfill('0') << std::setw(2) << std::hex << int(r) << std::setfill('0') << std::setw(2) << int(g) << std::setfill('0') << std::setw(2) << int(b) << std::dec;
  
  //std::cout << color.str().c_str() << std::endl;
  
  this->setFillColor( color.str().c_str() );
}

template <typename numberType>
CvoronoiCell<numberType> CvoronoiCell<numberType>::rotate( int n ) const
{
  n = n%numberType::rotateN();
  
  CvoronoiCell<numberType> Output = *this;
  for ( typename std::list<Cpoint<numberType> >::iterator it = Output.Cell->begin(); it != Output.Cell->end(); ++it )
  {
    for ( int i = 1; i <= n; ++i )
    {
      it->set( it->getX()*numberType::rotateA() + it->getY()*numberType::rotateB(), it->getX()*numberType::rotateC() + it->getY()*numberType::rotateD() );
    }
  }
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = Output.CarrierSet->begin(); it != Output.CarrierSet->end(); ++it )
  {
    for ( int i = 1; i <= n; ++i )
    {
      it->set( it->getX()*numberType::rotateA() + it->getY()*numberType::rotateB(), it->getX()*numberType::rotateC() + it->getY()*numberType::rotateD() );
    }
  }
  
  return Output;
}

template <typename numberType>
CvoronoiCell<numberType> CvoronoiCell<numberType>::flip() const
{
  numberType x = (numberType::windowA()+numberType::windowB());
  numberType y = (numberType::windowC()+numberType::windowD());
  numberType denominator = x*x+y*y;
  numberType a = (x*x-y*y)/denominator;
  numberType b = (numberType::get(2,0)*x*y)/denominator;
  numberType c = (numberType::get(2,0)*x*y)/denominator;
  numberType d = (y*y-x*x)/denominator;
  
  CvoronoiCell<numberType> Output = *this;
  Output.Cell->transform(a,b,c,d);
  Output.CarrierSet->transform(a,b,c,d);
  
  return Output;
}

template <typename numberType>
CvoronoiCell<numberType> CvoronoiCell<numberType>::flop() const
{
  numberType x = (numberType::windowA()-numberType::windowB());
  numberType y = (numberType::windowC()-numberType::windowD());
  numberType denominator = x*x+y*y;
  numberType a = (x*x-y*y)/denominator;
  numberType b = (numberType::get(2,0)*x*y)/denominator;
  numberType c = (numberType::get(2,0)*x*y)/denominator;
  numberType d = (y*y-x*x)/denominator;
  
  CvoronoiCell<numberType> Output = *this;
  Output.Cell->transform(a,b,c,d);
  Output.CarrierSet->transform(a,b,c,d);
  
  return Output;
}

template <typename numberType>
CvoronoiCell<numberType> CvoronoiCell<numberType>::star() const
{
  CvoronoiCell<numberType> Output( *this );
  Output->CarrierSet = Output.CarrierSet->star();
  Output->Cell = Output.Cell->star();
  Output.Center = Output.Center.star();
  Output.description = "**STARED**\n" + Output.description;
  
  return Output;
}


template <typename numberType>
std::string CvoronoiCell<numberType>::dna() const
{
  CvoronoiCell<numberType> rot = *this;
  
  rot.filterSet();
  
  *rot.Cell-= rot.Center;
  *rot.CarrierSet-= rot.Center;
  
  rot.CarrierSet->sortClockwise();
  
  CvoronoiCell<numberType> min = rot;
  CvoronoiCell<numberType> flip = rot.flip();
  CvoronoiCell<numberType> flop = rot.flop();
  
  for (int i = 1; i < numberType::rotateN(); ++i)
  {
    flip = flip.rotate(1);
    flop = flop.rotate(1);
    rot = rot.rotate(1);
    if (rot < min)
    {
      min = rot;
    }
    if (flip < min)
    {
      min = flip;
    }
    if (flop < min)
    {
      min = flop;
    }
  }
  
  //*this = min;
  
  min.CarrierSet->sortClockwise();
  
  return min.save();
}

template<typename numberType> numberType CvoronoiCell<numberType>::large = 0;

// SUPPORT FUNCTION ====================================================
template <typename numberType>
double euklid( const Cpoint<numberType>& a, const Cpoint<numberType>& b )
{
  return sqrt( (a.getX()-b.getX())*(a.getX()-b.getX()) + (a.getY()-b.getY())*(a.getY()-b.getY()) );
}

template <typename numberType>
double euklid2( const Cpoint<numberType>& a, const Cpoint<numberType>& b )
{
  return (a.getX()-b.getX())*(a.getX()-b.getX()) + (a.getY()-b.getY())*(a.getY()-b.getY());
}

template <typename numberType>
double euklid2(const Cpoint<numberType>& a)
{
  return a.getX()*a.getX() + a.getY()*a.getY();
}

template <typename numberType>
bool distanceComp( const Cpoint<numberType>& a, const Cpoint<numberType>& b )
{
  return ( (a.getX()*a.getX()+a.getY()*a.getY() < b.getX()*b.getX()+b.getY()*b.getY()) || ( (a.getX()*a.getX()+a.getY()*a.getY() == b.getX()*b.getX()+b.getY()*b.getY()) && (( a.getX() < b.getX() ) || (( a.getX() == b.getX() ) && ( a.getY() < b.getY() )))));
}

template <typename numberType>
bool clockwiseComp( const Cpoint<numberType>& a, const Cpoint<numberType>& b )
{
  return atan2(a.getY(), a.getX()) < atan2(b.getY(), b.getX());
}

template <typename numberType>
bool dnaComp( const CvoronoiCell<numberType>& a, const CvoronoiCell<numberType>& b )
{
  return a.dna() == b.dna();
}

#endif
