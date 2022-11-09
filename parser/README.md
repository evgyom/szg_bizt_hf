# CAFF parser

https://github.com/CodeIntelligenceTesting/cifuzz/tree/main/examples

## A parser használata

### Bemenetek:
* path1: a bemeneti CAFF fájl elérséi útvonala
* path2: a generálandó gif fájlneve és elérési útvonala
* file_length: szükséges beadni a fájl hosszát, mivel a bináris fájl tartalmazhat EOF-nek minősülő 0xFF karaktereket az RGB byte-okban. A fájl méretét byte-ra pontosan megállapíthatjuk pythonban

### Kimenetek:
(nem mind implementált)
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

### Kimeneti státuszok

| Név | Kód | Leírás  |
|---|---|---|
| PARSER_FP_NULL,          | 0 | Nem sikerül megnyitni a file-t. |
| PARSER_BUFFER_SIZE_ERROR | 1 | A beolvasás során a buffer méretét meghaladja a beírandó byte-ok száma. |
| PARSER_EOF_REACHED       | 2 | A parser váratlanul elért a file végére, annak ellenére. Ez az hibakód nem túl valószínű, mivel a parser bemenetként kapja a fájl hosszát, és ellenőrzi, hogy a megadott számú CIFF (adott mérettel) elfér-e a fájlban. |
| CAFF_FORMAT_ERROR        | 3 | Különböző hibák, amik sértik a CAFF formátum specifikációt. Ide tartozik az összes inkonzisztencia, különböző egymásból számolható paraméter között. |
| CAFF_FIELD_SIZE_ERROR    | 4 | Egy mező mérete meghaladta a parser által meximálisan kezeltet. |
| PARSER_GENERAL_ERROR     | 5 | Minden egyéb. |






## CAFF feldolgozás

Limitált méretek
* Creator name, CIFF captions, CIFF tags:  max 512 Byte
* Maximális CIFF méret: 2100000 pixel (1920x1080=2,073,600)

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