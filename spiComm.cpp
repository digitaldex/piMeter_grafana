#include "spiComm.h"

spiComm::spiComm() {
    /* start Init of BCM2835 and Write ADE9000 Register to defined values for Measuring - See StartUp Routine in Datasheet */
    initSPI();
    writeSPI(W_RUN_REGISTER_STOP);
    writeSPI(W_PGA_GAIN_REGISTER);
    writeSPIlong(W_VLEVEL_REGISTER);
    writeSPI(W_ACCMODE_REGISTER);
    writeSPI(W_EP_CFG_REGISTER);
    writeSPI(W_EGY_TIME_REGISTER);
    writeSPI(W_RUN_REGISTER_START);
    usleep(500000);
}

spiComm::~spiComm() {
    closeSPI();
}

int spiComm::initSPI() {
    /* Initialize the Raspberry SPI Bus and Enable ADE9000 */
    bcm2835_init();
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PIN, HIGH);
    if(!bcm2835_init()) {
        return 1;
    }
    if(!bcm2835_spi_begin()) {
        return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    return 0;
}

void spiComm::closeSPI() {
    bcm2835_spi_end();
    bcm2835_close();
}

void spiComm::resizeWord16(char byte[], unsigned short word) {
    /* Split 16bit SPI Commmand Word into 2 Bytes for Read Operation */
    byte[0] = word >> 8;
    byte[1] = word & 0xFF;
}

void spiComm::resizeWord32(char byte[], unsigned int word) {
    /* Split 32bit SPI Commmand Word into 4 Bytes for Write Operation */
    byte[0] = word >> 24;
    byte[1] = word >> 16;
    byte[2] = word >> 8;
    byte[3] = word & 0xFF;
}

void spiComm::resizeWord48(char byte[], uint64_t word) {
    /* Split 48bit SPI Commmand Word into 6 Bytes for Write Operation */
    byte[0] = word >> 40;
    byte[1] = word >> 32;
    byte[2] = word >> 24;
    byte[3] = word >> 16;
    byte[4] = word >> 8;
    byte[5] = word & 0xFF;
}

uint32_t spiComm::parse32bitReturnValue(char byte[]) {
    /* Shift Char Array into uint32_t */
    /* First two Bytes are SPI Read Command Echo */
    /* Last two Bytes are CRC for Debug purpose */
    uint32_t value = 0;
    value = (uint8_t)byte[2];
    value <<= 8;
    value |= (uint8_t)byte[3];
    value <<= 8;
    value |= (uint8_t)byte[4];
    value <<= 8;
    value |= (uint8_t)byte[5];
    /* DEBUG
       uint16_t crc = 0;
       crc = (uint8_t)byte[6];
       crc <<= 8;
       crc |= (uint8_t)byte[7];
       cout << "DEBUG: " << hex << value << " CRC: " << crc << endl;
     */
    return value;
}

uint16_t spiComm::parse16bitReturnValue(char byte[]) {
    /* Shift Char Array into uint16_t */
    /* First two Bytes are SPI Read Command Echo */
    uint16_t value = 0;
    value = (uint8_t)byte[2];
    value <<= 8;
    value |= (uint8_t)byte[3];
    return value;
}

void spiComm::writeSPI(unsigned int word) {
    /* The Register Write WORD is shifted into 4 Bytes */
    /* and gets transfered over SPI Bus */
    char spiWriteCommand[4];
    resizeWord32(spiWriteCommand, word);
    bcm2835_spi_transfern(spiWriteCommand, WORD_SIZE_WRITE16);
}

void spiComm::writeSPIlong(uint64_t word) {
    /* The Register Write WORD is shifted into 6 Bytes */
    /* and gets transfered over SPI Bus */
    char spiWriteCommand[6];
    resizeWord48(spiWriteCommand, word);
    bcm2835_spi_transfern(spiWriteCommand, WORD_SIZE_WRITE32);
}

float spiComm::readCurrent(unsigned short word) {
    /* The Register Read WORD is shifted into 2 Bytes */
    /* and gets transfered over SPI Bus */
    /* The Return Value is Float with 2 digits */
    char spiReadCommand[2];
    char spiMessageBuffer[8];
    float spiReturnValue = 0;
    resizeWord16(spiReadCommand, word);
    bcm2835_spi_transfernb(spiReadCommand, spiMessageBuffer, WORD_SIZE_READ);
    spiReturnValue = parse32bitReturnValue(spiMessageBuffer);
    spiReturnValue = spiReturnValue * CurrentConstant * pow(10, -6);
    return spiReturnValue;
}

float spiComm::readVoltage(unsigned short word) {
    /* The Register Read WORD is shifted into 2 Bytes */
    /* and gets transfered over SPI Bus */
    /* The Return Value is Float with 2 digits */
    char spiReadCommand[2];
    char spiMessageBuffer[8];
    float spiReturnValue = 0;
    resizeWord16(spiReadCommand, word);
    bcm2835_spi_transfernb(spiReadCommand, spiMessageBuffer, WORD_SIZE_READ);
    spiReturnValue = parse32bitReturnValue(spiMessageBuffer);
    spiReturnValue = spiReturnValue * VoltageConstant * pow(10, -6);
    return spiReturnValue;
}

float spiComm::readPower(unsigned short word) {
    /* The Register Read WORD is shifted into 2 Bytes */
    /* and gets transfered over SPI Bus */
    /* The Return Value is Float with 2 digits */
    char spiReadCommand[2];
    char spiMessageBuffer[8];
    float spiReturnValue = 0;
    resizeWord16(spiReadCommand, word);
    bcm2835_spi_transfernb(spiReadCommand, spiMessageBuffer, WORD_SIZE_READ);
    spiReturnValue = parse32bitReturnValue(spiMessageBuffer);
    spiReturnValue = spiReturnValue * PowerConstant * pow(10, -3);
    return spiReturnValue;
}

float spiComm::readEnergy(unsigned short word) {
    /* The Register Read WORD is shifted into 2 Bytes */
    /* and gets transfered over SPI Bus */
    /* The Return Value is Float with 2 digits */
    char spiReadCommand[2];
    char spiMessageBuffer[8];
    float spiReturnValue = 0;
    resizeWord16(spiReadCommand, word);
    bcm2835_spi_transfernb(spiReadCommand, spiMessageBuffer, WORD_SIZE_READ);
    spiReturnValue = parse32bitReturnValue(spiMessageBuffer);
    spiReturnValue = spiReturnValue * EnergyConstant * pow(10, -6);
    return spiReturnValue;
}
