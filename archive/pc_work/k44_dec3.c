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


//// mtk_clk_register_mux_clr_set_upd @ffffff8008ff31f0

void mtk_clk_register_mux_clr_set_upd(long param_1,undefined8 param_2,undefined8 param_3)

{
  long lVar1;
  long unaff_x19;
  ulong uVar2;
  undefined8 uStack_30;
  long lStack_28;
  undefined8 uStack_20;
  undefined1 uStack_18;
  undefined8 uStack_10;
  long lStack_8;
  
  lStack_8 = lRamffffff80093cff08;
  uVar2 = 0xfffffffffffffff4;
  lVar1 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x40);
  if (lVar1 == 0) goto LAB_ffffff8008ff32f0;
  uStack_30 = *(undefined8 *)(param_1 + 8);
  uStack_10 = 4;
  uStack_20 = *(undefined8 *)(param_1 + 0x10);
  uStack_18 = *(undefined1 *)(param_1 + 0x2c);
  lStack_28 = -0x7ff73a73f0;
  if (-1 < *(char *)(param_1 + 0x2a)) goto LAB_ffffff8008ff330c;
  lStack_28 = -0x7ff73a72b0;
  while( true ) {
    *(undefined8 *)(lVar1 + 0x18) = param_2;
    *(undefined4 *)(lVar1 + 0x20) = *(undefined4 *)(param_1 + 0x18);
    *(undefined4 *)(lVar1 + 0x24) = *(undefined4 *)(param_1 + 0x1c);
    *(undefined4 *)(lVar1 + 0x28) = *(undefined4 *)(param_1 + 0x20);
    *(undefined4 *)(lVar1 + 0x2c) = *(undefined4 *)(param_1 + 0x24);
    *(undefined1 *)(lVar1 + 0x30) = *(undefined1 *)(param_1 + 0x28);
    *(undefined1 *)(lVar1 + 0x31) = *(undefined1 *)(param_1 + 0x29);
    *(undefined1 *)(lVar1 + 0x32) = *(undefined1 *)(param_1 + 0x2a);
    *(undefined1 *)(lVar1 + 0x33) = *(undefined1 *)(param_1 + 0x2b);
    *(undefined8 *)(lVar1 + 0x38) = param_3;
    *(undefined8 **)(lVar1 + 0x10) = &uStack_30;
    uVar2 = clk_register(0,lVar1);
    unaff_x19 = lVar1;
    if (0xfffffffffffff000 < uVar2) {
      kfree(lVar1);
    }
LAB_ffffff8008ff32f0:
    lVar1 = unaff_x19;
    if (lStack_8 == lRamffffff80093cff08) break;
    lStack_28 = __stack_chk_fail(uVar2);
LAB_ffffff8008ff330c:
    lStack_28 = lStack_28 + 0x1e0;
  }
  return;
}


//// mtk_clk_register_composite @ffffff8008ff2aac

void mtk_clk_register_composite(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  int iVar9;
  undefined8 uStack_10;
  long lStack_8;
  
  lStack_8 = lRamffffff80093cff08;
  if (*(char *)(param_1 + 0x30) < '\0') goto LAB_ffffff8008ff2c6c;
  uVar6 = 0xfffffffffffffff4;
  lVar1 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x38);
  if (lVar1 == 0) goto LAB_ffffff8008ff2c4c;
  *(ulong *)(lVar1 + 0x18) = param_2 + (ulong)*(uint *)(param_1 + 0x24);
  uVar8 = 0xffffff8008c585b8;
  *(int *)(lVar1 + 0x28) = (int)(1L << ((long)*(char *)(param_1 + 0x31) & 0x3fU)) + -1;
  *(undefined1 *)(lVar1 + 0x2c) = *(undefined1 *)(param_1 + 0x30);
  *(undefined8 *)(lVar1 + 0x30) = param_3;
  iVar9 = (int)*(char *)(param_1 + 0x35);
  puVar7 = *(undefined8 **)(param_1 + 0x10);
  do {
    if (*(char *)(param_1 + 0x32) < '\0') {
      uVar4 = 0;
      lVar2 = 0;
LAB_ffffff8008ff2bb0:
      if (*(char *)(param_1 + 0x33) < '\0') {
        uVar5 = 0;
        lVar3 = 0;
      }
      else {
        lVar3 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x38);
        if (lVar3 == 0) goto LAB_ffffff8008ff2ca8;
        param_2 = param_2 + (ulong)*(uint *)(param_1 + 0x28);
        *(long *)(lVar3 + 0x18) = param_2;
        uVar5 = 0xffffff8008c581c0;
        *(undefined1 *)(lVar3 + 0x20) = *(undefined1 *)(param_1 + 0x33);
        *(undefined1 *)(lVar3 + 0x21) = *(undefined1 *)(param_1 + 0x34);
        *(undefined8 *)(lVar3 + 0x30) = param_3;
      }
      uVar6 = clk_register_composite
                        (0,*(undefined8 *)(param_1 + 8),puVar7,iVar9,lVar1,uVar8,lVar3,uVar5,lVar2,
                         uVar4,*(undefined4 *)(param_1 + 0x20));
      if (0xfffffffffffff000 < uVar6) {
        kfree(lVar2);
        kfree(lVar1);
      }
    }
    else {
      lVar2 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x30);
      if (lVar2 != 0) {
        *(ulong *)(lVar2 + 0x18) = param_2 + (ulong)*(uint *)(param_1 + 0x2c);
        uVar4 = 0xffffff8008c583c0;
        *(undefined1 *)(lVar2 + 0x20) = *(undefined1 *)(param_1 + 0x32);
        *(undefined8 *)(lVar2 + 0x28) = param_3;
        *(undefined1 *)(lVar2 + 0x21) = 1;
        goto LAB_ffffff8008ff2bb0;
      }
LAB_ffffff8008ff2ca8:
      uVar6 = 0xfffffffffffffff4;
      kfree(lVar1);
    }
LAB_ffffff8008ff2c4c:
    if (lStack_8 == lRamffffff80093cff08) {
      return;
    }
    __stack_chk_fail(uVar6);
LAB_ffffff8008ff2c6c:
    uStack_10 = *(undefined8 *)(param_1 + 0x18);
    iVar9 = 1;
    uVar8 = 0;
    lVar1 = 0;
    puVar7 = &uStack_10;
  } while( true );
}


