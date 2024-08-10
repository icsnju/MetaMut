/* { dg-additional-options "-fdiagnostics-text-art-charset=ascii -fdiagnostics-color=always" } */

int non_empty;

/* { dg-begin-multiline-output "" }

  A
   B
    C

   { dg-end-multiline-output "" } */

/* { dg-begin-multiline-output "" }

  [38;2;0;0;0;48;2;240;217;181m[K♜ [38;2;0;0;0;48;2;181;136;99m[K♞ [38;2;0;0;0;48;2;240;217;181m[K♝ [38;2;0;0;0;48;2;181;136;99m[K♛ [38;2;0;0;0;48;2;240;217;181m[K♚ [38;2;0;0;0;48;2;181;136;99m[K♝ [38;2;0;0;0;48;2;240;217;181m[K♞ [38;2;0;0;0;48;2;181;136;99m[K♜ [m[K
  [38;2;0;0;0;48;2;181;136;99m[K♟ [38;2;0;0;0;48;2;240;217;181m[K♟ [38;2;0;0;0;48;2;181;136;99m[K♟ [38;2;0;0;0;48;2;240;217;181m[K♟ [38;2;0;0;0;48;2;181;136;99m[K♟ [38;2;0;0;0;48;2;240;217;181m[K♟ [38;2;0;0;0;48;2;181;136;99m[K♟ [38;2;0;0;0;48;2;240;217;181m[K♟ [m[K
  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [m[K
  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [m[K
  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [m[K
  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [48;2;181;136;99m[K  [48;2;240;217;181m[K  [m[K
  [38;2;255;255;255;48;2;240;217;181m[K♙ [38;2;255;255;255;48;2;181;136;99m[K♙ [38;2;255;255;255;48;2;240;217;181m[K♙ [38;2;255;255;255;48;2;181;136;99m[K♙ [38;2;255;255;255;48;2;240;217;181m[K♙ [38;2;255;255;255;48;2;181;136;99m[K♙ [38;2;255;255;255;48;2;240;217;181m[K♙ [38;2;255;255;255;48;2;181;136;99m[K♙ [m[K
  [38;2;255;255;255;48;2;181;136;99m[K♖ [38;2;255;255;255;48;2;240;217;181m[K♘ [38;2;255;255;255;48;2;181;136;99m[K♗ [38;2;255;255;255;48;2;240;217;181m[K♕ [38;2;255;255;255;48;2;181;136;99m[K♔ [38;2;255;255;255;48;2;240;217;181m[K♗ [38;2;255;255;255;48;2;181;136;99m[K♘ [38;2;255;255;255;48;2;240;217;181m[K♖ [m[K

   { dg-end-multiline-output "" } */

/* { dg-begin-multiline-output "" }

  +--+
  |🙂|
  +--+

   { dg-end-multiline-output "" } */

/* { dg-begin-multiline-output "" }

  +-------+-----+---------------+---------------------+-----------------------+-----------------------+
  |Offsets|Octet|       0       |          1          |           2           |           3           |
  +-------+-----+-+-+-+-+-+-+-+-+-+-+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  | Octet | Bit |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
  +-------+-----+-+-+-+-+-+-+-+-+-+-+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  |   0   |  0  |Version|  IHL  |     DSCP      | ECN |                 Total Length                  |
  +-------+-----+-------+-------+---------------+-----+--------+--------------------------------------+
  |   4   | 32  |           Identification            | Flags  |           Fragment Offset            |
  +-------+-----+---------------+---------------------+--------+--------------------------------------+
  |   8   | 64  | Time To Live  |      Protocol       |                Header Checksum                |
  +-------+-----+---------------+---------------------+-----------------------------------------------+
  |  12   | 96  |                                  Source IP Address                                  |
  +-------+-----+-------------------------------------------------------------------------------------+
  |  16   | 128 |                               Destination IP Address                                |
  +-------+-----+-------------------------------------------------------------------------------------+
  |  20   | 160 |                                                                                     |
  +-------+-----+                                                                                     |
  |  ...  | ... |                                       Options                                       |
  +-------+-----+                                                                                     |
  |  56   | 448 |                                                                                     |
  +-------+-----+-------------------------------------------------------------------------------------+

   { dg-end-multiline-output "" } */
