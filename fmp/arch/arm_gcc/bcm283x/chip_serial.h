/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN  
 *  Copyright (C) 2007-2015 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_serial.h 1087 2016-10-17 00:00:00Z azo $
 */

/*
 * シリアルI/Oデバイス（SIO）ドライバ（BCM283X用）
 */

#ifndef TOPPERS_CHIP_SERIAL_H
#define TOPPERS_CHIP_SERIAL_H

#include <sil.h>
#include "bcm283x.h"

/*
 *  SIOの割込みハンドラのベクタ番号
 */
#define INHNO_SIO_PRC1   (0x10000|IRQ_UART)  /* 割込みハンドラ番号 */
#define INTNO_SIO_PRC1   (0x10000|IRQ_UART)  /* 割込み番号 */
#define INTPRI_SIO_PRC1       -2             /* 割込み優先度 */
#define INTATR_SIO_PRC1       0U             /* 割込み属性 */

/*
 *  シリアルI/Oポート数の定義
 */
#define TNUM_SIOP		1		/* サポートするシリアルI/Oポートの数 */

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_RDY_SND	1U		/* 送信可能コールバック */
#define SIO_RDY_RCV	2U		/* 受信通知コールバック */

#ifndef TOPPERS_MACRO_ONLY
/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block	SIOPCB;

/*
 * カーネル起動時のログ出力用の初期化（in usart.h）
 */
extern  void bcm283x_init_uart(uint_t siopid);

/*
 *  UARTからのポーリング出力
 */
Inline void
bcm283x_putc(char c)
{
#if BCM283X_USE_UART == 1
	uint32_t a;
	for(a=0;a<1000;a++) {
		if((sil_rew_mem((void *)BCM283X_AUX_MU_LSR_REG) & 0x20) != 0) break;
	}
//	while(1) {
//		if((sil_rew_mem((void *)BCM283X_AUX_MU_LSR_REG) & 0x20) != 0) break;
//	}
	sil_wrw_mem((void *)BCM283X_AUX_MU_IO_REG, c);
#else	/* BCM283X_USE_UART */
	sil_wrw_mem((void *)BCM283X_UART0_DR, c);		/* Write DR 0:7 */
#endif	/* BCM283X_USE_UART */
}

/*
 *  SIOドライバの初期化
 */
extern void sio_initialize(intptr_t exinf);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB *sio_opn_por(ID siopid, intptr_t exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void sio_cls_por(SIOPCB *p_siopcb);

/*
 *  SIOの割込みハンドラ
 */
extern void sio_handler(void);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern bool_t sio_snd_chr(SIOPCB *siopcb, char c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern int_t sio_rcv_chr(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void sio_irdy_snd(intptr_t exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void sio_irdy_rcv(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_SERIAL_H */
