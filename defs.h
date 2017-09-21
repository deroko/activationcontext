#include        <windows.h>
#include        <stdio.h>
#include        <Objbase.h>
#include        <Commctrl.h>
#include        <mshtml.h>

#include        "sxstypes.h"

#include "pshpack4.h"

#define ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_SUPPORTED_OS     1
#define ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_MITIGATION       2

typedef struct ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_ENTRY{
        GUID    Guid;
        ULONG   Type;
}ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_ENTRY, *PACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_ENTRY;

typedef struct ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_INFO{
        ULONG   ElementCount;
        ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_ENTRY Entry[];
}ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_INFO, *PACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_INFO;
#include "poppack.h"
        
        