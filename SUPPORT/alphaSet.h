#ifndef _alphaSet_
#define _alphaSet_

#include <cstring>
#include <iostream>
#include <cstdio>

typedef long long int number;

class alphaSet
{
    protected:
        number a, b; // a+b(beta)/c
        number c;
        static double constant;
    public:
        alphaSet();
        alphaSet(number I_a);
        alphaSet(number I_a, number I_b);
        alphaSet(number I_a, number I_b, number I_c);
        
        alphaSet static get(number I_a, number I_b);
        alphaSet static get(number I_a, number I_b, number I_c);
        alphaSet static betaK( int k );
        
        void set(number I_a, number I_b, number I_c);
        
        int simplify();
        
        alphaSet star()const;
        
        alphaSet& operator = ( const int &Input );
        
        alphaSet operator + ( const alphaSet &Input )const;
        alphaSet operator - ( const alphaSet &Input )const;
        alphaSet operator * ( const alphaSet &Input )const;
        alphaSet operator / ( const alphaSet &Input )const;
        alphaSet& operator += ( const alphaSet &Input );
        alphaSet& operator -= ( const alphaSet &Input );
        alphaSet& operator *= ( const alphaSet &Input );
        alphaSet& operator /= ( const alphaSet &Input );
        
        alphaSet operator * ( const number multiplicator )const;
        alphaSet operator / ( const number divider )const;
        alphaSet operator / ( const int term )const;
        alphaSet operator + ( const int term )const;
        
        bool operator == ( const alphaSet &Input )const;
        bool operator < ( const alphaSet &Input )const;
        bool operator > ( const alphaSet &Input )const;
        bool operator <= ( const alphaSet &Input )const;
        bool operator >= ( const alphaSet &Input )const;
        bool operator != ( const alphaSet &Input )const;
        
        operator double() const;
        
        alphaSet abs() const;
        
        friend std::ostream& operator << ( std::ostream & stream, const alphaSet Input );
        friend void print( std::ostream & stream, const alphaSet Input );
        friend void printFile( std::ostream & stream, const alphaSet Input );
        friend std::istream& operator >> ( std::istream & stream, alphaSet& Input );
        
        alphaSet static coveringR();
        alphaSet static circumscribedRhombusToCircle();
        alphaSet static inscribedRhombusToCircle();
        
        alphaSet static transformA(); //constants for transformBeta
        alphaSet static transformB();
        alphaSet static transformC();
        alphaSet static transformD();
        
        alphaSet static windowA(); //constants for window transformation
        alphaSet static windowB();
        alphaSet static windowC();
        alphaSet static windowD();
        
        
        // window parameters
        alphaSet static assign_L(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_M(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_S(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_Lchar(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_Mchar(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_Schar(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_a(const alphaSet c, const alphaSet d, const int k);
        alphaSet static assign_b(const alphaSet c, const alphaSet d, const int k);
};

number gcd( number a, number b );
number sign( number a );

#endif
