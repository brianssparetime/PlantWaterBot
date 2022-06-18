#include "Arduino.h" 
#include "Pins.h"
#include "Encoder.h"
#include <LiquidCrystal_I2C.h>

#include "LCD_Wrapper.h"
#include "Machine.h"
#include "UI_States.h"
#include "RHTimer.h"
#include "REWrapper.h"
#include "Relay.h"

#define DEBUG


/*

  For use on Arduino Nano v3

 */

// --------- pins -----------  see Pins.h

// relay
const int REL = RELAY_PIN;   

// rotary encoder
const int RE_BUT = RE_BUT_PIN; // rotary encoder button  // NOTE:  library does pinmode pullup
const int RE_A = RE_A_PIN; // rotary encoder motion
const int RE_B = RE_B_PIN; // rotary encoder motion
Encoder encoder(RE_A, RE_B, RE_BUT); // set up rotary encoder
REWrapper rew = REWrapper(encoder);
// LCD via I2C using A4 and A5
//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// this is set up in LCD_Wrapper




  /*     ----------    S E T U P     --------- */


  void setup() {

    
    #ifdef DEBUG 
      Serial.begin(115200);
      Serial.println("PlantBot online");
    #endif

    // set up pins
    pinMode(REL, OUTPUT); // low active

    // rotary encoder
    EncoderInterrupt.begin( &encoder );

    // initialize the lcd 
    //lcd.init();                      
   

    // default states
    RoughHoursTimer::start();
    Machine::changeState(new UI_Welcome());
    LCD_Wrapper::action();

  }



  /*     ----------    L O O P     --------- */



  void loop() {

    // update timer
    RoughHoursTimer::update();

    // update rotary encoder
    rew.update();

    // update lcd backlight auto-off
    LCD_Wrapper::update();

    //update Machine for any state auto-transitions
    Machine::update();

    //update relay for turning off
    Relay::update();

  } // end loop





  /* RESOURCES

      pinout:
      https://i.stack.imgur.com/W9Ril.png

      Power supply wiring:
      https://electronics.stackexchange.com/questions/60199/powering-arduino-nano-12volts

      RE:
      https://github.com/John-Lluch/Encoder
        https://www.instructables.com/Improved-Arduino-Rotary-Encoder-Reading/
        https://github.com/John-Lluch/Encoder/issues/4#issue-955354998
        https://github.com/PaulStoffregen/Encoder (didn't use)
        

      Button interrupts:
      https://www.allaboutcircuits.com/technical-articles/using-interrupts-on-arduino/
        rising falling
        https://forum.arduino.cc/index.php?topic=147825.0
        pull up pull down
        https://www.instructables.com/Understanding-the-Pull-up-Resistor-With-Arduino/
        debouncing
        https://forum.arduino.cc/t/debouncing-an-interrupt-trigger/45110/3
        https://github.com/thomasfredericks/Bounce2   <<<<< TODO:  seriously consider trying this!!!

      TM1637 4 segment display
      https://github.com/Seeed-Studio/Grove_4Digital_Display
      https://create.arduino.cc/projecthub/ryanchan/tm1637-digit-display-arduino-quick-tutorial-ca8a93
      https://www.makerguides.com/tm1637-arduino-tutorial/

      relay
      https://randomnerdtutorials.com/guide-for-relay-module-with-arduino/

      enum switch
      https://stackoverflow.com/questions/52932529/why-does-my-switch-case-default-when-using-enums



    FSM and singletons and static shit:
    https://www.aleksandrhovhannisyan.com/blog/implementing-a-finite-state-machine-in-cpp/
    https://refactoring.guru/design-patterns/singleton
    https://stackoverflow.com/questions/14676709/c-code-for-state-machine
    https://forum.arduino.cc/t/how-to-write-an-arduino-library-with-a-singleton-object/666625/2
    https://stackoverflow.com/questions/36240473/singleton-pattern-cannot-call-member-function-without-object



    VSCode:
    https://stackoverflow.com/questions/61954481/intellisense-throws-include-errors-when-working-with-arduino-in-vs-code

      
      USB Mini male plug pinout
      
      |_|     |_|
      |_|_____|_|
          | |   (top row when narrow side of connector is up)
        |  |  | (bottom row when narrow side of connector is up)
      GND     5v 


RE (rotary encoder) wiring
                    ____
      RE_A (7)   - |    |_  RE_BUT (6)
      GND        - |    |_  GND
      RE_B (8)   - |____|

     Use 10k pullups on RE_A and RE_B to 5v (if not using KY040 module)


     */
