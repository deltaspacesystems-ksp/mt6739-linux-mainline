//// mt_cpufreq_dts_map @ffffff800844ca68

undefined8 mt_cpufreq_dts_map(void)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;

  lVar1 = of_find_compatible_node(0,0,0xffffff8008dfa463);
  if (lVar1 == 0) {
    uVar2 = 0xffffff8008dfb810;
    uVar3 = 0x1c3;
LAB_ffffff800844caa4:
    aee_kernel_exception_api(0xffffff8008dfb740,uVar3,0,0xffffff8008dfb738,uVar2);
    uVar2 = 0xffffffed;
  }
  else {
    lRamffffff8009442898 = of_iomap(lVar1,0);
    if (lRamffffff8009442898 == 0) {
      uVar2 = 0xffffff8008dfb822;
      uVar3 = 0x1c7;
    }
    else {
      lVar1 = of_find_compatible_node(0,0,0xffffff8008dfb889);
      if (lVar1 == 0) {
        uVar2 = 0xffffff8008dfb835;
        uVar3 = 0x1cc;
        goto LAB_ffffff800844caa4;
      }
      lRamffffff8009442888 = of_iomap(lVar1,0);
      if (lRamffffff8009442888 == 0) {
        uVar2 = 0xffffff8008dfb84b;
        uVar3 = 0x1d0;
      }
      else {
        lVar1 = of_find_compatible_node(0,0,0xffffff8008dfb89e);
        if (lVar1 == 0) {
          uVar2 = 0xffffff8008dfb862;
          uVar3 = 0x1d5;
          goto LAB_ffffff800844caa4;
        }
        lRamffffff8009442890 = of_iomap(lVar1,0);
        if (lRamffffff8009442890 != 0) {
          return 0;
        }
        uVar2 = 0xffffff8008dfb875;
        uVar3 = 0x1d9;
        lRamffffff8009442890 = 0;
      }
    }
    aee_kernel_exception_api(0xffffff8008dfb740,uVar3,0,0xffffff8008dfb738,uVar2);
    uVar2 = 0xfffffff4;
  }
  return uVar2;
}


//// _cpu_clock_switch @ffffff800844c750

void _cpu_clock_switch(undefined8 param_1,uint param_2)

{
  if (param_2 < 2) {
    *puRamffffff8009442888 = (param_2 & 0xf) << 8 | *puRamffffff8009442888 & 0xfffff0ff;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009442890 + 0x104) = *(uint *)(lRamffffff8009442890 + 0x104) & 0xffffffef;
    DataSynchronizationBarrier(3,3,0);
    return;
  }
  if (param_2 != 2) {
    return;
  }
  *(uint *)(lRamffffff8009442890 + 0x104) = *(uint *)(lRamffffff8009442890 + 0x104) | 0x10;
  DataSynchronizationBarrier(3,3,0);
  __const_udelay(0x3255);
  *puRamffffff8009442888 = *puRamffffff8009442888 & 0xfffff0ff | 0x200;
  DataSynchronizationBarrier(3,3,0);
  return;
}


//// upmu_get_rgs_chrdet @ffffff80084c5098

undefined2 upmu_get_rgs_chrdet(void)

{
  undefined2 uVar1;

  uVar1 = pmic_get_register_value(0x4c1);
  if (3 < uRamffffff80095df254) {
    printk(0xffffff8008e1f9a4,0xffffff8008bd8378,uVar1);
  }
  return uVar1;
}


//// fan54005_get_otg_status @ffffff8008855674

void fan54005_get_otg_status(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(0,&uStack_9,1,7);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// fan54005_get_boost_status @ffffff80088557d0

void fan54005_get_boost_status(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(0,&uStack_9,1,3);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// fan54005_get_fault_status @ffffff8008855828

void fan54005_get_fault_status(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(0,&uStack_9,7,0);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}
