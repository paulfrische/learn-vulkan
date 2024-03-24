#include "engine.hpp"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    Engine engine {};

    return 0;
}
