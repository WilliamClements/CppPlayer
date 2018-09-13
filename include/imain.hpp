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

// IMain.hpp -- sets up infrastructure for playing or recording

#pragma once

#include "CppPlayer.hpp"
#include "CppRecorder.hpp"
#include "IOperational.hpp"
#include "NamespaceAliases.hpp"

enum IMain_Start_Flags
{
   Do_Execution = 0x1
   , Do_Recording = 0x2
   , Do_Playback = 0x4
};

class IMain : public IOperational
{
protected:
   CallMap&                            m_callMap;
   std::shared_ptr<CppPlayer>          Player;
   std::shared_ptr<CppRecorder>        Recorder;
   fs::path                            FilePath;

   // Construction
public:
   IMain(CallMap& callMap)
      : m_callMap(callMap)
      , Player()
      , Recorder()
   {
      Player = std::make_shared<CppPlayer>(m_callMap);
      Recorder = std::make_shared<CppRecorder>(m_callMap);
   }
   virtual ~IMain()
   {
      m_callMap.clear();
   }

   unsigned int& StartFlags()
   {
      return m_callMap.StartFlags;
   }
   void startCpp(unsigned int nFlags)
   {
      StartFlags() = nFlags;
      m_callMap.onStart(Player, Recorder);
   }
   void recordCpp(std::string filename)
   {
      Assert(
         !!(Do_Recording & StartFlags())
         , Assertions::StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      Assert(!bExists, Assertions::FileCannotBeCreated);

      Recorder->startRecording(FilePath);
   }
   void playbackCpp(std::string filename)
   {
      Assert(
         !!(Do_Playback & StartFlags())
         , Assertions::StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      Assert(bExists, Assertions::FileDoesNotExist);

      Player->playbackCppCalls(FilePath, shared_from_this());
   }
   void finishCpp()
   {
      Recorder->finishRecording(objectKey);
      Player->finishPlayback();
   }
};
