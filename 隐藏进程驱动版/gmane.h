



#ifndef GMANE_H
#define GMANE_H
#include <windows.h>
#include <winioctl.h>
#include <Winsvc.h>
#include <string>
#include <stdbool.h>
#include <numeric>



 // ÉùÃ÷ hDevice

#endif // GMANE_H


// IOCTL ¿ØÖÆ´úÂë
#define ioctl_TEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x550, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define ioctl_PRETEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x510, METHOD_BUFFERED, FILE_ANY_ACCESS)


 BOOLEAN   LOADENTRY(UINT pid);

