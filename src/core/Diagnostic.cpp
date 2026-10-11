#include "Diagnostic.hpp"

#include <print>

namespace milan {

void Diagnostic::log(LogLevel level, int row, int col, std::string_view msg) {
    std::string_view color = "\033[0m";
    std::string_view label = "info";

    switch (level) {
        case LogLevel::Error:
            color = "\033[1;31m";
            label = "error";
            ++errCnt;
            break;
        case LogLevel::Warning:
            color = "\033[1;33m";
            label = "warning";
            break;
        case LogLevel::Note:
            color = "\033[1;36m";
            label = "note";
            break;
        case LogLevel::Debug:
            color = "\033[1;35m";
            label = "debug";
            break;
        default:
            break;
    }

    if (row > 0)
        std::println(stderr, "\033[1m[{}:{}]\033[0m {}{}:\033[0m {}", row, col, color, label, msg);
    else
        std::println(stderr, "{}{}:\033[0m {}", color, label, msg);
}

void Diagnostic::error(int row, int col, std::string_view msg) {
    log(LogLevel::Error, row, col, msg);
}

void Diagnostic::warning(int row, int col, std::string_view msg) {
    log(LogLevel::Warning, row, col, msg);
}

void Diagnostic::note(int row, int col, std::string_view msg) {
    log(LogLevel::Note, row, col, msg);
}

bool Diagnostic::hasErrors() {
    return errCnt > 0;
}

int Diagnostic::getErrorCount() {
    return errCnt;
}

void Diagnostic::reset() {
    errCnt = 0;
}

} // namespace milan
