//// focal_core_module_init @ffffff8008fe99ec

int focal_core_module_init(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;

  if ((uRamffffff8008b46228 >> 2 & 1) != 0) {
    printk(0xffffff8008ec9d57,0xffffff8008c39a20);
  }
  lRamffffff80098005a8 = kmem_cache_alloc_trace(uRamffffff8009597958,0x24080c0,0x418);
  if (lRamffffff80098005a8 == 0) {
    iVar2 = -0xc;
    if ((uRamffffff8008b46228 & 1) != 0) {
      printk(0xffffff8008ecbace,0xffffff8008c39a20);
    }
  }
  else {
    lVar3 = of_find_compatible_node(0,0,0xffffff8008ec8723);
    if (lVar3 == 0) {
      if ((uRamffffff8008b46228 & 1) != 0) {
        printk(0xffffff8008ecbafe,0xffffff8008c39a20);
      }
    }
    else {
      for (lVar4 = of_get_next_child(lVar3,0); lVar4 != 0; lVar4 = of_get_next_child(lVar3,lVar4)) {
        iVar2 = of_device_is_compatible(lVar4,0xffffff8008ecbb2a);
        lVar1 = lRamffffff80098005a8;
        if (iVar2 != 0) {
          *(long *)(lRamffffff80098005a8 + 0xe8) = lVar4;
          *(undefined8 *)(lVar1 + 0xf0) = 0xffffff80094ade08;
          iVar2 = huawei_ts_chip_register();
          if (iVar2 != 0) {
            if ((uRamffffff8008b46228 & 1) != 0) {
              printk(0xffffff8008ecbb63,0xffffff8008c39a20,iVar2);
            }
            goto LAB_ffffff8008fe9a74;
          }
          lRamffffff80097fb808 = lRamffffff80098005a8;
          if ((uRamffffff8008b46228 >> 2 & 1) == 0) {
            return 0;
          }
          uVar5 = 0xffffff8008ecbb8f;
          goto LAB_ffffff8008fe9bd0;
        }
      }
      if ((uRamffffff8008b46228 & 1) != 0) {
        printk(0xffffff8008ecbb2e,0xffffff8008c39a20,0xffffff8008ecbb2a);
      }
    }
    iVar2 = -0x16;
  }
LAB_ffffff8008fe9a74:
  kfree(lRamffffff80098005a8);
  lRamffffff80098005a8 = 0;
  if ((uRamffffff8008b46228 >> 2 & 1) != 0) {
    uVar5 = 0xffffff8008ecbba8;
LAB_ffffff8008fe9bd0:
    printk(uVar5,0xffffff8008c39a20);
  }
  return iVar2;
}


//// focal_prase_ic_config_dts @ffffff80088028a0

undefined8 focal_prase_ic_config_dts(undefined8 param_1,long param_2)

{
  focal_of_property_read_u32_default(param_1,0xffffff8008ecf835,param_2 + 0x1a0,200);
  focal_of_property_read_u32_default(param_1,0xffffff8008ecf842,param_2 + 0x174,0x38);
  focal_of_property_read_u32_default
            (param_1,0xffffff8008ecf850,*(long *)(*(long *)(param_2 + 0x140) + 0x80) + 0x180,100);
  if ((uRamffffff8008b46258 >> 2 & 1) != 0) {
    printk(0xffffff8008ecf861,0xffffff8008c3a5c0,0xffffff8008ecf998,*(undefined4 *)(param_2 + 0x1a0)
           ,0xffffff8008eabfaa,*(undefined4 *)(param_2 + 0x174),0xffffff8008ecf850,
           *(undefined4 *)(*(long *)(*(long *)(param_2 + 0x140) + 0x80) + 0x180));
  }
  return 0;
}


//// focal_power_init @ffffff80087fd95c

int focal_power_init(void)

{
  int iVar1;
  undefined8 uVar2;
  int iVar3;

  iVar1 = regulator_set_voltage
                    (*(undefined8 *)(*(long *)(lRamffffff80098005a8 + 0x140) + 0x4918),2800000);
  if (iVar1 == 0) {
    iVar1 = regulator_enable(*(undefined8 *)(*(long *)(lRamffffff80098005a8 + 0x140) + 0x4918));
    if (iVar1 == 0) {
      if ((uRamffffff8008b46228 >> 2 & 1) == 0) {
        return 0;
      }
      iVar3 = 0;
      uVar2 = 0xffffff8008ecd41c;
    }
    else {
      if ((uRamffffff8008b46228 & 1) == 0) {
        return iVar1;
      }
      uVar2 = 0xffffff8008ecd3e0;
      iVar3 = iVar1;
    }
  }
  else {
    if ((uRamffffff8008b46228 & 1) == 0) {
      return iVar1;
    }
    uVar2 = 0xffffff8008ecd3a4;
    iVar3 = iVar1;
  }
  printk(uVar2,0xffffff8008c39e00,iVar3);
  return iVar1;
}


//// focal_regulator_power_switch @ffffff80087fda34

void focal_regulator_power_switch(char param_1)

{
  int iVar1;
  long lVar2;

  if ((uRamffffff8008b46228 >> 2 & 1) != 0) {
    printk(0xffffff8008ecd44e,0xffffff8008c39e18,param_1);
  }
  if (*(long *)(*(long *)(lRamffffff80098005a8 + 0x140) + 0x4918) != 0) {
    if (param_1 == '\x01') {
      iVar1 = regulator_enable();
      if ((iVar1 != 0) && ((uRamffffff8008b46228 >> 2 & 1) != 0)) {
        printk(0xffffff8008ecd46b,iVar1);
      }
      lVar2 = 0x15;
      while (lVar2 = lVar2 + -1, lVar2 != 0) {
        __const_udelay(0x418958);
      }
    }
    else {
      iVar1 = regulator_disable();
      if ((iVar1 != 0) && ((uRamffffff8008b46228 >> 2 & 1) != 0)) {
        printk(0xffffff8008ecd49d,iVar1);
      }
      lVar2 = 0x15;
      while (lVar2 = lVar2 + -1, lVar2 != 0) {
        __const_udelay(0x418958);
      }
    }
  }
  return;
}


//// focal_hardware_reset @ffffff80087fcb70

undefined8 focal_hardware_reset(uint param_1)

{
  long lVar1;
  undefined1 uVar2;

  lVar1 = lRamffffff80098005a8;
  if (*(char *)(lRamffffff80098005a8 + 8) != '\0') {
    bRamffffff80098005b0 = bRamffffff80098005b0 | 0x10;
  }
  *(undefined1 *)(lRamffffff80098005a8 + 0xb) = 1;
  uVar2 = 0xf0;
  disable_irq_nosync(*(undefined4 *)(*(long *)(lVar1 + 0x140) + 0x30));
  tpd_gpio_output(0,1);
  msleep(10);
  tpd_gpio_output(0,0);
  msleep(0x14);
  tpd_gpio_output(0,1);
  if (param_1 < 3) {
    uVar2 = *(undefined1 *)((ulong)param_1 - 0x7ff73c6358);
  }
  if ((uRamffffff8008b46228 >> 2 & 1) != 0) {
    printk(0xffffff8008eccbb7,0xffffff8008c39cb0,uVar2);
  }
  msleep(uVar2);
  enable_irq(*(undefined4 *)(*(long *)(lRamffffff80098005a8 + 0x140) + 0x30));
  if (*(char *)(lRamffffff80098005a8 + 8) != '\0') {
    bRamffffff80098005b0 = bRamffffff80098005b0 & 0xef;
  }
  *(undefined1 *)(lRamffffff80098005a8 + 0xb) = 0;
  return 0;
}


//// focal_read_reg @ffffff80087fb3d8

void focal_read_reg(undefined1 param_1,undefined8 param_2)

{
  undefined1 uStack_1;

  uStack_1 = param_1;
  focal_read(&uStack_1,1,param_2);
  return;
}


//// goodix_i2c_test @ffffff8008807d40

void goodix_i2c_test(void)

{
  undefined4 uVar1;
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  uVar1 = goodix_i2c_read(0xffff8047,&uStack_9,1);
  if ((uRamffffff8008b46278 >> 2 & 1) != 0) {
    printk(0xffffff8008ed18e4,uStack_9,uVar1);
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar1);
  }
  return;
}


//// goodix_i2c_read @ffffff8008807c74

void goodix_i2c_read(uint param_1,undefined8 param_2,undefined2 param_3)

{
  int iVar1;
  code *pcVar2;
  undefined1 uStack_10;
  undefined1 uStack_f;
  long lStack_8;

  iVar1 = -0x13;
  param_1 = param_1 & 0xffff;
  uStack_f = (undefined1)param_1;
  lStack_8 = lRamffffff80093cff08;
  uStack_10 = (undefined1)(param_1 >> 8);
  pcVar2 = *(code **)(*(long *)(*(long *)(*(long *)(lRamffffff8009800678 + 0xa8) + 0x140) + 0x68) +
                     0x10);
  if (pcVar2 != (code *)0x0) {
    iVar1 = (*pcVar2)(&uStack_10,2,param_2,param_3,3);
    if (iVar1 < 0) {
      printk(0xffffff8008ed18b0,0xffffff8008c3aa20,0xab,param_1,param_3);
    }
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(iVar1);
  }
  return;
}
