#include <cmath>
#include <iostream>

#include "betaSet.h"
//#include "geometricObject2.h"

double betaSet::constant = 2 + sqrt(3);

// BETA SET ------------------------------------------------------------

betaSet::betaSet()
{
  a = 0;
  b = 0;
  c = 1;
}

betaSet::betaSet(number I_a)
{
  a = I_a;
  b = 0;
  c = 1;
}

betaSet::betaSet(number I_a, number I_b)
{
  a = I_a;
  b = I_b;
  c = 1;
}

betaSet::betaSet(number I_a, number I_b, number I_c)
{
  a = I_a;
  b = I_b;
  c = I_c;
  
  this->simplify();
}

void betaSet::set(number I_a, number I_b, number I_c)
{
  a = I_a;
  b = I_b;
  c = I_c;
  
  this->simplify();
}

betaSet betaSet::get(number I_a, number I_b)
{
  betaSet Output;
  Output.a = I_a;
  Output.b = I_b;
  Output.c = 1;
  
  return Output;
}

betaSet betaSet::get(number I_a, number I_b, number I_c)
{
  betaSet Output;
  Output.a = I_a;
  Output.b = I_b;
  Output.c = I_c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::betaK( int k )
{
  betaSet Output = betaSet::get(0,1);
  if ( k > 1 )
  {
    for ( int i = 1; i != k; ++i )
    {
      Output *= betaSet::get(0,1);
    }
  }
  else if ( k < 1 )
  {
    for ( int i = 1; i != k; --i )
    {
      Output /= betaSet::get(0,1);
    }
  }
  
  Output.simplify();
  
  return Output;
}

betaSet& betaSet::operator = ( const int &Input )
{
  constant = 2 + sqrt(3);
  a = Input;
  b = 0;
  c = 1;
  
  return *this;
}

int betaSet::simplify()
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

betaSet betaSet::star()const
{
  betaSet Output;
  Output.a = a + 4*b;
  Output.b = -b;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator + ( const betaSet &Input )const
{
  betaSet Output;
  
  Output.a = (a*Input.c + Input.a*c);
  Output.b = (b*Input.c + Input.b*c);
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator - ( const betaSet &Input )const
{
  betaSet Output;
  
  Output.a = (a*Input.c - Input.a*c);
  Output.b = (b*Input.c - Input.b*c);
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator * ( const betaSet &Input )const
{
  betaSet Output;
  
  Output.a = a*Input.a - b*Input.b;
  Output.b = b*Input.a + a*Input.b + 4*b*Input.b;
  Output.c = Input.c*c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator / ( const betaSet &Input )const
{
  betaSet Output;
  
  Output.a = (a*Input.a + b*Input.b + 4*a*Input.b)*Input.c;
  Output.b = (b*Input.a - a*Input.b)*Input.c;
  Output.c = (Input.a*Input.a + 4*Input.a*Input.b + Input.b*Input.b)*c;
  
  Output.simplify();
  
  return Output;
}

betaSet& betaSet::operator += ( const betaSet &Input )
{
  number tmp = gcd(c, Input.c);
  
  a = (a*Input.c + Input.a*c)/tmp;
  b = (b*Input.c + Input.b*c)/tmp;
  c = Input.c*c/tmp;
  
  this->simplify();
  
  return *this;
}

betaSet& betaSet::operator -= ( const betaSet &Input )
{
  number tmp = gcd(c, Input.c);
  
  a = (a*Input.c - Input.a*c)/tmp;
  b = (b*Input.c - Input.b*c)/tmp;
  c = Input.c*c/tmp;
  
  this->simplify();
  
  return *this;
}

betaSet& betaSet::operator *= ( const betaSet &Input )
{
  number cache = a*Input.a - b*Input.b;
  b = b*Input.a + a*Input.b + 4*b*Input.b;
  a = cache;
  c = Input.c*c;
  
  this->simplify();
  
  return *this;
}

betaSet& betaSet::operator /= ( const betaSet &Input )
{
  number cache = (a*Input.a + b*Input.b + 4*a*Input.b)*Input.c;
  b = (b*Input.a - a*Input.b)*Input.c;
  a = cache;
  c = (Input.a*Input.a + 4*Input.a*Input.b + Input.b*Input.b)*c;
  
  this->simplify();
  
  return *this;
}

betaSet betaSet::operator * ( const number multiplicator )const
{
  betaSet Output;
  Output.a = a*multiplicator;
  Output.b = b*multiplicator;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator / ( const number divider )const
{
  betaSet Output;
  Output.a = a;
  Output.b = b;
  Output.c = c*divider;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator + ( const int term )const
{
  betaSet Output;
  Output.a = a + term;
  Output.b = b;
  Output.c = c;
  
  Output.simplify();
  
  return Output;
}

betaSet betaSet::operator / ( const int term )const
{
  betaSet Output;
  Output.a = a;
  Output.b = b;
  Output.c = c*term;
  
  Output.simplify();
  
  return Output;
}

bool betaSet::operator == ( const betaSet &Input )const
{
  if ( ( Input.c*a == Input.a*c ) && ( Input.c*b == Input.b*c ) )
    return true;
  
  return false;
}

bool betaSet::operator != ( const betaSet &Input )const
{
  if ( ( Input.c*a == Input.a*c ) && ( Input.c*b == Input.b*c ) )
    return false;
  
  return true;
}

bool betaSet::operator < ( const betaSet &Input )const
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
  
  if ( sign( A*z+B*z*2 - x*C-y*C*2 )*( A*z+B*z*2 - x*C-y*C*2 )*( A*z+B*z*2 - x*C-y*C*2 ) < sign(y*C - B*z)*(y*C - B*z)*(y*C - B*z)*3 )
    return true;
  
  return false;
}

bool betaSet::operator > ( const betaSet &Input )const
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
  
  if ( sign( A*z+B*z*2 - x*C-y*C*2 )*( A*z+B*z*2 - x*C-y*C*2 )*( A*z+B*z*2 - x*C-y*C*2 ) > sign(y*C - B*z)*(y*C - B*z)*(y*C - B*z)*3 )
    return true;
  
  return false;
}

bool betaSet::operator <= (const betaSet &Input)const
{
  return ((*this == Input) || (*this < Input));
}

bool betaSet::operator >= ( const betaSet &Input )const
{
  return ((*this == Input) || (*this > Input));
}

betaSet::operator double() const
{
  return (a + b*constant)/c;
}

betaSet betaSet::abs() const
{
  betaSet Output;
  if ( betaSet::get(a,b) < betaSet::get(0,0) )
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

std::ostream& operator << ( std::ostream & stream, const betaSet Input )
{
  //stream << "( " << Input.a << " + " << Input.b << "*beta )/ " << Input.c;
  stream.precision(5);
  stream << std::fixed << static_cast<double>(Input);
  
  return stream;
}

void print( std::ostream & stream, const betaSet Input )
{
  if ( Input.b >= 0 )
  {
    stream << "(" << Input.a << "+" << Input.b << "*beta)/" << Input.c;
  }
  else
  {
    stream << "(" << Input.a << "" << Input.b << "*beta)/" << Input.c;
  }
}

void printFile( std::ostream & stream, const betaSet Input )
{
  stream << Input.a << "_" << Input.b << "beta" << "_" << Input.c;
}

std::istream& operator >> ( std::istream & stream, betaSet& Input )
{
  //scanf( "( %ld %ld * sqrt(3) ) / %ld \t\n", &Input.a, &Input.b, &Input.c );
  stream.ignore(256,'(');
  stream >> Input.a >> Input.b;
  stream.ignore(256,'/');
  stream >> Input.c;
  
  return stream;
}

betaSet betaSet::coveringR()
{
  return betaSet::get(161,-43);
}

betaSet betaSet::circumscribedRhombusToCircle()
{
  return betaSet::get(4, 0);
}

betaSet betaSet::inscribedRhombusToCircle()
{
  return betaSet::get(15, -3, 4);
}

betaSet betaSet::transformA()
{
  return betaSet::get(1,0);
}

betaSet betaSet::transformB()
{
  return betaSet::get(2,-1,2);
}

betaSet betaSet::transformC()
{
  return betaSet::get(0,0);
}

betaSet betaSet::transformD()
{
  return betaSet::get(1,0,2);
}


betaSet betaSet::windowA()
{
  return betaSet::get(1,0);
}

betaSet betaSet::windowB()
{
  return betaSet::get(-2,1,2);
}

betaSet betaSet::windowC()
{
  return betaSet::get(0,0);
}

betaSet betaSet::windowD()
{
  return betaSet::get(1,0,2);
}



betaSet betaSet::rotateA()
{
  return betaSet::get(-2,1,2);
}

betaSet betaSet::rotateB()
{
  return betaSet::get(-1,0,2);
}

betaSet betaSet::rotateC()
{
  return betaSet::get(1,0,2);
}

betaSet betaSet::rotateD()
{
  return betaSet::get(-2,1,2);
}

int betaSet::rotateN()
{
  return 12;
}


betaSet betaSet::assign_L(const betaSet c, const betaSet d, const int k)
{
  if ( d - c < betaSet::get( -7, 2 ) )
  {
    return betaSet::get( -1, 4 );
  }
  else if ( d - c == betaSet ::get( -7, 2 ) ) // equal
  {
    return betaSet ::get( 0, 0 );
  }
  else if ( d - c < betaSet ::get( -3, 1 ) )
  {
    return betaSet ::get( -1, 3 );
  }
  else if ( d - c == betaSet ::get( -3, 1 ) ) // equal
  {
    return betaSet ::get( 0, 0 );
  }
  else if ( d - c < betaSet ::get( 1, 0 ) )
  {
    return betaSet ::get( -1, 2 );
  }
  else // equal 1
  {
    return betaSet ::get( 0, 0 );
  }
}

betaSet betaSet::assign_M(const betaSet c, const betaSet d, const int k)
{
  if ( d - c <= betaSet::get( -7, 2 ) )
  {
    return betaSet ::get( -1, 3 );
  }
  else if ( d - c <= betaSet ::get( -3, 1 ) )
  {
    return betaSet ::get( -1, 2 );
  }
  else if ( d - c <= betaSet ::get( 1, 0 ) )
  {
    return betaSet ::get( -1, 1 );
  }
}

betaSet betaSet::assign_S(const betaSet c, const betaSet d, const int k)
{
  return betaSet ::get( 0, 1 );
}

betaSet betaSet::assign_Lchar(const betaSet c, const betaSet d, const int k)
{
  if ( d - c < betaSet::get( -7, 2 ) )
  {
    return 'A';
  }
  else if ( d - c == betaSet ::get( -7, 2 ) ) // equal
  {
    return '0';
  }
  else if ( d - c < betaSet ::get( -3, 1 ) )
  {
    return 'B';
  }
  else if ( d - c == betaSet ::get( -3, 1 ) ) // equal
  {
    return '0';
  }
  else if ( d - c < betaSet ::get( 1, 0 ) )
  {
    return 'C';
  }
  else // equal 1
  {
    return '0';
  }
}

betaSet betaSet::assign_Mchar(const betaSet c, const betaSet d, const int k)
{
  if ( d - c <= betaSet::get( -7, 2 ) )
  {
    return 'B';
  }
  else if ( d - c <= betaSet ::get( -3, 1 ) )
  {
    return 'C';
  }
  else if ( d - c <= betaSet ::get( 1, 0 ) )
  {
    return 'E';
  }
}

betaSet betaSet::assign_Schar(const betaSet c, const betaSet d, const int k)
{
  return 'D';
}

betaSet betaSet::assign_a(const betaSet c, const betaSet d, const int k)
{
  return d + betaSet ::get( -4, 1 );
}

betaSet betaSet::assign_b(const betaSet c, const betaSet d, const int k)
{
  if ( d - c <= betaSet::get( -7, 2 ) )
  {
    return c + betaSet::get( -11, 3 );
  }
  else if ( d - c <= betaSet ::get( -3, 1 ) )
  {
    return c + betaSet ::get( -7, 2 );
  }
  else if ( d - c <= betaSet ::get( 1, 0 ) )
  {
    return c + betaSet ::get( -3, 1 );
  }
}


//SUPORT FUNCTIONS
number gcd( number a, number b )
{
  a = ( a > 0 ) ? a : -a;
  b = ( b > 0 ) ? b : -b;
  
  if (a == 0) 
    return b;
  if (b == 0)
    return a;
  
  int shift;
  for (shift = 0; ((a | b) & 1) == 0; ++shift)
  {
    a >>= 1;
    b >>= 1;
  }
  
  while ((a & 1) == 0)
  {
    a >>= 1;
  }
  
  do 
  {
    while ((b & 1) == 0)
    {
      b >>= 1;
    }
    
    if (a > b)
    {
      number tmp = a;
      a = b;
      b = tmp;
    }
    
    b = b - a;
  } while (b != 0);
  //std::cout << "gcd: " << a << shift << std::endl;
  return a << shift;
}

number sign( number a )
{
  if ( a > 0 )
    return 1;
  else if ( a < 0 )
    return -1;
  else
    return 0;
}
