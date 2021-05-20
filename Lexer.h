/**
 * Lexer class creates "Monomial" terms using a deterministic finite automaton which:
 * !!! does not accept power of power       e.g. 4a^2^4
 * !!! does not accept fractions            e.g. a/2
 * !!! does not accept parentheses          e.g. 2(a+b)
 * state transitions are done using conditional statements
 */

#ifndef CANONICALPOLYNOMIAL_LEXER_H
#define CANONICALPOLYNOMIAL_LEXER_H
#define CANONICALPOLYNOMIAL_LEXER_DEBUG true

#if CANONICALPOLYNOMIAL_LEXER_DEBUG
#include <iostream>
#endif

#include <fstream>
#include "Monomial.h"

class Lexer {
private:
    // states of DFA
    enum class State {
        // non-final states                                                 // state ordinal number
        PositiveStart, // start state                                       // 0
        NegativeStart,                                                      // 1
        Dot, // begin float                                                 // 2
        Exponent, // begin exponent for integer                             // 3
        ExponentDot, // begin float exponent for integer                    // 4
        VariableExponent, // begin exponent for variable                    // 5

        // final states
        Integer,                                                            // 6
        Float,                                                              // 7
        Variable,                                                           // 8
        ExponentInteger, // integer exponent                                // 9
        ExponentFloat, // float exponent                                    // 10
        VariableExponentInteger, // integer exponent for variable           // 11
        Undefined                                                           // 12
    };

public:
/**
 *  "cValue" is the current status of driver
 *  positive    indicates normal status
 *  -1          indicates end-of-line status
 *  -2          indicates end-of-stream status
 */

    int cValue = -1;

    char c;

    std::string token;

    Monomial m;

    std::ifstream inputFile;

    State currentState;

    bool ignoreWhitespace = false; // set false if terms can be separated by whitespace

    Lexer(std::string inFile);

    ~Lexer();

    inline static bool isFinal(State s) { return s > State::VariableExponent; }

    static State stateTransition(State s, char i_c);

/**
 * Driver of the FA
 * return  1    if a valid Term is found, assigns it to "m"
 * return  0    if an invalid character is found, assign it to "token"
 * return -1    if a valid Term is found at end-of-line, assigns it to "m"
 * return -2    if an invalid character is found at end-of-line, assigns it to "token"
 * return -3    if end-of-stream is reached
 **/

    int driver();

    int getChar();

    void updateTerm(State state);

};

#endif //CANONICALPOLYNOMIAL_LEXER_H