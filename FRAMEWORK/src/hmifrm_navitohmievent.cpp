/*******************************************************
File Name   : Frm_ModeEvent

Created by  :  Shi.Jianhang(Neusoft)

Created Date:  2015/07/15

Description   :   event management(navi to hmi)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "hmifrm_navitohmievent.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_AbstractModeClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : Type type
para2         : quint16 modeid
para3         : void *para
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_NaviToHMIEvent::Frm_NaviToHMIEvent(Type type, quint32 navievid,void *para)
    : Frm_AbstractEvent(type)
{
    navievID = navievid;
    evtPara = para;
}

/******************************************************************************
Function Name : ~Frm_AbstractModeClass
Description   : Destructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_NaviToHMIEvent::~Frm_NaviToHMIEvent()
{
}

