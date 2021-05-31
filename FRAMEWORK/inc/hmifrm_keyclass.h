#ifndef Frm_KeyClass_H
#define Frm_KeyClass_H

#include <QTimerEvent>
#include "hmifrm_keyevent.h"
#include "hmifrm_common.h"

//this class used to handle key inputs. it may support status as below:
//1.a key long pressed which can devide to 3 different time. And we should config the time3>time2>time1
//2.multi-key pressed. The class is designed to support only 4 keys pressed at the same time

enum KEY_CODE{
    KEY_CODE_NONE = 0,
    KEY_CODE_ENCODER_LEFT   = 9,    // 9
    KEY_CODE_ENCODER_RIGHT,         //10
    KEY_CODE_ENCODER_ENTER,         //11

    KEY_CODE_VOL_DN         = 48,   //48
    KEY_CODE_VOL_UP,                //49
    KEY_CODE_EQ,                    //50
    KEY_CODE_BACK,                  //51
    KEY_CODE_SEEK_UP,               //52
    KEY_CODE_SEEK_DN,               //53
    KEY_CODE_SETUP,                 //54
    KEY_CODE_USB,                   //55
    KEY_CODE_MENU,                  //56
    KEY_CODE_NAVI,                  //57
    KEY_CODE_MUTE           = 65,   //65
    KEY_CODE_POWER,                 //66
    KEY_CODE_BAND           = 67,   //67
    KEY_CODE_DISP,                  //68
    KEY_CODE_PHONE,                 //69
    KEY_CODE_LINK,                  //70
    KEY_CODE_STW_ASR        = 74,   //74
    // Steering wheel
    KEY_CODE_STW_VOL_DN 	= 76,
	KEY_CODE_STW_MUTE,
    KEY_CODE_STW_PHONE,
	KEY_CODE_STW_SEEK_DN    = 80,
    KEY_CODE_STW_SEEK_UP,
    KEY_CODE_STW_VOL_UP,
	KEY_CODE_STW_MODE 		= 85,   //85



    KEY_CODE_MAX
};

enum KEY_STATUS{
    KEY_STATUS_RELEASE = 0,
    KEY_STATUS_PRESS,
    KEY_STATUS_OFFTIME,
    KEY_STATUS_ROTARY,

    KEY_STATUS_INVALID
};

typedef struct{
    KEY_CODE    e_key1;
    KEY_CODE    e_key2;
    KEY_CODE    e_key3;
    KEY_CODE    e_key4;
    bool    b_send_longrelease; //wether send long release event 0402

    KEY_STATUS  e_status;
    unsigned int  i_time1;     //ms
    unsigned int  i_time2;     //ms
    unsigned int  i_time3;     //ms

    KeyEvent    e_out_event1;
    KeyEvent    e_out_event2;
    KeyEvent    e_out_event3;
    KeyEvent    e_Press_event;
    KeyEvent    e_Release_event;
    KeyEvent    e_LongRelease_event;
}KEY_TABLE_TYPE;

class Frm_KeyClass : public QObject
{
    Q_OBJECT

public:
    Frm_KeyClass();
    static Frm_KeyClass* getInstance();
    static void DestroyObject();

    void init(QObject* parent = 0);

    void PostKeyStatus(KEY_CODE key_code, KEY_STATUS key_status);
    void ResetHardkeyTbl(KEY_CODE key_code, KEY_STATUS key_status);
    void inithardkey();
    void setkeycount(int count); //14277 by xubing
    void ignoreLongRelease(KEY_CODE key_code); //wangtingting add on 0402
protected:
    void timerEvent(QTimerEvent *);

private:
    //Frm_KeyClass();

    int findKeyLongPressTable(KEY_CODE key_code);
    int findKeyMulPressTable(KEY_CODE key_code);
    int findKeyPressTable(KEY_CODE key_code);
    int findKeyRotaryTable(KEY_CODE key_code);
    bool checkMultiKey(int index);
    void sendKeyEventToFw(KeyEvent event);

    static Frm_KeyClass* m_instance;

    //Frm_CommonClass
    Frm_CommonClass* frameWorkClass;

    KEY_CODE        key_slot[4];
    int             keytbl_count;
    unsigned int    key_time_cnt;
    int             keytbl_index;
    int             key_count;
    int             ui_timer_id;
    int             ui_rotary_timer_id;

signals:
    void sigPostKeyStatus(KEY_CODE key_code, KEY_STATUS key_status);

public slots:
    void onPostKeyStatus(KEY_CODE key_code, KEY_STATUS key_status);

};

#define Frm_KeyClassIns    Frm_KeyClass::getInstance()

#endif // Frm_KeyClass_H
