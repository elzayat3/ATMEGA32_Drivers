#include "StdTypes.h"
#include "Uart_Int.h"
#include "SUART_Int.h"
#include "SUART_Private.h"
#include "SUART_Cfg.h"

static error_t TX_INQ(const c8 *str);
static error_t TX_DEQ(const c8 **str);
static void SUART_TxISR(void);
static void SUART_RxISR(void);


static const c8*TX_QUEUE[SUART_TX_QUEUE_SIZE]={NULLPTR};
static c8 *RX_Buffer = NULLPTR;
	
static volatile u8 TX_OUT=0;
static volatile u8 TX_IN=0;
static volatile u8 TX_Count=0;
static const c8 * TX_Str = NULLPTR;


static u8 volatile RX_index=0;
static u8 volatile RX_Size=0;
static volatile u8  RX_Busy=0;

static SUART_Callback_t SUART_TX_Fptr = NULLPTR;
static SUART_Callback_t SUART_RX_Fptr = NULLPTR;


static error_t TX_INQ(const c8*str)
{
	error_t status =OK; 
	if(TX_IN >= SUART_TX_QUEUE_SIZE )
	{
		TX_IN=0;
	}
	else{;}
		
	if(str == NULLPTR)
	{
		status =NULL_PTR;
	}
	else if(TX_Count >= SUART_TX_QUEUE_SIZE)
	{
		status=FULL;
	}
	else
	{
		TX_QUEUE[TX_IN]=str;
		TX_IN++;
		TX_Count++;
	}
	
	return status;
}
static error_t TX_DEQ(const c8**str)
{
	error_t state=OK;
	if(TX_OUT >= SUART_TX_QUEUE_SIZE)
	{
		TX_OUT=0;
	}
	else{;}
		
	if( str == NULLPTR )
	{
		state=NULL_PTR;
	}	
	else if(TX_Count == 0)
	{
		state=EMPTY;
	}
	else
	{
		*str=TX_QUEUE[TX_OUT];
		TX_QUEUE[TX_OUT] = NULLPTR;
		TX_OUT++;
		TX_Count--;
		
	}
	return state;
}

static void SUART_TxISR(void)
{
	static u8 TX_index=0;
	static u8 flag=0;
	static error_t state =OK;
	if(flag == 0)
	{
		state = TX_DEQ(&TX_Str);
		flag=1; 
	} 
	else{;}
	if(state == OK)
	{
		if(TX_Str[TX_index] != 0)
		{
			UART_SendDirect(TX_Str[TX_index]);
			TX_index++;
		}
		else
		{
			TX_index=0;
			flag=0;
			if (SUART_TX_Fptr != NULLPTR)
			{
				SUART_TX_Fptr();
			}
			else{;}
		}
		
	}
	else
	{
		flag=0;
		TX_index = 0U;
		TX_Str = NULLPTR;
		UART_UDRE_InterruptDisable();
	}
	
}
static void SUART_RxISR(void)
{
	RX_Buffer[RX_index++]=UART_ReceiveDirect();
	if(RX_index>=RX_Size)
	{
		UART_RX_InterruptDisable();
		RX_Busy=0U;
		RX_index = 0U;
		if (SUART_RX_Fptr != NULLPTR)
		{
			SUART_RX_Fptr();
		}
		else{;}
		RX_Buffer = NULLPTR;
	}
	else{;}
	
}

error_t SUART_Init(void)
{
	error_t Error_State = OK;  /* Final function status */
	
	/* Link lightweight ISR callbacks with UART MCAL */
	if (UART_UDRE_SetCallBack(SUART_TxISR) != OK)
	{
		Error_State = NOK;
	}
	else if (UART_RX_SetCallBack(SUART_RxISR) != OK)
	{
		Error_State = NOK;
	}
	else
	{
		/* Do nothing */
	}

	/* Keep UART interrupts disabled until the service needs them */
	UART_RX_InterruptDisable();
	UART_UDRE_InterruptDisable();

	return Error_State;
}

error_t SUART_SendAsync(const c8 *Data)
{
	error_t State =OK;
	if(Data == NULLPTR)
	{
		State=NULL_PTR;
	}
	else if (Data[0] == '\0')
	{
		State = NOK;
	}
	else
	{
		UART_UDRE_InterruptDisable();
		State = TX_INQ(Data);
		if( (State == OK) || (State == FULL))
		{
			UART_UDRE_InterruptEnable();
		}
		else{;}
	}
	return State;
}
error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack)
{
	error_t state =OK;
	if(Add_pfCallBack != NULLPTR)
	{
		SUART_TX_Fptr=Add_pfCallBack;
	}
	else
	{
		state=NULL_PTR;
	}
	return state;
}
error_t SUART_ReceiveAsync( c8 *srt , u8 size)
{
	error_t state =OK;
	if(RX_Busy == 0U)
	{
		if( (size <= 1U))
		{
			state = OUT_OF_RANGE;
		}
		else if(srt == NULLPTR)
		{
			state=NULL_PTR;
		}
		else
		{
			RX_Busy=1U;
			RX_index = 0U;
			RX_Buffer=srt;
			RX_Size=size-1U;
			RX_Buffer[size-1U] = '\0';
			UART_RX_InterruptEnable();
		}
	}
	else
	{
		state = IN_PROGRESS;
	}
	return state;
}
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack)
{
	error_t state =OK;
	if(Add_pfCallBack != NULLPTR)
	{
		SUART_RX_Fptr=Add_pfCallBack;
	}
	else
	{
		state=NULL_PTR;
	}
	return state;
	
}
