# CAFF parser

## Reader level

## Lexer level

## Parser level

The parser uses the following library to encode the data in the GIF format.
https://github.com/lecram/gifenc

About color similiarity:
https://www.baeldung.com/cs/compute-similarity-of-colours

# Tests

### Reader tests

### Lexer tests

* Process header function
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * CAFF magic 
    * frame size matches header size and == 20
* For process_credits only
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * non-ascii creator name
    * error with creator name buffer size
    
### Parser tests

## Todos
* jó így hívni python-ból?
    * Milyen infók kellenek? Creator name, date, caption, tags?
* ugly gif + add png?
* milyen doksi minek?
* static analyzer
* afl
* Add protection flags to cmakefile
    * https://stackoverflow.com/questions/54247344/cmake-different-compiler-flags-during-configuration