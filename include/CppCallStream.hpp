/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <GUIDvalue.hpp>
#include <ICppCallIo.hpp>
#include <CppCallIO_Json.hpp>
#include <unordered_map>

class ITrackable;

using Aliased
   = std::unordered_map<std::string, std::shared_ptr<ITrackable>, std::hash<std::string>>;
using AliasedURNs
   = std::unordered_map<std::string, std::string, std::hash<std::string>>;

class CppCallStream final
{
   std::unique_ptr<ICppCallIo>       m_io;
   CppCallFileHeader                 m_fileheader;
   Aliased                           m_aliased;
   AliasedURNs                       m_aliasedURNs;
   uint64_t                          m_u64CallsCounter;
   uint64_t                          m_u64CppCallsPreRecorded;
   std::string                       m_mainId;

public:
   CppCallStream()
      : m_u64CallsCounter()
      , m_u64CppCallsPreRecorded()
      , m_mainId()
      , m_io(makeIo())
   {}

   ~CppCallStream()
   {}

   ICppCallIo& io()                         { return *m_io.get(); }
   Aliased& aliased()                       { return m_aliased; }
   AliasedURNs& aliasedURNs()               { return m_aliasedURNs; }
   uint64_t& callsCounter()                 { return m_u64CallsCounter; }
   std::string& mainId()                    { return m_mainId; }

public:
   void startRecording()
   {
      m_fileheader.m_startTime = std::chrono::system_clock::now();
      m_io->startRecording();
   }
   void finishRecording(std::string mainId, std::string outputfilename)
   {
      m_fileheader.m_finishTime = std::chrono::system_clock::now();
      m_fileheader.m_numCppCallsRecorded = callsCounter();
      m_fileheader.m_mainId = mainId;
      m_io->setFileHeader(m_fileheader);
      m_io->finishRecording(outputfilename);
   }
   void onStartPlayback()
   {
      m_fileheader.m_startTime = std::chrono::system_clock::now();
      m_io->startRecording();
   }
   void finishPlayback()
   {}
   static std::unique_ptr<ICppCallIo> makeIo()
   {
      return std::make_unique<CppCallIO_Json>();
   }
   std::string breakStringVector(const std::vector<std::string>& vs)
   {
      std::string ret = "{";
      bool bFirst = true;
      for (const auto& id : vs)
      {
         if (!bFirst)
            ret += ",";
         ret += id;
         bFirst = false;
      }
      ret += "}";
      return ret;
   }
   std::vector<std::string> recomposeStringVector(std::string ss)
   {
      std::vector<std::string> ret;
      size_t totalLength = ss.size();
      size_t currentPos = 0;
      while ((currentPos < totalLength) && ('}' != ss[currentPos]))
      {
         if (0 == currentPos)
         {
            if ('{' != ss[currentPos])
               break;
            ++currentPos;
         }
         else
         {
            size_t nextPos = ss.find_first_of(",}", currentPos);
            if (std::string::npos == nextPos)
               break;
            ret.push_back(ss.substr(currentPos, nextPos - currentPos));
            currentPos = nextPos + 1;
         }
      }
      return ret;
   }
};

void fail(CppCallError err, const CppCallMapEntry* pEntry)
{
   fail(err, pEntry->api());
}

void fail(CppCallError err, std::string ssId)
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