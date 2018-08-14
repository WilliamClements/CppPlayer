/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ITrackable.hpp

#include <atomic>
#include <string>

// Root keyed class 
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

