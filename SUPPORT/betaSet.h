#ifndef _BETAsET_
#define _BETAsET_

#include <cstring>
#include <iostream>
#include <cstdio>

typedef long long int number;

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
        
        betaSet static coveringR();
        betaSet static circumscribedRhombusToCircle();
        betaSet static inscribedRhombusToCircle();
        
        betaSet static transformA(); //constants for transformBeta
        betaSet static transformB();
        betaSet static transformC();
        betaSet static transformD();
        
        betaSet static windowA(); //constants for window transformation
        betaSet static windowB();
        betaSet static windowC();
        betaSet static windowD();
        
        betaSet static rotateA(); //constants for rotation
        betaSet static rotateB();
        betaSet static rotateC();
        betaSet static rotateD();
        int static rotateN();
        
        
        // window parameters
        betaSet static assign_L(const betaSet c, const betaSet d, const int k);
        betaSet static assign_M(const betaSet c, const betaSet d, const int k);
        betaSet static assign_S(const betaSet c, const betaSet d, const int k);
        betaSet static assign_Lchar(const betaSet c, const betaSet d, const int k);
        betaSet static assign_Mchar(const betaSet c, const betaSet d, const int k);
        betaSet static assign_Schar(const betaSet c, const betaSet d, const int k);
        betaSet static assign_a(const betaSet c, const betaSet d, const int k);
        betaSet static assign_b(const betaSet c, const betaSet d, const int k);
};


number gcd( number a, number b );
number sign( number a );

#endif
