#include <stdio.h>

int main(void) {
    int x;
    int y;
    FILE *fp;
    FILE *gnupipe=NULL;
    char *GnuCommands [] = {"set title \"Demo\"","plot 'data.tmp' u 1:2 w linespoints pt 11"};

    fp = fopen("data.tmp","w");

    gnupipe = _popen("gnuplot -persitent","w"); //works for windows

    for (int i=0; i<11;i++) 
    {
        //generate the data 
        fprintf(fp,"%d %d\n",x,y);
        x=x+1;
        y=y+1;
    }
    for(int i=0; i<2;i++) //go through commands
    {
        //plot the data
        fprintf(gnupipe, "%s\n",GnuCommands[i]);
    }
    return 0;
}