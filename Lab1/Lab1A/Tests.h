#pragma once

#include <stdexcept>
#include <string>
#include <iostream>

void require(bool condition, const std::string& name);

void runTests();
