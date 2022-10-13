#ifndef MR60BHA1_H_
#define MR60BHA1_H_

#include <Arduino.h>

#define BUFFER_SIZE 5

typedef enum
{
  SYNC1,
  SYNC2,
  CD,
  OD,
  LEN_HIGH,
  LEN_LOW,
  DATA,
  CHK,
  EOF1,
  EOF2
} parser_state_t;

class MR60BHA1
{
    public:
        void setup();
        void loop();
        uint16_t get_human_distance();
        uint8_t get_heart_rate();
        uint8_t get_respiratory_rate();
        uint8_t get_respiratory_value();
        

    protected:
        void parser(uint8_t ch);
        uint8_t buffer[BUFFER_SIZE];
        void clear_buffer();
        void parser2(uint8_t cd, uint8_t od);
        uint16_t human_distance;
        uint8_t heart_rate;
        uint8_t respiratory_rate;
        uint8_t respiratory_value;
        void clear_values();

};

#endif // MR60BHA1_H_