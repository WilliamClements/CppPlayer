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

// CallStream.hpp - abstraction for streaming calls in or out, including pointer swizzling

#pragma once

#include "CallMap.hpp"
#include "IIo.hpp"
#include <unordered_map>

class ITrackable;

using Swizzled
   = std::unordered_map<std::string, std::shared_ptr<ITrackable>, std::hash<std::string>>;
using URNbindings
   = std::unordered_map<std::string, std::string, std::hash<std::string>>;

extern std::unique_ptr<IIo> makeIo();

class CallStream final
{
public:
   CallMap&                          m_callMap;

private:
   uint64_t                          m_u64CallsCounter = 0;
   URNbindings                       m_urnBindings;
   std::string                       m_mainId;
   std::unique_ptr<IIo>              m_io;
   CppFileHeader                     m_fileheader;
   Swizzled                          m_Swizzled;
   uint64_t                          m_u64CallsPreRecorded = 0;

public:
   CallStream(CallMap& callMap)
      : m_callMap(callMap)
   {}
   ~CallStream()
   {}

   IIo& io()                         { return *m_io.get(); }

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
   void bindURN(std::string liveURN, std::string streamURN)
   {
      // empty string maps to empty string
      if (!liveURN.empty())
      {
         // If create a repository on playback, branch URN will not be the same. Account for that.
         m_urnBindings[streamURN] = liveURN;
      }
   }
   std::string unbindURN(std::string streamURN)
   {
      // empty string maps to empty string
      if (streamURN.empty())
         return streamURN;
      // We need the live branch URN given original branch URN from file.
      auto live = m_urnBindings.find(streamURN);
      Assert(m_urnBindings.end() != live, Assertions_NoSuchURN);
      return live->second;
   }
   void unswizzle(const std::string objectKey, const std::shared_ptr<ITrackable> pTrackable)
   {
      // Associate key from file with corresponding live object just created.
      m_Swizzled[objectKey] = pTrackable;
   }
   std::shared_ptr<ITrackable> swizzle(std::string swizzleKey) const
   {
      // Get live object given key from file. Don't throw. Let error be forced by caller.
      std::shared_ptr<ITrackable> ret;
      auto live = m_Swizzled.find(swizzleKey);
      if (m_Swizzled.end() != live)
         ret = live->second;
      return ret;
   }
   void pushCall()
   {
      io().pushCurrentCall();
      ++m_u64CallsCounter;
   }
   void popCall()
   {
      ++m_u64CallsCounter;
   }
   void finishRecording(std::string mainId, fs::path outputfilepath)
   {
      if (m_io)
      {
         m_fileheader.m_finishTime = std::chrono::system_clock::now();
         m_fileheader.m_numCallsRecorded = m_u64CallsCounter;
         m_fileheader.m_mainId = mainId;
         m_io->setFileHeader(m_fileheader);
         m_io->finishRecording(outputfilepath);
      }
   }
   void startPlayback(std::shared_ptr<ITrackable> pIMain)
   {
      logPlayerDiagnostic("entered startPlayback");

      m_fileheader = m_io->getFileHeader();
      m_u64CallsPreRecorded = m_fileheader.m_numCallsRecorded;
      m_mainId = m_fileheader.m_mainId;
      unswizzle(m_mainId, pIMain);
   }
   void finishPlayback()
   {
      logPlayerDiagnostic("entered finishPlayback");
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
