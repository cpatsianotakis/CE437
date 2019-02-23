#ifndef FDS_MACROS_L
#define FDS_MACROS_L

#define AND( a, b)  ( (a == '1' && b == '1') ? '1' : '0')
#define OR( a, b)  ( (a == '0' && b == '0') ? '0' : '1')
#define NOT( a)  ( (a == '1') ? '0' : '1')

#endif