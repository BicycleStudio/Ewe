#ifndef IO_KEYS_H_
#define IO_KEYS_H_

#define IO_KEY_ESCAPE          0x01
#define IO_KEY_1               0x02
#define IO_KEY_2               0x03
#define IO_KEY_3               0x04
#define IO_KEY_4               0x05
#define IO_KEY_5               0x06
#define IO_KEY_6               0x07
#define IO_KEY_7               0x08
#define IO_KEY_8               0x09
#define IO_KEY_9               0x0A
#define IO_KEY_0               0x0B
#define IO_KEY_MINUS           0x0C    /* - on main keyboard */
#define IO_KEY_EQUALS          0x0D
#define IO_KEY_BACK            0x0E    /* backspace */
#define IO_KEY_TAB             0x0F
#define IO_KEY_Q               0x10
#define IO_KEY_W               0x11
#define IO_KEY_E               0x12
#define IO_KEY_R               0x13
#define IO_KEY_T               0x14
#define IO_KEY_Y               0x15
#define IO_KEY_U               0x16
#define IO_KEY_I               0x17
#define IO_KEY_O               0x18
#define IO_KEY_P               0x19
#define IO_KEY_LBRACKET        0x1A
#define IO_KEY_RBRACKET        0x1B
#define IO_KEY_RETURN          0x1C    /* Enter on main keyboard */
#define IO_KEY_LCONTROL        0x1D
#define IO_KEY_A               0x1E
#define IO_KEY_S               0x1F
#define IO_KEY_D               0x20
#define IO_KEY_F               0x21
#define IO_KEY_G               0x22
#define IO_KEY_H               0x23
#define IO_KEY_J               0x24
#define IO_KEY_K               0x25
#define IO_KEY_L               0x26
#define IO_KEY_SEMICOLON       0x27
#define IO_KEY_APOSTROPHE      0x28
#define IO_KEY_GRAVE           0x29    /* accent grave */
#define IO_KEY_LSHIFT          0x2A
#define IO_KEY_BACKSLASH       0x2B
#define IO_KEY_Z               0x2C
#define IO_KEY_X               0x2D
#define IO_KEY_C               0x2E
#define IO_KEY_V               0x2F
#define IO_KEY_B               0x30
#define IO_KEY_N               0x31
#define IO_KEY_M               0x32
#define IO_KEY_COMMA           0x33
#define IO_KEY_PERIOD          0x34    /* . on main keyboard */
#define IO_KEY_SLASH           0x35    /* / on main keyboard */
#define IO_KEY_RSHIFT          0x36
#define IO_KEY_MULTIPLY        0x37    /* * on numeric keypad */
#define IO_KEY_LMENU           0x38    /* left Alt */
#define IO_KEY_SPACE           0x39
#define IO_KEY_CAPITAL         0x3A
#define IO_KEY_F1              0x3B
#define IO_KEY_F2              0x3C
#define IO_KEY_F3              0x3D
#define IO_KEY_F4              0x3E
#define IO_KEY_F5              0x3F
#define IO_KEY_F6              0x40
#define IO_KEY_F7              0x41
#define IO_KEY_F8              0x42
#define IO_KEY_F9              0x43
#define IO_KEY_F10             0x44
#define IO_KEY_NUMLOCK         0x45
#define IO_KEY_SCROLL          0x46    /* Scroll Lock */
#define IO_KEY_NUMPAD7         0x47
#define IO_KEY_NUMPAD8         0x48
#define IO_KEY_NUMPAD9         0x49
#define IO_KEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define IO_KEY_NUMPAD4         0x4B
#define IO_KEY_NUMPAD5         0x4C
#define IO_KEY_NUMPAD6         0x4D
#define IO_KEY_ADD             0x4E    /* + on numeric keypad */
#define IO_KEY_NUMPAD1         0x4F
#define IO_KEY_NUMPAD2         0x50
#define IO_KEY_NUMPAD3         0x51
#define IO_KEY_NUMPAD0         0x52
#define IO_KEY_DECIMAL         0x53    /* . on numeric keypad */
#define IO_KEY_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define IO_KEY_F11             0x57
#define IO_KEY_F12             0x58
#define IO_KEY_F13             0x64    /*                     (NEC PC98) */
#define IO_KEY_F14             0x65    /*                     (NEC PC98) */
#define IO_KEY_F15             0x66    /*                     (NEC PC98) */
#define IO_KEY_KANA            0x70    /* (Japanese keyboard)            */
#define IO_KEY_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define IO_KEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define IO_KEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define IO_KEY_YEN             0x7D    /* (Japanese keyboard)            */
#define IO_KEY_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define IO_KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define IO_KEY_PREVTRACK       0x90    /* Previous Track (IO_KEY_CIRCUMFLEX on Japanese keyboard) */
#define IO_KEY_AT              0x91    /*                     (NEC PC98) */
#define IO_KEY_COLON           0x92    /*                     (NEC PC98) */
#define IO_KEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define IO_KEY_KANJI           0x94    /* (Japanese keyboard)            */
#define IO_KEY_STOP            0x95    /*                     (NEC PC98) */
#define IO_KEY_AX              0x96    /*                     (Japan AX) */
#define IO_KEY_UNLABELED       0x97    /*                        (J3100) */
#define IO_KEY_NEXTTRACK       0x99    /* Next Track */
#define IO_KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define IO_KEY_RCONTROL        0x9D
#define IO_KEY_MUTE            0xA0    /* Mute */
#define IO_KEY_CALCULATOR      0xA1    /* Calculator */
#define IO_KEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define IO_KEY_MEDIASTOP       0xA4    /* Media Stop */
#define IO_KEY_VOLUMEDOWN      0xAE    /* Volume - */
#define IO_KEY_VOLUMEUP        0xB0    /* Volume + */
#define IO_KEY_WEBHOME         0xB2    /* Web home */
#define IO_KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define IO_KEY_DIVIDE          0xB5    /* / on numeric keypad */
#define IO_KEY_SYSRQ           0xB7
#define IO_KEY_RMENU           0xB8    /* right Alt */
#define IO_KEY_PAUSE           0xC5    /* Pause */
#define IO_KEY_HOME            0xC7    /* Home on arrow keypad */
#define IO_KEY_UP              0xC8    /* UpArrow on arrow keypad */
#define IO_KEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define IO_KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define IO_KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define IO_KEY_END             0xCF    /* End on arrow keypad */
#define IO_KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define IO_KEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define IO_KEY_INSERT          0xD2    /* Insert on arrow keypad */
#define IO_KEY_DELETE          0xD3    /* Delete on arrow keypad */
#define IO_KEY_LWIN            0xDB    /* Left Windows key */
#define IO_KEY_RWIN            0xDC    /* Right Windows key */
#define IO_KEY_APPS            0xDD    /* AppMenu key */
#define IO_KEY_POWER           0xDE    /* System Power */
#define IO_KEY_SLEEP           0xDF    /* System Sleep */
#define IO_KEY_WAKE            0xE3    /* System Wake */
#define IO_KEY_WEBSEARCH       0xE5    /* Web Search */
#define IO_KEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define IO_KEY_WEBREFRESH      0xE7    /* Web Refresh */
#define IO_KEY_WEBSTOP         0xE8    /* Web Stop */
#define IO_KEY_WEBFORWARD      0xE9    /* Web Forward */
#define IO_KEY_WEBBACK         0xEA    /* Web Back */
#define IO_KEY_MYCOMPUTER      0xEB    /* My Computer */
#define IO_KEY_MAIL            0xEC    /* Mail */
#define IO_KEY_MEDIASELECT     0xED    /* Media Select */

#endif // IO_KEYS_H_