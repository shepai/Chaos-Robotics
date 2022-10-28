#include <stdio.h>
#include <cmath>
#include <iostream>

//store cordinae values
struct coords
{
     float x;
     float y;
};
/*
bvp
Generates the next x and y in the occilation
@param: x for the x coord
@param: y for the y coord

*/
coords bvp(float x,float y,int c,int a,float dt)
{
    float x_d=0.0,xs=0.0,ys=0.0;
    x_d=c*(x-((std::pow(x,3))/3) + y);
    xs=(c*(1-std::pow(x,2))*x_d) - x;
    ys=-x*a;
    //create coordinates to return
    coords res = {x+(xs*dt),y+(ys*dt)};
    return res;
}

int main(void) {
    float x=0.0,y=1.0;
    FILE *fp;
    FILE *gnupipe=NULL;
    char *GnuCommands [] = {"set title \"BVP signal\"","plot 'data.tmp' u 1:2 with linespoints lw 2 lc rgb \"black\""};

    fp = fopen("data.tmp","w");

    gnupipe = _popen("gnuplot -persitent","w"); //works for windows

    coords c;
    for (int i=0; i<1000;i++) 
    {
        //generate the data 
        fprintf(fp,"%f %f\n",x,y); //write to file
        c = bvp(x,y,10,10,0.005);
        x=c.x; //set the values
        y=c.y; //set the values
    }
    for(int i=0; i<2;i++) //go through commands
    {
        //plot the data
        fprintf(gnupipe, "%s\n",GnuCommands[i]);
    }
    
    std:: cout << "X: " << c.x << "Y: " << c.y;
    return 0;
}