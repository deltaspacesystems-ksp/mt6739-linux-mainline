//// mtk_clk_register_factors_pdn @ffffff8008ff2940

void mtk_clk_register_factors_pdn(int *param_1,int param_2,long *param_3,undefined8 param_4)

{
  ulong uVar1;
  int iVar2;

  for (iVar2 = 0; iVar2 < param_2; iVar2 = iVar2 + 1) {
    uVar1 = mtk_clk_register_fixed_factor_pdn
                      (0,*(undefined8 *)(param_1 + 2),*(undefined8 *)(param_1 + 4),4,param_1[6],
                       param_1[7],param_1[8],param_1[9],param_4);
    if (param_3 != (long *)0x0 && uVar1 < 0xfffffffffffff001) {
      *(ulong *)(*param_3 + (long)*param_1 * 8) = uVar1;
    }
    param_1 = param_1 + 10;
  }
  return;
}


//// mtk_clk_register_mux_clr_set_upds @ffffff8008ff3328

void mtk_clk_register_mux_clr_set_upds
               (int *param_1,int param_2,undefined8 param_3,undefined8 param_4,long *param_5)

{
  ulong uVar1;
  int iVar2;

  for (iVar2 = 0; iVar2 < param_2; iVar2 = iVar2 + 1) {
    uVar1 = mtk_clk_register_mux_clr_set_upd(param_1,param_3,param_4);
    if (uVar1 < 0xfffffffffffff001) {
      if (param_5 != (long *)0x0) {
        *(ulong *)(*param_5 + (long)*param_1 * 8) = uVar1;
      }
    }
    else {
      printk(0xffffff8008f08060,*(undefined8 *)(param_1 + 2),uVar1);
    }
    param_1 = param_1 + 0xc;
  }
  return;
}


//// mtk_clk_register_fixed_clks @ffffff8008ff2810

void mtk_clk_register_fixed_clks(int *param_1,int param_2,long *param_3)

{
  ulong uVar1;
  int iVar2;

  for (iVar2 = 0; iVar2 < param_2; iVar2 = iVar2 + 1) {
    uVar1 = clk_register_fixed_rate
                      (0,*(undefined8 *)(param_1 + 2),*(long *)(param_1 + 4),
                       (ulong)(*(long *)(param_1 + 4) == 0) << 4,*(undefined8 *)(param_1 + 6));
    if (uVar1 < 0xfffffffffffff001) {
      if (param_3 != (long *)0x0) {
        *(ulong *)(*param_3 + (long)*param_1 * 8) = uVar1;
      }
    }
    else {
      printk(0xffffff8008f08060,*(undefined8 *)(param_1 + 2),uVar1);
    }
    param_1 = param_1 + 8;
  }
  return;
}


//// mtk_pinctrl_update_gpio_value @ffffff80083b5a38

undefined8
mtk_pinctrl_update_gpio_value(long param_1,int param_2,uint param_3,int param_4,int *param_5)

{
  undefined8 uVar1;
  int iVar2;

  iVar2 = 0;
  while( true ) {
    if (param_4 <= iVar2) {
      return 0xffffffff;
    }
    if (*param_5 == param_2) break;
    iVar2 = iVar2 + 1;
    param_5 = param_5 + 3;
  }
  if (param_5 != (int *)0x0) {
    uVar1 = regmap_update_bits(*(undefined8 *)
                                (param_1 + ((ulong)*(byte *)((long)param_5 + 10) + 2) * 8),
                               param_5[1],
                               (1L << ((ulong)*(byte *)((long)param_5 + 9) & 0x3f)) + -1 <<
                               ((ulong)*(byte *)(param_5 + 2) & 0x3f),
                               (param_3 & 0xff) << (ulong)(*(byte *)(param_5 + 2) & 0x1f));
    return uVar1;
  }
  return 0xffffffff;
}
