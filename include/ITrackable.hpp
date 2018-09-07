/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ITrackable.hpp - keyed root class

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

