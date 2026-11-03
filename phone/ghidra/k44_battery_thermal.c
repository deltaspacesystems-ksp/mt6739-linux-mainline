//// get_device_adc_out @ffffff800850697c

undefined2 get_device_adc_out(short param_1,long param_2)

{
  undefined1 uVar1;
  short sVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  int iVar11;

  if (param_1 == 5) {
    uVar10 = 0;
LAB_ffffff8008506ae4:
    uVar9 = 0x78d;
  }
  else {
    if (param_1 == 6) {
      uVar10 = 1;
      goto LAB_ffffff8008506ae4;
    }
    if (param_1 != 4) goto LAB_ffffff80085069e0;
    if (3 < uRamffffff80095df258) {
      printk(0xffffff8008e20a68,0xffffff8008be4f98);
    }
    uVar10 = 0;
    uVar9 = 0x7ac;
  }
  pmic_set_hk_reg_value(uVar9,uVar10);
LAB_ffffff80085069e0:
  iVar11 = 0x66;
  pmic_set_hk_reg_value(*(undefined4 *)(param_2 + 4),1);
  __const_udelay(0xa7c6);
  do {
    sVar2 = pmic_get_register_value(*(undefined4 *)(param_2 + 8));
    if (sVar2 == 1) goto LAB_ffffff8008506ac0;
    usleep_range(0x514,0x5dc);
    iVar11 = iVar11 + -1;
  } while (iVar11 != 0);
  uVar1 = *(undefined1 *)(param_2 + 2);
  uVar6 = upmu_get_reg_value(0x1108);
  uVar7 = upmu_get_reg_value(0x110a);
  uVar8 = upmu_get_reg_value(0x110c);
  printk(0xffffff8008e20a9f,0xffffff8008be4fb0,uVar1,uVar6,uVar7,uVar8);
  uVar6 = upmu_get_reg_value(0xa20);
  uVar3 = pmic_get_register_value(0x5ee);
  uVar4 = pmic_get_register_value(0x5f6);
  uVar5 = pmic_get_register_value(0x12f);
  printk(0xffffff8008e20ad6,0xffffff8008be4fb0,0xa20,uVar6,uVar3,uVar4,uVar5);
LAB_ffffff8008506ac0:
  uVar3 = pmic_get_register_value(*(undefined4 *)(param_2 + 0xc));
  return uVar3;
}


//// BattThermistorConverTemp @ffffff8008841058

int BattThermistorConverTemp(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;

  if (param_1 < iRamffffff80094b57fc) {
    if (iRamffffff80094b589c < param_1) {
      piVar2 = (int *)0xffffff80094b57fc;
      iVar6 = 0;
      iVar5 = iRamffffff80094b57fc;
      iVar3 = iRamffffff80094b57f8;
      do {
        iVar1 = *piVar2;
        if (iVar1 <= param_1) {
          iVar4 = *(int *)((long)iVar6 * 8 + -0x7ff6b4a808);
          goto LAB_ffffff80088410b4;
        }
        iVar6 = iVar6 + 1;
        iVar3 = piVar2[-1];
        piVar2 = piVar2 + 2;
        iVar5 = iVar1;
      } while (iVar6 != 0x15);
      iVar4 = 0;
      iVar1 = 0;
LAB_ffffff80088410b4:
      iVar6 = 0;
      if (iVar5 - iVar1 != 0) {
        iVar6 = ((iVar5 - param_1) * iVar4 + (param_1 - iVar1) * iVar3) / (iVar5 - iVar1);
      }
      goto LAB_ffffff80088410cc;
    }
    iVar6 = 0x3c;
  }
  else {
    iVar6 = -0x28;
  }
  iVar4 = 0;
  iVar3 = 0;
  iVar1 = 0;
  iVar5 = 0;
LAB_ffffff80088410cc:
  if (7 < iRamffffff80094b1558) {
    printk(0xffffff8008ee4313,iVar5,iVar1,param_1,iVar3,iVar4,iVar6);
  }
  return iVar6;
}


//// BattVoltToTemp @ffffff8008841230

undefined4 BattVoltToTemp(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  ulong uVar4;

  if (param_1 == 0x708) {
    iVar2 = 1;
  }
  else {
    iVar1 = 0x708 - param_1;
    iVar2 = -iVar1;
    if (-1 < iVar1) {
      iVar2 = iVar1;
    }
  }
  uVar4 = 0;
  if ((long)iVar2 != 0) {
    uVar4 = (ulong)((long)param_1 * 0x4204) / (ulong)(long)iVar2;
  }
  iVar2 = pmic_is_bif_exist();
  if (iVar2 == 0) {
    uVar4 = uVar4 & 0xffffffff;
  }
  else {
    uVar4 = (ulong)((int)uVar4 - 16000);
  }
  uVar3 = BattThermistorConverTemp(uVar4);
  if (4 < iRamffffff80094b1558) {
    printk(0xffffff8008ee4381,param_1,0x4204,0x708,param_2);
  }
  return uVar3;
}


//// force_get_tbat_internal @ffffff80088412d4

void force_get_tbat_internal(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  long unaff_x20;
  int iVar4;
  long unaff_x23;
  long lVar5;
  long lVar6;
  int iVar7;
  int iVar8;
  undefined1 auVar9 [16];
  byte bStack_1d;
  int iStack_1c;
  long alStack_18 [2];
  long lStack_8;

  bStack_1d = 0;
  lStack_8 = lRamffffff80093cff08;
  lVar6 = -0x7ff67e4118;
  iStack_1c = 0;
  lVar5 = lVar6;
  iVar3 = 0x19;
  if (cRamffffff800981c23c != '\0') {
    unaff_x20 = -0x7ff67e5000;
    unaff_x23 = -0x7ff6b4f000;
    iVar3 = iRamffffff80094b1564;
    if ((iRamffffff80094b1564 == 0xffff) &&
       (lVar5 = -0x7ff67e4118, iVar3 = iRamffffff800981c244, iRamffffff800981c240 == 0))
    goto LAB_ffffff8008841370;
  }
  do {
    while( true ) {
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      __stack_chk_fail(iVar3);
      lVar6 = lVar5;
LAB_ffffff8008841370:
      iVar2 = pmic_get_v_bat_temp();
      if (iVar2 == 0) {
        iVar8 = 0;
        iVar4 = 0;
        iVar3 = 0;
        iVar7 = 0;
      }
      else {
        iVar8 = *(int *)(lVar6 + 0x148);
        iVar4 = *(int *)(lVar6 + 0x14c);
        gauge_dev_get_current(*(undefined8 *)(lVar6 + 0x3a8),&bStack_1d,&iStack_1c);
        iStack_1c = iStack_1c / 10;
        iVar3 = -10000;
        if (bStack_1d == 0) {
          iVar3 = 10000;
        }
        iVar7 = 0;
        if (iVar3 != 0) {
          iVar7 = ((iVar4 + iVar8) * iStack_1c) / iVar3;
        }
        iVar7 = iVar2 + iVar7;
        iVar3 = BattVoltToTemp(iVar7);
      }
      if (4 < *(int *)(unaff_x23 + 0x558)) {
        printk(0xffffff8008ee43a1,iVar2,iVar7,bStack_1d,iStack_1c,iVar4,iVar3,iVar8,iVar4,0);
      }
      lVar5 = unaff_x20 + 0xee8;
      if (*(int *)(unaff_x20 + 0x13d4) != 0) break;
      *(int *)(unaff_x20 + 0x13dc) = iVar7;
      *(int *)(unaff_x20 + 0x13e0) = iStack_1c;
      *(int *)(unaff_x20 + 0x13d4) = iVar3;
      *(int *)(unaff_x20 + 0x13d8) = iVar2;
      *(int *)(unaff_x20 + 0x13e8) = iVar4;
      *(uint *)(unaff_x20 + 0x13e4) = (uint)bStack_1d;
      ktime_get_with_offset(1);
      auVar9 = ns_to_timespec();
      *(undefined1 (*) [16])(unaff_x20 + 0x13f0) = auVar9;
    }
    ktime_get_with_offset(1);
    auVar9 = ns_to_timespec();
    set_normalized_timespec
              (alStack_18,auVar9._0_8_ - *(long *)(unaff_x20 + 0x13f0),
               auVar9._8_8_ - *(long *)(unaff_x20 + 0x13f8));
    lVar6 = alStack_18[0];
    if (alStack_18[0] < 0x15) {
      iVar1 = *(int *)(unaff_x20 + 0x13d4) - iVar3;
      iVar8 = -iVar1;
      if (-1 < iVar1) {
        iVar8 = iVar1;
      }
      if (iVar8 < 5) goto LAB_ffffff80088414ac;
LAB_ffffff80088414b4:
      if (2 < *(int *)(unaff_x23 + 0x558)) {
        printk(0xffffff8008ee43d2,iVar2,iVar7,bStack_1d,iStack_1c,iVar4,iVar3,
               *(undefined4 *)(unaff_x20 + 0x13d8),*(undefined4 *)(unaff_x20 + 0x13dc),
               *(undefined4 *)(unaff_x20 + 0x13e4),*(undefined4 *)(unaff_x20 + 0x13e0),
               *(undefined4 *)(unaff_x20 + 0x13e8),*(undefined4 *)(unaff_x20 + 0x13d4));
      }
    }
    else {
LAB_ffffff80088414ac:
      if (0x39 < iVar3) goto LAB_ffffff80088414b4;
    }
    *(int *)(unaff_x20 + 0x13d4) = iVar3;
    *(int *)(unaff_x20 + 0x13d8) = iVar2;
    *(int *)(unaff_x20 + 0x13dc) = iVar7;
    *(int *)(unaff_x20 + 0x13e0) = iStack_1c;
    *(uint *)(unaff_x20 + 0x13e4) = (uint)bStack_1d;
    *(int *)(unaff_x20 + 0x13e8) = iVar4;
    *(undefined1 (*) [16])(unaff_x20 + 0x13f0) = auVar9;
    if (7 < *(int *)(unaff_x23 + 0x558)) {
      printk(0xffffff8008ee441b,iVar2,iVar7,bStack_1d,iStack_1c,iVar4,iVar3,iVar2,iVar7,
             (uint)bStack_1d,iStack_1c,iVar4,iVar3,(int)lVar6);
    }
  } while( true );
}


//// fan54005_get_chip_status @ffffff80088556f0

void fan54005_get_chip_status(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(0,&uStack_9,3,4);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// fan54005_driver_probe @ffffff8008856128

void fan54005_driver_probe(long param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;
  ulong *puVar5;
  long unaff_x20;
  long *plVar6;
  undefined4 uVar7;
  ulong unaff_x25;
  char cStack_9;
  long lStack_8;

  plVar6 = (long *)0xffffff80093cf000;
  puVar5 = (ulong *)0xffffff80094b6000;
  cStack_9 = '\0';
  lStack_8 = lRamffffff80093cff08;
  *(undefined2 *)(param_1 + 2) = 0x6a;
  uVar7 = 0xfffffffb;
  lRamffffff8009824500 = param_1;
  iVar2 = ontim_dev_debug_file_exist(0xffffff80094b6600,0);
  if (iVar2 != 0) {
    fan54005_reg_config_interface(6,0x7b);
    fan54005_reg_config_interface(6,0x7b);
    fan54005_read_byte(6,&cStack_9);
    if ((cStack_9 != '{') && (iVar2 = chr_get_debug_level(), 0 < iVar2)) {
      printk(0xffffff8008eec8e3,0xffffff8008c4b0b0,0x53c);
    }
    fan54005_hw_component_detect();
    fan54005_dump_register();
    iVar2 = is_fan54005_exist();
    lVar1 = lRamffffff8009824518;
    if (iVar2 == 0) {
      if (lRamffffff8009824518 != 0) {
        lVar3 = strlen(lRamffffff8009824518);
        strncpy(0xffffff80094b6618,lVar1,lVar3 + 1);
      }
      iVar2 = chr_get_debug_level();
      if (0 < iVar2) {
        printk(0xffffff8008eeca39,0xffffff8008c4b0b0,0x543,0xffffff80094b6618);
      }
      unaff_x20 = -0x7ff6b49bb8;
      unaff_x25 = param_1 + 0x20;
      regester_and_init_ontim_debug(0xffffff80094b6600);
      uRamffffff8009824520 = 1;
      uRamffffff80095cc40b = 1;
      puVar5 = (ulong *)devm_kmalloc(unaff_x25,0x28,0x24080c0);
      uVar7 = 0;
      if (puVar5 != (ulong *)0x0) goto LAB_ffffff80088562d8;
      uVar7 = 0xfffffff4;
    }
    else {
      iVar2 = chr_get_debug_level();
      uVar7 = 0xffffffff;
      if (0 < iVar2) {
        printk(0xffffff8008eeca59,0xffffff8008c4b0b0,0x54a);
      }
    }
  }
  while( true ) {
    plVar6 = plVar6 + 0x1e1;
    if (lStack_8 == *plVar6) break;
    __stack_chk_fail(uVar7);
LAB_ffffff80088562d8:
    puVar5[1] = unaff_x25;
    puVar5[2] = 0xffffff8008eec7b3;
    puVar5[3] = 0xffffff8008e1f773;
    puVar5[4] = 0xffffff8008eeca7c;
    lRamffffff8009824500 = param_1;
    uVar4 = charger_device_register
                      (0xffffff8008e1f773,unaff_x25,puVar5,unaff_x20 + 0x208,puVar5 + 2);
    *puVar5 = uVar4;
    if ((uVar4 == 0) || (0xfffffffffffff000 < uVar4)) {
      iVar2 = chr_get_debug_level();
      if (0 < iVar2) {
        printk(0xffffff8008eeca85,0xffffff8008c4b0b0);
      }
      uVar7 = (undefined4)*puVar5;
    }
    else {
      iVar2 = chr_get_debug_level();
      if (0 < iVar2) {
        printk(0xffffff8008eecaab,0x560);
      }
    }
  }
  return;
}
