# CAFF parser

## A parser használata

Bemenetek:
* path1: a bemeneti CAFF fájl elérséi útvonala
* path2: a generálandó gif fájlneve és elérési útvonala
* file_length: szükséges beadni a fájl hosszát, mivel a bináris fájl tartalmazhat EOF-nek minősülő 0xFF karaktereket az RGB byte-okban. A fájl méretét byte-ra pontosan megállapíthatjuk pythonban

Kimenetek:
* creator_name (pointerként átadva)
* creator_name_length
* date(year, month, day, hour, minute)
* captions
* capion_length
* tags
* tags_length
* total_duration
* frame_width
* frame_height
* number_of_frames

## Reader level

## Lexer level

## Parser level

The parser uses the following library to encode the data in the GIF format.
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