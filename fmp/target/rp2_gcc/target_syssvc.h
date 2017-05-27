/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2008-2015 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_syssvc.h 1218 2017-05-28 00:00:00Z azo $
 */

/*
 *  システムサービスのチップ依存部（Raspberry Pi用）
 *
 *  システムサービスのチップ依存部のインクルードファイル．このファイルの
 *  内容は，コンポーネント記述ファイルに記述され，このファイルは無くなる
 *  見込み．
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

#include "bcm283x.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME		"Raspberry Pi 2(ARM)"
#define TARGET_COPYRIGHT	"Copyright (C) 2016 by AZO\n"

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char c);

#ifdef TOPPERS_ENABLE_TRACE
/*
 *  トレースログのファイル出力
 */
extern void	target_fput_log_file(char c);
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  シリアルポート数の定義
 *
 *  If global output (G_SYSLOG) is selected only one UART will be used.
 *  Otherwise, each core will have its own UART.
 *
 *  Note: When SafeG is not used, G_SYSLOG can be defined by the user's
 *  application. When using SafeG, G_SYSLOG is defined (see Makefile.target)
 *  by default to make things easier. For more complicated scenarios
 *  (e.g., each world using 2 UARTS) you have to adapt this file manually.
 */
#define TNUM_PORT       1
#define TNUM_SIOP       1

/*
 *  システムログタスク関連の定数の定義
 *
 *  デフォルト値の通り．
 */

/*
 *  システムログタスクが使用する変数のセクション指定
 */
#define SEC_LOGTASK(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc1p_logtask"),nocommon))
#define SEC_PRC1_LOGTASK(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc1p_logtask"),nocommon))
#define SEC_PRC2_LOGTASK(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc2p_logtask"),nocommon))
#define SEC_PRC3_LOGTASK(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc3p_logtask"),nocommon))
#define SEC_PRC4_LOGTASK(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc4p_logtask"),nocommon))

/*
 *  シリアルインタフェースドライバが使用する変数のセクション指定
 */
#define SEC_SERIAL1(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc1p_serial"),nocommon))

/*
 *  システムログ機能が使用する変数のセクション指定
 */
#define SEC_SYSLOG(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc1p_syslogcb"),nocommon))
#define SEC_PRC1_SYSLOG(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc1p_syslogcb"),nocommon))
#define SEC_PRC2_SYSLOG(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc2p_syslogcb"),nocommon))
#define SEC_PRC3_SYSLOG(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc3p_syslogcb"),nocommon))
#define SEC_PRC4_SYSLOG(type, variable) \
			type variable __attribute__((section(".bss._kernel_prc4p_syslogcb"),nocommon))

/*
 *  UART base address definitions (used in arm_primecell_uart.c)
 *
 *  Note: When using SafeG, G_SYSLOG is defined (see Makefile.target) so
 *  we select a single UART for each world (e.g. Secure uses UART1
 *  and Nonsecure uses UART0).
 */
#define     UART0_BASE  IRQ_UART

/*
 *  UART interrupt handler definitions (used in target_serial.cfg)
 *     INHNO: interrupt handler number
 *     INTNO: interrupt number
 *     INTPRI: interrupt priority (lower means higher priority)
 *     INTATR: interrupt attributes (0 means not enabled at the beginning)
 *
 *  Note: When using SafeG, G_SYSLOG is defined (see Makefile.target) so
 *  we select a single UART for each world (e.g. Secure uses UART0
 *  and Nonsecure uses UART1).
 */
#define    INHNO_SIO0  IRQ_UART
#define    INTNO_SIO0  IRQ_UART
#define    INTPRI_SIO0    -2
#define    INTATR_SIO0     0U

#endif /* TOPPERS_TARGET_SYSSVC_H */
