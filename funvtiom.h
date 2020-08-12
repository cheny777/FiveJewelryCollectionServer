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
bool Savefile(const char *strpath, std::vector<ASIXvalue> asv, int radian, int size);
bool SavefileNoW(const char *strpath,std::vector<ASIXvalue> asv,int radian,int size);
bool SaveFile(char *strpath, std::vector<std::vector<ASIXvalue> > asv,int size);




#endif // FUNVTIOM_H
