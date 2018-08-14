/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> CppCallError")

// CppCallError.hpp

#if !PERSISTENCEDB_DLL
#include <CRSerror.hpp>
#else
#include <CRSerror.h>
#endif
#include <iostream>
#include <string>

enum CppCallError
{
   CppCallError_UnequalReturnResult
   , CppCallError_DuplicateAPINames
   , CppCallError_NoSuchAPIName
   , CppCallError_StartFlagsProblem
   , CppCallError_FileCannotBeCreated
   , CppCallError_FileDoesNotExist
   , CppCallError_NoSuchTarget
   , CppCallError_NoSuchTrackable
   , CppCallError_WrongNumberOfFields
   , CppCallError_InvalidPath
   , CppCallError_NoSuchURN
   , CppCallError_UnmatchedFulfillVersusReserve
   , CppCallError_TrackableMustBeNull

   // Catch-all, keep last
   , CppCallError_OutOfSequence
};

inline void fail(CppCallError err, std::string ssId)
{
   std::string base = ssId + "/";
   switch (err)
   {
   case CppCallError_UnequalReturnResult:
      throw std::logic_error(base + "result of operation different from recording to playback");
   case CppCallError_DuplicateAPINames:
      throw std::logic_error(base + "only one entry should be registered per apiname");
   case CppCallError_NoSuchAPIName:
      throw std::logic_error(base + "api name lookup failed, did you forget to register a entry for it?");
   case CppCallError_FileCannotBeCreated:
      throw std::logic_error(base + "recording file could not be created");
   case CppCallError_FileDoesNotExist:
      throw std::logic_error(base + "playback file does not exist");
   case CppCallError_NoSuchTarget:
      throw std::logic_error(base + "lookup target failed; i.e. target referenced before it was created");
   case CppCallError_NoSuchTrackable:
      throw std::logic_error(base + "lookup trackable failed; i.e. trackable referenced before it was created");
   case CppCallError_WrongNumberOfFields:
      throw std::logic_error(base + "wrong number of fields pushed or popped");
   case CppCallError_InvalidPath:
      throw std::logic_error(base + "path cannot be correct");
   case CppCallError_NoSuchURN:
      throw std::logic_error(base + "lookup URN failed; i.e. URN referenced before it was created");
   case CppCallError_UnmatchedFulfillVersusReserve:
      throw std::logic_error(base + "tracking mismatch");
   case CppCallError_TrackableMustBeNull:
      throw std::logic_error(base + "non-NULL trackable unexpected");
      // catch all...keep it last
   case CppCallError_OutOfSequence:
      throw std::logic_error(base + "playback sequence did not match recording");
   default:
      throw std::logic_error(base + "please add a description for this error");
   }
}

inline void failUnlessPredicate(bool bPredicate, CppCallError err, std::string ssId)
{
   if (!bPredicate)
      fail(err, ssId);
}

class CppCallInfo final
{
public:
   inline static void reportInfo(Err, std::string ss)
   {
      std::cout << ss.c_str() << std::endl;
   }
   inline static void reportError(const char* operation, const std::shared_ptr<std::exception>& sException)
   {
      const auto* pE = sException.get();
      if (!pE)
      {
         // This should never happen...but if we are crashing,
         // try not to make matters worse
         reportInfo(ERR_SUCCESS, "Lynx: reportError -- Why no std::exception object?\n");
      }
      else
      {
         reportError(operation, *sException);
      }
   }
   inline void static reportError(const char* operation, const std::exception& error)
   {
      std::string ss;
      ss += "Failed to ";
      ss += operation;
      ss += " because ";
      ss += error.what();
      reportInfo(ERR_FAILURE, ss);
   }
};

#pragma message("WOX <- CppCallError")