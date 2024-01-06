// This is an Arduino recreation of "Experiment 48 - 2-bit LCD Display"
// from the book "123 PIC Microcontroller Experiments for the Evil Genius
// by Myke Predko.
//
// The schematics are at page 130 and the code is at pages 127, 128 and 227.

#define TWOBITLCD_DATA_PIN 2
#define TWOBITLCD_CLK_PIN 3

inline void strobe_EN() {
  digitalWrite(TWOBITLCD_CLK_PIN, 1);
  digitalWrite(TWOBITLCD_CLK_PIN, 0); 
}

void clear174() {

  digitalWrite(TWOBITLCD_DATA_PIN, 0);
  for(uint8_t i = 0; i < 6; i++)
    strobe_EN(); 
}

void nibbleShift(uint8_t LCDOut, uint8_t RS) {

  clear174();

  LCDOut = LCDOut | (1 << 5) | ((RS & 1) << 4);

  for(uint8_t i = 0; i < 6; i++)
  {
    if(0 != (LCDOut & (1 << 5)))
      digitalWrite(TWOBITLCD_DATA_PIN, 1);
    else
      digitalWrite(TWOBITLCD_DATA_PIN, 0);

    LCDOut = LCDOut << 1;

    strobe_EN();
  }

  digitalWrite(TWOBITLCD_DATA_PIN, 1);
  digitalWrite(TWOBITLCD_DATA_PIN, 0);
}

void LCDWrite(uint8_t LCDData, uint8_t RSValue) {

  nibbleShift((LCDData >> 4) & 0x0F, RSValue);
  nibbleShift(LCDData & 0x0F, RSValue);

  if((0 == (LCDData & 0xFC)) && (0 == RSValue))
    delay(5);
  else
    delayMicroseconds(200);
}

void LCDprint(const char* aText) {
  while(*aText){
    LCDWrite(*aText++, 1); 
  }
}

void LCDinit() {
  
  // Wait for LCD to power up:
  delay(20);

  // Start init process
  nibbleShift(0x03, 0);

  delay(5);

  // Repeat reset command
  nibbleShift(0x03, 0);

  delayMicroseconds(200);

  // Repeat reset command third time
  nibbleShift(0x03, 0);

  delayMicroseconds(200);

  // Initialize LCD 4-bit mode
  nibbleShift(0x02, 0);

  delayMicroseconds(200);

  // LCD is 4 bit I/F, 2 Line
  LCDWrite(0b00101000, 0);

  // Clear LCD
  LCDWrite(0b00000001, 0);

  // Move cursor after each character
  LCDWrite(0b00000110, 0);

  // Turn on LCD and enable cursor
  LCDWrite(0b00001110, 0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(TWOBITLCD_DATA_PIN, OUTPUT);
  pinMode(TWOBITLCD_CLK_PIN, OUTPUT);

  LCDinit();

  LCDWrite(0b10000000, 0);
  LCDprint("Hello World!");

  // Move cursor to the second line.
  LCDWrite(0b11000000, 0);
  LCDprint("1234567890");
}

void loop() {
  // put your main code here, to run repeatedly:

}
