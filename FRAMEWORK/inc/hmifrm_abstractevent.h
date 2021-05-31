#ifndef Frm_AbstractEvent_H
#define Frm_AbstractEvent_H

#include <QEvent>

enum ModeReqType
{
    EN_MODE_OFF = 0,
    EN_MODE_ON_INTERRUPT,
    EN_MODE_ON_BASE,
    EN_MODE_ON_REPLACE,//juanjuan add
    EN_MODE_OFF_NOSOURCE_CHANGE,
    EN_MODE_NONE
};

class Frm_AbstractEvent : public QEvent
{
    private:

        int evType;

    public:

        enum EventCode
        {
            EN_EV_MODETRANS_ON =
            QEvent::User,//moshiqianyi
            EN_EV_REQTRANS,//huamianqianyi
            EN_EV_REQFUNC,//gongnengxing
            EN_EV_APPTOHMI,//API xiaoqi
            EN_EV_KEYEVENT,//key
            EN_EV_AUDIOMODE,//Call event from Modemanage core

            EN_MAXEVENTNUM
        };

        enum EventType
        {
            EN_TYPE_MODETRANS = 0,
            EN_TYPE_REQTRANS,
            EN_TYPE_REQFUNC,
            EN_TYPE_APPTOHMI,
            EN_TYPE_KEYEVENT,
            EN_TYPE_AUDIOMODE,

            EN_MAXEVENTTYPENUM
        };

        Frm_AbstractEvent(Type type = QEvent::None);
        ~Frm_AbstractEvent();

        int getEventType(){return evType;}

};

#endif // FABSTRACTEVENT_H

