#pragma once

#include <IOperational.hpp>

using IHFDMDefaultCallback
   = std::function<void(std::shared_ptr<std::exception>)>;
using IHFDMDiagnosticCallback
   = std::function<void(Err, std::string)>;
using IHFDMLoginCallback
   = std::function<Err()>;

// Root class making it possible to address the test doubles or wrappers from automation
class IHFDMtrackable : public IOperational
{
   // Construction
public:
   IHFDMtrackable()
   {}
   virtual ~IHFDMtrackable() = 0
   {}

   static inline CallMap& libraryCallMap()
   {
      static CallMap IHFDM_Callmap;
      return IHFDM_Callmap;
   }
};