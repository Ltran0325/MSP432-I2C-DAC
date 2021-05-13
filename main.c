#include "msp.h"

void EUSCIB1_IRQHandler(void);

volatile int counter = 0;
uint8_t brightness = 0xE0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    //-- init I2C pins
    P6->SEL0 |= BIT4 | BIT5;    // UCB1SDA & UCB1SCL

    //-- init I2C
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST  |
                       EUSCI_B_CTLW0_MST    |       // master mode
                       EUSCI_B_CTLW0_MODE_3 |       // I2C mode
                       EUSCI_B_CTLW0_SSEL__SMCLK|   // SMCLK
                       EUSCI_B_CTLW0_TR;            // Transmitter

    EUSCI_B1->BRW = 30;
    EUSCI_B1 ->I2CSA  = 0x2C;

    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;

    EUSCI_B1->IE |= BIT1;
    EUSCI_B1->IFG &= ~BIT1;

    //-- Init NVIC
    NVIC -> ISER[0] = 1 << ((EUSCIB1_IRQn) & 31);//I2C EUSCIB1_IRQn interrupt enabled

    //-- Start I2C command
    EUSCI_B1 -> CTLW0 |= BIT1;

    while(1){

    }

}

void EUSCIB1_IRQHandler(void)
{
    if(EUSCI_B1->IV & 0x18)
    {
        // Write command
        if(counter==0){EUSCI_B1->TXBUF = 0x00;}
        // Brightness (0x00-0xFF)
        if(counter==1){EUSCI_B1->TXBUF = brightness;}
        // Stop condition
        if(counter==2){EUSCI_B1->CTLW0 |= BIT2;}

        counter++;
    }
}
