#include <antlr4-runtime.h>

/**
 * Since we are extending ANTLRInputStream, we need to include the same
 * __cplusplus logic as their header. We must match the C++ standard that
 * was used to compile libantlr4-runtime.so.
 *
 * If you get a "undefined reference" error when linking, try setting a
 * different standard when running configure:
 *
 * CXXFLAGS='--std=c++17' ./configure
 *
 * or depending on which one it cannot find...
 *
 * CXXFLAGS='--std=c++14' ./configure
 *
 * Currently, I have antlr4-runtime installed via pacman, and it was compiled
 * with the string_view version.
 */

class UpperStream : public antlr4::ANTLRInputStream
{
      public:
#if __cplusplus >= 201703L
	UpperStream(std::string_view input) : ANTLRInputStream(input) { }
#else
	UpperStream(const std::string& input) : ANTLRInputStream(input) { }
#endif
	UpperStream(std::istream& stream) : ANTLRInputStream(stream) { }
	size_t LA(ssize_t i) override;
};
