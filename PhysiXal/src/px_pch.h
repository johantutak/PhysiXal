#pragma once

// From Hazel Engine

#include <memory>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <fstream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <core/core.h>
#include "log/log.h"
#include <events/event.h>
#include "debug/profiler/instrumentor.h"

#ifdef PX_PLATFORM_WINDOWS
	#include <windows.h>
#endif