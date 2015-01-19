# SequenceL Regex
A complete SequenceL regular expression library in C++ with an array of match functions

This project is an implementation of the SequenceL regular expression library. SequenceL is a small,
statically typed, purly functional, and automatic parallel programming language developed by
Dr. Danial E. Cook at Texas Tech University.

For allowed character sets, please refer to the PDF reference guide.

Upon running, the program reads two strings S1 and S2, where S2 is a regualr expression to match S1.
The program upon running first tokenize and parse S1 and convert it into non-deterministic automata,
then deterministic automata to match any string.

The reference guides for regular expression tokenzier and paser are also in the Git. Please refer to
the PDF files for detailed regular expression tokens, context-free grammars, and LL(1) parse table.

After the regular expression DFA is available, the program is up to match the given S1. The project
provides a wide variety of FIND and MATCH functions. Some are given below as examples.

   1. Matches("ca", "(b|c)a") -> true
   2. FirstMatch("cabcr, "ab") -> struct(flag: true, begin: 2, end: 3)
   3. FindAll("The fox jumped over.", "\\a+") -> ["The", "fox", "jumped", "over"]

For more FIND and MATCH functions and their complete descriptions and examples, please refer to the
PDF reference guide.
