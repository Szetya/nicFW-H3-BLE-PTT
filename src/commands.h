#ifndef COMMANDS_H
#define COMMANDS_H

#define BLE_COMM_SERVICE                "0000FF00-0000-1000-8000-00805F9B34FB"
#define BLE_COMM_CHARACTERISTIC_NOTIFY  "0000FF01-0000-1000-8000-00805F9B34FB"
#define BLE_COMM_CHARACTERISTIC_WRITE   "0000FF02-0000-1000-8000-00805F9B34FB"

// Battery
#define CMD_READ_BATTERY            0x32    // Read battery radio>host Ack = 0x32, byte[2] 16 bit unsigned little endian
// 
#define CMD_DISABLE_RADIO_CHIP      0x45    // Disable radio chip - Ack = 0x45
#define CMD_ENABLE_RADIO_CHIP       0x46    // Enable radio chip - Ack = 0x46
// Lámpa parancsok
#define CMD_TURN_ON_FLASHLIGHT      0x47    // Lámpa bekapcsolása
#define CMD_TURN_OFF_FLASHLIGHT     0x48    // Lámpa kikapcsolása
// Reset command
#define CMD_RESET_RADIO             0x49    // RESET Radio
// Remote Sart / Stop
#define CMD_START_REMOTE            0x4A    // Start Remote Session
#define CMD_END_REMOTE              0x4B    // End Remote Session > Command mode

// Screen Commands
#define CMD_DRAW_TEXT               0x77    // Szöveg
#define CMD_FILL_REGION             0x78    // Régió kitöltés
#define CMD_SMETER_LEVEL            0x79    // S Meter Level
#define CMD_NOISEMETER_LEVEL        0x7A    // Noise Meter Level

// Channel Commands
#define CMD_READ_CHANNEL            0x30    // 0x30 byte[1]+1 channel number /radio > host: 0x30 byte[12] channel block byte[1] checksum of channel data
#define CMD_WRITE_CHANNEL           0x31    // 0x31 byte[1]+1 channel byte[12] channel block byte[1] checksum of channel data /radio > host 0x31

// Buttons
#define CMD_BTN_RELEASE             0xFF    // Releade
#define CMD_BTN_0                   0x80    // 0
#define CMD_BTN_1                   0x81    // 1
#define CMD_BTN_2                   0x82    // 2
#define CMD_BTN_3                   0x83    // 3
#define CMD_BTN_4                   0x84    // 4
#define CMD_BTN_5                   0x85    // 5
#define CMD_BTN_6                   0x86    // 6
#define CMD_BTN_7                   0x87    // 7
#define CMD_BTN_8                   0x88    // 8
#define CMD_BTN_9                   0x89    // 9
#define CMD_BTN_BLUE                0x8A    // Menu button
#define CMD_BTN_UP                  0x8B    // UP Arrow
#define CMD_BTN_DOWN                0x8C    // DOWN Arrow
#define CMD_BTN_RED                 0x8D    // RED Button 
#define CMD_BTN_STAR                0x8E    // * button
#define CMD_BTN_HASH                0x8F    // # button
#define CMD_BTN_PTT                 0x90    // PTT
#define CMB_BTN_PTT2                0x91    // Second PTT
#define CMD_BTN_LAMP                0x92    // Lamp button

// Delays
#define SHORTPRESS_DELAY            200     // Button short press delay CMD_BTN_XX this delay CMD_BTN_RELEASE
#define LONGPRESS_DELAY             1000    // 

#endif // COMMANDS_H
