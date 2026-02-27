#include "rom.hpp"


int EEPROM::read(void* dst, size_t nbyte, uint offset)
{
	return (i2c_i)->read(address, dst, nbyte, offset);
}

int EEPROM::write(const void* buffer, size_t nbyte, uint offset)
{
	return (i2c_i)->write(address, buffer, nbyte, offset);
}
