# CAFF parser

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
| PARSER_STATUS_OK         | 0 | OK |
| PARSER_FP_NULL,          | 1 | Nem sikerül megnyitni a file-t. |
| PARSER_BUFFER_SIZE_ERROR | 2 | A beolvasás során a buffer méretét meghaladja a beírandó byte-ok száma. |
| PARSER_EOF_REACHED       | 3 | A parser váratlanul elért a file végére, annak ellenére. Ez az hibakód nem túl valószínű, mivel a parser bemenetként kapja a fájl hosszát, és ellenőrzi, hogy a megadott számú CIFF (adott mérettel) elfér-e a fájlban. |
| CAFF_FORMAT_ERROR        | 4 | Különböző hibák, amik sértik a CAFF formátum specifikációt. Ide tartozik az összes inkonzisztencia, különböző egymásból számolható paraméter között. |
| CAFF_FIELD_SIZE_ERROR    | 5 | Egy mező mérete meghaladta a parser által meximálisan kezeltet. |
| CAFF_UNDEFINED_SECTION   | 6 | A CAFF feldolgozása végén nem értük el a file végét. Van plusz tartalom a file-ban. Ez lehet például azért, mert a header-ben a num_anim kisebb mint a CIFF-ek száma valójában. |
| PARSER_GENERAL_ERROR     | 7 | Minden egyéb. |

## CAFF feldolgozás

Limitált méretek
* Creator name, CIFF captions, CIFF tags:  max 512 Byte
* Maximális CIFF méret: 2100000 pixel (1920x1080=2,073,600)

A parser GIF-ek létrehozásához az alábbi c++ könyvtárat használja:
https://github.com/charlietangora/gif-h/blob/master/gif.h

## Fordítás

A fordítást cmake-kel végezhető, mi a vscode. Az alábbi gcc opció a top level CMakeLists.txt-ben engedélyezve lett. 

* -fstack-protector (stack smashing protection)

## Tesztek

### Unit tesztek

Az alább vázlatosan leírt unit tesztek mind sikeresen futottak. A /tests mappában található a részletes definíció.

* reader függvényekre
    * contents ok
    * eof reached
    * null file pointer
    * null pointer buffers
    * buffer size check
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

## Statikus code analízis

A kiválasztott cppcheck nevű eszköz segítségével néhány tipikus hibát észre tudtunk venni a c kódban (file pointer felszabadításának hiánya pl.). A megtalált hibák könnyen javíthatók voltak. A legutolsó report nem tartalmaz hibát, amit érdemes lenne bemutatni.

## Input fuzzing

Az AFL fuzzerrel több iterációban végeztünk fuzzolást és bugfixelést. Bemenetnek egy majdnem egész CIFF-et tartalmazó, 1 MB méretűre vágott CAFF fájl szolgált (1 MB az engedélyezett maximális). A fuzzolást az `afl_start.sh ` script indítja, ami definiálja a fordítást is. A konkrét, hibát adó bemenetek az `afl_test/past_crashes` mappában találhatók. A fájl nagy mérete miatt, és a CIFF lezáratlansága miatt azonban ez a teszt nem teljes értékű. 

Az alábbi táblázat mutatja az egyes iterációk részleteit. Az első két iteráció részletei hiányosak. 

| Idő     | Total execs | uniq. crashes | cycles done | total paths | komment |
|---      |---          |---            |---          |     ---     |   ---   |
| 30 perc | ~ 1 M       | 6             |             |             | signed változó túcsordult és a buffer méret ellenőrzés elbukott. javítva. | 
| 15 perc | ~ 500 k     | 4             |             |             | unsigned változóból kivonás. túlcsordult. javítva. |
| 15 perc | 607 k       | 0             | 0           | 93          | itt minden rendben | 

Flying Ducks