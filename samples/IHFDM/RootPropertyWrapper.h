#pragma once

#include <IRootProperty.h>
#include <CppRecorder.hpp>

namespace Lynx { namespace PropertySets { class NodeProperty; } }

// Implementation of the IRootProperty interface that wraps a real Lynx root property set.
class RootPropertyWrapper : public IRootProperty
{
public:
   RootPropertyWrapper(std::shared_ptr<Lynx::PropertySets::NodeProperty> rootProperty) :
      rootProperty(rootProperty)
   { }

   virtual std::shared_ptr<IPropertySet> getChild(const PropertyId& id) override;
   virtual std::shared_ptr<IPropertySet> appendChild(const std::string& typeId, const PropertyId& id) override;
   virtual std::shared_ptr<IPropertySet> resolvePath(const PropertyId& id) override;
   virtual std::vector<std::string> getDynamicIds() const override;

private:
   std::shared_ptr<Lynx::PropertySets::NodeProperty> rootProperty;

   // CppPlayer doubles
public:
   void playback_getChild                   (const ArgsReader&);
   void playback_appendChild                (const ArgsReader&);
   void playback_rootResolvePath            (const ArgsReader&);
   void playback_getDynamicIds              (const ArgsReader&);

private:
   AddToMap(RootPropertyWrapper, getChild);
   AddToMap(RootPropertyWrapper, appendChild);
   AddToMap(RootPropertyWrapper, rootResolvePath);
   AddToMap(RootPropertyWrapper, getDynamicIds);
};