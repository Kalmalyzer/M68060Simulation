
#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define M68060_MESSAGE(message) do { printf("%s(%d): %s\n", __FILE__, __LINE__, (message) ); } while (0)
#define M68060_WARNING(message) do { printf("%s(%d): WARNING: %s\n", __FILE__, __LINE__, (message) ); } while (0)
#define M68060_ERROR(message) do { printf("%s(%d): ERROR: %s\n", __FILE__, __LINE__, (message) ); exit(1); } while (0)

#define M68060_ASSERT(condition, message) do { if (!(condition)) { printf("%s(%d): ERROR: %s\n", __FILE__, __LINE__, (message) ); exit(1); } } while (0)

#endif
