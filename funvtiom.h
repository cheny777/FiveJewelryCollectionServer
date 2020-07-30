#ifndef FUNVTIOM_H
#define FUNVTIOM_H
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

typedef struct ASIXvalue
{
    double X;
    double Y;
    double Z;
    double A;
    double C;
    double W;
} ASIXvalue;


void Savefile(char *strpath,std::vector<ASIXvalue> asv);
void Savefile(char *strpath,std::vector<ASIXvalue> asv,int radian,int size);





#endif // FUNVTIOM_H