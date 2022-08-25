#include "aji.h"
#include "aji_00.h"
#include "data/mapdata.h"

#ifdef __MWERKS__
#define RELMODULE_EXPORT __declspec(export)
typedef void (*voidfunctionptr) (void);
__declspec(section ".init") extern voidfunctionptr _ctors[];
__declspec(section ".init") extern voidfunctionptr _dtors[];
#else
#define RELMODULE_EXPORT 
typedef void (*voidfunctionptr) (void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
#endif

extern s32 aaa_00_init_evt[];

//local prototypes
void _prolog(void);
void _epilog(void);
void _unresolved(void);

RELMODULE_EXPORT void _prolog(void) {
    voidfunctionptr* constructor;

    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }
    relSetEvtAddr("aji_00", aji_00_init_evt);
    /*
      code **ppcVar1;
  
    for (ppcVar1 = (code **)&DAT_80431cec; *ppcVar1 != (code *)0x0; ppcVar1 = ppcVar1 + 1) {
        (**ppcVar1)();
    }
    relSetEvtAddr("aji_00",&aji_00_init_evt);
    relSetEvtAddr("aji_01",&aji_01_init_evt);
    relSetEvtAddr("aji_02",&aji_02_init_evt);
    relSetEvtAddr("aji_03",&aji_03_init_evt);
    relSetEvtAddr("aji_04",&aji_04_init_evt);
    relSetEvtAddr("aji_05",&aji_05_init_evt);
    relSetEvtAddr("aji_06",&aji_06_init_evt);
    relSetEvtAddr("aji_07",&aji_07_init_evt);
    relSetEvtAddr("aji_08",&aji_08_init_evt);
    relSetEvtAddr("aji_09",&aji_09_init_evt);
    relSetEvtAddr("aji_10",&aji_10_init_evt);
    relSetEvtAddr("aji_11",&aji_11_init_evt);
    relSetEvtAddr("aji_12",&aji_12_init_evt);
    relSetEvtAddr("aji_13",&aji_13_init_evt);
    relSetEvtAddr("aji_14",&aji_14_init_evt);
    relSetEvtAddr("aji_15",&aji_15_init_evt);
    relSetEvtAddr("aji_16",&aji_16_init_evt);
    relSetEvtAddr("aji_17",&aji_17_init_evt);
    relSetEvtAddr("aji_18",&aji_18_init_evt);
    relSetEvtAddr("aji_19",&aji_19_init_evt);
    relSetBtlAddr("aji",&btlsetup_aji_tbl,&btl_aji_setup_no_tbl);
    */
}

RELMODULE_EXPORT void _epilog(void) {
    voidfunctionptr* destructor;

    //mapdelete();
    for (destructor = _dtors; *destructor; destructor++) {
        (*destructor)();
    }
}

RELMODULE_EXPORT void _unresolved(void) {
	; //stubbed in retail
}