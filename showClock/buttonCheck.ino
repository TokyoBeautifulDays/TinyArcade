// Button check (walk around for chattering problem)


uint16_t button1ReadOut() {
              uint16_t loop = 0;
              while (checkButton(TAButton1)) { loop ++ ; }  // Read chattering out !
              return loop;
}

uint16_t button2ReadOut() {
              uint16_t loop = 0;
              while (checkButton(TAButton2)) { loop ++ ; }  // Read chattering out !
              return loop;
}
uint16_t JoystickLeftReadOut() {
              uint16_t loop = 0;
              while (checkJoystick(TAJoystickLeft)) { loop ++ ; }  // Read chattering out !
              return loop;
}
uint16_t JoystickRightReadOut() {
              uint16_t loop = 0;
              while (checkJoystick(TAJoystickRight)) { loop ++ ; }  // Read chattering out !
              return loop;
}
uint16_t JoystickUpReadOut() {
              uint16_t loop = 0;
              while (checkJoystick(TAJoystickUp)) { loop ++ ; }  // Read chattering out !
              return loop;
}
uint16_t JoystickDownReadOut() {
              uint16_t loop = 0;
              while (checkJoystick(TAJoystickDown)) { loop ++ ; }  // Read chattering out !
              return loop;
}
/*

#define SHORTPUSH 1000
#define LONGPUSH 10000

 loop = button1ReadOut();  // walk around button1 chattering
              if (loop > SHORTPUSH) {
                on = !on;
                SerialUSB.print("SHORTPUSH = ");
                SerialUSB.println(loop);
                if (loop > LONGPUSH) {
                  SerialUSB.print("LONG PUSH = ");
                  SerialUSB.println(loop);
                }
              }

*/