#ifndef Frm_ApiClass_H
#define Frm_ApiClass_H
#include <QThread>
#include "hmiapi_abstract.h"
#include "singleton.h"


#define FrmApiClassIns   Frm_ApiClass::Instance()
#define UNUSED_PARAMETER(x) (void)x

class Frm_CommonClass;

class CPandaDbusEvent : public QEvent
{
private:

    enum EnumEventType
    {
        EV_REQUEST,
        EV_RESPONSE,
        EV_SIGNAL,
    };

    EnumEventType    m_type;
    /*tRequestHandler  m_request_handler;
    tResponseHandler m_response_handler;
    tSignalHandler   m_signal_handler;
    HStub            m_stub;
    HProxy           m_proxy;
    DBusMessage *    m_message;
    DBusMessage *    m_reply;*/

public:

    /*CPandaDbusEvent(tRequestHandler handler, HStub stub, DBusMessage *message, DBusMessage *reply)
        : QEvent(QEvent::Type(9999))
    {
        m_type = EV_REQUEST;
        m_request_handler = handler;
        m_stub = stub;
        m_message = message;
        m_reply = reply;
    }

    CPandaDbusEvent(tResponseHandler handler, HProxy proxy, DBusMessage *message, int state)
        : QEvent(QEvent::Type(9999))
    {
        UNUSED_PARAMETER(state);
        m_type = EV_RESPONSE;
        m_response_handler = handler;
        m_proxy = proxy;
        m_message = message;
    }

    CPandaDbusEvent(tSignalHandler handler, HProxy proxy, DBusMessage *message)
        : QEvent(QEvent::Type(9999))
    {
        m_type = EV_SIGNAL;
        m_signal_handler = handler;
        m_proxy = proxy;
        m_message = message;
    }
*/
    virtual ~CPandaDbusEvent()
    {
    }

    void relay()
    {
        switch(m_type)
        {
        case EV_REQUEST:
            //panda_dbus_relay_request(m_request_handler, m_stub, m_message, m_reply);
            break;
        case EV_RESPONSE:
            //panda_dbus_relay_response(m_response_handler, m_proxy, m_message);
            break;
        case EV_SIGNAL:
            //panda_dbus_relay_signal(m_signal_handler, m_proxy, m_message);
            break;
        }
    }
};

class Frm_ApiClass : public QThread
{
    Q_OBJECT
    //DECLARE_SINGLETON(Frm_ApiClass)
    private:
        //FrameWorkClass
        Frm_CommonClass* m_frameWorkClass;
    public:
        Frm_ApiClass(QObject* parent = 0);
        ~Frm_ApiClass();
        void init(QObject* frameWorkClass);
        void run();
/*
        void m_relay_request(tRequestHandler handler, HStub stub, DBusMessage *message, DBusMessage *reply);
        void m_relay_response(tResponseHandler handler, HProxy proxy, DBusMessage *message);
        void m_relay_signal(tSignalHandler handler, HProxy proxy, DBusMessage *message);
        */
    protected:

};

#endif // Frm_ApiClass_H

