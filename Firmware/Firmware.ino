// From PA0MHE's code.

#include <tinySPI.h>
#include <avr/sleep.h>

#define ADF4001  PB3      // 4001      - 10MHz      
#define ADF4001A PB4      // 4001      - 10MHz   
#define ADF4001B PB0      // 4001      - 10MHz       
       


uint32_t Reg4001[3] =  {
  0x1FC092, 
  0x100004, 
  0x000101,
} ; // 10Mhz oscillator, ref 10MHz, Muxout: digital lock detect


uint32_t Reg4001A[3] =  {
  0x1FC092, 
  0x100004, 
  0x000101,
} ; // 1st VHF oscillator


uint32_t Reg4001C[3] =  {
  0x1FC092, 
  0x100004, 
  0x000101,
} ; // 2nd VHF Oscillatr


void setup() {
  SPI.begin();

  pinMode(ADF4001, OUTPUT);
  digitalWrite(ADF4001, LOW);

  pinMode(ADF4001A, OUTPUT);
  digitalWrite(ADF4001A, LOW);

  pinMode(ADF4001B, OUTPUT);
  digitalWrite(ADF4001A, LOW);


  delay(500);
  ADFSet(Reg4001, 3, ADF4001);
  delay(500);
  ADFSet(Reg4001, 3, ADF4001A);
  delay(500);
  ADFSet(Reg4001, 3, ADF4001B);
  delay(500);

}


// Go to sleep, if for some reason you wake up again, go back to sleep.
void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void ADFWriteRegister32(const uint32_t value, byte LE) 
{
  digitalWrite(LE, LOW);
  for (int i = 3; i >= 0; i--) {             
    byte byteToSend = (value >> (i * 8)) & 0xFF;
    SPI.transfer(byteToSend);
  }
  digitalWrite(LE, HIGH);                    
  delay(1);
  digitalWrite(LE, LOW);                     
}
