#ifndef EEPromCRC_h
#define EEPromCRC_h

#include <EEProm.h>

class EEPromCRC {
  private:
    // Checksum table
    const uint32_t crcTable[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };
    // Properties
    uint32_t crcActive = 0;
    uint16_t crcAddress = 1019;
    int dataStorageStart = 0;
    int dataStorageEnd = 0;
    bool crcValid = false;

    // Get crc for EEPROM starting at eeAddress, checking from eeAdress to dataSize
    uint32_t calculateCrc() {
      uint32_t crc = ~0L;
      for (int index = this -> dataStorageStart; index < this -> dataStorageEnd; index++) {
        crc = this -> crcTable[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
        crc = this -> crcTable[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
      }
      return crc;
    }

    // Get stored crc
    void crcGet() {
      EEPROM.get(this -> crcAddress, this -> crcActive);
    }
    
  public:
    // Constructor
    EEPromCRC(uint16_t crcAddress, int dataStorageStart, int dataStorageLength);

    // Put crc value to EEPROM
    void crcPut();
    
    // Check stored crc against calculated crc
    void crcCheck();

    // Get state of crc
    bool crcIsValid();
};

#endif