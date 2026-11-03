//// usb_phy_poweron @ffffff800851a108

void usb_phy_poweron(void)

{
  __const_udelay(0x346de);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfbffffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) & 0xfffeffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffffcff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) & 0xff7fffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) | 0x40000;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffcffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfffbffff;
  __const_udelay(0x346de0);
  if (iRamffffff800945a3a0 != 0) {
    printk(0xffffff8008e28a95,0xffffff8008be76d0,0x1ff);
  }
  return;
}


//// usb_phy_recover @ffffff800851a4a8

/* WARNING: Removing unreachable block (ram,0xffffff800851ab98) */
/* WARNING: Removing unreachable block (ram,0xffffff800851aba4) */
/* WARNING: Removing unreachable block (ram,0xffffff800851abc0) */

void usb_phy_recover(void)

{
  uint uVar1;
  ulong uVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  
  __const_udelay(0x346de);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x81c) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x81c) & 0xffffefff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfbffffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) & 0xfffeffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffffdff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffffeff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfffbffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xffffff3f;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xffffffcf;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfffffffb;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xffffc3ff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xff45ffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) & 0xff7fffff;
  __const_udelay(0x346de0);
  set_usb_phy_mode(1);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) | 0x8000;
  __const_udelay(0x10c7);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf10) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf10) | 0x100;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) | 0x400;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) | 0x1000000;
  do {
    DataSynchronizationBarrier(3,1,0);
  } while ((*(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf10) & 0xff) !=
           1);
  DataSynchronizationBarrier(3,1,0);
  uVar4 = (ulong)*(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf0c);
  uVar2 = 0;
  if (uVar4 != 0) {
    uVar2 = 0x150000 / uVar4;
  }
  uVar3 = (uint)(uVar2 / 1000) & 0xff;
  uVar1 = uVar3 + 1 & 0xff;
  if (uVar2 - (long)(int)(uVar3 * 1000) < 500) {
    uVar1 = uVar3;
  }
  if (iRamffffff800945a3a0 != 0) {
    printk(0xffffff8008e28b75,0xffffff8008be7710,0x133,uVar4,uVar2,uVar1);
  }
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf00) & 0xfeffffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf10) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0xf10) & 0xfffffeff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) & 0xffff8fff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) =
       (uVar1 & 7) << 0xc |
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x814) & 0xffff7fff;
  uVar3 = get_devinfo_with_index(0x6c);
  if (((uVar3 & 0x1f) != 0) && (iRamffffff800945a3a0 != 0)) {
    printk(0xffffff8008e28bb9,0xffffff8008be7728,0x2c6,uVar3 & 0x1f);
  }
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  lVar5 = *(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38);
  *(uint *)(lVar5 + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) | 0xf0;
  if (iRamffffff800945a3a0 == 0) {
    DataSynchronizationBarrier(3,1,0);
    DataSynchronizationBarrier(3,1,0);
  }
  else {
    DataSynchronizationBarrier(3,1,0);
    DataSynchronizationBarrier(3,1,0);
    printk(0xffffff8008e28bfe,0xffffff8008be7728,0x2d0,*(undefined1 *)(lVar5 + 0x805),
           *(undefined4 *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818));
  }
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) & 0x3fffffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) | 0xc0000000;
  __const_udelay(0x53e3);
  if (iRamffffff800945a3a0 == 0) {
    DataSynchronizationBarrier(3,1,0);
    DataSynchronizationBarrier(3,1,0);
  }
  else {
    DataSynchronizationBarrier(3,1,0);
    DataSynchronizationBarrier(3,1,0);
    printk(0xffffff8008e28c29,0xffffff8008be7728,0x2dd,
           *(undefined1 *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x805),
           *(undefined4 *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818));
  }
  if (iRamffffff800945a3a0 != 0) {
    printk(0xffffff8008e28c54,0xffffff8008be7728,0x2e0);
  }
  return;
}


//// usb_enable_clock @ffffff8008519804

undefined8 usb_enable_clock(char param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
    printk(0xffffff8008e28935,0xffffff8008be76a0,0xd8,param_1,iRamffffff80095e1230,
           iRamffffff80095e122c,iRamffffff80095e1228,iRamffffff80095e1224,iRamffffff80095e1234);
  }
  uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1238);
  if (iRamffffff80095e1220 < 1) {
    if (iRamffffff800945a464 == 0) {
      if (iRamffffff800945a3a0 != 0) {
        printk(0xffffff8008e28968,0xffffff8008be76a0,0xdd);
      }
    }
    else {
      iVar1 = ___ratelimit(0xffffff800945a110,0xffffff8008be76a0);
      if (iVar1 == 0) {
        iRamffffff80095e123c = iRamffffff80095e123c + 1;
      }
      else {
        if (iRamffffff800945a3a0 != 0) {
          printk(0xffffff8008e28993,0xffffff8008be76a0,0xdd,iRamffffff80095e123c);
        }
        iRamffffff80095e123c = 0;
      }
    }
    if (param_1 != '\0') goto LAB_ffffff8008519980;
  }
  else {
    if (param_1 != '\0') {
      if (iRamffffff80095e1230 == 0) {
        iVar1 = clk_enable(uRamffffff80095e10b8);
        if (iVar1 == 0) {
          iVar1 = clk_enable(uRamffffff80095e10b0);
          if (iVar1 == 0) {
            usb_hal_dpidle_request(1);
            iRamffffff80095e1224 = iRamffffff80095e1224 + 1;
            goto LAB_ffffff8008519a58;
          }
          if (iRamffffff800945a3a0 != 0) {
            printk(0xffffff8008e289f0,0xffffff8008be76a0,0xe8);
          }
          clk_disable(uRamffffff80095e10b8);
        }
        else if (iRamffffff800945a3a0 != 0) {
          printk(0xffffff8008e289c3,0xffffff8008be76a0,0xe3);
        }
      }
      else {
LAB_ffffff8008519a58:
        iRamffffff80095e1230 = iRamffffff80095e1230 + 1;
      }
LAB_ffffff8008519980:
      iRamffffff80095e122c = iRamffffff80095e122c + 1;
      goto LAB_ffffff8008519990;
    }
    if (iRamffffff80095e1230 == 1) {
      clk_disable(uRamffffff80095e10b0);
      clk_disable(uRamffffff80095e10b8);
      usb_hal_dpidle_request(0);
      iRamffffff80095e1234 = iRamffffff80095e1234 + 1;
    }
    iRamffffff80095e1230 = iRamffffff80095e1230 - (uint)(iRamffffff80095e1230 != 0);
  }
  iRamffffff80095e1228 = iRamffffff80095e1228 + 1;
LAB_ffffff8008519990:
  _raw_spin_unlock_irqrestore(0xffffff80095e1238,uVar2);
  if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
    printk(0xffffff8008e28a15,0xffffff8008be76a0,0x106,param_1,iRamffffff80095e1230,
           iRamffffff80095e122c,iRamffffff80095e1228,iRamffffff80095e1224,iRamffffff80095e1234);
  }
  return 1;
}


//// usb_prepare_enable_clock @ffffff8008519adc

void usb_prepare_enable_clock(char param_1)

{
  if (param_1 == '\0') {
    usb_enable_clock(0);
    usb_prepare_clock(0);
  }
  else {
    usb_prepare_enable_clock_part_0();
  }
  return;
}


//// mt_usb_enable_clk @ffffff8008515dd0

void mt_usb_enable_clk(void)

{
  usb_enable_clock(1);
  return;
}


//// mt_usb_prepare_clk @ffffff8008515da0

void mt_usb_prepare_clk(void)

{
  usb_prepare_clock(1);
  return;
}


//// mt_usb_probe @ffffff8008516a10

int mt_usb_probe(long param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  long *plVar5;
  long lVar6;
  undefined1 *puVar7;
  long lVar8;
  long lVar9;
  undefined8 uVar10;
  long *plVar11;
  undefined8 uVar13;
  long *plVar12;
  
  uVar13 = *(undefined8 *)(param_1 + 0x238);
  plVar5 = (long *)kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x10);
  if (plVar5 == (long *)0x0) {
    return -0xc;
  }
  lVar1 = param_1 + 0x10;
  lVar6 = platform_device_alloc(0xffffff8008e27cf2,0xffffffff);
  if (lVar6 == 0) {
    iVar2 = -0xc;
    dev_notice(lVar1,0xffffff8008e27cfc);
    goto LAB_ffffff8008516a88;
  }
  uRamffffff80095e1048 = of_iomap(uVar13,1);
  puVar7 = (undefined1 *)devm_kmalloc(lVar1,0x40,0x24080c0);
  if (puVar7 == (undefined1 *)0x0) {
    dev_notice(lVar1,0xffffff8008e27d1c);
LAB_ffffff8008516ad8:
    iVar2 = -0xc;
  }
  else {
    lVar8 = devm_kmalloc(lVar1,0x20,0x24080c0);
    if (lVar8 == 0) goto LAB_ffffff8008516ad8;
    *puVar7 = 3;
    of_property_read_u32_array(uVar13,0xffffff8008e27d43,lVar8 + 0xe,1);
    lVar9 = of_find_property(uVar13,0xffffff8008e27d4b,0);
    *(byte *)(lVar8 + 0xc) = *(byte *)(lVar8 + 0xc) & 0xfe | lVar9 != 0;
    *(long *)(puVar7 + 0x28) = lVar8;
    uVar13 = uRamffffff800945a008;
    *(undefined8 *)(lVar6 + 0x1e8) = 0xffffff800945a008;
    *(undefined8 *)(lVar6 + 0x1f0) = uVar13;
    *(long *)(lVar6 + 0x10) = lVar1;
    *(undefined8 *)(param_1 + 0x1e8) = 0xffffff800945a008;
    *(undefined8 *)(param_1 + 0x1f0) = uVar13;
    arch_setup_dma_ops(lVar6 + 0x10,0,uVar13,0,0);
    *plVar5 = lVar1;
    plVar5[1] = lVar6;
    *(undefined8 *)(puVar7 + 0x38) = 0xffffff8008be7238;
    iVar2 = device_rename(lVar1,0xffffff8008e27d56);
    if (iVar2 != 0) {
      dev_notice(lVar1,0xffffff8008e27d5d);
    }
    *(long **)(param_1 + 0xb0) = plVar5;
    iVar2 = platform_device_add_resources
                      (lVar6,*(undefined8 *)(param_1 + 0x2b8),*(undefined4 *)(param_1 + 0x2b0));
    if (iVar2 == 0) {
      iVar2 = platform_device_add_data(lVar6,puVar7,0x40);
      if (iVar2 != 0) {
        uVar13 = 0xffffff8008e27d88;
        goto LAB_ffffff8008516bd4;
      }
      iVar2 = platform_device_add(lVar6);
      if (iVar2 != 0) {
        uVar13 = 0xffffff8008e27da5;
        goto LAB_ffffff8008516bd4;
      }
      uRamffffff800945a4d4 = 1;
      uRamffffff800945a4d0 = 0xf8;
      uRamffffff800945a4e4 = 0;
      register_usb_hal_dpidle_request(usb_dpidle_request);
      register_usb_hal_disconnect_check(trigger_disconnect_check_work);
      if (iRamffffff800945a3a0 != 0) {
        printk(0xffffff8008e27dc5,0xffffff8008be72a0,0x634);
      }
      uRamffffff80095e1050 = 0xfffffffe0;
      uRamffffff80095e1058 = 0xffffff80095e1058;
      uRamffffff80095e1060 = 0xffffff80095e1058;
      pcRamffffff80095e1068 = do_connection_work;
      init_timer_key(0xffffff80095e1070,0x200000,0,0);
      uRamffffff80095e1090 = 0xffffff80095e1050;
      uRamffffff80095e0f28 = 0xffffff80095e0f28;
      uRamffffff80095e0f30 = 0xffffff80095e0f28;
      pcRamffffff80095e1088 = delayed_work_timer_fn;
      pcRamffffff80095e0f38 = do_idle_work;
      uRamffffff80095e0f20 = 0xfffffffe0;
      init_timer_key(0xffffff80095e0f40,0x200000,0,0);
      pcRamffffff80095e0f58 = delayed_work_timer_fn;
      uRamffffff80095e0f60 = 0xffffff80095e0f20;
      if (iRamffffff800945a3a0 != 0) {
        printk(0xffffff8008e27df8,0xffffff8008be72a0,0x638);
      }
      uRamffffff80095e14fc = 0;
      uRamffffff80095e10b0 = devm_clk_get(lVar1,0xffffff8008e27e3c);
      if (uRamffffff80095e10b0 < 0xfffffffffffff001) {
        uRamffffff80095e10b8 = devm_clk_get(lVar1,0xffffff8008e27e6b);
        if (uRamffffff80095e10b8 < 0xfffffffffffff001) {
          uRamffffff80095e10c0 = devm_clk_get(lVar1,0xffffff8008e27eae);
          if (uRamffffff80095e10c0 < 0xfffffffffffff001) {
            iVar3 = usb20_phy_init_debugfs();
            if (iVar3 == 0) {
              plVar11 = (long *)0xffffff8009459ff8;
              while( true ) {
                if (*plVar11 == 0) {
                  if (iRamffffff800945a3a0 != 0) {
                    printk(0xffffff8008e27f4b,0xffffff8008be72a0,0x65a);
                  }
                  iVar2 = get_boot_mode();
                  if (iVar2 != 1) {
                    return 0;
                  }
                  if (iRamffffff800945a3a0 == 0) {
                    get_boot_mode();
                  }
                  else {
                    uVar4 = get_boot_mode();
                    printk(0xffffff8008e27f6b,0xffffff8008be72a0,0x662,uVar4);
                  }
                  uRamffffff80095e150c = 1;
                  return 0;
                }
                iVar3 = device_create_file(lVar1);
                if (iVar3 != 0) break;
                plVar11 = plVar11 + 1;
              }
              do {
                plVar12 = plVar11 + -1;
                device_remove_file(lVar1,*plVar11);
                plVar11 = plVar12;
              } while ((long *)0xffffff8009459ff7 < plVar12);
              if (iRamffffff800945a3a0 != 0) {
                iVar2 = 0;
                printk(0xffffff8008e27f26,0xffffff8008be72a0,0x656);
              }
            }
            else if (iRamffffff800945a3a0 != 0) {
              uVar10 = 0x650;
              uVar13 = 0xffffff8008e27ef9;
              goto LAB_ffffff8008516d74;
            }
          }
          else if (iRamffffff800945a3a0 != 0) {
            uVar10 = 0x64a;
            uVar13 = 0xffffff8008e27ec3;
            goto LAB_ffffff8008516d74;
          }
        }
        else if (iRamffffff800945a3a0 != 0) {
          uVar10 = 0x644;
          uVar13 = 0xffffff8008e27e7c;
          goto LAB_ffffff8008516d74;
        }
      }
      else if (iRamffffff800945a3a0 != 0) {
        uVar13 = 0xffffff8008e27e41;
        uVar10 = 0x63e;
LAB_ffffff8008516d74:
        printk(uVar13,0xffffff8008be72a0,uVar10);
      }
    }
    else {
      uVar13 = 0xffffff8008e27d6f;
LAB_ffffff8008516bd4:
      dev_notice(lVar1,uVar13);
    }
  }
  platform_device_put(lVar6);
LAB_ffffff8008516a88:
  kfree(plVar5);
  return iVar2;
}


//// musb_platform_reset @ffffff80085191f0

void musb_platform_reset(long param_1)

{
  ushort *puVar1;
  undefined8 uVar2;
  ushort uVar3;
  ulong uVar4;
  long lVar5;
  
  uVar4 = 0x204;
  lVar5 = *(long *)(param_1 + 0x3a0);
  do {
    if (cRamffffff80095e14fc == '\0') {
      uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1500);
      usb_enable_clock(1);
      if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
        printk(0xffffff8008e2788e,0xffffff8008be73a0,0x69,0xffffff8008be73a0,uVar4 & 0xffffffff);
      }
      DataSynchronizationBarrier(3,2,0);
      *(undefined2 *)(lVar5 + uVar4) = 0;
      usb_enable_clock();
      _raw_spin_unlock_irqrestore(0xffffff80095e1500,uVar2);
    }
    else {
      DataSynchronizationBarrier(3,2,0);
      *(undefined2 *)(lVar5 + uVar4) = 0;
    }
    uVar4 = uVar4 + 0x10;
  } while (uVar4 != 0x284);
  puVar1 = (ushort *)(lVar5 + 0x204);
  if (cRamffffff80095e14fc == '\0') {
    uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1500);
    usb_enable_clock(1);
    if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
      printk(0xffffff8008e2788e,0xffffff8008be7378,0x3c,0xffffff8008be7378,0x204);
    }
    uVar3 = *puVar1;
    DataSynchronizationBarrier(3,1,0);
    usb_enable_clock(0);
    _raw_spin_unlock_irqrestore(0xffffff80095e1500,uVar2);
  }
  else {
    uVar3 = *puVar1;
    DataSynchronizationBarrier(3,1,0);
  }
  if (cRamffffff80095e14fc == '\0') {
    uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1500);
    usb_enable_clock(1);
    if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
      printk(0xffffff8008e2788e,0xffffff8008be73a0,0x69,0xffffff8008be73a0,0x204);
    }
    DataSynchronizationBarrier(3,2,0);
    *puVar1 = uVar3 | 0x4600;
    usb_enable_clock(0);
    _raw_spin_unlock_irqrestore(0xffffff80095e1500,uVar2);
  }
  else {
    DataSynchronizationBarrier(3,2,0);
    *puVar1 = uVar3 | 0x4600;
  }
  puVar1 = (ushort *)(lVar5 + 0x74);
  if (cRamffffff80095e14fc == '\0') {
    uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1500);
    usb_enable_clock(1);
    if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
      printk(0xffffff8008e2788e,0xffffff8008be7378,0x3c,0xffffff8008be7378,0x74);
    }
    uVar3 = *puVar1;
    DataSynchronizationBarrier(3,1,0);
    usb_enable_clock(0);
    _raw_spin_unlock_irqrestore(0xffffff80095e1500,uVar2);
  }
  else {
    uVar3 = *puVar1;
    DataSynchronizationBarrier(3,1,0);
  }
  if (cRamffffff80095e14fc == '\0') {
    uVar2 = _raw_spin_lock_irqsave(0xffffff80095e1500);
    usb_enable_clock(1);
    if ((iRamffffff80095e14f8 != 0) && (iRamffffff800945a3a0 != 0)) {
      printk(0xffffff8008e2788e,0xffffff8008be73a0,0x69,0xffffff8008be73a0,0x74);
    }
    DataSynchronizationBarrier(3,2,0);
    *puVar1 = uVar3 | 3;
    usb_enable_clock(0);
    _raw_spin_unlock_irqrestore(0xffffff80095e1500,uVar2);
  }
  else {
    DataSynchronizationBarrier(3,2,0);
    *puVar1 = uVar3 | 3;
  }
  return;
}


//// usb_phy_savecurrent_internal @ffffff8008519d30

void usb_phy_savecurrent_internal(void)

{
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfbffffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x86c) & 0xfffeffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffffdff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x820) & 0xfffffeff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) & 0xff7fffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x818) & 0xffefffff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 8;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 0x40000;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 0xc0;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xffffffcf;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 0x10;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 4;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xffffc3ff;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) | 0xba0000;
  __const_udelay(0x346de0);
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,1,0);
  *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) =
       *(uint *)(*(long *)(*(long *)(lRamffffff80095e1510 + 0x3b8) + 0x38) + 0x868) & 0xfffffff7;
  __const_udelay(0x10c7);
  set_usb_phy_mode(0);
  return;
}


//// ddp_dsi_trigger @ffffff8008598780

void ddp_dsi_trigger(uint param_1,long param_2)

{
  uint *puVar1;
  undefined8 uVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  int unaff_w20;
  long *plVar6;
  ulong uVar7;
  int iVar8;
  ulong uVar9;
  uint uStack_50;
  undefined4 auStack_48 [16];
  long lStack_8;
  
  plVar6 = (long *)0xffffff80093cf000;
  uVar7 = (ulong)param_1;
  lStack_8 = lRamffffff80093cff08;
  dprec_logger_pr(2,0xffffff8008e4710a,0xffffff8008c00790);
  if (iRamffffff8009639114 != 0) {
    printk(0xffffff8008e47102,0xffffff8008c00790);
  }
  lVar3 = ddp_get_module_va(0x27);
  *(undefined4 *)(lVar3 + 0xf0) = 0x14;
  DataSynchronizationBarrier(3,3,0);
  uVar5 = 0xffffff8009626000;
  if (iRamffffff8009626588 == 0) {
    auStack_48[0] = 0x2c3909;
    DSI_set_cmdq(uVar7,param_2,auStack_48,1,0);
  }
  if (param_1 == 0x24) {
    unaff_w20 = 0x9626120;
    dprec_logger_pr(2,0xffffff8008e48022,(byte)*puRamffffff8009626120 & 1);
    printk(0xffffff8008e4801a,(byte)*puRamffffff8009626120 & 1);
    puVar1 = puRamffffff8009626120;
    if (param_2 == 0) {
      DataSynchronizationBarrier(3,3,0);
      uStack_50 = *puRamffffff8009626120 & 0xfffffffe;
      *puRamffffff8009626120 = uStack_50;
      DataSynchronizationBarrier(3,3,0);
      DataSynchronizationBarrier(3,3,0);
      uStack_50 = *(uint *)(lRamffffff8009626128 + 0x10) | 0x10;
      *(uint *)(lRamffffff8009626128 + 0x10) = uStack_50;
      DataSynchronizationBarrier(3,3,0);
    }
    else {
      uVar9 = (ulong)puRamffffff8009626120 & 0xfffffffffffff000;
      iVar8 = 0;
      do {
        uVar4 = ddp_get_module_va(iVar8);
        if (uVar4 == uVar9) {
          lVar3 = ddp_get_module_pa(iVar8);
          lVar3 = ((ulong)puVar1 & 0xfff) + lVar3;
          goto LAB_ffffff80085988c0;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,puVar1);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar3 = 0;
LAB_ffffff80085988c0:
      iVar8 = 0;
      cmdqRecWrite(param_2,lVar3,0,1);
      uStack_50 = 0x10;
      uVar9 = lRamffffff8009626128 + 0x10;
      do {
        uVar4 = ddp_get_module_va(iVar8);
        if (uVar4 == (uVar9 & 0xfffffffffffff000)) {
          lVar3 = ddp_get_module_pa(iVar8);
          lVar3 = (uVar9 & 0xfff) + lVar3;
          goto LAB_ffffff8008598918;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,uVar9);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar9);
      uVar9 = 0xffffff8008e456e8;
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar3 = 0;
LAB_ffffff8008598918:
      unaff_w20 = (int)uVar9;
      cmdqRecWrite(param_2,lVar3,0x10,0x10);
    }
  }
  DSI_Start(param_1,param_2);
  uVar2 = uRamffffff8009626e20;
  if (param_1 != 0x24) goto LAB_ffffff8008598be8;
  if (iRamffffff8009626588 != 0) goto LAB_ffffff8008598be8;
  if (param_2 == 0) {
    DataSynchronizationBarrier(3,3,0);
    uStack_50 = *(uint *)(lRamffffff8009626128 + 0x10) & 0xffffffef;
    *(uint *)(lRamffffff8009626128 + 0x10) = uStack_50;
    DataSynchronizationBarrier(3,3,0);
    goto LAB_ffffff8008598be8;
  }
  iVar8 = 0;
  puVar1 = puRamffffff8009626120 + 3;
  do {
    uVar7 = ddp_get_module_va(iVar8);
    if (uVar7 == ((ulong)puVar1 & 0xfffffffffffff000)) {
      lVar3 = ddp_get_module_pa(iVar8);
      lVar3 = ((ulong)puVar1 & 0xfff) + lVar3;
      goto LAB_ffffff8008598a58;
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 != 0x31);
  dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
  printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,puVar1);
  printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
  warn_slowpath_null(0xffffff8008e456e8,0xbb);
  lVar3 = 0;
LAB_ffffff8008598a58:
  cmdqRecBackupRegisterToSlot(param_2,uVar2,0,lVar3);
  unaff_w20 = 0;
  uVar5 = lRamffffff8009626128 + 0x10;
  uVar7 = uVar5 & 0xfffffffffffff000;
  uStack_50 = 0x10;
  do {
    uVar9 = ddp_get_module_va(unaff_w20);
    if (uVar9 != uVar7) goto LAB_ffffff8008598c04;
    lVar3 = ddp_get_module_pa(unaff_w20);
    lVar3 = (uVar5 & 0xfff) + lVar3;
    while( true ) {
      cmdqRecWrite(param_2,lVar3,0,uStack_50);
LAB_ffffff8008598be8:
      plVar6 = plVar6 + 0x1e1;
      if (lStack_8 == *plVar6) {
        return;
      }
      __stack_chk_fail(0);
LAB_ffffff8008598c04:
      unaff_w20 = unaff_w20 + 1;
      if (unaff_w20 != 0x31) break;
      dprec_logger_pr(0,0xffffff8008e456bf,uVar5);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar5);
      uVar5 = 0xffffff8008e456e8;
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar3 = 0;
    }
  } while( true );
}


//// DSI_Start @ffffff8008591b80

void DSI_Start(int param_1,long param_2)

{
  long lVar1;
  uint *puVar2;
  ulong uVar3;
  long lVar4;
  uint *puVar5;
  uint *puVar6;
  int unaff_w20;
  long *plVar7;
  int iVar8;
  ulong unaff_x23;
  ulong uVar9;
  uint uStack_10;
  
  puVar2 = puRamffffff8009626128;
  puVar5 = puRamffffff8009626120;
  lVar1 = lRamffffff80093cff08;
  plVar7 = (long *)0xffffff80093cf000;
  puVar6 = (uint *)0xffffff8009626000;
  if (param_1 == 0x23) {
    if (param_2 != 0) {
      iVar8 = 0;
      uVar9 = (ulong)puRamffffff8009626128 & 0xfffffffffffff000;
      do {
        uVar3 = ddp_get_module_va(iVar8);
        if (uVar3 == uVar9) {
          lVar4 = ddp_get_module_pa(iVar8);
          lVar4 = ((ulong)puVar2 & 0xfff) + lVar4;
          goto LAB_ffffff8008591c00;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,puVar2);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,puVar2);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar4 = 0;
LAB_ffffff8008591c00:
      unaff_w20 = 0;
      cmdqRecWrite(param_2,lVar4,0,1);
      unaff_x23 = (ulong)puRamffffff8009626128 & 0xfffffffffffff000;
      uStack_10 = 1;
      puVar6 = puRamffffff8009626128;
      goto LAB_ffffff8008591c34;
    }
    puVar6 = (uint *)0xffffff8009626120;
    DataSynchronizationBarrier(3,3,0);
    uStack_10 = *puRamffffff8009626128 & 0xfffffffe;
    *puRamffffff8009626128 = uStack_10;
    DataSynchronizationBarrier(3,3,0);
    uStack_10 = *puRamffffff8009626128;
    DataSynchronizationBarrier(3,3,0);
    puVar5 = puRamffffff8009626128;
  }
  else {
    if (param_2 != 0) {
      uVar9 = (ulong)puRamffffff8009626120 & 0xfffffffffffff000;
      iVar8 = 0;
      do {
        uVar3 = ddp_get_module_va(iVar8);
        if (uVar3 == uVar9) {
          lVar4 = ddp_get_module_pa(iVar8);
          lVar4 = ((ulong)puVar5 & 0xfff) + lVar4;
          goto LAB_ffffff8008591e08;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,puVar5);
      printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,puVar5);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar4 = 0;
LAB_ffffff8008591e08:
      unaff_w20 = 0;
      cmdqRecWrite(param_2,lVar4,0,1);
      puVar6 = puRamffffff8009626120;
      unaff_x23 = (ulong)puRamffffff8009626120 & 0xfffffffffffff000;
      uStack_10 = 1;
      do {
        uVar9 = ddp_get_module_va(unaff_w20);
        if (uVar9 == unaff_x23) goto LAB_ffffff8008591c44;
        unaff_w20 = unaff_w20 + 1;
        if (unaff_w20 == 0x31) goto LAB_ffffff8008591d60;
      } while( true );
    }
    DataSynchronizationBarrier(3,3,0);
    uStack_10 = *puRamffffff8009626120 & 0xfffffffe;
    *puRamffffff8009626120 = uStack_10;
    DataSynchronizationBarrier(3,3,0);
    uStack_10 = *puRamffffff8009626120;
    DataSynchronizationBarrier(3,3,0);
    puVar5 = puRamffffff8009626120;
  }
  uStack_10 = uStack_10 | 1;
  *puVar5 = uStack_10;
  DataSynchronizationBarrier(3,3,0);
LAB_ffffff8008591d38:
  plVar7 = plVar7 + 0x1e1;
  if (lVar1 == *plVar7) {
    return;
  }
  __stack_chk_fail(0);
  do {
    unaff_w20 = unaff_w20 + 1;
    if (unaff_w20 == 0x31) goto LAB_ffffff8008591d60;
LAB_ffffff8008591c34:
    uVar9 = ddp_get_module_va(unaff_w20);
  } while (uVar9 != unaff_x23);
LAB_ffffff8008591c44:
  lVar4 = ddp_get_module_pa(unaff_w20);
  lVar4 = ((ulong)puVar6 & 0xfff) + lVar4;
  goto LAB_ffffff8008591c54;
LAB_ffffff8008591d60:
  dprec_logger_pr(0,0xffffff8008e456bf,puVar6);
  printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,puVar6);
  puVar6 = (uint *)0xffffff8008e456e8;
  printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
  warn_slowpath_null(0xffffff8008e456e8,0xbb);
  lVar4 = 0;
LAB_ffffff8008591c54:
  cmdqRecWrite(param_2,lVar4,1,uStack_10);
  goto LAB_ffffff8008591d38;
}


//// ddp_dsi_start @ffffff8008599090

void ddp_dsi_start(int param_1,long param_2)

{
  uint uVar1;
  bool bVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  ulong uVar7;
  long lVar8;
  uint *puVar9;
  undefined8 uVar10;
  ulong uVar11;
  long lVar12;
  byte bVar13;
  long lVar14;
  ulong uVar15;
  ulong unaff_x22;
  uint *unaff_x23;
  long lVar16;
  uint auStack_10 [2];
  long lStack_8;
  
  uVar15 = 0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  uVar3 = disp_helper_get_option(4,0);
  uVar4 = disp_helper_get_option(5);
  dprec_logger_pr(2,0xffffff8008e4710a,0xffffff8008c007b8);
  if (iRamffffff8009639114 != 0) {
    printk(0xffffff8008e47102,0xffffff8008c007b8);
  }
  lVar14 = -0x7ff69da000;
  if (param_2 == 0) {
    DataSynchronizationBarrier(3,3,0);
    auStack_10[0] = *(uint *)(lRamffffff8009626120 + 400) | 2;
    *(uint *)(lRamffffff8009626120 + 400) = auStack_10[0];
    DataSynchronizationBarrier(3,3,0);
    DataSynchronizationBarrier(3,3,0);
    auStack_10[0] = *(uint *)(lRamffffff8009626120 + 400) & 0xfffffffe;
    *(uint *)(lRamffffff8009626120 + 400) = auStack_10[0];
    DataSynchronizationBarrier(3,3,0);
    goto LAB_ffffff80085992b0;
  }
  uVar15 = lRamffffff8009626120 + 400;
  iVar6 = 0;
  auStack_10[0] = 2;
  do {
    uVar7 = ddp_get_module_va(iVar6);
    if (uVar7 == (uVar15 & 0xfffffffffffff000)) {
      lVar8 = ddp_get_module_pa(iVar6);
      lVar8 = (uVar15 & 0xfff) + lVar8;
      goto LAB_ffffff800859916c;
    }
    iVar6 = iVar6 + 1;
  } while (iVar6 != 0x31);
  dprec_logger_pr(0,0xffffff8008e456bf,uVar15);
  printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar15);
  printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
  warn_slowpath_null(0xffffff8008e456e8,0xbb);
  lVar8 = 0;
LAB_ffffff800859916c:
  unaff_x22 = 0;
  cmdqRecWrite(param_2,lVar8,2,auStack_10[0]);
  uVar15 = lRamffffff8009626120 + 400;
  unaff_x23 = (uint *)(uVar15 & 0xfffffffffffff000);
  auStack_10[0] = 1;
LAB_ffffff80085991a0:
  iVar6 = (int)unaff_x22;
  puVar9 = (uint *)ddp_get_module_va(unaff_x22);
  if (puVar9 != unaff_x23) goto LAB_ffffff80085992ec;
  lVar8 = ddp_get_module_pa(unaff_x22);
  lVar8 = (uVar15 & 0xfff) + lVar8;
  do {
    cmdqRecWrite(param_2,lVar8,0,auStack_10[0]);
LAB_ffffff80085992b0:
    iVar6 = (int)unaff_x22;
    iVar5 = disp_helper_get_option(0x28);
    if (iVar5 != 0) {
      bVar2 = (param_1 - 0x22U & 0xfffffffd) != 0;
      uVar7 = (ulong)bVar2;
      unaff_x23 = auStack_10;
      lVar8 = 0;
LAB_ffffff8008599388:
      iVar6 = (int)unaff_x22;
      if ((int)((uint)bVar2 + (int)lVar8) <= (int)(uint)(param_1 != 0x22)) {
        iVar6 = disp_helper_get_option(0x29);
        if (iVar6 != 1) {
          iVar6 = disp_helper_get_option(0x29);
          if (iVar6 == 2) {
            if (param_2 != 0) {
              lVar16 = (uVar7 + lVar8) * 8;
              auStack_10[0] = 2;
              iVar6 = 0;
              uVar15 = *(long *)(lVar16 + lVar14 + 0x120) + 400;
              do {
                uVar11 = ddp_get_module_va(iVar6);
                if (uVar11 == (uVar15 & 0xfffffffffffff000)) {
                  lVar12 = ddp_get_module_pa(iVar6);
                  lVar12 = (uVar15 & 0xfff) + lVar12;
                  goto LAB_ffffff80085996e4;
                }
                iVar6 = iVar6 + 1;
              } while (iVar6 != 0x31);
              dprec_logger_pr(0,0xffffff8008e456bf,uVar15);
              printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar15);
              printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
              warn_slowpath_null(0xffffff8008e456e8,0xbb);
              lVar12 = 0;
LAB_ffffff80085996e4:
              cmdqRecWrite(param_2,lVar12,2,auStack_10[0]);
              auStack_10[0] = 1;
              iVar6 = 0;
              unaff_x22 = *(long *)(lVar16 + lVar14 + 0x120) + 400;
              do {
                uVar15 = ddp_get_module_va(iVar6);
                if (uVar15 == (unaff_x22 & 0xfffffffffffff000)) goto LAB_ffffff8008599460;
                iVar6 = iVar6 + 1;
              } while (iVar6 != 0x31);
              goto LAB_ffffff8008599614;
            }
            lVar16 = lVar14 + 0x120;
            lVar12 = (uVar7 + lVar8) * 8;
            DataSynchronizationBarrier(3,3,0);
            auStack_10[0] = *(uint *)(*(long *)(lVar12 + lVar16) + 400) | 2;
            *(uint *)(*(long *)(lVar12 + lVar16) + 400) = auStack_10[0];
            DataSynchronizationBarrier(3,3,0);
            auStack_10[0] = *(uint *)(*(long *)(lVar12 + lVar16) + 400);
            DataSynchronizationBarrier(3,3,0);
            bVar13 = (byte)auStack_10[0] & 0xfe;
            goto LAB_ffffff80085995ac;
          }
          goto LAB_ffffff8008599480;
        }
        if (param_2 != 0) {
          lVar16 = (uVar7 + lVar8) * 8;
          auStack_10[0] = 1;
          iVar6 = 0;
          uVar15 = *(long *)(lVar16 + lVar14 + 0x120) + 400;
          do {
            uVar11 = ddp_get_module_va(iVar6);
            if (uVar11 == (uVar15 & 0xfffffffffffff000)) {
              lVar12 = ddp_get_module_pa(iVar6);
              lVar12 = (uVar15 & 0xfff) + lVar12;
              goto LAB_ffffff8008599410;
            }
            iVar6 = iVar6 + 1;
          } while (iVar6 != 0x31);
          dprec_logger_pr(0,0xffffff8008e456bf,uVar15);
          printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar15);
          printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
          warn_slowpath_null(0xffffff8008e456e8,0xbb);
          lVar12 = 0;
LAB_ffffff8008599410:
          cmdqRecWrite(param_2,lVar12,1,auStack_10[0]);
          auStack_10[0] = 2;
          iVar6 = 0;
          unaff_x22 = *(long *)(lVar16 + lVar14 + 0x120) + 400;
          do {
            uVar15 = ddp_get_module_va(iVar6);
            if (uVar15 == (unaff_x22 & 0xfffffffffffff000)) goto LAB_ffffff8008599460;
            iVar6 = iVar6 + 1;
          } while (iVar6 != 0x31);
LAB_ffffff8008599614:
          dprec_logger_pr(0,0xffffff8008e456bf,unaff_x22);
          printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,unaff_x22);
          unaff_x22 = 0xffffff8008e456e8;
          printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
          warn_slowpath_null(0xffffff8008e456e8,0xbb);
          lVar16 = 0;
          goto LAB_ffffff8008599470;
        }
        lVar16 = lVar14 + 0x120;
        lVar12 = (uVar7 + lVar8) * 8;
        DataSynchronizationBarrier(3,3,0);
        auStack_10[0] = *(uint *)(*(long *)(lVar12 + lVar16) + 400) | 1;
        *(uint *)(*(long *)(lVar12 + lVar16) + 400) = auStack_10[0];
        DataSynchronizationBarrier(3,3,0);
        auStack_10[0] = *(uint *)(*(long *)(lVar12 + lVar16) + 400);
        DataSynchronizationBarrier(3,3,0);
        bVar13 = (byte)auStack_10[0] & 0xfd;
LAB_ffffff80085995ac:
        auStack_10[0] = CONCAT31(auStack_10[0]._1_3_,bVar13);
        *(uint *)(*(long *)(lVar12 + lVar14 + 0x120) + 400) = auStack_10[0];
        DataSynchronizationBarrier(3,3,0);
        goto LAB_ffffff80085995c4;
      }
    }
    if (param_1 == 0x24) {
      uVar10 = _ddp_dsi_start_dual(0x24,param_2);
    }
    else {
      if (param_1 == 0x22) {
        lVar8 = 0;
      }
      else {
        uVar10 = 0;
        if (param_1 != 0x23) goto LAB_ffffff80085992d0;
        lVar8 = 1;
      }
      lVar14 = lVar14 + 1000;
      uVar15 = lVar14 + lVar8 * 0x500;
      DSI_Send_ROI(param_1,param_2,uVar3,uVar4,*(undefined4 *)(lVar14 + lVar8 * 0x500),
                   *(undefined4 *)(uVar15 + 4));
      DSI_SetMode(param_1,param_2,*(undefined4 *)(uVar15 + 0x1a0));
      DSI_clk_HS_mode(param_1,param_2,1);
      uVar10 = 0;
    }
LAB_ffffff80085992d0:
    if (lStack_8 == lRamffffff80093cff08) {
      return;
    }
    __stack_chk_fail(uVar10);
LAB_ffffff80085992ec:
    unaff_x22 = (ulong)(iVar6 + 1U);
    if (iVar6 + 1U != 0x31) goto LAB_ffffff80085991a0;
    dprec_logger_pr(0,0xffffff8008e456bf,uVar15);
    printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar15);
    uVar15 = 0xffffff8008e456e8;
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar8 = 0;
  } while( true );
LAB_ffffff8008599460:
  lVar16 = ddp_get_module_pa(iVar6);
  lVar16 = (unaff_x22 & 0xfff) + lVar16;
LAB_ffffff8008599470:
  cmdqRecWrite(param_2,lVar16,0,auStack_10[0]);
LAB_ffffff8008599480:
  if (param_2 == 0) {
LAB_ffffff80085995c4:
    uVar15 = (uVar7 + lVar8) * 8;
    DataSynchronizationBarrier(3,3,0);
    auStack_10[0] = *(uint *)(*(long *)(uVar15 + lVar14 + 0x120) + 400) & 0xfffffffb;
    *(uint *)(*(long *)(uVar15 + lVar14 + 0x120) + 400) = auStack_10[0];
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    auStack_10[0] = 4;
    unaff_x22 = 0;
    uVar15 = *(long *)(lVar14 + 0x120 + (uVar7 + lVar8) * 8) + 400;
    do {
      uVar11 = ddp_get_module_va(unaff_x22);
      if (uVar11 == (uVar15 & 0xfffffffffffff000)) {
        lVar16 = ddp_get_module_pa(unaff_x22);
        lVar16 = (uVar15 & 0xfff) + lVar16;
        goto LAB_ffffff80085994d0;
      }
      uVar1 = (int)unaff_x22 + 1;
      unaff_x22 = (ulong)uVar1;
    } while (uVar1 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar15);
    printk(0xffffff8008e456a9,0xffffff8008c00678,0xba,uVar15);
    uVar15 = 0xffffff8008e456e8;
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar16 = 0;
LAB_ffffff80085994d0:
    cmdqRecWrite(param_2,lVar16,0,auStack_10[0]);
  }
  lVar8 = lVar8 + 1;
  goto LAB_ffffff8008599388;
}


//// _trigger_display_interface @ffffff80085d1550

undefined8 _trigger_display_interface(undefined4 param_1,undefined8 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  long lVar6;
  undefined8 uVar7;
  
  iVar1 = _should_wait_path_idle();
  if (iVar1 != 0) {
    lVar6 = _get_context();
    iVar1 = dpmgr_wait_event_timeout(*(undefined8 *)(lVar6 + 0xd8),0,300);
    if (iVar1 < 1) {
      primary_display_diagnose();
    }
  }
  iVar1 = _should_update_lcm();
  if (iVar1 != 0) {
    uVar2 = disp_helper_get_option(4);
    uVar3 = disp_helper_get_option(5);
    uVar4 = disp_helper_get_option(6);
    uVar5 = disp_helper_get_option(7);
    lVar6 = _get_context();
    disp_lcm_update(*(undefined8 *)(lVar6 + 0xc0),uVar2,uVar3,uVar4,uVar5,0);
  }
  iVar1 = _should_start_path();
  if (iVar1 != 0) {
    lVar6 = _get_context();
    uVar7 = *(undefined8 *)(lVar6 + 0xd8);
    uVar2 = primary_display_cmdq_enabled();
    dpmgr_path_start(uVar7,uVar2);
  }
  iVar1 = disp_helper_get_option(0x2f);
  if (((iVar1 != 0) && (iVar1 = primary_display_is_video_mode(), iVar1 != 0)) &&
     (iRamffffff8009639568 != 0)) {
    lVar6 = _get_context();
    uVar7 = *(undefined8 *)(lVar6 + 0xd0);
    lVar6 = _get_context();
    change_dsi_vfp(uVar7,*(undefined4 *)(lVar6 + 8));
    iRamffffff8009639568 = 0;
  }
  iVar1 = _should_trigger_path();
  if (iVar1 != 0) {
    if (iRamffffff8009634968 != 0) {
      lVar6 = _get_context();
      dpmgr_wait_event(*(undefined8 *)(lVar6 + 0xd8),7);
    }
    lVar6 = _get_context();
    uVar7 = *(undefined8 *)(lVar6 + 0xd8);
    uVar2 = primary_display_cmdq_enabled();
    dpmgr_path_trigger(uVar7,0,uVar2);
  }
  iVar1 = _should_set_cmdq_dirty();
  if (iVar1 != 0) {
    _cmdq_set_config_handle_dirty();
  }
  iVar1 = _should_insert_wait_frame_done_token();
  if (iVar1 != 0) {
    _cmdq_flush_config_handle(param_1,param_2,param_3);
  }
  iVar1 = _should_insert_wait_frame_done_token();
  if (iVar1 != 0) {
    _cmdq_reset_config_handle();
  }
  iVar1 = _should_set_cmdq_dirty();
  if (iVar1 != 0) {
    lVar6 = _get_context();
    _cmdq_handle_clear_dirty(*(undefined8 *)(lVar6 + 0xd0));
  }
  iVar1 = _should_insert_wait_frame_done_token();
  if (iVar1 != 0) {
    lVar6 = _get_context();
    _cmdq_insert_wait_frame_done_token_mira(*(undefined8 *)(lVar6 + 0xd0));
  }
  iVar1 = _need_lfr_check();
  if (iVar1 != 0) {
    lVar6 = _get_context();
    uVar7 = *(undefined8 *)(lVar6 + 0xd8);
    lVar6 = _get_context();
    dpmgr_path_build_cmdq(uVar7,*(undefined8 *)(lVar6 + 0xd0),0xd,0);
  }
  return 0;
}


//// primary_display_trigger_nolock.isra.46 @ffffff80085d66b8

void primary_display_trigger_nolock_isra_46(undefined4 param_1,ulong param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long *plVar11;
  ulong uVar12;
  uint uStack_10;
  int iStack_c;
  long lStack_8;
  
  lVar9 = -0x7ff73fe000;
  plVar11 = (long *)0xffffff80093cf000;
  param_2 = param_2 & 0xffffffff;
  lStack_8 = lRamffffff80093cff08;
  dprec_logger_pr(2,0xffffff8008e4710a,0xffffff8008c030b8);
  if (iRamffffff8009639114 != 0) {
    printk(0xffffff8008e47102,0xffffff8008c030b8);
  }
  uRamffffff8009639628 = sched_clock();
  if (cRamffffff8009639630 != '\0') {
    primary_display_switch_dst_mode(1);
    cRamffffff8009639630 = '\0';
  }
  uVar12 = 0xffffff8009639140;
  lVar8 = -0x7ff69c7000;
  if (iRamffffff8009639618 == 0) goto LAB_ffffff80085d6778;
  primary_display_release_fence_fake();
  lVar6 = -0x7ff69c7000;
  do {
    plVar11 = plVar11 + 0x1e1;
    if (lStack_8 == *plVar11) {
      return;
    }
    __stack_chk_fail(0);
    lVar8 = lVar6;
LAB_ffffff80085d6778:
    *(int *)(uVar12 + 0x4f4) = *(int *)(uVar12 + 0x4f4) + 1;
    piVar4 = (int *)_get_context();
    if (*piVar4 == 0xf1) {
      lVar9 = lVar9 + 0x10b8;
      dprec_logger_pr(0,0xffffff8008e547e0,lVar9);
      printk(0xffffff8008e547ca,lVar9,0x126b,lVar9);
    }
    else {
      primary_display_idlemgr_kick(lVar9 + 0x10b8,0);
      dprec_logger_start(0,0,0);
      lVar6 = _get_context();
      if ((*(int *)(lVar6 + 0x28) == 1) || (lVar6 = _get_context(), *(int *)(lVar6 + 0x28) == 5)) {
        _trigger_display_interface(param_1,_ovl_fence_release_callback,1);
      }
      else {
        lVar6 = _get_context();
        if (*(int *)(lVar6 + 0x28) == 3) {
          _trigger_display_interface(0,_ovl_fence_release_callback);
          lVar6 = _get_context();
          if (*(int *)(lVar6 + 0x18) == 0) {
            dprec_logger_pr(0,0xffffff8008e54821);
            printk(0xffffff8008e5480b,lVar9 + 0x10b8,0x127b);
          }
          else {
            if ((*(long *)(uVar12 + 0x4f8) == 0) &&
               (iVar2 = cmdqRecCreate(1,uVar12 + 0x4f8), iVar2 != 0)) {
              dprec_logger_pr(0,0xffffff8008e5489e);
              printk(0xffffff8008e54888,lVar9 + 0x10d8,0xb99);
            }
            else {
              if ((*(long *)(lVar8 + 0x640) == 0) &&
                 (iVar2 = cmdqRecCreate(0x10,lVar8 + 0x640), iVar2 != 0)) {
                dprec_logger_pr(0,0xffffff8008e5486b);
                printk(0xffffff8008e54855,lVar9 + 0x10d8,0xb86);
              }
              else {
                cmdqRecReset(*(undefined8 *)(lVar8 + 0x640));
                cmdqRecWait(*(undefined8 *)(lVar8 + 0x640),0x17);
                cmdqRecFlush(*(undefined8 *)(lVar8 + 0x640));
              }
              cmdqRecReset(*(undefined8 *)(lVar8 + 0x638));
              _cmdq_insert_wait_frame_done_token_mira(*(undefined8 *)(lVar8 + 0x638));
              uVar12 = *(ulong *)(lVar8 + 0x638);
              lVar9 = _get_context();
              param_2 = *(ulong *)(lVar9 + 0x120);
              uVar3 = disp_sync_get_output_timeline_id();
              cmdqRecBackupUpdateSlot(uVar12,param_2,uVar3,*(undefined4 *)(lVar8 + 0x478));
              lVar9 = _get_context();
              dpmgr_path_remove_memout(*(undefined8 *)(lVar9 + 0xd8),*(undefined8 *)(lVar8 + 0x638))
              ;
              cmdqRecClearEventToken(*(undefined8 *)(lVar8 + 0x638),0x17);
              _cmdq_set_config_handle_dirty_mira(*(undefined8 *)(lVar8 + 0x638));
              cmdqRecFlushAsyncCallback
                        (*(undefined8 *)(lVar8 + 0x638),_remove_memout_callback,
                         _wdma_fence_release_callback);
              lVar9 = _get_context();
              *(undefined4 *)(lVar9 + 0x18) = 0;
            }
            lVar9 = _get_context();
            *(undefined4 *)(lVar9 + 0x18) = 0;
          }
        }
        else {
          lVar9 = _get_context();
          if (*(int *)(lVar9 + 0x28) == 2) {
            lVar9 = _get_context();
            uVar12 = *(ulong *)(lVar9 + 0xe0);
            lVar9 = _get_context();
            _trigger_ovl_to_memory
                      (uVar12,*(undefined8 *)(lVar9 + 0xe8),_ovl_fence_release_callback,2);
          }
          else {
            lVar9 = _get_context();
            if (*(int *)(lVar9 + 0x28) == 4) {
              if (((int)param_2 == 0) || (iVar2 = primary_is_sec(), iVar2 != 0)) {
                trigger_decouple_mirror();
              }
              else {
                *(undefined4 *)(uVar12 + 0x350) = 1;
                __wake_up(0xffffff8009467bd0,3,1,0);
              }
            }
          }
        }
      }
      dprec_logger_done(0,0,0);
      iVar2 = disp_helper_get_option(0x20);
      if ((((iVar2 != 0) && (iVar2 = primary_display_is_video_mode(), iVar2 != 0)) &&
          ((lVar9 = _get_context(), *(int *)(lVar9 + 0x28) == 1 ||
           (lVar9 = _get_context(), *(int *)(lVar9 + 0x28) == 2)))) &&
         (piVar4 = (int *)_get_context(), *piVar4 == 0xf0)) {
        lVar9 = _get_context();
        uVar1 = *(uint *)(lVar9 + 4) / 100;
        fps_ctx_get_fps_constprop_58(&uStack_10,&iStack_c);
        lVar9 = _get_context();
        if ((*(int *)(lVar9 + 0x28) != 1) || (iStack_c != 0)) {
          if (uVar1 < uStack_10) {
            uStack_10 = uVar1;
          }
          lVar9 = _get_context();
          if (*(int *)(lVar9 + 0x28) == 2) {
            lVar9 = _get_context();
            uVar7 = *(undefined8 *)(lVar9 + 0xe0);
          }
          else {
            lVar9 = _get_context();
            uVar7 = *(undefined8 *)(lVar9 + 0xd8);
          }
          lVar5 = dpmgr_path_get_last_config(uVar7);
          lVar9 = 0;
          lVar6 = (ulong)*(uint *)(lVar5 + 0x960) * (ulong)*(uint *)(lVar5 + 0x95c) * 3;
          for (piVar4 = (int *)(lVar5 + 0x20); (int *)(lVar5 + 0x448) != piVar4;
              piVar4 = piVar4 + 0x26) {
            if (*piVar4 != 0) {
              lVar9 = lVar9 + (ulong)(((uint)piVar4[2] >> 0x14 & 7) * piVar4[0xf] * piVar4[0x10]);
            }
          }
          uVar12 = lVar9 * (ulong)uVar1;
          uVar10 = lVar6 * (ulong)uVar1 + (lVar6 + lVar9) * (ulong)uStack_10;
          lVar9 = _get_context();
          if (*(int *)(lVar9 + 0x28) == 1) {
            uVar12 = (uVar12 * 4) / 5;
            if (uVar10 < uVar12) {
              lVar9 = _get_context();
              uVar3 = *(undefined4 *)(lVar9 + 0x24);
              uVar7 = 2;
LAB_ffffff80085d69a8:
              do_primary_display_switch_mode(uVar7,uVar3,0,0,0);
            }
          }
          else if (uVar12 < (uVar10 * 4) / 5) {
            lVar9 = _get_context();
            uVar3 = *(undefined4 *)(lVar9 + 0x24);
            uVar7 = 1;
            goto LAB_ffffff80085d69a8;
          }
        }
      }
      *(undefined4 *)(lVar8 + 0x5c8) = 1;
    }
    lVar9 = lVar8 + 0x140;
    if ((1 < *(int *)(lVar8 + 0x634)) && (iVar2 = aee_kernel_Powerkey_is_press(), iVar2 != 0)) {
      aee_kernel_wdt_kick_Powkey_api(0xffffff8008e548b9,2);
      *(undefined4 *)(lVar8 + 0x634) = 0;
    }
    lVar5 = _get_context();
    lVar6 = lVar8;
    if (*(int *)(lVar5 + 0x24) != 0) {
      lVar6 = lVar8 + 0x140;
      lVar9 = lVar8 + 0x2c0;
      *(undefined4 *)(lVar9 + (ulong)*(uint *)(lVar8 + 0x5e4) * 0x10 + 0xc) = 1;
      ddp_mmp_get_events();
      lVar5 = _get_context();
      dprec_logger_frame_seq_begin
                (*(undefined4 *)(lVar5 + 0x24),
                 *(undefined4 *)(lVar9 + (ulong)*(uint *)(lVar8 + 0x5e4) * 0x10 + 8));
      *(uint *)(lVar8 + 0x5e4) = *(int *)(lVar8 + 0x5e4) + 1U & 3;
    }
  } while( true );
}


//// msdc_ungate_clock @ffffff80088a0344

void msdc_ungate_clock(long param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar2 = _raw_spin_lock_irqsave(param_1 + 0x44);
  *(int *)(param_1 + 0x54) = *(int *)(param_1 + 0x54) + 1;
  if ((*(uint *)((long)*(int *)(param_1 + 0x88) * 4 + -0x7ff67c1818) >> 10 & 1) != 0) {
    uVar1 = sp_el0;
    printk(0xffffff8008ef7364,*(int *)(param_1 + 0x88),0xffffff8008c4e288);
  }
  if (*(int *)(param_1 + 0x54) == 1) {
    msdc_clksrc_onoff(param_1);
  }
  _raw_spin_unlock_irqrestore(param_1 + 0x44,uVar2);
  return;
}


//// msdc_init_hw @ffffff80088a0534

void msdc_init_hw(long *param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long lVar4;
  int iVar5;
  uint uVar6;
  uint *puVar7;
  int iVar8;
  
  puVar7 = (uint *)param_1[0xf];
  msdc_ungate_clock();
  *puVar7 = *puVar7 | 1;
  DataSynchronizationBarrier(3,3,0);
  if ((*(char *)(*param_1 + 0x31) == '\x02') && (*(char *)((long)param_1 + 0x241) == '\x01')) {
    vcorefs_request_dvfs_opp(6,0);
    *puVar7 = *puVar7 & 0xafffffff;
    DataSynchronizationBarrier(3,3,0);
    vcorefs_request_dvfs_opp(6,0xffffffff);
  }
  *puVar7 = *puVar7 | 4;
  DataSynchronizationBarrier(3,3,0);
  iVar8 = 3;
  do {
    iVar5 = 0x3e9;
    do {
      if (iVar8 == 0) {
        msdc_dump_info((int)param_1[0x11]);
        warn_slowpath_null(0xffffff8008ef6183,0x3e2);
LAB_ffffff80088a05e4:
        msdc_clr_fifo((int)param_1[0x11]);
        puVar1 = puVar7 + 3;
        *puVar1 = *puVar1;
        DataSynchronizationBarrier(3,3,0);
        puVar7[2] = puVar7[2] & 0xfffffffe;
        DataSynchronizationBarrier(3,3,0);
        puVar2 = puVar7 + 0xc;
        puVar3 = puVar7 + 4;
        if ((*(uint *)(param_1[1] + 0x304) >> 8 & 1) == 0) {
          *puVar3 = *puVar3 & 0xfffffffd;
          DataSynchronizationBarrier(3,3,0);
          *puVar2 = *puVar2 & 0xfffffffd;
          DataSynchronizationBarrier(3,3,0);
        }
        *puVar3 = *puVar3 & (*puVar3 ^ 0xffffffff);
        DataSynchronizationBarrier(3,3,0);
        *puVar1 = *puVar1;
        DataSynchronizationBarrier(3,3,0);
        msdc_init_tune_setting(param_1);
        if ((int)param_1[0x11] == 1) {
          msdc_set_check_endbit(param_1,0);
        }
        *puVar2 = *puVar2 | 0x80000;
        DataSynchronizationBarrier(3,3,0);
        if (((uint)*(undefined8 *)(*param_1 + 0x28) >> 3 & 1) == 0) {
          uVar6 = *puVar2 & 0xffefffff;
        }
        else {
          uVar6 = *puVar2 | 0x100000;
        }
        *puVar2 = uVar6;
        DataSynchronizationBarrier(3,3,0);
        msdc_set_smt_by_id((int)param_1[0x11],1);
        msdc_set_driving_by_id((int)param_1[0x11],*param_1 + 0x10);
        msdc_set_pin_mode(param_1);
        puVar7[0x2c] = puVar7[0x2c] | 0x40000000;
        DataSynchronizationBarrier(3,3,0);
        *puVar2 = *puVar2 & 0xffffff | 0x3000000;
        DataSynchronizationBarrier(3,3,0);
        msdc_set_buswidth_isra_0(param_1[0xf],0);
        puVar7[0x2e] = puVar7[0x2e] | 2;
        DataSynchronizationBarrier(3,3,0);
        *(undefined4 *)(param_1 + 0x46) = 0;
        *(undefined4 *)(param_1 + 0x47) = 0;
        *(undefined4 *)((long)param_1 + 0x484) = 0;
        msdc_set_smpl_all(param_1,0);
        msdc_gate_clock(param_1,1);
        if ((*(uint *)((long)(int)param_1[0x11] * 4 + -0x7ff67c1818) >> 5 & 1) != 0) {
          lVar4 = sp_el0;
          printk(0xffffff8008ef7438,(int)param_1[0x11],0xffffff8008c4e2a0,0x422,lVar4 + 0x658,
                 *(undefined4 *)(lVar4 + 0x488));
        }
        return;
      }
      if ((*puVar7 >> 2 & 1) == 0) goto LAB_ffffff80088a05e4;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    iVar8 = iVar8 + -1;
    __const_udelay(0x418958);
  } while( true );
}


