/*

Copyright 2018 William Clements, Autodesk

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

// CppPlayer.hpp - orchestrate playback

#pragma once

#include "ArgsReader.hpp"
#include "Assertions.hpp"
#include "IPlayer.hpp"
#include "ITrackable.hpp"

class CppPlayer final : public IPlayer
{
public:
   CppPlayer(CallMap& callMap)
      : IPlayer{ callMap }
   {}
     
   void playbackCppCalls(fs::path filepath, std::shared_ptr<ITrackable> pRoot)
   {
      callStream().startStreaming();
      callStream().io().playbackAll(
         filepath
         , [this, pRoot]()
            {
               this->startPlayback(pRoot);
            }
         , [this](int nFields)
            {
               this->playbackOne(nFields);
            });
   }
   void playbackOne(int nFields)
   {
      callStream().popCall();

      ArgsReader ar{ callStream(), nFields };
      // scan
      ar.popHeader();
      // execute
      ar.invoke();
   }

   void startPlayback(std::shared_ptr<ITrackable> pIMain)
   {
      callStream().startPlayback(pIMain);
   }
   void finishPlayback()
   {
      callStream().finishPlayback();
   }
};
