/*******************************************************
 File Name   : hmifrm_abstractevent.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   event management
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_abstractevent.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_AbstractEvent
Description   : constructor
Date          : 2015/07/15
Parameter     : Type type
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractEvent::Frm_AbstractEvent(Type type): QEvent(type)
{
    switch((EventCode)type)
    {
        case EN_EV_MODETRANS_ON:
            evType = EN_TYPE_MODETRANS;
            break;

        case EN_EV_REQTRANS:
            evType = EN_TYPE_REQTRANS;
            break;
        case EN_EV_REQFUNC:
            evType = EN_TYPE_REQFUNC;
            break;
        case EN_EV_APPTOHMI:
            evType = EN_TYPE_APPTOHMI;
            break;
        case EN_EV_KEYEVENT:
            evType = EN_TYPE_KEYEVENT;
            break;
        case EN_EV_AUDIOMODE:
            evType = EN_TYPE_AUDIOMODE;
            break;
        default:
            break;
    }
}
/******************************************************************************
Function Name : ~Frm_AbstractEvent
Description   : Destructor
Date          : 2015/07/15
Parameter     :
para3         : quint16 attr
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractEvent::~Frm_AbstractEvent()
{
}
