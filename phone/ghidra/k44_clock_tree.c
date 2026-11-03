//// mtk_apmixed_init @ffffff8008ff34bc

void mtk_apmixed_init(undefined8 param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;

  lVar2 = of_iomap(param_1,0);
  if (lVar2 == 0) {
    printk(0xffffff8008f080c3,0xffffff8008c59070);
  }
  else {
    uVar3 = mtk_alloc_clk_data(7);
    mtk_clk_register_plls(param_1,0xffffff8008c59088,6,uVar3);
    iVar1 = of_clk_add_provider(param_1,of_clk_src_onecell_get,uVar3);
    if (iVar1 != 0) {
      printk(0xffffff8008f080db,0xffffff8008c59070,iVar1);
    }
    lRamffffff8009842358 = lVar2;
    *(uint *)(lVar2 + 0x220) = *(uint *)(lVar2 + 0x220) & 0x8fffffff;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842358 + 0x18) = *(uint *)(lRamffffff8009842358 + 0x18) & 0xfffdffff;
    DataSynchronizationBarrier(3,3,0);
  }
  return;
}


//// mtk_top_init @ffffff8008ff358c

void mtk_top_init(undefined8 param_1)

{
  int iVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;

  lVar2 = of_iomap(param_1,0);
  lVar3 = of_iomap(param_1,1);
  if (lVar2 == 0 || lVar3 == 0) {
    printk(0xffffff8008f080c3,0xffffff8008c59328);
  }
  else {
    uVar4 = mtk_alloc_clk_data(0x43);
    mtk_clk_register_fixed_clks(0xffffff800901bf08,1,uVar4);
    mtk_clk_register_factors_pdn(0xffffff800901bf28,0x28,uVar4,lVar3);
    mtk_clk_register_mux_clr_set_upds(0xffffff800901c568,0x18,lVar2,0xffffff8009842360,uVar4);
    iVar1 = of_clk_add_provider(param_1,of_clk_src_onecell_get,uVar4);
    if (iVar1 != 0) {
      printk(0xffffff8008f080db,0xffffff8008c59328,iVar1);
    }
    lRamffffff8009842368 = lVar2;
    *(uint *)(lVar2 + 0x200) = *(uint *)(lVar2 + 0x200) | 0x3ef;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842368 + 0x204) = *(uint *)(lRamffffff8009842368 + 0x204) | 0x11;
    DataSynchronizationBarrier(3,3,0);
  }
  return;
}


//// mtk_infra_init @ffffff8008ff36c4

void mtk_infra_init(undefined8 param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;

  lVar2 = of_iomap(param_1,0);
  if (lVar2 == 0) {
    printk(0xffffff8008f080c3,0xffffff8008c59338);
  }
  else {
    uVar3 = mtk_alloc_clk_data(0x50);
    mtk_clk_register_gates(param_1,0xffffff800901c9e8,0x4f,uVar3);
    iVar1 = of_clk_add_provider(param_1,of_clk_src_onecell_get,uVar3);
    if (iVar1 != 0) {
      printk(0xffffff8008f080db,0xffffff8008c59338,iVar1);
    }
    lRamffffff8009842370 = lVar2;
    *(undefined4 *)(lVar2 + 0x80) = 0x9bbfff20;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff8009842370 + 0x88) = 0x1e8f7f56;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff8009842370 + 0xa4) = 0x7fcc7dd;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff8009842370 + 0xc0) = 0xdff;
    DataSynchronizationBarrier(3,3,0);
  }
  return;
}


//// mtk_clk_register_plls @ffffff8008ff2d78

void mtk_clk_register_plls(undefined8 param_1,int *param_2,int param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  long *plVar4;
  long unaff_x22;
  undefined8 unaff_x23;
  int unaff_w25;
  undefined8 unaff_x26;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 *puStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  long lStack_8;

  plVar4 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  lVar1 = of_iomap(param_1,0,0);
  if (lVar1 == 0) {
    printk(0xffffff8008f080a4,0xffffff8008c58a18);
    goto LAB_ffffff8008ff2ecc;
  }
  unaff_x26 = 0xffffff8008f080bc;
  unaff_x23 = 0xffffff8008c58a30;
  unaff_w25 = 0;
  do {
    unaff_x22 = lVar1;
    if (unaff_w25 < param_3) {
      uStack_30 = 0;
      uStack_28 = 0;
      puStack_20 = (undefined8 *)0x0;
      uStack_18 = 0;
      uStack_10 = 0;
      uStack_38 = unaff_x26;
      lVar2 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x48);
      if (lVar2 == 0) goto LAB_ffffff8008ff2ee4;
      *(ulong *)(lVar2 + 0x18) = lVar1 + (ulong)(uint)param_2[4];
      *(ulong *)(lVar2 + 0x28) = lVar1 + (ulong)(uint)param_2[5];
      *(ulong *)(lVar2 + 0x20) = lVar1 + (ulong)(uint)param_2[7];
      *(ulong *)(lVar2 + 0x38) = lVar1 + (ulong)(uint)param_2[0x17];
      if (param_2[8] != 0) {
        *(ulong *)(lVar2 + 0x30) = lVar1 + (ulong)(uint)param_2[8];
      }
      *(int **)(lVar2 + 0x40) = param_2;
      *(undefined8 **)(lVar2 + 0x10) = &uStack_30;
      uStack_30 = *(undefined8 *)(param_2 + 2);
      puStack_20 = &uStack_38;
      uStack_18 = CONCAT71(uStack_18._1_7_,1);
      uStack_28 = unaff_x23;
      uVar3 = clk_register(0,lVar2);
      if (0xfffffffffffff000 < uVar3) {
        kfree(lVar2);
        goto LAB_ffffff8008ff2e98;
      }
      *(ulong *)(*param_4 + (long)*param_2 * 8) = uVar3;
    }
    else {
LAB_ffffff8008ff2ecc:
      plVar4 = plVar4 + 0x1e1;
      if (lStack_8 == *plVar4) {
        return;
      }
      __stack_chk_fail();
LAB_ffffff8008ff2ee4:
      uVar3 = 0xfffffffffffffff4;
LAB_ffffff8008ff2e98:
      printk(0xffffff8008f08060,*(undefined8 *)(param_2 + 2),uVar3);
      lVar1 = unaff_x22;
    }
    unaff_w25 = unaff_w25 + 1;
    param_2 = param_2 + 0x1c;
  } while( true );
}


//// mtk_clk_register_factors @ffffff8008ff28ac

void mtk_clk_register_factors(int *param_1,int param_2,long *param_3)

{
  ulong uVar1;
  int iVar2;

  for (iVar2 = 0; iVar2 < param_2; iVar2 = iVar2 + 1) {
    uVar1 = clk_register_fixed_factor
                      (0,*(undefined8 *)(param_1 + 2),*(undefined8 *)(param_1 + 4),4,param_1[6],
                       param_1[7]);
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
