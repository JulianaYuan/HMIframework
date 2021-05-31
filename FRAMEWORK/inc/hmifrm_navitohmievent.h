#ifndef FNAVITOHMIEVENT_H
#define FNAVITOHMIEVENT_H

#include "../../FRAMEWORK/inc/hmifrm_abstractevent.h"

class Frm_NaviToHMIEvent : public Frm_AbstractEvent
{
    private:
        quint32 navievID;

        void *evtPara;

    public:

        Frm_NaviToHMIEvent(Type type = QEvent::None, quint32 navievid = 0, void *para = NULL );

        ~Frm_NaviToHMIEvent();

        inline quint32 getNaviEventID(){return navievID;}
        inline void* getNaviEventPara(){return evtPara;}

};

#endif // FNAVITOHMIEVENT_H
