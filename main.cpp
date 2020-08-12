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
#include "gmath.h"
#include <algorithm>
#include <numeric>

#define PI 3.1415926535897932384626433832795
#define RADIAN PI/180.0


/*五轴连续采集*/
using namespace std;
#ifndef WIN32
const string axispath = "/home/Lynuc/Users/NCFiles/TEST_5AXIS.NC";
#else
const string axispath = "./TESTFILEn.NC";
#endif


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifndef WIN32
    int key =CreateCIKernel();
#endif

    int m_showDebug = 0;
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
    /*获取第一第二旋转轴*/
    double FristVector[3]={0};
    double SecondVector[3]={0};
    for(int i = 0; i < 3; i++)
    {
        GetMacroVal(key, THEFIRSTROTATIONAXISDEFLECTIONANGLEVECTOR + i,FristVector[i]);
        GetMacroVal(key, THESECONDROTATIONAXISDEFLECTIONANGLEVECTOR + i,SecondVector[i]);
    }
    if((FristVector[0] == 0)&&(FristVector[1]==0)&&(FristVector[2] == 0))
    {
        FristVector[0] = 1;
        FristVector[1] = 0;
        FristVector[2] = 0;
    }
    if((SecondVector[0] == 0)&&(SecondVector[1]==0)&&(SecondVector[2] == 0))
    {
        SecondVector[0] = 0;
        SecondVector[1] = 0;
        SecondVector[2] = 1;
    }
    double onoff=0;
    GetMacroVal(key, ROTATIONONOFF,onoff);
    int intonoff = onoff+0.00001;
    if(m_showDebug)
    {
        cout<<"vectoronoff:"<<intonoff<<endl;
    }
    if(intonoff==0)
    {
        FristVector[0] = 1;
        FristVector[1] = 0;
        FristVector[2] = 0;
        SecondVector[0] = 0;
        SecondVector[1] = 0;
        SecondVector[2] = 1;
    }

    if(m_showDebug)
    {
        for(int i =0;i<3;i++)
        {
            cout<<"firstVector :"<<FristVector[i]<<" , "<<SecondVector[i]<<endl;
        }
    }
#else
    /*获取第一第二旋转轴*/
    double FristVector[3]={0};
    double SecondVector[3]={0};
    FristVector[0] = 1;
    FristVector[1] = 0;
    FristVector[2] = 0;
    SecondVector[0] = 0;
    SecondVector[1] = 0;
    SecondVector[2] = 1;

#endif


    /*数据零时容器*/
    vector<vector<ASIXvalue> > flagAxisList;
    vector<ASIXvalue> flagAxisListList;
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
            double sdou;
            GetMacroVal(key,562,sdou);
            if((int )(sdou+0.00001) == 1)
            {
                flagAxisList.push_back(flagAxisListList);
                flagAxisListList.clear();
                SetMacroVal(key,562,0);
            }
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
            //ASIXvaluelist.push_back(v);
            flagAxisListList.push_back(v);
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
        SaveFile("/home/Lynuc/Users/NCFiles/OUTTESTNC.NC",flagAxisList,1);

//            /*筛选数据，确保数据再圆上，且没有多余点位*/
//            int flag = 0;
//            double lastpoint = 0;
//            for(int i =0;i<ASIXvaluelist.size();i++)
//            {
//                if(fabs(ASIXvaluelist[i].W)>1.0)
//                {
//                    double sqrt_l = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
//                    if(fabs(sqrt_l-lastpoint)<0.01)
//                    {
//                        flagAxisListList.push_back(ASIXvaluelist[i]);
//                    }
//                    if(i > ASIXvaluelist.size() - 5)
//                    {
//                        lastpoint = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
//                    }
//                    else
//                    {
//                        lastpoint = sqrt(ASIXvaluelist[i+4].X*ASIXvaluelist[i+4].X+ASIXvaluelist[i+4].Y*ASIXvaluelist[i+4].Y);
//                    }
//                    flag = 1;
//                }
//                else
//                {
//                    if(i > ASIXvaluelist.size() - 5)
//                    {
//                        lastpoint = sqrt(ASIXvaluelist[i].X*ASIXvaluelist[i].X+ASIXvaluelist[i].Y*ASIXvaluelist[i].Y);
//                    }
//                    else
//                    {
//                        lastpoint = sqrt(ASIXvaluelist[i+4].X*ASIXvaluelist[i+4].X+ASIXvaluelist[i+4].Y*ASIXvaluelist[i+4].Y);
//                    }

//                    if(flag != 0)
//                    {
//                        flagAxisList.push_back(flagAxisListList);
//                        flagAxisListList.clear();
//                        flag = 0;
//                    }
//                }
//            }

/**********************************/

            /*计算半径和*/
            vector<double > rediusList;
            for(int i =0;i<flagAxisList.size();i++)
            {
                double redius_l = 0;
                for(int j = 0 ;j<flagAxisList[i].size();j++)
                {
                    redius_l += sqrt(flagAxisList[i][j].X*flagAxisList[i][j].X+flagAxisList[i][j].Y*flagAxisList[i][j].Y);
                }
                rediusList.push_back(redius_l);
            }
            cout<<"-----------------"<<endl;
            /*计算平均半径*/
            for(int i = 0;i<rediusList.size();i++)
            {
                rediusList[i] = rediusList[i]/flagAxisList[i].size();
                cout<<rediusList[i]<<endl;
            }
            cout<<"-----------------"<<endl;
            /*均值新方法*/
//            for(int i =0;i<flagAxisList.size();i++)
//            {
//                double sum = accumulate(begin(flagAxisList[i]),end(flagAxisList[i]),0.0);
//                double mean = sum/flagAxisList[i].size();
//                rediusList.push_back(mean);
//            }

            /*每10度一个角，从5度开始*/
            double sinlimit[36];
            double coslimit[36];
            for(int i =0;i<36;i++)
            {
                sinlimit[i] = sin((355 - 10*i)*RADIAN);
                coslimit[i] = cos((355 - 10*i)*RADIAN);

            }
            /*差分每个角度下的点*/
            vector<ASIXvalue> limitCalculateList;
            vector<ASIXvalue> limitCalculateList36;
            for(int i =0 ;i<flagAxisList.size();i++)
            {
                limitCalculateList.clear();
                /*计算每个角度的点*/
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
                    /*判断是否在圆上的两点之间*/
                    if((limitCalculateList[flagi].X>=flagAxisList[i][j].X && limitCalculateList[flagi].X<flagAxisList[i][j+1].X)||
                            (limitCalculateList[flagi].X<=flagAxisList[i][j].X && limitCalculateList[flagi].X>flagAxisList[i][j+1].X))
                    {
                        if(limitCalculateList[flagi].X==flagAxisList[i][j].X)
                        {
                            limitCalculateList36.push_back(flagAxisList[i][j]);
                        }
                        else
                        {
                            /*差分计算*/
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
            /*横向纵向转化*/
            vector<ASIXvalue> limitCalculateList36Load;
            for(int i = 0;i<36; i++)
            {
                for(int j =0 ;j<flagAxisList.size();j++)
                {
                    limitCalculateList36Load.push_back(limitCalculateList36[j*36+i]);
                }
            }
            const char *sss = "./AXIS.nc";
            Savefile(sss,limitCalculateList36Load,36,flagAxisList.size());
            /*计算偏移*/
            for(int i =0;i<limitCalculateList36Load.size();i++)
            {
                double chaXYZ[3];
#ifndef WIN32
                CalDeltaMoveCoord(chaXYZ,-8-limitCalculateList36Load[i].W,FristVector,SecondVector,limitCalculateList36Load[i].A,limitCalculateList36Load[i].C);
#else
                CalDeltaMoveCoordE(chaXYZ,-8-limitCalculateList36Load[i].W,limitCalculateList36Load[i].A,limitCalculateList36Load[i].C);
#endif
                limitCalculateList36Load[i].X -=  chaXYZ[0];
                limitCalculateList36Load[i].Y -=  chaXYZ[1];
                limitCalculateList36Load[i].Z -=  chaXYZ[2];
            }

            /*保存*/
            SavefileNoW(axispath.c_str(),limitCalculateList36Load,36,flagAxisList.size());
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
