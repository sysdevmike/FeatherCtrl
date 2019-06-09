#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

class Matrix {
  public:
    enum class Dim {
      Row = 7,
      Col = 15,
    };

    struct Key {
      uint8_t r;
      uint8_t c;
    };

    Matrix(void);
    void begin(void);
    bool scan(void);
    bool debounce_tick(const int row, const int col, const bool pressed);
    bool pressed(const Key k) const;
    void sleep(void);

  private:
  
    struct KeyState {
      uint32_t pressTime;
      bool pressed;
    };

    KeyState keys[(int)Dim::Row][(int)Dim::Col];

    struct debounce_status_t {
      uint8_t state;
      uint32_t last_millis;
    };    
    
    debounce_status_t debounce[(int)Dim::Row][(int)Dim::Col];
    
    enum {
      STATE_NOT_PRESSED = 0,
      STATE_PRESSED_BOUNCING = 1, // freeze for 5ms, consider pressed
      STATE_PRESSED = 2,
      STATE_RELEASING_BOUNCING = 3, // freeze for 5ms
    };
        

    static const uint8_t rowPins[];
    static const uint8_t colPins[];
    static const uint8_t debounceTime = 5;    
};

#endif
