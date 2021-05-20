/**
 * Monomial class
 * !!! does not accept power of power       e.g. 4a^2^4
 * !!! does not accept fractions            e.g. a/2
 * !!! does not accept parentheses          e.g. 2(a+b)
 * variables are single ascii characters
 *
 * arithmetic operations can result in a positive or negative zero term
 * destructor removes this Monomial from linked list and connects together, adjacent Monomials from linked list
 */

#ifndef CANONICALPOLYNOMIAL_MONOMIAL_H
#define CANONICALPOLYNOMIAL_MONOMIAL_H
#define CANONICALPOLYNOMIAL_MONOMIAL_DEBUG true

#if CANONICALPOLYNOMIAL_MONOMIAL_DEBUG
#include <iostream>
#endif

class Monomial {
public:
    int variables[256]; // stores ascii value variables
    int degree = 0;
    float coefficient = 1;
    float constant = -1;
    float constant_exponent = -1;
    Monomial * next = nullptr;
    Monomial * prev = nullptr;

    Monomial();

    ~Monomial();

    Monomial(const Monomial & m); // does not copy "next" "prev" pointers from "m"

    inline bool isZero() const { return !coefficient || !constant; }

    inline bool isConstant() const { return constant > -1; }

    bool isLike(const Monomial & m);

    float getConstant() const;

    Monomial operator+(const Monomial & m);

    Monomial operator-(const Monomial & m);

    Monomial operator*(const Monomial & m);

    Monomial & operator=(const Monomial & m); // does not assign "next" "prev" pointers from "m"

    friend std::ostream & operator<<(std::ostream & o, const Monomial & t);

};

#endif // CANONICALPOLYNOMIAL_MONOMIAL_H