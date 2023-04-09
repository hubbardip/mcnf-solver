# Minimum-cost network flow solver

This is a MCNF solver built from scratch in pure C. It implements the successive shortest paths algorithm to solve the MCNF problem in pseudo-polynomial time.

Note: this is not intended to be used for any practical application. Any popular solver suite would certainly perform better.

## Usage
Compile by running `make`. Input is not provided in this repository; the solver can take input in the DIMACS network format or through manually creating a graph. See `main.c` for examples.

## Future work

Implement capacity scaling to reduce to polynomial time.