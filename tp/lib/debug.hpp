
# include "global.hpp"
# include "uart.hpp"

#define INIT

void initUART();
void debugPrint(uint16_t);
void debugPrint(const char* x);
void debugPrint(unsigned char* x);

#ifdef DEBUG
# ifdef INIT
#  define DEBUG_PRINT(x) initUART(); debugPrint(x) //If you define your UART transmision yourself, you have to undef INIT manually
#  undef INIT
# else
#  define DEBUG_PRINT(x) debugPrint(x)
# endif
#else
# define DEBUG_PRINT(x) do {} while (0) // code mort
#endif
