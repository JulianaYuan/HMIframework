#ifndef Frm_FuncEvent_H
#define Frm_FuncEvent_H

#include "hmifrm_abstractevent.h"

class Frm_FuncEvent : public Frm_AbstractEvent
{
    private:
        quint16 funcID;

    public:

        Frm_FuncEvent(Type type = QEvent::None, quint16 funcid = 0);

        ~Frm_FuncEvent();

        inline quint16 getFuncID(){return funcID;}

};

#endif // Frm_FuncEvent_H
