#include "Monomial.h"

Monomial::Monomial() {
    for (int i = 0; i < 256; i++) {
        variables[i] = 0;
    }
}

Monomial::~Monomial() {
    if (prev != nullptr) prev->next = next;
    if (next != nullptr) next->prev = prev;
}

Monomial::Monomial(const Monomial & m) {
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) copy constructor\tstart" << std::endl;
    std::copy(std::begin(m.variables), std::end(m.variables), std::begin(variables)); // copy "m" variables array
    degree = m.degree;
    coefficient = m.coefficient;
    constant = m.constant;
    constant_exponent = m.constant_exponent;
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) copy constructor\tend\t" << * this << std::endl;
}

// returns true if this and "m" are like terms
bool Monomial::isLike(const Monomial & m) {
    if (degree != m.degree) return false; // base case
    if (isConstant() && m.isConstant()) return true; // like terms with same degree of 0
    for (int c = 0; c < 256; c++) { // check if each variable has same degree
        if (variables[c] != m.variables[c]) return false;
    }
    return true;
}

// return simplified constant
float Monomial::getConstant() const {
    if (constant == -1) return -1; // no constant
    if (constant_exponent < 0) return constant; // no exponent
    if (constant_exponent == 0) return 1; // power of zero
    float result = constant;
    for (int e = 0; e < constant_exponent; e++) { // power
        result *= constant_exponent;
    }
    return coefficient * result;
}

/**
 * operator+
 * !!! only call if both terms are like terms
 * adds "m" to this
 */

Monomial Monomial::operator+(const Monomial & m) {
    Monomial sum;
    if (m.isConstant()) { // add constant terms
        sum.constant = getConstant() + m.getConstant();
        if (sum.constant < 0) { // keep negative to coefficient
            sum.coefficient = sum.coefficient * -1;
            sum.constant = sum.constant * -1;
        }
    } else { // add variable terms
        sum.coefficient = coefficient + m.coefficient;
        sum.degree = degree;
        std::copy(std::begin(variables), std::end(variables), std::begin(sum.variables));
    }
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) adding:\t\t" << m << " -> " << * this << " = " << sum << std::endl;
    return sum;
}

/**
 * operator-
 * !!! only call if both terms are like terms
 * subtract "m" from this
 */

Monomial Monomial::operator-(const Monomial & m) {
    Monomial difference;
    if (m.isConstant()) { // subtract constant terms
        difference.constant = getConstant() - m.getConstant();
        if (difference.constant < 0) { // move negative to coefficient
            difference.coefficient = difference.coefficient * -1;
            difference.constant = difference.constant * -1;
        }
    } else { // subtract variable terms
        difference.coefficient = coefficient - m.coefficient;
        difference.degree = degree;
        std::copy(std::begin(variables), std::end(variables), std::begin(difference.variables));
    }
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) subtracting:\t\t" << m << " -> " << * this << " = " << difference << std::endl;
    return difference;
}

// multiply this by "b"
Monomial Monomial::operator*(const Monomial & m) {
    Monomial product;
    if (m.isConstant()) { // multiply constant terms
        product.constant = getConstant() * m.getConstant();
        if (product.constant < 0) { // move negative to coefficient
            product.coefficient = product.coefficient * -1;
            product.constant = product.constant * -1;
        }
    } else { // multiply variable terms
        product.coefficient = coefficient * m.coefficient;
        product.degree = degree + m.degree;
        for (int c = 0; c < 256; c++) {
            product.variables[c] = variables[c] + m.variables[c];
        }
    }
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) multiplying:\t\t" << m << " -> " << * this << " = " << product << std::endl;
    return product;
}

Monomial & Monomial::operator=(const Monomial & m) { // copy variables of "m" to this except for "next" "prev" pointers
    if (CANONICALPOLYNOMIAL_MONOMIAL_DEBUG)
        std::cout << "(m) assigning:\t\t" << m << " -> " << * this << std::endl;
    if (this == & m) return * this;
    degree = m.degree;
    constant = m.constant;
    constant_exponent = m.constant_exponent;
    coefficient = m.coefficient;
    for (int c = 0; c < 256; c++) {
        variables[c] = m.variables[c];
    }
    return * this;
}

std::ostream & operator<<(std::ostream & o, const Monomial & m) {
    if (m.isZero()) o << "+0"; // zero term
    else if (m.constant > -1) { // constant term
        if (m.coefficient > 0) o << "+";
        o << m.coefficient * m.constant;
    } else { // variable term
        // output coefficient
        if (m.coefficient > 0) { // positive
            if (m.coefficient == 1) o << "+"; // invisible one
            else o << "+" << m.coefficient;
        } else { // negative
            if (m.coefficient == -1) o << "-"; // invisible one
            else o << m.coefficient;
        }
        // output variables
        for (int i = 0; i < 256; i++) {
            if (m.variables[i] > 0) { // variables with non-zero exponent
                if (m.variables[i] == 1) o << (char) i;
                else o << (char) i << "^" << m.variables[i];
            }
        }
    }
    return o;
}