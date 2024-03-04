#ifndef SPI_H_
#define SPI_H_

#include <cstddef>
#include "stm32f722xx.h"
#include "stm32f7xx_it.h"
#include "core_cm7.h"
#include "GPIO.hpp"

namespace COM {


class SPI {
public:

	constexpr SPI(SPI_TypeDef* spi, GPIO_TypeDef* mosi , uint8_t mosi_pin, uint8_t mosi_afr,
									GPIO_TypeDef* miso , uint8_t miso_pin, uint8_t miso_afr,
									GPIO_TypeDef* sck , uint8_t sck_pin, uint8_t sck_afr,
									GPIO_TypeDef* cs , uint8_t cs_pin) : spi(spi),
									mosi(mosi), mosi_pin(mosi_pin), mosi_afr(mosi_afr),
									miso(miso), miso_pin(miso_pin), miso_afr(miso_afr),
									sck(sck), sck_pin(sck_pin), sck_afr(sck_afr),
									cs(cs), cs_pin(cs_pin),
									tx_current(nullptr), rx_current(nullptr),
									tx_end(nullptr), rx_end(nullptr), done(false), IRQn(SPI1_IRQn), mode(SPI_Mode::RX){}

	void Init();
	void Interrupt_Handler() noexcept;

	void Enable();
	void Disable();

	void Transmit(const uint8_t *data, std::size_t size);
	void Receive(uint8_t *data, std::size_t size);
	void TransmitReceive(uint8_t *txdata, uint8_t *rxdata , std::size_t size);

	void CS_EN();
	void CS_DIS();

private:
	SPI_TypeDef* const spi;
	IRQn_Type IRQn;

	GPIO_TypeDef *mosi, *miso, *sck, *cs;
	const uint8_t mosi_pin, miso_pin, sck_pin, cs_pin;
	uint8_t mosi_afr, miso_afr, sck_afr;

	enum class SPI_Mode:uint8_t{
		TX,
		RX,
		TRX
	};

	SPI_Mode mode;
	const uint8_t* tx_current;
	uint8_t* rx_current;

	const uint8_t* tx_end;
	uint8_t* rx_end;

	volatile bool done;
};

} /* namespace COM */

#endif /* SPI_H_ */
