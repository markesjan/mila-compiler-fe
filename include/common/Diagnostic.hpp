#pragma once

#include <cstddef>
#include <string_view>

#include "SourceLocation.hpp"

namespace milan {

enum class LogLevel { Note, Warning, Error, Debug };

class Diagnostic {
public:
    static void log(LogLevel level, SourceLocation loc, std::string_view msg);

    static void error(std::string_view msg);
    static void error(SourceLocation loc, std::string_view msg);
    static void warning(SourceLocation loc, std::string_view msg);
    static void note(SourceLocation loc, std::string_view msg);

    static bool hasErrors();
    static std::size_t getErrorCount();
    static void reset();

private:
    inline static std::size_t errCnt = 0;
};

} // namespace milan
