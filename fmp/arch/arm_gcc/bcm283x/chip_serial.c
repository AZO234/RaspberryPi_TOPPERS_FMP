/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007 by GJ Business Division RICOH COMPANY,LTD. JAPAN  
 *  Copyright (C) 2005-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_serial.c 1087 2016-10-17 00:00:00Z azo $
 */

/*
 *  シリアルI/Oデバイス（SIO）ドライバ（BCM283X用）
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_kernel.h"  /* for TOPPERS_MASTER_PRCID */
#include <target_syssvc.h>
#include "chip_serial.h"  /* for prototypes */

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block 
{
	uint_t intno;
}
SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block 
{
	const SIOPINIB  *p_siopinib;  /* シリアルI/Oポート初期化ブロック */
	intptr_t        exinf;        /* 拡張情報 */
	uint8_t usart_rev_buf;        /* 受信用バッファ */
	bool_t    openflag;           /* オープン済みフラグ */
	bool_t    sendflag;           /* 送信割込みイネーブルフラグ */
	bool_t    getready;           /* 文字を受信した状態 */
	bool_t    putready;           /* 文字を送信できる状態 */
};

/*
 * シリアルI/Oポート初期化ブロック
 */
static const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{
		(uint_t)(INTNO_SIO_PRC1),
	},
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SEC_SERIAL1(static SIOPCB, siopcb1);

static SIOPCB* const p_siopcb_table[TNUM_SIOP] = {
	&siopcb1
};

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid) - 1))
#define get_siopcb(siopid)  (p_siopcb_table[INDEX_SIOP(siopid)])

/*
 * 文字を受信したか?
 */ 
Inline bool_t
uart_getready(SIOPCB *p_siopcb)
{
#if BCM283X_USE_UART == 1
#ifdef BCM283X_UART_ENABLE_FIFO
	return((sil_rew_mem(BCM283X_AUX_MU_LSR_REG) & 0x01) != 0);
#else	/* BCM283X_UART_ENABLE_FIFO */
	return(true);
#endif	/* BCM283X_UART_ENABLE_FIFO */
#else	/* BCM283X_USE_UART */
#ifdef BCM283X_UART_ENABLE_FIFO
	return((sil_rew_mem(BCM283X_UART0_FR) & (1 << 4)) == 0);	/* Receive FIFO is not empty is true */
#else	/* BCM283X_UART_ENABLE_FIFO */
	return(true);
#endif	/* BCM283X_UART_ENABLE_FIFO */
#endif	/* BCM283X_USE_UART */
}

/*
 * 文字を送信できるか?
 */
Inline bool_t
uart_putready(SIOPCB *p_siopcb)
{
#if BCM283X_USE_UART == 1
	if (p_siopcb->openflag) {
		return((sil_rew_mem(BCM283X_AUX_MU_LSR_REG) & 0x20) != 0);
	} else {
		return(true);
	}
#else	/* BCM283X_USE_UART */
	return((sil_rew_mem(BCM283X_UART0_FR) & (1 << 5)) == 0);	/* Transmit FIFO is not full is true */
#endif	/* BCM283X_USE_UART */
}

/*
 *  受信した文字の取り出し
 */
Inline uint8_t
uart_getchar(SIOPCB *p_siopcb)
{
#if BCM283X_USE_UART == 1
	return((uint8_t)(sil_rew_mem(BCM283X_AUX_MU_IO_REG) & 0xFF));
#else	/* BCM283X_USE_UART */
	return((uint8_t)(sil_rew_mem(BCM283X_UART0_DR)) & 0xFF);	/* Read DR 0:7 */
#endif	/* BCM283X_USE_UART */
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *p_siopcb, uint8_t c)
{
#if BCM283X_USE_UART == 1
	sil_wrw_mem(BCM283X_AUX_MU_IO_REG, c);
#else	/* BCM283X_USE_UART */
	sil_wrw_mem(BCM283X_UART0_DR, c);		/* Write DR 0:7 */
#endif	/* BCM283X_USE_UART */
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *p_siopcb)
{
	uint32_t	imsc;

#if BCM283X_USE_UART == 1
	imsc = sil_rew_mem(BCM283X_AUX_MU_IER_REG);
//	imsc |= 0x2;
	sil_wrw_mem(BCM283X_AUX_MU_IER_REG, imsc);
#else	/* BCM283X_USE_UART */
	imsc = sil_rew_mem(BCM283X_UART0_IMSC);
//	imsc |= 0x20;
	sil_wrw_mem(BCM283X_UART0_IMSC, imsc);
#endif	/* BCM283X_USE_UART */
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *p_siopcb)
{
	uint32_t	imsc;

#if BCM283X_USE_UART == 1
	imsc = sil_rew_mem(BCM283X_AUX_MU_IER_REG);
	imsc &= ~(0x2);
	sil_wrw_mem(BCM283X_AUX_MU_IER_REG, imsc);
#else	/* BCM283X_USE_UART */
	imsc = sil_rew_mem(BCM283X_UART0_IMSC);
	imsc &= ~(0x20);
	sil_wrw_mem(BCM283X_UART0_IMSC, imsc);
#endif	/* BCM283X_USE_UART */
}


/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *p_siopcb)
{
	uint32_t	imsc;

#if BCM283X_USE_UART == 1
	imsc = sil_rew_mem(BCM283X_AUX_MU_IER_REG);
	imsc |= 0x5;
	sil_wrw_mem(BCM283X_AUX_MU_IER_REG, imsc);
#else	/* BCM283X_USE_UART */
	imsc = sil_rew_mem(BCM283X_UART0_IMSC);
	imsc |= 0x10;
	sil_wrw_mem(BCM283X_UART0_IMSC, imsc);
#endif	/* BCM283X_USE_UART */
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *p_siopcb)
{
	uint32_t	imsc;

#if BCM283X_USE_UART == 1
	imsc = sil_rew_mem(BCM283X_AUX_MU_IER_REG);
	imsc &= ~(0x5);
	sil_wrw_mem(BCM283X_AUX_MU_IER_REG, imsc);
#else	/* BCM283X_USE_UART */
	imsc = sil_rew_mem(BCM283X_UART0_IMSC);
	imsc &= ~(0x10);
	sil_wrw_mem(BCM283X_UART0_IMSC, imsc);
#endif	/* BCM283X_USE_UART */
}


/*
 *  SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	uint_t  i = (uint_t)exinf;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	(p_siopcb_table[i])->p_siopinib = &(siopinib_table[i]);
	(p_siopcb_table[i])->openflag = false;
	(p_siopcb_table[i])->sendflag = false;
}

/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
bcm283x_uart_opn_por(SIOPCB *p_siopcb, intptr_t exinf)
{   
	uint_t ATTR_baudRate = 115200;
#if BCM283X_USE_UART == 1
	uint_t baud;
#else	/* BCM283X_USE_UART */
	uint_t ibrd, fbrd;
#endif	/* BCM283X_USE_UART */
	p_siopcb->exinf = exinf;
	p_siopcb->getready = p_siopcb->putready = false;

	if (!(p_siopcb->openflag)) {
#if BCM283X_USE_UART == 1
		baud = (250000000 / (8 * ATTR_baudRate)) - 1;

		sil_wrw_mem(BCM283X_AUX_ENABLES, 1);		/* Enable UART1 */
		sil_wrw_mem(BCM283X_AUX_MU_IER_REG, 0);		/* Disable interrupt */
		sil_wrw_mem(BCM283X_AUX_MU_CNTL_REG, 0);	/* Disable Transmitter and Receiver */
		sil_wrw_mem(BCM283X_AUX_MU_LCR_REG, 3);		/* Works in 8-bit mode */
		sil_wrw_mem(BCM283X_AUX_MU_MCR_REG, 0);		/* Disable RTS */
#ifdef BCM283X_UART_ENABLE_FIFO
		sil_wrw_mem(BCM283X_AUX_MU_IIR_REG, 0xC6);	/* Enable FIFO, Clear FIFO */
#else	/* BCM283X_UART_ENABLE_FIFO */
		sil_wrw_mem(BCM283X_AUX_MU_IIR_REG, 0)	;	/* Disable FIFO */
#endif	/* BCM283X_UART_ENABLE_FIFO */
		sil_wrw_mem(BCM283X_AUX_MU_BAUD_REG, baud);	/* 115200 = system clock 250MHz / (8 * (baud + 1)), baud = 270 */
		sil_wrw_mem(BCM283X_AUX_MU_CNTL_REG, 3);	/* Enable Transmitter and Receiver */
#else	/* BCM283X_USE_UART */
		ibrd = 3000000 / (ATTR_baudRate * 16);
		fbrd = (3000000 - (ibrd * ATTR_baudRate * 16)) * 4 / ATTR_baudRate;

		sil_wrw_mem(BCM283X_UART0_CR, 0x0);			/* Disable UART0 */
		sil_wrw_mem(BCM283X_UART0_ICR, 0x7FF);			/* Interrupt clear */
		sil_wrw_mem(BCM283X_UART0_IBRD, ibrd);			/* 3000000(300MHz) / (115200 * 16) = 625/384 = 1 + 241/384 */
		sil_wrw_mem(BCM283X_UART0_FBRD, fbrd);			/* 241/384 * 64 = about 40 */
#ifdef BCM283X_UART_ENABLE_FIFO
		sil_wrw_mem(BCM283X_UART0_LCRH, 0x70);			/* FIFO Enable, 8 n 1 */
		sil_wrw_mem(BCM283X_UART0_IFLS, 0x0);			/* Receive interrupt FIFO 1/8, Transmit interrupt FIFO 1/8 */
		sil_wrw_mem(BCM283X_UART0_CR, 0x301);			/* Transmit Receive Enable UART0 */
		while((sil_rew_mem(BCM283X_UART0_FR) & 0x10) == 0) {	/* FIFO clear */
			sil_rew_mem(BCM283X_UART0_DR);
		}
#else	/* BCM283X_UART_ENABLE_FIFO */
		sil_wrw_mem(BCM283X_UART0_LCRH, 0x60);			/* FIFO Disable, 8 n 1 */
		sil_wrw_mem(BCM283X_UART0_CR, 0x301);			/* Transmit Receive Enable UART0 */
#endif	/* BCM283X_UART_ENABLE_FIFO */
#endif	/* BCM283X_USE_UART */

		p_siopcb->openflag = true;
	}

	return(p_siopcb);
}


/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB  *p_siopcb = get_siopcb(siopid);
	bool_t    opnflg;
	ER      ercd;
    
	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = p_siopcb->openflag;

	/*
	 *  デバイス依存のオープン処理．
	 */
	bcm283x_uart_opn_por(p_siopcb, exinf);

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg && p_siopcb->openflag) {
		ercd = ena_int(p_siopcb->p_siopinib->intno);
		assert(ercd == E_OK);
	}
	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	/*
	 *  デバイス依存のクローズ処理．
	 */
#if BCM283X_USE_UART == 1
	sil_wrw_mem(BCM283X_AUX_ENABLES, 0);		/* Disable UART1 */
	sil_wrw_mem(BCM283X_AUX_MU_IER_REG, 0);		/* Disable interrupt */
	sil_wrw_mem(BCM283X_AUX_MU_CNTL_REG, 0);	/* Disable Transmitter and Receiver */
#else	/* BCM283X_USE_UART */
	sil_wrw_mem(BCM283X_UART0_CR, 0x0);		/* Disable UART0 */
#endif	/* BCM283X_USE_UART */

	p_siopcb->openflag = false;
    
	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	dis_int(p_siopcb->p_siopinib->intno);
}


/*
 *  SIOの割込みハンドラ
 */
void
sio_handler(void)
{
	SIOPCB *p_siopcb = get_siopcb(1);
	if (uart_getready(p_siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sio_irdy_rcv(p_siopcb->exinf);
	}
	if (uart_putready(p_siopcb)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
//		sio_irdy_snd(p_siopcb->exinf);
	}
}


/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char c)
{    
	if (uart_putready(siopcb)){
		uart_putchar(siopcb, c);
		return(true);
	}
	return(false);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
	if (uart_getready(siopcb)) {
		return((int_t)(uint8_t) uart_getchar(siopcb));
	}
	return(-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_enable_send(siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_enable_rcv(siopcb);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_disable_send(siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_disable_rcv(siopcb);
		break;
	}
}

