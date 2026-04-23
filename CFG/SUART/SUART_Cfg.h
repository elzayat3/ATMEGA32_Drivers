#ifndef SUART_CFG_H_
#define SUART_CFG_H_

/**
 * @file SUART_Cfg.h
 * @brief Configuration file for SUART service.
 *
 * @author Abdelrahman Elzayat
 */

/**
 * @brief TX queue capacity.
 *
 * This value defines how many pending string requests can be stored
 * before the queue becomes full.
 */
#define SUART_TX_QUEUE_SIZE    5U

#endif /* SUART_CFG_H_ */