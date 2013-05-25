/*
 Copyright (c) 2010 by arms22 (arms22 at gmail.com)
 Microchip 23x256 SPI SRAM library for Arduino

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <Arduino.h>
#include <SPI.h>

#include "SerialFRAM.h"

void SerialFRAM::init() {
	pinMode(_cspin, OUTPUT);
	csHigh();
	_address = 0;
//	select();
//	writeStatusRegister(SEQ_MODE);
//	deselect();
}

byte SerialFRAM::status(void) {
	byte stat;
	select();
	SPI.transfer(FM25_RDSR);
	stat = SPI.transfer(0);
	deselect();
	return stat;
}

byte SerialFRAM::status(byte val) {
	byte stat;
	select();
	SPI.transfer(FM25_WRSR);
	stat = SPI.transfer(val);
	deselect();
	return stat;
}

int SerialFRAM::read(const uint32_t & addr, byte *buffer, const size_t & size) {
	select();
	SPI.transfer(FM25_READ);
	SPI.transfer( addr >>8 & 0xff);
	SPI.transfer( addr & 0xff);
	for (uint16_t i = 0; i < size; i++)
		*buffer++ = SPI.transfer(0);
	deselect();
	return size;
}

int SerialFRAM::read(const uint32_t & addr) {
	int data;
	read(addr,(byte*) &data, 1);
	return data;
}


size_t SerialFRAM::write(const uint32_t & addr, byte *buffer, const size_t & size) {
	select();
	SPI.transfer(FM25_WREN); // write must be enabled
	csHigh();
	csLow();
	SPI.transfer(FM25_WRITE);
	SPI.transfer( addr >>8 & 0xff);
	SPI.transfer( addr & 0xff);
	for (uint16_t i = 0; i < size; i++)
		SPI.transfer(*buffer++);
	deselect();
	return size;
}

size_t SerialFRAM::write(const uint32_t & addr, byte data) {
	write(addr, &data, 1);
	return 1;
}

