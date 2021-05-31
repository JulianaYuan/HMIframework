/*******************************************************
 File Name   : hmifrm_keyevent.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   event management(key)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "hmifrm_keyevent.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_KeyEvent
Description   : constructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_KeyEvent::Frm_KeyEvent(Type type, KeyEvent eventID): Frm_AbstractEvent(type)
{
    keyEventID = eventID;
}

/******************************************************************************
Function Name : Frm_KeyEvent
Description   : Destructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_KeyEvent::~Frm_KeyEvent()
{
}

