
#ifndef ASSERT_H
#define ASSERT_H

#define M68060_MESSAGE(x) do { printf("%s(%d): %s\n", __FILE__, __LINE__, (x) ); } while (0)
#define M68060_WARNING(x) do { printf("%s(%d): WARNING: %s\n", __FILE__, __LINE__, (x) ); } while (0)
#define M68060_ERROR(x) do { printf("%s(%d): ERROR: %s\n", __FILE__, __LINE__, (x) ); exit(1); } while (0)


#endif
