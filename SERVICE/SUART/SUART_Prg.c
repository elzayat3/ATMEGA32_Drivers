#include "StdTypes.h"
#include "Uart_Int.h"
#include "SUART_Int.h"
#include "SUART_Private.h"
#include "SUART_Cfg.h"


static SUART_TxRequest_t SUART_TxQueue[SUART_TX_QUEUE_SIZE];
static SUART_RxRequest_t SUART_RxQueue[SUART_RX_QUEUE_SIZE];

static const u8 *SUART_pu8CurrentTxData = NULLPTR;
static u16 SUART_u16CurrentTxLength = 0U;
static u16 SUART_u16CurrentTxIndex  = 0U;
static u8 SUART_u8TxHead            = 0U;
static u8 SUART_u8TxTail            = 0U;
static u8 SUART_u8TxCount           = 0U;
static u8 SUART_u8TxState           = SUART_IDLE;

static u8 *SUART_pu8CurrentRxBuffer = NULLPTR;
static u16 SUART_u16CurrentRxLength = 0U;
static u16 SUART_u16CurrentRxIndex  = 0U;
static u8 SUART_u8RxHead            = 0U;
static u8 SUART_u8RxTail            = 0U;
static u8 SUART_u8RxCount           = 0U;
static u8 SUART_u8RxState           = SUART_IDLE;

static SUART_Callback_t SUART_TX_Fptr = NULLPTR;
static SUART_Callback_t SUART_RX_Fptr = NULLPTR;

/**
 * @brief Internal UART TX empty callback handler used by the service layer.
 *
 * @return void
 */
static void SUART_TxHandler(void)
{
	/* Load a new TX request if there is no active one */
	if (SUART_pu8CurrentTxData == NULLPTR)
	{
		/* Check if there are queued TX requests */
		if (SUART_u8TxCount > 0U)
		{
			/* Load the next TX request from the queue */
			SUART_pu8CurrentTxData = SUART_TxQueue[SUART_u8TxHead].Data;
			SUART_u16CurrentTxLength = SUART_TxQueue[SUART_u8TxHead].Length;
			SUART_u16CurrentTxIndex = 0U;

			/* Move head to the next queue slot */
			SUART_u8TxHead++;
			if (SUART_u8TxHead >= SUART_TX_QUEUE_SIZE)
			{
				SUART_u8TxHead = 0U;
			}
			else
			{
				;
			}

			/* Decrease number of queued TX requests */
			SUART_u8TxCount--;
		}
		else
		{
			/* No more TX requests, stop transmission */
			UART_UDRE_InterruptDisable();
			SUART_u8TxState = SUART_IDLE;

			/* Notify user that TX queue became empty */
			if (SUART_TX_Fptr != NULLPTR)
			{
				SUART_TX_Fptr();
			}
			else
			{
				;
			}
		}
	}
	else
	{
		;
	}

	/* Process current TX request if available */
	if (SUART_pu8CurrentTxData != NULLPTR)
	{
		/* Send next byte from current TX buffer */
		if (SUART_u16CurrentTxIndex < SUART_u16CurrentTxLength)
		{
			UART_SendDirect(SUART_pu8CurrentTxData[SUART_u16CurrentTxIndex]);
			SUART_u16CurrentTxIndex++;
		}
		else
		{
			/* Current TX request finished */
			SUART_pu8CurrentTxData = NULLPTR;
			SUART_u16CurrentTxLength = 0U;
			SUART_u16CurrentTxIndex = 0U;
		}
	}
	else
	{
		;
	}
}

/**
 * @brief Internal UART RX callback handler used by the service layer.
 *
 * @return void
 */
static void SUART_RxHandler(void)
{
	u8 Local_u8ReceivedData = 0U; /* Holds received byte from UART */

	/* Load a new RX request if there is no active one */
	if (SUART_pu8CurrentRxBuffer == NULLPTR)
	{
		/* Check if there are queued RX requests */
		if (SUART_u8RxCount > 0U)
		{
			/* Load the next RX request from the queue */
			SUART_pu8CurrentRxBuffer = SUART_RxQueue[SUART_u8RxHead].Buffer;
			SUART_u16CurrentRxLength = SUART_RxQueue[SUART_u8RxHead].Length;
			SUART_u16CurrentRxIndex  = 0U;

			/* Move head to the next queue slot */
			SUART_u8RxHead++;
			if (SUART_u8RxHead >= SUART_RX_QUEUE_SIZE)
			{
				SUART_u8RxHead = 0U;
			}
			else
			{
				;
			}

			/* Decrease number of queued RX requests */
			SUART_u8RxCount--;
		}
		else
		{
			/* Read and discard unexpected received byte */
			(void)UART_ReceiveDirect();

			/* No more RX requests, stop reception */
			UART_RX_InterruptDisable();
			SUART_u8RxState = SUART_IDLE;
		}
	}
	else
	{
		;
	}

	/* Read UART data only if there is an active RX request */
	if (SUART_pu8CurrentRxBuffer != NULLPTR)
	{
		/* Read received byte directly from UART */
		Local_u8ReceivedData = UART_ReceiveDirect();

		/* Store received byte if there is still space */
		if (SUART_u16CurrentRxIndex < SUART_u16CurrentRxLength)
		{
			SUART_pu8CurrentRxBuffer[SUART_u16CurrentRxIndex] = Local_u8ReceivedData;
			SUART_u16CurrentRxIndex++;

			/* Check if current RX request is complete */
			if (SUART_u16CurrentRxIndex >= SUART_u16CurrentRxLength)
			{
				/* Release current RX request */
				SUART_pu8CurrentRxBuffer = NULLPTR;
				SUART_u16CurrentRxLength = 0U;
				SUART_u16CurrentRxIndex  = 0U;

				/* Notify user that one RX request is completed */
				if (SUART_RX_Fptr != NULLPTR)
				{
					SUART_RX_Fptr();
				}
				else
				{
					;
				}
			}
			else
			{
				;
			}
		}
		else
		{
			/* Invalid state, release current RX request */
			SUART_pu8CurrentRxBuffer = NULLPTR;
			SUART_u16CurrentRxLength = 0U;
			SUART_u16CurrentRxIndex  = 0U;
		}
	}
	else
	{
		;
	}
}


error_t SUART_Init(void)
{
	error_t Local_ErrorState = OK;  /* Final function status */
	error_t Local_TxCbState  = OK;  /* Status of linking TX handler */
	error_t Local_RxCbState  = OK;  /* Status of linking RX handler */
	u8 Local_u8Index         = 0U;  /* Loop counter */

	/* Reset TX queue entries */
	for (Local_u8Index = 0U; Local_u8Index < SUART_TX_QUEUE_SIZE; Local_u8Index++)
	{
		SUART_TxQueue[Local_u8Index].Data   = NULLPTR;
		SUART_TxQueue[Local_u8Index].Length = 0U;
	}

	/* Reset TX runtime state */
	SUART_pu8CurrentTxData = NULLPTR;
	SUART_u16CurrentTxLength = 0U;
	SUART_u16CurrentTxIndex = 0U;
	SUART_u8TxHead = 0U;
	SUART_u8TxTail = 0U;
	SUART_u8TxCount = 0U;
	SUART_u8TxState = SUART_IDLE;
	SUART_TX_Fptr = NULLPTR;

	/* Reset RX queue entries */
	for (Local_u8Index = 0U; Local_u8Index < SUART_RX_QUEUE_SIZE; Local_u8Index++)
	{
		SUART_RxQueue[Local_u8Index].Buffer = NULLPTR;
		SUART_RxQueue[Local_u8Index].Length = 0U;
	}

	/* Reset RX runtime state */
	SUART_pu8CurrentRxBuffer = NULLPTR;
	SUART_u16CurrentRxLength = 0U;
	SUART_u16CurrentRxIndex = 0U;
	SUART_u8RxHead = 0U;
	SUART_u8RxTail = 0U;
	SUART_u8RxCount = 0U;
	SUART_u8RxState = SUART_IDLE;
	SUART_RX_Fptr = NULLPTR;

	/* Link service handlers with UART MCAL callbacks */
	Local_TxCbState = UART_UDRE_SetCallBack(SUART_TxHandler);
	Local_RxCbState = UART_RX_SetCallBack(SUART_RxHandler);

	/* Check callback registration result */
	if ((Local_TxCbState != OK) || (Local_RxCbState != OK))
	{
		Local_ErrorState = NOK;
	}
	else
	{
		/* Keep service-controlled interrupts disabled until needed */
		UART_UDRE_InterruptDisable();
		UART_RX_InterruptDisable();
	}

	return Local_ErrorState;
}

error_t SUART_SendAsync(const u8 *Add_pu8Data, u16 Copy_u16Length)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check input pointer validity */
	if (Add_pu8Data == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check length validity */
	else if (Copy_u16Length == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	else
	{
		/* Check if TX queue has free space */
		if (SUART_u8TxCount < SUART_TX_QUEUE_SIZE)
		{
			/* Store the TX request in the queue */
			SUART_TxQueue[SUART_u8TxTail].Data   = Add_pu8Data;
			SUART_TxQueue[SUART_u8TxTail].Length = Copy_u16Length;

			/* Move tail to the next queue slot */
			SUART_u8TxTail++;
			if (SUART_u8TxTail >= SUART_TX_QUEUE_SIZE)
			{
				SUART_u8TxTail = 0U;
			}
			else
			{
				;
			}

			/* Increase number of queued TX requests */
			SUART_u8TxCount++;

			/* Start transmission if the service is currently idle */
			if (SUART_u8TxState == SUART_IDLE)
			{
				SUART_u8TxState = SUART_BUSY;
				UART_UDRE_InterruptEnable();
			}
			else
			{
				;
			}
		}
		else
		{
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}


error_t SUART_SendBuffersAsync(const u8 * const Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count)
{
	error_t Local_ErrorState = OK; /* Final function status */
	u8 Local_u8Index = 0U;         /* Loop counter */

	/* Check input array pointers validity */
	if ((Add_pu8BufferArr == NULLPTR) || (Add_pu16LengthArr == NULLPTR))
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check count validity */
	else if (Copy_u8Count == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	/* Check if TX queue has enough free space for all requests */
	else if ((SUART_u8TxCount + Copy_u8Count) > SUART_TX_QUEUE_SIZE)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		/* Validate all pointers and lengths first */
		for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
		{
			if (Add_pu8BufferArr[Local_u8Index] == NULLPTR)
			{
				Local_ErrorState = NULL_PTR;
				break;
			}
			else if (Add_pu16LengthArr[Local_u8Index] == 0U)
			{
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
			else
			{
				;
			}
		}

		/* Queue all TX requests only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_TxQueue[SUART_u8TxTail].Data   = Add_pu8BufferArr[Local_u8Index];
				SUART_TxQueue[SUART_u8TxTail].Length = Add_pu16LengthArr[Local_u8Index];

				SUART_u8TxTail++;
				if (SUART_u8TxTail >= SUART_TX_QUEUE_SIZE)
				{
					SUART_u8TxTail = 0U;
				}
				else
				{
					;
				}

				SUART_u8TxCount++;
			}

			if (SUART_u8TxState == SUART_IDLE)
			{
				SUART_u8TxState = SUART_BUSY;
				UART_UDRE_InterruptEnable();
			}
			else
			{
				;
			}
		}
		else
		{
			;
		}
	}

	return Local_ErrorState;
}


error_t SUART_ReceiveAsync(u8 *Add_pu8Buffer, u16 Copy_u16Length)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check input pointer validity */
	if (Add_pu8Buffer == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check length validity */
	else if (Copy_u16Length == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	else
	{
		/* Check if RX queue has free space */
		if (SUART_u8RxCount < SUART_RX_QUEUE_SIZE)
		{
			/* Store the RX request in the queue */
			SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8Buffer;
			SUART_RxQueue[SUART_u8RxTail].Length = Copy_u16Length;

			/* Move tail to the next queue slot */
			SUART_u8RxTail++;
			if (SUART_u8RxTail >= SUART_RX_QUEUE_SIZE)
			{
				SUART_u8RxTail = 0U;
			}
			else
			{
				;
			}

			/* Increase number of queued RX requests */
			SUART_u8RxCount++;

			/* Start reception if the service is currently idle */
			if (SUART_u8RxState == SUART_IDLE)
			{
				SUART_u8RxState = SUART_BUSY;
				UART_RX_InterruptEnable();
			}
			else
			{
				;
			}
		}
		else
		{
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}

error_t SUART_ReceiveBuffersAsync(u8 *Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count)
{
	error_t Local_ErrorState = OK; /* Final function status */
	u8 Local_u8Index = 0U;         /* Loop counter */

	/* Check input array pointers validity */
	if ((Add_pu8BufferArr == NULLPTR) || (Add_pu16LengthArr == NULLPTR))
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check count validity */
	else if (Copy_u8Count == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	/* Check if RX queue has enough free space for all requests */
	else if ((SUART_u8RxCount + Copy_u8Count) > SUART_RX_QUEUE_SIZE)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		/* Validate all pointers and lengths first */
		for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
		{
			if (Add_pu8BufferArr[Local_u8Index] == NULLPTR)
			{
				Local_ErrorState = NULL_PTR;
				break;
			}
			else if (Add_pu16LengthArr[Local_u8Index] == 0U)
			{
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
			else
			{
				;
			}
		}

		/* Queue all RX requests only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8BufferArr[Local_u8Index];
				SUART_RxQueue[SUART_u8RxTail].Length = Add_pu16LengthArr[Local_u8Index];

				SUART_u8RxTail++;
				if (SUART_u8RxTail >= SUART_RX_QUEUE_SIZE)
				{
					SUART_u8RxTail = 0U;
				}
				else
				{
					;
				}

				SUART_u8RxCount++;
			}

			if (SUART_u8RxState == SUART_IDLE)
			{
				SUART_u8RxState = SUART_BUSY;
				UART_RX_InterruptEnable();
			}
			else
			{
				;
			}
		}
		else
		{
			;
		}
	}

	return Local_ErrorState;
}


error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check callback pointer validity */
	if (Add_pfCallBack != NULLPTR)
	{
		SUART_TX_Fptr = Add_pfCallBack;
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}


error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check callback pointer validity */
	if (Add_pfCallBack != NULLPTR)
	{
		SUART_RX_Fptr = Add_pfCallBack;
	}
	else
	{
		Local_ErrorState = NULL_PTR;
	}

	return Local_ErrorState;
}