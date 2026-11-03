//// mtk_pmx_set_mux @ffffff80083b5d54

undefined8 mtk_pmx_set_mux(undefined8 param_1,uint param_2,ulong param_3)

{
  char cVar1;
  long lVar2;
  long *plVar3;
  ulong extraout_x9;
  long extraout_x10;
  long *extraout_x11;

  lVar2 = pinctrl_dev_get_drvdata();
  cVar1 = mtk_pctrl_is_function_valid_isra_3
                    (*(undefined8 *)(lVar2 + 0xe8),
                     *(undefined4 *)(*(long *)(lVar2 + 0xc0) + (param_3 & 0xffffffff) * 0x18 + 0x10)
                     ,param_2);
  if (cVar1 == '\0') {
    dev_err(*(undefined8 *)(extraout_x10 + 0xb0),0xffffff8008de857a,param_2,param_3 & 0xffffffff);
  }
  else {
    for (plVar3 = *(long **)(*extraout_x11 + (extraout_x9 & 0xffffffff) * 0x28 + 0x20);
        (plVar3 != (long *)0x0 && (*plVar3 != 0)); plVar3 = plVar3 + 2) {
      if (param_2 == *(byte *)(plVar3 + 1)) {
        mtk_pmx_set_mode(param_1,extraout_x9 & 0xffffffff,param_2 & 0xff);
        return 0;
      }
    }
  }
  return 0xffffffea;
}


//// mtk_pinctrl_set_gpio_value @ffffff80083b56a8

undefined8
mtk_pinctrl_set_gpio_value(long param_1,int param_2,char param_3,int param_4,int *param_5)

{
  uint uVar1;
  int iVar2;

  iVar2 = 0;
  while( true ) {
    if (param_4 <= iVar2) {
      return 0xffffffff;
    }
    if (*param_5 == param_2) break;
    iVar2 = iVar2 + 1;
    param_5 = param_5 + 3;
  }
  if (param_5 == (int *)0x0) {
    return 0xffffffff;
  }
  uVar1 = (uint)*(byte *)(*(long *)(param_1 + 0xe8) + 0x212);
  if (param_3 == '\0') {
    uVar1 = uVar1 << 1;
  }
  regmap_write(*(undefined8 *)(param_1 + ((ulong)*(byte *)((long)param_5 + 10) + 2) * 8),
               uVar1 + param_5[1],1L << ((ulong)*(byte *)(param_5 + 2) & 0x3f));
  return 0;
}


//// mtk_pinctrl_set_gpio_direction @ffffff80083b5738

void mtk_pinctrl_set_gpio_direction(void)

{
  mtk_pinctrl_set_gpio_value();
  return;
}


//// goodix_chip_reset @ffffff800880ae10

void goodix_chip_reset(void)

{
  undefined2 auStack_10 [4];
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  printk(0xffffff8008ed2d48,0xffffff8008c3ae40,0x7aa);
  gt1x_select_addr();
  msleep(10);
  tpd_gpio_output(1,0);
  msleep(0x3a);
  tpd_gpio_as_int(1);
  auStack_10[0] = 0xaa;
  printk(0xffffff8008ed2d66,0xffffff8008c3ae58,0x14f);
  goodix_i2c_write(0xffff8041,auStack_10,1);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail();
  }
  return;
}


//// goodix_chip_detect @ffffff800880b1ec

int goodix_chip_detect(long param_1)

{
  long *plVar1;
  long *plVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;

  iVar5 = -5;
  iVar4 = ontim_dev_debug_file_exist(0xffffff80094ae290);
  if (iVar4 != 0) {
    if (param_1 == 0) {
      iVar5 = -0xc;
      if ((uRamffffff8008b46278 & 1) != 0) {
        printk(0xffffff8008ecd6a8,0xffffff8008c3aea8);
      }
    }
    else {
      printk(0xffffff8008ed2f51,0xffffff8008c3aea8,0x8e9);
      plVar1 = plRamffffff8009800678;
      plRamffffff8009800678[0x15] = (long)puRamffffff8009800680;
      plVar1[0x16] = param_1;
      *plVar1 = *(long *)(param_1 + 0x78);
      puVar3 = puRamffffff8009800680;
      plVar1 = plRamffffff8009800678;
      *(long *)(puRamffffff8009800680 + 0x140) = param_1;
      *(undefined8 *)(*plVar1 + 0x238) = *(undefined8 *)(puVar3 + 0xe8);
      iVar4 = goodix_prase_ic_config_dts(*(undefined8 *)(puVar3 + 0xe8));
      if ((iVar4 != 0) && ((uRamffffff8008b46278 & 1) != 0)) {
        printk(0xffffff8008ecd6f5,0xffffff8008c3aea8,iVar4);
      }
      *(short *)(*(long *)(param_1 + 0x60) + 2) =
           (short)*(undefined4 *)(puRamffffff8009800680 + 0x174);
      puRamffffff8009800680[2] = 0;
      puRamffffff8009800680[3] = 0;
      *puRamffffff8009800680 = 0;
      plVar2 = plRamffffff8009800678;
      plVar1 = plRamffffff8009800678 + 0x10;
      plRamffffff8009800678[0x18] = (long)goodix_i2c_read;
      plVar2[0x17] = (long)goodix_i2c_write;
      plVar2[0x19] = (long)goodix_chip_reset;
      plVar2[0x1a] = (long)goodix_send_cmd;
      plVar2[0x1b] = (long)goodix_send_cfg;
      plVar2[0x1c] = (long)goodix_i2c_read_dbl_check;
      plVar2[0x1d] = (long)goodix_read_version;
      plVar2[0x1e] = (long)goodix_parse_cfg_data;
      *(undefined1 *)((long)plVar2 + 0x4c2) = 1;
      __mutex_init(plVar1,0xffffff8008ed2f6f,0xffffff80098007b0);
      __mutex_init(plRamffffff8009800678 + 0xb,0xffffff8008ed2f85,0xffffff80098007b0);
      __mutex_init(0xffffff8009800690,0xffffff8008ecb938,0xffffff80098007b0);
      printk(0xffffff8008ed2fac,0xffffff8008c3aea8,0x90e,0xffffff8008ed2f9b);
      printk(0xffffff8008ed2fd1,0xffffff8008c3aea8,0x90f,
             *(undefined2 *)(*(long *)(param_1 + 0x60) + 2));
      msleep(0x14);
      hw_ts_power_switch(1);
      msleep(100);
      iVar5 = goodix_chip_reset();
      if ((iVar5 < 0) || (iVar5 = goodix_i2c_test(), iVar5 < 0)) {
        hw_ts_power_switch(0);
      }
      else {
        if ((uRamffffff8008b46278 >> 2 & 1) != 0) {
          printk(0xffffff8008ed2ffd,0xffffff8008c3aea8);
        }
        if ((uRamffffff8008b46278 >> 2 & 1) != 0) {
          printk(0xffffff8008ed3021,0xffffff8008c3aea8);
        }
        iVar5 = 0;
        regester_and_init_ontim_debug(0xffffff80094ae290);
      }
    }
  }
  return iVar5;
}


//// hw_ts_power_switch @ffffff80087eece4

void hw_ts_power_switch(int param_1)

{
  int iVar1;
  undefined8 uVar2;

  tpd_gpio_output(0,0);
  tpd_gpio_output(1,0);
  msleep(0x14);
  if (param_1 == 0) {
    iVar1 = regulator_disable(uRamffffff8009800128);
    if (iVar1 == 0) {
      if ((uRamffffff8008b461e8 >> 2 & 1) == 0) {
        return;
      }
      uVar2 = 0xffffff8008ec7ef6;
    }
    else {
      if ((uRamffffff8008b461e8 & 1) == 0) {
        return;
      }
      uVar2 = 0xffffff8008ec7ecc;
    }
  }
  else if (param_1 == 1) {
    iVar1 = regulator_set_voltage(uRamffffff8009800128,2800000);
    if ((iVar1 != 0) && ((uRamffffff8008b461e8 & 1) != 0)) {
      printk(0xffffff8008ec7e4b);
    }
    iVar1 = regulator_enable(uRamffffff8009800128);
    if (iVar1 == 0) {
      if ((uRamffffff8008b461e8 >> 2 & 1) == 0) {
        return;
      }
      uVar2 = 0xffffff8008ec7ea2;
    }
    else {
      if ((uRamffffff8008b461e8 & 1) == 0) {
        return;
      }
      uVar2 = 0xffffff8008ec7e79;
    }
  }
  else {
    if ((uRamffffff8008b461e8 & 1) == 0) {
      return;
    }
    uVar2 = 0xffffff8008ec7f21;
  }
  printk(uVar2);
  return;
}


//// mtk_btif_probe @ffffff800870a40c

undefined8 mtk_btif_probe(long param_1)

{
  if (1 < iRamffffff800948fee0) {
    hal_log_print(0xffffff8008ea30fa,0xffffff8008c224a8);
  }
  lRamffffff80094902c8 = param_1 + 0x10;
  *(undefined8 *)(param_1 + 0xb0) = 0xffffff800948fee8;
  hal_btif_clk_get_and_prepare(param_1);
  return 0;
}


//// ccci_platform_init @ffffff8008704458

undefined8 ccci_platform_init(void)

{
  undefined8 uVar1;

  uVar1 = of_find_compatible_node(0,0,0xffffff8008e06532);
  uRamffffff80096a20b8 = of_iomap(uVar1,0);
  ccci_dump_write(0xffffffff,0,1,0xffffff8008ea06e9,0,uRamffffff80096a20b8);
  if (iRamffffff8009481c28 == 1) {
    printk(0xffffff8008ea0701,0,uRamffffff80096a20b8);
  }
  return 0;
}


//// ccci_md_start @ffffff80086e0c64

void ccci_md_start(uint param_1)

{
  long lVar1;

  if ((param_1 & 0xff) < 5) {
    lVar1 = *(long *)((long)(int)(param_1 & 0xff) * 8 + -0x7ff6960890);
  }
  else {
    lVar1 = 0;
  }
  (**(code **)(*(long *)(lVar1 + 0x10) + 8))();
  return;
}
