# su20-proj1-starter

Two bugs waste about an hour to fix:
1. strncmp returns zero when two strings are equal, whereas the function used to check if two strings are the same should return 1 when two strings are equal, so ! should be added before strncmp function.
2. Variable "wordlowernofirst" is miswritten as "wordlower".
