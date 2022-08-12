#include "Arduino.h" 
#include "PinsGlobals.h"
#include "Encoder.h"
#include <LiquidCrystal_I2C.h>

#include "LCD_Wrapper.h"
#include "Machine.h"
#include "UI_States.h"
#include "RHTimer.h"
#include "REWrapper.h"
#include "Relay.h"


//comment out later for smoother running
//#define DEBUG

/*

  For use on Arduino Nano v3

 */




// --------- pins -----------       see PinsGlobals.h
// rotary encoder
Encoder encoder(RE_A_PIN, RE_B_PIN, RE_BUT_PIN); // set up rotary encoder
REWrapper rew = REWrapper( &encoder);


  /*     ----------    S E T U P     --------- */


  void setup() {

    
    #ifdef DEBUG 
      Serial.begin(115200);
      Serial.println("PlantBot online");
    #endif

    // set up relay pins
    Relay::init();


    // rotary encoder
    EncoderInterrupt.begin( &encoder );

    // default states
    LCD_Wrapper::init(); // this must come up top!
    Machine::init();
    Machine::changeState(new UI_Welcome());
    RHTimer::start();
    #ifdef DEBUG 
      Serial.println("finished setup");
    #endif 
  }



  /*     ----------    L O O P     --------- */



  void loop() {

    // update timer
    RHTimer::update();

    // // update rotary encoder
    rew.update();

    // update lcd backlight auto-off
    LCD_Wrapper::update();

    //update Machine for any state auto-transitions
    Machine::update(); 
    
    // //update relay for turning off
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
    https://stackoverflow.com/questions/64563173/state-machine-change-state



    VSCode:
    https://stackoverflow.com/questions/61954481/intellisense-throws-include-errors-when-working-with-arduino-in-vs-code


    F() macro / storing strings in progmem / buffer copying
    https://www.baldengineer.com/arduino-f-macro.html
    https://forum.arduino.cc/t/how-do-i-format-an-f-macro-string/997992/21?page=2
    https://stackoverflow.com/questions/69139334/how-can-i-use-flashstringhelper-in-a-function-which-takes-char




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
