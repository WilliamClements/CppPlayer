#pragma once

#include <memory>
#include <string>
#include <IHFDMtrackable.h>

class IPropertySet;
class PropertyId;

// Interface to the root property of a Lynx property set repository.
class IRootProperty : public IHFDMtrackable
{
public:
   FORCEINLINE IRootProperty()
   { }
   virtual ~IRootProperty()
   { }

   virtual std::shared_ptr<IPropertySet> getChild(const PropertyId& id) = 0;
   virtual std::shared_ptr<IPropertySet> appendChild(const std::string& typeId, const PropertyId& id) = 0;
   virtual std::shared_ptr<IPropertySet> resolvePath(const PropertyId& id) = 0;
   virtual std::vector<std::string> getDynamicIds() const = 0;
};