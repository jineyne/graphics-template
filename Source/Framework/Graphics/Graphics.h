#pragma once

#include "pch.h"

/** Checks if there have been any OpenGL errors since the last call, and if so reports them. */
void check_gl_error(const char *function, const char *file, int32_t line);

#if DEBUG_MODE
#define CHECK_GL_ERROR() check_gl_error(__PRETTY_FUNCTION__, __FILE__,__LINE__)
#else
#define CHECK_GL_ERROR()
#endif
