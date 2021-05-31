/*******************************************************
 File Name   : hmiapi_devices.h

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/08/26
l
Description   :   devices Hmiapi
*********************************************************/

/****************Include  Section Begin************************************************/
#include <cstring>
#include <stdlib.h>
#include "hmiapi_audiomanager.h"
#include "hmifrm_funcevent.h"
/****************Include  Section  End*************************************************/
#ifndef HMI_API_STUB
HProxy PROXY_AUDIOMANAGER;
#endif //HMI_API_STUB
#define UNUSED_PARAMETER(x) (void)x

IMPLEMENT_SINGLETON(AudioManagerClass)

AudioManagerClass::AudioManagerClass()
{
}

AudioManagerClass::~AudioManagerClass()
{

}

void init_client_sx5_audiomanager(tInterfaceClient_sx5_audiomanager *client)
{/*
    // init the callbacks
    client->on_signal_VolumeStep = hmi_signal_VolumeStep;
    client->on_signal_PhoneVolumeStep = hmi_signal_PhoneVolumeStep;
    client->on_signal_NaviVolumeStep = hmi_signal_NaviVolumeStep;
    client->on_signal_CarplayAltVolumeStep = hmi_signal_CarplayAltVolumeStep;
    client->on_signal_FaderStep = hmi_signal_FaderStep;
    client->on_signal_BalanceStep = hmi_signal_BalanceStep;
    client->on_signal_TrebleStep = hmi_signal_TrebleStep;
    client->on_signal_MidStep = hmi_signal_MidStep;
    client->on_signal_BassStep = hmi_signal_BassStep;
    client->on_signal_KeyMuteStatus = hmi_signal_KeyMuteStatus;
    //...
    */
}

void AudioManagerClass::AudioManager_Init(QObject *parent)
{
    if(NULL == parent)
    {
        HMI_WARNING("NULL == parent");
    }

    frameWorkClass = dynamic_cast<Frm_CommonClass*>(parent);

#ifdef HMI_API_STUB
    tInterfaceClient_sx5_audiomanager* pclient;
   // pclient = sx5_audiomanager_init();
   // init_client_sx5_audiomanager(pclient);
#else //HMI_API_STUB
    proxy_AUDIOMANAGER.connection = mBusConnection;
    proxy_AUDIOMANAGER.path = "/sx5/audiomanager";		//audiomanager path
    proxy_AUDIOMANAGER.service = "sx5.audiomanager";    // audiomanager name
    PROXY_AUDIOMANAGER = &proxy_AUDIOMANAGER;

    // init the entertainment manager dbus API proxy
    init_client_sx5_audiomanager(&INTERFACE_CLIENT_sx5_audiomanager);
#endif //HMI_API_STUB
    return;
}

void AudioManagerClass::concreteEvent(QEvent *e)
{
    //appClass->customEvent(e);
    if (NULL == e)
    {
        HMI_WARNING("NULL == e");
    }

}

void AudioManagerClass::sendEvent(int eventID)
{
    Frm_FuncEvent* FuncEvent = new Frm_FuncEvent((static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_APPTOHMI)), eventID);

    QGuiApplication::postEvent(frameWorkClass, static_cast<QEvent*> (FuncEvent));

    return;
}

//CM7C-3392 add
void AudioManagerClass::VRvolumeExit(int exit)
{

}
void AudioManagerClass::Ope_setVolumeValue(int val)
{

//CM7C-3392 end
}

void AudioManagerClass::Ope_setPhoneVolumeValue(int val)
{
    HMI_LOG("val = %d", val);

}

void AudioManagerClass::Ope_setNaviVolumeValue(int val)
{
    HMI_LOG("val = %d", val);

}

void AudioManagerClass::Ope_setCarplayVolumeValue(int val)
{
    HMI_LOG("val = %d", val);

}

void AudioManagerClass::Ope_VolumeUp()
{

}

void AudioManagerClass::Ope_VolumeDown()
{

}

void AudioManagerClass::Ope_VolumeUp_Mix()
{
}

void AudioManagerClass::Ope_VolumeDown_Mix()
{

}

void AudioManagerClass::Ope_VolumeUp_CarplayAlt()
{
    HMI_LOG("ENTER");
}

void AudioManagerClass::Ope_VolumeDown_CarplayAlt()
{
    HMI_LOG("ENTER");
}

void AudioManagerClass::Ope_Mute()
{

}

void AudioManagerClass::Ope_UnMute()
{

}

int AudioManagerClass::Get_muteflag()
{
return 0;
}

int AudioManagerClass::Get_VolumeValue()
{
return 0;
}
void AudioManagerClass::initvolflag()
{
    return;
}

void AudioManagerClass::updateVolumeValue(int val)
{

}

void AudioManagerClass::set_PhoneVolumeValue(int val)
{

}

int AudioManagerClass::Get_PhoneVolumeValue()
{
return 0;
}

void AudioManagerClass::set_naivVolumeValue(int val)
{

}
int AudioManagerClass::Get_naivVolumeValue()
{
return 0;
}

void AudioManagerClass::set_CarPlayVolumeValue(int val)
{

}
/*
void AudioManagerClass::set_KeyMuteStatus(enum EAMKMuteStatus status)
{

}
*/
int AudioManagerClass::Get_CarPlayVolumeValue()
{
return 0;
}

void hmi_signal_VolumeStep(HProxy proxy, unsigned int step)
{

    if (NULL == proxy)
    {
        HMI_LOG("proxy == NULL");
    }

    HMI_LOG("step= %d ", step);

    AudioManagerClassIns->updateVolumeValue(step);
    AudioManagerClassIns->sendEvent(D_HMIAPI_EV_AM_VOLUME_CHANGED);

    return;
}

void hmi_signal_PhoneVolumeStep(HProxy proxy, unsigned int step)
{
    //HMI_TRACE(" proxy = %d, step = %d", proxy,step);
    UNUSED_PARAMETER(proxy);
    AudioManagerClassIns->set_PhoneVolumeValue(step);
}

void hmi_signal_NaviVolumeStep(HProxy proxy, unsigned int step)
{
    //HMI_TRACE(" proxy = %d, step = %d", proxy,step);
    UNUSED_PARAMETER(proxy);
    AudioManagerClassIns->set_naivVolumeValue(step);
}

void hmi_signal_CarplayAltVolumeStep(HProxy proxy, unsigned int step)
{
    //HMI_LOG(" proxy = %d, step = %d", proxy,step);
    UNUSED_PARAMETER(proxy);
    AudioManagerClassIns->set_CarPlayVolumeValue(step);
}
/*
void hmi_signal_KeyMuteStatus(HProxy proxy, enum EAMKMuteStatus status)
{
//    HMI_LOG("proxy = %d,stauts = %d", proxy, status);
    UNUSED_PARAMETER(proxy);
    AudioManagerClassIns->set_KeyMuteStatus(status);
    return;
}
*/

//added by Frank Wang on March 18th
void hmi_signal_FaderStep(HProxy proxy, char fader)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    AudioManagerClassIns->diagnostic_show_fader((int)fader);
}

void hmi_signal_BalanceStep(HProxy proxy, char balance)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    AudioManagerClassIns->diagnostic_show_Balance((int)balance);
}

void hmi_signal_TrebleStep(HProxy proxy, char treble)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    AudioManagerClassIns->diagnostic_show_Treble((int)treble);
}

void hmi_signal_MidStep(HProxy proxy, char mid)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    AudioManagerClassIns->diagnostic_show_Mid((int)mid);
}

void hmi_signal_BassStep(HProxy proxy, char bass)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    AudioManagerClassIns->diagnostic_show_Mid((int)bass);
}

void AudioManagerClass::diagnostic_show_fader(int fader)
{
    HMI_LOG("fader = %d", fader);

    if( HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status() )
    {
        QString str_temp = "";

        str_temp = "Fader:"+QString::number(fader);

        sendEvent(D_HMIAPI_EV_AM_DIAGONS_CHANGED);
    }
}

void AudioManagerClass::diagnostic_show_Balance(int balance)
{
    HMI_LOG("balance = %d", balance);

    if( HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status() )
    {
        QString str_temp = "";

        str_temp = "Balance:"+QString::number(balance);

        sendEvent(D_HMIAPI_EV_AM_DIAGONS_CHANGED);
    }
}

void AudioManagerClass::diagnostic_show_Treble(int treble)
{
    HMI_LOG("treble = %d", treble);

    if( HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status() )
    {
        QString str_temp = "";

        str_temp = "Treble:"+QString::number(treble);

        sendEvent(D_HMIAPI_EV_AM_DIAGONS_CHANGED);
    }
}

void AudioManagerClass::diagnostic_show_Mid(int mid)
{
    HMI_LOG("mid = %d", mid);

    if( HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status() )
    {
        QString str_temp = "";

        str_temp = "Mid:"+QString::number(mid);

        sendEvent(D_HMIAPI_EV_AM_DIAGONS_CHANGED);
    }
}

void AudioManagerClass::diagnostic_show_Bass(int bass)
{
    HMI_LOG("bass = %d", bass);

    if( HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status() )
    {
        QString str_temp = "";

        str_temp = "Bass:"+QString::number(bass);

        sendEvent(D_HMIAPI_EV_AM_DIAGONS_CHANGED);
    }
}

void AudioManagerClass::screenUnMute()
{
    sendEvent(D_HMIAPI_EV_AM_SCREEN_UNMUTE);

    return;
}

//ren add start 0518
void AudioManagerClass::Ope_VolumeUp_SetSoundVehiclestatus()
{
    HMI_LOG("ENTER");
    sendEvent(D_HMIAPI_EV_AM_SONNDVEHICLON);
}

void AudioManagerClass::Ope_VolumeDown_SetSoundVehiclestatus()
{
    HMI_LOG("ENTER");
    sendEvent(D_HMIAPI_EV_AM_SONNDVEHICLOFF);
}
//ren add start 0518

//wangtingting add on 0413
void AudioManagerClass::Ope_reqSoftMute()
{
    HMI_LOG("Enter");
}
