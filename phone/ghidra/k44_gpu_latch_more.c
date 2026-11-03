//// module_to_m4u_port @ffffff80085a5658

undefined4 module_to_m4u_port(int param_1)

{
  long lVar1;

  if (param_1 == 0) {
    lVar1 = 0;
  }
  else if (param_1 == 7) {
    lVar1 = 1;
  }
  else {
    if (param_1 != 10) {
      dprec_logger_pr(0,0xffffff8008e4a101,param_1);
      printk(0xffffff8008e4a0f3,param_1);
      return 0x1d;
    }
    lVar1 = 2;
  }
  return *(undefined4 *)(lVar1 * 8 + -0x7ff73ff17c);
}


//// config_display_m4u_port @ffffff80085a5824

void config_display_m4u_port(void)

{
  uint uVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  long *plVar7;
  ulong uVar8;
  undefined4 *puVar9;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_10;
  undefined4 uStack_c;
  long lStack_8;

  plVar7 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  uVar8 = 0;
  puVar9 = (undefined4 *)0xffffff8008c00e84;
  iVar3 = disp_helper_get_option(1,0);
  uVar2 = 0xffffff8008e4d675;
  if (iVar3 == 0) {
    uVar2 = 0xffffff8008e4a1b9;
  }
  uStack_20 = 0;
  uStack_1c = disp_helper_get_option(1);
  uStack_c = 0;
  uStack_18 = 0;
  uStack_10 = 1;
  lVar5 = -0x7ff7400000;
  do {
    uStack_20 = *puVar9;
    iVar3 = m4u_config_port(&uStack_20);
    lVar6 = lVar5;
    if (iVar3 == 0) goto LAB_ffffff80085a5928;
    lVar6 = lVar5 + 0xe80;
    uVar8 = (ulong)*(uint *)(lVar6 + uVar8 * 8);
    uVar4 = ddp_get_module_name(uVar8);
    dprec_logger_pr(0,0xffffff8008e4a1d8,uVar4,uVar2,iVar3);
    uVar4 = ddp_get_module_name(uVar8);
    printk(0xffffff8008e4a1c2,lVar5 + 0xe98,0x72,uVar4,uVar2,iVar3);
    iVar3 = -1;
    do {
      plVar7 = plVar7 + 0x1e1;
      if (lStack_8 == *plVar7) {
        return;
      }
      __stack_chk_fail(iVar3);
LAB_ffffff80085a5928:
      uVar1 = (int)uVar8 + 1;
      uVar8 = (ulong)uVar1;
      puVar9 = puVar9 + 2;
      lVar5 = lVar6;
    } while (uVar1 == 3);
  } while( true );
}


//// ovl_start @ffffff800857c0dc

undefined8 ovl_start(undefined8 param_1,long param_2)

{
  uint *puVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  ulong uVar5;
  int iVar6;

  lVar2 = ovl_base_addr();
  puVar1 = (uint *)(lVar2 + 0xc);
  if (param_2 == 0) {
    *puVar1 = *puVar1 | 1;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lVar2 + 4) = 0x21f2;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lVar2 + 0x24) = 0x85000001;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    iVar6 = 0;
    do {
      uVar3 = ddp_get_module_va(iVar6);
      if (uVar3 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar4 = ddp_get_module_pa(iVar6);
        lVar4 = ((ulong)puVar1 & 0xfff) + lVar4;
        goto LAB_ffffff800857c178;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar4 = 0;
LAB_ffffff800857c178:
    uVar3 = lVar2 + 4;
    iVar6 = 0;
    cmdqRecWrite(param_2,lVar4,1);
    do {
      uVar5 = ddp_get_module_va(iVar6);
      if (uVar5 == (uVar3 & 0xfffffffffffff000)) {
        lVar4 = ddp_get_module_pa(iVar6);
        lVar4 = (uVar3 & 0xfff) + lVar4;
        goto LAB_ffffff800857c1b4;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar3);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar3);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar4 = 0;
LAB_ffffff800857c1b4:
    uVar3 = lVar2 + 0x24;
    iVar6 = 0;
    cmdqRecWrite(param_2,lVar4,0x21f2,0xffffffff);
    do {
      uVar5 = ddp_get_module_va(iVar6);
      if (uVar5 == (uVar3 & 0xfffffffffffff000)) {
        lVar2 = ddp_get_module_pa(iVar6);
        lVar2 = (uVar3 & 0xfff) + lVar2;
        goto LAB_ffffff800857c1f0;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar3);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar3);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar2 = 0;
LAB_ffffff800857c1f0:
    cmdqRecWrite(param_2,lVar2,0x85000001,0xffffffff);
  }
  return 0;
}


//// ovl_get_basic_info: brak symbolu
//// RGXLoadFirmware @ffffff80086355fc

void RGXLoadFirmware(long param_1,long param_2,long param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uStack_10;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uVar2 = **(undefined8 **)(param_1 + 0x110);
  if (param_2 == 0) goto LAB_ffffff8008635650;
  iVar1 = request_firmware(&uStack_10,param_2,uVar2,0);
  uVar2 = uStack_10;
  if (iVar1 == 0) goto LAB_ffffff8008635698;
  if (param_3 != 0) goto LAB_ffffff80086356b0;
  do {
    uVar2 = **(undefined8 **)(param_1 + 0x110);
LAB_ffffff8008635650:
    iVar1 = request_firmware(&uStack_10,0xffffff8008e69b38,uVar2);
    uVar2 = uStack_10;
    if (iVar1 != 0) {
      PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e69b46,0xffffff8008c08590,
                        0xffffff8008e69b38,iVar1);
      uVar2 = 0;
    }
LAB_ffffff8008635698:
    do {
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      __stack_chk_fail(uVar2);
LAB_ffffff80086356b0:
      iVar1 = request_firmware(&uStack_10,param_3,**(undefined8 **)(param_1 + 0x110));
      uVar2 = uStack_10;
    } while (iVar1 == 0);
  } while( true );
}


//// rgx_bvnc_packed @ffffff800865cfe0

void rgx_bvnc_packed(ulong *param_1,char *param_2,uint param_3,long param_4,char *param_5,
                    ulong param_6,ulong param_7)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;

  *param_1 = (param_6 & 0xffffffff) << 0x10 | param_7 & 0xffffffff | param_4 << 0x20;
  if (param_2 != (char *)0x0) {
    pcVar2 = param_5;
    if (param_5 == (char *)0x0) {
LAB_ffffff800865d000:
      pcVar2 = param_2;
      do {
        pcVar3 = pcVar2 + 1;
        *pcVar2 = '\0';
        pcVar2 = pcVar3;
      } while (pcVar3 != param_2 + (ulong)param_3 + 1);
      return;
    }
    for (; *pcVar2 != '\0'; pcVar2 = pcVar2 + 1) {
    }
    for (; param_3 != 0; param_3 = param_3 - 1) {
      cVar1 = *param_5;
      if (cVar1 == '\0') goto LAB_ffffff800865d000;
      if ((byte)(cVar1 - 0x30U) < 10) {
        *param_2 = cVar1;
        param_2 = param_2 + 1;
      }
      param_5 = param_5 + 1;
    }
    *param_2 = '\0';
  }
  return;
}


//// rgx_bvnc_pack_hw @ffffff800865d060

void rgx_bvnc_pack_hw(undefined8 *param_1,long param_2,undefined1 *param_3,long param_4,
                     ulong param_5,ulong param_6,ulong param_7)

{
  byte bVar1;
  undefined1 *puVar2;
  long extraout_x1;
  char *pcVar4;
  long lVar5;
  uint uVar6;
  undefined1 *puVar7;
  long *plVar8;
  undefined1 uStack_10;
  undefined1 auStack_f [2];
  char cStack_d;
  char acStack_c [4];
  long lStack_8;
  undefined1 *puVar3;

  plVar8 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  puVar7 = (undefined1 *)((param_6 & 0xffffffff) << 0x10 | param_7 & 0xffffffff | param_4 << 0x20);
  *param_1 = puVar7;
  if (param_2 == 0) goto LAB_ffffff800865d10c;
  puVar7 = &uStack_10;
  uVar6 = (uint)param_5;
  if (9999 < uVar6) goto LAB_ffffff800865d124;
  pcVar4 = acStack_c;
  if ((uVar6 < 1000) && (pcVar4 = &cStack_d, uVar6 < 100)) {
    puVar3 = puVar7;
    if (9 < uVar6) {
      puVar3 = auStack_f;
    }
    pcVar4 = puVar3 + 1;
  }
  do {
    *pcVar4 = '\0';
    pcVar4[-1] = '0';
    while( true ) {
      pcVar4 = pcVar4 + -1;
      if ((uint)param_5 == 0) break;
      uVar6 = (uint)param_5 / 10;
      *pcVar4 = (char)param_5 + (char)uVar6 * -10 + '0';
      param_5 = (ulong)uVar6;
    }
    lVar5 = 0;
LAB_ffffff800865d0e4:
    puVar3 = (undefined1 *)(param_2 + lVar5);
    if ((int)param_3 == 0) {
      *puVar3 = 0;
    }
    else {
      bVar1 = puVar7[lVar5];
      param_5 = (ulong)bVar1;
      if (bVar1 != 0) break;
      param_3 = puVar3 + ((ulong)param_3 & 0xffffffff) + 1;
      do {
        puVar2 = puVar3 + 1;
        *puVar3 = 0;
        puVar3 = puVar2;
      } while (puVar2 != param_3);
    }
LAB_ffffff800865d10c:
    plVar8 = plVar8 + 0x1e1;
    if (lStack_8 == *plVar8) {
      return;
    }
    __stack_chk_fail();
    param_2 = extraout_x1;
LAB_ffffff800865d124:
    pcVar4 = puVar7 + 5;
  } while( true );
  *(byte *)(param_2 + lVar5) = bVar1;
  param_3 = (undefined1 *)(ulong)((int)param_3 - 1);
  lVar5 = lVar5 + 1;
  goto LAB_ffffff800865d0e4;
}
