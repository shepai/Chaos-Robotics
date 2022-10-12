
#ifndef VEC2_H
#define VEC2_H


struct VEC2
{
public:
	double	x,y;

	void	Zero( void );
	void	Set( double X, double Y );
	void	Set( VEC2 );
	void	Normalize( void );
	void	Negate(void);
	double	Mag( void );
	double	Mag2( void );
	
	VEC2	operator/( double );
	VEC2	operator*( double );
	VEC2	operator+( VEC2 );
	VEC2	operator-( VEC2 );
	double	operator*( VEC2 );

};



#endif