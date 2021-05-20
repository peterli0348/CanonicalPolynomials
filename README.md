# CanonicalPolynomials

Project is for CSCI381 Advanced OOP in C++

## Highlights

* **Lexical Analyzer**

* **Operator Overload**

* **Function Overload**

* **Well documented Code**

* **Headers**

* **Debug logs**

* **File Manipulation**

* **Copy Constructor**

## Requirements
* Read polynomials line by line from input text file

* Do not use C++ STL

* Output text file containing:
  1.   polynomials as read from file
  2.   polynomials in canonical form
  3.   sum of polynomials in canonical form
  4.   difference of polynomials in canonical form
  5.   product of polynomials in canonical form

## Sample Inputs and Outputs

sample_input_2.txt

> 4X^5+2X^2+3X^3+4X^4
>
> X^2+4X

output_sample_input_2.txt

> polynomials as read from file
> 
> +4X^5+2X^2+3X^3+4X^4
> 
> +2X^2+4X
> 
> polynomials in canonical form
> 
> +4X^5+4X^4+3X^3+2X^2
> 
> +2X^2+4X
> 
> sum of polynomials in canonical form
> 
> +4X^5+4X^4+3X^3+4X^2+4X
> 
> difference of polynomials in canonical form
> 
> +4X^5+4X^4+3X^3+4X
> 
> product of polynomials in canonical form
> 
> +32X^8+32X^7+24X^6+16X^5

---

sample_input_1.txt

> 1 5 2 2 3 3 4 4
> 2 2 4 1

output_sample_input_1.txt

> polynomials as read from file
> 
> +1+5+2+2+3+3+4+4
> 
> +2+2+4
> 
> polynomials in canonical form
> 
> +24
> 
> +8
> 
> sum of polynomials in canonical form
> 
> +32
> 
> difference of polynomials in canonical form
> 
> +16
> 
> product of polynomials in canonical form
> 
> +192

