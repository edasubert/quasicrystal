#include <cmath>
#include <iostream>

#include "alphaSet.h"
#include "geometricObject2.h"

double alphaSet::constant = 1 + sqrt(2);

// ALPHA SET ------------------------------------------------------------

alphaSet::alphaSet()
{
  a = 0;
  b = 0;
  c = 1;
}

alphaSet::alphaSet(number I_a)
{
  a = I_a;
  b = 0;
  c = 1;
}

alphaSet::alphaSet(number I_a, number I_b)
{
  a = I_a;
  b = I_b;
  c = 1;
}

alphaSet::alphaSet(number I_a, number I_b, number I_c)
{
  a = I_a;
  b = I_b;
  c = I_c;
  
  this->simplify();
}

void alphaSet::set(number I_a, number I_b, number I_c)
{
  a = I_a;
  b = I_b;
  c = I_c;
  
  this->simplify();
}

alphaSet alphaSet::get(number I_a, number I_b)
{
  alphaSet Output;
  Output.a = I_a;
  Output.b = I_b;
  Output.c = 1;
  
  return Output;
}

alphaSet alphaSet::get(number I_a, number I_b, number I_c)
{
  alphaSet Output;
  Output.a = I_a;
  Output.b = I_b;
  Output.c = I_c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::betaK( int k )
{
  alphaSet Output = alphaSet::get(0,1);
  if ( k > 1 )
  {
    for ( int i = 1; i != k; ++i )
    {
      Output *= alphaSet::get(0,1);
    }
  }
  else if ( k < 1 )
  {
    for ( int i = 1; i != k; --i )
    {
      Output /= alphaSet::get(0,1);
    }
  }
  
  Output.simplify();
  
  return Output;
}

alphaSet& alphaSet::operator = ( const int &Input )
{
  a = Input;
  b = 0;
  c = 1;
  
  return *this;
}

int alphaSet::simplify()
{
  if ((a == 0) && (b == 0))
  {
    c = 1;
    return 1;
  }
  
  if (c < 0)
  {
    a = -a;
    b = -b;
    c = -c;
  }
  
  number cache;
  if ( ( ( cache = gcd(a,b) ) != 0 ) && ( ( cache = gcd(cache,c) ) != 0 ) && ( cache > 1 ) )
  {
    //std::cout << a << ", " << b << ", " << c << " => " << cache << std::endl;
    a = static_cast<number>(a/cache);
    b = static_cast<number>(b/cache);
    c = static_cast<number>(c/cache);
    
    if (c < 0)
    {
      a = -a;
      b = -b;
      c = -c;
    }
    
    return 1;
  }
  
  return 0;
}

alphaSet alphaSet::star()const
{
  alphaSet Output;
  Output.a = a + 2*b;
  Output.b = -b;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator + ( const alphaSet &Input )const
{
  alphaSet Output;
  
  Output.a = (a*Input.c + Input.a*c);
  Output.b = (b*Input.c + Input.b*c);
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator - ( const alphaSet &Input )const
{
  alphaSet Output;
  
  Output.a = (a*Input.c - Input.a*c);
  Output.b = (b*Input.c - Input.b*c);
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator * ( const alphaSet &Input )const
{
  alphaSet Output;
  
  Output.a = a*Input.a + b*Input.b;
  Output.b = b*Input.a + a*Input.b + 2*b*Input.b;
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator / ( const alphaSet &Input )const
{
  alphaSet Output;
  
  Output.a = (a*Input.a - b*Input.b + 2*a*Input.b)*Input.c;
  Output.b = (b*Input.a - a*Input.b)*Input.c;
  Output.c = (Input.a*Input.a + 2*Input.a*Input.b - Input.b*Input.b)*c;
  
  Output.simplify();
  
  return Output;
}

alphaSet& alphaSet::operator += ( const alphaSet &Input )
{
  a = (a*Input.c + Input.a*c);
  b = (b*Input.c + Input.b*c);
  c = Input.c*c;
  
  this->simplify();
  
  return *this;
}

alphaSet& alphaSet::operator -= ( const alphaSet &Input )
{
  a = (a*Input.c - Input.a*c);
  b = (b*Input.c - Input.b*c);
  c = Input.c*c;
  
  this->simplify();
  
  return *this;
}

alphaSet& alphaSet::operator *= ( const alphaSet &Input )
{
  number cache = a*Input.a + b*Input.b;
  b = b*Input.a + a*Input.b + 2*b*Input.b;
  a = cache;
  c = Input.c*c;
  
  this->simplify();
  
  return *this;
}

alphaSet& alphaSet::operator /= ( const alphaSet &Input )
{
  number cache = (a*Input.a - b*Input.b + 2*a*Input.b)*Input.c;
  b = (b*Input.a - a*Input.b)*Input.c;
  a = cache;
  c = (Input.a*Input.a + 2*Input.a*Input.b - Input.b*Input.b)*c;
  
  this->simplify();
  
  return *this;
}

alphaSet alphaSet::operator * ( const number multiplicator )const
{
  alphaSet Output;
  Output.a = a*multiplicator;
  Output.b = b*multiplicator;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator / ( const number divider )const
{
  alphaSet Output;
  Output.a = a;
  Output.b = b;
  Output.c = c*divider;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator + ( const int term )const
{
  alphaSet Output;
  Output.a = a + term;
  Output.b = b;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

alphaSet alphaSet::operator / ( const int term )const
{
  alphaSet Output;
  Output.a = a;
  Output.b = b;
  Output.c = c*term;
  
  Output.simplify();
  
  return Output;
}

bool alphaSet::operator == ( const alphaSet &Input )const
{
  if ( ( Input.c*a == Input.a*c ) && ( Input.c*b == Input.b*c ) )
    return true;
  
  return false;
}

bool alphaSet::operator != ( const alphaSet &Input )const
{
  if ( ( Input.c*a == Input.a*c ) && ( Input.c*b == Input.b*c ) )
    return false;
  
  return true;
}

bool alphaSet::operator < ( const alphaSet &Input )const
{
  number x;
  number y;
  number z;
  
  number A;
  number B;
  number C;
  
  if (sign(Input.c) == -1)
  {
    x = -Input.a;
    y = -Input.b;
    z = -Input.c;
  }
  else
  {
    x = Input.a;
    y = Input.b;
    z = Input.c;
  }
  
  if (sign(c) == -1)
  {
    A = -a;
    B = -b;
    C = -c;
  }
  else
  {
    A = a;
    B = b;
    C = c;
  }
  
  return ( 2*sign(z*B-C*y)*(z*B-C*y)*(z*B-C*y) < sign(C*x-z*A-z*B+C*y)*(C*x-z*A-z*B+C*y)*(C*x-z*A-z*B+C*y) );
}

bool alphaSet::operator > ( const alphaSet &Input )const
{
  number x;
  number y;
  number z;
  
  number A;
  number B;
  number C;
  
  if (sign(Input.c) == -1)
  {
    x = -Input.a;
    y = -Input.b;
    z = -Input.c;
  }
  else
  {
    x = Input.a;
    y = Input.b;
    z = Input.c;
  }
  
  if (sign(c) == -1)
  {
    A = -a;
    B = -b;
    C = -c;
  }
  else
  {
    A = a;
    B = b;
    C = c;
  }
  
  return ( 2*sign(z*B-C*y)*(z*B-C*y)*(z*B-C*y) > sign(C*x-z*A-z*B+C*y)*(C*x-z*A-z*B+C*y)*(C*x-z*A-z*B+C*y) );
}

bool alphaSet::operator <= (const alphaSet &Input)const
{
  return ((*this == Input) || (*this < Input));
}

bool alphaSet::operator >= ( const alphaSet &Input )const
{
  return ((*this == Input) || (*this > Input));
}

alphaSet::operator double() const
{
  return (a + b*constant)/c;
}

alphaSet alphaSet::abs() const
{
  alphaSet Output;
  if ( alphaSet::get(a,b) < alphaSet::get(0,0) )
  {
    Output.a = -a;
    Output.b = -b;
  }
  else
  {
    Output.a = a;
    Output.b = b;
  }
  Output.c = ( c < 0 ) ? -c : c;
  
  Output.simplify();
  
  return Output;
}

std::ostream& operator << ( std::ostream & stream, const alphaSet Input )
{
  //stream << "( " << Input.a << " + " << Input.b << "*beta )/ " << Input.c;
  stream.precision(5);
  stream << std::fixed << static_cast<double>(Input);
  
  return stream;
}

void print( std::ostream & stream, const alphaSet Input )
{
  if ( Input.b >= 0 )
  {
    stream << "(" << Input.a << "+" << Input.b << "*alpha)/" << Input.c;
  }
  else
  {
    stream << "(" << Input.a << "" << Input.b << "*alpha)/" << Input.c;
  }
}

void printFile( std::ostream & stream, const alphaSet Input )
{
  stream << Input.a << "_" << Input.b << "_" << Input.c;
}

std::istream& operator >> ( std::istream & stream, alphaSet& Input )
{
  //scanf( "( %ld %ld * sqrt(3) ) / %ld \t\n", &Input.a, &Input.b, &Input.c );
  stream.ignore(256,'(');
  stream >> Input.a >> Input.b;
  stream.ignore(256,'/');
  stream >> Input.c;
  
  return stream;
}

alphaSet alphaSet::coveringR()
{
  return alphaSet::get(10,0,18);
}

alphaSet alphaSet::circumscribedRhombusToCircle()
{
  return alphaSet::get(-2, 2);
}

alphaSet alphaSet::inscribedRhombusToCircle()
{
  return alphaSet::get(2*5411, 0, 10000);
}

alphaSet alphaSet::transformA()
{
  return alphaSet::get(1,0);
}

alphaSet alphaSet::transformB()
{
  return alphaSet::get(1,-1,2);
}

alphaSet alphaSet::transformC()
{
  return alphaSet::get(0,0);
}

alphaSet alphaSet::transformD()
{
  return alphaSet::get(-1,1,2);
}


alphaSet alphaSet::windowA()
{
  return alphaSet::get(1,0);
}

alphaSet alphaSet::windowB()
{
  return alphaSet::get(-1,1,2);
}

alphaSet alphaSet::windowC()
{
  return alphaSet::get(0,0);
}

alphaSet alphaSet::windowD()
{
  return alphaSet::get(-1,1,2);
}

alphaSet alphaSet::assign_L(const alphaSet c, const alphaSet d, const int k)
{
  if ( d - c < alphaSet::get( 3, -1 ) )
  {
    return alphaSet::get( 1, 3 );
  }
  else if ( d - c == alphaSet ::get( 3, -1 ) ) // equal
  {
    return alphaSet ::get( 0, 0 );
  }
  else if ( d - c < alphaSet ::get( 1, 0 ) )
  {
    return alphaSet ::get( 1, 2 );
  }
  else // equal 1
  {
    return alphaSet ::get( 0, 0 );
  }
}

alphaSet alphaSet::assign_M(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    return alphaSet::get(0, 1);
  }
  else
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return alphaSet ::get( 1, 2 );
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return alphaSet ::get( 1, 1 );
    }
  }
}

alphaSet alphaSet::assign_S(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return alphaSet ::get( 1, 2 );
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return alphaSet ::get( 1, 1 );
    }
  }
  else
  {
    return alphaSet::get(0, 1);
  }
}

alphaSet alphaSet::assign_Lchar(const alphaSet c, const alphaSet d, const int k)
{
  if ( d - c < alphaSet::get( 3, -1 ) )
  {
    return 'K';
  }
  else if ( d - c == alphaSet ::get( 3, -1 ) ) // equal
  {
    return '0';
  }
  else if ( d - c < alphaSet ::get( 1, 0 ) )
  {
    return 'L';
  }
  else // equal 1
  {
    return '0';
  }
}

alphaSet alphaSet::assign_Mchar(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    return 'N';
  }
  else
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return 'L';
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return 'M';
    }
  }
}
  
alphaSet alphaSet::assign_Schar(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return 'L';
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return 'M';
    }
  }
  else
  {
    return 'N';
  }
}

alphaSet alphaSet::assign_a(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return d + alphaSet::get(-5, 2);
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return d + alphaSet::get(-3, 1);
    }
  }
  else
  {
    return d - alphaSet::get(-2, 1);
  }
}

alphaSet alphaSet::assign_b(const alphaSet c, const alphaSet d, const int k)
{
  if (k%2 == 0)
  {
    return c + alphaSet::get(-2, 1);
  }
  else
  {
    if ( d - c <= alphaSet::get( 3, -1 ) )
    {
      return c - alphaSet::get(-5, 2);
    }
    else if ( d - c <= alphaSet ::get( 1, 0 ) )
    {
      return c - alphaSet::get(-3, 1);
    }
  }
}
