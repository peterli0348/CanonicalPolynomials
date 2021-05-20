#include "Polynomial.h"

Polynomial::Polynomial() {
    head = new Monomial();
    tail = new Monomial();
    head->next = tail;
    tail->prev = head;
}

Polynomial::~Polynomial() {
    while (head->next != tail) { // delete linked list
        delete head->next;
    }
    delete head;
    delete tail;
}

Polynomial::Polynomial(const Polynomial & p) { // copy "p"
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) copy constructor\tstart (p)" << std::endl;
    head = new Monomial();
    tail = new Monomial();
    head->next = tail;
    tail->prev = head;
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) { // copy each Monomial in "p"
        concat(* pointer);
        pointer = pointer->next;
    }
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) copy constructor\tend (p)\t\t" << * this << std::endl;
}

Polynomial::Polynomial(const Monomial & m) { // copy "m"
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) copy constructor\tstart (m)" << std::endl;
    head = new Monomial();
    tail = new Monomial();
    head->next = tail;
    tail->prev = head;
    concat(m); // copy Monomial "m"
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) copy constructor\tend (m)\t\t" << * this << std::endl;
}

void Polynomial::concat(const Monomial & m) { // link a copy of "m" to end of linked list
    Monomial * term = new Monomial(m);
    term->prev = tail->prev;
    term->next = tail;
    tail->prev->next = term;
    tail->prev = term;
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) concatenating:\t" << m << " -> " << * this << std::endl;
}

Polynomial Polynomial::canonical() { // returns a new Polynomial in canonical form
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) canonical start:\t" << * this << std::endl;
    if (head->next == tail) return Polynomial(); // zero term
    Polynomial canonical;
    return canonical + * this;
}

void Polynomial::canonicalInsert(const Monomial & m) { // insert "m" in canonical form
    if (m.isZero()) return; // skip inserting zero term
    Monomial * term = new Monomial(m);
    Monomial * pointer = head;
    while (pointer->next->degree > term->degree) pointer = pointer->next; // find canonical spot for "m"
    term->next = pointer->next;
    term->prev = pointer;
    pointer->next->prev = term;
    pointer->next = term;
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) inserting:\t\t" << m << " -> " << * this << std::endl;
}

Polynomial Polynomial::operator+(const Polynomial & p) {
    if (p.isZero()) return Polynomial(* this); // adding zero term
    Polynomial sum = Polynomial(* this);
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) { // for each Monomial in "p"
        Monomial * sumP = sum.head->next;
        bool added = false;
        while (sumP != sum.tail) { // for each Monomial in "sum"
            if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
                std::cout << "(p) comparing:\t\t" << * sumP << " <> " << * pointer << std::endl;
            if (sumP->isLike(* pointer)) { // addition possible
                * sumP = * sumP + * pointer;
                if (sumP->isZero()) delete sumP; // terms cancel out
                added = true;
                break;
            }
            sumP = sumP->next;
        }
        if (!added) sum.canonicalInsert(* pointer);
        pointer = pointer->next;
    }
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) adding:\t\t" << p << " -> " << * this << " => " << sum << std::endl;
    return sum;
}

Polynomial Polynomial::operator+(const Monomial & m) {
    if (m.isZero()) return Polynomial(* this); // adding zero term
    if (this->isZero()) return Polynomial(m); // adding to zero term
    return * this + Polynomial(m);
}

Polynomial Polynomial::operator-(const Polynomial & p) {
    if (p.isZero()) return Polynomial(* this); // subtracting zero term
    Polynomial difference = Polynomial(* this);
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) { // for each Monomial in "p"
        Monomial * differenceP = difference.head->next;
        bool subtracted = false;
        while (differenceP != difference.tail) { // for each Monomial in "difference"
            if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
                std::cout << "(p) comparing:\t\t" << * differenceP << " <> " << * pointer << std::endl;
            if (differenceP->isLike(* pointer)) { // subtraction possible
                * differenceP = * differenceP - * pointer;
                if (differenceP->isZero()) delete differenceP; // terms cancel out
                subtracted = true;
                break;
            }
            differenceP = differenceP->next;
        }
        if (!subtracted) difference.canonicalInsert(* pointer);
        pointer = pointer->next;
    }
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) subtracting:\t\t" << p << " -> " << * this << " => " << difference << std::endl;
    return difference;
}

Polynomial Polynomial::operator-(const Monomial & m) {
    if (m.isZero()) return Polynomial(* this); // subtracting zero term
    return * this - Polynomial(m);
}

Polynomial Polynomial::operator*(const Polynomial & p) {
    if (isZero() || p.isZero()) return Polynomial(); // multiplying zero term
    Polynomial product = Polynomial(* this);
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) { // for each Monomial in "p"
        Monomial * productP = product.head->next;
        while (productP != product.tail) { // for each Monomial currently in "product"
            * productP = * productP * * pointer;
            if (productP->isZero()) { // terms cancel out
                Monomial * temp = pointer;
                pointer = pointer->prev;
                delete temp;
            }
            productP = productP->next;
        }
        pointer = pointer->next;
    }
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) multiplying:\t\t" << p << " -> " << * this << " => " << product << std::endl;
    return product;
}

Polynomial Polynomial::operator*(const Monomial & m) {
    if (isZero() || m.isZero()) return Polynomial(); // multiplying zero term
    return * this * Polynomial(m);
}

Polynomial & Polynomial::operator=(const Polynomial & p) { // copies linked list of "p"
    if (CANONICALPOLYNOMIAL_POLYNOMIAL_DEBUG)
        std::cout << "(p) assigning:\t\t" << p << " -> " << * this << std::endl;
    if (this == & p) return * this;
    while (head->next != tail) { // delete linked list
        delete head->next;
    }
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) {
        concat(* pointer);
        pointer = pointer->next;
    }
    return * this;
}

std::ostream & operator<<(std::ostream & o, const Polynomial & p) {
    if (p.isZero()) { // output 0 for empty polynomial
        o << 0;
        return o;
    }
    Monomial * pointer = p.head->next;
    while (pointer != p.tail) {
        o << * pointer;
        pointer = pointer->next;
    }
    return o;
}