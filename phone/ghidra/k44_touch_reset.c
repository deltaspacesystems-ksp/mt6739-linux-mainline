//// gt1x_select_addr @ffffff800880adb0

void gt1x_select_addr(void)

{
  tpd_gpio_output(0,0);
  msleep(0x14);
  tpd_gpio_output(1,*(short *)(*(long *)(*(long *)(*(long *)(lRamffffff8009800678 + 0xa8) + 0x140) +
                                        0x60) + 2) == 0x14);
  msleep(0x14);
  tpd_gpio_output(0,1);
  return;
}


//// mtk_pmx_set_mode @ffffff80083b5abc

undefined8 mtk_pmx_set_mode(undefined8 param_1,ulong param_2,ulong param_3)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  long lVar5;
  ulong uVar6;

  lVar2 = pinctrl_dev_get_drvdata();
  lVar5 = *(long *)(lVar2 + 0xe8);
  if (*(long *)(lVar5 + 0x30) == 0) {
    if (*(code **)(lVar5 + 0x158) == (code *)0x0) {
      lVar1 = (param_2 / 5) * 5;
      uVar6 = (ulong)(uint)(*(int *)(lVar5 + 0x208) +
                           (int)(param_2 / 5 << ((ulong)*(byte *)(lVar5 + 0x210) & 0x3f)));
      uVar4 = (ulong)(uint)(((int)param_2 - (int)lVar1) * 3);
      uVar3 = mtk_get_regmap(lVar2,param_2,0,lVar1,uVar4,uVar6);
      uVar3 = regmap_update_bits(uVar3,uVar6 & 0xffffffff,7 << (ulong)((uint)uVar4 & 0x1f),
                                 param_3 << (uVar4 & 0x3f));
    }
    else {
      (**(code **)(lVar5 + 0x158))(param_2 | 0x80000000,param_3);
      uVar3 = 0;
    }
  }
  else {
    uVar3 = mtk_pinctrl_update_gpio_value
                      (lVar2,param_2 & 0xffffffff,param_3 & 0xffffffff,*(undefined4 *)(lVar5 + 0x38)
                      );
  }
  return uVar3;
}


//// tpd_gpio_output @ffffff80087eec44

void tpd_gpio_output(int param_1,int param_2)

{
  undefined8 uVar1;

  mutex_lock(0xffffff80094ad870);
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec7e13,param_1,param_2);
  }
  if (param_1 == 1) {
    uVar1 = uRamffffff8009800118;
    if (param_2 != 0) {
      uVar1 = uRamffffff8009800110;
    }
  }
  else {
    uVar1 = uRamffffff8009800108;
    if (param_2 != 0) {
      uVar1 = uRamffffff80098000f8;
    }
  }
  pinctrl_select_state(uRamffffff80098000f0,uVar1);
  mutex_unlock(0xffffff80094ad870);
  return;
}


//// tpd_gpio_as_int @ffffff80087eedf8

void tpd_gpio_as_int(int param_1)

{
  mutex_lock(0xffffff80094ad870);
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec7f4c);
  }
  if (param_1 == 1) {
    pinctrl_select_state(uRamffffff80098000f0,uRamffffff8009800120);
  }
  mutex_unlock(0xffffff80094ad870);
  return;
}
