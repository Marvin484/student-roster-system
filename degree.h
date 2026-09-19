#pragma once
#include <string>

// Enumerated type for degree program (enum class avoids leaking names into global scope)
enum class DegreeProgram { SECURITY, NETWORK, SOFTWARE };

// Parallel array of strings for display purposes
const std::string DEGREE_PROGRAM_NAMES[] = { "SECURITY", "NETWORK", "SOFTWARE" };
