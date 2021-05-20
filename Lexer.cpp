#include "Lexer.h"

Lexer::Lexer(std::string inFile) {
    inputFile.open(inFile, std::ifstream::in);
}

Lexer::~Lexer() {
    if (inputFile.is_open()) inputFile.close();
}

/**
 * returns next state of the finite automaton given the current state and input char
 * uses switch conditionals for state determination instead of 2D array
 */

Lexer::State Lexer::stateTransition(State s, char i_c) {
    switch (s) {
        case State::PositiveStart:
            if (isdigit(i_c)) // positive integer or float
                return State::Integer;
            else if (isalpha(i_c)) // positive variable
                return State::Variable;
            else if (i_c == '+') // positive start
                return State::PositiveStart;
            else if (i_c == '-') // negative start
                return State::NegativeStart;
            else if (i_c == '.') // begin float
                return State::Dot;
            else
                return State::Undefined;
        case State::NegativeStart:
            if (isdigit(i_c)) // negative integer or float
                return State::Integer;
            else if (isalpha(i_c)) // negative variable
                return State::Variable;
            else if (i_c == '.') // negative float
                return State::Dot;
            else
                return State::Undefined;
        case State::Integer:
            if (isdigit(i_c)) // integer
                return State::Integer;
            else if (isalpha(i_c)) // variable with integer coefficient
                return State::Variable;
            else if (i_c == '.') // begin float
                return State::Dot;
            else if (i_c == '^') // begin integer or float exponent with integer base
                return State::Exponent;
            else
                return State::Undefined;
        case State::Dot:
            if (isdigit(i_c)) // float
                return State::Float;
            else
                return State::Undefined;
        case State::Float:
            if (isdigit(i_c)) // float
                return State::Float;
            else if (isalpha(i_c)) // variable with float coefficient
                return State::Variable;
            else if (i_c == '^') // begin integer or float exponent with float base
                return State::Exponent;
            else
                return State::Undefined;
        case State::Exponent:
            if (isdigit(i_c)) // integer or float exponent with constant base
                return State::ExponentInteger;
            else if (i_c == '.') // begin float exponent with constant base
                return State::ExponentDot;
            else
                return State::Undefined;
        case State::ExponentInteger:
            if (isdigit(i_c)) // integer or float exponent with constant base
                return State::ExponentInteger;
            else if (i_c == '.') // float exponent with constant base
                return State::ExponentDot;
            else
                return State::Undefined;
        case State::ExponentDot:
            if (isdigit(i_c)) // float exponent with constant base
                return State::ExponentFloat;
            else
                return State::Undefined;
        case State::ExponentFloat:
            if (isdigit(i_c)) // float exponent with constant base
                return State::ExponentFloat;
            else
                return State::Undefined;
        case State::Variable:
            if (isalpha(i_c)) // variable
                return State::Variable;
            else if (i_c == '^') // begin integer exponent with variable base
                return State::VariableExponent;
            else
                return State::Undefined;
        case State::VariableExponent:
            if (isdigit(i_c)) // integer exponent with variable base
                return State::VariableExponentInteger;
            else
                return State::Undefined;
        case State::VariableExponentInteger:
            if (isalpha(i_c)) // variable
                return State::Variable;
            else if (isdigit(i_c)) // integer exponent with variable base
                return State::VariableExponentInteger;
            else
                return State::Undefined;
        default:
            return State::Undefined;
    }
}

/**
 * This is the driver of the finite automaton
 * return  1    if a valid Term is found, assigns it to "m"
 * return  0    if an invalid character is found, assign it to "token"
 * return -1    if a valid Term is found at end-of-line, assigns it to "m"
 * return -2    if an invalid character is found at end-of-line, assigns it to "token"
 * return -3    if end-of-stream is reached
 **/

int Lexer::driver() {
    State nextState;
    token = ""; // reset token
    currentState = State::PositiveStart; // reset state
    m = Monomial(); // reset Monomial term

    while (cValue == -1 || isspace(cValue)) { // get next non-whitespace character
        cValue = getChar();

        if (cValue == -2) return -3; // reached end-of-stream
    }

    while (cValue > -1) { // while not reached end-of-stream
        c = (char) cValue;
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "next:-------------------\t" << c << std::endl;
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "token:------------------\t" << token << std::endl;
        nextState = stateTransition(currentState, c);
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "state:\t\t" << static_cast<int>(currentState) << " -> " << static_cast<int>(nextState) << std::endl;
        switch (nextState) { // check next stage
            case State::Undefined: // FA will halt
                if (isFinal(currentState)) { // halted in accepted final state
                    updateTerm(nextState);
                    if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                        std::cout << "finished term:\t" << m << std::endl << "-----" << std::endl << std::endl;
                    return 1;
                } else { // "c" is unexpected value
                    token = token + c;
                    if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                        std::cout << "invalid token:\t" << token << std::endl << "-----" << std::endl << std::endl;
                    cValue = getChar();
                    return 0; // invalid token found
                }
            case State::Exponent: // update constant and reset token
                if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                    std::cout << "constant: " << token << std::endl;
                m.constant = stod(token); // convert string to double for constant
                token = "";
                currentState = nextState;
                cValue = getChar();
                break;
            case State::Variable: // update coefficient, variable, and token
                updateTerm(nextState);
                m.variables[cValue]++; // increment variable once before checking its exponent
                token = c;
                if (m.variables[token[0]] > m.degree) {
                    m.degree = m.variables[token[0]]; // update degree
                    if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                        std::cout << "degree:\t\t" << m.degree << std::endl;
                }
                currentState = nextState;
                cValue = getChar();
                break;
            case State::NegativeStart: // negative term
                m.coefficient = -1;
                currentState = nextState;
                cValue = getChar();
                break;
            case State::PositiveStart: // reset token
                currentState = nextState;
                cValue = getChar();
                break;
            case State::VariableExponent: // continue FA
                currentState = nextState;
                cValue = getChar();
                break;
            default: // update token
                currentState = nextState;
                token = token + c;
                cValue = getChar();
        }
    }

    // end-of-line is reached while a token is being extracted
    if (isFinal(currentState)) { // halted in accepted final state
        updateTerm(currentState);
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "finished term:\t" << m << std::endl << "-----" << std::endl << std::endl;
        if (cValue == -2) return -3; // end-of-stream is reached
        return -1; // valid token extracted at end-of-line
    } else { // "c" is unexpected value
        token = token + c;
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "invalid token:\t" << token << std::endl << "-----" << std::endl << std::endl;
        if (cValue == -2) return -3; // end-of-stream is reached
        return -2; // invalid token found at end-of-line
    }
} // end driver

/**
 * returns integer value of next character from file stream
 * returns -1 if file reached end-of-line
 * returns -2 if file reached end-of-stream
 */

int Lexer::getChar() {
    char ascii;
    if (inputFile.is_open()) {
        ascii = inputFile.get();
        if (ascii == '\n') return -1;
        if (inputFile.eof()) return -2;
        if (ignoreWhitespace) { // skip whitespaces
            while (isspace(ascii)) {
                ascii = inputFile.get();
                if (ascii == '\n') return -1;
                if (inputFile.eof()) return -2;
            }
        }
        return (int) ascii;
    }
    return -2; // input file is not open
}

void Lexer::updateTerm(State state) {
    if (currentState == State::VariableExponentInteger) { // token contains variable with non-zero degree
        int variableExponent = stod(token.substr(1, token.length())) - 1;
        m.variables[token[0]] += variableExponent; // increment variable by its exponent - 1
        if (m.variables[token[0]] > m.degree) {
            m.degree = m.variables[token[0]]; // update degree value
            if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                std::cout << "degree:\t\t" << m.degree << std::endl;
        }
        if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
            std::cout << "variable:\t" << token[0] << "^" << variableExponent + 1 << std::endl;
    } else if (currentState == State::Integer || currentState == State::Float) { // token contains constant
        if (state == State::Variable) {
            m.coefficient = m.coefficient * stod(token); // update coefficient value
            if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                std::cout << "coefficient:\t" << m.coefficient << std::endl;
        } else {
            m.constant = stod(token); // update constant value
            if (CANONICALPOLYNOMIAL_LEXER_DEBUG)
                std::cout << "constant:\t" << token << std::endl;
        }
    }
}