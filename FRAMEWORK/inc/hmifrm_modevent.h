#ifndef FMODEEVENT_H
#define FMODEEVENT_H

#include "hmifrm_abstractevent.h"

#define MODE_ON_PERCENT_0 (1)
#define MODE_ON_PERCENT_100 (2)

class Frm_ModeEvent : public Frm_AbstractEvent
{
    private:
        quint16 modeID;
        quint16 screenID;
        ModeReqType reqType;
    public:

        Frm_ModeEvent(Type type = QEvent::None, quint16 modeid = 0, quint16 screenid = 0, ModeReqType reqtype = EN_MODE_NONE);
        ~Frm_ModeEvent();
        quint16 getModeID(){return modeID;}
        inline quint16 getScreenID(){return screenID;}
        inline ModeReqType getReqType(){return reqType;}
};

#endif // FMODEEVENT_H

