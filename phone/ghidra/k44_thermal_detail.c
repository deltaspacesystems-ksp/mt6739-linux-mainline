//// pmic_get_v_bat_temp @ffffff80085121cc

undefined4 pmic_get_v_bat_temp(void)

{
  char cVar1;
  undefined4 uVar2;

  cVar1 = is_evb_load();
  uVar2 = 0;
  if (cVar1 == '\0') {
    uVar2 = pmic_get_auxadc_value(2);
  }
  return uVar2;
}


//// pmic_is_bif_exist @ffffff80085122f0

undefined1 pmic_is_bif_exist(void)

{
  undefined1 uVar1;

  uVar1 = func_0xffffff8008512258();
  return uVar1;
}


//// pmic_get_register_value @ffffff80084bead0

void pmic_get_register_value(ulong param_1)

{
  undefined2 uVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  uint uStack_c;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_c = 0;
  lVar2 = (param_1 & 0xffffffff) * 0xc;
  uVar1 = *(undefined2 *)(lVar2 + -0x7ff7427c6c);
  uVar4 = pmic_read_device(uVar1,&uStack_c,*(undefined2 *)(lVar2 + -0x7ff7427c6a),
                           *(undefined1 *)(lVar2 + -0x7ff7427c68));
  uVar3 = uStack_c;
  if (uVar4 != 0) {
    printk(0xffffff8008e1caec,0xffffff8008bd76b8,uVar4,uVar1);
    uVar3 = uVar4;
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar3 & 0xffff);
  }
  return;
}


//// pmic_set_hk_reg_value @ffffff80084bed08

uint pmic_set_hk_reg_value(uint param_1,undefined4 param_2)

{
  long lVar1;
  undefined8 uVar2;
  uint uVar3;
  uint uVar4;
  long lVar5;
  ulong uVar6;

  uVar6 = (ulong)param_1;
  lVar5 = uVar6 * 0xc;
  if (param_1 == *(uint *)(lVar5 + -0x7ff7427c70)) {
    lVar1 = sp_el0;
    if ((((*(int *)(lVar1 + 0x10) < 1) && (uVar2 = daif, ((uint)uVar2 >> 7 & 1) == 0)) &&
        (iRamffffff80093cff04 == 1)) && (iRamffffff800950dc88 == 0)) {
      mutex_lock(0xffffff800944f718);
      uVar4 = pmic_write_device(*(undefined2 *)(lVar5 + -0x7ff7427c6c),param_2,
                                *(undefined2 *)(lVar5 + -0x7ff7427c6a),
                                *(undefined1 *)(lVar5 + -0x7ff7427c68));
      mutex_unlock(0xffffff800944f718);
    }
    else {
      lVar5 = uVar6 * 0xc;
      uVar4 = pmic_write_device(*(undefined2 *)(lVar5 + -0x7ff7427c6c),param_2,
                                *(undefined2 *)(lVar5 + -0x7ff7427c6a),
                                *(undefined1 *)(lVar5 + -0x7ff7427c68));
    }
    uVar3 = 0;
    if (uVar4 != 0) {
      printk(0xffffff8008e1cabe,0xffffff8008bd7710,uVar4,
             *(undefined2 *)(uVar6 * 0xc + -0x7ff7427c6c),param_2);
      uVar3 = uVar4 & 0xffff;
    }
  }
  else {
    printk(0xffffff8008e1caa3,0xffffff8008bd7710);
    uVar3 = 1;
  }
  return uVar3;
}
