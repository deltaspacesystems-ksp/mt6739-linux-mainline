//// mstar_chip_detect @ffffff8008818390

void mstar_chip_detect(long param_1)

{
  undefined1 *puVar1;
  int iVar2;
  int iVar3;
  undefined1 uStack_9;
  long lStack_8;

  iVar3 = -5;
  lStack_8 = lRamffffff80093cff08;
  iVar2 = ontim_dev_debug_file_exist(0xffffff80094ae538,0);
  puVar1 = puRamffffff80098008b0;
  if (iVar2 != 0) {
    *(long *)(puRamffffff80098008b0 + 0x140) = param_1;
    if ((param_1 == 0) && (lRam0000000000000078 == 0)) {
      iVar3 = -0xc;
      if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008ecd6a8,0xffffff8008c3bb40);
      }
    }
    else {
      iVar3 = mstar_parse_ic_config_dts(*(undefined8 *)(puVar1 + 0xe8));
      puVar1 = puRamffffff80098008b0;
      if (iVar3 == 0) {
        *(undefined8 *)(*(long *)(*(long *)(puRamffffff80098008b0 + 0x140) + 0x78) + 0x238) =
             *(undefined8 *)(puRamffffff80098008b0 + 0xe8);
        uRamffffff80098008c0 = *(undefined8 *)(*(long *)(puVar1 + 0x140) + 0x60);
        puVar1[2] = 0;
        puRamffffff80098008b0[3] = 0;
        *puRamffffff80098008b0 = 0;
        mstar_mutex_init();
        tpd_gpio_output(0,0);
        tpd_gpio_output(1,0);
        mstar_power_init();
        msleep(0x32);
        mstar_dev_hw_reset();
        uStack_9 = 0;
        if (*(long *)(param_1 + 0x60) == 0) {
          iVar3 = -0x13;
          if ((uRamffffff8008b462c8 & 1) != 0) {
            printk(0xffffff8008edaccc,0xffffff8008c3bb58);
          }
        }
        else {
          *(short *)(*(long *)(param_1 + 0x60) + 2) =
               (short)*(undefined4 *)(puRamffffff80098008b0 + 0x174);
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008edacf1,0xffffff8008c3bb58,
                   *(undefined2 *)(*(long *)(param_1 + 0x60) + 2));
          }
          uStack_9 = 0x37;
          iVar2 = mstar_iic_write_data(uRamffffff80094ae4e4,&uStack_9,1);
          if (-1 < iVar2) {
            if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
              printk(0xffffff8008edad48,0xffffff8008c3bb58);
            }
            strncpy(puRamffffff80098008b0 + 0x1c,0xffffff8008ed8ca5,0x1f);
            uRamffffff8009800179 = 1;
            if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
              printk(0xffffff8008edada0,0xffffff8008c3bb40);
            }
            regester_and_init_ontim_debug(0xffffff80094ae538);
            goto LAB_ffffff800881865c;
          }
          if ((uRamffffff8008b462c8 & 1) != 0) {
            printk(0xffffff8008edad11,0xffffff8008c3bb58,iVar2,0);
          }
          msleep(0x32);
          iVar3 = iVar2;
        }
        if ((uRamffffff8008b462c8 & 1) != 0) {
          printk(0xffffff8008edad71,0xffffff8008c3bb40,iVar3);
        }
        mstar_regulator_power_switch(0);
      }
      else if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008ecd6f5,0xffffff8008c3bb40,iVar3);
      }
    }
    if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
      printk(0xffffff8008edadcb,0xffffff8008c3bb40);
    }
  }
LAB_ffffff800881865c:
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(iVar3);
  }
  return;
}
