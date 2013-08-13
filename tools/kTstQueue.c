/* $Id: kTstQueue.c,v 1.1 2000/05/23 14:36:46 stknut Exp $
 *
 * This test program creates a queue and writes to it until the
 * writes failes (normally due to ERROR_QUE_NO_MEMORY).
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


/**
 *
 */
void main(int argc, char **argv)
{
    HQUEUE  hqTest;
    ULONG   cMaxElements;
    APIRET  rc;

    if (argc > 1)
        cMaxElements = atol(argv[1]);
    else
        cMaxElements = 0xffffffff;

    rc = DosCreateQueue(&hqTest, QUE_FIFO, "\\QUEUES\\KTSTQUEUE");
    if (rc == NO_ERROR)
    {
        int i;
        for (i = 0; i < cMaxElements; i++)
        {
            rc = DosWriteQueue(hqTest, i, 1234, (PVOID)0x10000, i);
            if (rc != NO_ERROR )
            {
                printf("Write failed with rc=%d.\n", rc);
                break;
            }
        }

        printf("Wrote %d queue elements\npress key to close queue...\n", i);
        getch();

        DosCloseQueue(hqTest);
    }
    else
        printf("Create failed with rc=%d\n", rc);
}

