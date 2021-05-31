/*******************************************************
 File Name   : hmiapi_devices.h

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/08/26
l
Description   :   devices Hmiapi
*********************************************************/

#ifndef HMIAPI_AUDIOMANAGER
#define HMIAPI_AUDIOMANAGER

/****************Include  Section Begin***************************************************/
//#include "gen_type_sx5_audiomanager.h"
//#include "gen_proxy_sx5_audiomanager.h"
#include "singleton.h"
#include "hmiapi_abstract.h"
#include "hmiapi_modemanager.h"
#ifndef HMI_API_STUB
//#include "panda_dbus.h"
//#include "panda_dbus_private.h"
#endif //HMI_API_STUB
/****************Include  Section End*****************************************************/

/****************Marco Definition Section Begin*******************************************/
#define AudioManagerClassIns   AudioManagerClass::Instance()

#define D_HMIAPI_EV_AM_UNMUTE_BY_TOUCH              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 1)
#define D_HMIAPI_EV_AM_VOLUME_CHANGED               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 2)
#define D_HMIAPI_EV_AM_DIAGONS_CHANGED              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 3)
#define D_HMIAPI_EV_AM_SCREEN_UNMUTE                M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 4)
#define D_HMIAPI_EV_AM_CARPLAYVOLUME_CHANGED        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 5)
#define D_HMIAPI_EV_AM_KEYMUTESTATUS_CHANGED        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 6)
//ren add start 0519
#define D_HMIAPI_EV_AM_SONNDVEHICLOFF               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 7)
#define D_HMIAPI_EV_AM_SONNDVEHICLON                M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_AUDIOMANAGER, 8)
//ren add end 0519
/****************Marco Definition Section End*********************************************/

#ifndef HMI_API_STUB
extern HProxy PROXY_AUDIOMANAGER;
#endif //HMI_API_STUB

typedef int tInterfaceClient_sx5_audiomanager;
typedef long HProxy;
/****************Struct Definition Section Start******************************************/

/****************Struct Definition Section End*********************************************/

/****************Class Declaration Section Begin*******************************************/
class AudioManagerClass : public AbstractAPIClass
{
    Q_OBJECT
#if 0
    DECLARE_SINGLETON(AudioManagerClass)
#else
protected: \
        AudioManagerClass();\
        static  AudioManagerClass* m_pInstance; \
public:  \
        static  AudioManagerClass* Instance();
#endif
public:

    ~AudioManagerClass();

    void sendEvent(int eventID);
    void concreteEvent(QEvent *);
    void AudioManager_Init(QObject* parent = 0);

    void Ope_setVolumeValue(int val);
    void Ope_setPhoneVolumeValue(int val);
    void Ope_setNaviVolumeValue(int val);
    void Ope_setCarplayVolumeValue(int val);
    int Get_VolumeValue();
    int Get_PhoneVolumeValue();
    int Get_naivVolumeValue();
    int Get_CarPlayVolumeValue();
    //enum EAMKMuteStatus Get_keymutestatus();

    void Ope_VolumeUp();
    void Ope_VolumeDown();
    void Ope_VolumeUp_Mix();
    void Ope_VolumeDown_Mix();
    void Ope_VolumeUp_CarplayAlt();
    void Ope_VolumeDown_CarplayAlt();
    void Ope_Mute();
    void Ope_UnMute();
    //void Ope_AudioActive(enum EAMFocusID focusID, enum EAMActiveReq Req);

    void updateVolumeValue(int val);
    void set_PhoneVolumeValue(int val);
    void set_naivVolumeValue(int val);
    void set_CarPlayVolumeValue(int val);
    //void set_KeyMuteStatus(enum EAMKMuteStatus status);
    int  Get_muteflag();
    //added by Frank Wang on March 18th
    void diagnostic_show_fader(int fader);
    void diagnostic_show_Balance(int balance);
    void diagnostic_show_Treble(int treble);
    void diagnostic_show_Mid(int mid);
    void diagnostic_show_Bass(int bass);
    void screenUnMute();
    void initvolflag();
	//ren add start 0518
    void Ope_VolumeUp_SetSoundVehiclestatus();
    void Ope_VolumeDown_SetSoundVehiclestatus();
	//ren add start 0518
    void VRvolumeExit(int exit);//CM7C-3392
    void Ope_reqSoftMute();//wangtingting add on 0403
signals:


public slots:


private:
#ifdef HMI_API_STUB
    HProxy PROXY_AUDIOMANAGER;
#else //HMI_API_STUB
    struct tRemoteComponent proxy_AUDIOMANAGER;
#endif //HMI_API_STUB

    Frm_CommonClass* frameWorkClass;
};
/****************Class Declaration Section End*********************************************/

/****************Function Prototype Declaration Section Begin******************************/
void hmi_signal_VolumeStep(HProxy proxy, unsigned int step);
void hmi_signal_PhoneVolumeStep(HProxy proxy, unsigned int step);
void hmi_signal_NaviVolumeStep(HProxy proxy, unsigned int step);
void hmi_signal_CarplayAltVolumeStep(HProxy proxy, unsigned int step);
//void hmi_signal_KeyMuteStatus(HProxy proxy, enum EAMKMuteStatus status);


//added by Frank Wang on March 18th
void hmi_signal_FaderStep(HProxy proxy, char fader);
void hmi_signal_BalanceStep(HProxy proxy, char balance);
void hmi_signal_TrebleStep(HProxy proxy, char treble);
void hmi_signal_MidStep(HProxy proxy, char mid);
void hmi_signal_BassStep(HProxy proxy, char bass);


/****************Function Prototype Declaration Section End********************************/



#endif // HMIAPI_AUDIOMANAGER

