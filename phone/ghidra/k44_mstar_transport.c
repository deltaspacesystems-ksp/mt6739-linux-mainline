//// mstar_iic_write_data @ffffff80088128ec

uint mstar_iic_write_data(uint param_1,char *param_2,uint param_3)

{
  bool bVar1;
  uint uVar2;
  undefined8 uVar3;

  param_1 = param_1 & 0xff;
  if (lRamffffff80098008c0 == 0) {
    if ((uRamffffff8008b462c8 & 1) == 0) {
      return 0;
    }
    uVar3 = 0xffffff8008ed8da6;
LAB_ffffff8008812970:
    printk(uVar3);
    return 0;
  }
  param_3 = param_3 & 0xffff;
  if ((((sRamffffff80098008c8 == 0x85 || sRamffffff80098008c8 == 0xbf) ||
        (sRamffffff80098008c8 == 0x2117 || sRamffffff80098008c8 == 0x2118)) &&
      (param_1 == uRamffffff80094ae4e0)) && (cRamffffff80098008ca != '\0')) {
    if ((uRamffffff8008b462c8 & 1) == 0) {
      return 0;
    }
    uVar3 = 0xffffff8008ed8d19;
    goto LAB_ffffff8008812970;
  }
  *(short *)(*(long *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0x60) + 2) = (short)param_1;
  uVar2 = tpkit_i2c_write(param_2,param_3);
  if (param_3 == 5) {
    if (((*param_2 != 'S') || (param_2[1] != 'E')) || ((param_2[2] != 'R' || (param_2[3] != 'D'))))
    goto LAB_ffffff80088129e0;
    bVar1 = param_2[4] == 'B';
  }
  else {
    if (param_3 != 1) goto LAB_ffffff80088129e0;
    bVar1 = *param_2 == 'E';
  }
  if (bVar1) {
    return param_3;
  }
LAB_ffffff80088129e0:
  if (-1 < (int)uVar2) {
    return uVar2;
  }
  if ((uRamffffff8008b462c8 & 1) != 0) {
    printk(0xffffff8008ed8d61,uVar2,param_1,param_3);
  }
  uRamffffff80098008f8 = 0xfe;
  return uVar2;
}


//// mstar_dev_hw_reset @ffffff8008815778

void mstar_dev_hw_reset(void)

{
  if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
    printk(0xffffff8008ed9d3d);
  }
  tpd_gpio_output(0,1);
  msleep(10);
  tpd_gpio_output(0,0);
  msleep(10);
  tpd_gpio_output(0,1);
  msleep(0x19);
  if (*(char *)(lRamffffff80098008b0 + 8) != '\0') {
    uRamffffff8009801658 = 1;
    uRamffffff80094ae4f8 = 1;
  }
  return;
}


//// mstar_power_init @ffffff80088182b4

int mstar_power_init(void)

{
  int iVar1;
  undefined8 uVar2;
  int iVar3;

  iVar1 = regulator_set_voltage
                    (*(undefined8 *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0x4918),2800000);
  if (iVar1 == 0) {
    iVar1 = regulator_enable(*(undefined8 *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0x4918));
    if (iVar1 == 0) {
      if ((uRamffffff8008b462c8 >> 2 & 1) == 0) {
        return 0;
      }
      iVar3 = 0;
      uVar2 = 0xffffff8008ecd41c;
    }
    else {
      if ((uRamffffff8008b462c8 & 1) == 0) {
        return iVar1;
      }
      uVar2 = 0xffffff8008ecd3e0;
      iVar3 = iVar1;
    }
  }
  else {
    if ((uRamffffff8008b462c8 & 1) == 0) {
      return iVar1;
    }
    uVar2 = 0xffffff8008ecd3a4;
    iVar3 = iVar1;
  }
  printk(uVar2,0xffffff8008c3bb28,iVar3);
  return iVar1;
}


//// mstar_get_chip_type @ffffff8008820a4c

ushort mstar_get_chip_type(void)

{
  ushort uVar1;
  ushort uVar2;

  if ((uRamffffff8008b462e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ecd4d0,0xffffff8008c3de58);
  }
  mstar_dbbus_enter_serial_debug();
  mstar_dbbus_stop_mcu();
  mstar_dbbus_iic_use_bus();
  mstar_dbbus_iic_reshape();
  mstar_dbbus_i2c_response_ack();
  mstar_set_reg_low_byte(0xfe6,1);
  mstar_set_reg_low_byte(0x1616,0xffffffaa);
  mstar_set_reg_low_byte(0x1617,0x55);
  mstar_set_reg_low_byte(0x1618,0xffffffa5);
  mstar_set_reg_low_byte(0x1619,0x5a);
  mstar_set_reg_low_byte(0x1608,0x20);
  mstar_set_reg_low_byte(0x1606,0x20);
  mstar_set_reg_low_byte(0x1607,0);
  mstar_set_reg_low_byte(0x1607,8);
  mstar_set_reg_low_byte(0x1604,1);
  mstar_set_reg_low_byte(0x1606,1);
  mstar_set_reg_low_byte(0x1610,1);
  mstar_set_reg_low_byte(0x1611,0);
  mstar_set_reg_low_byte(0x1600,5);
  mstar_set_reg_low_byte(0x1601,0);
  uVar1 = mstar_get_reg_16bit(0x160a);
  if ((ushort)(uVar1 + 0xdee9) < 2) {
    uVar2 = uVar1;
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462e8 >> 2 & 1) != 0)) {
      printk(0xffffff8008edcbfe,uVar1);
    }
LAB_ffffff8008820ba4:
    uRamffffff80098016a4 = uVar2;
    if (cRamffffff80097fb3fc == '\0') goto LAB_ffffff8008820bac;
  }
  else {
    if (((uVar1 + 0xd7ca & 0xffef) != 0) && ((uVar1 & 0xffef) != 0x5846)) {
      uVar2 = mstar_get_reg_16bit(0x1ecc);
      uVar2 = uVar2 & 0xff;
      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462e8 >> 2 & 1) != 0)) {
        printk(0xffffff8008edcbfe,uVar2);
      }
      uVar1 = uVar2;
      if (((uVar2 != 0x7a && 2 < (ushort)(uVar2 - 1)) && (uVar2 != 0x85 && uVar2 != 0xbf)) &&
         (uVar2 != 0)) {
        uVar1 = 0xbf;
      }
      goto LAB_ffffff8008820ba4;
    }
    uRamffffff80098016a4 = uVar1;
    if (cRamffffff80097fb3fc == '\0') {
      uVar1 = 0xbf;
      goto LAB_ffffff8008820bac;
    }
    if ((uRamffffff8008b462e8 >> 2 & 1) != 0) {
      printk(0xffffff8008edcbfe,uVar1);
      uVar2 = uRamffffff80098016a4;
      uVar1 = 0xbf;
      goto LAB_ffffff8008820ba4;
    }
    uVar1 = 0xbf;
  }
  if ((uRamffffff8008b462e8 >> 2 & 1) != 0) {
    printk(0xffffff8008edcc29,uRamffffff80098016a4);
  }
LAB_ffffff8008820bac:
  if ((uRamffffff8008b462e8 >> 2 & 1) != 0) {
    printk(0xffffff8008edcc56,uVar1);
  }
  mstar_dbbus_iic_not_use_bus();
  mstar_dbbus_not_stop_mcu();
  mstar_dbbus_exit_serial_debug();
  return uVar1;
}
