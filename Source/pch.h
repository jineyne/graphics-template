#pragma once

#if defined (_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#include <cassert>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdarg>

#include <memory>

#include <any>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>
#include <tuple>

#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>
#include <limits>
#include <iterator>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <float.h>

extern "C" {
#   include <sys/types.h>
#   include <sys/stat.h>
}

#include <spdlog/spdlog.h>
#include <FreeImagePlus.h>
#include <alc.h>


#define DEFINE_THIS_PTR(TYPE) \
    private: std::weak_ptr<TYPE> _thisPtr; \
    protected: void setThisPtr(const std::shared_ptr<TYPE> &ptr) { _thisPtr = ptr; } \
    public: std::shared_ptr<TYPE> getThisPtr() const { return _thisPtr.lock(); }
