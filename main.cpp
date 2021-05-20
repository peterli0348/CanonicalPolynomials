/**
 * Peter Li (23375692)
 * CSCI 381 Advance C++
 * Project 2: Canonical Polynomial
 * Due March 7th, 2020 before 11:59PM
 * -10 points for each day late
 *
 * Do not use STL
 *
 * Read polynomials line by line from input text file
 * Output text file containing:
 * 1.   polynomials as read from file
 * 2.   polynomials in canonical form
 * 3.   sum of polynomials in canonical form
 * 4.   difference of polynomials in canonical form
 * 5.   product of polynomials in canonical form
 */

#include <iostream>
#include "Lexer.h"
#include "Polynomial.h"
#define DEBUG true
#define MAXSIZE 1000

using namespace std;

/**
 * inputs:
 * 1. input text file with polynomials on each line
 * 2. output text file (optional)
 *
 * if no output file provided, one will be created using name of input file
 */

int main(int argc, char * argv[]) {
    ofstream outFile;

    if (argc != 3) {
        if (argc == 2) {
            string input = argv[1];
            cout << "File Error: missing output file" << endl << "Default output file: output_" + input << endl;
            outFile.open("output_" + input);
        } else {
            cout << "File Error: missing input file, output file";
            return 0;
        }
    } else outFile.open(argv[2]);

    cout << "Reading input file: " << argv[1] << endl;

    Lexer analyzer(argv[1]);

    Polynomial polynomials[MAXSIZE]; // change MAXSIZE to hold more polynomials
    int size = 0; // number of polynomials minus 1


/**
 * status   1: a valid Term is found
 * status   0: an invalid token is found
 * status  -1: a valid Term is found at end-of-line
 * status  -2: an invalid token is found at end-of-line
 * status  -3: end-of-stream is reached
 **/

    int status = -2; // status starts as -2
    while (status > -3) {
        Polynomial p;
        if (status > -1) p.concat(analyzer.m); // saves last valid Term created
        while (status > -1) {
            status = analyzer.driver(); // create Term in Lexer class
            if (status == 1 || status == -1) {
                p.concat(analyzer.m); // save Term before creating new Term to avoid memory leak
            }
        }
        if (!p.isZero()) {
            polynomials[size] = p;
            size++;
        }
        if (status > -3) status = analyzer.driver(); // create last Term
    }

    // output 1
    outFile << "polynomials as read from file" << endl;
    for (int i = 0; i < size; i++) {
        outFile << polynomials[i] << endl;
        if (DEBUG) cout << polynomials[i] << endl << "-----" << endl;
    }
    // output 2
    outFile << "polynomials in canonical form" << endl;
    for (int i = 0; i < size; i++) {
        polynomials[i] = polynomials[i].canonical();
        outFile << polynomials[i] << endl;
        if (DEBUG) cout << "canonical form:\t" << polynomials[i] << endl << "-----" << endl;
    }
    // output 3
    outFile << "sum of polynomials in canonical form" << endl;
    Polynomial sum(polynomials[0]);
    for (int i = 1; i < size; i++) {
        sum = sum +  polynomials[i];
    }
    outFile << sum << endl;
    if (DEBUG) {
        cout << "sum of polynomials: " << endl << "   (" << polynomials[0] << ")" << endl;
        for (int i = 1; i < size; i++) {
            cout << " + (" << polynomials[i] << ")" << endl;
        }
        cout << " = " <<  sum << endl << "-----" << endl;
    }
    // output 4
    outFile << "difference of polynomials in canonical form" << endl;
    Polynomial difference(polynomials[0]);
    for (int i = 1; i < size; i++) {
        difference = difference - polynomials[i];
    }
    outFile << difference << endl;
    if (DEBUG) {
        cout << "difference of polynomials: " << endl << "   (" << polynomials[0] << ")" << endl;
        for (int i = 1; i < size; i++) {
            cout << " - (" << polynomials[i] << ")" << endl;
        }
        cout << " = " <<  difference << endl << "-----" << endl;
    }
    // output 5
    outFile << "product of polynomials in canonical form" << endl;
    Polynomial product(polynomials[0]);
    for (int i = 1; i < size; i++) {
        product = product * polynomials[i];
    }
    outFile << product << endl;
    if (DEBUG) {
        cout << "product of polynomials: " << endl << "   (" << polynomials[0] << ")" << endl;
        for (int i = 1; i < size; i++) {
            cout << " * (" << polynomials[i] << ")" << endl;
        }
        cout << " = " <<  product << endl << "-----" << endl;
    }

    outFile.close();
    return 0;
}