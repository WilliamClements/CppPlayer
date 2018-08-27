/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CallStream.hpp

#include "CallMap.hpp"
#include "IIo.hpp"
#include <unordered_map>

class ITrackable;

using Aliased
   = std::unordered_map<std::string, std::shared_ptr<ITrackable>, std::hash<std::string>>;
using AliasedURNs
   = std::unordered_map<std::string, std::string, std::hash<std::string>>;

extern std::unique_ptr<IIo> makeIo();

class CallStream final
{
public:
   CallMap&                          m_callMap;

private:
   std::unique_ptr<IIo>              m_io;
   CppFileHeader                     m_fileheader;
   Aliased                           m_aliased;
   AliasedURNs                       m_aliasedURNs;
   uint64_t                          m_u64CallsCounter;
   uint64_t                          m_u64CallsPreRecorded;
   std::string                       m_mainId;

public:
   CallStream(CallMap& callMap)
      : m_callMap(callMap)
      , m_io()
      , m_u64CallsCounter()
      , m_u64CallsPreRecorded()
      , m_mainId()
   {}

   ~CallStream()
   {}

   IIo& io()                         { return *m_io.get(); }
   Aliased& aliased()                { return m_aliased; }
   AliasedURNs& aliasedURNs()        { return m_aliasedURNs; }
   uint64_t& callsCounter()          { return m_u64CallsCounter; }
   std::string& mainId()             { return m_mainId; }

public:
   bool streaming() const
   {
      return !!m_io;
   }
   void startStreaming()
   {
      m_io = makeIo();
   }
   void startRecording()
   {
      startStreaming();
      m_fileheader.m_startTime = std::chrono::system_clock::now();
      m_io->startRecording();
   }
   void finishRecording(std::string mainId, fs::path outputfilepath)
   {
      if (m_io)
      {
         m_fileheader.m_finishTime = std::chrono::system_clock::now();
         m_fileheader.m_numCallsRecorded = callsCounter();
         m_fileheader.m_mainId = mainId;
         m_io->setFileHeader(m_fileheader);
         m_io->finishRecording(outputfilepath);
      }
   }
   void onStartPlayback()
   {
      m_fileheader = m_io->getFileHeader();
      m_u64CallsPreRecorded = m_fileheader.m_numCallsRecorded;
      m_mainId = m_fileheader.m_mainId;
   }
   void finishPlayback()
   {}
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
