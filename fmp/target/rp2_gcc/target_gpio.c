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
 *  @(#) $Id: target_gpio.c 1838 2016-10-17 00:00:00Z azo $
 */

#include <sil.h>
#include "bcm283x.h"
#include "target_gpio.h"

/*
 *  GPIO設定．
 */
void target_gpio_initialize(void) {
	volatile uint32_t r,a;

#if BCM283X_USE_UART == 1
	/* AUX mini UART : GPIO14 ALT5 PullUD disable / GPIO15 ALT5 PullUD disable */

	r = sil_rew_mem(BCM283X_GPIO_GPFSEL1);
	r &= ~(7<<12);	/* GPIO14 ALT5 */
	r |= 2<<12;
	r &= ~(7<<15);	/* GPIO15 ALT5 */
	r |= 2<<15;
	sil_wrw_mem(BCM283X_GPIO_GPFSEL1, r);

	sil_wrw_mem(BCM283X_GPIO_GPPUD, 0);
	for(r = 0; r < 150; r++) a = sil_rew_mem(BCM283X_GPIO_GPFSEL1);	/* dummy */
	sil_wrw_mem(BCM283X_GPIO_GPPUDCLK0, (1<<14)|(1<<15));	/* GPIO14 PullUD disable / GPIO15 PullUD disable */
	for(r = 0; r < 150; r++) a = sil_rew_mem(BCM283X_GPIO_GPFSEL1);	/* dummy */
	sil_wrw_mem(BCM283X_GPIO_GPPUDCLK0, 0);
#else	/* BCM283X_USE_UART */
	/* PL011 UART : GPIO14 ALT0 PullUD disable / GPIO15 ALT0 PullUD disable */

	r = sil_rew_mem(BCM283X_GPIO_GPFSEL1);
	r &= ~(7<<12);	/* GPIO14 ALT0 */
	r |= 4<<12;
	r &= ~(7<<15);	/* GPIO15 ALT0 */
	r |= 4<<15;
	sil_wrw_mem(BCM283X_GPIO_GPFSEL1, r);

	sil_wrw_mem(BCM283X_GPIO_GPPUD, 0);
	for(r = 0; r < 150; r++) a = sil_rew_mem(BCM283X_GPIO_GPFSEL1);	/* dummy */
	sil_wrw_mem(BCM283X_GPIO_GPPUDCLK0, (1<<14)|(1<<15));	/* GPIO14 PullUD disable / GPIO15 PullUD disable */
	for(r = 0; r < 150; r++) a = sil_rew_mem(BCM283X_GPIO_GPFSEL1);	/* dummy */
	sil_wrw_mem(BCM283X_GPIO_GPPUDCLK0, 0);
#endif	/* BCM283X_USE_UART */
}

