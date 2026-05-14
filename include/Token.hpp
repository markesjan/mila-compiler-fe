#pragma once

namespace milan {

enum Token {
    tok_eof = -1,
    tok_error = -41,

    // numbers and identifiers
    tok_identifier = -2,
    tok_number = -3,

    // keywords
    tok_begin = -4,
    tok_end = -5,
    tok_const = -6,
    tok_procedure = -7,
    tok_forward = -8,
    tok_function = -9,
    tok_if = -10,
    tok_then = -11,
    tok_else = -12,
    tok_program = -13,
    tok_while = -14,
    tok_exit = -15,
    tok_var = -16,
    tok_integer = -17,
    tok_for = -18,
    tok_do = -19,

    // 2-character operators
    tok_notequal = -20,
    tok_lessequal = -21,
    tok_greaterequal = -22,
    tok_assign = -23,
    tok_or = -24,

    // 3-character operators (keywords)
    tok_mod = -25,
    tok_div = -26,
    tok_not = -27,
    tok_and = -28,
    tok_xor = -29,

    // keywords in for loop
    tok_to = -30,
    tok_downto = -31,

    // keywords for array
    tok_array = -32,
    tok_of = -33,
    tok_range = -34,

    // extra keywords for bonuses
    tok_break = -35,
    tok_continue = -36,
    tok_boolean = -37,
    tok_true = -38,
    tok_false = -39,
    tok_real = -40
};

} // namespace milan
