/*******************************************************
 File Name   : hmifrm_apiclass.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   api management
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_apiclass.h"
#include "hmiapi_modemanager.h"
#include "hmifrm_viewclass.h"
#ifndef HMI_API_STUB
#endif //HMI_API_STUB
/****************Include  Section End***************************************************/

static Frm_ApiClass *m_instance = NULL;

/******************************************************************************
Function Name : Frm_ApiClass
Description   : constructor
Date          : 2015/07/15
Parameter     : QObject* parent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_ApiClass::Frm_ApiClass(QObject* parent)
{
    if( NULL == parent )
    {
    }

    m_instance = this;

    if (m_instance == NULL)
    {
        HMI_ERROR("Get API Class Instance Failed");
    }
}

/******************************************************************************
Function Name : ~Frm_ApiClass
Description   : Destructor
Date          : 2015/07/15
Parameter     : QObject* parent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_ApiClass::~Frm_ApiClass()
{
}

#ifndef HMI_API_STUB
static void bus_signals(HProxy proxy, DBusMessage *message)
{
    if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_entertainment))
    {
        HMI_LOG("bus_signals sx5_entertainment_signals");
        sx5_entertainment_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_modemanager))
    {
        HMI_LOG("bus_signals sx5_modemanager_signals");
        sx5_modemanager_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_settings))
    {
        HMI_LOG("bus_signals sx5_settings_signals");
        sx5_settings_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_cmu_swdl))
    {
        HMI_LOG("bus_signals cmu_swdl_signals");
        cmu_swdl_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_devices))
    {
        HMI_LOG("bus_signals sx5_devices_signals");
        sx5_devices_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_audiomanager))
    {
        HMI_LOG("bus_signals sx5_devices_signals");
        sx5_audiomanager_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_phone))
    {
        HMI_LOG("bus_signals sx5_phone_signals");
        sx5_phone_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_vr))
    {
        HMI_LOG("bus_signals sx5_vr_signals");
        sx5_vr_signals(proxy, message);
    }
    else if (dbus_message_has_interface(message, INTERFACE_NAME_cmu_phonebook))
    {
        HMI_LOG("bus_signals cmu_phonebook_signals");
        cmu_phonebook_signals(proxy, message);
    }
    #if 0
	else if (dbus_message_has_interface(message, INTERFACE_NAME_cmu_resourcemanager))
    {
        HMI_LOG("bus_signals sx5_vr_signals");
        cmu_resourcemanager_signals(proxy, message);
    }
#endif
    else if (dbus_message_has_interface(message, INTERFACE_NAME_cmu_usbsync))
    {
        HMI_LOG("bus_signals cmu_usbsync_signals ");
        cmu_usbsync_signals(proxy, message);
    }
#if 0
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_diagnostic))
    {
        HMI_LOG("bus_signals sx5_diagnostic_signals ");
        sx5_diagnostic_signals(proxy, message);
    }
#endif
#if 0
    /*[add] [by] [neusoft] [luzhj] [2017/4/13] [START]*/
    else if (dbus_message_has_interface(message, INTERFACE_NAME_sx5_tbox))
    {
        HMI_LOG("bus_signals sx5_tbox_signals ");
        sx5_tbox_signals(proxy, message);
    }
    /*[add] [by] [neusoft] [luzhj] [2017/4/13] [END]*/
#endif
    else if(dbus_message_has_interface(message, INTERFACE_NAME_cmu_welink)) //1219
    {
         HMI_LOG("bus_signals cmu_welink_signals ");
         cmu_welink_signals(proxy,message);
    }
    else
    {
        HMI_LOG("bus_signals not has interface");
    }
}

static DBusHandlerResult bus_filter(DBusConnection *conn, DBusMessage *message, void *user_data)
{
    if ((NULL == conn) || (NULL == message) || (NULL == user_data))
    {
        //HMI_LOG("(NULL == conn) || (NULL == message) || (NULL == user_data)");
    }

    if (dbus_message_get_type(message) != DBUS_MESSAGE_TYPE_SIGNAL)
    {
        HMI_LOG("bus_filter DBUS_HANDLER_RESULT_NOT_YET_HANDLED");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    if (dbus_message_has_path(message, PROXY_ENTERTAINMENT->path))
    {
        HMI_LOG("bus_filter PROXY_ENTERTAINMENT");
        bus_signals(PROXY_ENTERTAINMENT, message);
    }
    else if (dbus_message_has_path(message, PROXY_MODE->path))
    {
        HMI_LOG("bus_filter PROXY_MODEMANAGER");
        bus_signals(PROXY_MODE, message);
    }
    else if (dbus_message_has_path(message, PROXY_SETTINGS->path))
    {
        HMI_LOG("bus_filter PROXY_SETTINGS");
        bus_signals(PROXY_SETTINGS, message);
    }
    else if (dbus_message_has_path(message, PROXY_SWDL->path))
    {
        HMI_LOG("bus_filter PROXY_SWDL");
        bus_signals(PROXY_SWDL, message);
    }
    else if (dbus_message_has_path(message, PROXY_DEVICES->path))
    {
        HMI_LOG("bus_filter PROXY_DEVICES");
        bus_signals(PROXY_DEVICES, message);
    }
    else if (dbus_message_has_path(message, PROXY_AUDIOMANAGER->path))
    {
        HMI_LOG("bus_filter PROXY_AUDIOMANAGER");
        bus_signals(PROXY_AUDIOMANAGER, message);
    }
    else if (dbus_message_has_path(message, PROXY_PHONE->path))
    {
        HMI_LOG("bus_filter PROXY_PHONE");
        bus_signals(PROXY_PHONE, message);
    }
    else if (dbus_message_has_path(message, PROXY_VR->path))
    {
        HMI_LOG("bus_filter PROXY_VR");
        bus_signals(PROXY_VR, message);
    }
    else if (dbus_message_has_path(message, PROXY_PHONEBOOK->path))
    {
        HMI_LOG("bus_filter PROXY_VR");
        bus_signals(PROXY_PHONEBOOK, message);
    }
#if 0
    else if (dbus_message_has_path(message, PROXY_RESOURCEMANAGER->path))
    {
        HMI_LOG("bus_filter PROXY_RESOURCEMANAGER");
        bus_signals(PROXY_RESOURCEMANAGER, message);
    }
#endif
    else if (dbus_message_has_path(message, PROXY_SYNC->path))
    {
        HMI_LOG("bus_filter PROXY_SYNC");
        bus_signals(PROXY_SYNC, message);
    }
#if 0
    else if (dbus_message_has_path(message, PROXY_DIAG->path))
    {
        HMI_LOG("bus_filter PROXY_DIAG");
        bus_signals(PROXY_DIAG, message);
    }
#endif
#if 0
    else if (dbus_message_has_path(message, PROXY_TBOX->path))
    {
        HMI_LOG("bus_filter PROXY_TBOX");
        bus_signals(PROXY_TBOX, message);
    }
#endif
    else if(dbus_message_has_path(message, PROXY_WELINK->path)) //1219
    {
        HMI_LOG("bus_filter PROXY_WELINK");
        bus_signals(PROXY_WELINK, message);
    }
    else
    {
        //HMI_LOG("bus_filter not has the path");
    }

    return DBUS_HANDLER_RESULT_HANDLED;
}


void main_event_handler(tEvent *ev)
{
    HMI_LOG("main_event_handler called");
    if (NULL == ev)
    {
        HMI_LOG(" ERROR NULL == ev");
    }
}

void Frm_ApiClass::m_relay_request(tRequestHandler handler, HStub stub, DBusMessage *message, DBusMessage *reply)
{
    CPandaDbusEvent *ev = new CPandaDbusEvent(handler, stub, message, reply);
    QGuiApplication::postEvent(m_frameWorkClass, ev);
}

void Frm_ApiClass::m_relay_response(tResponseHandler handler, HProxy proxy, DBusMessage *message)
{
    CPandaDbusEvent *ev = new CPandaDbusEvent(handler, proxy, message, 0);
    QGuiApplication::postEvent(m_frameWorkClass, ev);
}

void Frm_ApiClass::m_relay_signal(tSignalHandler handler, HProxy proxy, DBusMessage *message)
{
    CPandaDbusEvent *ev = new CPandaDbusEvent(handler, proxy, message);
    QGuiApplication::postEvent(m_frameWorkClass, ev);
}

static void on_relay_request(tRequestHandler handler, HStub stub, DBusMessage *message, DBusMessage *reply)
{
    if (NULL != m_instance)
    {
        HMI_LOG("Receive Dbus Request");
        m_instance->m_relay_request(handler, stub, message, reply);
    }
    else
    {
        HMI_ERROR("API Class Instance is NULL");
    }
}

static void on_relay_response(tResponseHandler handler, HProxy proxy, DBusMessage *message)
{
    if (NULL != m_instance)
    {
        HMI_LOG("Receive Dbus Response");
        m_instance->m_relay_response(handler, proxy, message);
    }
    else
    {
        HMI_ERROR("API Class Instance is NULL");
    }
}

static void on_relay_signal(tSignalHandler handler, HProxy proxy, DBusMessage *message)
{
    if (NULL != m_instance)
    {
        HMI_LOG("Receive Dbus Signal");
        m_instance->m_relay_signal(handler, proxy, message);
    }
    else
    {
        HMI_ERROR("API Class Instance is NULL");
    }
}

void main_start()
{
    HMI_LOG("main_start");
	panda_dbus_set_relay(on_relay_request, on_relay_response, on_relay_signal);
    SettingsClassIns->Req_getMileSigLoseRemindInter();
    SettingsClassIns->Req_getMaintnanceRemindType();
    DevicesClassIns->Req_GetUsbList();
    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_USB);
//    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_MTP);
    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_IPOD);
    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_CARPLAY);
    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_MIRACAST);
    DevicesClassIns->Req_GetDeviceListData(DEVICE_TYPE_MIRRORLINK);
    DevicesClassIns->Req_GetUsbSyncInfo();
    SettingsClassIns->Req_getEQValue();
    SettingsClassIns->Req_getBrightness();
    SettingsClassIns->Req_getVideoPlayTime(); //wangtingtnig add
    SettingsClassIns->Req_getCanRTStatus(); //wangtingting add for SX5M-839
    ModemanagerClassIns->Req_ModeObtain();
	SettingsClassIns->Req_getRadarStatus(); //wangtingting add for SX5M-1333
    WeLinkClassIns->set_weLinkOnOff(false);
    HMI_LOG("main_end");
}
#endif //HMI_API_STUB

/******************************************************************************
Function Name : init
Description   : init api class
Date          : 2015/07/15
Parameter     : QObject* parent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_ApiClass::init(QObject* frameWorkClass)
{
    HMI_LOG("Frm_ApiClass::init START");
#ifndef HMI_API_STUB
    panda_bus_init();		//dbus init
    panda_bus_connect();	// dbus connect
#endif //HMI_API_STUB

    m_frameWorkClass = dynamic_cast<Frm_CommonClass*>(frameWorkClass);

    ModemanagerClassIns->Modemanager_Init(frameWorkClass);
    AudioManagerClassIns->AudioManager_Init(frameWorkClass);

#ifdef HMI_API_STUB
    ModemanagerClassIns->Req_ModeObtain();
#endif //HMI_API_STUB
    HMI_LOG("Frm_ApiClass::init END");

    return;
}

/******************************************************************************
Function Name : run
Description   : Dbus start
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_ApiClass::run()
{
    HMI_LOG("FAppClass::run()");
#ifndef HMI_API_STUB
    panda_bus_work(main_start, main_event_handler); //start the loop, it will call main_start() first
#endif //HMI_API_STUB
    return;
}

