/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallError.hpp

class CppCallMapEntry;

enum CppCallError
{
   CppCallError_UnequalReturnResult
   , CppCallError_DuplicateAPINames
   , CppCallError_NoSuchAPIName
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

void fail(CppCallError err, const CppCallMapEntry* pEntry = nullptr);
void fail(CppCallError err, std::string ssId);

inline void failUnlessPredicate(bool bPredicate, CppCallError err, const CppCallMapEntry* pEntry = nullptr)
{
   if (!bPredicate)
      fail(err, pEntry);
}
inline void failUnlessPredicate(bool bPredicate, CppCallError err, std::string ssId)
{
   if (!bPredicate)
      fail(err, ssId);
}

