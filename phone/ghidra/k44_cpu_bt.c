//// mt_dfs_armpll @ffffff800867b4bc

undefined8 mt_dfs_armpll(void)

{
  undefined8 uVar1;

  if (lRamffffff8009662c78 != 0) {
    uVar1 = (**(code **)(lRamffffff8009662c78 + 0xb0))();
    return uVar1;
  }
  return 1;
}


//// _mt_cpufreq_init @ffffff80084497e8

undefined8 _mt_cpufreq_init(undefined8 *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined8 uVar6;

  *(undefined4 *)(param_1 + 3) = 3;
  *param_1 = 0x3ff;
  *(undefined4 *)(param_1 + 6) = 1000;
  puVar1 = (undefined8 *)0xffffff80094429d8;
  uVar3 = arch_get_cluster_id(*(undefined4 *)((long)param_1 + 0x1c));
  if (uVar3 != 0) {
    puVar1 = (undefined8 *)0x0;
  }
  uVar4 = _mt_cpufreq_get_cpu_level();
  if (iRamffffff80095c3128 != 0) {
    printk(0xffffff8008dfaaa0,*puVar1,*(undefined4 *)((long)puVar1 + 0xc));
  }
  *(uint *)(puVar1 + 2) = uVar4;
  lVar2 = -0x7ff6bbd628;
  iVar5 = arch_get_cluster_id(*(undefined4 *)((long)param_1 + 0x1c));
  if (iVar5 != 0) {
    lVar2 = 0;
  }
  iVar5 = cpufreq_frequency_table_cpuinfo(param_1,*(undefined8 *)(lVar2 + 0x58));
  if (iVar5 == 0) {
    param_1[0x14] = *(undefined8 *)(lVar2 + 0x58);
  }
  uVar6 = *(undefined8 *)((ulong)*(uint *)((long)param_1 + 0x1c) * 0x20 + -0x7ff6af5a88);
  *param_1 = uVar6;
  param_1[1] = uVar6;
  *(undefined4 *)(param_1 + 5) = *(undefined4 *)puVar1[7];
  *(undefined4 *)((long)param_1 + 0x2c) =
       *(undefined4 *)(puVar1[7] + (long)*(int *)(puVar1 + 8) * 8 + -8);
  iVar5 = -1;
  puVar1[6] = *(undefined8 *)(((ulong)uVar3 * 3 + (ulong)uVar4) * 8 + -0x7ff6bbd9c0);
  mutex_lock(0xffffff8009442618);
  if (puVar1[7] != 0) {
    iVar5 = _sync_opp_tbl_idx(puVar1);
  }
  if (iRamffffff80095c3128 != 0) {
    printk(0xffffff8008dfaadb,*puVar1,
           *(undefined4 *)(puVar1[7] + (long)*(int *)((long)puVar1 + 0x44) * 8));
  }
  if ((-1 < iVar5) && (*(int *)((long)puVar1 + 0x54) == -1)) {
    *(undefined4 *)((long)puVar1 + 0x54) = *(undefined4 *)((long)puVar1 + 0x44);
  }
  *(undefined4 *)((long)param_1 + 0x3c) =
       *(undefined4 *)(puVar1[7] + (long)*(int *)((long)puVar1 + 0x44) * 8);
  *(undefined4 *)(param_1 + 7) =
       *(undefined4 *)(puVar1[7] + (long)*(int *)((long)puVar1 + 0x4c) * 8);
  *(undefined4 *)((long)param_1 + 0x34) =
       *(undefined4 *)(puVar1[7] + (long)*(int *)(puVar1 + 9) * 8);
  puVar1[3] = param_1;
  *(undefined4 *)(puVar1 + 10) = 1;
  mutex_unlock(0xffffff8009442618);
  return 0;
}


//// hal_btif_clk_get_and_prepare @ffffff800871281c

int hal_btif_clk_get_and_prepare(long param_1)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 uVar4;

  uRamffffff80096a32a8 = devm_clk_get(param_1 + 0x10,0xffffff8008ea6786);
  if (uRamffffff80096a32a8 < 0xfffffffffffff001) {
    iVar1 = mtk_btif_hal_get_log_lvl();
    if (-1 < iVar1) {
      hal_log_print(0xffffff8008ea67bf,0xffffff8008c231b0,0x120,uRamffffff80096a32a8);
    }
    uRamffffff80096a32b0 = devm_clk_get(param_1 + 0x10,0xffffff8008ea67e3);
    if (uRamffffff80096a32b0 < 0xfffffffffffff001) {
      iVar1 = mtk_btif_hal_get_log_lvl();
      if (-1 < iVar1) {
        hal_log_print(0xffffff8008ea6823,0xffffff8008c231b0,0x126,uRamffffff80096a32b0);
      }
      iVar1 = clk_prepare(uRamffffff80096a32a8);
      if (iVar1 == 0) {
        iVar1 = clk_prepare(uRamffffff80096a32b0);
        if (iVar1 == 0) {
          return 0;
        }
        iVar2 = mtk_btif_hal_get_log_lvl();
        if (iVar2 < 0) {
          return iVar1;
        }
        uVar4 = 0x130;
        uVar3 = 0xffffff8008ea6884;
      }
      else {
        iVar2 = mtk_btif_hal_get_log_lvl();
        if (iVar2 < 0) {
          return iVar1;
        }
        uVar3 = 0xffffff8008ea684d;
        uVar4 = 0x12a;
      }
      hal_log_print(uVar3,0xffffff8008c231b0,uVar4,iVar1);
    }
    else {
      iVar1 = mtk_btif_hal_get_log_lvl();
      if (-1 < iVar1) {
        hal_log_print(0xffffff8008ea67ea,0xffffff8008c231b0,0x123);
      }
      iVar1 = (int)uRamffffff80096a32b0;
    }
  }
  else {
    iVar1 = mtk_btif_hal_get_log_lvl();
    if (-1 < iVar1) {
      hal_log_print(0xffffff8008ea678c,0xffffff8008c231b0,0x11d);
    }
    iVar1 = (int)uRamffffff80096a32a8;
  }
  return iVar1;
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
