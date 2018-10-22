#pragma once

#include <IPropertySet.h>
#include <CppRecorder.hpp>
#include "IPropertySet.h"
//#include <propertysets/properties/reference_property.h>
//#include <propertysets/properties/string_property.h>

// Implementation of the IPropertySet interface that wraps real Lynx property sets.
class PropertySetWrapper : public IPropertySet
{
public:
   PropertySetWrapper(std::shared_ptr<Lynx::PropertySets::BaseProperty> propertySet);
   ~PropertySetWrapper();

   virtual std::shared_ptr<IPropertySet> resolvePath(const std::string& path) override;
   virtual std::shared_ptr<IPrimitiveProperty> resolvePathToPrimitive(const std::string& path) override;
   virtual std::shared_ptr<IReferenceProperty> resolvePathToReference(const std::string& path) override;
   virtual std::shared_ptr<IArrayProperty> resolvePathToArray(const std::string& path) override;

   virtual bool isNull() const override;

   virtual void insert(const std::string& key, std::shared_ptr<IPropertySet> property) override;
   virtual void insertStringValue(const std::string& key, const std::string& value) override;
   virtual void insertBoolValue(const std::string& key, const bool& value) override;
   virtual void insertUint8Value(const std::string& key, const uint8_t& value) override;
   virtual void insertInt16Value(const std::string& key, const int16_t& value) override;
   virtual void insertInt32Value(const std::string& key, const int32_t& value) override;
   virtual void insertInt64Value(const std::string& key, const int64_t& value) override;
   virtual void insertFloatValue(const std::string& key, const float& value) override;
   virtual void insertDoubleValue(const std::string& key, const double& value) override;

   virtual VisitingContinuation visitCollection(const std::function<VisitingContinuation(std::shared_ptr<IPropertySet>)>&) override;

   virtual void remove(const std::string& childId) override;
   virtual void clearStringArray() override;
   virtual void clearBoolArray() override;
   virtual void clearUint8Array() override;
   virtual void clearInt16Array() override;
   virtual void clearInt32Array() override;
   virtual void clearInt64Array() override;
   virtual void clearFloatArray() override;
   virtual void clearDoubleArray() override;
   virtual void clearReferenceArray() override;
   virtual void clearPropertyArray() override;
   virtual void pushArrayValue(std::string typeId) override;
   virtual std::shared_ptr<IPropertySet> getArrayValue(unsigned int index) override;
   virtual std::shared_ptr<IPrimitiveArrayElement> createNextPrimitive() override;
   virtual std::shared_ptr<IReferenceProperty> createNextReference() override;

   virtual std::string                   getTypeId() const override;
   virtual PropertyId                    getId() const override;
   virtual const ARuntimeClass*          getARuntimeClass() const override;
   virtual std::shared_ptr<IPropertySet> getReferencedProperty() const override;
   virtual std::string                   getForeignClassName() const override;

   virtual std::string          getStringValue() const override;
   virtual bool                 getBoolValue() const override;
   virtual uint8_t              getUint8Value() const override;
   virtual int16_t              getInt16Value() const override;
   virtual int32_t              getInt32Value() const override;
   virtual int64_t              getInt64Value() const override;
   virtual float                getFloatValue() const override;
   virtual double               getDoubleValue() const override;

   std::shared_ptr<Lynx::PropertySets::BaseProperty> accessPropertyToAssign() const
   {
      return propertySet;
   }

private:
   std::shared_ptr<Lynx::PropertySets::BaseProperty> propertySet;
   size_t index;
   std::shared_ptr<Lynx::PropertySets::ArrayProperty> propertyArray;
   std::shared_ptr<Lynx::PropertySets::StringArrayProperty> stringArray;
   std::shared_ptr<Lynx::PropertySets::BoolArrayProperty> boolArray;
   std::shared_ptr<Lynx::PropertySets::Uint8ArrayProperty> uint8Array;
   std::shared_ptr<Lynx::PropertySets::Int16ArrayProperty> int16Array;
   std::shared_ptr<Lynx::PropertySets::Int32ArrayProperty> int32Array;
   std::shared_ptr<Lynx::PropertySets::Int64ArrayProperty> int64Array;
   std::shared_ptr<Lynx::PropertySets::Float32ArrayProperty> floatArray;
   std::shared_ptr<Lynx::PropertySets::Float64ArrayProperty> doubleArray;
   std::shared_ptr<Lynx::PropertySets::ReferenceArrayProperty> referenceArray;

   // CppPlayer doubles
public:
   void playback_resolvePath                (const ArgsReader&);
   void playback_resolvePathToPrimitive     (const ArgsReader&);
   void playback_resolvePathToReference     (const ArgsReader&);
   void playback_remove                     (const ArgsReader&);
   void playback_clearStringArray           (const ArgsReader&);
   void playback_clearBoolArray             (const ArgsReader&);
   void playback_clearUint8Array            (const ArgsReader&);
   void playback_clearInt16Array            (const ArgsReader&);
   void playback_clearInt32Array            (const ArgsReader&);
   void playback_clearInt64Array            (const ArgsReader&);
   void playback_clearFloatArray            (const ArgsReader&);
   void playback_clearDoubleArray           (const ArgsReader&);
   void playback_clearReferenceArray        (const ArgsReader&);
   void playback_clearPropertyArray         (const ArgsReader&);
   void playback_pushArrayValue             (const ArgsReader&);
   void playback_getArrayValue              (const ArgsReader&);
   void playback_createNextPrimitive        (const ArgsReader&);
   void playback_createNextReference        (const ArgsReader&);

private:
   AddToMap(PropertySetWrapper, resolvePath);
   AddToMap(PropertySetWrapper, resolvePathToPrimitive);
   AddToMap(PropertySetWrapper, resolvePathToReference);
   AddToMap(PropertySetWrapper, remove);
   AddToMap(PropertySetWrapper, clearStringArray);
   AddToMap(PropertySetWrapper, clearBoolArray);
   AddToMap(PropertySetWrapper, clearUint8Array);
   AddToMap(PropertySetWrapper, clearInt16Array);
   AddToMap(PropertySetWrapper, clearInt32Array);
   AddToMap(PropertySetWrapper, clearInt64Array);
   AddToMap(PropertySetWrapper, clearFloatArray);
   AddToMap(PropertySetWrapper, clearDoubleArray);
   AddToMap(PropertySetWrapper, clearReferenceArray);
   AddToMap(PropertySetWrapper, clearPropertyArray);
   AddToMap(PropertySetWrapper, pushArrayValue);
   AddToMap(PropertySetWrapper, getArrayValue);
   AddToMap(PropertySetWrapper, createNextPrimitive);
   AddToMap(PropertySetWrapper, createNextReference);

   friend class ManagedResource;
};

class ReferencePropertySet : public IReferenceProperty
{
public:
   ReferencePropertySet(std::shared_ptr<Lynx::PropertySets::ReferenceProperty> propertySet) : propertySet(propertySet) { }

   static std::shared_ptr<Lynx::PropertySets::BaseProperty> resolveTargetProperty(
      std::shared_ptr<Lynx::PropertySets::BaseProperty> referenceProperty,
      const PropertyId* propertyId)
   {
      if (!propertyId)
      {
         return nullptr;
      }

      auto targetProperty = referenceProperty->getRoot();
      for (auto pathSegment : propertyId->getAbsolutePath())
      {
         if (!(targetProperty = targetProperty->get(pathSegment)))
            break;
      }
      return targetProperty;
   }

   virtual void setReferenceValue(const PropertyId* propertyId) override
   {
      this->propertySet->set(resolveTargetProperty(this->propertySet, propertyId));
   }

   std::shared_ptr<Lynx::PropertySets::ReferenceProperty> accessReferenceToAssign() const
   {
      return propertySet;
   }

private:
   std::shared_ptr<Lynx::PropertySets::ReferenceProperty> propertySet;
};

class ReferenceArrayElement : public IReferenceProperty
{
public:
   ReferenceArrayElement(std::shared_ptr<Lynx::PropertySets::ReferenceArrayProperty> arrayProperty, size_t index) :
      arrayProperty(arrayProperty),
      index(index)
   { }

   virtual void setReferenceValue(const PropertyId* propertyId) override
   {
      this->arrayProperty->set(this->index, ReferencePropertySet::resolveTargetProperty(this->arrayProperty, propertyId));
   }

   std::shared_ptr<IPropertySet> getReferencedProperty() const
   {
      return std::make_shared<PropertySetWrapper>(this->arrayProperty->get(this->index));
   }

private:
   std::shared_ptr<Lynx::PropertySets::ReferenceArrayProperty> arrayProperty;
   size_t index;
};

class ArrayPropertyWrapper : public IArrayProperty
{
   std::shared_ptr<Lynx::PropertySets::ArrayProperty> propertyArray;
   std::shared_ptr<Lynx::PropertySets::StringArrayProperty> stringArray;
   std::shared_ptr<Lynx::PropertySets::BoolArrayProperty> boolArray;
   std::shared_ptr<Lynx::PropertySets::Uint8ArrayProperty> uint8Array;
   std::shared_ptr<Lynx::PropertySets::Int16ArrayProperty> int16Array;
   std::shared_ptr<Lynx::PropertySets::Int32ArrayProperty> int32Array;
   std::shared_ptr<Lynx::PropertySets::Int64ArrayProperty> int64Array;
   std::shared_ptr<Lynx::PropertySets::Float32ArrayProperty> floatArray;
   std::shared_ptr<Lynx::PropertySets::Float64ArrayProperty> doubleArray;
   std::shared_ptr<Lynx::PropertySets::ReferenceArrayProperty> referenceArray;

public:
   ArrayPropertyWrapper(std::shared_ptr<Lynx::PropertySets::BaseProperty> theArray);

   size_t                       getLength() const override;

   std::string                  getStringValue(size_t) const override;
   bool                         getBoolValue(size_t) const override;
   uint8_t                      getUint8Value(size_t) const override;
   int16_t                      getInt16Value(size_t) const override;
   int32_t                      getInt32Value(size_t) const override;
   int64_t                      getInt64Value(size_t) const override;
   float                        getFloatValue(size_t) const override;
   double                       getDoubleValue(size_t) const override;

   void                         setStringValue(size_t, const std::string& value) override;
   void                         setBoolValue(size_t, const bool& value) override;
   void                         setUint8Value(size_t, const uint8_t& value) override;
   void                         setInt16Value(size_t, const int16_t& value) override;
   void                         setInt32Value(size_t, const int32_t& value) override;
   void                         setInt64Value(size_t, const int64_t& value) override;
   void                         setFloatValue(size_t, const float& value) override;
   void                         setDoubleValue(size_t, const double& value) override;

   virtual std::shared_ptr<IPropertySet>
                                getProperty(size_t) override;
   virtual std::shared_ptr<IReferenceProperty>
                                getReference(size_t) override;
   virtual std::vector<std::string> getIds() const override;

   virtual void                 setProperty(size_t, std::shared_ptr<IPropertySet>) override;
   virtual void                 setReference(size_t, std::shared_ptr<IReferenceProperty>) override;

   // CppPlayer doubles
public:
   void playback_getStringValue             (const ArgsReader&);
   void playback_getBoolValue               (const ArgsReader&);
   void playback_getUint8Value              (const ArgsReader&);
   void playback_getInt16Value              (const ArgsReader&);
   void playback_getInt32Value              (const ArgsReader&);
   void playback_getInt64Value              (const ArgsReader&);
   void playback_getFloatValue              (const ArgsReader&);
   void playback_getDoubleValue             (const ArgsReader&);
   void playback_setStringValue             (const ArgsReader&);
   void playback_setBoolValue               (const ArgsReader&);
   void playback_setUint8Value              (const ArgsReader&);
   void playback_setInt16Value              (const ArgsReader&);
   void playback_setInt32Value              (const ArgsReader&);
   void playback_setInt64Value              (const ArgsReader&);
   void playback_setFloatValue              (const ArgsReader&);
   void playback_setDoubleValue             (const ArgsReader&);

private:
   AddToMap(ArrayPropertyWrapper, getStringValue);
   AddToMap(ArrayPropertyWrapper, getBoolValue);
   AddToMap(ArrayPropertyWrapper, getUint8Value);
   AddToMap(ArrayPropertyWrapper, getInt16Value);
   AddToMap(ArrayPropertyWrapper, getInt32Value);
   AddToMap(ArrayPropertyWrapper, getInt64Value);
   AddToMap(ArrayPropertyWrapper, getFloatValue);
   AddToMap(ArrayPropertyWrapper, getDoubleValue);
   AddToMap(ArrayPropertyWrapper, setStringValue);
   AddToMap(ArrayPropertyWrapper, setBoolValue);
   AddToMap(ArrayPropertyWrapper, setUint8Value);
   AddToMap(ArrayPropertyWrapper, setInt16Value);
   AddToMap(ArrayPropertyWrapper, setInt32Value);
   AddToMap(ArrayPropertyWrapper, setInt64Value);
   AddToMap(ArrayPropertyWrapper, setFloatValue);
   AddToMap(ArrayPropertyWrapper, setDoubleValue);
};

class PrimitivePropertySet : public IPrimitiveProperty
{
public:
   PrimitivePropertySet(std::shared_ptr<Lynx::PropertySets::BaseProperty> propertySet) : propertySet(propertySet) { }

   virtual void setStringValue(const std::string& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::StringProperty>(this->propertySet)->setValue(value);
      recordMethod(capture_setStringValue, value);
   }

   virtual void setBoolValue(const bool& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::BoolProperty>(this->propertySet)->setValue(value);
      recordMethod(capture_setBoolValue, value);
   }

   virtual void setUint8Value(const uint8_t& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Uint8Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setUint8Value, value);
   }

   virtual void setInt16Value(const int16_t& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Int16Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setInt16Value, value);
   }

   virtual void setInt32Value(const int32_t& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Int32Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setInt32Value, value);
   }

   virtual void setInt64Value(const int64_t& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Int64Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setInt64Value, value);
   }

   virtual void setFloatValue(const float& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Float32Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setFloatValue, value);
   }

   virtual void setDoubleValue(const double& value) override
   {
      std::dynamic_pointer_cast<Lynx::PropertySets::Float64Property>(this->propertySet)->setValue(value);
      recordMethod(capture_setDoubleValue, value);
   }

public:
   std::shared_ptr<Lynx::PropertySets::BaseProperty> propertySet;

   // CppPlayer doubles
public:
   void playback_setStringValue             (const ArgsReader&);
   void playback_setBoolValue               (const ArgsReader&);
   void playback_setUint8Value              (const ArgsReader&);
   void playback_setInt16Value              (const ArgsReader&);
   void playback_setInt32Value              (const ArgsReader&);
   void playback_setInt64Value              (const ArgsReader&);
   void playback_setFloatValue              (const ArgsReader&);
   void playback_setDoubleValue             (const ArgsReader&);

private:
   AddToMap(PrimitivePropertySet, setStringValue);
   AddToMap(PrimitivePropertySet, setBoolValue);
   AddToMap(PrimitivePropertySet, setUint8Value);
   AddToMap(PrimitivePropertySet, setInt16Value);
   AddToMap(PrimitivePropertySet, setInt32Value);
   AddToMap(PrimitivePropertySet, setInt64Value);
   AddToMap(PrimitivePropertySet, setFloatValue);
   AddToMap(PrimitivePropertySet, setDoubleValue);
};

