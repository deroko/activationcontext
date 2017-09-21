#include        "defs.h"

#define BUFFSIZE        0x10000


char    *g_szRunLevel[] = {
        "unspecified",
        "asInvoker",
        "highestAvailable",
        "requireAdministrator",
        "numbers"
};


VOID    DumpActivationContext(PACTIVATION_CONTEXT_DATA     pActivationContextData){
        PCACTIVATION_CONTEXT_DATA_TOC_HEADER    pTocHeader;
        PCACTIVATION_CONTEXT_DATA_TOC_ENTRY     pTocEntry;
        ULONG                                   index, jindex, kindex;
        PACTIVATION_CONTEXT_STRING_SECTION_HEADER       pStringHeader;
        PACTIVATION_CONTEXT_GUID_SECTION_HEADER         pGuidHeader;
        PACTIVATION_CONTEXT_STRING_SECTION_ENTRY        pStringEntry;
        PACTIVATION_CONTEXT_GUID_SECTION_ENTRY          pGuidEntry;
        
        PCACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER        pAssemblyHeader;
        PACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY          pAssemblyEntry;
        PACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION           pAssemblyInfo;
        
        PACTIVATION_CONTEXT_DATA_DLL_REDIRECTION                pDllRedirection;
        PACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT   pDllRedirectionPathSegment;
        
        PACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION       pWindowClassRedirection;
        
        PACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION         pComServerRedirection;
        PACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM    pComServerRedirectionShim;
        PACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION      pComInterfaceRedirection;
        PACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION         pProgIdRedirection;
        PCACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION  pTypeLibraryRedirection;      
        PCACTIVATION_CONTEXT_DATA_CLR_SURROGATE                 pClrSurrogate;
        PACTIVATION_CONTEXT_DATA_APPLICATION_SETTINGS           pApplicationSettings;
        
        PACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION           pAssemblyInformation;
        
        PACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_INFO pApplicationCompatibilityInfo;
        
        
        CHAR                                                    tmp[BUFFSIZE];               
        ULONG_PTR                                               AssemblyInformationSection;  
        WCHAR                                                   wsguid[MAX_PATH];
        
        if (pActivationContextData == NULL) return;
                        
        printf("DefaultTocOffset        :       %.08X\n", pActivationContextData->DefaultTocOffset);
        printf("ExtendedTocOffset       :       %.08X\n", pActivationContextData->ExtendedTocOffset);
        printf("AssemblyRosterOffset    :       %.08X\n", pActivationContextData->AssemblyRosterOffset);

        pTocHeader = (PCACTIVATION_CONTEXT_DATA_TOC_HEADER)((ULONG_PTR)pActivationContextData + pActivationContextData->DefaultTocOffset);
        pAssemblyHeader = (PCACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER)((ULONG_PTR)pActivationContextData + pActivationContextData->AssemblyRosterOffset);
        
        AssemblyInformationSection = (ULONG_PTR)pActivationContextData + pAssemblyHeader->AssemblyInformationSectionOffset;
        
        pAssemblyEntry  = (PACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY)((ULONG_PTR)pActivationContextData + pAssemblyHeader->FirstEntryOffset);
        
        //__debugbreak();
        //Entry 0 is reserved so we can skip it right away...
        for (index = 1; index < pAssemblyHeader->EntryCount; index++){
                if (pAssemblyEntry[index].Flags & ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY_INVALID) continue;
        
                memset(tmp, 0, sizeof(tmp));
                memcpy(tmp, (PVOID)((ULONG_PTR)pActivationContextData + pAssemblyEntry[index].AssemblyNameOffset), pAssemblyEntry[index].AssemblyNameLength);
                printf("AssemblyName       : %S\n", tmp);
        
                pAssemblyInfo = (PACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION)((ULONG_PTR)pActivationContextData + pAssemblyEntry[index].AssemblyInformationOffset);
                
                printf("Assembly info size : %.08X\n", pAssemblyInfo->Size);
                
                if (pAssemblyInfo->PolicyPathOffset != 0){
                        memset(tmp, 0, sizeof(tmp));
                        memcpy(tmp, (PVOID)((ULONG_PTR)AssemblyInformationSection + pAssemblyInfo->PolicyPathOffset), pAssemblyInfo->PolicyPathLength);
                        printf("PolicyPath         : %S\n", tmp);
                }
                if (pAssemblyInfo->ManifestPathOffset != 0){
                        memset(tmp, 0, sizeof(tmp));
                        memcpy(tmp, (PVOID)((ULONG_PTR)AssemblyInformationSection + pAssemblyInfo->ManifestPathOffset), pAssemblyInfo->ManifestPathLength);
                        printf("Manifest Path      : %S\n", tmp);
                }
                //__debugbreak();
                if (pAssemblyInfo->AssemblyDirectoryNameOffset != 0){
                        memset(tmp, 0, sizeof(tmp));
                        memcpy(tmp, (PVOID)((ULONG_PTR)AssemblyInformationSection + pAssemblyInfo->AssemblyDirectoryNameOffset), pAssemblyInfo->AssemblyDirectoryNameLength);
                        printf("AssemblyDirectory  : %S\n", tmp);
                        //printf("AssemblyDirectory:%S\n", (ULONG_PTR)AssemblyInformationSection + pAssemblyInfo->AssemblyDirectoryNameOffset);        
                }
                
                printf("RunLevel           : %s\n", g_szRunLevel[pAssemblyInfo->RunLevel]);
                printf("uiAccess           : %s\n", (pAssemblyInfo->UiAccess) ? "true" : "false");
                
                //printf("NumOfFilesInAssembly: %.08X\n", pAssemblyInfo->NumOfFilesInAssembly);
                
                //printf("%.08X\n", pAssemblyInfo->AssemblyDirectoryNameOffset + AssemblyInformationSection);
                //printf("AssemblyNameOffset      :       %.08X\n", pAssemblyEntry[index].AssemblyNameOffset);
                //printf("AssemblyNameLength      :       %.08X\n", pAssemblyEntry[index].AssemblyNameLength);
                //printf("AssemblyInformationOffset:      %.08X\n", pAssemblyEntry[index].AssemblyInformationOffset);
                //printf("AssemblyInformationLength:      %.08X\n", pAssemblyEntry[index].AssemblyInformationLength);              
                
        }        
        printf("Toc.EntryCount           :      %.08X\n", pTocHeader->EntryCount);
        printf("Toc.EntryFirstEntryOffset:      %.08X\n", pTocHeader->FirstEntryOffset);
        
        pTocEntry = (PCACTIVATION_CONTEXT_DATA_TOC_ENTRY)((ULONG_PTR)pActivationContextData + pTocHeader->FirstEntryOffset);

        for (index = 0; index < pTocHeader->EntryCount; index++){
                printf("\n");
                switch(pTocEntry[index].Id)
                {
                case ACTIVATION_CONTEXT_SECTION_ASSEMBLY_INFORMATION         :
                        printf("ACTIVATION_CONTEXT_SECTION_ASSEMBLY_INFORMATION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION              :
                        printf("ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_WINDOW_CLASS_REDIRECTION     :
                        printf("ACTIVATION_CONTEXT_SECTION_WINDOW_CLASS_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_COM_SERVER_REDIRECTION       :
                        printf("ACTIVATION_CONTEXT_SECTION_COM_SERVER_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_COM_INTERFACE_REDIRECTION    :
                        printf("ACTIVATION_CONTEXT_SECTION_COM_INTERFACE_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_COM_TYPE_LIBRARY_REDIRECTION :
                        printf("ACTIVATION_CONTEXT_SECTION_COM_TYPE_LIBRARY_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_COM_PROGID_REDIRECTION       :
                        printf("ACTIVATION_CONTEXT_SECTION_COM_PROGID_REDIRECTION\n"); break;
                case ACTIVATION_CONTEXT_SECTION_GLOBAL_OBJECT_RENAME_TABLE   :
                        printf("ACTIVATION_CONTEXT_SECTION_GLOBAL_OBJECT_RENAME_TABLE\n"); break;
                case ACTIVATION_CONTEXT_SECTION_CLR_SURROGATES               :
                        printf("ACTIVATION_CONTEXT_SECTION_CLR_SURROGATES\n"); break;
                case ACTIVATION_CONTEXT_SECTION_APPLICATION_SETTINGS         :
                        printf("ACTIVATION_CONTEXT_SECTION_APPLICATION_SETTINGS\n");break;
                case ACTIVATION_CONTEXT_SECTION_COMPATIBILITY_INFO           :
                        printf("ACTIVATION_CONTEXT_SECTION_COMPATIBILITY_INFO\n");break;
                }
                printf("\n");
                
                printf("pTocEntry[%.2d].Id                  : %.08X\n", index, pTocEntry[index].Id);        
                if (pTocEntry[index].Format == ACTIVATION_CONTEXT_SECTION_FORMAT_STRING_TABLE){
                        pStringHeader = (PACTIVATION_CONTEXT_STRING_SECTION_HEADER)((ULONG_PTR)pActivationContextData + pTocEntry[index].Offset);
                        
                        if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_COMPATIBILITY_INFO){
                                pApplicationCompatibilityInfo = (PACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_INFO)pStringHeader;
                                printf("pAppCompatibility->ElementCount   : %.08X\n", pApplicationCompatibilityInfo->ElementCount);
                                for (jindex = 0; jindex < pApplicationCompatibilityInfo->ElementCount; jindex++){
                                        //can be atm ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_SUPPORTED_OS or
                                        //           ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_MITIGATION
                                        printf("pAppCompatibility->Entry[%.02d].Type : %.08X\n", jindex, pApplicationCompatibilityInfo->Entry[jindex].Type);
                                        switch (pApplicationCompatibilityInfo->Entry[jindex].Type)
                                        {
                                        case ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_SUPPORTED_OS:
                                                printf("pAppCompatibility->Entry[%.02d].Type : SupportedOS\n", jindex); break;
                                        case ACTIVATION_CONTEXT_DATA_APPLICATION_COMPATIBILITY_TYPE_MITIGATION:
                                                printf("pAppCompatibility->Entry[%.02d].Type : Mitigation\n", jindex); break;
                                        }
                                        StringFromGUID2(&pApplicationCompatibilityInfo->Entry[jindex].Guid, wsguid, MAX_PATH);
                                        printf("pAppCompatibility->Entry[%.02d].Guid : %S\n", jindex, wsguid);       
                                }
                                        
                                continue;
                        }
                        
                        if (pStringHeader->Magic != ACTIVATION_CONTEXT_STRING_SECTION_MAGIC){
                                printf("pStringHeader->Magic doesnt match  : %.08X\n", pStringHeader->Magic);
                                continue;
                        }
                        printf("pStringHeader.ElementCount        : %.08X\n", pStringHeader->ElementCount);
                     
                        pStringEntry = (PACTIVATION_CONTEXT_STRING_SECTION_ENTRY)((ULONG_PTR)pStringHeader + pStringHeader->ElementListOffset);
                        for (jindex = 0; jindex < pStringHeader->ElementCount; jindex++){
                                printf("Key                               : %S\n", (ULONG_PTR)pStringHeader + pStringEntry[jindex].KeyOffset);
                                printf("pStringEntry->AssemblyRosterIndex : %.08X\n", pStringEntry[jindex].AssemblyRosterIndex);
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION){
                                        //__debugbreak();
                                        pDllRedirection = (PACTIVATION_CONTEXT_DATA_DLL_REDIRECTION)((ULONG_PTR)pStringHeader + pStringEntry[jindex].Offset);        
                                        printf("pDllRedirection flags             : %.08X\n", pDllRedirection->Flags);
                                        pDllRedirectionPathSegment = (PACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT)((ULONG_PTR)pStringHeader + pDllRedirection->PathSegmentOffset);
                                        for (kindex = 0; kindex < pDllRedirection->PathSegmentCount; kindex++){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pDllRedirectionPathSegment->Offset), pDllRedirectionPathSegment->Length);
                                                printf("segment                           : %S\n", tmp);        
                                                
                                        }
                                }
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_WINDOW_CLASS_REDIRECTION){
                                        pWindowClassRedirection = (PACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION)((ULONG_PTR)pStringHeader + pStringEntry[jindex].Offset);
                                        
                                        if (pWindowClassRedirection->VersionSpecificClassNameOffset != 0){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pWindowClassRedirection + pWindowClassRedirection->VersionSpecificClassNameOffset), pWindowClassRedirection->VersionSpecificClassNameLength);
                                                printf("Version specific class name       : %S\n", tmp);        
                                        }        
                                        if (pWindowClassRedirection->DllNameOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pWindowClassRedirection->DllNameOffset), pWindowClassRedirection->DllNameLength);
                                                printf("Dll name                          : %S\n", tmp);
                                        }
                                        
                                }
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_COM_PROGID_REDIRECTION){
                                        pProgIdRedirection = (PACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION)((ULONG_PTR)pStringHeader + pStringEntry[jindex].Offset);
                                        
                                        StringFromGUID2((GUID *)(pProgIdRedirection->ConfiguredClsidOffset + (ULONG_PTR)pStringHeader), wsguid, MAX_PATH);
                                        printf("ProgId ConfiguredClsid            : %S\n", wsguid);     
                                }
                                
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_ASSEMBLY_INFORMATION){
                                        pAssemblyInformation = (PACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION)((ULONG_PTR)pStringHeader + pStringEntry[jindex].Offset);
                                        
                                        if (pAssemblyInformation->ManifestPathOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pAssemblyInformation->ManifestPathOffset), pAssemblyInformation->ManifestPathLength);
                                                printf("Manifest path                     : %S\n", tmp);        
                                        }
                                        
                                        if (pAssemblyInformation->PolicyPathOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pAssemblyInformation->PolicyPathOffset), pAssemblyInformation->PolicyPathLength);
                                                printf("Policy Path                       : %S\n", tmp);
                                        }
                                        
                                        if (pAssemblyInformation->AssemblyDirectoryNameOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pAssemblyInformation->AssemblyDirectoryNameOffset), pAssemblyInformation->AssemblyDirectoryNameLength);
                                                printf("Assembly Directory                : %S\n", tmp);
                                        }
                                        
                                        if (pAssemblyInformation->LanguageOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pStringHeader + pAssemblyInformation->LanguageOffset), pAssemblyInformation->LanguageLength);
                                                printf("Language                          : %S\n", tmp);
                                        }
                                        
                                        printf("RunLevel                          : %s\n", g_szRunLevel[pAssemblyInformation->RunLevel]);
                                        printf("uiAccess                          : %s\n", (pAssemblyInformation->UiAccess) ? "true" : "false");
                                
                                }  
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_APPLICATION_SETTINGS){
                                        pApplicationSettings = (PACTIVATION_CONTEXT_DATA_APPLICATION_SETTINGS)((ULONG_PTR)pStringHeader + pStringEntry[jindex].Offset);
                                        if (pApplicationSettings->SettingNamespaceOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pApplicationSettings->SettingNamespaceOffset + (ULONG_PTR)pApplicationSettings), pApplicationSettings->SettingNamespaceLength);
                                                printf("SettingNamespace                  : %S\n", tmp);        
                                        }
                                        
                                        if (pApplicationSettings->SettingNameOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pApplicationSettings->SettingNameOffset + (ULONG_PTR)pApplicationSettings), pApplicationSettings->SettingNameLength);
                                                printf("SettingName                       : %S\n", tmp);
                                        }
                                        
                                        if (pApplicationSettings->SettingValueOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pApplicationSettings->SettingValueOffset + (ULONG_PTR)pApplicationSettings), pApplicationSettings->SettingValueLength);
                                                printf("SettingValue                      : %S\n", tmp);
                                        }
                                }      
                        }
                        
                }else if (pTocEntry[index].Format == ACTIVATION_CONTEXT_SECTION_FORMAT_GUID_TABLE){
                        pGuidHeader   = (PACTIVATION_CONTEXT_GUID_SECTION_HEADER)((ULONG_PTR)pActivationContextData + pTocEntry[index].Offset);
                        if (pGuidHeader->Magic != ACTIVATION_CONTEXT_GUID_SECTION_MAGIC){
                                printf("pGuidHeader->Magic doesnt match    : %.08X\n", pGuidHeader->Magic);
                                continue;
                        }
                        printf("pGuidHeader.ElementCount          : %.08X\n", pGuidHeader->ElementCount);

                        pGuidEntry = (PACTIVATION_CONTEXT_GUID_SECTION_ENTRY)((ULONG_PTR)pGuidHeader + pGuidHeader->ElementListOffset);
                        for (jindex = 0; jindex < pGuidHeader->ElementCount; jindex++){        
                                memset(wsguid, 0, sizeof(wsguid));
                                StringFromGUID2(&pGuidEntry[jindex].Guid, wsguid, MAX_PATH);
                                printf("guid                              : %S\n", wsguid);
                                printf("pGuidEntry->AssemblyRosterIndex   : %.08X\n", pGuidEntry->AssemblyRosterIndex);      
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_COM_SERVER_REDIRECTION){
                                        pComServerRedirection = (PACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION)((ULONG_PTR)pGuidHeader + pGuidEntry[jindex].Offset);
                                        printf("COM dll server redirection        : %S\n", (ULONG_PTR)pGuidHeader + pComServerRedirection->ModuleOffset);
                                        StringFromGUID2(&pComServerRedirection->ReferenceClsid, wsguid, MAX_PATH);
                                        printf("COM ReferenceClsid                : %S\n", wsguid);
                                        StringFromGUID2(&pComServerRedirection->ConfiguredClsid, wsguid, MAX_PATH);
                                        printf("COM ConfiguredClsid               : %S\n", wsguid);
                                        StringFromGUID2(&pComServerRedirection->ImplementedClsid, wsguid, MAX_PATH);
                                        printf("COM ImplementedClsid              : %S\n", wsguid);
                                        StringFromGUID2(&pComServerRedirection->TypeLibraryId, wsguid, MAX_PATH);
                                        printf("COM TypeLibraryId                 : %S\n", wsguid);
                                        if (pComServerRedirection->ProgIdOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pComServerRedirection + pComServerRedirection->ProgIdOffset), pComServerRedirection->ProgIdLength);
                                                printf("COM ProgId                        : %S\n", tmp);        
                                        }
                                        
                                        if (pComServerRedirection->ShimDataOffset){
                                                __debugbreak();
                                                //not used atm... debugbreak is there to signal it's being hit...
                                                pComServerRedirectionShim = (PACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM)((ULONG_PTR)pGuidHeader + pComServerRedirection->ShimDataOffset);
                                                if (pComServerRedirectionShim->ModuleOffset){
                                                        
                                                }        
                                        }
                                        
                                }
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_COM_INTERFACE_REDIRECTION){
                                        pComInterfaceRedirection = (PACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION)((ULONG_PTR)pGuidHeader + pGuidEntry[jindex].Offset);
                                        StringFromGUID2(&pComInterfaceRedirection->ProxyStubClsid32, wsguid, MAX_PATH);
                                        printf("Interface ProxyStubClsid32        : %S\n", wsguid);
                                        StringFromGUID2(&pComInterfaceRedirection->TypeLibraryId, wsguid, MAX_PATH);
                                        printf("Interface TypeLibraryId           : %S\n", wsguid);
                                        StringFromGUID2(&pComInterfaceRedirection->BaseInterface, wsguid, MAX_PATH);
                                        printf("Interface BaseInterface           : %S\n", wsguid);
                                        printf("Interface NumMethods              : %.08X\n", pComInterfaceRedirection->NumMethods);
                                        if (pComInterfaceRedirection->NameOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pComInterfaceRedirection + pComInterfaceRedirection->NameOffset), pComInterfaceRedirection->NameLength);
                                                printf("Interface name                    : %S\n", tmp);                
                                        }
                                }
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_CLR_SURROGATES){
                                        pClrSurrogate = (PACTIVATION_CONTEXT_DATA_CLR_SURROGATE)((ULONG_PTR)pGuidHeader + pGuidEntry[jindex].Offset);
                                        StringFromGUID2(&pClrSurrogate->SurrogateIdent, wsguid, MAX_PATH);
                                        printf("ClrSurrogate SurrogateIdent       : %S\n", wsguid);
                                        if (pClrSurrogate->VersionOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pClrSurrogate + pClrSurrogate->VersionOffset), pClrSurrogate->VersionLength);        
                                                printf("ClrSurrogate Version              : %S\n", tmp);
                                        }
                                }
                                
                                if (pTocEntry[index].Id == ACTIVATION_CONTEXT_SECTION_COM_TYPE_LIBRARY_REDIRECTION){
                                        pTypeLibraryRedirection = (PCACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION)((ULONG_PTR)pGuidHeader + pGuidEntry[jindex].Offset);
                                        if (pTypeLibraryRedirection->NameOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pGuidHeader + pTypeLibraryRedirection->NameOffset), pTypeLibraryRedirection->NameLength);
                                                printf("TypeLib Redirection Name          : %S\n", tmp);        
                                        }        
                                        
                                        if (pTypeLibraryRedirection->HelpDirOffset){
                                                memset(tmp, 0, sizeof(tmp));
                                                memcpy(tmp, (PVOID)((ULONG_PTR)pTypeLibraryRedirection + pTypeLibraryRedirection->HelpDirOffset), pTypeLibraryRedirection->HelpDirLength);
                                                printf("TypeLib HelpDir                   : %S\n", tmp);
                                        }
                                        
                                        printf("TypeLib ResourceId                : %.08X\n", pTypeLibraryRedirection->ResourceId);
                                }
                        }
                        
                        
                }else{
                        __debugbreak();
                }       
        
        
        }
}




int __cdecl wmain(int argc, wchar_t **argv){
        ULONG_PTR                       peb;
        PACTIVATION_CONTEXT_DATA        pActivationContextData;        
                 
        IUnknown                        *pUnknown;
        BOOL                            b_use_system_default = FALSE;
        
        if (argc == 2 && !_wcsicmp(argv[1], L"--system"))
                b_use_system_default = TRUE;    
                
        //trigger loading of class from thisismyhtmldummydll.dll, it doesn't exist, but you may
        //see in debugger call to LoadLibraryExW with this name from manifest.manifest    
        CoInitialize(NULL);
        CoCreateInstance(&CLSID_HTMLApplication, 0, CLSCTX_INPROC_SERVER, &IID_IUnknown, &pUnknown);
        CoUninitialize();
        
        LoadLibrary(L"comctl32.dll");
        //this is file redirection in manifest.manifest...
        LoadLibrary(L"xmllite.dll");
        #ifdef _WIN64
        peb = __readgsqword(0x60);
        if (!b_use_system_default)
                pActivationContextData  = (PACTIVATION_CONTEXT_DATA)*(ULONG_PTR *)(peb + 0x2f8);
        else
                pActivationContextData  = (PACTIVATION_CONTEXT_DATA)*(ULONG_PTR *)(peb + 0x308);
        #else
        peb = __readfsdword(0x30);
        if (!b_use_system_default)
                pActivationContextData  = (PACTIVATION_CONTEXT_DATA)*(ULONG_PTR *)(peb + 0x1f8);
        else
                pActivationContextData  = (PACTIVATION_CONTEXT_DATA)*(ULONG_PTR *)(peb + 0x200);
        #endif        
        DumpActivationContext(pActivationContextData);
}
