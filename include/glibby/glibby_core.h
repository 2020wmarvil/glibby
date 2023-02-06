#pragma once

#include <iostream>

#ifdef GLIBBY_DEBUG
	#define LOG_INFO(msg)    std::cerr << "\033[36m" << "INFO: "     << msg << "\033[0m" << std::endl;
	#define LOG_WARNING(msg) std::cerr << "\033[33m" << "WARNING: "  << msg << "\033[0m" << std::endl;
	#define LOG_ERROR(msg)   std::cerr << "\033[31m" << "ERROR: "    << msg << "\033[0m" << std::endl;
	#define ASSERT(eval, msg) if (!(eval)) LOG_ERROR(msg)
#else
	#define LOG_INFO(msg)
	#define LOG_WARNING(msg)
	#define LOG_ERROR(msg)
	#define ASSERT(eval, msg)
#endif
