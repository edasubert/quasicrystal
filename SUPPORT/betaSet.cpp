#include <cmath>
#include <iostream>

#include "betaSet.h"
#include "geometricObject2.h"

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
  stream << Input.a << "_" << Input.b << "_" << Input.c;
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

// WINDOW --------------------------------------------------------------

window::window()
{
  m_c = betaSet::get( 0, 0 );
  m_d = betaSet::get( 1, 0 );
  m_k = 0;
  
  m_L = betaSet::get( 0, 0 );
  m_M = betaSet::get( 0, 1 );
  m_S = betaSet::get( -1, 1 );
  
  m_Lchar = '0';
  m_Mchar = 'D';
  m_Schar = 'E';
  
  m_a = betaSet::get( -3, 1 );
  m_b = betaSet::get( -3, 1 );
}

window::window( betaSet l )
{
  m_c = betaSet::get( 0, 0 );
  m_d = l;
  
  assign();
}

window::window( betaSet c, betaSet d )
{
  m_c = c;
  m_d = d;
  
  assign();
}

void window::assign()
{
  betaSet tmp = betaSet::get( 1, 0 );
  m_k = 0;
  
  while ( tmp*m_d - tmp*m_c <= betaSet::get( 4, -1 ) ) {
    tmp*= betaSet::get( 0, 1 );
    m_k++;
  }
  while ( tmp*m_d - tmp*m_c > betaSet::get( 1, 0 ) ) {
    tmp*= betaSet::get( 4, -1 ); // 1/beta
    m_k--;
  }
  
  if ( tmp*m_d - tmp*m_c < betaSet::get( -7, 2 ) )
  {
    m_L = betaSet::get( -1, 4 )*tmp;
    m_M = betaSet::get( -1, 3 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = 'A';
    m_Mchar = 'B';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -11, 3 )/tmp;
  }
  else if ( tmp*m_d - tmp*m_c == betaSet::get( -7, 2 ) ) // equal
  {
    m_L = betaSet::get( 0, 0 )*tmp;
    m_M = betaSet::get( -1, 3 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = '0';
    m_Mchar = 'B';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -11, 3 )/tmp;
  }
  else if ( tmp*m_d - tmp*m_c < betaSet::get( -3, 1 ) )
  {
    m_L = betaSet::get( -1, 3 )*tmp;
    m_M = betaSet::get( -1, 2 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = 'B';
    m_Mchar = 'C';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -7, 2 )/tmp;
  }
  else if ( tmp*m_d - tmp*m_c == betaSet::get( -3, 1 ) ) // equal
  {
    m_L = betaSet::get( 0, 0 )*tmp;
    m_M = betaSet::get( -1, 2 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = '0';
    m_Mchar = 'C';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -7, 2 )/tmp;
  }
  else if ( tmp*m_d - tmp*m_c < betaSet::get( 1, 0 ) )
  {
    m_L = betaSet::get( -1, 2 )*tmp;
    m_M = betaSet::get( -1, 1 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = 'C';
    m_Mchar = 'E';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -3, 1 )/tmp;
  }
  else // equal 1
  {
    m_L = betaSet::get( 0, 0 )*tmp;
    m_M = betaSet::get( -1, 1 )*tmp;
    m_S = betaSet::get( 0, 1 )*tmp;
    
    m_Lchar = '0';
    m_Mchar = 'E';
    m_Schar = 'D';
    
    m_a = m_d + betaSet::get( -4, 1 )/tmp;
    m_b = m_c + betaSet::get( -3, 1 )/tmp;
  }
}

betaSet window::L() const
{
  return m_L;
}
betaSet window::M() const
{
  return m_M;
}
betaSet window::S() const
{
  return m_S;
}

betaSet window::Small() const
{
  if ( ( m_S < m_M ) && ( ( m_S < m_L ) || ( m_L == betaSet::get(0,0) ) ) )
  {
    return m_S;
  }
  else if ( ( m_M < m_L ) || ( m_L == betaSet::get(0,0) ) )
  {
    return m_M;
  }
  return m_L;
}
betaSet window::Large() const
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

char window::Lchar() const
{
  return m_Lchar;
}
char window::Mchar() const
{
  return m_Mchar;
}
char window::Schar() const
{
  return m_Schar;
}

betaSet window::c() const
{
  return m_c;
}
betaSet window::d() const
{
  return m_d;
}
betaSet window::a() const
{
  return m_a;
}
betaSet window::b() const
{
  return m_b;
}
betaSet window::l() const
{
  return m_d-m_c;
}
int window::k() const
{
  return m_k;
}

betaSet window::char2space( char letter ) const
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
    return betaSet::get( 0, 0 );
  }
}

betaSet window::step( betaSet xStar, bool leftLimit )
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

betaSet window::stepBack( betaSet xStar )
{
  if ( ( m_c <= xStar && xStar < m_c + m_d - m_b ) )
  {
    return xStar - m_M.star();
  }
  else if ( ( m_c + m_d - m_b <= xStar && xStar < m_c + m_d - m_a) )
  {
    return xStar - m_L.star();
  }
  else if ( ( m_c + m_d - m_a <= xStar && xStar < m_d ) )
  {
    return xStar - m_S.star();
  }
}

void window::var_dump( std::ostream& out ) const
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
  print( out, m_k );
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

betaSet window::seed() const
{
  return ( ( m_c + m_d )*betaSet::get(1,0,2) ).star();
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
