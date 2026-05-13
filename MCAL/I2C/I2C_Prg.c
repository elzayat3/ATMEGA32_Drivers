#include "StdTypes.h"
#include "MemMap.h"
#include "TWI_Int.h"
#include "TWI_Private.h"
#include "TWI_Cfg.h"

/********************************************************************/
/*                        Private Functions                         */
/********************************************************************/

static error_t TWI_WaitForTWINT(void)
{
    u32 local_u32Timeout = TWI_TIMEOUT_COUNTS;
    error_t local_ErrorState = OK;

    while ((GET_BIT(TWCR, TWINT) == 0u) && (local_u32Timeout > 0u))
    {
        local_u32Timeout--;
    }

    if (local_u32Timeout == 0u)
    {
        local_ErrorState = TIMEOUT;
    }

    return local_ErrorState;
}

static error_t TWI_GetPrescalerBits(u8 *ptr_u8PrescalerBits, u8 *ptr_u8PrescalerValue)
{
    error_t local_ErrorState = OK;

    if ((ptr_u8PrescalerBits == NULLPTR) || (ptr_u8PrescalerValue == NULLPTR))
    {
        local_ErrorState = NULL_PTR;
    }
    else
    {
        switch (TWI_PRESCALER_CFG)
        {
            case TWI_PRESCALER_1:
                *ptr_u8PrescalerBits  = TWI_PRESCALER_BITS_1;
                *ptr_u8PrescalerValue = 1u;
                break;

            case TWI_PRESCALER_4:
                *ptr_u8PrescalerBits  = TWI_PRESCALER_BITS_4;
                *ptr_u8PrescalerValue = 4u;
                break;

            case TWI_PRESCALER_16:
                *ptr_u8PrescalerBits  = TWI_PRESCALER_BITS_16;
                *ptr_u8PrescalerValue = 16u;
                break;

            case TWI_PRESCALER_64:
                *ptr_u8PrescalerBits  = TWI_PRESCALER_BITS_64;
                *ptr_u8PrescalerValue = 64u;
                break;

            default:
                local_ErrorState = OUT_OF_RANGE;
                break;
        }
    }

    return local_ErrorState;
}

/********************************************************************/
/*                        Public Functions                          */
/********************************************************************/

error_t TWI_Init(void)
{
    error_t local_ErrorState = OK;
    u8 local_u8PrescalerBits = 0u;
    u8 local_u8PrescalerValue = 0u;
    u32 local_u32TWBRValue = 0u;
    u32 local_u32Numerator = 0u;
    u32 local_u32Denominator = 0u;
	

    local_ErrorState = TWI_GetPrescalerBits(&local_u8PrescalerBits, &local_u8PrescalerValue);

    if (local_ErrorState == OK)
    {
        if ((TWI_SCL_FREQ_HZ == 0u) ||
            (TWI_CPU_FREQ_HZ <= (TWI_FREQ_BASE_VALUE * TWI_SCL_FREQ_HZ)))
        {
            local_ErrorState = OUT_OF_RANGE;
        }
        else
        {
            /*
             * SCL = F_CPU / (16 + 2 * TWBR * Prescaler)
             * TWBR = (F_CPU - 16*SCL) / (2 * Prescaler * SCL)
             */
            local_u32Numerator   = TWI_CPU_FREQ_HZ - (TWI_FREQ_BASE_VALUE * TWI_SCL_FREQ_HZ);
            local_u32Denominator = TWI_FREQ_MULTIPLIER_VALUE *
                                    ((u32)local_u8PrescalerValue) *
                                    TWI_SCL_FREQ_HZ;

            local_u32TWBRValue = local_u32Numerator / local_u32Denominator;

            if (local_u32TWBRValue > TWI_MAX_U8_VALUE)
            {
                local_ErrorState = OUT_OF_RANGE;
            }
            else
            {
                /* Set prescaler bits in TWSR */
                TWSR &= (u8)(~TWI_PRESCALER_BITS_MASK);
                TWSR |= local_u8PrescalerBits;

                /* Set bit rate register */
                TWBR = (u8)local_u32TWBRValue;

                /* Enable TWI peripheral */
                TWCR = TWI_ENABLE_COMMAND;
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_StartCondition(void)
{
    error_t local_ErrorState = OK;

    TWCR = TWI_START_COMMAND;

    local_ErrorState = TWI_WaitForTWINT();

    if (local_ErrorState == OK)
    {
        if (TWI_GetStatus() != TWI_START_STATUS)
        {
            local_ErrorState = NOK;
        }
    }

    return local_ErrorState;
}

error_t TWI_RepeatedStart(void)
{
    error_t local_ErrorState = OK;

    TWCR = TWI_START_COMMAND;

    local_ErrorState = TWI_WaitForTWINT();

    if (local_ErrorState == OK)
    {
        if (TWI_GetStatus() != TWI_REPEATED_START_STATUS)
        {
            local_ErrorState = NOK;
        }
    }

    return local_ErrorState;
}

error_t TWI_StopCondition(void)
{
    error_t local_ErrorState = OK;
    u32 local_u32Timeout = TWI_TIMEOUT_COUNTS;

    TWCR = TWI_STOP_COMMAND;

    while ((GET_BIT(TWCR, TWSTO) == 1u) && (local_u32Timeout > 0u))
    {
        local_u32Timeout--;
    }

    if (local_u32Timeout == 0u)
    {
        local_ErrorState = TIMEOUT;
    }

    return local_ErrorState;
}

error_t TWI_SendSlaveAddressWrite(u8 copy_u8SlaveAddress)
{
    error_t local_ErrorState = OK;

    if (copy_u8SlaveAddress > TWI_MAX_7BIT_ADDRESS)
    {
        local_ErrorState = OUT_OF_RANGE;
    }
    else
    {
        TWDR = (u8)((copy_u8SlaveAddress << TWI_ADDRESS_SHIFT) | TWI_WRITE_BIT);
		
		/* Clear TWINT flag to start transmitting the byte loaded in TWDR */
        TWCR = TWI_CLEAR_FLAG_ENABLE;

        local_ErrorState = TWI_WaitForTWINT();

        if (local_ErrorState == OK)
        {
			//SLA+W transmitted and ACK received
            if (TWI_GetStatus() != TWI_MT_SLA_W_ACK_STATUS)
            {
                local_ErrorState = NOK;
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_SendSlaveAddressRead(u8 copy_u8SlaveAddress)
{
    error_t local_ErrorState = OK;

    if (copy_u8SlaveAddress > TWI_MAX_7BIT_ADDRESS)
    {
        local_ErrorState = OUT_OF_RANGE;
    }
    else
    {
        TWDR = (u8)((copy_u8SlaveAddress << TWI_ADDRESS_SHIFT) | TWI_READ_BIT);
		
		/* Clear TWINT flag to start transmitting the byte loaded in TWDR */
        TWCR = TWI_CLEAR_FLAG_ENABLE;

        local_ErrorState = TWI_WaitForTWINT();

        if (local_ErrorState == OK)
        {
            if (TWI_GetStatus() != TWI_MR_SLA_R_ACK_STATUS)
            {
                local_ErrorState = NOK;
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterSendData(u8 copy_u8Data)
{
    error_t local_ErrorState = OK;

    TWDR = copy_u8Data;
	
	/* Clear TWINT flag to start transmitting the byte loaded in TWDR */
    TWCR = TWI_CLEAR_FLAG_ENABLE;

    local_ErrorState = TWI_WaitForTWINT();

    if (local_ErrorState == OK)
    {
        if (TWI_GetStatus() != TWI_MT_DATA_ACK_STATUS)
        {
            local_ErrorState = NOK;
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterReceiveDataAck(u8 *ptr_u8Data)
{
    error_t local_ErrorState = OK;

    if (ptr_u8Data == NULLPTR)
    {
        local_ErrorState = NULL_PTR;
    }
    else
    {
        TWCR = TWI_READ_ACK_COMMAND;

        local_ErrorState = TWI_WaitForTWINT();

        if (local_ErrorState == OK)
        {
            if (TWI_GetStatus() == TWI_MR_DATA_ACK_STATUS)
            {
                *ptr_u8Data = TWDR;
            }
            else
            {
                local_ErrorState = NOK;
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterReceiveDataNack(u8 *ptr_u8Data)
{
    error_t local_ErrorState = OK;

    if (ptr_u8Data == NULLPTR)
    {
        local_ErrorState = NULL_PTR;
    }
    else
    {
        TWCR = TWI_READ_NACK_COMMAND;

        local_ErrorState = TWI_WaitForTWINT();

        if (local_ErrorState == OK)
        {
            if (TWI_GetStatus() == TWI_MR_DATA_NACK_STATUS)
            {
                *ptr_u8Data = TWDR;
            }
            else
            {
                local_ErrorState = NOK;
            }
        }
    }

    return local_ErrorState;
}

u8 TWI_GetStatus(void)
{
    return (u8)(TWSR & TWI_STATUS_MASK);
}

error_t TWI_MasterWriteByte(u8 copy_u8SlaveAddress, u8 copy_u8Data)
{
    error_t local_ErrorState = OK;

    local_ErrorState = TWI_StartCondition();

    if (local_ErrorState == OK)
    {
        local_ErrorState = TWI_SendSlaveAddressWrite(copy_u8SlaveAddress);

        if (local_ErrorState == OK)
        {
            local_ErrorState = TWI_MasterSendData(copy_u8Data);
        }

        if (local_ErrorState == OK)
        {
            local_ErrorState = TWI_StopCondition();
        }
        else
        {
            (void)TWI_StopCondition();
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterReadByte(u8 copy_u8SlaveAddress, u8 *ptr_u8Data)
{
    error_t local_ErrorState = OK;

    if (ptr_u8Data == NULLPTR)
    {
        local_ErrorState = NULL_PTR;
    }
    else
    {
        local_ErrorState = TWI_StartCondition();

        if (local_ErrorState == OK)
        {
            local_ErrorState = TWI_SendSlaveAddressRead(copy_u8SlaveAddress);

            if (local_ErrorState == OK)
            {
                local_ErrorState = TWI_MasterReceiveDataNack(ptr_u8Data);
            }

            if (local_ErrorState == OK)
            {
                local_ErrorState = TWI_StopCondition();
            }
            else
            {
                (void)TWI_StopCondition();
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterWriteBuffer(u8 copy_u8SlaveAddress,
                              const u8 *ptr_u8Buffer,
                              u8 copy_u8Size)
{
    error_t local_ErrorState = OK;
    u8 local_u8Counter = 0u;

    if (ptr_u8Buffer == NULLPTR)
    {
        local_ErrorState = NULL_PTR;
    }
    else if (copy_u8Size == 0u)
    {
        local_ErrorState = OUT_OF_RANGE;
    }
    else
    {
        local_ErrorState = TWI_StartCondition();

        if (local_ErrorState == OK)
        {
            local_ErrorState = TWI_SendSlaveAddressWrite(copy_u8SlaveAddress);

            for (local_u8Counter = 0u;
                 (local_u8Counter < copy_u8Size) && (local_ErrorState == OK);
                 local_u8Counter++)
            {
                local_ErrorState = TWI_MasterSendData(ptr_u8Buffer[local_u8Counter]);
            }

            if (local_ErrorState == OK)
            {
                local_ErrorState = TWI_StopCondition();
            }
            else
            {
                (void)TWI_StopCondition();
            }
        }
    }

    return local_ErrorState;
}

error_t TWI_MasterReadBuffer(u8 copy_u8SlaveAddress,
                             u8 *ptr_u8Buffer,
                             u8 copy_u8Size)
{
    error_t local_ErrorState = OK;
    u8 local_u8Counter = 0u;

    if (ptr_u8Buffer == NULLPTR)
    {
        local_ErrorState = NULL_PTR;
    }
    else if (copy_u8Size == 0u)
    {
        local_ErrorState = OUT_OF_RANGE;
    }
    else
    {
        local_ErrorState = TWI_StartCondition();

        if (local_ErrorState == OK)
        {
            local_ErrorState = TWI_SendSlaveAddressRead(copy_u8SlaveAddress);

            if (local_ErrorState == OK)
            {
                for (local_u8Counter = 0u;
                     (local_u8Counter < copy_u8Size) && (local_ErrorState == OK);
                     local_u8Counter++)
                {
                    if (local_u8Counter == (copy_u8Size - 1u))
                    {
                        local_ErrorState = TWI_MasterReceiveDataNack(&ptr_u8Buffer[local_u8Counter]);
                    }
                    else
                    {
                        local_ErrorState = TWI_MasterReceiveDataAck(&ptr_u8Buffer[local_u8Counter]);
                    }
                }
            }

            if (local_ErrorState == OK)
            {
                local_ErrorState = TWI_StopCondition();
            }
            else
            {
                (void)TWI_StopCondition();
            }
        }
    }

    return local_ErrorState;
}