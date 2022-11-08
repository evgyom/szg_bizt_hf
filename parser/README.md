# CAFF parser

## Reader level

## Lexer level

## Parser level

The parser uses the following library to encode the data in the GIF format.
https://github.com/lecram/gifenc
https://github.com/charlietangora/gif-h/blob/master/gif.h

About color similiarity:
https://www.baeldung.com/cs/compute-similarity-of-colours

# Tests

## Unit tests

### Reader tests

### Lexer tests

* process_header
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * CAFF magic 
    * frame size matches header size and == 20
* process_credits
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * non-ascii creator name
    * error with creator name buffer size

### Parser tests

## Static code analysis

## Input fuzzing

## Todos
* átlátszóság
* kivonni bytokat creator névből
* tests:
    *
* https://github.com/charlietangora/gif-h/blob/master/gif-h-demo.cpp
* formátum szigorúan vétele?
    * little endian !?
* jó így hívni python-ból?
    * Milyen infók kellenek? Creator name, date, caption, tags?
    * Milyen error code-ot lenne jó vissza adni?
        * CAFF_FORMAT_ERROR ?
* ugly gif + add png?
* milyen doksi minek?
* static analyzer
* afl
* Add protection flags to cmakefile
    * https://stackoverflow.com/questions/54247344/cmake-different-compiler-flags-during-configuration