#include "twine.h"

void powerInit(void)
{
    PM5CTL0 &= ~LOCKLPM5;
}
