#include "StdTypes.h"
#include "Uart_Int.h"
#include "SUART_Int.h"
#include "SUART_Private.h"
#include "SUART_Cfg.h"

static SUART_TxRequest_t SUART_TxQueue[SUART_TX_QUEUE_SIZE];
static SUART_RxRequest_t SUART_RxQueue[SUART_RX_QUEUE_SIZE];

static const u8 *SUART_pu8CurrentTxStr = NULLPTR;
static SUART_TxMode_t SUART_CurrentTxMode = SUART_TX_NORMAL;
static u8 SUART_u8TxHead = 0U;
static u8 SUART_u8TxTail = 0U;
static u8 SUART_u8TxCount = 0U;
static u8 SUART_u8TxState = SUART_IDLE;
static u8 SUART_u8TxTerminatorIndex = 0U;

static u8 *SUART_pu8CurrentRxBuffer = NULLPTR;
static SUART_RxMode_t SUART_CurrentRxMode = SUART_RX_STRING;
static u16 SUART_u16CurrentRxLength = 0U;
static u16 SUART_u16CurrentRxIndex = 0U;
static u8 SUART_u8RxHead = 0U;
static u8 SUART_u8RxTail = 0U;
static u8 SUART_u8RxCount = 0U;
static u8 SUART_u8RxState = SUART_IDLE;
static u8 SUART_u8RxTerminatorIndex = 0U;

static SUART_Callback_t SUART_TX_Fptr = NULLPTR;
static SUART_Callback_t SUART_RX_Fptr = NULLPTR;
static void SUART_TxHandler(void)
{
	/* Check if there is no active TX request */
	if (SUART_pu8CurrentTxStr == NULLPTR)
	{
		/* Check if there are queued TX requests */
		if (SUART_u8TxCount > 0U)
		{
			/* Load the next TX request from the queue */
			SUART_pu8CurrentTxStr = SUART_TxQueue[SUART_u8TxHead].Str;
			SUART_CurrentTxMode   = SUART_TxQueue[SUART_u8TxHead].Mode;

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

			/* Reset TX terminator index for the new request */
			SUART_u8TxTerminatorIndex = 0U;
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
	if (SUART_pu8CurrentTxStr != NULLPTR)
	{
		/* Send characters of the current string until null terminator */
		if (*SUART_pu8CurrentTxStr != '\0')
		{
			UART_SendDirect(*SUART_pu8CurrentTxStr);
			SUART_pu8CurrentTxStr++;
		}
		else
		{
			/* Current string characters are finished */
			if (SUART_CurrentTxMode == SUART_TX_NORMAL)
			{
				/* Send configured TX terminator in normal mode */
				if (SUART_u8TxTerminatorIndex < SUART_TX_TERMINATOR_LEN)
				{
					UART_SendDirect(SUART_TxTerminator[SUART_u8TxTerminatorIndex]);
					SUART_u8TxTerminatorIndex++;
				}
				else
				{
					/* Terminator finished, release current TX request */
					SUART_pu8CurrentTxStr     = NULLPTR;
					SUART_u8TxTerminatorIndex = 0U;
				}
			}
			else
			{
				/* Raw mode does not append terminator */
				SUART_pu8CurrentTxStr = NULLPTR;
			}
		}
	}
	else
	{
		;
	}
}
static void SUART_RxHandler(void)
{
	u8 Local_u8ReceivedData = 0U; /* Holds received byte from UART */

	/* Load next RX request if no active request exists */
	if (SUART_pu8CurrentRxBuffer == NULLPTR)
	{
		if (SUART_u8RxCount > 0U)
		{
			SUART_pu8CurrentRxBuffer = SUART_RxQueue[SUART_u8RxHead].Buffer;
			SUART_CurrentRxMode      = SUART_RxQueue[SUART_u8RxHead].Mode;
			SUART_u16CurrentRxLength = SUART_RxQueue[SUART_u8RxHead].Length;

			SUART_u8RxHead++;
			if (SUART_u8RxHead >= SUART_RX_QUEUE_SIZE)
			{
				SUART_u8RxHead = 0U;
			}
			else
			{
				;
			}

			SUART_u8RxCount--;
			SUART_u16CurrentRxIndex   = 0U;
			SUART_u8RxTerminatorIndex = 0U;
		}
		else
		{
			/* No active request and no queued request */
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
		Local_u8ReceivedData = UART_ReceiveDirect();

		if (SUART_CurrentRxMode == SUART_RX_STRING)
		{
			if (SUART_u16CurrentRxIndex < (SUART_u16CurrentRxLength - 1U))
			{
				SUART_pu8CurrentRxBuffer[SUART_u16CurrentRxIndex] = Local_u8ReceivedData;
				SUART_u16CurrentRxIndex++;

				if (Local_u8ReceivedData == SUART_RxTerminator[SUART_u8RxTerminatorIndex])
				{
					SUART_u8RxTerminatorIndex++;

					if (SUART_u8RxTerminatorIndex >= SUART_RX_TERMINATOR_LEN)
					{
						SUART_u16CurrentRxIndex -= SUART_RX_TERMINATOR_LEN;
						SUART_pu8CurrentRxBuffer[SUART_u16CurrentRxIndex] = '\0';

						SUART_pu8CurrentRxBuffer  = NULLPTR;
						SUART_u16CurrentRxLength  = 0U;
						SUART_u16CurrentRxIndex   = 0U;
						SUART_u8RxTerminatorIndex = 0U;

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
					if (Local_u8ReceivedData == SUART_RxTerminator[0])
					{
						SUART_u8RxTerminatorIndex = 1U;
					}
					else
					{
						SUART_u8RxTerminatorIndex = 0U;
					}
				}
			}
			else
			{
				SUART_pu8CurrentRxBuffer[SUART_u16CurrentRxIndex] = '\0';

				SUART_pu8CurrentRxBuffer  = NULLPTR;
				SUART_u16CurrentRxLength  = 0U;
				SUART_u16CurrentRxIndex   = 0U;
				SUART_u8RxTerminatorIndex = 0U;

				if (SUART_RX_Fptr != NULLPTR)
				{
					SUART_RX_Fptr();
				}
				else
				{
					;
				}
			}
		}
		else
		{
			if (SUART_u16CurrentRxIndex < SUART_u16CurrentRxLength)
			{
				SUART_pu8CurrentRxBuffer[SUART_u16CurrentRxIndex] = Local_u8ReceivedData;
				SUART_u16CurrentRxIndex++;

				if (SUART_u16CurrentRxIndex >= SUART_u16CurrentRxLength)
				{
					SUART_pu8CurrentRxBuffer  = NULLPTR;
					SUART_u16CurrentRxLength  = 0U;
					SUART_u16CurrentRxIndex   = 0U;
					SUART_u8RxTerminatorIndex = 0U;

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
				SUART_pu8CurrentRxBuffer  = NULLPTR;
				SUART_u16CurrentRxLength  = 0U;
				SUART_u16CurrentRxIndex   = 0U;
				SUART_u8RxTerminatorIndex = 0U;
			}
		}
	}
	else
	{
		;
	}
}

error_t SUART_Init(void)
{
	error_t Local_ErrorState = OK;   /* Final function status */
	error_t Local_TxCbState  = OK;   /* Status of linking TX handler */
	error_t Local_RxCbState  = OK;   /* Status of linking RX handler */
	u8 Local_u8Index         = 0U;   /* Loop counter */

	/* Reset TX queue entries */
	for (Local_u8Index = 0U; Local_u8Index < SUART_TX_QUEUE_SIZE; Local_u8Index++)
	{
		SUART_TxQueue[Local_u8Index].Str  = NULLPTR;         /* No string stored */
		SUART_TxQueue[Local_u8Index].Mode = SUART_TX_NORMAL; /* Default TX mode */
	}

	/* Reset TX runtime state */
	SUART_pu8CurrentTxStr     = NULLPTR;         /* No active TX string */
	SUART_CurrentTxMode       = SUART_TX_NORMAL; /* Default active TX mode */
	SUART_u8TxHead            = 0U;              /* TX queue head index */
	SUART_u8TxTail            = 0U;              /* TX queue tail index */
	SUART_u8TxCount           = 0U;              /* Number of queued TX requests */
	SUART_u8TxState           = SUART_IDLE;      /* TX service starts idle */
	SUART_u8TxTerminatorIndex = 0U;              /* Terminator sending index reset */
	SUART_TX_Fptr             = NULLPTR;         /* No TX notification callback */

	/* Reset RX queue entries */
	for (Local_u8Index = 0U; Local_u8Index < SUART_RX_QUEUE_SIZE; Local_u8Index++)
	{
		SUART_RxQueue[Local_u8Index].Buffer = NULLPTR;         /* No buffer stored */
		SUART_RxQueue[Local_u8Index].Length = 0U;              /* No valid length */
		SUART_RxQueue[Local_u8Index].Mode   = SUART_RX_STRING; /* Default RX mode */
	}

	/* Reset RX runtime state */
	SUART_pu8CurrentRxBuffer  = NULLPTR;         /* No active RX buffer */
	SUART_CurrentRxMode       = SUART_RX_STRING; /* Default active RX mode */
	SUART_u16CurrentRxLength  = 0U;              /* Current RX target length */
	SUART_u16CurrentRxIndex   = 0U;              /* Current RX write index */
	SUART_u8RxHead            = 0U;              /* RX queue head index */
	SUART_u8RxTail            = 0U;              /* RX queue tail index */
	SUART_u8RxCount           = 0U;              /* Number of queued RX requests */
	SUART_u8RxState           = SUART_IDLE;      /* RX service starts idle */
	SUART_u8RxTerminatorIndex = 0U;              /* Terminator match index reset */
	SUART_RX_Fptr             = NULLPTR;         /* No RX notification callback */

	/* Link service handlers with UART MCAL callbacks */
	Local_TxCbState = UART_UDRE_SetCallBack(SUART_TxHandler);
	Local_RxCbState = UART_RX_SetCallBack(SUART_RxHandler);

	/* Check callback registration result */
	if ((Local_TxCbState != OK) || (Local_RxCbState != OK))
	{
		Local_ErrorState = NOK; /* Initialization failed */
	}
	else
	{
		/* Keep service-controlled interrupts disabled until needed */
		UART_UDRE_InterruptDisable();
		UART_RX_InterruptDisable();
	}

	return Local_ErrorState;
}
error_t SUART_SendStringAsync(const u8 *Add_pu8Str)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check input pointer validity */
	if (Add_pu8Str == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	else
	{
		/* Check if TX queue has free space */
		if (SUART_u8TxCount < SUART_TX_QUEUE_SIZE)
		{
			/* Store the string pointer in the TX queue */
			SUART_TxQueue[SUART_u8TxTail].Str  = Add_pu8Str;
			SUART_TxQueue[SUART_u8TxTail].Mode = SUART_TX_NORMAL;

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
				SUART_u8TxState = SUART_BUSY;       /* Mark TX service as busy */
				UART_UDRE_InterruptEnable();        /* Start interrupt-driven transmission */
			}
			else
			{
				; /* Transmission already in progress */
			}
		}
		else
		{
			/* Queue is full */
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}
error_t SUART_SendStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count)
{
	error_t Local_ErrorState = OK; /* Final function status */
	u8 Local_u8Index = 0U;         /* Loop counter */

	/* Check array pointer validity */
	if (Add_pu8StrArr == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check count validity */
	else if (Copy_u8Count == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	/* Check if TX queue has enough free space for all strings */
	else if ((SUART_u8TxCount + Copy_u8Count) > SUART_TX_QUEUE_SIZE)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		/* Validate all string pointers first */
		for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
		{
			if (Add_pu8StrArr[Local_u8Index] == NULLPTR)
			{
				Local_ErrorState = NULL_PTR;
				break;
			}
			else
			{
				;
			}
		}

		/* Queue all strings only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_TxQueue[SUART_u8TxTail].Str  = Add_pu8StrArr[Local_u8Index];
				SUART_TxQueue[SUART_u8TxTail].Mode = SUART_TX_NORMAL;

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
error_t SUART_SendRawStringAsync(const u8 *Add_pu8Str)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check input pointer validity */
	if (Add_pu8Str == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	else
	{
		/* Check if TX queue has free space */
		if (SUART_u8TxCount < SUART_TX_QUEUE_SIZE)
		{
			/* Store the raw string pointer in the TX queue */
			SUART_TxQueue[SUART_u8TxTail].Str  = Add_pu8Str;
			SUART_TxQueue[SUART_u8TxTail].Mode = SUART_TX_RAW;

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
				SUART_u8TxState = SUART_BUSY; /* Mark TX service as busy */
				UART_UDRE_InterruptEnable();  /* Start interrupt-driven transmission */
			}
			else
			{
				; /* Transmission already in progress */
			}
		}
		else
		{
			/* Queue is full */
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}
error_t SUART_SendRawStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count)
{
	error_t Local_ErrorState = OK; /* Final function status */
	u8 Local_u8Index = 0U;         /* Loop counter */

	/* Check array pointer validity */
	if (Add_pu8StrArr == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check count validity */
	else if (Copy_u8Count == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	/* Check if TX queue has enough free space for all raw strings */
	else if ((SUART_u8TxCount + Copy_u8Count) > SUART_TX_QUEUE_SIZE)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		/* Validate all raw string pointers first */
		for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
		{
			if (Add_pu8StrArr[Local_u8Index] == NULLPTR)
			{
				Local_ErrorState = NULL_PTR;
				break;
			}
			else
			{
				;
			}
		}

		/* Queue all raw strings only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_TxQueue[SUART_u8TxTail].Str  = Add_pu8StrArr[Local_u8Index];
				SUART_TxQueue[SUART_u8TxTail].Mode = SUART_TX_RAW;

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
error_t SUART_ReceiveStringAsync(u8 *Add_pu8Str, u16 Copy_u16BufferSize)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check buffer pointer validity */
	if (Add_pu8Str == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check buffer size validity */
	else if (Copy_u16BufferSize == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	else
	{
		/* Check if RX queue has free space */
		if (SUART_u8RxCount < SUART_RX_QUEUE_SIZE)
		{
			/* Store the destination buffer in the RX queue */
			SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8Str;
			SUART_RxQueue[SUART_u8RxTail].Length = Copy_u16BufferSize;
			SUART_RxQueue[SUART_u8RxTail].Mode   = SUART_RX_STRING;

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
				SUART_u8RxState = SUART_BUSY; /* Mark RX service as busy */
				UART_RX_InterruptEnable();    /* Start interrupt-driven reception */
			}
			else
			{
				; /* Reception already in progress */
			}
		}
		else
		{
			/* Queue is full */
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}
error_t SUART_ReceiveStringsAsync(u8 *Add_pu8StrArr[], const u16 Add_pu16BufferSizeArr[], u8 Copy_u8Count)
{
	error_t Local_ErrorState = OK; /* Final function status */
	u8 Local_u8Index = 0U;         /* Loop counter */

	/* Check input array pointers validity */
	if ((Add_pu8StrArr == NULLPTR) || (Add_pu16BufferSizeArr == NULLPTR))
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
		/* Validate all buffers and sizes first */
		for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
		{
			if (Add_pu8StrArr[Local_u8Index] == NULLPTR)
			{
				Local_ErrorState = NULL_PTR;
				break;
			}
			else if (Add_pu16BufferSizeArr[Local_u8Index] == 0U)
			{
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
			else
			{
				;
			}
		}

		/* Queue all RX string requests only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8StrArr[Local_u8Index];
				SUART_RxQueue[SUART_u8RxTail].Length = Add_pu16BufferSizeArr[Local_u8Index];
				SUART_RxQueue[SUART_u8RxTail].Mode   = SUART_RX_STRING;

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
error_t SUART_ReceiveRawBufferAsync(u8 *Add_pu8Buffer, u16 Copy_u16Length)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check buffer pointer validity */
	if (Add_pu8Buffer == NULLPTR)
	{
		Local_ErrorState = NULL_PTR;
	}
	/* Check requested length validity */
	else if (Copy_u16Length == 0U)
	{
		Local_ErrorState = OUT_OF_RANGE;
	}
	else
	{
		/* Check if RX queue has free space */
		if (SUART_u8RxCount < SUART_RX_QUEUE_SIZE)
		{
			/* Store the raw buffer request in the RX queue */
			SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8Buffer;
			SUART_RxQueue[SUART_u8RxTail].Length = Copy_u16Length;
			SUART_RxQueue[SUART_u8RxTail].Mode   = SUART_RX_RAW;

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
				SUART_u8RxState = SUART_BUSY; /* Mark RX service as busy */
				UART_RX_InterruptEnable();    /* Start interrupt-driven reception */
			}
			else
			{
				; /* Reception already in progress */
			}
		}
		else
		{
			/* Queue is full */
			Local_ErrorState = NOK;
		}
	}

	return Local_ErrorState;
}
error_t SUART_ReceiveRawBuffersAsync(u8 *Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count)
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
		/* Validate all buffers and lengths first */
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

		/* Queue all raw RX requests only if validation succeeded */
		if (Local_ErrorState == OK)
		{
			for (Local_u8Index = 0U; Local_u8Index < Copy_u8Count; Local_u8Index++)
			{
				SUART_RxQueue[SUART_u8RxTail].Buffer = Add_pu8BufferArr[Local_u8Index];
				SUART_RxQueue[SUART_u8RxTail].Length = Add_pu16LengthArr[Local_u8Index];
				SUART_RxQueue[SUART_u8RxTail].Mode   = SUART_RX_RAW;

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
		SUART_TX_Fptr = Add_pfCallBack; /* Store TX notification callback */
	}
	else
	{
		Local_ErrorState = NULL_PTR; /* Invalid input pointer */
	}

	return Local_ErrorState;
}
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack)
{
	error_t Local_ErrorState = OK; /* Final function status */

	/* Check callback pointer validity */
	if (Add_pfCallBack != NULLPTR)
	{
		SUART_RX_Fptr = Add_pfCallBack; /* Store RX notification callback */
	}
	else
	{
		Local_ErrorState = NULL_PTR; /* Invalid input pointer */
	}

	return Local_ErrorState;
}