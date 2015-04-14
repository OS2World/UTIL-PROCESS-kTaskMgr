/* $Id: kError.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kError (kClassLib/kTaskMgr).
 *
 * kError     - a collection of kMyError, kDiveError and kWinError.
 * kMyError   - an simple error class
 * kDiveError - an simple error interface to dive errors.
 * kWinError  - an simple error interface to Win-/Gpi-Api errors.
 * kDosError  - an simple error interface to Dos-Api errors.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

#ifndef _kError_h_
#define _kError_h_

#ifndef _DIVE_H_
   #include <dive.h>
#endif

#ifndef INCL_WINERRORS
   #error ("INCL_WINERRORS is not defined");
#endif


/**
 * My errors - and kPmCam errors and kTaskMgr errors.
 * @author      knut st. osmundsen
 * @version     1.0
 */
class kMyError
{
   private:
      ULONG ulMyErrorNo;

   protected:
      kMyError &setError(ULONG ulError);

   public:
      kMyError();
      kMyError(const kMyError &obj);
      kMyError(ULONG ulError);
      ULONG       getErrorNo() const;
      const char *getDescription() const;

      enum enmError
      {
         unused, new_failed,
         /* common main window stuff */
         WinCreateWindow_failed_frame, WinCreateWindow_failed_client, user_cancel,
         /* kMovie */
         getPalette_failed, createPalette_failed, dive_failed, error_opening_movie,
         /* kMovieFrame */
         unsupported_bitcount, compressDiff_failed,
         /* kMouseFrame */
         getMousePos_failed,
         /* Dos* functions */
         DosCreateEventSem_failed,
         /* Win* functions */
         WinRegisterClass_failed,
         /* stdio file functions */
         ftell_failed, fread_failed, fopen_failed, fseek_failed, fwrite_failed,
         /* other runtime library functions */
         beginthread_failed, alloc_failed,
         /* general read errors */
         invalid_class, invalid_object, invalid_fileformat
      };
};



/**
 * Dive error.
 * @description Dive errors.
 * @purpose     Error throwing in kDive and it's children.
 * @assums      Nothing.
 * @author      knut st. osmundsen
 */
class kDiveError
{
   private:
      ULONG ulDiveError;

   protected:
      kDiveError &setError(ULONG ulError);
      kDiveError();

   public:
      kDiveError(ULONG ulError);
      kDiveError(const kDiveError &obj);
      ULONG       getErrorNo() const;
      const char *getDescription() const;

      enum enmDive
      {
         /* custom dive errors */
         kDive_FirstError = 0xdeadbeefL,
         alloc_mem_failed,
         not_direct_access,

         /*DiveWnd*/
         failed_to_create_framewnd,
         failed_to_create_mutex,
         failed_to_create_clientwnd,
         queryRectangles_failed,
         unsupported_colorencoding
      };
};



/**
 * Win-api errors.
 * @description Encapsulates win and gpi errors. Designed to very easily get a decent errordescription.
 * @author      knut st. osmundsen
 */
class kWinError
{
   private:
      ERRORID   errid;
      PERRINFO  pErrInfo;
      BOOL      fErrInfoDelete;
      char     *psz;

      const char *getErrorInfoText(ULONG ulLevel = 0) const;

   protected:
      kWinError  &setError(HAB hab = NULLHANDLE);

   public:
      kWinError(HAB hab = NULLHANDLE, BOOL fInitiate = TRUE);
      kWinError(const kWinError &obj);
      virtual ~kWinError();

      const char *getDescription() const;
      ULONG       getErrorNo() const;
      ULONG       getServerity() const;
};




/**
 * Dos-api errors.
 * @description Encapsulates standard OS/2 (Dos-api) errors. Designed to very easily get a decent errordescription.
 * @author      knut st. osmundsen
 */
class kDosError
{
   private:
      ULONG  ulRc;
      PSZ    psz;

      VOID   prefetchMsg();

   protected:
      kDosError &setError(ULONG ulRc);

   public:
      kDosError();
      kDosError(ULONG ulRc);
      kDosError(const kDosError &obj);
      virtual ~kDosError();
      const char *getDescription() const;
      ULONG       getErrorNo() const;
};



/**
 * Collection of errors.
 * @description Combines all k*Errors classes into one class.
 * @author      knut st. osmundsen
 */
class kError: public kWinError, public kDiveError, public kMyError, public kDosError
{
   private:
      unsigned long  ulErrorType;
      char          *psz;

   public:
      kError(unsigned long ulErrorNo, unsigned long ulErrorType = kError::my);
      kError(const kError &obj);
      ~kError();

      kError        &setError(unsigned long ulErrorNo, unsigned long ulErrorType = kError::my);

      const char    *getDescription() const;
      unsigned long  getErrorNo() const;
      unsigned long  getErrorType() const;

      BOOL           showError(const char *pszTitle) const;
      const kError  &logError(const char *pszFunction, ULONG ulLine) const;
      const kError  &logErrorStr(const char *pszUser, const char *pszFunction, ULONG ulLine) const;

      enum enmErrorType { my, dive, win, dos };
};




/*********/
/* Macro */
/*********/
#define logError() logError(__FUNCTION__, __LINE__)

#endif

