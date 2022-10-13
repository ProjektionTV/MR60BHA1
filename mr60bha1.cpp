#include <mr60bha1.h>

void MR60BHA1::setup()
{
    Serial2.begin(115200);
}


void MR60BHA1::loop()
{
  int16_t ch = Serial2.read();
  if(ch>=0)
    parser(ch);
}

void MR60BHA1::clear_buffer()
{
  for(uint8_t i=0; i<BUFFER_SIZE; i++)
    buffer[i] = 0x00;
}


void MR60BHA1::parser(uint8_t ch)
{
  static uint8_t state = SYNC1;
  static uint8_t chksum = 0;
  static uint8_t cd = 0;
  static uint8_t od = 0;
  static uint16_t len = 0;

  switch(state)
  {
    case SYNC1:
      clear_buffer();
      chksum = 0;
      cd = 0;
      od = 0;
      len = 0;
      if(ch == 0x53)
      {
        chksum += ch;
        state++;
      }
    break;

    case SYNC2:
      if(ch == 0x59)
      {
        chksum += ch;
        state++;
      }
      else
        state=0;
    break;
    case CD:
      chksum += ch;
      cd = ch;
      state++;
    break;
    case OD:
      chksum += ch;
      od = ch;
      state++;
    break;
    case LEN_HIGH:
      chksum += ch;
      len = uint16_t(ch<<8);
      state++;
    break;

    case LEN_LOW:
      chksum += ch;
      len += ch;
      state++;
    break;

    case DATA:
      chksum += ch;
      len--;

      len = constrain(len, 0, BUFFER_SIZE);
      buffer[len] = ch;

      if(!len)
        state++;
    break;

    case CHK:
      if(chksum == ch)
        state++;
      else
        state=SYNC1;
    break;
    case EOF1:
      if(ch == 0x54)
        state++;
      else
        state=SYNC1;
    break;
    case EOF2:
      if(ch == 0x43)
        parser2(cd, od);

      state=SYNC1;
    break;
  }
}

void MR60BHA1::parser2(uint8_t cd, uint8_t od)
{
  if(cd == 0x80 && od == 0x04) // human distance
  {
    human_distance = uint16_t(buffer[1]<<8) | buffer[0];
    if(!human_distance)
        clear_values();
  }

  if(cd == 0x85 && od == 0x02) // heart rate
  {
    heart_rate = buffer[0];
  }

  if(cd == 0x81 && od == 0x03) // respiratory rate
  {
    respiratory_rate = buffer[0];
  }

  if(cd == 0x81 && od == 0x02) // respiratory value
  {
    respiratory_value = buffer[0];
  }
}

void MR60BHA1::clear_values()
{
    human_distance = 0;
    heart_rate = 0;
    respiratory_rate = 0;
    respiratory_value = 0;
}


uint16_t MR60BHA1::get_human_distance()
{
    return human_distance;
}

uint8_t MR60BHA1::get_heart_rate()
{
    return heart_rate;
}

uint8_t MR60BHA1::get_respiratory_rate()
{
    return respiratory_rate;
}

uint8_t MR60BHA1::get_respiratory_value()
{
    return respiratory_value;
}