/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2006-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: bcm283x.h 1838 2016-10-17 00:00:00Z azo $
 */

#ifndef TOPPERS_BCM283X_H
#define TOPPERS_BCM283X_H

#include <sil.h>
#include "mpcore.h"

/*
 *  メモリマップの定義（MMUに設定するために必要）
 */
#define SDRAM_ADDR		0x00000000		/* 内蔵RAM */
#if defined(TOPPERS_USE_BCM2835)
#define SDRAM_SIZE		0x10000000		/* 256MB */
#elif defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
#define SDRAM_SIZE		0x3F000000		/* about 1GB */
#else
#error Either TOPPERS_USE_BCM283X must be defined.
#endif

/*
 *  BCM383Xペリフェラルアドレス
 */
#if defined(TOPPERS_USE_BCM2835)
#define BCM283X_PER_BASE	0x20000000
#elif defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
#define BCM283X_PER_BASE	0x3F000000
#else
#error Define BCM283X type.
#endif

#define IO1_ADDR		BCM283X_PER_BASE	/* I/O領域（予約領域を含む）*/
#define IO1_SIZE		0x01000000		/* 16MB */
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
#define IO2_ADDR		0x40000000		/* I/O領域（予約領域を含む）*/
#define IO2_SIZE		0x01000000		/* 16MB */
#endif

/*
 * 割り込みの数
 */
#define BCM283X_TNUM_INT		UINT_C(128)
#define BCM283X_TMIN_INTNO		0U
#define BCM283X_TMAX_INTNO		(BCM283X_TMIN_INTNO + BCM283X_TNUM_INT - 1)

/*
 * 割込みハンドラ番号から，IRC操作のためのビットパターンを求めるマクロ(0<-intno<-31)
 */
#define INTNO_BITPAT(intno) (1U << (intno))

/*
 *  割込みコントローラ関係
 */
#define AIC_BASE		(BCM283X_PER_BASE + 0xB200)
#define AIC_BASIC		(AIC_BASE + 0x00)
#define AIC_PEND1		(AIC_BASE + 0x04)
#define AIC_PEND2		(AIC_BASE + 0x08)
#define AIC_FIQ_CONTROL		(AIC_BASE + 0x0C)
#define AIC_ENABLE_IRQ1		(AIC_BASE + 0x10)
#define AIC_ENABLE_IRQ2		(AIC_BASE + 0x14)
#define AIC_ENABLE_BASIC	(AIC_BASE + 0x18)
#define AIC_DISABLE_IRQ1	(AIC_BASE + 0x1C)
#define AIC_DISABLE_IRQ2	(AIC_BASE + 0x20)
#define AIC_DISABLE_BASIC	(AIC_BASE + 0x24)

#define IRQ_STIMER	33
#define IRQ_UART0	89
#define IRQ_AUX_MU	61
#define IRQ_MB0		100
#define IRQ_MB1		101
#define IRQ_MB2		102
#define IRQ_MB3		103
#define IRQ_LTIMER	107

/* target/rpn_gcc/target_sil.h の切り替えも必要 */
//#if BCM283X_USE_UART == 1
#define IRQ_UART	IRQ_AUX_MU
//#else	/* BCM283X_USE_UART */
//#define IRQ_UART	IRQ_UART0
//#endif	/* BCM283X_USE_UART */

/*
 *  GPIO関連
 */
#define BCM283X_GPIO_BASE (BCM283X_PER_BASE + 0x200000)
#define BCM283X_GPIO_GPFSEL0	(BCM283X_GPIO_BASE + 0x00)	/* GPIO Function Select 0 32bit R/W */
#define BCM283X_GPIO_GPFSEL1	(BCM283X_GPIO_BASE + 0x04)	/* GPIO Function Select 1 32bit R/W */
#define BCM283X_GPIO_GPFSEL2	(BCM283X_GPIO_BASE + 0x08)	/* GPIO Function Select 2 32bit R/W */
#define BCM283X_GPIO_GPFSEL3	(BCM283X_GPIO_BASE + 0x0C)	/* GPIO Function Select 3 32bit R/W */
#define BCM283X_GPIO_GPFSEL4	(BCM283X_GPIO_BASE + 0x10)	/* GPIO Function Select 4 32bit R/W */
#define BCM283X_GPIO_GPFSEL5	(BCM283X_GPIO_BASE + 0x14)	/* GPIO Function Select 5 32bit R/W */
#define BCM283X_GPIO_GPSET0	(BCM283X_GPIO_BASE + 0x1C)	/* GPIO Pin Output Set 0 32bit W */
#define BCM283X_GPIO_GPSET1	(BCM283X_GPIO_BASE + 0x20)	/* GPIO Pin Output Set 1 32bit W */
#define BCM283X_GPIO_GPCLR0	(BCM283X_GPIO_BASE + 0x28)	/* GPIO Pin Output Clear 0 32bit W */
#define BCM283X_GPIO_GPCLR1	(BCM283X_GPIO_BASE + 0x2C)	/* GPIO Pin Output Clear 1 32bit W */
#define BCM283X_GPIO_GPLEV0	(BCM283X_GPIO_BASE + 0x34)	/* GPIO Pin Level 0 32bit R */
#define BCM283X_GPIO_GPLEV1	(BCM283X_GPIO_BASE + 0x38)	/* GPIO Pin Level 1 32bit R */
#define BCM283X_GPIO_GPEDS0	(BCM283X_GPIO_BASE + 0x40)	/* GPIO Pin Event Detect Status 0 32bit R/W */
#define BCM283X_GPIO_GPEDS1	(BCM283X_GPIO_BASE + 0x44)	/* GPIO Pin Event Detect Status 1 32bit R/W */
#define BCM283X_GPIO_GPREN0	(BCM283X_GPIO_BASE + 0x4C)	/* GPIO Pin Rising Edge Detect Enable 0 32bit R/W */
#define BCM283X_GPIO_GPREN1	(BCM283X_GPIO_BASE + 0x50)	/* GPIO Pin Rising Edge Detect Enable 1 32bit R/W */
#define BCM283X_GPIO_GPFEN0	(BCM283X_GPIO_BASE + 0x58)	/* GPIO Pin Falling Edge Detect Enable 0 32bit R/W */
#define BCM283X_GPIO_GPFEN1	(BCM283X_GPIO_BASE + 0x5C)	/* GPIO Pin Falling Edge Detect Enable 1 32bit R/W */
#define BCM283X_GPIO_GPHEN0	(BCM283X_GPIO_BASE + 0x64)	/* GPIO Pin High Detect Enable 0 32bit R/W */
#define BCM283X_GPIO_GPHEN1	(BCM283X_GPIO_BASE + 0x68)	/* GPIO Pin High Detect Enable 1 32bit R/W */
#define BCM283X_GPIO_GPLEN0	(BCM283X_GPIO_BASE + 0x70)	/* GPIO Pin Low Detect Enable 0 32bit R/W */
#define BCM283X_GPIO_GPLEN1	(BCM283X_GPIO_BASE + 0x74)	/* GPIO Pin Low Detect Enable 1 32bit R/W */
#define BCM283X_GPIO_GPAREN0	(BCM283X_GPIO_BASE + 0x7C)	/* GPIO Pin Async. Rising Edge Detect 0 32bit R/W */
#define BCM283X_GPIO_GPAREN1	(BCM283X_GPIO_BASE + 0x80)	/* GPIO Pin Async. Rising Edge Detect 1 32bit R/W */
#define BCM283X_GPIO_GPAFEN0	(BCM283X_GPIO_BASE + 0x88)	/* GPIO Pin Async. Falling Edge Detect 0 32bit R/W */
#define BCM283X_GPIO_GPAFEN1	(BCM283X_GPIO_BASE + 0x8C)	/* GPIO Pin Async. Falling Edge Detect 1 32bit R/W */
#define BCM283X_GPIO_GPPUD	(BCM283X_GPIO_BASE + 0x94)	/* GPIO Pin Pull-up/down Enable 32bit R/W */
#define BCM283X_GPIO_GPPUDCLK0	(BCM283X_GPIO_BASE + 0x98)	/* GPIO Pin Pull-up/down Enable Clock 0 32bit R/W */
#define BCM283X_GPIO_GPPUDCLK1	(BCM283X_GPIO_BASE + 0x9C)	/* GPIO Pin Pull-up/down Enable Clock 1 32bit R/W */

/*
 *  UARTレジスタの番地の定義
 */
#define BCM283X_UART0_BASE (BCM283X_PER_BASE + 0x201000)
#define BCM283X_UART0_DR	(BCM283X_UART0_BASE + 0x00)	/* Data Register */
#define BCM283X_UART0_RSRECR	(BCM283X_UART0_BASE + 0x04)
#define BCM283X_UART0_FR	(BCM283X_UART0_BASE + 0x18)	/* Flag register */
#define BCM283X_UART0_ILPR	(BCM283X_UART0_BASE + 0x20)
#define BCM283X_UART0_IBRD	(BCM283X_UART0_BASE + 0x24)	/* Integer Baud rate divisor */
#define BCM283X_UART0_FBRD	(BCM283X_UART0_BASE + 0x28)	/* Fractional Baud rate divisor	*/
#define BCM283X_UART0_LCRH	(BCM283X_UART0_BASE + 0x2C)	/* Line Control register */
#define BCM283X_UART0_CR	(BCM283X_UART0_BASE + 0x30)	/* Control register */
#define BCM283X_UART0_IFLS	(BCM283X_UART0_BASE + 0x34)	/* Interupt FIFO Level Select Register */
#define BCM283X_UART0_IMSC	(BCM283X_UART0_BASE + 0x38)	/* Interupt Mask Set Clear Register */
#define BCM283X_UART0_RIS	(BCM283X_UART0_BASE + 0x3C)	/* Raw Interupt Status Register */
#define BCM283X_UART0_MIS	(BCM283X_UART0_BASE + 0x40)	/* Masked Interupt Status Register */
#define BCM283X_UART0_ICR	(BCM283X_UART0_BASE + 0x44)	/* Interupt Clear Register */
#define BCM283X_UART0_DMACR	(BCM283X_UART0_BASE + 0x48)	/* DMA Control Register */
#define BCM283X_UART0_ITCR	(BCM283X_UART0_BASE + 0x80)	/* Test Control register */
#define BCM283X_UART0_ITIP	(BCM283X_UART0_BASE + 0x84)	/* Integration test input reg */
#define BCM283X_UART0_ITOP	(BCM283X_UART0_BASE + 0x88)	/* Integration test output reg */
#define BCM283X_UART0_TDR	(BCM283X_UART0_BASE + 0x8C)	/* Test Data reg */

/*
 *  システムタイマ関係
 */
#define BCM283X_STIMER_BASE (BCM283X_PER_BASE + 0x3000)
#define BCM283X_STIMER_CS	(BCM283X_STIMER_BASE + 0x00)	/* System Timer Control / Status */
#define BCM283X_STIMER_CLO	(BCM283X_STIMER_BASE + 0x04)	/* System Timer Counter Lower bits. */
#define BCM283X_STIMER_CHI	(BCM283X_STIMER_BASE + 0x08)	/* System Timer Counter Higher bits. */
#define BCM283X_STIMER_C0	(BCM283X_STIMER_BASE + 0x0C)	/* System Timer Compare0. (not use) */
#define BCM283X_STIMER_C1	(BCM283X_STIMER_BASE + 0x10)	/* System Timer Compare1. */
#define BCM283X_STIMER_C2	(BCM283X_STIMER_BASE + 0x14)	/* System Timer Compare2. (not use) */
#define BCM283X_STIMER_C3	(BCM283X_STIMER_BASE + 0x18)	/* System Timer Compare3. */

/*
 *  Auxiliary関係
 */
#define BCM283X_AUX_BASE (BCM283X_PER_BASE + 0x215000)
#define BCM283X_AUX_IRQ			(BCM283X_AUX_BASE + 0x00)	/* Auxiliary Interrupt status 3 */
#define BCM283X_AUX_ENABLES		(BCM283X_AUX_BASE + 0x04)	/* Auxiliary enables 3bit */
#define BCM283X_AUX_MU_IO_REG		(BCM283X_AUX_BASE + 0x40)	/* Mini Uart I/O Data 8bit */
#define BCM283X_AUX_MU_IER_REG		(BCM283X_AUX_BASE + 0x44)	/* Mini Uart Interrupt Enable 8bit */
#define BCM283X_AUX_MU_IIR_REG		(BCM283X_AUX_BASE + 0x48)	/* Mini Uart Interrupt Identify 8bit */
#define BCM283X_AUX_MU_LCR_REG		(BCM283X_AUX_BASE + 0x4C)	/* Mini Uart Line Control 8bit */
#define BCM283X_AUX_MU_MCR_REG		(BCM283X_AUX_BASE + 0x50)	/* Mini Uart Modem Control 8bit */
#define BCM283X_AUX_MU_LSR_REG		(BCM283X_AUX_BASE + 0x54)	/* Mini Uart Line Status 8bit */
#define BCM283X_AUX_MU_MSR_REG		(BCM283X_AUX_BASE + 0x58)	/* Mini Uart Modem Status 8bit */
#define BCM283X_AUX_MU_SCRATCH		(BCM283X_AUX_BASE + 0x5C)	/* Mini Uart Scratch 8bit */
#define BCM283X_AUX_MU_CNTL_REG		(BCM283X_AUX_BASE + 0x60)	/* Mini Uart Extra Control 8bit */
#define BCM283X_AUX_MU_STAT_REG		(BCM283X_AUX_BASE + 0x64)	/* Mini Uart Extra Status 32bit */
#define BCM283X_AUX_MU_BAUD_REG		(BCM283X_AUX_BASE + 0x68)	/* Mini Uart Baudrate 16bit */
#define BCM283X_AUX_SPI0_CNTL0_REG	(BCM283X_AUX_BASE + 0x80)	/* SPI 1 Control register 0 32bit */
#define BCM283X_AUX_SPI0_CNTL1_REG	(BCM283X_AUX_BASE + 0x84)	/* SPI 1 Control register 1 8bit */
#define BCM283X_AUX_SPI0_STAT_REG	(BCM283X_AUX_BASE + 0x88)	/* SPI 1 Status 32bit */
#define BCM283X_AUX_SPI0_IO_REG		(BCM283X_AUX_BASE + 0x90)	/* SPI 1 Data 32bit */
#define BCM283X_AUX_SPI0_PEEK_REG	(BCM283X_AUX_BASE + 0x94)	/* SPI 1 Peek 16bit */
#define BCM283X_AUX_SPI1_CNTL0_REG	(BCM283X_AUX_BASE + 0xC0)	/* SPI 2 Control register 0 32bit */
#define BCM283X_AUX_SPI1_CNTL1_REG	(BCM283X_AUX_BASE + 0xC4)	/* SPI 2 Control register 1 8bit */

#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
#define BCM283X_PER_BASE_40000000 (0x40000000)

/*
 *  コアタイマ関係
 */
#define BCM283X_CTIMER_CTL	(BCM283X_PER_BASE_40000000 + 0x00)	/* Control register */
#define BCM283X_CTIMER_PRE	(BCM283X_PER_BASE_40000000 + 0x08)	/* Core timer prescaler */
#define BCM283X_CTIMER_LS32B	(BCM283X_PER_BASE_40000000 + 0x1C)	/* Core timer access LS 32 bits */
#define BCM283X_CTIMER_MS32B	(BCM283X_PER_BASE_40000000 + 0x20)	/* Core timer access MS 32 bits */

#define BCM283X_CTIMER_CLOCK_FREQ_MHZ 250

/*
 *  GPU関係
 */
#define BCM283X_GPUINT_ROUTE	(BCM283X_PER_BASE_40000000 + 0x0C)	/* interrupts routing */

/*
 *  Performance Monitor関係
 */
#define BCM283X_PMINT_ROUTESET	(BCM283X_PER_BASE_40000000 + 0x10)	/* Performance Monitor Interrupts routing-set */
#define BCM283X_PMINT_ROUTECLR	(BCM283X_PER_BASE_40000000 + 0x14)	/* Performance Monitor Interrupts routing-clear */

/*
 *  ローカルタイマ関係
 */
#define BCM283X_LTIMER_CTLSTA	(BCM283X_PER_BASE_40000000 + 0x34)	/* Local timer control & status */
#define BCM283X_LTIMER_WRITE	(BCM283X_PER_BASE_40000000 + 0x38)	/* Local timer write flags */
#define BCM283X_LTIMER_ROUTE	(BCM283X_PER_BASE_40000000 + 0x24)	/* Local Interrupt 0 [1-7] routing */

/*
 *  AXIタイマ関係
 */
#define BCM283X_AXI_CTL		(BCM283X_PER_BASE_40000000 + 0x2C)	/* Axi outstanding counters */
#define BCM283X_AXI_IRQ		(BCM283X_PER_BASE_40000000 + 0x30)	/* Axi outstanding IRQ */

/*
 *  ARM Generic Timer関係
 */
#define BCM283X_C0TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x40)	/* Core0 timers Interrupt control */
#define BCM283X_C1TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x44)	/* Core1 timers Interrupt control */
#define BCM283X_C2TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x48)	/* Core2 timers Interrupt control */
#define BCM283X_C3TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x4C)	/* Core3 timers Interrupt control */
/*
 *  ARM Coreメールボックス関係
 */
#define BCM283X_C0MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x50)	/* Core0 Mailboxes Interrupt control */
#define BCM283X_C1MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x54)	/* Core1 Mailboxes Interrupt control */
#define BCM283X_C2MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x58)	/* Core2 Mailboxes Interrupt control */
#define BCM283X_C3MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x5C)	/* Core3 Mailboxes Interrupt control */
/*
 *  ARM Core IRQ・FIQ要因関係
 */
#define BCM283X_C0_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x60)	/* Core0 IRQ Source */
#define BCM283X_C1_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x64)	/* Core1 IRQ Source */
#define BCM283X_C2_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x68)	/* Core2 IRQ Source */
#define BCM283X_C3_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x6C)	/* Core3 IRQ Source */
#define BCM283X_C0_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x70)	/* Core0 FIQ Source */
#define BCM283X_C1_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x74)	/* Core1 FIQ Source */
#define BCM283X_C2_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x78)	/* Core2 FIQ Source */
#define BCM283X_C3_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x7C)	/* Core3 FIQ Source */
/*
 *  ARM Coreメールボックス関係
 */
#define BCM283X_C0MB_0WS	(BCM283X_PER_BASE_40000000 + 0x80)	/* Core 0 Mailbox 0 write-set (WO) */
#define BCM283X_C0MB_1WS	(BCM283X_PER_BASE_40000000 + 0x84)	/* Core 0 Mailbox 1 write-set (WO) */
#define BCM283X_C0MB_2WS	(BCM283X_PER_BASE_40000000 + 0x88)	/* Core 0 Mailbox 2 write-set (WO) */
#define BCM283X_C0MB_3WS	(BCM283X_PER_BASE_40000000 + 0x8C)	/* Core 0 Mailbox 3 write-set (WO) */
#define BCM283X_C1MB_0WS	(BCM283X_PER_BASE_40000000 + 0x90)	/* Core 1 Mailbox 0 write-set (WO) */
#define BCM283X_C1MB_1WS	(BCM283X_PER_BASE_40000000 + 0x94)	/* Core 1 Mailbox 1 write-set (WO) */
#define BCM283X_C1MB_2WS	(BCM283X_PER_BASE_40000000 + 0x98)	/* Core 1 Mailbox 2 write-set (WO) */
#define BCM283X_C1MB_3WS	(BCM283X_PER_BASE_40000000 + 0x9C)	/* Core 1 Mailbox 3 write-set (WO) */
#define BCM283X_C2MB_0WS	(BCM283X_PER_BASE_40000000 + 0xA0)	/* Core 2 Mailbox 0 write-set (WO) */
#define BCM283X_C2MB_1WS	(BCM283X_PER_BASE_40000000 + 0xA4)	/* Core 2 Mailbox 1 write-set (WO) */
#define BCM283X_C2MB_2WS	(BCM283X_PER_BASE_40000000 + 0xA8)	/* Core 2 Mailbox 2 write-set (WO) */
#define BCM283X_C2MB_3WS	(BCM283X_PER_BASE_40000000 + 0xAC)	/* Core 2 Mailbox 3 write-set (WO) */
#define BCM283X_C3MB_0WS	(BCM283X_PER_BASE_40000000 + 0xB0)	/* Core 3 Mailbox 0 write-set (WO) */
#define BCM283X_C3MB_1WS	(BCM283X_PER_BASE_40000000 + 0xB4)	/* Core 3 Mailbox 1 write-set (WO) */
#define BCM283X_C3MB_2WS	(BCM283X_PER_BASE_40000000 + 0xB8)	/* Core 3 Mailbox 2 write-set (WO) */
#define BCM283X_C3MB_3WS	(BCM283X_PER_BASE_40000000 + 0xBC)	/* Core 3 Mailbox 3 write-set (WO) */
#define BCM283X_C0MB_0RC	(BCM283X_PER_BASE_40000000 + 0xC0)	/* Core 0 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C0MB_1RC	(BCM283X_PER_BASE_40000000 + 0xC4)	/* Core 0 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C0MB_2RC	(BCM283X_PER_BASE_40000000 + 0xC8)	/* Core 0 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C0MB_3RC	(BCM283X_PER_BASE_40000000 + 0xCC)	/* Core 0 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C1MB_0RC	(BCM283X_PER_BASE_40000000 + 0xD0)	/* Core 1 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C1MB_1RC	(BCM283X_PER_BASE_40000000 + 0xD4)	/* Core 1 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C1MB_2RC	(BCM283X_PER_BASE_40000000 + 0xD8)	/* Core 1 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C1MB_3RC	(BCM283X_PER_BASE_40000000 + 0xDC)	/* Core 1 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C2MB_0RC	(BCM283X_PER_BASE_40000000 + 0xE0)	/* Core 2 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C2MB_1RC	(BCM283X_PER_BASE_40000000 + 0xE4)	/* Core 2 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C2MB_2RC	(BCM283X_PER_BASE_40000000 + 0xE8)	/* Core 2 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C2MB_3RC	(BCM283X_PER_BASE_40000000 + 0xEC)	/* Core 2 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C3MB_0RC	(BCM283X_PER_BASE_40000000 + 0xF0)	/* Core 3 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C3MB_1RC	(BCM283X_PER_BASE_40000000 + 0xF4)	/* Core 3 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C3MB_2RC	(BCM283X_PER_BASE_40000000 + 0xF8)	/* Core 3 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C3MB_3RC	(BCM283X_PER_BASE_40000000 + 0xFC)	/* Core 3 Mailbox 3 read & write-high-to-clear */
#endif

#endif /* TOPPERS_BCM283X_H */
