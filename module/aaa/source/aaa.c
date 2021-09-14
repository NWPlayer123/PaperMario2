#include "aaa_00.h"

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

RELMODULE_EXPORT void _prolog(void) {
    voidfunctionptr* constructor;

    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }

}

RELMODULE_EXPORT void _epilog(void) {
    voidfunctionptr* destructor;

    mapdelete();
    for (destructor = _dtors; *destructor; destructor++) {
        (*destructor)();
    }
}

RELMODULE_EXPORT void _unresolved(void) {
	; //stubbed in retail
}