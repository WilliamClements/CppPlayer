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

// IPlayer.hpp - methods, if any, for a playing type of IAgent

#pragma once

#include "IAgent.hpp"

class IPlayer : public IAgent
{
public:
   IPlayer(CallMap& callMap)
      : IAgent{ callMap }
   {}
   virtual ~IPlayer() {}
};


