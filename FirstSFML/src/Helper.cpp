#include "helper.h"

namespace helper {
	namespace {
		static std::random_device rd;
		static std::mt19937 eng(rd());
	}

	int getRandomInt(int min, int max)
	{
		std::uniform_int_distribution<> distr(min, max);
		return distr(eng);
	}
}