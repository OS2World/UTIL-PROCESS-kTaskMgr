/* $Id: Exists.cmd,v 1.1 2000/05/23 00:40:55 stknut Exp $
 *
 * Simple rexx util which checks if an file exists.
 *
 * Syntax: exists.cmd <filename>
 * Return code: 0 - file exists
 *              1 - file don't exist.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
    parse arg sFile

    if (stream(sFile, 'c', 'query exists') = '') then
        exit(1);
    exit(0);

