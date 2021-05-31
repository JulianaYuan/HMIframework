#ifndef Frm_TransEvent_H
#define Frm_TransEvent_H

#include "hmifrm_abstractevent.h"

#define TRANS_PATTERN_NORMAL 0
#define TRANS_PATTERN_RETURN 1
#define TRANS_PATTERN_CHANGE 2

class Frm_TransEvent : public Frm_AbstractEvent
{
    private:
        quint16 m_modeID; //wangtingting add on 0509
        quint16 m_screenID;
        quint16 m_pattern;

    public:

        Frm_TransEvent(Type type = QEvent::None,quint16 modeid = 0,quint16 screenid = 0, quint16 pattern = TRANS_PATTERN_NORMAL);
        ~Frm_TransEvent();

        quint16 getScreenID(){return m_screenID;};
        quint16 getPattern(){return m_pattern;};
        quint16 getModeID(){return m_modeID;};//wangtingting add on 0509
};

#endif // Frm_TransEvent_H

