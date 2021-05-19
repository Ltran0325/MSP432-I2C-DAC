#include "msp.h"

void I2C_LED(uint8_t byte);
void delay(uint16_t count);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //-- init I2C
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST  |
                       EUSCI_B_CTLW0_MST    |       // master mode
                       EUSCI_B_CTLW0_MODE_3 |       // I2C mode
                       EUSCI_B_CTLW0_SSEL__SMCLK|   // SMCLK
                       EUSCI_B_CTLW0_TR;            // Transmitter

    P6->SEL0 |= BIT4 | BIT5;    // UCB1SDA & UCB1SCL

    EUSCI_B1->BRW = 1;

    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;

    EUSCI_B1->I2CSA = 0x2C;

    EUSCI_B1->IE |= EUSCI_B_IE_TXIE0; //Transmit interrupt enable
    EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG0;

    I2C_LED(0x1F);

    while(1){


    }

}

void I2C_LED(uint8_t byte){

    // start condition
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    // command byte
    while(!(EUSCI_B1->IFG & BIT1)){};      // wait for txbuff empty in master mode
    EUSCI_B1->TXBUF = 0x00;

    // output byte
    while(!(EUSCI_B1->IFG & BIT1)){};      // wait for txbuff empty in master mode
    EUSCI_B1->TXBUF = byte;

    // stop condition
    while(!(EUSCI_B1->IFG & BIT1)){};      // wait for txbuff empty in master mode
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

}
void delay(uint16_t count){

    while(count != 0){
        count--;
    }

}


