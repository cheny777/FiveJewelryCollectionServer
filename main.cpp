#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include "cicomm.h"
#include <iomanip>
#include <vector>
#include <math.h>
#include <map>
#include "funvtiom.h"

#define PI 3.1415926535897932384626433832795
#define RADIAN PI/180.0


/*五轴连续采集*/
using namespace std;
#ifndef WIN32
const string axispath = "/home/Lynuc/Users/NCFiles/TESTFILE.NC";
#else
const string axispath = "./TESTFILEn.NC";
#endif


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifndef WIN32
    int key =CreateCIKernel();
#endif
    cout<<"start 1"<<endl;
    double dou;
    /*X:30118
      Y:30168
      Z:30218
      A:30268
      B:30318
      C:30368
      W:30528
    */
    vector<ASIXvalue> ASIXvaluelist;
    vector<int> CIValue;
    CIValue.push_back(30118);
    CIValue.push_back(30168);
    CIValue.push_back(30218);
    CIValue.push_back(30268);
    CIValue.push_back(30368);
    CIValue.push_back(30528);
    double XYZAC[6];
    cout<<"start "<<endl;
#ifndef WIN32
    while (1)
#endif
    {
        usleep(10000);
#ifndef WIN32
        GetMacroVal(key,561,dou);
        if((int )(dou+0.00001) == 1)
#endif
        {
#ifndef WIN32
            /*数据采集*/
            for(int i =0 ;i<CIValue.size();i++)
            {
                GetMacroVal(key,CIValue[i],XYZAC[i]);
            }
            ASIXvalue v;
            v.X = XYZAC[0];
            v.Y = XYZAC[1];
            v.Z = XYZAC[2];
            v.A = XYZAC[3];
            v.C = XYZAC[4];
            v.W = XYZAC[5];
            ASIXvaluelist.push_back(v);
#else
            ifstream infile;
            infile.open("./TESTFILE.NC",ios::in);
            if(infile.is_open())
            {
                char axis;
                while(!infile.eof())
                {
                    infile>>axis>>XYZAC[0]>>
                            axis>>XYZAC[1]>>
                            axis>>XYZAC[2]>>
                            axis>>XYZAC[3]>>
                            axis>>XYZAC[4]>>
                            axis>>XYZAC[5];
                    ASIXvalue v;
                    v.X = XYZAC[0];
                    v.Y = XYZAC[1];
                    v.Z = XYZAC[2];
                    v.A = XYZAC[3];
                    v.C = XYZAC[4];
                    v.W = XYZAC[5];
                    ASIXvaluelist.push_back(v);
                }
            }
            else
            {
                cout<<"open file error !"<<endl;
            }
#endif
        }
#ifndef WIN32
        else if ((int )(dou+0.00001) == 2)
#endif
        {
            /*数据零时容器*/
            vector<vector<ASIXvalue>> flagAxisList;
            vector<ASIXvalue> flagAxisListList;

            /*筛选数据，确保数据再圆上，且没有多余点位*/
            int flag = 0;
            double lastpoint = 0;
            for(int i =0;i<ASIXvaluelist.size();i++)
            {
                if(fabs(ASIXvaluelist[i].W)>1.0)
                {
                    double sqrt_l = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
                    if(fabs(sqrt_l-lastpoint)<0.01)
                    {
                        flagAxisListList.push_back(ASIXvaluelist[i]);
                    }
                    if(i > ASIXvaluelist.size() - 5)
                    {
                        lastpoint = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
                    }
                    else
                    {
                        lastpoint = sqrt(ASIXvaluelist[i+4].X*ASIXvaluelist[i+4].X+ASIXvaluelist[i+4].Y*ASIXvaluelist[i+4].Y);
                    }
                    flag = 1;
                }
                else
                {
                    if(i > ASIXvaluelist.size() - 5)
                    {
                        lastpoint = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
                    }
                    else
                    {
                        lastpoint = sqrt(ASIXvaluelist[i+4].X*ASIXvaluelist[i+4].X+ASIXvaluelist[i+4].Y*ASIXvaluelist[i+4].Y);
                    }

                    if(flag != 0)
                    {
                        flagAxisList.push_back(flagAxisListList);
                        flagAxisListList.clear();
                        flag = 0;
                    }
                }
            }


            cout<<"open file"<<endl;
            ofstream outfile;
            outfile.open(axispath.c_str(),ios::out|ios::trunc);

            if(outfile.is_open())
            {
#ifndef WIN32
                for(int i =0;i<ASIXvaluelist.size();i++)
                {
                    outfile<<"X"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].X
                          <<"Y"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].Y
                         <<"Z"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].Z
                        <<"A"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].A
                       <<"C"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].C
                      <<"W"<<setiosflags(ios::fixed)<<setprecision(4)<<ASIXvaluelist[i].W<<endl;
                }
#else
                /*测试半径*/
//                ofstream outfile2;
//                outfile2.open("./test.nc",ios::out|ios::trunc);
//                if(outfile2.is_open())
//                {
//                    for(int i =0;i<flagAxisList.size();i++)
//                    {
//                        for(int j =0 ;j<flagAxisList[i].size();j++)
//                        {
//                            outfile2<<"X"<<setiosflags(ios::fixed)<<setprecision(4)<<sqrt(flagAxisList[i][j].X*flagAxisList[i][j].X+flagAxisList[i][j].Y*flagAxisList[i][j].Y)
//                                  <<"Y"<<setiosflags(ios::fixed)<<setprecision(4)<<i<<endl;
//                        }
//                    }
//                }


                vector<double > rediusList;
                   // flagAxisList.erase()
                for(int i =0;i<flagAxisList.size();i++)
                {

                    double redius_l = 0;
                    for(int j = 2 ;j<flagAxisList[i].size();j++)
                    {
                        outfile<<"X"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].X
                              <<"Y"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].Y
                             <<"Z"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].Z
                            <<"A"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].A
                           <<"C"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].C
                          <<"W"<<setiosflags(ios::fixed)<<setprecision(4)<<flagAxisList[i][j].W<<endl;
                        redius_l += sqrt(flagAxisList[i][j].X*flagAxisList[i][j].X+flagAxisList[i][j].Y*flagAxisList[i][j].Y);
                    }
                    rediusList.push_back(redius_l);
                    cout<<flagAxisList[i].size()<<endl;
                }

                cout<<"-----------------"<<endl;
                for(int i = 0;i<rediusList.size();i++)
                {
                    rediusList[i] = rediusList[i]/flagAxisList[i].size();
                    cout<<rediusList[i]<<endl;
                }
                cout<<"-----------------"<<endl;


                double sinlimit[36];
                double coslimit[36];
                for(int i =0;i<36;i++)
                {
                    sinlimit[i] = sin((5 + 10*i)*RADIAN);
                    coslimit[i] = cos((5 + 10*i)*RADIAN);

                }
                vector<ASIXvalue> limitCalculateList;
                vector<ASIXvalue> limitCalculateList36;
                for(int i =0 ;i<flagAxisList.size();i++)
                {
                    limitCalculateList.clear();
                    for(int ii = 0;ii<36;ii++)
                    {
                        ASIXvalue limit;
                        limit.X = rediusList[i]* coslimit[ii];
                        limit.Y = rediusList[i]* sinlimit[ii];
                        limitCalculateList.push_back(limit);
                    }
                    int flagi = 0;
                    for(int j =0 ;j<flagAxisList[i].size()-1;j++)
                    {
                        if((limitCalculateList[flagi].X>=flagAxisList[i][j].X && limitCalculateList[flagi].X<flagAxisList[i][j+1].X)||
                                (limitCalculateList[flagi].X<=flagAxisList[i][j].X && limitCalculateList[flagi].X>flagAxisList[i][j+1].X))
                        {
                            if(limitCalculateList[flagi].X==flagAxisList[i][j].X)
                            {
                               limitCalculateList36.push_back(flagAxisList[i][j]);
                            }
                            else
                            {
                                double radio = (flagAxisList[i][j].X-limitCalculateList[flagi].X)/(flagAxisList[i][j].X-flagAxisList[i][j+1].X);
                                ASIXvalue rediovalue;
                                rediovalue.X = limitCalculateList[flagi].X;
                                rediovalue.Y = limitCalculateList[flagi].Y;
                                rediovalue.Z = (flagAxisList[i][j+1].Z-flagAxisList[i][j].Z)*radio+flagAxisList[i][j].Z;
                                rediovalue.A = (flagAxisList[i][j+1].A-flagAxisList[i][j].A)*radio+flagAxisList[i][j].A;\
                                rediovalue.C = (flagAxisList[i][j+1].C-flagAxisList[i][j].C)*radio+flagAxisList[i][j].C;
                                rediovalue.W = (flagAxisList[i][j+1].W-flagAxisList[i][j].W)*radio+flagAxisList[i][j].W;
                                limitCalculateList36.push_back(rediovalue);

                            }
                            flagi++;
                        }
                    }
                }

                vector<ASIXvalue> limitCalculateList36Load;
                for(int i = 0;i<36; i++)
                {
                    for(int j =0 ;j<flagAxisList.size();j++)
                    {
                        limitCalculateList36Load.push_back(limitCalculateList36[j*36+i]);
                    }
                }

                Savefile("./tttt.nc",limitCalculateList36Load,36,flagAxisList.size());
                cout<<sin(30*RADIAN)<<endl;
#endif
            }
            else
            {
                cout<<"open error!"<<endl;
            }
            outfile.close();
            ASIXvaluelist.clear();
#ifndef WIN32
            SetMacroVal(key,561,0);
#endif
        }
#ifndef WIN32
        else
#endif
        {
            ASIXvaluelist.clear();
        }
    }

    cout<<"OK"<<endl;
    return a.exec();
}
