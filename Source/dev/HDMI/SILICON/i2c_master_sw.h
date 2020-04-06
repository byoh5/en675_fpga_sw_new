byte I2C_ReadByte(byte deviceID, byte offset);
void I2C_WriteByte(byte deviceID, byte offset, byte value);
byte I2C_ReadBlock(byte deviceID, byte offset, byte* buffer, word length);
byte I2C_ReadSegmentBlock(byte deviceID, byte segment, byte offset, byte *buffer, word length);
byte I2C_WriteBlock(byte deviceID, byte offset, byte* buffer, word length);
