//// fan54005_hw_component_detect @ffffff8008855fa4

void fan54005_hw_component_detect(void)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  byte bStack_a;
  char cStack_9;
  long lStack_8;

  plVar5 = (long *)0xffffff80093cf000;
  lVar3 = -0x7ff6b4a000;
  lStack_8 = lRamffffff80093cff08;
  bStack_a = 0;
  fan54005_read_interface(3,&bStack_a,0xffffffff,0);
  uVar2 = (uint)bStack_a;
  if (bStack_a != 0x41) goto LAB_ffffff8008856064;
  lRamffffff8009824518 = -0x7ff6b49aa8;
  uRamffffff8009824514 = 1;
  lVar4 = lVar3;
  while( true ) {
    if ((bStack_a & 0xef) == 0x41 || bStack_a == 0x94) {
      *(undefined4 *)(lVar4 + 0x5fc) = 0;
    }
    else {
      *(undefined4 *)(lVar4 + 0x5fc) = 1;
    }
    iVar1 = chr_get_debug_level();
    lVar3 = lVar4;
    if (0 < iVar1) {
      lVar3 = lVar4 + 0x448;
      printk(0xffffff8008eec9d2,*(undefined4 *)(lVar4 + 0x5fc),bStack_a);
    }
    plVar5 = plVar5 + 0x1e1;
    if (lStack_8 == *plVar5) break;
    uVar2 = __stack_chk_fail();
LAB_ffffff8008856064:
    lVar4 = lVar3;
    if (uVar2 == 0x51) {
      cStack_9 = '\0';
      fan54005_read_interface(7,&cStack_9,0xffffffff,0);
      iVar1 = chr_get_debug_level();
      if (0 < iVar1) {
        printk(0xffffff8008eec9a3,cStack_9);
      }
      if (cStack_9 == -1) {
        lRamffffff8009824518 = lVar3 + 0x590;
      }
      else {
        lRamffffff8009824518 = lVar3 + 0x5c8;
      }
    }
  }
  return;
}


//// fan54005_read_byte @ffffff8008854f6c

void fan54005_read_byte(undefined1 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  undefined1 auStack_31 [9];
  undefined2 uStack_28;
  undefined2 uStack_26;
  undefined2 uStack_24;
  undefined1 *puStack_20;
  undefined2 uStack_18;
  undefined2 uStack_16;
  undefined2 uStack_14;
  undefined8 uStack_10;
  long lStack_8;

  lVar4 = -0x7ff6b4a000;
  lStack_8 = lRamffffff80093cff08;
  auStack_31[0] = param_1;
  mutex_lock(0xffffff80094b6530,0);
  uStack_28 = *(undefined2 *)(lRamffffff8009824500 + 2);
  puStack_20 = auStack_31;
  uStack_24 = 1;
  uStack_26 = 0;
  uStack_18 = *(undefined2 *)(lRamffffff8009824500 + 2);
  uStack_16 = 1;
  uStack_14 = 1;
  uStack_10 = param_2;
  iVar2 = i2c_transfer(*(undefined8 *)(lRamffffff8009824500 + 0x18),&uStack_28,2);
  iVar3 = iVar2;
  if (iVar2 != -6) goto LAB_ffffff8008855044;
  printk(0xffffff8008eec872,*(long *)(lRamffffff8009824500 + 0x18) + 0x2ec);
  lVar5 = lVar4;
  while( true ) {
    lVar4 = lVar5 + 0x448;
    mutex_unlock(lVar5 + 0x530);
    uVar1 = 1;
    if (iVar2 != 2) {
      uVar1 = 0xffffffff;
    }
    if (lStack_8 == lRamffffff80093cff08) break;
    iVar3 = __stack_chk_fail(uVar1);
LAB_ffffff8008855044:
    lVar5 = lVar4;
    if ((iVar3 != 2) && (iVar3 = chr_get_debug_level(), 0 < iVar3)) {
      printk(0xffffff8008eec896);
    }
  }
  return;
}


//// fan54005_get_vender_code @ffffff8008855bfc

void fan54005_get_vender_code(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(3,&uStack_9,7,5);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// fan54005_get_pn @ffffff8008855c58

void fan54005_get_pn(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(3,&uStack_9,3);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// is_fan54005_exist @ffffff80088560e4

undefined4 is_fan54005_exist(void)

{
  int iVar1;

  iVar1 = chr_get_debug_level();
  if (0 < iVar1) {
    printk(0xffffff8008eeca0b,uRamffffff80094b65fc);
  }
  return uRamffffff80094b65fc;
}
