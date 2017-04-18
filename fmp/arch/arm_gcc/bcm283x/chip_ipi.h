/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_ipi.h 955 2016-10-17 00:00:00Z azo $
 */

/*
 *  プロセッサ間割込みドライバ（BCM283X用）
 */

#ifndef TOPPERS_CHIP_IPI_H
#define TOPPERS_CHIP_IPI_H

#include "bcm283x.h"

/*
 *  プロセッサ間割込みの割込み番号
 */
#define INTNO_IPI_PRC0	IRQ_MB0
#define INTNO_IPI_PRC1	IRQ_MB1
#define INTNO_IPI_PRC2	IRQ_MB2
#define INTNO_IPI_PRC3	IRQ_MB3

/*
 *  プロセッサ間割込みの割込みハンドラ番号
 */
#define INHNO_IPI_PRC0	INTNO_IPI_PRC0
#define INHNO_IPI_PRC1	INTNO_IPI_PRC1
#define INHNO_IPI_PRC2	INTNO_IPI_PRC2
#define INHNO_IPI_PRC3	INTNO_IPI_PRC3

/*
 *  Numbers for software generated interrupts SGI
 */
#define IPINO_DIS  IRQ_MB0
#define IPINO_EXT  IRQ_MB0

/*
 *  Priority for inter-processor interrupts
 */
#define INTPRI_IPI -2

/*
 *  ディスパッチ要求用プロセッサ間割込みの割込み番号
 */
#define INTNO_IPI_DIS_PRC1   (0x00010000 |  IPINO_DIS     )
#define INTNO_IPI_DIS_PRC2   (0x00010000 | (IPINO_DIS + 1))
#define INTNO_IPI_DIS_PRC3   (0x00010000 | (IPINO_DIS + 2))
#define INTNO_IPI_DIS_PRC4   (0x00010000 | (IPINO_DIS + 3))

/*
 *  カーネル終了用のプロセッサ間割込みの割込み番号
 */
#define INTNO_IPI_EXT_PRC1   (0x00010000 |  IPINO_EXT     )
#define INTNO_IPI_EXT_PRC2   (0x00010000 | (IPINO_EXT + 1))
#define INTNO_IPI_EXT_PRC3   (0x00010000 | (IPINO_EXT + 2))
#define INTNO_IPI_EXT_PRC4   (0x00010000 | (IPINO_EXT + 3))

/*
 *  ディスパッチ要求用のプロセッサ間割込みの割込みハンドラ番号
 */
#define INHNO_IPI_DIS_PRC1   INTNO_IPI_DIS_PRC1
#define INHNO_IPI_DIS_PRC2   INTNO_IPI_DIS_PRC2
#define INHNO_IPI_DIS_PRC3   INTNO_IPI_DIS_PRC3
#define INHNO_IPI_DIS_PRC4   INTNO_IPI_DIS_PRC4

/*
 *  カーネル終了用のプロセッサ間割込みの割込みハンドラ番号
 */
#define INHNO_IPI_EXT_PRC1   INTNO_IPI_EXT_PRC1
#define INHNO_IPI_EXT_PRC2   INTNO_IPI_EXT_PRC2
#define INHNO_IPI_EXT_PRC3   INTNO_IPI_EXT_PRC3
#define INHNO_IPI_EXT_PRC4   INTNO_IPI_EXT_PRC4

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ディスパッチ用プロセッサ間割込みの発行
 */
Inline void
target_ipi_raise(uint_t prcid)
{
	CP15_DATA_SYNC_BARRIER();
	switch(prcid) {
	case 1:
		sil_wrw_mem((void *)BCM283X_C0MB_0WS, IPINO_DIS);
		break;
	case 2:
		sil_wrw_mem((void *)BCM283X_C1MB_1WS, IPINO_DIS);
		break;
	case 3:
		sil_wrw_mem((void *)BCM283X_C2MB_2WS, IPINO_DIS);
		break;
	case 4:
		sil_wrw_mem((void *)BCM283X_C3MB_3WS, IPINO_DIS);
		break;
	}
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  ディスパッチ用プロセッサ間割込みのクリア
 */
Inline void
target_ipi_clear(void)
{
	CP15_DATA_SYNC_BARRIER();
	switch(bcm283x_getmycoreindex() + 1) {
	case 1:
		sil_wrw_mem((void *)BCM283X_C0MB_0RC, 0xFFFFFFFFU);
		break;
	case 2:
		sil_wrw_mem((void *)BCM283X_C1MB_1RC, 0xFFFFFFFFU);
		break;
	case 3:
		sil_wrw_mem((void *)BCM283X_C2MB_2RC, 0xFFFFFFFFU);
		break;
	case 4:
		sil_wrw_mem((void *)BCM283X_C3MB_3RC, 0xFFFFFFFFU);
		break;
	}
	CP15_DATA_SYNC_BARRIER();
}

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  カーネル終了処理用のプロセッサ間割込みハンドラ
 */
extern void _kernel_ipi_ext_handler(void);
#endif /* USE_IPI_DIS_HANDER_BYPASS */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_CHIP_IPI_H */
