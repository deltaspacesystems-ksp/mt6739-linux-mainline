//// dsi_wait_not_busy @ffffff800858aa70

void dsi_wait_not_busy(uint param_1,long param_2)

{
  uint uVar1;
  ulong uVar2;
  long lVar3;
  ulong unaff_x19;
  long *plVar4;
  int unaff_w21;
  ulong uVar5;
  undefined4 auStack_30 [6];
  undefined8 **ppuStack_18;
  undefined8 **ppuStack_10;
  long lStack_8;

  plVar4 = (long *)0xffffff80093cf000;
  uVar5 = (ulong)param_1;
  lStack_8 = lRamffffff80093cff08;
  if (param_1 != 0x22) {
    if (param_1 == 0x23) {
      unaff_w21 = 1;
      goto LAB_ffffff800858aac8;
    }
    if (param_1 != 0x24) goto LAB_ffffff800858ab24;
  }
  unaff_w21 = 0;
LAB_ffffff800858aac8:
  unaff_x19 = *(ulong *)((long)unaff_w21 * 8 + -0x7ff69d9ee0);
  if ((*(byte *)(unaff_x19 + 0x14) & 3) != 0) goto LAB_ffffff800858ab24;
  if (param_2 == 0) {
    if (-1 < *(char *)(unaff_x19 + 0xf)) goto LAB_ffffff800858ab24;
    ppuStack_18 = &ppuStack_18;
    lVar3 = 0x1e;
    auStack_30[0] = 0;
    param_2 = (long)unaff_w21 * 0x500 + -0x7ff69d9818;
    ppuStack_10 = ppuStack_18;
    while( true ) {
      prepare_to_wait_event(param_2,auStack_30,2);
      uVar1 = ~(uint)*(byte *)(*(long *)((long)unaff_w21 * 8 + -0x7ff69d9ee0) + 0xf) >> 7 & 1;
      if ((uVar1 & lVar3 == 0) != 0) {
        lVar3 = 1;
      }
      if (uVar1 != 0 || lVar3 == 0) break;
      lVar3 = schedule_timeout(lVar3);
    }
    finish_wait(param_2,auStack_30);
    unaff_x19 = 0xffffff8009626120;
    if ((int)lVar3 != 0) goto LAB_ffffff800858ab24;
    dprec_logger_pr(0,0xffffff8008e4706e,unaff_w21);
    printk(0xffffff8008e47058,0xffffff8008c00690,0x27a,unaff_w21);
    disp_dsi_dump_reg(param_1);
    DSI_Reset(param_1,0);
    goto LAB_ffffff800858ab24;
  }
  unaff_x19 = unaff_x19 + 0xc;
  unaff_w21 = 0;
  uVar5 = unaff_x19 & 0xfffffffffffff000;
  do {
    uVar2 = ddp_get_module_va(unaff_w21);
    if (uVar2 != uVar5) goto LAB_ffffff800858ab3c;
    lVar3 = ddp_get_module_pa(unaff_w21);
    lVar3 = (unaff_x19 & 0xfff) + lVar3;
    while( true ) {
      cmdqRecPoll(param_2,lVar3,0,0x80000000);
LAB_ffffff800858ab24:
      plVar4 = plVar4 + 0x1e1;
      if (lStack_8 == *plVar4) {
        return;
      }
      __stack_chk_fail();
LAB_ffffff800858ab3c:
      unaff_w21 = unaff_w21 + 1;
      if (unaff_w21 != 0x31) break;
      dprec_logger_pr(0,0xffffff8008e456bf,unaff_x19);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,unaff_x19);
      unaff_x19 = 0xffffff8008e456e8;
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar3 = 0;
    }
  } while( true );
}


//// ddp_ovl_get_cur_addr @ffffff800857ed1c

uint ddp_ovl_get_cur_addr(char param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;

  lVar3 = ovl_base_addr(0);
  if (param_1 == '\0') {
    lVar3 = lVar3 + (param_2 << 5);
    uVar1 = *(uint *)(lVar3 + 0xc0);
    uVar2 = uVar1 & 1;
    if ((uVar1 & 1) != 0) {
      uVar2 = *(uint *)(lVar3 + 0xf40);
    }
  }
  else {
    lVar3 = ddp_get_module_va(7);
    uVar2 = *(uint *)(lVar3 + 0xf00);
  }
  return uVar2;
}


//// ovl_get_address @ffffff800857ed7c

void ovl_get_address(undefined8 param_1,long param_2)

{
  uint uVar1;
  long lVar2;
  uint *puVar3;

  lVar2 = ovl_base_addr();
  uVar1 = *(uint *)(lVar2 + 0x2c);
  puVar3 = (uint *)(lVar2 + 0xf40);
  lVar2 = 0;
  do {
    if ((1 << (ulong)((uint)lVar2 & 0x1f) & uVar1) == 0) {
      *(undefined8 *)(param_2 + lVar2 * 8) = 0;
    }
    else {
      *(ulong *)(param_2 + lVar2 * 8) = (ulong)*puVar3;
    }
    lVar2 = lVar2 + 1;
    puVar3 = puVar3 + 8;
  } while (lVar2 != 4);
  return;
}


//// ddp_mutex_enable @ffffff80085a48b0

undefined8 ddp_mutex_enable(int param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  ulong uVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;

  iVar2 = ddp_debug_dbg_log_level();
  if ((iVar2 != 0) && (dprec_logger_pr(2,0xffffff8008e49f32,param_1), iRamffffff8009639114 != 0)) {
    printk(0xffffff8008e49f2a,param_1);
  }
  if (param_4 == 0) {
    lVar3 = ddp_get_module_va(0x28);
    *(undefined4 *)(lVar3 + 8) = 0;
    DataSynchronizationBarrier(3,3,0);
    lVar3 = ddp_get_module_va(0x28);
    *(undefined4 *)((long)(param_1 << 5) + 0x20 + lVar3) = 1;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    lVar3 = ddp_get_module_va(0x28);
    uVar1 = lVar3 + 8;
    iVar2 = 0;
    do {
      uVar4 = ddp_get_module_va(iVar2);
      if (uVar4 == (uVar1 & 0xfffffffffffff000)) {
        lVar3 = ddp_get_module_pa(iVar2);
        lVar3 = (uVar1 & 0xfff) + lVar3;
        goto LAB_ffffff80085a498c;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00e68,0xba,uVar1);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar3 = 0;
LAB_ffffff80085a498c:
    cmdqRecWrite(param_4,lVar3,0,0xffffffff);
    iVar2 = 0;
    lVar3 = ddp_get_module_va(0x28);
    uVar1 = (long)(param_1 << 5) + 0x20 + lVar3;
    do {
      uVar4 = ddp_get_module_va(iVar2);
      if (uVar4 == (uVar1 & 0xfffffffffffff000)) {
        lVar3 = ddp_get_module_pa(iVar2);
        lVar3 = (uVar1 & 0xfff) + lVar3;
        goto LAB_ffffff80085a49dc;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00e68,0xba,uVar1);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar3 = 0;
LAB_ffffff80085a49dc:
    cmdqRecWrite(param_4,lVar3,1,0xffffffff);
  }
  return 0;
}


//// ddp_mutex_set_sof_wait @ffffff80085a4764

undefined8 ddp_mutex_set_sof_wait(uint param_1,long param_2,uint param_3)

{
  ulong uVar1;
  uint uVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  int iVar7;

  if (param_1 < 4) {
    if (param_2 == 0) {
      lVar4 = ddp_get_module_va(0x28);
      lVar5 = (long)(int)(param_1 << 5) + 0x2c;
      uVar2 = *(uint *)(lVar4 + lVar5);
      lVar4 = ddp_get_module_va(0x28);
      *(uint *)(lVar4 + lVar5) = (param_3 & 1) << 5 | uVar2 & 0xffffffdf;
      DataSynchronizationBarrier(3,3,0);
    }
    else {
      lVar5 = ddp_get_module_va(0x28);
      iVar7 = 0;
      uVar1 = (long)(int)(param_1 << 5) + 0x2c + lVar5;
      do {
        uVar6 = ddp_get_module_va(iVar7);
        if (uVar6 == (uVar1 & 0xfffffffffffff000)) {
          lVar5 = ddp_get_module_pa(iVar7);
          lVar5 = (uVar1 & 0xfff) + lVar5;
          goto LAB_ffffff80085a4844;
        }
        iVar7 = iVar7 + 1;
      } while (iVar7 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,uVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00e68,0xba,uVar1);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar5 = 0;
LAB_ffffff80085a4844:
      cmdqRecWrite(param_2,lVar5,param_3 << 5,0x20);
    }
    uVar3 = 0;
  }
  else {
    dprec_logger_pr(0,0xffffff8008e49ece,3);
    printk(0xffffff8008e49f01,3);
    uVar3 = 0xffffffff;
  }
  return uVar3;
}
