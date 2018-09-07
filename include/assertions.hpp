/*

Copyright 2018 Autodesk

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

// Assertions.hpp - provide Assert function, explain specific error codes

#pragma once

#include <iostream>
#include <string>

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
   , Assertions_TrackableMustBeNull
   , Assertions_UnsupportedArgumentType

   // Catch-all, keep last
   , Assertions_OutOfSequence
};

inline void fail(Assertions err, std::string ssId)
{
   std::string base = ssId + "/";
   switch (err)
   {
   case Assertions_StartFlagsProblem:
      throw std::logic_error(base + "must specify supported flags for recording/playback");
   case Assertions_UnequalReturnResult:
      throw std::logic_error(base + "playback function return different from recording");
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
   case Assertions_TrackableMustBeNull:
      throw std::logic_error(base + "non-NULL trackable unexpected");
      // catch all...keep it last
   case Assertions_OutOfSequence:
      throw std::logic_error(base + "playback sequence did not match recording");
   default:
      throw std::logic_error(base + "please add a description for this error");
   }
}

inline void Assert(bool bPredicate, Assertions err, std::string ssId = "")
{
   if (!bPredicate)
      fail(err, ssId);
}
inline void logPlayerDiagnostic(std::string ss)
{
   std::cout << ss.c_str() << std::endl;
}
inline void logPlayerException(const char* operation, const std::exception& error)
{
   std::string ss;
   ss += "Failed to ";
   ss += operation;
   ss += " because ";
   ss += error.what();
   logPlayerDiagnostic(ss);
}