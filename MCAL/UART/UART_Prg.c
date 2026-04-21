#include "StdTypes.h"
#include "MemMap.h"
#include "UART_Int.h"
#include "UART_Private.h"
#include "UART_Cfg.h"

static UART_Callback_t UART_RX_Fptr   = NULLPTR;
static UART_Callback_t UART_TX_Fptr   = NULLPTR;
static UART_Callback_t UART_UDRE_Fptr = NULLPTR;

void UART_Init(void)
{
	u8 Local_u8UCSRCValue = UART_UCSRC_SELECT;

	/* =========================
	 * Baud Rate Configuration
	 * ========================= */
	UBRRH = (u8)(UART_Config.baud_rate >> 8U);
	UBRRL = (u8)(UART_Config.baud_rate);

	/* =========================
	 * Speed Configuration
	 * =========================
	 * @note Current baud enum values are precomputed for:
	 *       8 MHz / Asynchronous / Normal Speed
	 */
	switch (UART_Config.speed)
	{
		case UART_NORMAL_SPEED:
			CLR_BIT(UCSRA, U2X);
			break;

		case UART_DOUBLE_SPEED:
			SET_BIT(UCSRA, U2X);
			break;

		default:
			CLR_BIT(UCSRA, U2X);
			break;
	}

	/* =========================
	 * Mode Configuration
	 * ========================= */
	switch (UART_Config.mode)
	{
		case UART_ASYNCHRONOUS_MODE:
			Local_u8UCSRCValue |= UART_ASYNC_MODE_CFG;
			break;

		case UART_SYNCHRONOUS_MODE:
			Local_u8UCSRCValue |= UART_SYNC_MODE_CFG;
			break;

		default:
			Local_u8UCSRCValue |= UART_ASYNC_MODE_CFG;
			break;
	}

	/* =========================
	 * Parity Configuration
	 * ========================= */
	switch (UART_Config.parity)
	{
		case UART_PARITY_DISABLED:
			Local_u8UCSRCValue |= UART_PARITY_DISABLED_CFG;
			break;

		case UART_PARITY_EVEN:
			Local_u8UCSRCValue |= UART_PARITY_EVEN_CFG;
			break;

		case UART_PARITY_ODD:
			Local_u8UCSRCValue |= UART_PARITY_ODD_CFG;
			break;

		default:
			Local_u8UCSRCValue |= UART_PARITY_DISABLED_CFG;
			break;
	}

	/* =========================
	 * Stop Bits Configuration
	 * ========================= */
	switch (UART_Config.stop_bits)
	{
		case UART_ONE_STOP_BIT:
			Local_u8UCSRCValue |= UART_ONE_STOP_BIT_CFG;
			break;

		case UART_TWO_STOP_BITS:
			Local_u8UCSRCValue |= UART_TWO_STOP_BITS_CFG;
			break;

		default:
			Local_u8UCSRCValue |= UART_ONE_STOP_BIT_CFG;
			break;
	}

	/* =========================
	 * Character Size Configuration
	 * ========================= */
	switch (UART_Config.char_size)
	{
		case UART_5_BIT_CHAR:
			Local_u8UCSRCValue |= UART_5_BIT_CHAR_CFG;
			CLR_BIT(UCSRB, UCSZ2);
			break;

		case UART_6_BIT_CHAR:
			Local_u8UCSRCValue |= UART_6_BIT_CHAR_CFG;
			CLR_BIT(UCSRB, UCSZ2);
			break;

		case UART_7_BIT_CHAR:
			Local_u8UCSRCValue |= UART_7_BIT_CHAR_CFG;
			CLR_BIT(UCSRB, UCSZ2);
			break;

		case UART_8_BIT_CHAR:
			Local_u8UCSRCValue |= UART_8_BIT_CHAR_CFG;
			CLR_BIT(UCSRB, UCSZ2);
			break;

		case UART_9_BIT_CHAR:
			Local_u8UCSRCValue |= UART_9_BIT_CHAR_CFG;
			SET_BIT(UCSRB, UCSZ2);
			break;

		default:
			Local_u8UCSRCValue |= UART_8_BIT_CHAR_CFG;
			CLR_BIT(UCSRB, UCSZ2);
			break;
	}

	/* Write frame format and mode configuration to UCSRC */
	UCSRC = Local_u8UCSRCValue;

	/* =========================
	 * Transmitter / Receiver Enable
	 * ========================= */
	if (UART_Config.tx_enable == TRUE)
	{
		SET_BIT(UCSRB, TXEN);
	}
	else
	{
		CLR_BIT(UCSRB, TXEN);
	}

	if (UART_Config.rx_enable == TRUE)
	{
		SET_BIT(UCSRB, RXEN);
	}
	else
	{
		CLR_BIT(UCSRB, RXEN);
	}
}
void UART_Send(u8 Copy_u8Data)
{
	/* Wait until the transmit data register becomes empty */
	while (GET_BIT(UCSRA, UDRE) == 0U);
	/* Write data to UART data register */
	UDR = Copy_u8Data;
}

u8 UART_Receive(void)
{
	/* Wait until a new byte is received */
	while (GET_BIT(UCSRA, RXC) == 0U);
	/* Return received data */
	return UDR;
}

error_t UART_SendNoBlock(u8 Copy_u8Data)
{
	error_t Local_ErrorState = NOK;

	/* Check if UART data register is empty */
	if (GET_BIT(UCSRA, UDRE) != 0U)
	{
		UDR = Copy_u8Data;
		Local_ErrorState = OK;
	}
	else
	{
		Local_ErrorState = NOK;
	}

	return Local_ErrorState;
}

error_t UART_ReceiveNoBlock(u8 *Add_pu8Data)
{
	error_t Local_ErrorState = NOK;

	if (Add_pu8Data != NULLPTR)
	{
		if (GET_BIT(UCSRA, RXC) != 0U)
		{
			*Add_pu8Data = UDR;
			Local_ErrorState = OK;
		}
		else
		{
			Local_ErrorState = NOK;
		}
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}

void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}

void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}

void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}

void UART_TX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}

void UART_UDRE_InterruptEnable(void)
{
	SET_BIT(UCSRB, UDRIE);
}


void UART_UDRE_InterruptDisable(void)
{
	CLR_BIT(UCSRB, UDRIE);
}


error_t UART_RX_SetCallBack(UART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK;

	if (Add_pfCallBack != NULLPTR)
	{
		UART_RX_Fptr = Add_pfCallBack;
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}

error_t UART_TX_SetCallBack(UART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK;

	if (Add_pfCallBack != NULLPTR)
	{
		UART_TX_Fptr = Add_pfCallBack;
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}


error_t UART_UDRE_SetCallBack(UART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK;

	if (Add_pfCallBack != NULLPTR)
	{
		UART_UDRE_Fptr = Add_pfCallBack;
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}

void UART_SendDirect(u8 Copy_u8Data)
{
	UDR = Copy_u8Data;
}

u8 UART_ReceiveDirect(void)
{
	return UDR;
}


ISR(UART_RX_vect)
{
	if (UART_RX_Fptr != NULLPTR)
	{
		UART_RX_Fptr(); /* callback must read UDR */
	}
}

ISR(UART_TX_vect)
{
	if (UART_TX_Fptr!=NULLPTR)
	{
		UART_TX_Fptr();
	}
}
/* 
 * UDRE ISR:
 * Called when UDR is empty and ready for a new byte.
 * The callback should send the next byte or disable UDRE interrupt
 * when transmission is finished.
 */
ISR(UART_UDRE_vect)
{
	if (UART_UDRE_Fptr != NULLPTR)
	{
		UART_UDRE_Fptr();
	}
}