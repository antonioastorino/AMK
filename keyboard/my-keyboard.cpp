#include "common.h"
#include "my-keyboard.h"

#if defined(_USING_HID)

//	Keyboard

static const uint8_t _hidReportDescriptor[] PROGMEM = {

    //  Keyboard
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x85, 0x02, //   REPORT_ID (2)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)

    0x95, 0x08, //   REPORT_COUNT (8)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0x95, 0x01, //   REPORT_COUNT (1)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x81, 0x03, //   INPUT (Cnst,Var,Abs)

    0x95, 0x06, //   REPORT_COUNT (6)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x73, //   LOGICAL_MAXIMUM (115)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73, //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, //   INPUT (Data,Ary,Abs)
    0xc0,       // END_COLLECTION
};

Keyboard_::Keyboard_(void)
{
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

void Keyboard_::begin(void) {}

void Keyboard_::end(void) {}

void Keyboard_::sendReport() { HID().SendReport(2, &_keyReport, sizeof(KeyReport)); }

uint8_t USBPutChar(uint8_t c);

size_t Keyboard_::pressRaw(uint8_t k, uint8_t index)
{
    _keyReport.keys[index] = k;
    SERIAL_PRINT("Key: 0x");
    SERIAL_PRINT(k, HEX);
    SERIAL_PRINTLN();
    return 1;
}

void Keyboard_::setModifiers(uint8_t k)
{
    _keyReport.modifiers |= k;
    SERIAL_PRINT("Modifier: 0x");
    SERIAL_PRINT(k, HEX);
    SERIAL_PRINTLN();
}

void Keyboard_::releaseAll(void)
{
    _keyReport.keys[0]   = 0;
    _keyReport.keys[1]   = 0;
    _keyReport.keys[2]   = 0;
    _keyReport.keys[3]   = 0;
    _keyReport.keys[4]   = 0;
    _keyReport.keys[5]   = 0;
    _keyReport.modifiers = 0;
}
size_t Keyboard_::write(const uint8_t c) {}

size_t Keyboard_::write(const uint8_t* buffer, size_t size)
{
    size_t n = 0;
    while (size--)
    {
        if (*buffer != '\r')
        {
            if (write(*buffer))
            {
                n++;
            }
            else
            {
                break;
            }
        }
        buffer++;
    }
    return n;
}

Keyboard_ Keyboard;

#endif
