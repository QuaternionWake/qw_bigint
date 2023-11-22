First version of my bigint implementation, from the limited
testing I've done I believe everything works correctly, though
again, limited testing.

Currently my implementations of the basic arithmetic operations
is about 1000x slower than when doing the same operations on
regular ints with identical content.

Currently digits are stored as 32 bit numbers however shortly
before making the first commit I found out it is possible to
get the complete carry from 64 bit multiplication, so the next
version should be rewriting this to be 64 bit using those
functions.
