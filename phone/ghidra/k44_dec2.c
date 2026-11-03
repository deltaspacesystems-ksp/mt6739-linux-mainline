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


//// clk_mt6739_init @ffffff8008ff34b4

undefined8 clk_mt6739_init(void)

{
  return 0;
}


//// mtk_clk_register_gates @ffffff8008ff29d0

ulong mtk_clk_register_gates(long param_1,int *param_2,int param_3,long *param_4)

{
  undefined4 *puVar1;
  ulong uVar2;
  ulong uVar3;
  int iVar4;
  
  if (param_4 != (long *)0x0) {
    uVar2 = syscon_node_to_regmap();
    if (uVar2 < 0xfffffffffffff001) {
      for (iVar4 = 0; iVar4 < param_3; iVar4 = iVar4 + 1) {
        puVar1 = *(undefined4 **)(param_2 + 6);
        uVar3 = mtk_clk_register_gate
                          (*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4),uVar2,puVar1[2]
                           ,puVar1[1],*puVar1,(char)param_2[8],*(undefined8 *)(param_2 + 10));
        if (uVar3 < 0xfffffffffffff001) {
          *(ulong *)(*param_4 + (long)*param_2 * 8) = uVar3;
        }
        else {
          printk(0xffffff8008f08060,*(undefined8 *)(param_2 + 2),uVar3);
        }
        param_2 = param_2 + 0xc;
      }
      uVar2 = 0;
    }
    else {
      printk(0xffffff8008f08082,*(undefined8 *)(param_1 + 0x18),uVar2);
      uVar2 = uVar2 & 0xffffffff;
    }
    return uVar2;
  }
  return 0xfffffff4;
}


//// mtk_clk_register_gate @ffffff8008ff2f18

void mtk_clk_register_gate
               (undefined8 param_1,long param_2,undefined8 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined1 param_7,undefined8 param_8)

{
  long lVar1;
  ulong uVar2;
  long lStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  long *plStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  long lStack_8;
  
  lStack_8 = lRamffffff80093cff08;
  uStack_30 = 0;
  uStack_28 = 0;
  plStack_20 = (long *)0x0;
  uStack_18 = 0;
  uVar2 = 0xfffffffffffffff4;
  uStack_10 = 0;
  lStack_38 = param_2;
  lVar1 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x30);
  if (lVar1 != 0) {
    uStack_10 = 4;
    *(undefined1 *)(lVar1 + 0x2c) = param_7;
    plStack_20 = &lStack_38;
    *(undefined4 *)(lVar1 + 0x28) = param_6;
    *(undefined4 *)(lVar1 + 0x20) = param_4;
    *(undefined4 *)(lVar1 + 0x24) = param_5;
    if (lStack_38 == 0) {
      plStack_20 = (long *)0x0;
    }
    uStack_18 = CONCAT71(uStack_18._1_7_,lStack_38 != 0);
    *(undefined8 **)(lVar1 + 0x10) = &uStack_30;
    *(undefined8 *)(lVar1 + 0x18) = param_3;
    uStack_30 = param_1;
    uStack_28 = param_8;
    uVar2 = clk_register(0,lVar1);
    if (0xfffffffffffff000 < uVar2) {
      kfree(lVar1);
    }
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar2);
  }
  return;
}


//// mtk_mm_init @ffffff8008ff3888

void mtk_mm_init(undefined8 param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  
  lVar2 = of_iomap(param_1,0);
  if (lVar2 == 0) {
    printk(0xffffff8008f080c3,0xffffff8008c59358);
    lVar2 = lRamffffff8009842380;
  }
  else {
    uVar3 = mtk_alloc_clk_data(0x20);
    mtk_clk_register_gates(param_1,0xffffff800901dac8,0x20,uVar3);
    iVar1 = of_clk_add_provider(param_1,of_clk_src_onecell_get,uVar3);
    if (iVar1 != 0) {
      printk(0xffffff8008f080db,0xffffff8008c59358,iVar1);
    }
  }
  lRamffffff8009842380 = lVar2;
  return;
}


