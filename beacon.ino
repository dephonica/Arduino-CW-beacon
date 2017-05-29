// Arduino simply CW beacon
// (c) 2012 Max Klimenko
// emaster [at] mail.ru

// Beacon message
const char message[] = "VVV CQ CQ CQ DX DE BEACON BAND 40M 8000KHZ";

// Period of single point (ms)
const int point_time = 80;

// Carrier frequency divider.
// Carrier freq. (MHz) = 16 / (2 * (1 + freq_div))
const unsigned char freq_div = 1;

//////////////////////////////////////////////////////////////////
struct s_abc
{
        char letter;
        char sign[7];
};

const s_abc abc[] = {
        'A',".-", 'B',"-...", 'W',".--", 'G',"--.", 'D',"-..", 
        'E',".", 'V',"...-", 'Z',"--..", 'I',"..",
        'J',".---", 'K',"-.-", 'L',".-..", 'M',"--", 'N',"-.", 
        'O',"---", 'P',".--.", 'R',".-.", 'S',"...",
        'T',"-", 'U',"..-", 'F',"..-.", 'H',"....", 'C',"-.-.", 
        'Q',"--.-", 'Y',"-.--", 'X',"-..-", '1',".----",
        '2',"..---", '3',"...--", '4',"....-", '5',".....", 
        '6',"-....", '7',"--...", '8',"---..", '9',"----.",
        '0',"-----", '.',"......", ',',".-.-.-", ':',"---...", 
        ';',"-.-.-.", '(',"-.--.-", '`',".----.",
        '"',".-..-.", '-',"-....-", '/',"-..-.", '?',"..--..", 
        '!',"--..--", '@',".--.-.", '\\',"..-.-" };

unsigned char abc_size = sizeof(abc) / sizeof(abc[0]);

void setup()
{
  PORTB = 0;
  DDRB |= 1<<1;
  
  OCR1A = freq_div;
  TCCR1A = 0x48;
  TCCR1B = 0x09;
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);   // set the LED on
}

void send_letter(char l)
{
  if (l == ' ')
  {
    delay(point_time * 7);
    return;
  }
  
  unsigned char idx = 255;
  for (unsigned char i = 0; i < abc_size; i++)
    if (abc[i].letter == l)
    {
      idx = i;
      break;
    }
    
  if (idx == 255) return;
  
  const char *s = abc[idx].sign;
  
  for (unsigned char c = 0; s[c] != 0; c++)
  {
    char q = s[c];
    
    DDRB |= 1<<1;
    digitalWrite(13, HIGH);   // set the LED on
    
    if (q == '.') delay(point_time);
    else delay(point_time * 3);
    
    DDRB &= ~(1<<1);
    digitalWrite(13, LOW);   // set the LED off
    
    delay(point_time);
  }

  delay(point_time * 2);
}

void loop()
{
  for (int n = 0; message[n] != 0; n++)
    send_letter(message[n]);
  
  delay(2000);
}

