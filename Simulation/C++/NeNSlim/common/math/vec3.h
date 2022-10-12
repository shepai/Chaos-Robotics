
#ifndef VEC3_H
#define VEC3_H




struct VEC3
{
public:
	double	x,y,z;

	void	Zero( void );
	void	Set( double X, double Y, double Z );
	void	Set( VEC3 );
	void	Normalize( void );
	void	Negate(void);
	double	Mag( void );
	double	Mag2( void );
	void	RotateABS( VEC3, double );

	VEC3	operator/( double );
	VEC3	operator*( double );
	VEC3	operator+( VEC3 );
	VEC3	operator-( VEC3 );
	double	operator*( VEC3 );
	VEC3	operator^( VEC3 );
};




#endif