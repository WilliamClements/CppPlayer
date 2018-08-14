/*
 * Copyright 2018 Autodesk
 */

#pragma once

// Assertions.hpp

#include <iostream>
#include <string>

enum Err;

enum Assertions
{
   Assertions_UnequalReturnResult
   , Assertions_DuplicateAPINames
   , Assertions_NoSuchAPIName
   , Assertions_StartFlagsProblem
   , Assertions_FileCannotBeCreated
   , Assertions_FileDoesNotExist
   , Assertions_NoSuchTarget
   , Assertions_NoSuchTrackable
   , Assertions_WrongNumberOfFields
   , Assertions_InvalidPath
   , Assertions_NoSuchURN
   , Assertions_UnmatchedFulfillVersusReserve
   , Assertions_TrackableMustBeNull

   // Catch-all, keep last
   , Assertions_OutOfSequence
};

inline void fail(Assertions err, std::string ssId)
{
   std::string base = ssId + "/";
   switch (err)
   {
   case Assertions_UnequalReturnResult:
      throw std::logic_error(base + "result of operation different from recording to playback");
   case Assertions_DuplicateAPINames:
      throw std::logic_error(base + "only one entry should be registered per apiname");
   case Assertions_NoSuchAPIName:
      throw std::logic_error(base + "api name lookup failed, did you forget to register a entry for it?");
   case Assertions_FileCannotBeCreated:
      throw std::logic_error(base + "recording file could not be created");
   case Assertions_FileDoesNotExist:
      throw std::logic_error(base + "playback file does not exist");
   case Assertions_NoSuchTarget:
      throw std::logic_error(base + "lookup target failed; i.e. target referenced before it was created");
   case Assertions_NoSuchTrackable:
      throw std::logic_error(base + "lookup trackable failed; i.e. trackable referenced before it was created");
   case Assertions_WrongNumberOfFields:
      throw std::logic_error(base + "wrong number of fields pushed or popped");
   case Assertions_InvalidPath:
      throw std::logic_error(base + "path cannot be correct");
   case Assertions_NoSuchURN:
      throw std::logic_error(base + "lookup URN failed; i.e. URN referenced before it was created");
   case Assertions_UnmatchedFulfillVersusReserve:
      throw std::logic_error(base + "tracking mismatch");
   case Assertions_TrackableMustBeNull:
      throw std::logic_error(base + "non-NULL trackable unexpected");
      // catch all...keep it last
   case Assertions_OutOfSequence:
      throw std::logic_error(base + "playback sequence did not match recording");
   default:
      throw std::logic_error(base + "please add a description for this error");
   }
}

inline void failUnlessPredicate(bool bPredicate, Assertions err, std::string ssId)
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
         reportInfo(Err(0), "Lynx: reportError -- Why no std::exception object?\n");
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
      reportInfo(Err(1), ss);
   }
};
