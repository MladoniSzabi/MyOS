#include "sys/keyboard.h"
#include "sys/debug.h"

const uint64_t scancode_set_2_presscodes[] = {
    0,
    /*KEYCODE_ESCAPE*/ 0x76,
    /*KEYCODE_KEY_ONE*/ 0x16,
    /*KEYCODE_KEY_TWO*/ 0x1E,
    /*KEYCODE_KEY_THREE*/ 0x26,
    /*KEYCODE_KEY_FOUR*/ 0x25,
    /*KEYCODE_KEY_FIVE*/ 0x2E,
    /*KEYCODE_KEY_SIX*/ 0x36,
    /*KEYCODE_KEY_SEVEN*/ 0x3D,
    /*KEYCODE_KEY_EIGHT*/ 0x3E,
    /*KEYCODE_KEY_NINE*/ 0x46,
    /*KEYCODE_KEY_ZERO*/ 0x45,
    /*KEYCODE_DASH*/ 0x4E,
    /*KEYCODE_EQUAL*/ 0x55,
    /*KEYCODE_BACKSPACE*/ 0x66,
    /*KEYCODE_TAB*/ 0x0D,
    /*KEYCODE_KEY_Q*/ 0x15,
    /*KEYCODE_KEY_W*/ 0x1D,
    /*KEYCODE_KEY_E*/ 0x24,
    /*KEYCODE_KEY_R*/ 0x2D,
    /*KEYCODE_KEY_T*/ 0x2C,
    /*KEYCODE_KEY_Y*/ 0x35,
    /*KEYCODE_KEY_U*/ 0x3C,
    /*KEYCODE_KEY_I*/ 0x43,
    /*KEYCODE_KEY_O*/ 0x44,
    /*KEYCODE_KEY_P*/ 0x4D,
    /*KEYCODE_SQUARE_BRACKET_OPEN*/ 0x54,
    /*KEYCODE_SQUARE_BRACKET_CLOSE*/ 0x5B,
    /*KEYCODE_ENTER*/ 0x5A,
    /*KEYCODE_LEFT_CONTROL*/ 0x14,
    /*KEYCODE_KEY_A*/ 0x1C,
    /*KEYCODE_KEY_S*/ 0x1B,
    /*KEYCODE_KEY_D*/ 0x23,
    /*KEYCODE_KEY_F*/ 0x2B,
    /*KEYCODE_KEY_G*/ 0x34,
    /*KEYCODE_KEY_H*/ 0x33,
    /*KEYCODE_KEY_J*/ 0x3B,
    /*KEYCODE_KEY_K*/ 0x42,
    /*KEYCODE_KEY_L*/ 0x4B,
    /*KEYCODE_SEMICOLON*/ 0x4C,
    /*KEYCODE_SINGLE_QUOTE*/ 0x52,
    /*KEYCODE_BACK_TICK*/ 0x0E,
    /*KEYCODE_LEFT_SHIFT*/ 0x12,
    /*KEYCODE_BACKSLASH*/ 0x5D,
    /*KEYCODE_KEY_Z*/ 0x1A,
    /*KEYCODE_KEY_X*/ 0x22,
    /*KEYCODE_KEY_C*/ 0x21,
    /*KEYCODE_KEY_V*/ 0x2A,
    /*KEYCODE_KEY_B*/ 0x32,
    /*KEYCODE_KEY_N*/ 0x31,
    /*KEYCODE_KEY_M*/ 0x3A,
    /*KEYCODE_COMMA*/ 0x41,
    /*KEYCODE_PERIOD*/ 0x49,
    /*KEYCODE_SLASH*/ 0x4A,
    /*KEYCODE_RIGHT_SHIFT*/ 0x59,
    /*KEYCODE_KP_MUL*/ 0x7C,
    /*KEYCODE_LEFT_ALT*/ 0x11,
    /*KEYCODE_SPACE*/ 0x29,
    /*KEYCODE_CAPS_LOCK*/ 0x58,
    /*KEYCODE_F1*/ 0x05,
    /*KEYCODE_F2*/ 0x06,
    /*KEYCODE_F3*/ 0x04,
    /*KEYCODE_F4*/ 0x0C,
    /*KEYCODE_F5*/ 0x03,
    /*KEYCODE_F6*/ 0x0B,
    /*KEYCODE_F7*/ 0x83,
    /*KEYCODE_F8*/ 0x0A,
    /*KEYCODE_F9*/ 0x01,
    /*KEYCODE_F10*/ 0x09,
    /*KEYCODE_NUM_LOCK*/ 0x77,
    /*KEYCODE_SCROLL_LOCK*/ 0x7E,
    /*KEYCODE_KP_7*/ 0x6C,
    /*KEYCODE_KP_8*/ 0x75,
    /*KEYCODE_KP_9*/ 0x7D,
    /*KEYCODE_KP_DASH*/ 0x7B,
    /*KEYCODE_KP_4*/ 0x6B,
    /*KEYCODE_KP_5*/ 0x73,
    /*KEYCODE_KP_6*/ 0x74,
    /*KEYCODE_KP_PLUS*/ 0x79,
    /*KEYCODE_KP_1*/ 0x69,
    /*KEYCODE_KP_2*/ 0x72,
    /*KEYCODE_KP_3*/ 0x7A,
    /*KEYCODE_KP_0*/ 0x70,
    /*KEYCODE_KP_PERIOD*/ 0x71,
    /*KEYCODE_F11*/ 0x78,
    /*KEYCODE_F12*/ 0x07,

    /*KEYCODE_PREV_TRACK*/ 0xE015,
    /*KEYCODE_NEXT_TRACK*/ 0xE04D,
    /*KEYCODE_KP_ENTER*/ 0xE05A,
    /*KEYCODE_RIGHT_CONTROL*/ 0xE014,
    /*KEYCODE_MUTE*/ 0xE023,
    /*KEYCODE_CALCULATOR*/ 0xE02B,
    /*KEYCODE_PLAY*/ 0xE034,
    /*KEYCODE_STOP*/ 0xE03B,
    /*KEYCODE_VOLUME_DOWN*/ 0xE021,
    /*KEYCODE_VOLUME_UP*/ 0xE032,
    /*KEYCODE_WWW_HOME*/ 0xE03A,
    /*KEYCODE_KP_SLASH*/ 0xE04A,
    /*KEYCODE_RIGHT_ALT*/ 0xE011,
    /*KEYCODE_HOME*/ 0xE06C,
    /*KEYCODE_CURSOR_UP*/ 0xE075,
    /*KEYCODE_CURSOR_LEFT*/ 0xE06B,
    /*KEYCODE_CURSOR_RIGHT*/ 0xE074,
    /*KEYCODE_END*/ 0xE069,
    /*KEYCODE_CURSOR_DOWN*/ 0xE072,
    /*KEYCODE_PAGE_DOWN*/ 0xE07A,
    /*KEYCODE_INSERT*/ 0xE070,
    /*KEYCODE_DELETE*/ 0xE071,
    /*KEYCODE_LEFT_GUI*/ 0xE01F,
    /*KEYCODE_RIGHT_GUI*/ 0xE027,
    /*KEYCODE_APPS*/ 0xE02F,
    /*KEYCODE_ACPI_POWER*/ 0xE037,
    /*KEYCODE_ACPI_SLEEP*/ 0xE03F,
    /*KEYCODE_ACPI_WAKE*/ 0xE05E,
    /*KEYCODE_WWW_SEARCH*/ 0xE010,
    /*KEYCODE_WWW_FAVORITES*/ 0xE018,
    /*KEYCODE_WWW_REFRESH*/ 0xE020,
    /*KEYCODE_WWW_STOP*/ 0xE028,
    /*KEYCODE_WWW_FORWARD*/ 0xE030,
    /*KEYCODE_WWW_BACK*/ 0xE038,
    /*KEYCODE_MY_COMPUTER*/ 0xE040,
    /*KEYCODE_EMAIL*/ 0xE048,
    /*KEYCODE_MEDIA_SELECT*/ 0xE050,
    /*KEYCODE_PRINTSCREEN*/ 0xE012E07C,
    /*KEYCODE_PAUSE*/ 0xE11477E1F014F077};

const uint64_t scancode_set_2_releasecodes[] = {
    0,
    /*KEYCODE_ESCAPE*/ 0xF076,
    /*KEYCODE_KEY_ONE*/ 0xF016,
    /*KEYCODE_KEY_TWO*/ 0xF01E,
    /*KEYCODE_KEY_THREE*/ 0xF026,
    /*KEYCODE_KEY_FOUR*/ 0xF025,
    /*KEYCODE_KEY_FIVE*/ 0xF02E,
    /*KEYCODE_KEY_SIX*/ 0xF036,
    /*KEYCODE_KEY_SEVEN*/ 0xF03D,
    /*KEYCODE_KEY_EIGHT*/ 0xF03E,
    /*KEYCODE_KEY_NINE*/ 0xF046,
    /*KEYCODE_KEY_ZERO*/ 0xF045,
    /*KEYCODE_DASH*/ 0xF04E,
    /*KEYCODE_EQUAL*/ 0xF055,
    /*KEYCODE_BACKSPACE*/ 0xF066,
    /*KEYCODE_TAB*/ 0xF00D,
    /*KEYCODE_KEY_Q*/ 0xF015,
    /*KEYCODE_KEY_W*/ 0xF01D,
    /*KEYCODE_KEY_E*/ 0xF024,
    /*KEYCODE_KEY_R*/ 0xF02D,
    /*KEYCODE_KEY_T*/ 0xF02C,
    /*KEYCODE_KEY_Y*/ 0xF035,
    /*KEYCODE_KEY_U*/ 0xF03C,
    /*KEYCODE_KEY_I*/ 0xF043,
    /*KEYCODE_KEY_O*/ 0xF044,
    /*KEYCODE_KEY_P*/ 0xF04D,
    /*KEYCODE_SQUARE_BRACKET_OPEN*/ 0xF054,
    /*KEYCODE_SQUARE_BRACKET_CLOSE*/ 0xF05B,
    /*KEYCODE_ENTER*/ 0xF05A,
    /*KEYCODE_LEFT_CONTROL*/ 0xF014,
    /*KEYCODE_KEY_A*/ 0xF01C,
    /*KEYCODE_KEY_S*/ 0xF01B,
    /*KEYCODE_KEY_D*/ 0xF023,
    /*KEYCODE_KEY_F*/ 0xF02B,
    /*KEYCODE_KEY_G*/ 0xF034,
    /*KEYCODE_KEY_H*/ 0xF033,
    /*KEYCODE_KEY_J*/ 0xF03B,
    /*KEYCODE_KEY_K*/ 0xF042,
    /*KEYCODE_KEY_L*/ 0xF04B,
    /*KEYCODE_SEMICOLON*/ 0xF04C,
    /*KEYCODE_SINGLE_QUOTE*/ 0xF052,
    /*KEYCODE_BACK_TICK*/ 0xF00E,
    /*KEYCODE_LEFT_SHIFT*/ 0xF012,
    /*KEYCODE_BACKSLASH*/ 0xF05D,
    /*KEYCODE_KEY_Z*/ 0xF01A,
    /*KEYCODE_KEY_X*/ 0xF022,
    /*KEYCODE_KEY_C*/ 0xF021,
    /*KEYCODE_KEY_V*/ 0xF02A,
    /*KEYCODE_KEY_B*/ 0xF032,
    /*KEYCODE_KEY_N*/ 0xF031,
    /*KEYCODE_KEY_M*/ 0xF03A,
    /*KEYCODE_COMMA*/ 0xF041,
    /*KEYCODE_PERIOD*/ 0xF049,
    /*KEYCODE_SLASH*/ 0xF04A,
    /*KEYCODE_RIGHT_SHIFT*/ 0xF059,
    /*KEYCODE_KP_MUL*/ 0xF07C,
    /*KEYCODE_LEFT_ALT*/ 0xF011,
    /*KEYCODE_SPACE*/ 0xF029,
    /*KEYCODE_CAPS_LOCK*/ 0xF058,
    /*KEYCODE_F1*/ 0xF005,
    /*KEYCODE_F2*/ 0xF006,
    /*KEYCODE_F3*/ 0xF004,
    /*KEYCODE_F4*/ 0xF00C,
    /*KEYCODE_F5*/ 0xF003,
    /*KEYCODE_F6*/ 0xF00B,
    /*KEYCODE_F7*/ 0xF083,
    /*KEYCODE_F8*/ 0xF00A,
    /*KEYCODE_F9*/ 0xF001,
    /*KEYCODE_F10*/ 0xF009,
    /*KEYCODE_NUM_LOCK*/ 0xF077,
    /*KEYCODE_SCROLL_LOCK*/ 0xF07E,
    /*KEYCODE_KP_7*/ 0xF06C,
    /*KEYCODE_KP_8*/ 0xF075,
    /*KEYCODE_KP_9*/ 0xF07D,
    /*KEYCODE_KP_DASH*/ 0xF07B,
    /*KEYCODE_KP_4*/ 0xF06B,
    /*KEYCODE_KP_5*/ 0xF073,
    /*KEYCODE_KP_6*/ 0xF074,
    /*KEYCODE_KP_PLUS*/ 0xF079,
    /*KEYCODE_KP_1*/ 0xF069,
    /*KEYCODE_KP_2*/ 0xF072,
    /*KEYCODE_KP_3*/ 0xF07A,
    /*KEYCODE_KP_0*/ 0xF070,
    /*KEYCODE_KP_PERIOD*/ 0xF071,
    /*KEYCODE_F11*/ 0xF078,
    /*KEYCODE_F12*/ 0xF007,

    /*KEYCODE_PREV_TRACK*/ 0xE0F015,
    /*KEYCODE_NEXT_TRACK*/ 0xE0F04D,
    /*KEYCODE_KP_ENTER*/ 0xE0F05A,
    /*KEYCODE_RIGHT_CONTROL*/ 0xE0F014,
    /*KEYCODE_MUTE*/ 0xE0F023,
    /*KEYCODE_CALCULATOR*/ 0xE0F02B,
    /*KEYCODE_PLAY*/ 0xE0F034,
    /*KEYCODE_STOP*/ 0xE0F03B,
    /*KEYCODE_VOLUME_DOWN*/ 0xE0F021,
    /*KEYCODE_VOLUME_UP*/ 0xE0F032,
    /*KEYCODE_WWW_HOME*/ 0xE0F03A,
    /*KEYCODE_KP_SLASH*/ 0xE0F04A,
    /*KEYCODE_RIGHT_ALT*/ 0xE0F011,
    /*KEYCODE_HOME*/ 0xE0F06C,
    /*KEYCODE_CURSOR_UP*/ 0xE0F075,
    /*KEYCODE_CURSOR_LEFT*/ 0xE0F06B,
    /*KEYCODE_CURSOR_RIGHT*/ 0xE0F074,
    /*KEYCODE_END*/ 0xE0F069,
    /*KEYCODE_CURSOR_DOWN*/ 0xE0F072,
    /*KEYCODE_PAGE_DOWN*/ 0xE0F07A,
    /*KEYCODE_INSERT*/ 0xE0F070,
    /*KEYCODE_DELETE*/ 0xE0F071,
    /*KEYCODE_LEFT_GUI*/ 0xE0F01F,
    /*KEYCODE_RIGHT_GUI*/ 0xE0F027,
    /*KEYCODE_APPS*/ 0xE0F02F,
    /*KEYCODE_ACPI_POWER*/ 0xE0F037,
    /*KEYCODE_ACPI_SLEEP*/ 0xE0F03F,
    /*KEYCODE_ACPI_WAKE*/ 0xE0F05E,
    /*KEYCODE_WWW_SEARCH*/ 0xE0F010,
    /*KEYCODE_WWW_FAVORITES*/ 0xE0F018,
    /*KEYCODE_WWW_REFRESH*/ 0xE0F020,
    /*KEYCODE_WWW_STOP*/ 0xE0F028,
    /*KEYCODE_WWW_FORWARD*/ 0xE0F030,
    /*KEYCODE_WWW_BACK*/ 0xE0F038,
    /*KEYCODE_MY_COMPUTER*/ 0xE0F040,
    /*KEYCODE_EMAIL*/ 0xE0F048,
    /*KEYCODE_MEDIA_SELECT*/ 0xE0F050,
    /*KEYCODE_PRINTSCREEN*/ 0xE012E07C,
    /*KEYCODE_PAUSE*/ 0xE11477E1F014F077};

const char keycode_to_ascii_noshift[58] = {
    0,
    0,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    0,
    '\t',
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',
    0,
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    0,
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    0,
    0,
    0,
    ' ',
};

const char keycode_to_ascii_numpad[] = {
    '7',
    '8',
    '9',
    '-',
    '4',
    '5',
    '6',
    '+',
    '1',
    '2',
    '3',
    '0',
    '.',
};

const char keycode_to_ascii_shift[57] = {
    0,
    0,
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '_',
    '+',
    0,
    0,
    'Q',
    'W',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    '{',
    '}',
    '\n',
    0,
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    ':',
    '"',
    '~',
    0,
    '|',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',
    '<',
    '>',
    '?',
    0,
    0,
    0,
    0,
};

volatile bool keystates[189];

#define KEYBOARD_BUFFER_SIZE_D 64
const uint8_t KEYBOARD_BUFFER_SIZE = KEYBOARD_BUFFER_SIZE_D;

volatile extern uint8_t keyboard_buffer_end;
volatile uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE_D] = {};
volatile uint8_t keyboard_buffer_start = 0;

volatile bool is_numlock_on = true;
volatile bool is_capslock_on = false;
volatile bool is_scrolllock_on = false;

uint16_t process_keyboard()
{
    if (keyboard_buffer_start == keyboard_buffer_end)
        return 0;

    uint8_t kb_buff_start = keyboard_buffer_start;
    uint64_t scancode = keyboard_buffer[kb_buff_start];
    while (kb_buff_start != keyboard_buffer_end)
    {
        kb_buff_start = (kb_buff_start + 1) % KEYBOARD_BUFFER_SIZE;
        for (uint16_t i = 0; i < KEYCODE_COUNT; i++)
        {
            if (scancode_set_2_presscodes[i] == scancode)
            {
                keyboard_buffer_start = kb_buff_start;
                keystates[i] = true;
                return i;
            }

            if (scancode_set_2_releasecodes[i] == scancode)
            {
                keystates[i] = false;
                keyboard_buffer_start = kb_buff_start;
                return i | 0x100;
            }
        }
        scancode = (scancode << 8) | keyboard_buffer[kb_buff_start];
    }

    return 0;
}

char keycode_to_ascii(uint8_t keycode)
{
    if (keycode <= 57)
    {
        if (keystates[KEYCODE_LEFT_SHIFT] || keystates[KEYCODE_RIGHT_SHIFT])
            return keycode_to_ascii_shift[keycode];

        return keycode_to_ascii_noshift[keycode];
    }

    if (71 <= keycode && keycode <= 83)
    {
        return keycode_to_ascii_numpad[keycode];
    }

    if (keycode == KEYCODE_KP_SLASH)
    {
        return '/';
    }

    if (keycode == KEYCODE_KP_MUL)
    {
        return '*';
    }

    return 0;
}

void __attribute__((cdecl)) c_keyboard_interrupt(uint16_t __keycode)
{
    uint8_t keycode = __keycode;
    keyboard_buffer[keyboard_buffer_end] = keycode;
    keyboard_buffer_end = (keyboard_buffer_end + 1) % KEYBOARD_BUFFER_SIZE;
}