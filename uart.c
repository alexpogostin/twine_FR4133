#include "twine.h"

void uartInit(void)
{
    P1SEL0 |= BIT0;
    P1SEL0 |= BIT1;

    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL_3;

// 9600 baud @ 2Mhz SMCLK
#ifdef BAUD_RATE_9600
    UCA0MCTLW = 0x2000;   // UCBRSx
    UCA0MCTLW |= BIT0;    // UCOS16
    UCA0BRW = 0x0006;     // UCBRx
    UCA0MCTLW |= UCBRF_8; // UCBRFx
#endif

// 115200 baud @ 2Mhz SMCLK
#ifdef BAUD_RATE_115200
    UCA0MCTLW = 0xD600;   // UCBRSx
    UCA0MCTLW &= ~BIT0;   // UCOS16
    UCA0BRW = 0x0008;     // UCBRx
    UCA0MCTLW |= UCBRF_0; // UCBRFx
#endif

    UCA0CTLW0 &= ~UCSWRST;

    UCA0IE |= UCRXIE;
}

/*****************************************************************************/
short uartTx(unsigned char (*txStr)[UART_RX_BUF_SIZE], unsigned int node, unsigned char *str)
{

    unsigned int i = 0;
    unsigned int j = 0;

    UCA0IE &= ~UCTXIE;
    do
        txStr[node][i++] = str[j];
    while(str[j++]);
    if(!(UCA0IFG & UCTXIE))
        UCA0IFG |= UCTXIE;
    UCA0IE |= UCTXIE;

    return j;
}
