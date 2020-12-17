
/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS 		0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define RF24_NOP      0xFF

/* Non-P omissions */
#define LNA_HCURR   0

/* P model memory Map */
#define RPD         0x09
#define W_TX_PAYLOAD_NO_ACK  0xB0

/* P model bit Mnemonics */
#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2

#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)

#define _BV(x) (1<<(x))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#include <stdbool.h>
#include <string.h>
#include "gpio.h"
#include "spi.h"
#include "dma.h"
#include "main.h"

		//! acklowlegement message struct
		struct AckData{
			int16_t 				yaw;															//!< angle motion yaw
			int16_t 				pitch;															//!< true motion pitch
			int16_t 				roll;															//!< true motion roll

			uint16_t 				heading;														//!< magnetic heading
			uint32_t 				altitude; 														//!< barometric altitide

			uint32_t 				LV03x;															//!< Position Swiss LV03 x
			uint32_t 				LV03y;															//!< Position Swiss LV03 y

			uint16_t 				flags;															//!< Option Flags
			uint32_t				uptdate_time; 													//!< Loop time
		};

		//! receive message struct
		struct RadioData{
			int16_t					yaw;															//!< Setpoint yaw
			int16_t					pitch;															//!< Setpoint pitch
			int16_t					roll;															//!< Setpoint roll
			uint16_t				throttle;														//!< Throttle

			uint16_t				rp_P;															//!< roll pitch P-Gain
			uint16_t				rp_I;															//!< roll pitch I-Gain
			uint16_t				rp_D;															//!< roll pitch D-Gain

			uint16_t				y_P;															//!< yaw P-Gain
			uint16_t				y_I;															//!< yaw I-Gain
			uint16_t				y_D;															//!< yaw D-Gain

			uint16_t				flags;															//!< 2 bytes 10
			uint32_t				data;															//!< 4 bytes 14
		};

		struct AckData 					ackData;														//!< acknowlegement data
		struct RadioData 				recvData;														//!< receive data

typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;
typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;

bool p_variant; /* False for RF24L01 and true for RF24L01P */
uint8_t payload_size; /**< Fixed size of payloads */
bool dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
uint8_t pipe0_reading_address[5]; /**< Last address set on pipe 0 for reading. */
uint8_t addr_width; /**< The address width to use - 3,4 or 5 bytes. */

bool RF24_failureDetected;
uint32_t RF24_txDelay;
uint32_t RF24_csDelay;

void RF24_Init();

uint8_t RF24_readRegister(uint8_t reg, uint8_t* buf, uint8_t len); /**/
uint8_t _RF24_readRegister(uint8_t reg);/**/

uint8_t RF24_writeRegister(uint8_t reg, uint8_t* buf, uint8_t len);/**/
uint8_t __RF24_writeRegister(uint8_t reg, const uint8_t* buf, uint8_t len);
uint8_t _RF24_writeRegister(uint8_t reg, uint8_t value);/**/

uint8_t RF24_write_payload(const void* buf, uint8_t len, const uint8_t writeType);/**/
uint8_t RF24_read_payload(void* buf, uint8_t len);/**/

uint8_t RF24_get_status(void);/**/

void RF24_toggle_features(void);/**/

uint8_t RF24_spiTrans(uint8_t cmd);/**/


bool RF24_begin(void);/**/
bool RF24_isChipConnected();/**/

void RF24_startListening(void);/**/
void RF24_stopListening(void);/**/

bool RF24_available(void);/**/
bool _RF24_available(uint8_t* pipe_num);/**/

void RF24_read( void* buf, uint8_t len );/**/


void RF24_openWritingPipe(const uint8_t *address);/**/
void RF24_openReadingPipe(uint8_t number, const uint8_t *address);/**/

void _RF24_openReadingPipe(uint8_t number, uint64_t address);/**/
void _RF24_openWritingPipe(uint64_t address);/**/


void setPayloadSize(uint8_t size);/**/
uint8_t getPayloadSize(void);/**/

bool RF24_rxFifoFull();/**/

void RF24_powerDown(void);/**/
void RF24_powerUp(void) ;/**/

bool _RF24_write( const void* buf, uint8_t len, const bool multicast );/**/
bool RF24_write( const void* buf, uint8_t len );/**/
bool RF24_writeFast( const void* buf, uint8_t len );/**/
bool _RF24_writeFast( const void* buf, uint8_t len, const bool multicast );/**/
bool RF24_writeBlocking( const void* buf, uint8_t len, uint32_t timeout );/**/

bool RF24_txStandBy();/**/
bool _RF24_txStandBy(uint32_t timeout, bool startTx);/**/
bool __RF24_txStandBy(uint32_t timeout);/**/

void RF24_writeAckPayload(uint8_t pipe, const void* buf, uint8_t len);/**/
bool RF24_isAckPayloadAvailable(void);/**/

void RF24_whatHappened(uint8_t* tx_ok, uint8_t* tx_fail, uint8_t* rx_ready);/**/

void RF24_startFastWrite(const void* buf, uint8_t len, const bool multicast, bool startTx );/**/
void _RF24_startFastWrite( const void* buf, uint8_t len, const bool multicast);/**/
void RF24_startWrite( const void* buf, uint8_t len, const bool multicast );/**/

void RF24_reUseTX();/**/
uint8_t RF24_flush_tx(void);/**/

bool RF24_testCarrier(void);/**/
bool RF24_testRPD(void) ;/**/

void RF24_closeReadingPipe( uint8_t pipe ) ;/**/

void RF24_setAddressWidth(uint8_t a_width);/**/

void RF24_setRetries(uint8_t delay, uint8_t count);/**/

void RF24_setChannel(uint8_t channel);/**/

uint8_t RF24_getChannel(void);/**/

void RF24_setPayloadSize(uint8_t size);
uint8_t RF24_getPayloadSize(void);
uint8_t RF24_getDynamicPayloadSize(void);/**/
void RF24_enableAckPayload(void);/**/
void RF24_enableDynamicPayloads(void);/**/
void RF24_disableDynamicPayloads(void);/**/

bool RF24_isPVariant(void) ;/**/

void RF24_setAutoAck(bool enable);/**/
void _RF24_setAutoAck( uint8_t pipe, bool enable );/**/

void RF24_setPALevel ( uint8_t level );/**/

uint8_t RF24_getPALevel( void );/**/

bool RF24_setDataRate(rf24_datarate_e speed);/**/

rf24_datarate_e RF24_getDataRate( void ) ;/**/

void RF24_setCRCLength(rf24_crclength_e length);/**/

rf24_crclength_e RF24_getCRCLength(void);/**/

void RF24_disableCRC( void ) ;/**/

void RF24_maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready);/**/

uint8_t RF24_flush_rx(void);/**/
