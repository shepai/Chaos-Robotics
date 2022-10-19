



#ifndef VECND_H
#define VECND_H


#define MAX_NUM_DIM	100

class VECTORN
{
public:
	
	double	val[MAX_NUM_DIM];

	VECTORN();
	VECTORN( double* );
	~VECTORN();

	void	Zero( void );	
	void	Normalize( void );
	void	Negate(void);
	double	Mag( void );
	
	VECTORN	operator/( double );
	VECTORN	operator*( double );
	VECTORN	operator+( const VECTORN &v );
	VECTORN	operator-( const VECTORN &v );
	double	operator*( const VECTORN &v );

	VECTORN	&operator=( const VECTORN &v );

//	void	RotateABS( VEC3, double );
//	VEC3	operator^( VEC3 );

};

void SetNumDimension_VecNd( int );

#endif