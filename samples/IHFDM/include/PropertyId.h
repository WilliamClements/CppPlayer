#pragma once

#include <string>
#include <vector>

// Class that identifies a Lynx property set by pairing the property ID string with the path to the property's parent,
// if any, relative to the root property of the repository.
class PropertyId
{
public:
   PropertyId() { }
   explicit PropertyId(std::string id) : path({ id }) { }
   explicit PropertyId(std::vector<std::string> path) : path(path) { }

   std::string getId() const
   {
      if (path.size() > 0)
      {
         return this->path.back();
      }

      return "";
   }
   PropertyId getParentId() const { return PropertyId{ this->getParentPath() }; }
   std::vector<std::string> getAbsolutePath() const { return this->path; }
   std::vector<std::string> getParentPath() const
   {
      if (path.size() > 1)
      {
         return std::vector<std::string>{ this->path.begin(), this->path.end() - 1 };
      }

      return {};
   }

   bool operator<(const PropertyId& other) const
   {
      if (this->path.size() != other.path.size())
      {
         return this->path.size() < other.path.size();
      }

      for (size_t i = 0; i < this->path.size(); i++)
      {
         if (this->path[i] != other.path[i])
         {
            return this->path[i] < other.path[i];
         }
      }

      return false;
   }

private:
   std::vector<std::string> path;
};