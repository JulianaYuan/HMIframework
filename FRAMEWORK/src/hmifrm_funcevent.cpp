/*******************************************************
 File Name   : hmifrm_funcevent.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   event management(function)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "hmifrm_funcevent.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_AbstractEvent
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : Type type
para2         : quint16 funcid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_FuncEvent::Frm_FuncEvent(Type type, quint16 funcid): Frm_AbstractEvent(type)
{
    funcID = funcid;
}


/******************************************************************************
Function Name : ~Frm_AbstractEvent
Description   : Destructor
Date          : 2015/07/15
Parameter     : Type type
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_FuncEvent::~Frm_FuncEvent()
{
}


