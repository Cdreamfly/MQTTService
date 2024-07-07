#pragma once

#include <functional>
#include <string>

namespace cm{
	using MessageCallback = std::function<void(const std::string&)>;
	using ConnectionCallback = std::function<void(int)>;
}