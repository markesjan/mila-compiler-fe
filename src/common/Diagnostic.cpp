#include "Diagnostic.hpp"

#include <print>

#include "SourceLocation.hpp"

namespace milan {

void Diagnostic::log(LogLevel level, SourceLocation loc, std::string_view msg) {
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

    if (loc.line > 0)
        std::println(stderr, "\033[1m[{}:{}]\033[0m {}{}:\033[0m {}", loc.line, loc.column, color,
                     label, msg);
    else
        std::println(stderr, "{}{}:\033[0m {}", color, label, msg);
}

void Diagnostic::error(std::string_view msg) {
    error(SourceLocation(0, 0), msg);
}

void Diagnostic::error(SourceLocation loc, std::string_view msg) {
    log(LogLevel::Error, loc, msg);
}

void Diagnostic::warning(SourceLocation loc, std::string_view msg) {
    log(LogLevel::Warning, loc, msg);
}

void Diagnostic::note(SourceLocation loc, std::string_view msg) {
    log(LogLevel::Note, loc, msg);
}

bool Diagnostic::hasErrors() {
    return errCnt > 0;
}

std::size_t Diagnostic::getErrorCount() {
    return errCnt;
}

void Diagnostic::reset() {
    errCnt = 0;
}

} // namespace milan
