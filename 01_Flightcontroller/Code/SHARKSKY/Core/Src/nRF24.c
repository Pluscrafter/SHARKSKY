#include "nRF24.h"

uint8_t RF24_readRegister(uint8_t reg, uint8_t* buf, uint8_t len){
	uint8_t status;
	uint8_t temp = (R_REGISTER | ( REGISTER_MASK & reg));

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &temp, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, buf, len, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}

uint8_t _RF24_readRegister(uint8_t reg){
	uint8_t result;
	uint8_t temp = (R_REGISTER | ( REGISTER_MASK & reg));

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, &result, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return result;
}

uint8_t RF24_writeRegister(uint8_t reg, uint8_t* buf, uint8_t len){
	uint8_t status;
	uint8_t temp = (W_REGISTER | ( REGISTER_MASK & reg));

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &temp, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, (uint8_t *) buf, len, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}

uint8_t __RF24_writeRegister(uint8_t reg, const uint8_t* buf, uint8_t len){
	uint8_t status;
	uint8_t temp = (W_REGISTER | ( REGISTER_MASK & reg));

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &temp, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, (uint8_t *) buf, len, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}

uint8_t _RF24_writeRegister(uint8_t reg, uint8_t value){
	uint8_t status;
	uint8_t temp = (W_REGISTER | ( REGISTER_MASK & reg));

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &temp, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, &value, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}

uint8_t RF24_write_payload(const void* buf, uint8_t data_len, uint8_t writeType){
	uint8_t status, temp;
	const uint8_t* current = (const uint8_t*)(buf);

	data_len = rf24_min(data_len, payload_size);
	uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &writeType, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, (uint8_t *) current, data_len, HAL_MAX_DELAY);
	temp = 0;
	while (blank_len--)
	{
		HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	}

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}


uint8_t RF24_read_payload(void* buf, uint8_t data_len){
	uint8_t status;
	uint8_t* current = (uint8_t*)(buf);
	uint8_t temp = R_RX_PAYLOAD;
	if (data_len > payload_size)
		data_len = payload_size;
	uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

	//printf("[Reading %u bytes %u blanks]",data_len,blank_len);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &temp, &status, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, current, data_len, HAL_MAX_DELAY);
	temp = 1;
	while (blank_len--)
	{
		HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	}

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}

uint8_t RF24_flush_rx(void){
	return RF24_spiTrans( FLUSH_RX);
}


uint8_t RF24_flush_tx(void){
	return RF24_spiTrans( FLUSH_TX);
}

uint8_t RF24_spiTrans(uint8_t cmd){

	uint8_t status;

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi2, &cmd, &status, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	return status;
}


uint8_t RF24_get_status(void){
	return RF24_spiTrans(RF24_NOP);
}

void RF24_setChannel(uint8_t channel){
	const uint8_t max_channel = 125;
	_RF24_writeRegister(RF_CH, rf24_min(channel, max_channel));
}

uint8_t RF24_getChannel(){
	return _RF24_readRegister(RF_CH);
}

void RF24_setPayloadSize(uint8_t size){
	payload_size = rf24_min(size, 32);
}


uint8_t RF24_getPayloadSize(void){
	return payload_size;
}

/************************************************************/
bool RF24_begin(void)
{

	uint8_t setup = 0;

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);

	HAL_Delay(5);

	// Reset NRF_CONFIG and enable 16-bit CRC.
	_RF24_writeRegister(CONFIG, 0x0C);

	// Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
	// WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
	// sizes must never be used. See documentation for a more complete explanation.
	RF24_setRetries(5, 15);

	// Reset value is MAX
	//setPALevel( RF24_PA_MAX ) ;

	// check for connected module and if this is a p nRF24l01 variant
	//
	if (RF24_setDataRate(RF24_250KBPS))
	{
		p_variant = true;
	}
	setup = _RF24_readRegister(RF_SETUP);
	/*if( setup == 0b00001110 )     // register default for nRF24L01P
	 {
	 p_variant = true ;
	 }*/

	// Then set the data rate to the slowest (and most reliable) speed supported by all
	// hardware.
	RF24_setDataRate(RF24_1MBPS);

	// Initialize CRC and request 2-byte (16bit) CRC
	//setCRCLength( RF24_CRC_16 ) ;

	// Disable dynamic payloads, to match dynamic_payloads_enabled setting - Reset value is 0
	RF24_toggle_features();
	_RF24_writeRegister(FEATURE, 0);
	_RF24_writeRegister(DYNPD, 0);
	dynamic_payloads_enabled = false;

	// Reset current status
	// Notice reset and flush is the last thing we do
	_RF24_writeRegister(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));

	// Set up default configuration.  Callers can always change it later.
	// This channel should be universally safe and not bleed over into adjacent
	// spectrum.
	RF24_setChannel(76);

	// Flush buffers
	RF24_flush_rx();
	RF24_flush_tx();

	RF24_powerUp(); //Power up by default when begin() is called

	// Enable PTX, do not write CE high so radio will remain in standby I mode ( 130us max to transition to RX or TX instead of 1500us from powerUp )
	// PTX should use only 22uA of power
	_RF24_writeRegister(CONFIG, (_RF24_readRegister(CONFIG)) & ~_BV(PRIM_RX));

	// if setup is 0 or ff then there was no response from module
	return (setup != 0 && setup != 0xff);
}

/****************************************************************************/

bool RF24_isChipConnected(){
	uint8_t setup = _RF24_readRegister(SETUP_AW);
	if (setup >= 1 && setup <= 3)
	{
		return true;
	}

	return false;
}

/****************************************************************************/

void RF24_startListening(void){
	RF24_powerUp();
	_RF24_writeRegister(CONFIG, _RF24_readRegister(CONFIG) | _BV(PRIM_RX));
	_RF24_writeRegister(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
	// Restore the pipe0 adddress, if exists
	if (pipe0_reading_address[0] > 0)
	{
		RF24_writeRegister(RX_ADDR_P0, pipe0_reading_address, addr_width);
	}
	else
	{
		RF24_closeReadingPipe(0);
	}

	// Flush buffers
	//flush_rx();
	if (_RF24_readRegister(FEATURE) & _BV(EN_ACK_PAY))
	{
		RF24_flush_tx();
	}

	// Go!
	//HAL_DelayHAL_MAX_DELAY;//us
}

/****************************************************************************/
static const uint8_t child_pipe_enable[] = //look here again "PROGMEM"
		{
		ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5 };

void RF24_stopListening(void){
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);

	HAL_Delay(RF24_txDelay); //us

	if (_RF24_readRegister(FEATURE) & _BV(EN_ACK_PAY))
	{
		HAL_Delay(RF24_txDelay); //200us
		RF24_flush_tx();
	}
	//flush_rx();
	_RF24_writeRegister(CONFIG, (_RF24_readRegister(CONFIG)) & ~_BV(PRIM_RX));
	_RF24_writeRegister(EN_RXADDR, _RF24_readRegister(EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[0]))); // Enable RX on pipe0

	//HAL_DelayHAL_MAX_DELAY;//us

}

/****************************************************************************/

void RF24_powerDown(void){
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET); // Guarantee CE is low on powerDown
	_RF24_writeRegister(CONFIG, _RF24_readRegister(CONFIG) & ~_BV(PWR_UP));
}

/****************************************************************************/

//Power up now. Radio will not power down unless instructed by MCU for config changes etc.
void RF24_powerUp(void){
	uint8_t cfg = _RF24_readRegister(CONFIG);

	// if not powered up then power up and wait for the radio to initialize
	if (!(cfg & _BV(PWR_UP)))
	{
		_RF24_writeRegister(CONFIG, cfg | _BV(PWR_UP));

		// For nRF24L01+ to go from power down mode to TX or RX mode it must first pass through stand-by mode.
		// There must be a delay of Tpd2stby (see Table 16.) after the nRF24L01+ leaves power down mode before
		// the CEis set high. - Tpd2stby can be up to 5ms per the 1.0 datasheet
		HAL_Delay(5);
	}
}



//Similar to the previous write, clears the interrupt flags
bool _RF24_write(const void* buf, uint8_t len, const bool multicast){
	//Start Writing
	_RF24_startFastWrite(buf, len, multicast);

	while (!(RF24_get_status() & ( _BV(TX_DS) | _BV(MAX_RT)))){

	}

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);

	uint8_t status = _RF24_writeRegister(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));

	//Max retries exceeded
	if (status & _BV(MAX_RT))
	{
		RF24_flush_tx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
		return 0;
	}
	//TX OK 1 or 0
	return 1;
}

bool RF24_write(const void* buf, uint8_t len)
{
	return _RF24_write(buf, len, 0);
}
/****************************************************************************/

//For general use, the interrupt flags are not important to clear
bool RF24_writeBlocking(const void* buf, uint8_t len, uint32_t timeout){
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//This way the FIFO will fill up and allow blocking until packets go through
	//The radio will auto-clear everything in the FIFO as long as CE remains high

	uint32_t timer = HAL_GetTick();							  //Get the time that the payload transmission started

	while ((RF24_get_status() & (_BV(TX_FULL))))
	{		  //Blocking only if FIFO is full. This will loop and block until TX is successful or timeout

		if (RF24_get_status() & _BV(MAX_RT))
		{					  //If MAX Retries have been reached
			RF24_reUseTX();										  //Set re-transmit and clear the MAX_RT interrupt flag
			if (HAL_GetTick() - timer > timeout)
			{
				return 0;
			}		  //If this payload has exceeded the user-defined timeout, exit and return 0
		}

	}

	//Start Writing
	_RF24_startFastWrite(buf, len, 0);								  //Write the payload if a buffer is clear

	return 1;												  //Return 1 to indicate successful transmission
}

/****************************************************************************/

void RF24_reUseTX(){
	_RF24_writeRegister(STATUS, _BV(MAX_RT));			  //Clear max retry flag
	RF24_spiTrans( REUSE_TX_PL);
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);										  //Re-Transfer packet
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
}

/****************************************************************************/

bool _RF24_writeFast(const void* buf, uint8_t len, const bool multicast){
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//Return 0 so the user can control the retrys and set a timer or failure counter if required
	//The radio will auto-clear everything in the FIFO as long as CE remains high

	while ((RF24_get_status() & (_BV(TX_FULL))))
	{			  //Blocking only if FIFO is full. This will loop and block until TX is successful or fail

		if (RF24_get_status() & _BV(MAX_RT))
		{
			//reUseTX();										  //Set re-transmit
			_RF24_writeRegister(STATUS, _BV(MAX_RT));			  //Clear max retry flag
			return 0;										  //Return 0. The previous payload has been retransmitted
															  //From the user perspective, if you get a 0, just keep trying to send the same payload
		}

	}
	//Start Writing
	_RF24_startFastWrite(buf, len, multicast);

	return 1;
}

bool RF24_writeFast(const void* buf, uint8_t len){
	return _RF24_writeFast(buf, len, 0);
}

/****************************************************************************/

//Per the documentation, we want to set PTX Mode when not listening. Then all we do is write data and set CE high
//In this mode, if we can keep the FIFO buffers loaded, packets will transmit immediately (no 130us delay)
//Otherwise we enter Standby-II mode, which is still faster than standby mode
//Also, we remove the need to keep writing the config register over and over and delaying for 150 us each time if sending a stream of data
void RF24_startFastWrite(const void* buf, uint8_t len, const bool multicast, bool startTx){ //TMRh20

	//write_payload( buf,len);
	RF24_write_payload(buf, len, multicast ? W_TX_PAYLOAD_NO_ACK : W_TX_PAYLOAD);
	if (startTx)
	{
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
	}

}

void _RF24_startFastWrite(const void* buf, uint8_t len, const bool multicast){ //TMRh20

	RF24_startFastWrite(buf,  len, multicast, 1);
}

/****************************************************************************/

//Added the original startWrite back in so users can still use interrupts, ack payloads, etc
//Allows the library to pass all tests
void RF24_startWrite(const void* buf, uint8_t len, const bool multicast){

	// Send the payload

	//write_payload( buf, len );
	RF24_write_payload(buf, len, multicast ? W_TX_PAYLOAD_NO_ACK : W_TX_PAYLOAD);
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
#if !defined(F_CPU) || F_CPU > 20000000
	HAL_Delay(10); //us
#endif
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);

}

/****************************************************************************/

bool RF24_rxFifoFull(){
	return _RF24_readRegister(FIFO_STATUS) & _BV(RX_FULL);
}
/****************************************************************************/

bool RF24_txStandBy(){

	while (!(_RF24_readRegister(FIFO_STATUS) & _BV(TX_EMPTY)))
	{
		if (RF24_get_status() & _BV(MAX_RT))
		{
			_RF24_writeRegister(STATUS, _BV(MAX_RT));
			HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
			RF24_flush_tx();    //Non blocking, flush the data
			return 0;
		}
	}

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);			   //Set STANDBY-I mode
	return 1;
}

/****************************************************************************/

bool _RF24_txStandBy(uint32_t timeout, bool startTx){

	if (startTx)
	{
		RF24_stopListening();
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
	}
	uint32_t start = HAL_GetTick();

	while (!(_RF24_readRegister(FIFO_STATUS) & _BV(TX_EMPTY)))
	{
		if (RF24_get_status() & _BV(MAX_RT))
		{
			_RF24_writeRegister(STATUS, _BV(MAX_RT));
			HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);										  //Set re-transmit
			HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
			if (HAL_GetTick() - start >= timeout)
			{
				HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
				RF24_flush_tx();
				return 0;
			}
		}
	}

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);				   //Set STANDBY-I mode
	return 1;

}

bool __RF24_txStandBy(uint32_t timeout){
	return _RF24_txStandBy(timeout, 0);
}

/****************************************************************************/

void RF24_maskIRQ(bool tx, bool fail, bool rx){

	uint8_t config = _RF24_readRegister(CONFIG);
	/* clear the interrupt flags */
	config &= ~(1 << MASK_MAX_RT | 1 << MASK_TX_DS | 1 << MASK_RX_DR);
	/* set the specified interrupt flags */
	config |= fail << MASK_MAX_RT | tx << MASK_TX_DS | rx << MASK_RX_DR;
	_RF24_writeRegister(CONFIG, config);
}

/****************************************************************************/

uint8_t RF24_getDynamicPayloadSize(void){
	uint8_t result = 0;
	uint8_t temp = R_RX_PL_WID;
	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, &result, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

	if (result > 32)
	{
		RF24_flush_rx();
		HAL_Delay(100);
		return 0;
	}
	return result;
}

/****************************************************************************/

bool RF24_available(void){
	return _RF24_available(NULL);
}

/****************************************************************************/

bool _RF24_available(uint8_t* pipe_num){
	if (!(_RF24_readRegister(FIFO_STATUS) & _BV(RX_EMPTY)))
	{

		// If the caller wants the pipe number, include that
		if (pipe_num)
		{
			uint8_t status = RF24_get_status();
			*pipe_num = (status >> RX_P_NO) & 0x07;
		}
		return 1;
	}

	return 0;

}

/****************************************************************************/

void RF24_read(void* buf, uint8_t len){

	// Fetch the payload
	RF24_read_payload(buf, len);

	//Clear the two possible interrupt flags with one command
	_RF24_writeRegister(STATUS, _BV(RX_DR) | _BV(MAX_RT) | _BV(TX_DS));

}

/****************************************************************************/

void RF24_whatHappened(uint8_t* tx_ok, uint8_t* tx_fail, uint8_t* rx_ready){
	// Read the status & reset the status in one easy call
	// Or is that such a good idea?
	uint8_t status = _RF24_writeRegister(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));

	// Report to the user what happened
	*tx_ok = status & _BV(TX_DS);
	*tx_fail = status & _BV(MAX_RT);
	*rx_ready = status & _BV(RX_DR);
}

/****************************************************************************/

void _RF24_openWritingPipe(uint64_t value){
	// Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
	// expects it LSB first too, so we're good.

	RF24_writeRegister(RX_ADDR_P0, (uint8_t*)(&value), addr_width);
	RF24_writeRegister(TX_ADDR, (uint8_t*)(&value), addr_width);

	//const uint8_t max_payload_size = 32;
	//write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
	_RF24_writeRegister(RX_PW_P0, payload_size);
}

/****************************************************************************/
void RF24_openWritingPipe(const uint8_t *address){
	// Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
	// expects it LSB first too, so we're good.

	__RF24_writeRegister(RX_ADDR_P0, address, addr_width);
	__RF24_writeRegister(TX_ADDR, address, addr_width);

	//const uint8_t max_payload_size = 32;
	//write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
	_RF24_writeRegister(RX_PW_P0, payload_size);
}

/****************************************************************************/
static const uint8_t child_pipe[] =
{
RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5 };
static const uint8_t child_payload_size[] =
{
RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5 };

void _RF24_openReadingPipe(uint8_t child, uint64_t address){
	// If this is pipe 0, cache the address.  This is needed because
	// openWritingPipe() will overwrite the pipe 0 address, so
	// startListening() will have to restore it.
	if (child == 0)
	{
		memcpy(pipe0_reading_address, &address, addr_width);
	}

	if (child <= 6)
	{
		// For pipes 2-5, only write the LSB
		if (child < 2)
			RF24_writeRegister(pgm_read_byte(&child_pipe[child]), (uint8_t*)(&address), addr_width);
		else
			RF24_writeRegister(pgm_read_byte(&child_pipe[child]), (uint8_t*)(&address), 1);

		_RF24_writeRegister(pgm_read_byte(&child_payload_size[child]), payload_size);

		// Note it would be more efficient to set all of the bits for all open
		// pipes at once.  However, I thought it would make the calling code
		// more simple to do it this way.
		_RF24_writeRegister(EN_RXADDR, _RF24_readRegister(EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[child])));
	}
}

/****************************************************************************/
void RF24_setAddressWidth(uint8_t a_width){

	if (a_width -= 2)
	{
		_RF24_writeRegister(SETUP_AW, a_width % 4);
		addr_width = (a_width % 4) + 2;
	}
	else
	{
		_RF24_writeRegister(SETUP_AW, 0);
		addr_width = 2;
	}

}

/****************************************************************************/

void RF24_openReadingPipe(uint8_t child, const uint8_t *address){
	// If this is pipe 0, cache the address.  This is needed because
	// openWritingPipe() will overwrite the pipe 0 address, so
	// startListening() will have to restore it.
	if (child == 0)
	{
		memcpy(pipe0_reading_address, address, addr_width);
	}
	if (child <= 6)
	{
		// For pipes 2-5, only write the LSB
		if (child < 2)
		{
			__RF24_writeRegister(pgm_read_byte(&child_pipe[child]), address, addr_width);
		}
		else
		{
			__RF24_writeRegister(pgm_read_byte(&child_pipe[child]), address, 1);
		}
		_RF24_writeRegister(pgm_read_byte(&child_payload_size[child]), payload_size);

		// Note it would be more efficient to set all of the bits for all open
		// pipes at once.  However, I thought it would make the calling code
		// more simple to do it this way.
		_RF24_writeRegister(EN_RXADDR, _RF24_readRegister(EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[child])));

	}
}

/****************************************************************************/

void RF24_closeReadingPipe(uint8_t pipe){
	_RF24_writeRegister(EN_RXADDR, _RF24_readRegister(EN_RXADDR) & ~_BV(pgm_read_byte(&child_pipe_enable[pipe])));
}

/****************************************************************************/

void RF24_toggle_features(void){
	uint8_t temp = ACTIVATE;
	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	temp = 0x73;
	HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);
}

/****************************************************************************/

void RF24_enableDynamicPayloads(void){
	// Enable dynamic payload throughout the system

	//toggle_features();
	_RF24_writeRegister(FEATURE, _RF24_readRegister(FEATURE) | _BV(EN_DPL));

	// Enable dynamic payload on all pipes
	//
	// Not sure the use case of only having dynamic payload on certain
	// pipes, so the library does not support it.
	_RF24_writeRegister(DYNPD, _RF24_readRegister(DYNPD) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) | _BV(DPL_P1) | _BV(DPL_P0));

	dynamic_payloads_enabled = true;
}

/****************************************************************************/
void RF24_disableDynamicPayloads(void){
	// Disables dynamic payload throughout the system.  Also disables Ack Payloads

	//toggle_features();
	_RF24_writeRegister(FEATURE, 0);

	// Disable dynamic payload on all pipes
	//
	// Not sure the use case of only having dynamic payload on certain
	// pipes, so the library does not support it.
	_RF24_writeRegister(DYNPD, 0);

	dynamic_payloads_enabled = false;
}

/****************************************************************************/

void RF24_enableAckPayload(void){
	//
	// enable ack payload and dynamic payload features
	//

	//toggle_features();
	_RF24_writeRegister(FEATURE, _RF24_readRegister(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL));

	//
	// Enable dynamic payload on pipes 0 & 1
	//

	_RF24_writeRegister(DYNPD, _RF24_readRegister(DYNPD) | _BV(DPL_P1) | _BV(DPL_P0));
	dynamic_payloads_enabled = true;
}

/****************************************************************************/

void RF24_enableDynamicAck(void){
	//
	// enable dynamic ack features
	//
	//toggle_features();
	_RF24_writeRegister(FEATURE, _RF24_readRegister(FEATURE) | _BV(EN_DYN_ACK));
}

/****************************************************************************/

void RF24_writeAckPayload(uint8_t pipe, const void* buf, uint8_t len){
	const uint8_t* current = (const uint8_t*)(buf);
	uint8_t temp = (W_ACK_PAYLOAD | (pipe & 0x07));

	uint8_t data_len = rf24_min(len, 32);


	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2, &temp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, (uint8_t *) current, data_len, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(RF24_NSS_GPIO_Port, RF24_NSS_Pin, GPIO_PIN_SET);

}

/****************************************************************************/

bool RF24_isAckPayloadAvailable(void){
	return !(_RF24_readRegister(FIFO_STATUS) & _BV(RX_EMPTY));
}

/****************************************************************************/

bool RF24_isPVariant(void){
	return p_variant;
}

/****************************************************************************/

void RF24_setAutoAck(bool enable){
	if (enable)
		_RF24_writeRegister(EN_AA, 0x3F);
	else
		_RF24_writeRegister(EN_AA, 0);
}

/****************************************************************************/

void _RF24_setAutoAck(uint8_t pipe, bool enable){
	if (pipe <= 6)
	{
		uint8_t en_aa = _RF24_readRegister( EN_AA);
		if (enable)
		{
			en_aa |= _BV(pipe);
		}
		else
		{
			en_aa &= ~_BV(pipe);
		}
		_RF24_writeRegister( EN_AA, en_aa);
	}
}

/****************************************************************************/

bool RF24_testCarrier(void){
	return (_RF24_readRegister(CD) & 1);
}

/****************************************************************************/

bool RF24_testRPD(void){
	return (_RF24_readRegister(RPD) & 1);
}

/****************************************************************************/

void RF24_setPALevel(uint8_t level){

	uint8_t setup = _RF24_readRegister(RF_SETUP) & 0xF8;

	if (level > 3){  						// If invalid level, go to max PA
		level = (RF24_PA_MAX << 1) + 1;		// +1 to support the SI24R1 chip extra bit
	}
	else{
		level = (level << 1) + 1;	 		// Else set level as requested
	}

	_RF24_writeRegister( RF_SETUP, setup |= level);	// Write it to the chip
}

/****************************************************************************/

uint8_t RF24_getPALevel(void){

	return (_RF24_readRegister(RF_SETUP) & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH))) >> 1;
}

/****************************************************************************/

bool RF24_setDataRate(rf24_datarate_e speed){
	bool result = false;
	uint8_t setup = _RF24_readRegister(RF_SETUP);

	// HIGH and LOW '00' is 1Mbs - our default
	setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));


	RF24_txDelay = 250;
	if (speed == RF24_250KBPS){
		// Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
		// Making it '10'.
		setup |= _BV(RF_DR_LOW);

		RF24_txDelay = 450;
	}
	else{
		// Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
		// Making it '01'
		if (speed == RF24_2MBPS){
			setup |= _BV(RF_DR_HIGH);
			RF24_txDelay = 190;
		}
	}
	_RF24_writeRegister(RF_SETUP, setup);

	// Verify our result
	if (_RF24_readRegister(RF_SETUP) == setup)
	{
		result = true;
	}
	return result;
}

/****************************************************************************/

rf24_datarate_e RF24_getDataRate(void){
	rf24_datarate_e result;
	uint8_t dr = _RF24_readRegister(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

	// switch uses RAM (evil!)
	// Order matters in our case below
	if (dr == _BV(RF_DR_LOW))
	{
		// '10' = 250KBPS
		result = RF24_250KBPS;
	}
	else if (dr == _BV(RF_DR_HIGH))
	{
		// '01' = 2MBPS
		result = RF24_2MBPS;
	}
	else
	{
		// '00' = 1MBPS
		result = RF24_1MBPS;
	}
	return result;
}

/****************************************************************************/

void RF24_setCRCLength(rf24_crclength_e length){
	uint8_t config = _RF24_readRegister(CONFIG) & ~( _BV(CRCO) | _BV(EN_CRC));

	// switch uses RAM (evil!)
	if (length == RF24_CRC_DISABLED)
	{
		// Do nothing, we turned it off above.
	}
	else if (length == RF24_CRC_8)
	{
		config |= _BV(EN_CRC);
	}
	else
	{
		config |= _BV(EN_CRC);
		config |= _BV(CRCO);
	}
	_RF24_writeRegister(CONFIG, config);
}


rf24_crclength_e RF24_getCRCLength(void){
	rf24_crclength_e result = RF24_CRC_DISABLED;

	uint8_t config = _RF24_readRegister(CONFIG) & ( _BV(CRCO) | _BV(EN_CRC));
	uint8_t AA = _RF24_readRegister(EN_AA);

	if (config & _BV(EN_CRC) || AA)
	{
		if (config & _BV(CRCO))
			result = RF24_CRC_16;
		else
			result = RF24_CRC_8;
	}

	return result;
}

void RF24_disableCRC(void){
	uint8_t disable = _RF24_readRegister(CONFIG) & ~_BV(EN_CRC);
	_RF24_writeRegister(CONFIG, disable);
}

void RF24_setRetries(uint8_t delay, uint8_t count){
	_RF24_writeRegister(SETUP_RETR, (delay & 0xf) << ARD | (count & 0xf) << ARC);
}
