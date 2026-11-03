//// pmic_set_register_value @ffffff80084bea20

uint pmic_set_register_value(uint param_1,ulong param_2)

{
  undefined2 uVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  
  lVar4 = (ulong)param_1 * 0xc;
  if (param_1 == *(uint *)(lVar4 + -0x7ff7427c70)) {
    uVar1 = *(undefined2 *)(lVar4 + -0x7ff7427c6c);
    uVar3 = pmic_config_interface
                      (uVar1,param_2,*(undefined2 *)(lVar4 + -0x7ff7427c6a),
                       *(undefined1 *)(lVar4 + -0x7ff7427c68));
    uVar2 = 0;
    if (uVar3 != 0) {
      printk(0xffffff8008e1cabe,0xffffff8008bd76a0,uVar3,uVar1,param_2 & 0xffffffff);
      uVar2 = uVar3 & 0xffff;
    }
  }
  else {
    printk(0xffffff8008e1caa3,0xffffff8008bd76a0);
    uVar2 = 1;
  }
  return uVar2;
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


//// pmic_config_interface @ffffff80084be938

undefined4
pmic_config_interface(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  long lVar1;
  undefined8 uVar2;
  undefined4 uVar3;
  
  lVar1 = sp_el0;
  if ((((*(int *)(lVar1 + 0x10) < 1) && (uVar2 = daif, ((uint)uVar2 >> 7 & 1) == 0)) &&
      (iRamffffff80093cff04 == 1)) && (iRamffffff800950dc88 == 0)) {
    mutex_lock(0xffffff800944f6f0);
    uVar3 = pmic_write_device(param_1,param_2,param_3,param_4);
    mutex_unlock(0xffffff800944f6f0);
  }
  else {
    uVar3 = pmic_write_device(param_1,param_2,param_3,param_4);
  }
  return uVar3;
}


//// connectivity_export_pmic_set_register_value @ffffff8008709b6c

void connectivity_export_pmic_set_register_value(void)

{
  pmic_set_register_value();
  return;
}


