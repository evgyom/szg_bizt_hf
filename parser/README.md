# CAFF parser

## Tests

### Reader tests

### Lexer tests

* For all functions
    * null file pointer
    * eof reached
    * correct frame id
* For process_header only
    * CAFF magic 
    * frame size matches header size and == 20
    * header contents
* For process_credits only
    * credits contents
    * non-ascii creator name
    * error with creator name buffer
    

### Parser tests

## Todos
* start get_ciff_frame

## Parsing
* https://prettydiff.com/2/guide/unrelated_parser.xhtml#howto-lexer
* https://supunsetunga.medium.com/writing-a-parser-getting-started-44ba70bb6cc9

## What is in a GIF?
https://giflib.sourceforge.net/whatsinagif/bits_and_bytes.html
https://giflib.sourceforge.net/whatsinagif/lzw_image_data.html

* Header
    * Signature
    * Version
* Logic Screen Descriptor
    * Canvas width (not really relevant)
    * Canvas height (not really relevant)
    * Packed field
        * Global color table - 1 bit
        * Color resolution (N)  - 3 bits
        * Sort flag - 1 bit
        * Background color intex
        * Pixel aspect ratio
* Global Color Table
    * 2^(N+1) colors, each color 3 bytes
    * each color has an index
* Graphics Control Extension
* Image Descriptor
    * Image separator
    * Postions left, top, width, height
    * Packed field
* Local color table
    * Same as the global color table
 * Image data
    * LZW Compression
    * 
* Trailer: 3B

### Animation
https://giflib.sourceforge.net/whatsinagif/animation_and_transparency.html

###

https://github.com/lecram/gifenc