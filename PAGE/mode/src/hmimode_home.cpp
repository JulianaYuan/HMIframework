#include "hmimode_home.h"
#include "hmiview_home.h"

const Frm_AbstractModeClass::S_VIEWTABLE viewTable[] =
{
    {"NULL",   1, 0, (void*)(View_Home::constructer), NULL },
};

Mode_Home::Mode_Home(QObject* Framework,QObject* parent, quint16 modeid, quint16 screenid,quint16 modeattr )
    : Frm_AbstractModeClass(Framework,parent,modeid,screenid, modeattr)
{
    ViewTable = (S_VIEWTABLE*)(&viewTable);
    ViewTableNum = sizeof(viewTable) / sizeof(S_VIEWTABLE);
}

Mode_Home::~Mode_Home(){}

bool Mode_Home::concreteEvent(QEvent *e)
{
    bool b_ret = false;
    Frm_AbstractEvent* pAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    if( NULL != pAbstractEvent )
    {
        switch( pAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
        {
            //b_ret = hmiEventProc(e);
            break;
        }
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
        {
            b_ret = hardKeyEventProc(e);
            break;
        }
        default:
            break;
        }
    }
    return b_ret;
}
/******************************************************************************
Function Name : consumeEvent
Description   : concrete the event
Date          : 2017/09/28
Parameter     :
para1         : QEvent *e
Author        : juanjuan(visteon)
******************************************************************************/
int Mode_Home::consumeEvent(QEvent *e)
{
    int i_ret =false;
    Frm_AbstractEvent* fAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    HMI_LOG("consumeEvent");
    if( NULL != fAbstractEvent )
    {
        switch( fAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
            //HMI_LOG("event ID %d",fAbstractEvent->getFuncID());
            break;
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
            //HMI_LOG("event ID %d",fAbstractEvent->getFuncID());
            break;
        default:
            break;
        }
    }
    return i_ret;
}
bool Mode_Home::hardKeyEventProc(QEvent *e){
    Frm_KeyEvent* pHKEvent = dynamic_cast<Frm_KeyEvent*>(e);

    bool b_ret = true;
    if( NULL != pHKEvent )
    {
        switch(pHKEvent->getFuncID())
        {
        case KEYEVENT_HOME_RELEASE:
            b_ret = true;
            break;

        default:
            b_ret = false;
            break;
        }
    }
    else
    {
        b_ret = false;
    }
    return b_ret;
}

void Mode_Home::concreteVisible(){HMI_LOG("IN");}
void Mode_Home::concreteInvisible(){HMI_LOG("IN");}
void Mode_Home::concreteStart(){HMI_LOG("IN");}
void Mode_Home::concreteEnd(){HMI_LOG("IN");}

