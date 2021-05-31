/*******************************************************
 File Name   : hmiapi_abstract.cpp

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/07/15
l
Description   :   Abstract Hmiapi
*********************************************************/

/****************Include  Section Begin************************************************/
#include "../inc/hmiapi_abstract.h"
/****************Include  Section  End*************************************************/

AbstractAPIClass::AbstractAPIClass(QObject* parent) : QObject(parent)
{
    setParent(parent);

    appClass = dynamic_cast<Frm_ApiClass*>(parent);

#if D_HMIAPI_CARPLAY_SWITCH
    m_bCarplaySwitch = true;
#else
    m_bCarplaySwitch = false;
#endif

#if D_HMIAPI_MIRLINK_SWITCH
    m_bMirlinkSwitch = true;
#else
    m_bMirlinkSwitch = false;
#endif

#if D_HMIAPI_WELINK_SWITCH
    m_bWelinkSwitch = true;
#else
    m_bWelinkSwitch = false;
#endif

#if D_HMIAPI_NAVI_SWITCH
    m_bNaviSwitch = true;
#else
    m_bNaviSwitch = false;
#endif

#if D_HMIAPI_MIRCAST_SWITCH
    m_bMirCastSwitch = true;
#else
    m_bMirCastSwitch = false;
#endif

#if D_HMIAPI_IPOD_SWITCH
    m_bIpodSwitch = true
#else
    m_bIpodSwitch = false;
#endif

#if D_HMIAPI_VR_SWITCH
    m_bVRSwitch = true;
#else
    m_bVRSwitch = false;
#endif

}

void AbstractAPIClass::customEvent(QEvent *e)
{
    concreteEvent(e);

    return;
}

void AbstractAPIClass::noticeCleareImportClass()
{
    return;
}

AbstractAPIClass::~AbstractAPIClass()
{
}

void AbstractAPIClass::CopyStr(char* str1, const char* str2, int sizemax)
{
    int isize = 0;

    if((NULL == str1)||(NULL == str2))
    {
        HMI_LOG("error!!");
        return;
    }

    isize = strlen(str2);

    if(isize >= sizemax)
    {
       isize = sizemax - 1;
    }
    memcpy(str1, str2, isize);
    str1[isize] = '\0';

    return;
}

bool AbstractAPIClass::CmpStr(char* str1, const char* str2)
{
    int isize1 = 0;
    int isize2 = 0;

    if((NULL == str1)||(NULL == str2))
    {
        HMI_LOG("error!!");
        return false;
    }

    isize1 = strlen(str1);
    isize2 = strlen(str2);

    if ( 0 != isize1 && 0 == isize2 )
    {
        return false;
    }

    if(0 == memcmp(str1, str2, isize2))
    {
        return true;
    }
    else
    {
        return false;
    }
}
