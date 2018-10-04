#ifndef SERIALDATA_H
#define SERIALDATA_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"
#include "LEDControl.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

// max length of data packets
const U32 MAX_SERIAL_PACKET_LEN = 13;
const U8  PREAMBLE = 0xEE;

struct PacketData
{
    U8 data[MAX_SERIAL_PACKET_LEN];  // raw packet data
};

// turn byte-packing on
#pragma pack(push, 1)

//----------------------------------------------------------------------------
//  Generic Packet Header
//----------------------------------------------------------------------------
struct DataHeader
{
    U8 preamble;
    U8 length;    // Length of packet payload (bytes)

    // LED command.
    enum Command
    {
        MODE_CMD     = 0x00,
        COLOR_CMD    = 0x01,
        SPECTRUM_CMD = 0x02
    } command : 8;
};

//----------------------------------------------------------------------------
//  Generic Packet Footer
//----------------------------------------------------------------------------
struct DataFooter
{
    U16 crc;  // CRC value.
};


//----------------------------------------------------------------------------
//  Mode Command - 0x00
//----------------------------------------------------------------------------
struct ModeCommand
{
    DataHeader header;

    // Desired LED mode.
    LEDModes mode;

    DataFooter footer;
};


//----------------------------------------------------------------------------
//  Color Command - 0x01
//----------------------------------------------------------------------------
struct ColorCommand
{
    DataHeader header;

    // Desired LED color.
    U8 red;
    U8 green;
    U8 blue;

    DataFooter footer;
};


//----------------------------------------------------------------------------
//  Spectrum Command - 0x02
//----------------------------------------------------------------------------
struct SpectrumCommand
{
    DataHeader header;

    // Desired LED color.
    U8 ledHeight[NUM_LED_STRIPS];

    DataFooter footer;
};

#pragma pack(pop)

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
