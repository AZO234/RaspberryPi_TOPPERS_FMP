$
$       オフセットファイル生成用テンプレートファイル（BCM283X用）
$

$
$  標準テンプレートファイルのインクルード
$
$INCLUDE "kernel/genoffset.tf"$

$
$  コア依存テンプレートのインクルード（ARM用）
$
$INCLUDE"../common/core_offset.tf"$

$
$  オフセット値のマクロ定義の生成
$
$DEFINE("PCB_p_inh_tbl", offsetof_PCB_p_inh_tbl)$
