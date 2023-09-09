// From PA0MHE's code.

#include <tinySPI.h>
#include <avr/sleep.h>

#define ADF4001 DDB4      // 4001                
#define ADF4153 DDB3      // 4153        


uint32_t Reg4001[5] =  {0x5A0038, 0x8008051, 0x1A004E42, 0x4B3, 0x9A003C} ; // 10Mhz oscillator, ref 10MHz, Muxout: digital lock detect
uint32_t Reg4153[6] =  {
  0x000003, /* clear spurs mode */
  0x3C7,    /* write desired spurs mode */
  0x001BC6, /* Enable counter reset */
  0x144191, /* Write R1 */
  0x1E80C8, /* Write R2 2450 MHz */
  0x1BC2    /* Disable counter reset  1BC2 =5mA */
 } ; 

void setup() {
  SPI.begin();

  pinMode(ADF4001, OUTPUT);
  digitalWrite(ADF4001, LOW);

  pinMode(ADF4153, OUTPUT);
  digitalWrite(ADF4153, LOW);

  delay(500);
  ADFSet(Reg4001, 5, ADF4001);
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
