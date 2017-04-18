/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: chip_support.S 955 2016-10-19 00:00:00Z azo $
 */

/*
 *  ターゲット依存モジュール アセンブリ言語部（ARM用）
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "bcm283x.h"
#include "kernel_impl.h"
#include "target_asm.inc"
#include "target_ipi.h"
#include "offset.h"

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．
 */
    ATEXT
    AALIGN(2)
    AGLOBAL(hardware_init_hook)
ALABEL(hardware_init_hook)
#ifdef TOPPERS_ENA_PERF
    stmfd   sp!, {lr}
    bl      perf_boot_init
    ldmfd   sp!, {lr}
#endif /* TOPPERS_ENA_PERF */
    bx  lr

/*
 * 割込みハンドラ
 */
    ATEXT
    AALIGN(2)
    AGLOBAL(irq_handler)
ALABEL(irq_handler)
    /*
     * IRQモードで実行される
     */
    sub   lr, lr, #4 /* 戻り先アドレス  */

    /*
     * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
     * 保存．
     */
    srsdb #CPSR_SVC!

    /*
     *  割込み前のモード(スーパーバイザーモード)へ
     *  移行し，コンテキストを保存する
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)
    stmfd sp!, {r0-r5,ip,lr}
#if defined(SUPPORT_VFPv3_NEON)
    fmrx    r0, fpscr
    push    {r0}
    vpush   {d0-d7} /* スクラッチレジスタの保存 */
#endif /* SUPPORT_VFPv3_NEON */

    /*
     *  PCBの取得
     */
    my_pcb r0, r3

#if TTYPE_KLOCK != G_KLOCK
    /*
     *  プロセッサ・細粒度ロックの場合はロックを取得しているかチェックして
     *  取得していればロックを解放する．
     */
ALABEL(release_lock)
    ldr   r3, [r0, #PCB_p_firstlock]  /* p_firstlock を取得 */
    cmp   r3, #0                      /* NULLなら終了       */
    beq   release_lock_end
    mov   r2, #0
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev
    str   r2, [r0, #PCB_p_firstlock]  /* p_firstlock を NULL へ */
    ldr   r3, [r0, #PCB_p_secondlock] /* p_secondlock を取得 */
    cmp   r3, #0                      /* NULLなら終了       */
    beq   release_lock_end
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev
    str   r2, [r0, #PCB_p_firstlock]  /* p_secondlock を NULL へ */
ALABEL(release_lock_end)
#endif /* TTYPE_KLOCK != G_KLOCK */

    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

	/*
	 *  割り込み要因の判定．
	 */
	/* MailBoxを優先でチェック */
	my_core_index r2	/* コア番号の取得 0〜3 */
	ldr   r1, =BCM283X_C0_IRQSOURCE
	ldr   r1, [r1, r2, lsl #2]
	tst   r1, #0xF0
	bne   bsc283x_irc_begin_int_mb	/* MailBox割り込みならジャンプ */
	cmp   r2, #0
	bne   undef_handler	/* コア0以外がここに来たらエラー */
	/* コア0の処理 */
	ldr   r1, =AIC_BASIC
	ldr   r5, [r1]
	tst   r5, #0x200
	movne r4, #64
	ldrne r1, =AIC_PEND2
	ldrne r5, [r1]
	bne   bsc283x_irc_begin_int_1
	tst   r5, #0x100
	movne r4, #32
	ldrne r1, =AIC_PEND1
	ldrne r5, [r1]
	bne   bsc283x_irc_begin_int_1
	bic   r5, r5, #0x300
	mov   r4, #0
bsc283x_irc_begin_int_1:
	mov   r3, #1
bsc283x_irc_begin_int_2:
	ands  r1, r5, r3
	movne r3, #0xF			/* 要因番号が確定 */
	bne   bsc283x_irc_begin_int_9
	add   r4, r4, #1
	lsls  r3, r3, #1
	bne   bsc283x_irc_begin_int_2
bsc283x_irc_begin_int_9:
	cmp   r3, #0xF			/* 要因番号が未確定かチェック */
	beq   bsc283x_irc_begin_int_8
	mov   r4, #96
	ldr   r1, =BCM283X_C0_IRQSOURCE
	ldr   r5, [r1]
	bic   r5, r5, #0xF0	/* MailBox以外 */
	ldr   r1, =bcm283x_enable_intflag	/* 許可されている要因だけチェック */
	ldr   r3, [r1]
		orr   r3, r3, #0x800
	and   r5, r5, r3
	mov   r3, #1
bsc283x_irc_begin_int_3:
	ands  r1, r5, r3
	movne r3, #0xF			/* 要因番号が確定 */
	bne   bsc283x_irc_begin_int_8
	add   r4, r4, #1
	lsls  r3, r3, #1
	bne   bsc283x_irc_begin_int_3
bsc283x_irc_begin_int_8:
	cmp   r3, #0xF			/* 要因番号が未確定かチェック */
	bne   undef_handler
	mov   r1, r4
	b     bsc283x_irc_begin_int_6
	/* MailBoxの処理 */
bsc283x_irc_begin_int_mb:
	mov   r3, #0x10
	lsl   r3, r3, r2
	ands  r1, r1, r3
	beq   undef_handler
	mov   r3, #0xE			/* MailBox割り込みが確定 */
	add   r1, r2, #100
bsc283x_irc_begin_int_6:
	/* r1<-割り込み要因番号 */

   /*
    *  spurious割込み(INT_ID=1023(0x03ff))の場合，割込み処理を終了
    */
   ldr   r2, =(0x03ff)
   cmp   r2, r1
   beq   ret_int_2

#ifdef TOPPERS_SAFEG_SECURE
   /*
    *  TrustZoneを用いるときは，INT_ID=1022(0x03fe)も
    *  spurious割込みとなる．
    */
   ldr   r2, =(0x03fe)
   cmp   r2, r1
   beq   ret_int_2
#endif /* TOPPERS_SAFEG_SECURE */

	/* MailBox割り込みは、NMIなのでクリアする */
	cmp   r3, #0xE
	bne   bsc283x_int_mb_continue
	ldr   r2, =0xFFFFFFFF
	cmp   r1, #IRQ_MB0
	ldreq r3, =BCM283X_C0MB_0RC
	cmp   r1, #IRQ_MB1
	ldreq r3, =BCM283X_C1MB_1RC
	cmp   r1, #IRQ_MB2
	ldreq r3, =BCM283X_C2MB_2RC
	cmp   r1, #IRQ_MB3
	ldreq r3, =BCM283X_C3MB_3RC
	str   r2, [r3]			/* EOIレジスタへの書き込み */
bsc283x_int_mb_continue:

	/* LTIMER割り込みは、NMIなのでクリアする */
	cmp   r1, #IRQ_LTIMER
	bne   bsc283x_int_ltimer_continue
	mov   r2, #0x80000000
	ldr   r4, =BCM283X_LTIMER_WRITE
	str   r2, [r4]
bsc283x_int_ltimer_continue:

    /*
     *  多重割込みか判定
     */
    ldr   r3, [r0, #PCB_excpt_nest_count]  /* 例外・割込みネスト回数を取得 */
    add   r2, r3, #1                       /* 例外・割込みネスト回数を更新 */
    str   r2, [r0, #PCB_excpt_nest_count]
    cmp   r3, #0

    /*
     *  多重割込みの場合はスタックを8バイト境界に調整
     */
    andne r2, sp, #4
    subne sp, sp, r2

    /*
     * 一段目の例外（割込み）ならスタックを変更する
     */
    ldreq sp, [r0, #PCB_istkpt]

    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 -4 */

    /*
     *  割込みハンドラのアドレスを取得
     */
    ldr   r3, [r0, #PCB_p_inh_tbl]   /* 割込みハンドラテーブルの読み込み */
    ldr   r0, [r3, r1, lsl #2]       /* r0<-割込みハンドラ */

	my_core_index r2	/* コア番号の取得 0〜3 */
	cmp   r2, #0
	bne   bsc283x_ipi_1		/* コア0以外はAICを操作しない */

	/*
	 *  割込み要因の割込み優先度を求め(モデル上の)割込み優先度マスクをセット
	 *  する．またその際，ハンドラ実行前の(モデル上の)割込み優先度マスクを
	 *  保存する．
	 */
	ldr   r2, =_kernel_inh_ipm_table	/* 割込み優先度を取得 */
	ldr   r4, [r2, r1, lsl #2]		/* r4<-割込み優先度   */
	ldr   r2, =ipm			/* 割込み発生前の割込み優先度マスクをスタックに保存 -8 */
	ldr   r3, [r2]              
	stmfd sp!,{r3}
	str   r4, [r2]			/* (モデル上の)割込み優先度マスクをセット */

	/*
	 * (モデル上の)割込み優先度マスクの設定
	 * 
	 * 受け付けた割込みの割込み優先度に設定された割込み要求禁止フラグの
	 * テーブルの値と(モデル上の)各割込みの割込み要求禁止フラグの状態を
	 * 保持した変数の値とのORをIRCの割込み要求禁止フラグにセットする．
	 */
	mvn   r4, r4			/* 要素番号となるように反転+1 */
	add   r4, r4, #1
	lsl   r4, r4, #4		/* x16 */
	ldr   r5, =_kernel_ipm_mask_table		/* (モデル上)の割込み優先度を実現するための */
	add   r5, r5, r4
	ldr   r2, =idf			/* 各割込みの割込み要求禁止フラグの状態を取得 */
	ldr   r3, [r5]  		/* 割込み要求禁止フラグ(BASIC)を取得 */
	ldr   r4, [r2]
	orr   r3, r3, r4		/* マスク指定されていない割込みの許可(BASIC) */
	ldr   r4, =AIC_DISABLE_BASIC
	str   r3, [r4]
	ldr   r3, [r5, #4]		/* 割込み要求禁止フラグ(IRQ1)を取得 */
	ldr   r4, [r2, #4]
	orr   r3, r3, r4		/* マスク指定されていない割込みの許可(IRQ1) */
	ldr   r4, =AIC_DISABLE_IRQ1
	str   r3, [r4]
	ldr   r3, [r5, #8]		/* 割込み要求禁止フラグ(IRQ2)を取得 */
	ldr   r4, [r2, #8]
	orr   r3, r3, r4		/* マスク指定されていない割込みの許可(IRQ2) */
	ldr   r4, =AIC_DISABLE_IRQ2
	str   r3, [r4]
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	ldr   r3, [r5, #12]		/* 割込み要求禁止フラグ(Core0)を取得 */
	ldr   r4, [r2, #12]
	orr   r3, r3, r4		/* マスク指定されていない割込みの許可(Core0) */
	ldr   r4, =bcm283x_enable_intflag
	ldr   r5, [r4]
	bic   r3, r3, r5
	str   r3, [r4]
#endif

bsc283x_ipi_1:		/* コア0以外はAICを操作しない */

    stmfd sp!,{r1}       /* inhno を保存  -16 */

#ifdef TOPPERS_SAFEG_LOAD
    stmfd sp!,{r0,r1}  /* r1はスタック境界調整のためのダミー */
    bl safeg_load_int
    ldmfd sp!,{r0,r1}  /* r1はスタック境界調整のためのダミー */
#endif /* TOPPERS_SAFEG_LOAD */

    /*
     * 割り込み許可
     */
#ifndef ENABLE_NMFI
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_ALWAYS_SET)
#endif  /* ENABLE_NMFI */

#ifdef LOG_INH_ENTER
    stmfd sp!,{r0,r1}    /* r1はスタック境界調整のためのダミー */
    mov   r0, r1         /* inhno をパラメータに    */
    bl    log_inh_enter  /* log_inh_enterを呼び出す */
    ldmfd sp!,{r0,r1}    /* r1はスタック境界調整のためのダミー */
#endif /* LOG_INH_ENTER */

    /*
     * 割込みハンドラの呼び出し
     */
    mov   lr,  pc
    bx    r0

ALABEL(target_ret_int)

    ldmfd sp!,{r0}       /* inhno を復帰  +4 */

#ifdef LOG_INH_LEAVE
    bl    log_inh_leave  /* log_inh_leaveを呼び出す */
#endif /* LOG_INH_LEAVE */

    /*
     * カーネル管理の割込みを禁止する
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)

	my_core_index r2	/* コア番号の取得 0〜3 */
	cmp   r2, #0
	bne   bsc283x_ipi_2		/* コア0以外はAICを操作しない */

	/*
	 * 割込み優先度マスクを元に戻す
	 */
	/*
	 * 各割込みの割込み要求禁止フラグの状態と(モデル上)の割込み優先度のを実現する
	 * ための割込み要求禁止フラグの状態のORをとり，それの否定を求めることにより，
	 * IRCの割込み許可レジスタへの設定値を生成し設定する．
	 */
	ldmfd sp!, {r1}             /* 元の割込み優先度マスクを取得 +12 */
	ldr   r0, =ipm              /* 割込み優先度マスクを復帰     */
	str   r1, [r0]
	mvn   r1, r1
	add   r1, r1, #1
	mov   r0, #0x10
	mul   r1, r1, r0
	ldr   r0, =_kernel_ipm_mask_table     /* (モデル上)の割込み優先度のを実現するための */
	add   r0, r0, r1
	ldr   r1, =idf              /* 各割込みの割込み要求禁止フラグの状態を取得 */
	ldr   r2, [r0]          /* 割込み要求禁止フラグ(BASIC)を取得 */
	ldr   r3, [r1]
	orr   r2, r2, r3            /* マスク指定されていない割込みの許可 */
	mvn   r2, r2
	ldr   r3, =AIC_ENABLE_BASIC
	str   r2, [r3]
	ldr   r2, [r0, #4]          /* 割込み要求禁止フラグ(IRQ1)を取得 */
	ldr   r3, [r1, #4]
	orr   r2, r2, r3            /* マスク指定されていない割込みの許可 */
	mvn   r2, r2
	ldr   r3, =AIC_ENABLE_IRQ1
	str   r2, [r3]
	ldr   r2, [r0, #8]          /* 割込み要求禁止フラグ(IRQ2)を取得 */
	ldr   r3, [r1, #8]
	orr   r2, r2, r3            /* マスク指定されていない割込みの許可 */
	mvn   r2, r2
	ldr   r3, =AIC_ENABLE_IRQ2
	str   r2, [r3]
#if defined(TOPPERS_USE_BCM2836) || defined(TOPPERS_USE_BCM2837)
	ldr   r2, [r0, #12]         /* 割込み要求禁止フラグ(Core0)を取得 */
	ldr   r3, [r1, #12]
	orr   r2, r2, r3            /* マスク指定されていない割込みの許可 */
	ldr   r3, =bcm283x_enable_intflag
	ldr   r4, [r3]
	orr   r2, r2, r4
	str   r2, [r3]
#endif

bsc283x_ipi_2:		/* コア0以外はAICを操作しない */

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!,{r2}              /* 元のスタックポインタを取得 */
    mov   sp, r2

    /*
     * 後の処理はARM依存部で実行
     */
    b     ret_int


/*
 * ターゲット依存の例外入口処理
 * ARMv6/7はARMv4と呼び出し時条件が異なるため注意すること．
 * スーパーバイザーモードで呼び出される．
 */
    ATEXT
    AGLOBAL(target_exc_handler)
ALABEL(target_exc_handler)
    mov   lr, sp         /* この時点のスタックを復帰のため取得 */

//    ldr   r0, =(DIC_CPUIF_PMR) /* 例外発生前の割込み優先度マスクを取得 */
    mov   r0, #0
//    ldr   r3, [r0]
//    asr   r3, r3, #DIC_CPUIF_PMR_OFFSET /* 外部表現に変更 */
//    sub   r3, r3, #(IPM_BIT_MASK)
    mov   r3, #0
    stmfd sp!, {r3}  /* 例外発生前の割込み優先度マスクをスタックに保存 */

    /*
     *  コンテキスト判定のため，excpt_nest_count をスタックに保存．
     *  スタックに保存せず，現在のexcpt_nest_countを-1すると取得できるが，
     *  スタックに積んでおいた方がデバッグ等が行いやすいので，スタックに
     *  保存する．
     */
    my_pcb r0, r3
    ldr   r3, [r0, #PCB_excpt_nest_count]
    stmfd sp!, {r3}

    mov   r3,  sp         /* 例外フレーム番地を保存 */

    /*
     * カーネル管理外のCPU例外か判定する
     *
     * カーネル管理外のCPU例外は，カーネル実行中，全割込みロック状態，
     * CPUロック状態，カーネル管理外の割込みハンドラ実行中に発生した
     * CPU例外である．ARMの場合は，SPSRのI/Fフラグのいずれかが'1'なら，
     * 該当する．
     */
    ldr   r1, [sp, #EXC_FRAME_cpsr]     /* 例外フレームからspsr(例外発生時のcpsr)を取得 */
    tst   r1, #(CPSR_FIQ_BIT AOR CPSR_IRQ_BIT)
    bne   target_kernel_unc_exc_handler /* カーネル管理外のCPU例外の処理へ */

    /*
     *  例外発生時のコンテキストを判定
     */
    ldr   r1, [r0, #PCB_excpt_nest_count]
    add   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]
    cmp   r1, #1

    /*
     *  多重割込みの場合はスタックを8バイト境界に調整
     */
    andne r2, sp, #4
    subne sp, sp, r2

    /*
     * タスクコンテキストでCPU例外が発生した場合
     * スタックを非タスクコンテキストに切り替える．
     */
    ldreq sp, [r0, #PCB_istkpt]

ALABEL(target_exc_handler_1)
    stmfd sp!, {lr}     /* 復帰用のスタックポインタの保存 +4 */

    /*
     *  locspnidを保存
     */
    my_pcb r0, r1
    ldr    r1, [r0,#PCB_locspnid]
    stmfd  sp!,{r1}     /* +8 */

    /*
     * 割込み発生前の割込み優先度マスクをスタックに保存
     * 割込みハンドラと出口ルーチンを共有するために保存
     */
//    ldr   r12, =(DIC_CPUIF_PMR) /* 例外発生前の割込み優先度マスクを取得 */
//    ldr   r1, [r12]
    stmfd sp!, {r1}  /* 例外発生前の割込み優先度マスクをスタックに保存 +12 */

    /*
     *  CPU例外ハンドラの起動番地を取得
     */
    ldr   r12, [r0, #PCB_p_exch_tbl]  /* 割込みハンドラテーブルの読み出し */
    ldr   r1, [r12, r2, lsl #2]       /* r1<-例外ハンドラ                 */

    stmfd sp!,{r2}      /* excno を保存 +16 */

    /*
     * 割り込み許可
     * CPU管理外の例外ハンドラは別ルーチンで呼び出すため，単純にSVCモード
     * にするだけでよい．
     */
#ifndef ENABLE_NMFI
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_ALWAYS_SET)
#endif /* ENABLE_NMFI */

#ifdef LOG_EXC_ENTER
    stmfd sp!,{r1, r3}   /* -8 */
    mov   r0, r2         /* excno をパラメータに    */
    bl    log_exc_enter  /* log_exc_enterを呼び出す */
    ldmfd sp!,{r1, r3}   /* +8 */
#endif /* LOG_EXC_ENTER */

    /*
     * CPU例外ハンドラの呼び出し
     *
     * 例外フレームの先頭を引数として渡す
     */
    mov   r0,  r3
    mov   lr,  pc
    bx    r1

ALABEL(target_ret_exc)
    ldmfd sp!,{r0}       /* excno を復帰  -4 */
#ifdef LOG_EXC_LEAVE
    stmfd sp!,{r0}       /* スタックアライン調整のためのダミー  -8 */
    bl    log_exc_leave  /* log_exc_leaveを呼び出す */
    stmfd sp!,{r0}       /* スタックアライン調整のためのダミー  -4 */
#endif /* LOG_EXC_LEAVE */

    /*
     * カーネル管理の割込みを禁止する
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)

    /*
     * 割込み優先度マスクを元に戻す
     */
    ldmfd sp!,{r1}                       /* 元の割込み優先度マスクを取得 */
//    ldr   r0, =(DIC_CPUIF_PMR)           /* DICにセット */
//    str   r1, [r0]

    /*
     * CPU例外発生時のlocspnidをr0へ
     */
    ldmfd sp!,{r0}

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!,{r2}              /* 元のスタックポインタを取得 */
    mov   sp, r2

    /*
     * 後の処理はARM依存部で実行
     */
    b     ret_exc


/*
 * カーネル管理外のCPU例外の出入口処理
 */
ALABEL(target_kernel_unc_exc_handler)
    /*
     * 割込み発生時のコンテキストを判定
     */
    ldr   r1, [r0, #PCB_excpt_nest_count]
    add   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]
    cmp   r1, #1

    /*
     *  多重割込みの場合はスタックを8バイト境界に調整
     */
    andne r2, sp, #4
    subne sp, sp, r2

    /*
     * タスクコンテキストでCPU例外が発生した場合
     * スタックを非タスクコンテキストに切り替える．
     */
    ldreq sp, [r0, #PCB_istkpt]

    stmfd sp!, {r1,lr}   /* 復帰用のスタックポインタ(lr)の保存,r1はスタック境界調整のためのダミー -8 */

    /*
     *  CPU例外ハンドラの起動番地を取得
     */
    ldr   r1, [r0, #PCB_p_exch_tbl]   /* 割込みハンドラテーブルの読み出し */
    ldr   r1, [r1, r2, lsl #2]        /* r1<-例外ハンドラ                 */

   /*
    * システム状態（コンテキストは除く）を，CPU例外発生時の状態へ
    */
    ldr   r0, [r3, #EXC_FRAME_cpsr]     /* CPU例外発生前のCPSRの取得 */
    and   r0, r0, #(CPSR_IRQ_BIT AOR CPSR_FIQ_BIT)
    orr   r0, r0, #(CPSR_SVC AOR CPSR_ALWAYS_SET)
    msr   cpsr_cxsf, r0

    /*
     * CPU例外ハンドラの呼び出し
     * 例外フレームの先頭を引数として渡す
     */
    mov   r0, r3
    mov   lr, pc
    bx    r1

    /*
     *  例外・割込みのネストカウント（excpt_nest_count)のデクリメント
     */
    my_pcb r0, r3
    ldr   r1, [r0, #PCB_excpt_nest_count]
    sub   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]

    /*
     * スタックポインタの復帰
     */
    ldmfd sp!, {r1,r2}    /* 元のスタックポインタ(r2)を取得，r1はスタック境界調整のためのダミー +8 */
    mov   sp, r2

    /*
     * CPU例外からの復帰
     */
#if defined(SUPPORT_VFPv3_NEON)
    vpop    {d0-d7}             /* スクラッチレジスタの復帰 */
    pop     {r0}                /* FPSCRレジスタの復帰      */
    vmsr    fpscr, r0
#endif /* SUPPORT_VFPv3_NEON */
    ldmfd sp!,{r0-r3,ip,lr}
    rfeia sp!

    ATEXT
    AGLOBAL(bcm283x_getmycoreindex)
ALABEL(bcm283x_getmycoreindex)
    my_core_index r0
    bx    lr

