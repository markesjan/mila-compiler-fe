#pragma once

#include <string_view>

namespace milan {

enum class LogLevel {
    Note,
    Warning,
    Error,
    Debug
};

class Diagnostic {
public:
    static void log(LogLevel level, int row, int col, std::string_view msg);

    static void error(int row, int col, std::string_view msg);
    static void warning(int row, int col, std::string_view msg);
    static void note(int row, int col, std::string_view msg);

    static bool hasErrors();
    static int getErrorCount();
    static void reset();

private:
    inline static int errCnt = 0;
};

} // namespace milan
