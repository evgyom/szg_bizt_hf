# CAFF parser

NOTE: VSCode + cmake build
https://github.com/CodeIntelligenceTesting/cifuzz/tree/main/examples

## A parser használata

A példák:
* c++: `debug_demo/main.cpp`
* python: `tester.py`

### Bemenetek:
* path1: a bemeneti CAFF fájl elérséi útvonala
* path2: a generálandó gif fájlneve és elérési útvonala
* file_length: szükséges beadni a fájl hosszát, mivel a bináris fájl tartalmazhat EOF-nek minősülő 0xFF karaktereket az RGB byte-okban. A fájl méretét byte-ra pontosan megállapíthatjuk pythonban

### Kimenetek:
* n_frames: a CIFF frame-ek száma
* creator_name
* creator_name_length
* date(year, month, day, hour, minute)
* captions: az első CIFF szerint
* capion_length: az első CIFF szerint
* tags: az első CIFF szerint
* tags_length: az első CIFF szerint
* n_tags: az első CIFF szerint
* total_duration
* frame_width
* frame_height

### Kimeneti státuszok

| Név | Kód | Leírás  |
|---|---|---|
| PARSER_FP_NULL,          | 0 | Nem sikerül megnyitni a file-t. |
| PARSER_BUFFER_SIZE_ERROR | 1 | A beolvasás során a buffer méretét meghaladja a beírandó byte-ok száma. |
| PARSER_EOF_REACHED       | 2 | A parser váratlanul elért a file végére, annak ellenére. Ez az hibakód nem túl valószínű, mivel a parser bemenetként kapja a fájl hosszát, és ellenőrzi, hogy a megadott számú CIFF (adott mérettel) elfér-e a fájlban. |
| CAFF_FORMAT_ERROR        | 3 | Különböző hibák, amik sértik a CAFF formátum specifikációt. Ide tartozik az összes inkonzisztencia, különböző egymásból számolható paraméter között. |
| CAFF_FIELD_SIZE_ERROR    | 4 | Egy mező mérete meghaladta a parser által meximálisan kezeltet. |
| CAFF_UNDEFINED_SECTION   | 5 | A CAFF feldolgozása végén nem értük el a file végét. Van plusz tartalom a file-ban. Ez lehet például azért, mert a header-ben a num_anim kisebb mint a CIFF-ek száma valójában. |
| PARSER_GENERAL_ERROR     | 6 | Minden egyéb. |



## CAFF feldolgozás

Limitált méretek
* Creator name, CIFF captions, CIFF tags:  max 512 Byte
* Maximális CIFF méret: 2100000 pixel (1920x1080=2,073,600)


The parser uses the following library to encode the data in the GIF format.
https://github.com/charlietangora/gif-h/blob/master/gif.h

About color similiarity:
https://www.baeldung.com/cs/compute-similarity-of-colours

# Tests

## Unit tesztek
* reader ...
    * 
* process_header function
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * CAFF magic 
    * frame size matches header size and == 20
* process_credits function
    * contents
    * null file pointer
    * eof reached
    * correct frame id
    * non-ascii creator name
    * error with creator name buffer size
* process_ciff_frame function
    * content
* parse function
    * content
    * több num_anim a header-ben, mint amennyi valójában van -> CAFF_FORMAT_ERROR
    * kisebb num_anim a hedear-ben -> CAFF_UNDEFINED_SECTION
    * más paraméter
        * creator_len
        * caff_header_size
        * ciff_header_size
        * width, height (azonos szorzattal)
        * new_line in tags
    * only header

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