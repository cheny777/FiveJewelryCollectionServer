#ifndef GMATH_H
#define GMATH_H

#include <math.h>
/*��һ�����ַ��ʣ��ÿ��+50 */
#define AXISDATA 30126

/*��һ��ת�����*/
#define FIRSTAXISOFROTATION 41185
/*�ڶ���ת�����*/
#define SECONDAXISOFROTATION 41188

/*��һ��ת��ƫת��ʸ��*/
#define THEFIRSTROTATIONAXISDEFLECTIONANGLEVECTOR 41130
/*�ڶ���ת��ƫת��ʸ��*/
#define THESECONDROTATIONAXISDEFLECTIONANGLEVECTOR 41135
/*��ת��������������*/
#define ROTATIONONOFF 34594


#ifdef WIN32
#define CIPATH  "./ci_set.txt"
#else
#define CIPATH "/home/Lynuc/Users/Config/ci_set.txt"
#endif

#define NCPATH "/home/Lynuc/Users/NCFiles/AXISNC.NC"

#define TRAN_PI 3.1415926535897932384626433832795
#define TRAN_DEG2RAD (TRAN_PI / 180.0)
#define TRAN_DEG2DEG (180.0 / TRAN_PI)

void RotVector(double *dRes, double *dSrc, double *dDir, double dAgl, bool bRightHand)
{
    double dCos = cos(dAgl);
    double dSin = sin(dAgl);
    double d1_cfq = (1.0 - dCos) * (dDir[0] * dSrc[0] + dDir[1] * dSrc[1] + dDir[2] * dSrc[2]);
    double dTemp1[3];
    double dTemp2[3];

    if(fabs(dAgl) < 1.0E-8)
    {
        dRes[0] = dSrc[0];
        dRes[1] = dSrc[1];
        dRes[2] = dSrc[2];
        return;
    }

    if(!bRightHand)
    {
        dSin = - dSin;
    }

    dTemp1[0] = (dDir[1] * dSrc[2] - dDir[2] * dSrc[1]) * dSin;
    dTemp1[1] = (dDir[2] * dSrc[0] - dDir[0] * dSrc[2]) * dSin;
    dTemp1[2] = (dDir[0] * dSrc[1] - dDir[1] * dSrc[0]) * dSin;

    dTemp2[0] = dSrc[0] * dCos + dDir[0] * d1_cfq;
    dTemp2[1] = dSrc[1] * dCos + dDir[1] * d1_cfq;
    dTemp2[2] = dSrc[2] * dCos + dDir[2] * d1_cfq;

    dRes[0] = dTemp1[0] + dTemp2[0];
    dRes[1] = dTemp1[1] + dTemp2[1];
    dRes[2] = dTemp1[2] + dTemp2[2];
}
/**
 * ���ݸ߶ȼ����µ�XYZ
 * dResDltaMove���µ�XYZ
 * dDeltaH���߶�
 * dFirstDir����һ��ת��ʸ��
 * dSecDir���ڶ���ת��ʸ��
 * dFridtAngle����һ��Ƕ�
 * dSecAngle���ڶ���Ƕ�
*/

void CalDeltaMoveCoord(double *dResDltaMove, double dDeltaH, double *dFirstDir, double *dSecDir, double dFridtAngle, double dSecAngle)
{
    double dDirW[3] = {0.0, 0.0, 0.0};
    double dTempVec[3] = {0.0, 0.0, 0.0};
    double dToolDir[3] = {0.0, 0.0, -1.0};
    RotVector(dTempVec, dToolDir, dFirstDir, dFridtAngle * TRAN_DEG2RAD, 1);
    RotVector(dDirW, dTempVec, dSecDir, dSecAngle * TRAN_DEG2RAD, 1);

    dResDltaMove[0] = dDirW[0] * dDeltaH;
    dResDltaMove[1] = dDirW[1] * dDeltaH;
    dResDltaMove[2] = dDirW[2] * dDeltaH;

}
void CalDeltaMoveCoordE(double *dResDltaMove, double dDeltaH,double dFridtAngle, double dSecAngle)
{
        double dTempVec[3] = {0.0, 0.0, 0.0};
        double dToolDir[3] = {0.0, 0.0, -1.0};

        dTempVec[0] = dToolDir[2] *sin(dFridtAngle*TRAN_DEG2RAD)*sin(dSecAngle*TRAN_DEG2RAD);
        dTempVec[1] = -dToolDir[2] *sin(dFridtAngle*TRAN_DEG2RAD)*cos(dSecAngle*TRAN_DEG2RAD);
        dTempVec[2] = dToolDir[2] *cos(dFridtAngle*TRAN_DEG2RAD);

        dResDltaMove[0] = dTempVec[0] * dDeltaH;
        dResDltaMove[1] = dTempVec[1] * dDeltaH;
        dResDltaMove[2] = dTempVec[2] * dDeltaH;
}

#endif // GMATH_H
