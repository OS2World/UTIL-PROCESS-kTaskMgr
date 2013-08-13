/* $Id: kOpenFile.c,v 1.1 2000/05/23 14:36:46 stknut Exp $
 *
 * This test program open the files specified on the commmandline,
 * waits for a keystroke, and exits.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
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
#include <stdlib.h>
#include <conio.h>
#include <string.h>

/**
 *
 */
void main(int argc, char **argv)
{
    int argi;
    int cOpenFiles = 0;

    for (argi = 1; argi < argc; argi++)
    {
        APIRET          rc;
        FILEFINDBUF3    findbuf;
        ULONG           cFiles = 1;
        HDIR            hDir = HDIR_CREATE;
        CHAR            szFilename[CCHMAXPATH];
        PSZ             psz;

        strcpy(szFilename, argv[argi]);
        psz = &szFilename[strlen(szFilename)];
        do
        {
            psz--;
        } while (psz >= &szFilename[0]
                 && *psz != '\\' && *psz != '/' && *psz != ':');
        psz++;

        rc = DosFindFirst(argv[argi], &hDir, FILE_NORMAL,
                          &findbuf, sizeof(findbuf),
                          &cFiles, FIL_STANDARD);
        while (rc == NO_ERROR && cFiles > 0)
        {
            ULONG   ulAction;
            HFILE   hf;

            strcpy(psz, findbuf.achName);
            rc = DosOpen(&szFilename[0], &hf, &ulAction, 0, FILE_NORMAL,
                         OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                         OPEN_FLAGS_RANDOM | OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
                         NULL);
            if (rc == ERROR_TOO_MANY_OPEN_FILES)
            {
                rc = DosSetMaxFH(cOpenFiles + 20);
                rc = DosOpen(&szFilename[0], &hf, &ulAction, 0, FILE_NORMAL,
                             OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                             OPEN_FLAGS_RANDOM | OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
                             NULL);
            }

            if (rc != NO_ERROR)
                printf("Failed with rc=%d when trying to open %s\n", rc, szFilename);
            else
            {
                printf("hf=0x%04x %s\n", hf, szFilename);
                cOpenFiles++;
            }

            rc = DosFindNext(hDir, &findbuf, sizeof(findbuf), &cFiles);
        }

        DosFindClose(hDir);
    }

    printf("Opened %d files.\n", cOpenFiles);
    getch();
}

