//// get_cur_phy_freq @ffffff800844c628

ulong get_cur_phy_freq(undefined8 *param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  ulong uVar5;

  uVar2 = ((*(uint *)param_1[2] & 0x3fffff) * 0x1a >> 0xe) * 1000;
  uVar5 = (ulong)uVar2;
  switch(*(uint *)param_1[2] >> 0x18 & 7) {
  case 0:
    break;
  case 1:
    uVar5 = (ulong)(uVar2 >> 1);
    break;
  case 2:
    uVar5 = (ulong)(uVar2 >> 2);
    break;
  case 3:
    uVar5 = (ulong)(uVar2 >> 3);
    break;
  default:
    uVar5 = (ulong)(uVar2 >> 4);
  }
  iVar3 = (int)uVar5;
  switch(*(uint *)param_1[4] & 0x1f) {
  case 9:
    uVar5 = (ulong)(uint)(iVar3 * 3);
  case 0xb:
    uVar5 = uVar5 >> 2;
    goto LAB_ffffff800844c6d4;
  case 10:
    uVar5 = uVar5 >> 1;
  default:
    goto LAB_ffffff800844c6d4;
  case 0x11:
    uVar5 = (ulong)(uint)(iVar3 << 2);
  case 0x14:
code_r0xffffff800844c710:
    uVar4 = (uint)uVar5;
    uVar2 = 5;
    goto code_r0xffffff800844c714;
  case 0x12:
    uVar5 = (ulong)(uint)(iVar3 * 3);
    goto code_r0xffffff800844c710;
  case 0x13:
    uVar5 = (ulong)(uint)(iVar3 << 1);
    goto code_r0xffffff800844c710;
  case 0x19:
    uVar5 = (ulong)(uint)(iVar3 * 5);
    break;
  case 0x1a:
    uVar5 = (ulong)(uint)(iVar3 << 2);
    break;
  case 0x1b:
    uVar5 = (ulong)(uint)(iVar3 * 3);
    break;
  case 0x1c:
    uVar5 = (ulong)(uint)(iVar3 << 1);
    break;
  case 0x1d:
    break;
  }
  uVar4 = (uint)uVar5;
  uVar2 = 6;
code_r0xffffff800844c714:
  uVar1 = 0;
  if (uVar2 != 0) {
    uVar1 = uVar4 / uVar2;
  }
  uVar5 = (ulong)uVar1;
LAB_ffffff800844c6d4:
  if (iRamffffff80095c3128 != 0) {
    printk(0xffffff8008dfb6d5,0xffffff8008bc3a78,*param_1,uVar5);
  }
  return uVar5;
}


//// prepare_pll_addr @ffffff800844c8dc

void prepare_pll_addr(void)

{
  long lVar1;

  lVar1 = id_to_pll_ctrl();
  *(long *)(lVar1 + 0x10) = lRamffffff8009442898 + 0x204;
  *(long *)(lVar1 + 0x20) = lRamffffff8009442888 + 0x2c;
  return;
}


//// _get_cpu_clock_switch @ffffff800844c574

uint _get_cpu_clock_switch(void)

{
  return *puRamffffff8009442888 >> 8 & 0xf;
}


//// get_clkdiv @ffffff800844c95c

undefined4 get_clkdiv(long param_1)

{
  uint uVar1;
  undefined4 uVar2;

  uVar1 = **(uint **)(param_1 + 0x20) & 0x1f;
  if (uVar1 == 8) {
    uVar2 = 1;
  }
  else {
    uVar2 = 2;
    if (uVar1 != 10) {
      uVar2 = 4;
      if (uVar1 != 0xb) {
        uVar2 = 1;
      }
      return uVar2;
    }
  }
  return uVar2;
}


//// get_posdiv @ffffff800844c928

undefined4 get_posdiv(long param_1)

{
  uint uVar1;
  undefined4 uVar2;

  uVar1 = **(uint **)(param_1 + 0x10) >> 0x18 & 7;
  if (uVar1 == 0) {
    uVar2 = 1;
  }
  else {
    uVar2 = 2;
    if (uVar1 != 1) {
      uVar2 = 4;
      if (uVar1 != 2) {
        uVar2 = 1;
      }
      return uVar2;
    }
  }
  return uVar2;
}


//// get_cur_volt_proc1_cpu @ffffff800844c604

int get_cur_volt_proc1_cpu(void)

{
  int iVar1;

  iVar1 = regulator_get_voltage(uRamffffff80095c31b0);
  return iVar1 / 10;
}


//// get_cur_volt_sram1_cpu @ffffff800844c5e0

int get_cur_volt_sram1_cpu(void)

{
  int iVar1;

  iVar1 = regulator_get_voltage(uRamffffff80095c31a8);
  return iVar1 / 10;
}


//// charging_get_charging_status @ffffff8008855748

undefined8 charging_get_charging_status(long param_1,undefined1 *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;

  iVar1 = battery_get_bat_soc();
  iVar2 = battery_get_bat_uisoc();
  piVar4 = *(int **)(*(long *)(param_1 + 0x3c8) + 0x18);
  iVar3 = fan54005_get_chip_status();
  if (iVar3 == 2) {
    iVar3 = *piVar4;
    if (iVar3 == 1) {
      *param_2 = 1;
      return 0;
    }
    if (iVar3 != 5) {
      return 0;
    }
    *param_2 = 1;
    if (0x62 < iVar1 || iVar2 != 100) {
      return 0;
    }
  }
  *param_2 = 0;
  return 0;
}


//// fan54005_get_chip_status @ffffff80088556f0

void fan54005_get_chip_status(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(0,&uStack_9,3,4);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}


//// fan54005_get_en_level @ffffff8008855eb4

void fan54005_get_en_level(void)

{
  undefined1 uStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uStack_9 = 0;
  fan54005_read_interface(5,&uStack_9,1,3);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uStack_9);
  }
  return;
}
