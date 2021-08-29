#pragma once

#pragma warning(disable: 4244)
#pragma warning(disable: 26812)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <cassert>
#include <mutex>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <thread>
#include <cstdarg>
#include <unordered_set>
#include <csetjmp>
#include <deque>
#include <optional>
#include <map>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <MinHook.h>
#include <gsl/gsl>

using namespace std::literals;