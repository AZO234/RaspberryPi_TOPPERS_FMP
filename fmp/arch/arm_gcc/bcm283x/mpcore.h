/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
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
 *  @(#) $Id: mpcore.h 1180 2016-07-08 05:57:35Z ertl-honda $
 */

/*
 *  MPCORE のハードウェア資源の定義
 */

#ifndef TOPPERS_MPCORE_H
#define TOPPERS_MPCORE_H

#include <sil.h>

/*
 *  MPCORE のハードウェア資源のツール依存部の定義
 */
#include <mpcore_tool.h>

/*
 *  コプロのビット定義
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_V_BIT    (1 << 13)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_Z_BIT    (1 << 11)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#if __TARGET_ARCH_ARM == 6
#define CP15_AUXILIARY_SA_BIT (1 << 5)
#define CP15_CPUID_BIT        (0x0f)
#else /* __TARGET_ARCH_ARM == 7 */
#define CP15_AUXILIARY_SA_BIT (1 << 6)
#define CP15_AUXILIARY_EX_BIT (1 << 7)
#define CP15_AUXILIARY_FW_BIT (1 << 0)
#define CP15_CPUID_BIT        (0x03)
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TTB0_RGN_S       (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

#if __TARGET_ARCH_ARM == 7
#define CP15_TTB0_IRGN_WBWA   ((1 << 6)|(0))

#define CP15_NSACR_NS_SMP (1 << 18)

#define CCSIDR_LINE_SIZE_MASK        0x00000007
#define CCSIDR_LINE_SIZE_OFFSET      0
#define CCSIDR_ASSOCIATIVITY_MASK    0x00001FF8
#define CCSIDR_ASSOCIATIVITY_OFFSET  3
#define CCSIDR_NUM_SETS_MASK         0x0FFFE000
#define CCSIDR_NUM_SETS_OFFSET      13

#define CLIDR_CTYPE_NO_CACHE          0
#define CLIDR_CTYPE_INSTRUCTION_ONLY  1
#define CLIDR_CTYPE_DATA_ONLY         2
#define CLIDR_CTYPE_INSTRUCTION_DATA  3
#define CLIDR_CTYPE_UNIFIED           4

#define CLIDR_MAX_CTYPE         7
#define CLIDR_CTYPE_MASK      0x7
#define CLIDR_CTYPE_BITWIDTH    3

#define CSSELR_IND_DATA_UNIFIED       0
#define CSSELR_IND_INSTRUCTION        1

#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  内蔵タイマ関連の定義
 */
#define MPCORE_TM_LR     (MPCORE_PMR_BASE + 0x0600)  /* ロードレジスタ           */
#define MPCORE_TM_COUNT  (MPCORE_PMR_BASE + 0x0604)  /* カウンターレジスタ       */
#define MPCORE_TM_CNT    (MPCORE_PMR_BASE + 0x0608)  /* コントロールレジスタ     */
#define MPCORE_TM_ISR    (MPCORE_PMR_BASE + 0x060C)  /* 割込みステータスレジスタ */

#define MPCORE_TM_CNT_EN        0x1
#define MPCORE_TM_CNT_AR        0x2
#define MPCORE_TM_CNT_IEN       0x4
#define MPCORE_TM_CNT_PS_OFFSET   8

#define MPCORE_TM_ISR_SCBIT    0x01


#ifndef TOPPERS_MACRO_ONLY

/*
 *  コプロを用いたルーチン
 */

/*
 *  High exception vector を使うかの設定
 */
Inline void
set_high_vector(bool_t enable)
{
	uint32_t control;

	CP15_CONTROL_READ(control);

	if (enable) {
		control |= CP15_CONTROL_V_BIT;
	}
	else {
		control &= ~CP15_CONTROL_V_BIT;
	}

	CP15_CONTROL_WRITE(control);
}

/*
 *  プロセッサINDEX（0オリジン）の取得
 */
Inline uint32_t
x_prc_index(void)
{
	uint32_t index;

	CP15_CPUID_READ(index);
	return((index & 0x0fU));
}

/*
 *  Data Synchronization Barrier
 *  ・先に発行された read と write の終了を待つ
 *  ・キャッシュ，ブランチプリディクション，TLBの操作の終了を待つ
 *  ・この命令の後に書かれた命令は実行されない
 */
Inline void
mpcore_data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  ・プログラムの記述に従って，先に書かれた命令でのメモリアクセスが
 *     終了するまで待つ．
 *
 */
Inline void
mpcore_data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}

/*
 *  TLBの無効化
 */
Inline void
mpcore_invalidate_unfied_tlb(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  ARMv7用のキャッシュ操作ルーチン
 */
extern void armv7_invalidate_dcache_all();
extern void armv7_clean_invalidate_dcache_all();
extern void armv7_clean_dcache_all();

/*
 *  Dキャッシュの無効化
 */
Inline void
mpcore_dcache_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	armv7_invalidate_dcache_all();
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  Dキャッシュのクリーンと無効化
 */
Inline void
mpcore_dcache_clean_and_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_CLEAN_AND_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	armv7_clean_invalidate_dcache_all();
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  Dキャッシュのクリーン
 */
Inline void
mpcore_dcache_clean(void)
{
#if __TARGET_ARCH_ARM == 6
#error
#elif __TARGET_ARCH_ARM == 7
	armv7_clean_dcache_all();
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  Iキャッシュの無効化
 */
Inline void
mpcore_icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
	CP15_BRANCHP_INVALIDATE();
	CP15_DATA_SYNC_BARRIER();
	CP15_PBUFFER_FLUSH();
}

/*
 *  プリフェッチバッファをクリア
 */
Inline void
mpcore_pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}

/*
 *  ブランチプリディクターの全無効化
 */
Inline void
bpi_invalidate_all(void)
{
	CP15_BPI_INVALIDATEALL(0);
	CP15_DATA_SYNC_BARRIER();
	CP15_PBUFFER_FLUSH();
}

/*
 *  ブランチプリディクターを有効に
 */
Inline void
btac_enable(void)
{
	uint32_t tmp;

	CP15_CONTROL_READ(tmp);
	tmp |= CP15_CONTROL_Z_BIT;
	CP15_CONTROL_WRITE(tmp);
}

/*
 *  SMPモードの有効化
 */
extern void mpcore_smp_mode_enable(void);

/*
 *  MMU関連
 */

/*
 * 変換テーブルへの設定内容
 * va   : 仮想アドレス
 * pa   : 物理アドレス
 * size : サイズ
 * s    : 共有指定
 * tex  : C Bとの組み合わせで変化
 * ap   : アクセス権
 * c    : キャッシュ
 * b    : バッファ
 */
typedef struct{
	uintptr_t   va;
	uintptr_t   pa;
	uint32_t    size;
	uint8_t     ns;
	uint8_t     s;
	uint8_t     tex;
	uint8_t     ap;
	uint8_t     c;
	uint8_t     b;
}MEMORY_ATTRIBUTE;

/*
 *  MMUの初期化
 */
extern void mpcore_mmu_init(void);

/*
 *  MMUによるメモリのマッピング
 */
extern void mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute);

/*
 *  I/Dキャッシュを両方を有効に
 */
extern void mpcore_cache_enable(void);

/*
 *  I/Dキャッシュを両方を無効に
 */
extern void mpcore_cache_disable(void);

/*
 *  Iキャッシュを有効に
 */
extern void mpcore_icache_enable(void);

/*
 *  Iキャッシュを無効に
 */
extern void mpcore_icache_disable(void);

/*
 *  Dキャッシュを有効に
 */
extern void mpcore_dcache_enable(void);

/*
 *  Dキャッシュを無効に
 */
extern void mpcore_dcache_disable(void);


#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_MPCORE_H */
