/*******************************************************
 File Name   : hmiapi_abstract.h

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/07/15
l
Description   :   Abstract Hmiapi
*********************************************************/
#ifndef ABSTRACT_H
#define ABSTRACT_H
/****************Include  Section Begin***************************************************/
#include "hmifrm_common.h"
#include "hmifrm_apiclass.h"
#include "hmifrm_funcevent.h"
#include "hmi_log.h"
/****************Include  Section End*****************************************************/

/****************Struct Definition Section Start******************************************/
typedef struct
{
    int eventID;
    void* para;
}S_MSG;
/****************Struct Definition Section End********************************************/

/****************Struct Definition Section Begin******************************************/
#define D_HMIAPI_DOMAIN_MODEMANAGER             1
#define D_HMIAPI_DOMAIN_ENTERTAINMENT			2
#define D_HMIAPI_DOMAIN_SETTINGS    			3
#define D_HMIAPI_DOMAIN_SWDL                    4
#define D_HMIAPI_DOMAIN_DEVICES                 5
#define D_HMIAPI_DOMAIN_PHONECONTROL            6
#define D_HMIAPI_DOMAIN_MIRRORLINK              7
#define D_HMIAPI_DOMAIN_WIFISETTINGS            8
#define D_HMIAPI_DOMAIN_AUDIOMANAGER            9
#define D_HMIAPI_DOMAIN_VR                      10
#define D_HMIAPI_DOMAIN_CANSERVICE              11
#define D_HMIAPI_DOMAIN_IPC                     12
#define D_HMIAPI_DOMAIN_MIRRORCAST              13
#define D_HMIAPI_DOMAIN_NAVI                    14
#define D_HMIAPI_DOMAIN_RESOURCEMANAGER         15
#define D_HMIAPI_DOMAIN_USBSYNC                 16
#define D_HMIAPI_DOMAIN_DIAG                    17
#define D_HMIAPI_DOMAIN_TBOX                    18
#define D_HMIAPI_DOMAIN_WELINK                  19
#define M_HMIAPI_MAKEEV( domain, eventid )	(domain << 10 | eventid )
/****************Struct Definition Section End*********************************************/

/*****Feature enable and disable switch definition start****/
#define D_HMIAPI_AQS_SWITCH    1  //enale or diable AQS feature enable:1 disable:0
#define D_HMIAPI_TBOX_SWITCH   0  //enale or diable TBOX feature enable:1 disable:0
#define D_HMIAPI_NAVI_SWITCH   0  //enale or diable NAVI feature enable:1 disable:0 1212
#define D_HMIAPI_WELINK_SWITCH  1  //enale or diable WELINK feature enable:1 disable:0

#if D_HMIAPI_WELINK_SWITCH
#define D_HMIAPI_MIRLINK_SWITCH  0  //enale or diable MIRLINK feature enable:1 disable:0
#define D_HMIAPI_CARPLAY_SWITCH  0  //enale or diable CARPLAY feature enable:1 disable:0
#define D_HMIAPI_MIRCAST_SWITCH  0  //enale or diable MIRCAST feature enable:1 disable:0
#else
#define D_HMIAPI_MIRLINK_SWITCH  1  //enale or diable MIRLINK feature enable:1 disable:0
#define D_HMIAPI_CARPLAY_SWITCH  1  //enale or diable CARPLAY feature enable:1 disable:0
#define D_HMIAPI_MIRCAST_SWITCH  1  //enale or diable MIRCAST feature enable:1 disable:0
#endif

#define D_HMIAPI_IPOD_SWITCH  0  //enale or diable IPOD feature enable:1 disable:0

#define D_HMIAPI_VR_SWITCH  0  //enale or diable VR feature enable:1 disable:0

/*****Feature enable and disable switch definition end****/


class Frm_ApiClass;

/****************Class Declaration Section Begin******************************************/
class AbstractAPIClass : public QObject
{
    Q_OBJECT

    protected:

        Frm_ApiClass*   appClass;
        bool m_bWelinkSwitch; //1212
        bool m_bMirlinkSwitch;
        bool m_bCarplaySwitch;
        bool m_bNaviSwitch;//0：无导航功能；1：有导航功能
        bool m_bMirCastSwitch;
        bool m_bIpodSwitch;
        bool m_bVRSwitch;
    public:

        AbstractAPIClass(QObject* parent = 0);
        ~AbstractAPIClass();

#if 1   //add by Li.Shanda
        //The type of the message depend on the subsystem or the virtual BUS, here just use void*.
        virtual void sendEvent(int eventID) = 0;
#endif
        void customEvent(QEvent *);

        virtual void concreteEvent(QEvent *) = 0;

        void noticeCleareImportClass();

        void CopyStr(char* str1, const char* str2, int sizemax);
        bool CmpStr(char* str1, const char* str2);

    protected:

};
/****************Class Declaration Section End*******************************************/
#endif // ABSTRACT_H

