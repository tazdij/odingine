#ifndef INPUT_H
#define INPUT_H

/**
    Input - handles all of the input mapping for odingine
    
    Description:
        Input is a configurable mapper, which issues commands (using a
    Command-like Pattern) to any system listening to input.
**/

typedef void (*Input_listener)(int key);



#endif