#include "StdTypes.h"
#include "MemMap.h"
#include "SPI_Int.h"
#include "SPI_Private.h"
#include "SPI_Cfg.h"

/* ========================================================================== */
/*                           Static Global Variables                          */
/* ========================================================================== */

/**
 * @brief Callback function pointer for SPI transfer complete interrupt.
 */
static SPI_Callback_t SPI_CallBack = NULLPTR;

static volatile bool_t SPI_BusyState = FALSE;

/* ========================================================================== */
/*                         Static Function Prototypes                         */
/* ========================================================================== */

/**
 * @brief Checks whether SPI configuration is valid.
 *
 * @param config Pointer to SPI configuration structure.
 *
 * @retval TRUE   Configuration is valid.
 * @retval FALSE  Configuration is invalid.
 */
static bool_t SPI_IsValidConfig(const SPI_Config_t *config);

/**
 * @brief Applies SPI master/slave mode configuration.
 *
 * @param mode SPI operating mode.
 *
 * @return void
 */
static void SPI_ApplyMode(SPI_Mode_t mode);

/**
 * @brief Applies SPI data order configuration.
 *
 * @param data_order SPI data order.
 *
 * @return void
 */
static void SPI_ApplyDataOrder(SPI_DataOrder_t data_order);

/**
 * @brief Applies SPI clock polarity configuration.
 *
 * @param clock_polarity SPI clock polarity.
 *
 * @return void
 */
static void SPI_ApplyClockPolarity(SPI_ClockPolarity_t clock_polarity);

/**
 * @brief Applies SPI clock phase configuration.
 *
 * @param clock_phase SPI clock phase.
 *
 * @return void
 */
static void SPI_ApplyClockPhase(SPI_ClockPhase_t clock_phase);

/**
 * @brief Applies SPI clock rate configuration.
 *
 * @param clock_rate SPI clock rate.
 *
 * @return void
 */
static void SPI_ApplyClockRate(SPI_ClockRate_t clock_rate);

/**
 * @brief Applies SPI interrupt enable/disable configuration.
 *
 * @param interrupt_state Interrupt state.
 *
 * @return void
 */
static void SPI_ApplyInterruptState(bool_t interrupt_state);

/* ========================================================================== */
/*                         Static Function Definitions                        */
/* ========================================================================== */

static bool_t SPI_IsValidConfig(const SPI_Config_t *config)
{
    bool_t state = TRUE;

    if (config == NULLPTR)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_MODE(config->mode) == FALSE)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_DATA_ORDER(config->data_order) == FALSE)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_CLOCK_POLARITY(config->clock_polarity) == FALSE)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_CLOCK_PHASE(config->clock_phase) == FALSE)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_CLOCK_RATE(config->clock_rate) == FALSE)
    {
        state = FALSE;
    }
    else if (SPI_IS_VALID_BOOL(config->interrupt_enable) == FALSE)
    {
        state = FALSE;
    }
    else
    {
        /* Configuration is valid. */
    }

    return state;
}

static void SPI_ApplyMode(SPI_Mode_t mode)
{
    if (mode == SPI_MASTER_MODE)
    {
        SET_BIT(SPCR, MSTR);
    }
    else
    {
        CLR_BIT(SPCR, MSTR);
    }
}

static void SPI_ApplyDataOrder(SPI_DataOrder_t data_order)
{
    if (data_order == SPI_LSB_FIRST)
    {
        SET_BIT(SPCR, DORD);
    }
    else
    {
        CLR_BIT(SPCR, DORD);
    }
}

static void SPI_ApplyClockPolarity(SPI_ClockPolarity_t clock_polarity)
{
    if (clock_polarity == SPI_IDLE_HIGH)
    {
        SET_BIT(SPCR, CPOL);
    }
    else
    {
        CLR_BIT(SPCR, CPOL);
    }
}

static void SPI_ApplyClockPhase(SPI_ClockPhase_t clock_phase)
{
    if (clock_phase == SPI_SAMPLE_TRAILING)
    {
        SET_BIT(SPCR, CPHA);
    }
    else
    {
        CLR_BIT(SPCR, CPHA);
    }
}

static void SPI_ApplyClockRate(SPI_ClockRate_t clock_rate)
{
    SPI_CLR_MASK(SPCR, SPI_CLOCK_RATE_MASK);
    SPI_CLR_MASK(SPSR, SPI_DOUBLE_SPEED_MASK);

    switch (clock_rate)
    {
        case SPI_CLOCK_DIV_4:
            /* SPR1 = 0, SPR0 = 0, SPI2X = 0 */
            break;

        case SPI_CLOCK_DIV_16:
            SET_BIT(SPCR, SPR0);
            break;

        case SPI_CLOCK_DIV_64:
            SET_BIT(SPCR, SPR1);
            break;

        case SPI_CLOCK_DIV_128:
            SET_BIT(SPCR, SPR1);
            SET_BIT(SPCR, SPR0);
            break;

        case SPI_CLOCK_DIV_2:
            SET_BIT(SPSR, SPI2X);
            break;

        case SPI_CLOCK_DIV_8:
            SET_BIT(SPCR, SPR0);
            SET_BIT(SPSR, SPI2X);
            break;

        case SPI_CLOCK_DIV_32:
            SET_BIT(SPCR, SPR1);
            SET_BIT(SPSR, SPI2X);
            break;

        default:
            /* Invalid clock rate is checked before calling this function. */
            break;
    }
}

static void SPI_ApplyInterruptState(bool_t interrupt_state)
{
    if (interrupt_state == TRUE)
    {
        SET_BIT(SPCR, SPIE);
    }
    else
    {
        CLR_BIT(SPCR, SPIE);
    }
}
error_t SPI_Init(void)
{
	error_t status = NOK;

	if (SPI_IsValidConfig(&SPI_Config) == FALSE)
	{
		status = NOK;
	}
	else
	{
		SPI_Disable();

		SPI_ApplyMode(SPI_Config.mode);
		
		SPI_ApplyDataOrder(SPI_Config.data_order);
		
		SPI_ApplyClockPolarity(SPI_Config.clock_polarity);
		
		SPI_ApplyClockPhase(SPI_Config.clock_phase);
		
		SPI_ApplyClockRate(SPI_Config.clock_rate);
		
		SPI_ApplyInterruptState(SPI_Config.interrupt_enable);

		SPI_Enable();

		status = OK;
	}

	return status;
}
void SPI_Enable(void)
{
	SET_BIT(SPCR, SPE);
}

void SPI_Disable(void)
{
	CLR_BIT(SPCR, SPE);
}
error_t SPI_SendByte(uint8_t data)
{
	uint8_t dummy_read = 0u;

	return SPI_TransceiveByte(data, &dummy_read);
}

error_t SPI_ReceiveByte(uint8_t *data)
{
	return SPI_TransceiveByte(SPI_DUMMY_BYTE, data);
}

error_t SPI_TransceiveByte(uint8_t tx_data, uint8_t *rx_data)
{
	error_t status = OK;
	uint32_t timeout = SPI_TIMEOUT_VALUE;

	if (rx_data == NULLPTR)
	{
		status = NULL_PTR;
	}
	else
	{
		SPDR = tx_data;

		while ((SPI_IS_TRANSFER_DONE() == 0u) && (timeout > 0u))
		{
			timeout--;
		}

		if (timeout == 0u)
		{
			status = TIMEOUT;
		}
		else
		{
			*rx_data = SPDR;
			status = OK;
		}
	}

	return status;
}
error_t SPI_SendByteNoBlock(uint8_t data)
{
	error_t status = IN_PROGRESS;
	volatile uint8_t dummy_read = 0u;

	if (SPI_BusyState == FALSE)
	{
		SPDR = data;
		SPI_BusyState = TRUE;
		status = IN_PROGRESS;
	}
	else if (SPI_IS_TRANSFER_DONE() != 0u)
	{
		dummy_read = SPDR;
		(void)dummy_read;

		SPI_BusyState = FALSE;
		status = OK;
	}
	else
	{
		status = IN_PROGRESS;
	}

	return status;
}
error_t SPI_ReceiveByteNoBlock(uint8_t *data)
{
    error_t status = IN_PROGRESS;

    if (data == NULLPTR)
    {
        status = NULL_PTR;
    }
    else if (SPI_BusyState == FALSE)
    {
        /*
         * Start SPI transfer by sending dummy byte.
         * In master mode, clock is generated only when data is written to SPDR.
         */
        SPDR = SPI_DUMMY_BYTE;
        SPI_BusyState = TRUE;
        status = IN_PROGRESS;
    }
    else if (SPI_IS_TRANSFER_DONE() != 0u)
    {
        /*
         * Read SPDR to get received data and clear SPIF flag.
         */
        *data = SPDR;
        SPI_BusyState = FALSE;
        status = OK;
    }
    else
    {
        status = IN_PROGRESS;
    }

    return status;
}
error_t SPI_TransceiveByteNoBlock(uint8_t tx_data, uint8_t *rx_data)
{
    error_t status = IN_PROGRESS;

    if (rx_data == NULLPTR)
    {
        status = NULL_PTR;
    }
    else if (SPI_BusyState == FALSE)
    {
        /*
         * Start SPI transfer.
         * SPI is full-duplex, so writing to SPDR starts transmit and receive.
         */
        SPDR = tx_data;
        SPI_BusyState = TRUE;
        status = IN_PROGRESS;
    }
    else if (SPI_IS_TRANSFER_DONE() != 0u)
    {
        /*
         * Transfer completed.
         * Reading SPDR gets received byte and clears SPIF flag.
         */
        *rx_data = SPDR;
        SPI_BusyState = FALSE;
        status = OK;
    }
    else
    {
        status = IN_PROGRESS;
    }

    return status;
}
void SPI_InterruptEnable(void)
{
	SET_BIT(SPCR, SPIE);
}
void SPI_InterruptDisable(void)
{
	CLR_BIT(SPCR, SPIE);
}
error_t SPI_SetCallBack(SPI_Callback_t callback)
{
	error_t status = OK;

	if (callback == NULLPTR)
	{
		status = NULL_PTR;
	}
	else
	{
		SPI_CallBack = callback;
		status = OK;
	}

	return status;
}
ISR(SPI_STC_vect)
{
	SPI_BusyState = FALSE;

	if (SPI_CallBack != NULLPTR)
	{
		SPI_CallBack();
	}
	else
	{
		/* No callback registered. */
	}
}