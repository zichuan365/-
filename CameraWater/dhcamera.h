#ifndef DHCAMERA_H
#define DHCAMERA_H
#include <GxIAPI.h>
#include <DxImageProc.h>

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <queue>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <map>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

////Show error message
#define GX_VERIFY(emStatus) \
    if (emStatus != GX_STATUS_SUCCESS)     \
    {                                      \
        GetErrorString(emStatus);          \
        return emStatus;                   \
    }

////Show error message, close device and lib
#define GX_VERIFY_EXIT(emStatus) \
    if (emStatus != GX_STATUS_SUCCESS)     \
    {                                      \
        GetErrorString(emStatus);          \
        GXCloseDevice(m_hDevice);          \
        m_hDevice = NULL;                  \
        GXCloseLib();                      \
        printf("<App Exit!>\n");           \
        return emStatus;                   \
    }
class CaptureHander;
typedef map<int,CaptureHander*> Cmap;

class CaptureHander
{
public:
    CaptureHander(bool isColor,bool& isScan,std::queue<cv::Mat>& imgQueTem);
    cv::Mat img;
    std::queue<cv::Mat>* imgQueue;

    bool m_isColorCamera;
    bool* m_isScan;

    //�����Ϊ��ɫ,�������RGBͼ�� / GRAYͼ��
    bool colorCameraOutput = true;

    //�ص�����
    static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

    void* g_pRGBImageBuf = NULL;                        ///< Memory for RAW8toRGB24
    void* g_pMonoImageBuf = NULL;                       ///< Memory for RAW16toRAW8


     //*�������
    static CaptureHander* p;
    void setCur()
    {
        p = this;
    }


};

class DhCamera
{
public:
    DhCamera();
    ~DhCamera();

    //������
    GX_DEV_HANDLE m_hDevice = NULL;

    //ע������Ļص�����
    CaptureHander* captureImgHander;

    std::queue<cv::Mat>* ImgQue;

    double m_ExposureMax,m_ExposureMin;
    double m_Exposure,m_ExposureOld;

    double m_GainMax,m_GainMin;
    double m_Gain,m_GainOld;

    //�ж�����Ƿ�Ϊ��ɫ���
    bool m_IsColor = true;
    int64_t m_ColorFilter = GX_COLOR_FILTER_NONE;

    bool m_ImgGet;

    bool m_IsScan = false;
    bool m_IsSourceTrigger = false;

    int64_t m_nPayloadSize;

    //    GX_OPEN_PARAM CAM;
    //    CAM.accessMode = GX_ACCESS_EXCLUSIVE;
    //    CAM.openMode   = GX_OPEN_SN;
    //    CAM.pszContent = "XE0190090156";

    //Ĭ�ϵ�һ�����Ϊ0
    bool ConnectCamera(int index,std::queue<cv::Mat>& imaQue,float Exposure,float Gain);
    bool ConnectCamera(GX_OPEN_PARAM *pOpenParam,std::queue<cv::Mat>& imaQue,float Exposure,float Gain);


    bool DisConnectCamera();
    bool SetFramerate(double val);

    bool SetExposure(float val);

    bool SetGain(float val);

    bool SetTriggerMode(bool flag);
    //timeOutһ֡��ռ�õ�ʱ��
    bool GrabAImg(cv::Mat& img,bool imgColor = false,double timeOut = 1);

    //ע���ص�����
    bool RegisterCaptuer();

    //��ÿ֡�����ڴ�
    void PreForAcquisition();

    void UnPreForAcquisition();

};

//��ȡerror��Ϣ
void GetErrorString(GX_STATUS emErrorStatus);
//��ȡ��ǰʱ��
std::string get_time();

void List(const char *path, int level, vector<string> &strvec);
#endif // DHCAMERA_H
