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

// NamespaceAliases.hpp - abbreviations, if any, for namespaces

#pragma once

// For C++ 17 and above use built in filesystem
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)

#include <filesystem>
namespace fs = std::filesystem;

#else

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#endif

