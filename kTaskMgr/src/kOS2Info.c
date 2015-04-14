/* $Id: kOS2Info.c,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * OS/2 Information.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 * Project Odin Software License can be found in LICENSE.TXT
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_BASE


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "kOS2Info.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
BOOL     fOS2InfoInited = FALSE;
ULONG    ulBuildLevel;
BOOL     fSMPKernel;
BOOL     fW4Kernel;
BOOL     fUNIKernel;

ULONG    ulMaxSysInfoIndex;
SYSINFO  SysInfo;



/**
 * Initatiates cached data (global variables)
 * Collects the information
 * @author  knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark  Changes all global variables in this module.
 */
BOOL OS2InfoInit(VOID)
{
    char    szOS2Krnl[] = "x:\\OS2KRNL";
    FILE   *phOS2Krnl = NULL;

    if (fOS2InfoInited)
        return TRUE;

    /*
     * We need to have the SysInfo array.
     */
    if (!OS2UpdateSysInfo())
        return FALSE;

    /*
     * Get build number from os2krnl files "Internal revision" number.
     * (The internal revision string is found in one of the first data objects of the file.)
     *
     */
    szOS2Krnl[0] = (char)(SysInfo.s.ulBootDrive + 'A' - 1); /* ulBootDrive = 1: 'A' */
    phOS2Krnl = fopen(szOS2Krnl, "rb");
    if (phOS2Krnl)
    {
        #define MIN_LEFT   50
        #define BUFFERSIZE 4096
        char    achBuffer[MIN_LEFT + BUFFERSIZE];
        char  * pch = &achBuffer[MIN_LEFT];

        for (;;)
        {
            /*
             * Read next the buffer part
             */
            size_t  cbRead = fread(&achBuffer[MIN_LEFT], 1, BUFFERSIZE, phOS2Krnl);
            char *  pchEnd = &achBuffer[cbRead];

            /*
             * Search thru the buffer.
             */
            while (pch < pchEnd)
            {
                if (!memcmp(pch, "Internal revision ", 18))
                {
                    /*
                     * Found "Internal revision "!
                     */
                    pch += 18;          /* now point to the start of the number. */
                    ulBuildLevel = 0;
                    while ((*pch >= '0' && *pch <= '9') || *pch == '.')
                    {
                        if (*pch != '.')
                            ulBuildLevel = (ulBuildLevel*10) + *pch++ - '0';
                        else
                            pch++;
                    }

                    /*
                     * See if there any SMP flags...
                     */
                    if (*pch != ',' &&
                        ((pch[0] == '_' && (pch[1] & ~0x20) == 'S')
                         ||
                         (pch[1] == '_' && (pch[2] & ~0x20) == 'S')
                         )
                        )
                    {
                        fSMPKernel = TRUE;;
                        fW4Kernel = fUNIKernel = FALSE;
                    }
                    else if (*pch != ',' &&
                            ((pch[0] == '_' && (pch[1] & ~0x20) == 'W' && pch[2] == '4')
                             ||
                             (pch[1] == '_' && (pch[2] & ~0x20) == 'W' && pch[3] == '4')
                             )
                            )
                    {
                        fW4Kernel = TRUE;
                        fUNIKernel = fSMPKernel = FALSE;
                    }
                    else
                    {
                        fUNIKernel = FALSE;
                        fSMPKernel = fW4Kernel = FALSE;
                    }

                    fOS2InfoInited = TRUE;
                    fclose(phOS2Krnl);
                    return TRUE;
                }

                /* next char */
                ++pch;
            }

            /*
             * finished?
             */
            if (pch != &achBuffer[BUFFERSIZE])
                break;

            /*
             * Copy the part we've left at the upper end of the buffer
             */
            memcpy(&achBuffer[0], &achBuffer[BUFFERSIZE], MIN_LEFT);
            pch = &achBuffer[0];
        }

        fclose(phOS2Krnl);
    }

    return FALSE;
}


/**
 * Updates the SysInfo array.
 * @returns Success indicator (TRUE / FALSE)
 * @author  knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL OS2UpdateSysInfo(VOID)
{
    APIRET rc = 0;

    /*
     * Initiate the SysInfo array.
     */
    if (!fOS2InfoInited)
        ulMaxSysInfoIndex = QSV_FOREGROUND_PROCESS;
    rc = DosQuerySysInfo(QSV_MAX_PATH_LENGTH,
                         ulMaxSysInfoIndex,
                         &SysInfo.ach[0],
                         ulMaxSysInfoIndex * sizeof(ULONG));
    if (rc != NO_ERROR)
        ulMaxSysInfoIndex = 0;
    do
    {
        ulMaxSysInfoIndex++;
        rc = DosQuerySysInfo(ulMaxSysInfoIndex,
                             ulMaxSysInfoIndex,
                             &SysInfo.ach[ulMaxSysInfoIndex-1],
                             sizeof(ULONG));
    } while (rc == NO_ERROR && ulMaxSysInfoIndex < 50);
    ulMaxSysInfoIndex--;

    return ulMaxSysInfoIndex >= QSV_VERSION_REVISION;
}

