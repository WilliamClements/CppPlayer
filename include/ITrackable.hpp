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

// ITrackable.hpp - keyed root class

#pragma once

#include <atomic>
#include <string>

template<class ITarget>
class CppCall;

class ITrackable
{
   // Construction
public:
   ITrackable()
   {
      static std::atomic_uint64_t serial = 1000000;
      objectKey = std::to_string(++serial);
   }
   virtual ~ITrackable() = 0
   {}

   // Attributes
   std::string objectKey;
};

