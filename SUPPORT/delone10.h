#ifndef _DELONE10_
#define _DELONE10_

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iterator>

#include "betaSet.h"
#include "geometricObject2.h"
#include "window2.h"


template <typename numberType>
class CdeloneSet10 : public CdeloneSet<numberType>//, public virtual Cfigure<numberType>
{
  protected:
    std::list<Cpoint<numberType> > potential;
  public:
    void addPotential( const std::list<Cpoint<numberType> >& I_potential );
    void addPotential( const Cpoint<numberType>& I_potential );
    Cpoint<numberType> popPotential();
    bool isPotential();
    bool operator == ( const CdeloneSet10<numberType> &compare ) const;
    bool operator <  ( const CdeloneSet10<numberType> &compare ) const;
    void filterDistanceOrigin(const numberType dist);
    void removeOrigin();
    void filterByVoronoi();
    template <typename windowType>
    void filterPotentialByWindow(windowType win);
    void clearPotential();
    std::list<Cpoint<numberType> > getPotential();
    
    CdeloneSet10<numberType>& operator + ( const Cpoint<numberType>& point );
    
    int sizePotential();
    void sortPotentialByDistance();
    
    void svg( std::ostream& out );
};


template <typename numberType>
void CdeloneSet10<numberType>::addPotential( const std::list<Cpoint<numberType> >& I_potential )
{
  potential = I_potential;
}

template <typename numberType>
void CdeloneSet10<numberType>::addPotential( const Cpoint<numberType>& I_potential )
{
  potential.push_back(I_potential);
}

template <typename numberType>
Cpoint<numberType> CdeloneSet10<numberType>::popPotential()
{
  Cpoint<numberType> tmp = potential.front();
  potential.pop_front();
  
  return tmp;
}

template <typename numberType>
bool CdeloneSet10<numberType>::isPotential()
{
  return !potential.empty();
}

template <typename numberType>
bool CdeloneSet10<numberType>::operator == ( const CdeloneSet10<numberType> &compare ) const
{
  return (*(this->points) == *(compare.points) && (this->potential) == (compare.potential));
}
    
template <typename numberType>
bool CdeloneSet10<numberType>::operator < ( const CdeloneSet10<numberType> &compare ) const
{
  return (*(this->points) < *(compare.points) && (this->potential) < (compare.potential));
}

template <typename numberType>
void CdeloneSet10<numberType>::filterDistanceOrigin(const numberType dist)
{
  Cpoint<numberType> origin(0, 0);
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); )
  {
    if (euklid2(origin,*it) > dist*dist)
    {
      it = this->removePoint( it );
    }
    else
    {
      ++it;
    }
  }
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); )
  {
    if (euklid2(origin,*it) > dist*dist)
    {
      it = this->potential.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

template <typename numberType>
void CdeloneSet10<numberType>::removeOrigin()
{
  Cpoint<numberType> origin(0, 0);
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); )
  {
    if (*it == origin)
    {
      it = this->potential.erase(it);
    }
    else
    {
      ++it;
    }
  }
}


template <typename numberType>
void CdeloneSet10<numberType>::filterByVoronoi()
{
  Cpoint<numberType> origin( numberType::get(0,0), numberType::get(0,0) );
  CvoronoiCell<numberType> voronoi;
    
  *(voronoi.CarrierSet) = *this;
  voronoi.CarrierSet->sortByDistance();
  voronoi.CarrierSet->setPackingR();
  voronoi.CarrierSet->setCoveringR(CvoronoiCell<numberType>::large);
  voronoi.setCenter(origin);
  voronoi.construct();
  voronoi.filterSet();
  
  *(this->points) = *(voronoi.CarrierSet->points);
  
  numberType dist(0,0);
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = voronoi.Cell->begin(); it != voronoi.Cell->end(); ++it )
  {
    if (euklid(origin,*it) > dist )
    {
      dist = euklid(origin,*it);
    }
  }
  
  dist = dist*numberType::get(2,0);
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); )
  {
    if (euklid(origin,*it) > dist )
    {
      it = this->removePoint( it );
      //std::cout << "|";
    }
    else
    {
      ++it;
    }
  }
  //std::cout << std::endl;
}

template <typename numberType>
template <typename windowType>
void CdeloneSet10<numberType>::filterPotentialByWindow(windowType win)
{
  for (typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); )
  {
    //std::cout << "INSIDE" << std::endl << std::flush;
    windowType windowCheck = win;
    CdeloneSet10<numberType> delone = *this;
    //std::cout << "CLONING " << it->getX() << std::endl << std::flush;
    delone << *it;
    
    if (!fitToWindow(&windowCheck, delone))
    {
      std::cout << "\t not fit" << std::endl << std::flush;
      it = potential.erase(it);
    }
    else
    {
      //std::cout << "\t fits" << std::endl << std::flush;
      ++it;
    }
  }
}

template <typename numberType>
void CdeloneSet10<numberType>::clearPotential()
{
  potential.clear();
}

template <typename numberType>
std::list<Cpoint<numberType> > CdeloneSet10<numberType>::getPotential()
{
  return potential;
}


template <typename numberType>
CdeloneSet10<numberType>& CdeloneSet10<numberType>::operator + ( const Cpoint<numberType>& point )
{
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->set( it->getX() + point.getX(), it->getY() + point.getY() );
  }
  
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); ++it )
  {
    it->set( it->getX() + point.getX(), it->getY() + point.getY() );
  }
  
  return *this;
}

template <typename numberType>
int CdeloneSet10<numberType>::sizePotential()
{
  return potential.size();
}


template <typename numberType>
void CdeloneSet10<numberType>::sortPotentialByDistance()
{
  this->potential.sort(distanceComp<numberType>);
}

template <typename numberType>
void CdeloneSet10<numberType>::svg( std::ostream& out ) 
{
  if ( this->description.length() > 0 )
  {
    out << "<!-- " << this->description << " -->" << std::endl;
  }
  out << "<g id=\"" << this->name << "\">" << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->points->begin(); it != this->points->end(); ++it )
  {
    it->setColor( this->fillColor, this->strokeColor, this->strokeWidth );
    it->svg(out);
    out << "YEA" << std::endl;
  }
  out << "<!-- POTENTIAL -->" << this->points->size() << std::endl;
  for ( typename std::list<Cpoint<numberType> >::iterator it = this->potential.begin(); it != this->potential.end(); ++it )
  {
    it->setColor( this->fillColor, "#FFEA00", this->strokeWidth );
    it->svg(out);
  }
  out << "</g>" << std::endl;
}

#endif
