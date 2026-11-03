//// mt6357_get_auxadc_nolock @ffffff80085077e8

int mt6357_get_auxadc_nolock(uint param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  int iVar5;

  param_1 = param_1 & 0xff;
  if (0 < (int)(param_1 - 0xc)) {
    printk(0xffffff8008e20e7a,0xffffff8008be53a8,param_1);
    return -0x16;
  }
  iVar2 = get_device_adc_out(param_1,(ulong)param_1 * 0x10 + -0x7ff6bae4e8);
  iVar5 = iVar2;
  if (param_1 != 9) {
    lVar4 = (long)(int)param_1 * 0x10;
    if (*(char *)(lVar4 + -0x7ff6bae4e8) == '\f') {
      bVar1 = *(byte *)(lVar4 + -0x7ff6bae4e7);
      iVar3 = 0x1000;
    }
    else {
      iVar5 = 0;
      if (*(char *)(lVar4 + -0x7ff6bae4e8) != '\x0f') goto LAB_ffffff800850789c;
      bVar1 = *(byte *)(lVar4 + -0x7ff6bae4e7);
      iVar3 = 0x8000;
    }
    iVar5 = 0;
    if (iVar3 != 0) {
      iVar5 = (int)((uint)bVar1 * iVar2 * 0x708) / iVar3;
    }
  }
LAB_ffffff800850789c:
  printk(0xffffff8008e20e96,0xffffff8008be53a8,param_1,
         *(undefined1 *)((long)(int)param_1 * 0x10 + -0x7ff6bae4e6),iVar2,iVar5);
  return iVar5;
}


//// mt6357_get_auxadc_value @ffffff80085078f0

void mt6357_get_auxadc_value(uint param_1)

{
  byte bVar1;
  long lVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  undefined8 extraout_x1;
  long *plVar8;
  int iStack_c;
  long lStack_8;

  plVar8 = (long *)0xffffff80093cf000;
  param_1 = param_1 & 0xff;
  iStack_c = 0;
  lStack_8 = lRamffffff80093cff08;
  uVar7 = 0;
  if ((int)(param_1 - 0xc) < 1) goto LAB_ffffff800850796c;
  iVar6 = -0x16;
  printk(0xffffff8008e20e7a,0xffffff8008be53c8,param_1);
  iVar4 = iRamffffff80095df528;
LAB_ffffff8008507950:
  do {
    iRamffffff80095df528 = iVar4;
    plVar8 = plVar8 + 0x1e1;
    if (lStack_8 == *plVar8) {
      return;
    }
    __stack_chk_fail(iVar6);
    uVar7 = extraout_x1;
LAB_ffffff800850796c:
    lVar2 = (ulong)param_1 * 0x10;
    pmic_auxadc_lock(lVar2,uVar7);
    if (param_1 == 2) {
      mutex_lock(0xffffff8009451af0);
    }
    iVar4 = get_device_adc_out(param_1,lVar2 + -0x7ff6bae4e8);
    if (param_1 == 2) {
      mutex_unlock(0xffffff8009451af0);
    }
    pmic_auxadc_unlock();
    iVar6 = iVar4;
    if (param_1 != 9) {
      lVar2 = (ulong)param_1 * 0x10;
      if (*(char *)(lVar2 + -0x7ff6bae4e8) == '\f') {
        bVar1 = *(byte *)(lVar2 + -0x7ff6bae4e7);
        iVar5 = 0x1000;
      }
      else {
        iVar6 = 0;
        if (*(char *)(lVar2 + -0x7ff6bae4e8) != '\x0f') goto LAB_ffffff80085079f8;
        bVar1 = *(byte *)(lVar2 + -0x7ff6bae4e7);
        iVar5 = 0x8000;
      }
      iVar6 = 0;
      if (iVar5 != 0) {
        iVar6 = (int)((uint)bVar1 * iVar4 * 0x708) / iVar5;
      }
    }
LAB_ffffff80085079f8:
    if (((param_1 - 0xb & 0xff) < 2) ||
       (iVar5 = ___ratelimit(0xffffff8009451be8,0xffffff8008be53c8), iVar5 == 0)) {
      if (3 < uRamffffff80095df258) {
        printk(0xffffff8008e20f2a,0xffffff8008be53c8,0xffffff8008be53c8,param_1,
               *(undefined1 *)((long)(int)param_1 * 0x10 + -0x7ff6bae4e6),iVar4,iVar6);
      }
      if (param_1 == 2) goto LAB_ffffff8008507a78;
LAB_ffffff8008507b38:
      iVar4 = iRamffffff80095df528;
      if (param_1 == 0) {
        mt6357_auxadc_monitor_mts_regs();
        iVar4 = iRamffffff80095df528;
      }
      goto LAB_ffffff8008507950;
    }
    if (param_1 != 2) {
      printk(0xffffff8008e20e96,0xffffff8008be53c8,param_1,
             *(undefined1 *)((long)(int)param_1 * 0x10 + -0x7ff6bae4e6),iVar4,iVar6);
      goto LAB_ffffff8008507b38;
    }
    cVar3 = gauge_get_current(&iStack_c);
    if (cVar3 == '\0') {
      iStack_c = -iStack_c;
    }
    printk(0xffffff8008e20ed9,0xffffff8008be53c8,2,uRamffffff8009451b3a,iStack_c,iVar4,iVar6);
LAB_ffffff8008507a78:
    iVar4 = iRamffffff80095df528;
    if (cRamffffff80095e0740 == '\0') {
      uRamffffff80095e0838 = uRamffffff80095e0838 + 1;
      if ((iRamffffff80095df528 == 0) ||
         (((199 < iVar6 && (iVar6 - iRamffffff80095df528 < 0x65)) &&
          (iRamffffff80095df528 - iVar6 < 0x65)))) {
        iVar4 = iVar6;
        if (uRamffffff80095e0838 % 0x32 == 0) {
          wk_auxadc_battmp_dbg(iVar6);
        }
      }
      else {
        printk(0xffffff8008e20f78,iRamffffff80095df528,iVar6);
        iVar6 = wk_auxadc_battmp_dbg(iVar6);
        iVar4 = iVar6;
      }
    }
  } while( true );
}


//// pwrkey_int_handler @ffffff80084c588c

void pwrkey_int_handler(void)

{
  undefined2 uVar1;

  if (3 < uRamffffff80095df25c) {
    uVar1 = pmic_get_register_value(0x53);
    printk(0xffffff8008e1fcfe,0xffffff8008be4cb8,uVar1);
  }
  kpd_pwrkey_pmic_handler(1);
  return;
}


//// homekey_int_handler @ffffff80084c5920

void homekey_int_handler(void)

{
  undefined2 uVar1;

  if (3 < uRamffffff80095df25c) {
    uVar1 = pmic_get_register_value(0x55);
    printk(0xffffff8008e1fd68,0xffffff8008be4ce8,uVar1);
  }
  kpd_pmic_rstkey_handler(1);
  return;
}


//// kpd_init_keymap @ffffff80087ea52c

void kpd_init_keymap(long param_1)

{
  long lVar1;
  undefined4 *puVar2;

  if (iRamffffff80097fb1f4 != 0) {
    uRamffffff80094ac800 = 0xffff;
  }
  lVar1 = 0;
  puVar2 = (undefined4 *)0xffffff80097fb1fc;
  do {
    *(short *)(param_1 + lVar1) = (short)*puVar2;
    lVar1 = lVar1 + 2;
    puVar2 = puVar2 + 1;
  } while (lVar1 != 0x90);
  return;
}
