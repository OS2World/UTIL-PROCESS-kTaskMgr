/* $Id: kError.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kError (kClassLib/kTaskMgr) - Implementation of the kError class
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSMISC
#define INCL_DOSERRORS
#define INCL_WINERRORS
#define INCL_WINDIALOGS

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include <string.h>
#include <stdio.h>

#include "kError.h"
#undef   logError




/**
 * Sets the error number.
 * @returns   Reference to this object.
 * @param     ulError  Error number
 */
kMyError &kMyError::setError(ULONG ulError)
{
    ulMyErrorNo = ulError;
    return *this;
}



/**
 * Creates and sets the error number.
 * @param     Error number
 */
kMyError::kMyError()
{
    ulMyErrorNo = unused;
}



/**
 * Creates and sets the error number.
 * @param     ulError  Error number
 */
kMyError::kMyError(ULONG ulError)
{
    ulMyErrorNo = ulError;
}



/**
 * Copy constructor.
 * @param     obj  Object to copy.
 */
kMyError::kMyError(const kMyError &obj)
{
    ulMyErrorNo = obj.ulMyErrorNo;
}



/**
 * Gets the error number.
 * @returns   Error number
 */
ULONG kMyError::getErrorNo() const
{
    return ulMyErrorNo;
}



/**
 * Get the error description.
 * @returns   Pointer to a _read-only_ string.
 */
const char *kMyError::getDescription() const
{
    const char *pszRet;

    switch (ulMyErrorNo)
    {
        case unused:
            pszRet = "...";
            break;

        case new_failed:
            pszRet = "new returned a null pointer.";
            break;

        case WinCreateWindow_failed_frame:
            pszRet = "WinCreateWindow frame failed.";
            break;

        case WinCreateWindow_failed_client:
            pszRet = "WinCreateWindow client failed.";
            break;

        case user_cancel:
            pszRet = "User canceld operation.";
            break;

        case compressDiff_failed:
            pszRet = "compressDiff failed.";
            break;

        case unsupported_bitcount:
            pszRet = "Unsupported bitcount.";
            break;

        case dive_failed:
            pszRet = "A kDive operation failed.";
            break;

        case error_opening_movie:
            pszRet = "Error opening movie.";
            break;

        case getPalette_failed:
            pszRet = "getPalette failed.";
            break;

        case createPalette_failed:
            pszRet = "createPalette failed.";
            break;

        case getMousePos_failed:
            pszRet = "getMousePos failed.";
            break;

        case DosCreateEventSem_failed:
            pszRet = "DosCreateEventSem failed.";
            break;

        case WinRegisterClass_failed:
            pszRet = "WinRegisterClass failed.";
            break;

        case fopen_failed:
            pszRet = "fopen failed.";
            break;

        case ftell_failed:
            pszRet = "ftell failed.";
            break;

        case fread_failed:
            pszRet = "fread failed.";
            break;

        case fseek_failed:
            pszRet = "fseek failed.";
            break;

        case fwrite_failed:
            pszRet = "fwrite failed.";
            break;

        case beginthread_failed:
            pszRet = "_beginthread failed";
            break;

        case alloc_failed:
            pszRet = "(c/m/re)alloc failed";
            break;

        case invalid_class:
            pszRet = "Invalid class (restoring objects from a file).";
            break;

        case invalid_object:
            pszRet = "Invalid object (restoring objects from a file).";
            break;

        case invalid_fileformat:
            pszRet = "Invalid fileformat (reading file header).";
            break;

            /*
         case :
            pszRet = "";
            break;
         */

        default:
            pszRet = "unknown error";
    }
    return pszRet;
}








/* * * * * * * * * * * */
/* k D i v e E r r o r */
/* k D i v e E r r o r */
/* k D i v e E r r o r */
/* * * * * * * * * * * */


/**
 * Set error, procteded.
 * @returns   Reference to this object.
 * @param     ulError  Error number.
 */
kDiveError &kDiveError::setError(ULONG ulError)
{
    ulDiveError = ulError;
    return *this;
}



/**
 * "not used" - Constructor
 * @param     ulError   Dive errornumber.
 */
kDiveError::kDiveError()
{
    ulDiveError = 0xdeadbeefL;
}



/**
 * Dive error class contstructor.
 * @param     ulError   Dive errornumber.
 */
kDiveError::kDiveError(ULONG ulError)
{
    ulDiveError = ulError;
}



/**
 * Dive error class copy contstructor.
 * @param     obj   Object to copy.
 */
kDiveError::kDiveError(const kDiveError &obj)
{
    ulDiveError = obj.ulDiveError;
}



/**
 * Gets the error number.
 * @returns   Error number.
 */
ULONG kDiveError::getErrorNo() const
{
    return ulDiveError;
}



/**
 * Translates the ulError number into a short error description text. This text is
 * staticly kept in the data segment, and therefor not necessary to free.
 * NOTE: it must not be changed!
 * @returns   Pointer to error description (text)
 * @remark    Do not altern returned text!
 */
const char *kDiveError::getDescription() const
{
    const char *pszRet;

    switch (ulDiveError)
    {
        case DIVE_SUCCESS:
            pszRet = "Command completed successfully.";
            break;

        case DIVE_ERR_INVALID_INSTANCE:
            pszRet = "The DIVE instance handle specified in the hDiveInst parameter is invalid.";
            break;

        case DIVE_ERR_SOURCE_FORMAT:
            pszRet = "The FOURCC of the source format is not a recognized FOURCC.";
            break;

        case DIVE_ERR_DESTINATION_FORMAT:
            pszRet = "The FOURCC of the destination format is not a recognized FOURCC.";
            break;

        case DIVE_ERR_BLITTER_NOT_SETUP:
            pszRet = "DiveSetupBlitter must be called before a call is made to DiveBlitImage.";
            break;

        case DIVE_ERR_INSUFFICIENT_LENGTH:
            pszRet = "The pFormatData of length ulFormatLength (specified in the DIVE_CAPS structure) is not large "
                     "enough for the total number of input and output formats.";
            break;

        case DIVE_ERR_TOO_MANY_INSTANCES:
            pszRet = "There are not enough resources for another DIVE instance.";
            break;

        case DIVE_ERR_NO_DIRECT_ACCESS:
            pszRet = "The display adapter, display driver, or current video mode does not support direct-to-screen "
                     "access.";
            break;

        case DIVE_ERR_NOT_BANK_SWITCHED:
            pszRet = "The display adapter is not a bank-switched adapter.";
            break;

        case DIVE_ERR_INVALID_BANK_NUMBER:
            pszRet = "The specified bank number is invalid.";
            break;

        case DIVE_ERR_FB_NOT_ACQUIRED:
            pszRet = "The frame buffer was not acquired in this instance.";
            break;

        case DIVE_ERR_FB_ALREADY_ACQUIRED:
            pszRet = "The frame buffer has already been acquired by this instance.";
            break;

        case DIVE_ERR_ACQUIRE_FAILED:
            pszRet = "The acquire action did not complete successfully.";
            break;

        case DIVE_ERR_BANK_SWITCH_FAILED:
            pszRet = "The bank could not be switched to the specified parameters.";
            break;

        case DIVE_ERR_DEACQUIRE_FAILED:
            pszRet = "The deacquire action did not complete successfully.";
            break;

        case DIVE_ERR_INVALID_PALETTE:
            pszRet = "The palette specified for the source or destination data is invalid.";
            break;

        case DIVE_ERR_INVALID_DESTINATION_RECTL:
            pszRet = "The rectangle of the output window is an invalid region.";
            break;

        case DIVE_ERR_INVALID_BUFFER_NUMBER:
            pszRet = "The buffer number must be a previously allocated or associated buffer number before it can "
                     "be accessed.";
            break;

        case DIVE_ERR_SSMDD_NOT_INSTALLED:
            pszRet = "The device driver SSMDD.SYS is missing from CONFIG.SYS.";
            break;

        case DIVE_ERR_BUFFER_ALREADY_ACCESSED:
            pszRet = "The buffer has already been accessed by a previous call.";
            break;

        case DIVE_ERR_BUFFER_NOT_ACCESSED:
            pszRet = "The buffer has not been accessed (occurs on systems with accelerator-enabled hardware).";
            break;

        case DIVE_ERR_TOO_MANY_BUFFERS:
            pszRet = "There are not enough resources for another DIVE buffer.";
            break;

        case DIVE_ERR_ALLOCATION_ERROR:
            pszRet = "The hardware blitter memory allocation failed.";
            break;

        case DIVE_ERR_INVALID_LINESIZE:
            pszRet = "The scan line size specified in DiveAllocImageBuffer is invalid.";
            break;

        case DIVE_ERR_FATAL_EXCEPTION:
            pszRet = "DIVE is unable to register the exception handler to handle bank switching.";
            break;

        case DIVE_ERR_INVALID_CONVERSION:
            pszRet = "The source image format cannot be converted as requested to the destination image format.";
            break;

        case DIVE_ERR_VSD_ERROR:
            pszRet = "The vendor-specific driver (VSD) handle is invalid.";
            break;

        case DIVE_ERR_COLOR_SUPPORT:
            pszRet = "DIVE does not support any of the available hardware formats (occurs on overlay-accelerated"
                     "systems).";
            break;

        case DIVE_ERR_OUT_OF_RANGE:
            pszRet = "Currently, DIVE_ERR_OUT_OF_RANGE is not returned by any function.";
            break;

        case DIVE_WARN_NO_SIZE:
            pszRet = "No blitting will occur because either the destination width or height is zero.";
            break;

        case (ULONG)alloc_mem_failed:
            pszRet = "DosAllocMem failed.";
            break;

        case (ULONG)not_direct_access:
            pszRet = "No Direct Screen access possible.";
            break;

        case (ULONG)failed_to_create_framewnd:
            pszRet = "Failed to create kDiveWnd frame window.";
            break;

        case (ULONG)failed_to_create_mutex:
            pszRet = "Failed to create kDiveWnd access mutex(hmtx).";
            break;

        case (ULONG)failed_to_create_clientwnd:
            pszRet = "Failed to create kDiveWnd client window.";
            break;

        case (ULONG)queryRectangles_failed:
            pszRet = "queryRectangles failed.";
            break;

        case (ULONG)unsupported_colorencoding:
            pszRet = "kDiveWnd: Unsuported colorencoding.";
            break;

        default:
            pszRet = "Unknown error.";
    }

    return pszRet;
}






/* * * * * * * * * * */
/* k W i n E r r o r */
/* k W i n E r r o r */
/* k W i n E r r o r */
/* * * * * * * * * * */


/**
 * Gets the error info string.
 * @param     ulLevel  Error info level. 0 based!
 * @returns   Pointer to string. May be NULL-pointer.
 */
const char *kWinError::getErrorInfoText(ULONG ulLevel /*= 0*/) const
{
    char *pszRet = psz;

    if (pErrInfo != NULL)
    {
        if (ulLevel > pErrInfo->cDetailLevel)
            ulLevel = pErrInfo->cDetailLevel - 1;

        pszRet = (char*)((int)pErrInfo + ((PUSHORT)((ULONG)pErrInfo + pErrInfo->offaoffszMsg))[ulLevel]);
    }
    return pszRet;
}



/**
 * Set the errid and pErrInfo.
 * @returns   Reference to this object.
 * @param     hab  Handle to anchorblock.
 */
kWinError &kWinError::setError(HAB hab /*= NULLHANDLE*/)
{
    /* cleanup */
    if (pErrInfo != NULL)
    {
        #if 0
        WinFreeErrorInfo(pErrInfo); /* is this correct? Don't think so.... */
        #endif
        pErrInfo = NULL;
    }
    if (psz != NULL)
    {
        delete psz;
        psz = NULL;
    }
    /* get error info */
    errid    = WinGetLastError(hab);
    pErrInfo = WinGetErrorInfo(hab);
    if (pErrInfo == NULL)
    {
        psz = new char[50];
        if (psz != NULL)
            sprintf(psz, "error no.0x%04x (serv. 0x%x)", (USHORT)errid, (USHORT)(errid >> 16));
    }
    return *this;
}



/**
 * Constructor.
 * @param     hab        Handle to anchor block, NULLHANDLE is allowed and default.
 * @param     fInitiate  TRUE:  Initiate error
 *                       FALSE: do noting.
 */
kWinError::kWinError(HAB hab /*= NULLHANDLE*/, BOOL fInitiate /*= TRUE*/) : fErrInfoDelete(FALSE)
{
    if (fInitiate)
        setError(hab);
    else
    {
        psz = NULL;
        pErrInfo = NULL;
        errid = 0xdeadbeefL;
    }
}



/**
 * Constructor.
 * @param     hab        Handle to anchor block, NULLHANDLE is allowed and default.
 * @param     fInitiate  TRUE:  Initiate error
 *                       FALSE: do noting.
 */
kWinError::kWinError(const kWinError &obj) : fErrInfoDelete(TRUE)
{
    if (obj.psz != NULL)
    {
        psz = new char[strlen(obj.psz) + 1];
        strcpy(psz, obj.psz);
    }
    else
        psz = NULL;

    if (obj.pErrInfo != NULL)
    {
        pErrInfo = (PERRINFO)new char[obj.pErrInfo->cbFixedErrInfo];
        memcpy(pErrInfo, obj.pErrInfo, (size_t)obj.pErrInfo->cbFixedErrInfo);
    }
    else
        pErrInfo = NULL;

    errid = obj.errid;
}

/**
 * Destructor frees error info.
 */
kWinError::~kWinError()
{
    if (pErrInfo != NULL)
    {
        if (fErrInfoDelete)
            delete pErrInfo;
        #if 0
        else
            WinFreeErrorInfo(pErrInfo); /* is this correct? Don't think so.... */
        #endif
        pErrInfo = NULL;
    }
    if (psz != NULL)
    {
        delete psz;
        psz = NULL;
    }
}



/**
 * Get error description.
 * @returns   Pointer to description text.
 * @remark    return text should be used as read only data.
 */
const char *kWinError::getDescription() const
{
    const char *pszRet = getErrorInfoText(0);

    return pszRet == NULL ? "" : pszRet;
}



/**
 * Get error number. This does not include the serverity code.
 * @returns   Error number. This does not include the serverity code.
 */
ULONG kWinError::getErrorNo() const
{
    return(ULONG)(USHORT)errid;
}



/**
 * Get the serverity code.
 * @returns   The serverity code.
 */
ULONG kWinError::getServerity() const
{
    return(ULONG)(USHORT)(errid >> 16);
}






/* * * * * * * * * * */
/* k D o s E r r o r */
/* k D o s E r r o r */
/* k D o s E r r o r */
/* * * * * * * * * * */


/**
 * Prefetches the error message from the OS/2 message.
 */
VOID kDosError::prefetchMsg()
{
    /* not fetched from message file. */
    if (ulRc == NO_ERROR
        || (ulRc >= 40 && ulRc <= 49)
       )
        return;

    /* delete psz if allready allocated */
    if (psz != NULL)
        delete psz;

    psz = new char[4096];
    if (psz != NULL)
    {
        ULONG ulRet;

        /* try DosGetMessage() first */
        if (DosGetMessage(NULL, 0, psz, 4096, ulRc, "OSO001.MSG", &ulRet) == NO_ERROR
            && ulRet != 0)
        {
            ULONG ulRet2;

            psz[ulRet] = '\0';
            strcat(psz, "\n\n");
            if (DosGetMessage(NULL, 0, &psz[strlen(psz)], 4096 - strlen(psz), ulRc, "OSO001H.MSG", &ulRet2) != NO_ERROR)
                psz[ulRet] = '\0';
            else
                psz[ulRet+ulRet2] = '\0';
        }
        else
            sprintf(psz, "Error description not found for error %d (0x%x).", ulRc, ulRc);
    }
}


/**
 * Sets error.
 * @returns   Reference to this object.
 * @param     ulRc  Error number
 */
kDosError &kDosError::setError(ULONG ulRc)
{
    this->ulRc = ulRc;
    prefetchMsg();
    return *this;
}



/**
 * Get error number.
 * @param     ulRc  Error number
 */
kDosError::kDosError(ULONG ulRc) : ulRc(ulRc), psz(NULL)
{
    prefetchMsg();
}



/**
 * Default contructor.
 */
kDosError::kDosError() : ulRc(0xdeadbeefL), psz(NULL)
{
}



/**
 * Copy contructor.
 */
kDosError::kDosError(const kDosError &obj) : ulRc(obj.ulRc)
{
    if (obj.psz != NULL)
    {
        psz = new char[strlen(obj.psz) + 1];
        strcpy(psz, obj.psz);
    }
    else
        psz = NULL;
}



/**
 * Destructor.
 */
kDosError::~kDosError()
{
    if (psz != NULL)
    {
        delete psz;
        psz = NULL;
    }
}



/**
 * Gets error number.
 * @returns   Error number.
 */
const char *kDosError::getDescription() const
{
    const char *pszRet;

    switch (ulRc)
    {
        case NO_ERROR:       pszRet = "no error."; break;

            /* errornumbers which are not in message files: - remeber to exclude these in prefetchMsg()*/
        case 40:             pszRet = "ERROR_BAD_COMMAND"; break;
        case 41:             pszRet = "ERROR_CRC"; break;
        case 42:             pszRet = "ERROR_BAD_LENGTH"; break;
        case 43:             pszRet = "ERROR_SEEK"; break;
        case 44:             pszRet = "ERROR_NOT_DOS_DISK"; break;
        case 45:             pszRet = "ERROR_SECTOR_NOT_FOUND"; break;
        case 46:             pszRet = "ERROR_OUT_OF_PAPER"; break;
        case 47:             pszRet = "ERROR_WRITE_FAULT"; break;
        case 48:             pszRet = "ERROR_READ_FAULT"; break;
        case 49:             pszRet = "ERROR_GEN_FAILURE"; break;


        default:
            if (psz == NULL)
                return "IPE! Unknown error.";
            else
                pszRet = psz;
    }
    return pszRet;
}


/**
 * Gets error number.
 * @returns   Error number.
 */
ULONG kDosError::getErrorNo() const
{
    return ulRc;
}






/* * * * * * * */
/* k E r r o r */
/* k E r r o r */
/* k E r r o r */
/* * * * * * * */


/**
 * Constructor.
 * @param     ulErrorNo    Error number
 * @param     ulErrorType  Error type.
 */
kError::kError(unsigned long ulErrorNo, unsigned long ulErrorType/* = kError::my*/) :
kWinError(NULLHANDLE, FALSE),
psz(NULL)
{
    setError(ulErrorNo, ulErrorType);
}



/**
 * Copy constructor.
 * @param     obj  Object to copy.
 */
kError::kError(const kError &obj) : kMyError(obj), kDiveError(obj), kWinError(obj), kDosError(obj)
{
    if (obj.psz != NULL)
    {
        psz = new char[strlen(obj.psz) + 1];
        strcpy(psz, obj.psz);
    }
    else
        psz = NULL;
    ulErrorType = obj.ulErrorType;
}



/**
 * Destructor - frees psz.
 */
kError::~kError()
{
    if (psz != NULL)
    {
        delete psz;
        psz = NULL;
    }
}



/**
 * Set the error.
 * @param     ulErrorNo    Error number
 * @param     ulErrorType  Error type.
 * @returns   Reference to this object.
 */
kError &kError::setError(unsigned long ulErrorNo, unsigned long ulErrorType /*= kError::my*/)
{
    switch (ulErrorType)
    {
        case my:
            kMyError::setError(ulErrorNo);
            break;

        case dive:
            kDiveError::setError(ulErrorNo);
            break;

        case win:
            kMyError::setError(ulErrorNo);
            kWinError::setError();

            if (psz != NULL)
                delete psz;
            psz = new char[strlen(kMyError::getDescription()) + strlen(kWinError::getDescription()) + 3 + 1];
            if (psz != NULL)
                strcat(strcat(strcpy(psz, kMyError::getDescription()), " - "), kWinError::getDescription());
            break;

        case dos:
            kDosError::setError(ulErrorNo);
            break;
    }
    this->ulErrorType = ulErrorType;
    return *this;
}



/**
 * Get the error description.
 * @returns   Pointer to a _read-only_ string.
 */
const char *kError::getDescription() const
{
    const char *pszRet;

    switch (ulErrorType)
    {
        case my:
            pszRet = kMyError::getDescription();
            break;

        case dive:
            pszRet = kDiveError::getDescription();
            break;

        case win:
            if (psz != NULL)
                pszRet = psz;
            else
                pszRet = "kError: new failed.";
            break;

        case dos:
            pszRet = kDosError::getDescription();
            break;

        default:
            pszRet = "IPE! Invalid error type.";
    }

    return pszRet;
}



/**
 * Gets the error number.
 * @returns   Error number
 */
unsigned long kError::getErrorNo() const
{
    switch (ulErrorType)
    {
        case my:
            return kMyError::getErrorNo();

        case dive:
            return kMyError::getErrorNo();

        case win:
            return kMyError::getErrorNo();

        case dos:
            return kMyError::getErrorNo();
    }

    return 0xdeadbeefL;
}



/**
 * Gets error type.
 * @returns   Error type.
 */
unsigned long  kError::getErrorType() const
{
    return ulErrorType;
}



/**
 * Shows the error description, using WinMessageBox();
 * @returns   TRUE/FALSE according to result.
 * @param     pszTitle  Message box title. May NOT be NULL.
 * @remark    Should not be used in text mode.
 */
BOOL kError::showError(const char *pszTitle) const
{
    const char *pszDscr;

    pszDscr = getDescription();

    return WinMessageBox(HWND_DESKTOP,
                         HWND_DESKTOP,
                         pszDscr,
                         pszTitle,
                         0,
                         MB_OK | MB_ERROR | MB_MOVEABLE )
        != MBID_ERROR;
}



/**
 * Writes info to the log.
 * @return    returns reference to this object.
 * @param     pszFunction
 * @param     ulLine
 */
const kError &kError::logError(const char *pszFunction, ULONG ulLine) const
{
    #ifdef USE_KLIB
    kDebugLog("%s - %d: %s (errorno %d)\r\n", pszFunction, ulLine, getDescription(), getErrorNo());
    #else
    pszFunction = pszFunction;
    ulLine = ulLine;
    #endif
    return *this;
}



/**
 * Writes info to the log.
 * @return    returns reference to this object.
 * @param     pszUser
 * @param     pszFunction
 * @param     ulLine
 */
const kError &kError::logErrorStr(const char *pszUser, const char *pszFunction, ULONG ulLine) const
{
    #ifdef USE_KLIB
    kDebugLog("%s - %d: %s - %s (errorno %d)\r\n", pszFunction, ulLine, pszUser, getDescription(), getErrorNo());
    #else
    pszUser = pszUser;
    pszFunction = pszFunction;
    ulLine = ulLine;
    #endif
    return *this;
}

