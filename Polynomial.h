/**
 * Polynomial class stores Monomial objects
 * !!! does not accept power of power       e.g. 4a^2^4
 * !!! does not accept fractions            e.g. a/2
 * !!! does not accept parentheses          e.g. 2(a+b)
 * Polynomial is a doubly linked list of Monomials
 *
 * copy constructor with Monomial parameter returns a Polynomial with only one Monomial
 * destructor deletes first Monomial from the linked list until linked list is empty
 * zero terms returned by a Monomial arithmetic operation are removed from the linked list
 */

#ifndef CANONICALPOLYNOMIAL_POLYNOMIAL_H
#define CANONICALPOLYNOMIAL_POLYNOMIAL_H
#define CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG true

#if CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG
#include <iostream>
#endif

#include "Monomial.h"

class Polynomial {
public:
    Monomial * head = nullptr;
    Monomial * tail = nullptr;

    Polynomial();

    ~Polynomial();  // delete first Monomial in Polynomial until Polynomial is zero term

    Polynomial(const Polynomial & p);

    Polynomial(const Monomial & m);

    inline bool isZero() const { return head->next == tail; }

    Polynomial canonical(); // returns a similar Polynomial in canonical form

    void canonicalInsert(const Monomial & m); // insert Monomial in canonical form

    void concat(const Monomial & m); // link Monomial to end of linked list

    Polynomial operator+(const Polynomial & p);

    Polynomial operator+(const Monomial & m);

    Polynomial operator-(const Polynomial & p);

    Polynomial operator-(const Monomial & m);

    Polynomial operator*(const Polynomial & p);

    Polynomial operator*(const Monomial & m);

    Polynomial & operator=(const Polynomial & p);

    friend std::ostream & operator<<(std::ostream & o, const Polynomial & p);

};

#endif //CANONICALPOLYNOMIAL_POLYNOMIAL_H