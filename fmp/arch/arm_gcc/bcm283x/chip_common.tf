$ 
$     パス2のチップ依存テンプレート（BCM283X用）
$     ARMCCとの共通部

$ 
$ 有効なCPU例外ハンドラ番号
$ 
$EXCNO_VALID = {
	0x10001,0x10002,0x10003,0x10004,0x10006;
	0x20001,0x20002,0x20003,0x20004,0x20006;
	0x30001,0x30002,0x30003,0x30004,0x30006;
	0x40001,0x40002,0x40003,0x40004,0x40006
}$

$ 
$  有効な割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号
$ 
$INTNO_VALID = {
	0x10000,0x10001,...,0x1007F
}$
$INHNO_VALID = INTNO_VALID$

$ 
$  ATT_ISRで使用できる割込み番号とそれに対応する割込みハンドラ番号
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INTで使用できる割込みハンドラ番号
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$
$INTPRI_CFGINT_VALID = { -1,-2,...,-15 }$

$ 
$  オブジェクト関連のメモリの配置先指定（プロセッサ）
$ 

$ 
$  クラスに対する定義
$ 

$ 
$  クラス数
$ 
$NUM_CLASS = 9$

$ 
$ クラス TCL_1（クラスID 1）に関する指定
$ 

$  初期割付けプロセッサ(ID指定)
$CLASS_AFFINITY_INI[1] = 1$

$  マイグレーション可能なプロセッサのマスク指定
$CLASS_AFFINITY_MASK[1] = 0x0000000f$

$  オブジェクトロック
$ P_LOCKの場合に使用するオブジェクトロックを持つプロセッサのIDを指定
$CLASS_OBJ_LOCK[1] = 1$

$ 
$ クラス TCL_2（クラスID 2）に関する指定
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x0000000f$
$CLASS_OBJ_LOCK[2] = 2$

$ 
$ クラス TCL_3（クラスID 3） に関する指定
$ 
$CLASS_AFFINITY_INI[3] = 3$
$CLASS_AFFINITY_MASK[3] = 0x0000000f$
$CLASS_OBJ_LOCK[3] = 3$

$ 
$ クラス TCL_4（クラスID 4） に関する指定
$ 
$CLASS_AFFINITY_INI[4] = 4$
$CLASS_AFFINITY_MASK[4] = 0x0000000f$
$CLASS_OBJ_LOCK[4] = 4$

$ 
$ クラス TCL_1_ONLY（クラスID 5）に関する指定
$ 
$CLASS_AFFINITY_INI[5] = 1$
$CLASS_AFFINITY_MASK[5] = 0x00000001$
$CLASS_OBJ_LOCK[5] = 1$

$ 
$ クラス TCL_2_ONLY（クラスID 6）に関する指定
$ 
$CLASS_AFFINITY_INI[6] = 2$
$CLASS_AFFINITY_MASK[6] = 0x00000002$
$CLASS_OBJ_LOCK[6] = 2$

$ 
$ クラス TCL_3_ONLY（クラスID 7） に関する指定
$ 
$CLASS_AFFINITY_INI[7] = 3$
$CLASS_AFFINITY_MASK[7] = 0x00000004$
$CLASS_OBJ_LOCK[7] = 3$

$ 
$ クラス TCL_4_ONLY（クラスID 8） に関する指定
$ 
$CLASS_AFFINITY_INI[8] = 4$
$CLASS_AFFINITY_MASK[8] = 0x00000008$
$CLASS_OBJ_LOCK[8] = 4$

$ 
$ クラス TCL_SYSTIM_PRC（クラスID 9） に関する指定
$ 
$CLASS_AFFINITY_INI[9] = TOPPERS_SYSTIM_PRCID$
$CLASS_AFFINITY_MASK[9] = 0x1 << (TOPPERS_SYSTIM_PRCID - 1) $
$CLASS_OBJ_LOCK[9] = TOPPERS_SYSTIM_PRCID$

$ 
$  コア依存テンプレートのインクルード（ARM用）
$ 
$INCLUDE"../../arch/arm_gcc/common/core.tf"$

$ 
$  割込み優先度テーブル
$ 

const PRI _kernel_inh_ipm_table[TNUM_INT] = {$NL$
$FOREACH inhno INHNO_VALID$
	$inhnop = inhno & 0xFFFF$
	$IF LENGTH(INH.INHNO[inhno])$
	  $TAB$$INT.INTPRI[inhno]$,
	$ELSE$
	  $TAB$0,
	$END$
	$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
$END$
};$NL$
$NL$

$ 
$  割込みマスクテーブル
$ 

const uint32_t _kernel_ipm_mask_table[TNUM_INTPRI][4]={$NL$
$FOREACH intpri { 0,-1,...,-15 }$
	$intmaskb = 0$
	$intmask1 = 0$
	$intmask2 = 0$
	$intmaskc0 = 0$
	$FOREACH intno (INT.ID_LIST)$
		$intnop = intno & 0xFFFF$
		$IF intnop < 32$
			$IF INT.INTPRI[intno] >= intpri$
				$intmaskb = intmaskb | (0x1 << intnop)$
			$END$
		$ELSE$
			$IF intnop < 64$
				$IF INT.INTPRI[intno] >= intpri$
					$intmask1 = intmask1 | (0x1 << (intnop - 32))$
				$END$
			$ELSE$
				$IF intnop < 96$
					$IF INT.INTPRI[intno] >= intpri$
						$intmask2 = intmask2 | (0x1 << (intnop - 64))$
					$END$
				$ELSE$
					$IF INT.INTPRI[intno] >= intpri$
						$intmaskc0 = intmaskc0 | (0x1 << (intnop - 96))$
					$END$
				$END$
			$END$
		$END$
	 $END$
 $TAB${UINT32_C($FORMAT("0x%08x", intmaskb)$), UINT32_C($FORMAT("0x%08x", intmask1)$), UINT32_C($FORMAT("0x%08x", intmask2)$), UINT32_C($FORMAT("0x%08x", intmaskc0)$)}, /* Priority $+intpri$ */$NL$
$END$
};$NL$
$NL$

$ 
$  割込みハンドラテーブル
$ 

const FP _kernel_inh_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$inhnop = inhno & 0xFFFF$
	$IF LENGTH(INH.INHNO[inhno]) != 0$
		$TAB$(FP)($INH.INTHDR[inhno]$),
	$ELSE$
		$TAB$(FP)(_kernel_default_int_handler),
	$END$
	$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
$END$
};$NL$
$NL$

