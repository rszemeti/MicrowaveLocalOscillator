// From PA0MHE's code.

#include <tinySPI.h>
#include <avr/sleep.h>

#define ADF4001 DDB4      // 4001                
#define ADF4153 DDB3      // 4153        


uint32_t Reg4001[5] =  {
  0xD8092, 
  0x4, 
  0x200101,
} ; // 10Mhz oscillator, ref 10MHz, Muxout: digital lock detect

uint32_t Reg4153[6] =  {
  0x000003, /* clear spurs mode R3*/
  0x383,    /* write desired spurs mode R3*/
  0x1386,   /* Enable counter reset R2 =R2+4 */
  0x44015,  /* Write R1 */
  0x3E0008, /* Write R0 */
  0x1382    /* Disable counter reset  R2*/
 } ; 

void setup() {
  SPI.begin();

  pinMode(ADF4001, OUTPUT);
  digitalWrite(ADF4001, LOW);

  pinMode(ADF4153, OUTPUT);
  digitalWrite(ADF4153, LOW);

  delay(500);
  ADFSet(Reg4001, 3, ADF4001);
  delay(500);
  ADFSet(Reg4153, 6, ADF4153);
  delay(500);


}


// Go to sleep, if for some reason you wake up again, go back to sleep.
void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void ADFSet(uint32_t reg[], int c, byte LE) {
  for (int i = 0; i < c; i++) {
    ADFWriteRegister32(reg[i], LE);
  }
}

void ADFWriteRegister32(const uint32_t value, byte LE) //Program data into one of the 32bits register, the register that is selected by the four LSBs.
{
  digitalWrite(LE, LOW);
  for (int i = 3; i >= 0; i--)               // loop through 4 x 8bits
    SPI.transfer((value >> 8 * i) & 0xFF);   // shift , mask and send octet via SPI
  digitalWrite(LE, HIGH);                    // Load Enable rising edge the data stored in the shift register
  delay(1);
  digitalWrite(LE, LOW);                     //
}
