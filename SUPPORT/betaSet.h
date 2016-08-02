#ifndef _BETAsET_
#define _BETAsET_

#include <cstring>
#include <iostream>
#include <cstdio>

typedef long long number;

class betaSet
{
    protected:
        number a, b; // a+b(beta)/c
        number c;
        static double constant;
    public:
        betaSet();
        betaSet(number I_a);
        betaSet(number I_a, number I_b);
        betaSet(number I_a, number I_b, number I_c);
        //~betaSet();
        
        betaSet static get(number I_a, number I_b);
        betaSet static get(number I_a, number I_b, number I_c);
        betaSet static betaK( int k );
        
        void set(number I_a, number I_b, number I_c);
        
        int simplify();
        
        betaSet star()const;
        
        betaSet& operator = ( const int &Input );
        
        betaSet operator + ( const betaSet &Input )const;
        betaSet operator - ( const betaSet &Input )const;
        betaSet operator * ( const betaSet &Input )const;
        betaSet operator / ( const betaSet &Input )const;
        betaSet& operator += ( const betaSet &Input );
        betaSet& operator -= ( const betaSet &Input );
        betaSet& operator *= ( const betaSet &Input );
        betaSet& operator /= ( const betaSet &Input );
        
        betaSet operator * ( const number multiplicator )const;
        betaSet operator / ( const number divider )const;
        betaSet operator / ( const int term )const;
        betaSet operator + ( const int term )const;
        
        bool operator == ( const betaSet &Input )const;
        bool operator < ( const betaSet &Input )const;
        bool operator > ( const betaSet &Input )const;
        bool operator <= ( const betaSet &Input )const;
        bool operator >= ( const betaSet &Input )const;
        bool operator != ( const betaSet &Input )const;
        
        operator double() const;
        
        betaSet abs() const;
        
        friend std::ostream& operator << ( std::ostream & stream, const betaSet Input );
        friend void print( std::ostream & stream, const betaSet Input );
        friend void printFile( std::ostream & stream, const betaSet Input );
        friend std::istream& operator >> ( std::istream & stream, betaSet& Input );
        
};

class window {
    protected:
        betaSet m_c, m_d; // borders of window interval
        betaSet m_a, m_b; // dividors of diferent spaces
        int m_k;
        betaSet m_L, m_M, m_S; // sizes of spaces
        char m_Lchar, m_Mchar, m_Schar; // character representation of spaces
        
        void assign();
    public:
        window();
        window( betaSet l );
        window( betaSet c, betaSet d );
        
        betaSet c() const;
        betaSet d() const;
        betaSet a() const;
        betaSet b() const;
        betaSet l() const;
        int k() const;
        
        // S,M,L does not mean small, medium, large but name third, first and second semi-interval
        betaSet S() const;
        betaSet M() const;
        betaSet L() const;
        
        betaSet Small() const;
        betaSet Large() const;
        
        char Schar() const;
        char Mchar() const;
        char Lchar() const;
        
        betaSet char2space( char letter ) const;
        
        betaSet step( betaSet xStar, bool leftLimit = false );
        betaSet stepBack( betaSet xStar );
        
        void var_dump( std::ostream& out ) const;
        
        betaSet seed() const; // returns point of quasicrystal
};



number gcd( number a, number b );
number sign( number a );

#endif
