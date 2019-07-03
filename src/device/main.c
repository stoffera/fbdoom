#include <stdio.h>
#include <termios.h>

#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"

int main(int argc, const char** argv)
{
    myargc = argc;
    myargv = argv;

    // struct termios initial_settings, new_settings;
    // tcgetattr(0,&initial_settings);

    // new_settings = initial_settings;
    // new_settings.c_lflag &= ~ICANON;
    // new_settings.c_lflag &= ~ECHO;
    // new_settings.c_lflag &= ~ISIG;
    // new_settings.c_cc[VMIN] = 0;
    // new_settings.c_cc[VTIME] = 0;

    // tcsetattr(0, TCSANOW, &new_settings);

    D_DoomMain();

    // tcsetattr(0, TCSANOW, &initial_settings);

    return 0;
}

void I_StartTic (void)
{
    // event_t event = {0,0,0,0};
    // char key = getchar();
    // if (key != EOF)
    // {
    //          if (key == 'a') key = KEY_LEFTARROW;
    //     else if (key == 'd') key = KEY_RIGHTARROW;
    //     else if (key == 'w') key = KEY_UPARROW;
    //     else if (key == 's') key = KEY_DOWNARROW;

    //     event.type = ev_keydown;
    //     event.data1 = key;
    //     D_PostEvent(&event);

    //     event.type = ev_keyup;
    //     event.data1 = key;
    //     D_PostEvent(&event);
    // }
}