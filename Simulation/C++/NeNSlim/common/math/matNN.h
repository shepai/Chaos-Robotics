



#ifndef MATRIXNXN_H
#define MATRIXNXN_H


#include "vecN.h"

class MATRIXNXN
{
public:
	
	double** val;

	MATRIXNXN();
	MATRIXNXN::MATRIXNXN( double** );
	~MATRIXNXN();

	void	Zero( void );
	void	Set_Identity( void );

	MATRIXNXN	operator*( const MATRIXNXN &m );
	MATRIXNXN	operator+( const MATRIXNXN &m );
	MATRIXNXN	operator-( const MATRIXNXN &m );
	MATRIXNXN	&operator=( const MATRIXNXN &m );

	void Set( double arg, ... );

	void CopyFrom( MATRIXNXN );

	void Transpose( void );

	void QR_Factorisation( MATRIXNXN& , MATRIXNXN& );

	void QR_Factorisation_LCE( MATRIXNXN& , MATRIXNXN& );


//	MATRIXNXN	operator/( double );

//	double	operator*( MATRIXNXN &v );

//	MATRIXNXN	&operator=( MATRIXNXN &m );



};

void SetNumDimension_NxN( int );
void Delete_Global_Variables_NxN( void );

#endif