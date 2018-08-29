#pragma once

#include <IHFDMtrackable.h>
#include <CppRecorder.hpp>
#include <memory>
#include <PropertyId.h>
#include <string>

class ARuntimeClass;
class IArrayProperty;

// Interface to a standalone Lynx PropertySet primitive. 
class IPrimitiveProperty : public IHFDMtrackable
{
public:
   FORCEINLINE IPrimitiveProperty()
   { }
   virtual ~IPrimitiveProperty()
   { }

   virtual void setStringValue(const std::string& value) = 0;
   virtual void setBoolValue(const bool& value) = 0;
   virtual void setUint8Value(const uint8_t& value) = 0;
   virtual void setInt16Value(const int16_t& value) = 0;
   virtual void setInt32Value(const int32_t& value) = 0;
   virtual void setInt64Value(const int64_t& value) = 0;
   virtual void setFloatValue(const float& value) = 0;
   virtual void setDoubleValue(const double& value) = 0;
};

// Interface to a Lynx PropertySet primitive that is part of an array.
class IPrimitiveArrayElement : public IHFDMtrackable
{
public:
   FORCEINLINE IPrimitiveArrayElement()
   { }
   virtual ~IPrimitiveArrayElement()
   { }

   virtual void pushBackStringValue(const std::string& value) = 0;
   virtual void pushBackBoolValue(const bool& value) = 0;
   virtual void pushBackUint8Value(const uint8_t& value) = 0;
   virtual void pushBackInt16Value(const int16_t& value) = 0;
   virtual void pushBackInt32Value(const int32_t& value) = 0;
   virtual void pushBackInt64Value(const int64_t& value) = 0;
   virtual void pushBackFloatValue(const float& value) = 0;
   virtual void pushBackDoubleValue(const double& value) = 0;

   void accumulateStringValue(const std::string& value)
   {
      this->stringAccumulator.append(value);
   }

   void commitStringValue()
   {
      this->pushBackStringValue(this->stringAccumulator);
      this->stringAccumulator.clear();
   }

private:
   std::string stringAccumulator;
};

// Interface to a Lynx Reference property. This interface might wrap either a standalone 
// Reference property or an element in a Reference property array.
class IReferenceProperty : public IHFDMtrackable
{
public:
   FORCEINLINE IReferenceProperty()
   { }
   virtual ~IReferenceProperty()
   { }

   virtual void setReferenceValue(const PropertyId* propertyId) = 0;
};

// Interface to a Lynx property set. The underlying implementation might be an array
// property or an individual base property.
class IPropertySet : public IHFDMtrackable
{
public:
   FORCEINLINE IPropertySet()
   { }
   virtual ~IPropertySet()
   { }

   virtual std::shared_ptr<IPropertySet> resolvePath(const std::string& path) = 0;
   virtual std::shared_ptr<IPrimitiveProperty> resolvePathToPrimitive(const std::string& path) = 0;
   virtual std::shared_ptr<IReferenceProperty> resolvePathToReference(const std::string& path) = 0;
   virtual std::shared_ptr<IArrayProperty> resolvePathToArray(const std::string& path) = 0;

   virtual bool isNull() const = 0;

   virtual void insert(const std::string& key, std::shared_ptr<IPropertySet> property) = 0;
   virtual void insertStringValue(const std::string& key, const std::string& value) = 0;
   virtual void insertBoolValue(const std::string& key, const bool& value) = 0;
   virtual void insertUint8Value(const std::string& key, const uint8_t& value) = 0;
   virtual void insertInt16Value(const std::string& key, const int16_t& value) = 0;
   virtual void insertInt32Value(const std::string& key, const int32_t& value) = 0;
   virtual void insertInt64Value(const std::string& key, const int64_t& value) = 0;
   virtual void insertFloatValue(const std::string& key, const float& value) = 0;
   virtual void insertDoubleValue(const std::string& key, const double& value) = 0;

   virtual VisitingContinuation visitCollection(const std::function<VisitingContinuation(std::shared_ptr<IPropertySet>)>&) = 0;

   virtual void remove(const std::string& childId) = 0;
   virtual void clearStringArray() = 0;
   virtual void clearBoolArray() = 0;
   virtual void clearUint8Array() = 0;
   virtual void clearInt16Array() = 0;
   virtual void clearInt32Array() = 0;
   virtual void clearInt64Array() = 0;
   virtual void clearFloatArray() = 0;
   virtual void clearDoubleArray() = 0;
   virtual void clearReferenceArray() = 0;
   virtual void clearPropertyArray() = 0;
   virtual void pushArrayValue(std::string typeId) = 0;
   virtual std::shared_ptr<IPropertySet> getArrayValue(unsigned int index) = 0;
   virtual std::shared_ptr<IPrimitiveArrayElement> createNextPrimitive() = 0;
   virtual std::shared_ptr<IReferenceProperty> createNextReference() = 0;

   virtual std::string                   getTypeId() const = 0;
   virtual PropertyId                    getId() const = 0;
   virtual const ARuntimeClass*          getARuntimeClass() const = 0;
   virtual std::shared_ptr<IPropertySet> getReferencedProperty() const = 0;
   virtual std::string                   getForeignClassName() const = 0;

   virtual std::string          getStringValue() const = 0;
   virtual bool                 getBoolValue() const = 0;
   virtual uint8_t              getUint8Value() const = 0;
   virtual int16_t              getInt16Value() const = 0;
   virtual int32_t              getInt32Value() const = 0;
   virtual int64_t              getInt64Value() const = 0;
   virtual float                getFloatValue() const = 0;
   virtual double               getDoubleValue() const = 0;
};

// Interface to a Lynx array property. It supports random access.
class IArrayProperty : public IHFDMtrackable
{
public:
   virtual size_t               getLength() const = 0;

   virtual std::string          getStringValue(size_t) const = 0;
   virtual bool                 getBoolValue(size_t) const = 0;
   virtual uint8_t              getUint8Value(size_t) const = 0;
   virtual int16_t              getInt16Value(size_t) const = 0;
   virtual int32_t              getInt32Value(size_t) const = 0;
   virtual int64_t              getInt64Value(size_t) const = 0;
   virtual float                getFloatValue(size_t) const = 0;
   virtual double               getDoubleValue(size_t) const = 0;

   virtual void                 setStringValue(size_t, const std::string& value) = 0;
   virtual void                 setBoolValue(size_t, const bool& value) = 0;
   virtual void                 setUint8Value(size_t, const uint8_t& value) = 0;
   virtual void                 setInt16Value(size_t, const int16_t& value) = 0;
   virtual void                 setInt32Value(size_t, const int32_t& value) = 0;
   virtual void                 setInt64Value(size_t, const int64_t& value) = 0;
   virtual void                 setFloatValue(size_t, const float& value) = 0;
   virtual void                 setDoubleValue(size_t, const double& value) = 0;

   virtual std::shared_ptr<IPropertySet>
                                getProperty(size_t) = 0;
   virtual std::shared_ptr<IReferenceProperty>
                                getReference(size_t) = 0;
   virtual std::vector<std::string> getIds() const = 0;

   virtual void                 setProperty(size_t, std::shared_ptr<IPropertySet>) = 0;
   virtual void                 setReference(size_t, std::shared_ptr<IReferenceProperty>) = 0;
};