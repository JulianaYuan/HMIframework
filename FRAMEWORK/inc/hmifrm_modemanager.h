/*******************************************************
 File Name   : hmifrm_modemanager.h

 Created by  :  Mao.Tianliang(Neusoft)

 Created Date:  2015/08/03

Description   :   interacts with Modemanager, pairing between viewmode and audiomode
*********************************************************/
#ifndef HMIFRM_MODEMANAGER_H
#define HMIFRM_MODEMANAGER_H

#include <QObject>
#include "../inc/hmifrm_abstractevent.h"


typedef enum tagSourceChangeStepEnum
{
    EN_MODEMANAGER_STEP_IDLE,
    EN_MODEMANAGER_STEP_NORMAL_TRANS_REQ,
    EN_MODEMANAGER_STEP_LAST_MODE_REQ,
    EN_MODEMANAGER_STEP_EJECT_TRANS_REQ,
    EN_MODEMANAGER_STEP_INIT,
    EN_MODEMANAGER_STEP_VIDEO_FORBID,
    EN_MODEMANAGER_STEP_VIDEO_FORBID_TANS_REQ,
    EN_MODEMANAGER_STEP_MODEOFF_TRANS_REQ,

    EN_MODEMANAGER_STEP_MAX
}SourceChangeStepEnum;

typedef enum tagDeviceEjectEnum
{
    EN_MODEMANAGER_EJECT_USB,
    EN_MODEMANAGER_EJECT_IPOD,
    EN_MODEMANAGER_EJECT_BTA,
    EN_MODEMANAGER_EJECT_AUX,
    EN_MODEMANAGER_EJECT_LINK,

    EN_MODEMANAGER_EJECT_MAX,
    EN_MODEMANAGER_EJECT_NONE

}DeviceEjectEnum;

class Frm_modemanager : public QObject
{
    Q_OBJECT
public:
    explicit Frm_modemanager(QObject *parent = 0);

    bool isReqCurrentMode(QEvent *p_event);
    void reqModeTrans(QEvent *p_event);
    void customEvProc(QEvent *p_event);
    void backFromNaviMode();

    void Init_AudioSource();
    void USB_Eject();
    void IPOD_Eject();
    void BTA_Eject();
    void AUX_Eject();
    void LINK_Eject();
    //void back_to_previous_mode_from_Movie(int transtype);
    void timerEvent(QTimerEvent* event);
    void USB_TransToPrevSource();

private:
    Frm_AbstractEvent m_ModeTransEvent;
    int i_reqViewmode;
    int i_reqSource;
    int i_reqScreenid;
    int i_curMode;
    int i_ViewModeOfSource;
    bool is_View_change;
    int m_starttimer;
    int m_homeStartTimer; //wait 3 seconds, untill no other interrupt mode need show, then show home, or do nothing, because the interrupt mode will default use home as its base mode.
    int m_sourceStartTimer;
    int m_screentype;

    SourceChangeStepEnum en_SourceStep;
    DeviceEjectEnum en_DeviceEject;
    int i_movieforbid_transtype;

    void pairAudioMode2ViewMode(int *i_curViewMode, int i_curMode);
    void pairSourceMode2ViewMode(int *i_curViewMode, int i_curMode);
    void pairViewMode2AudioMode(int *pi_Mode, int i_ViewMode);

    bool is_Source_available(int mode);
    void lastMode_getted();
    void ActiveBaseMode_getted();
    void FocusedMode_getted();
    void CycleNextSource(int i_curSource);
};

#endif // HMIFRM_MODEMANAGER_H
