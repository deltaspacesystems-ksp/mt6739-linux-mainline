//// pwrap_init @ffffff80085151d4

void pwrap_init(void)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_c [4];
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  iVar1 = pwrap_of_iomap(0xffffff80093cff08,0);
  if (iVar1 == 0) {
    *(undefined4 *)(lRamffffff80095e0ef8 + 0x80) = 0xf;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80095e0ef8 + 0xa4) = 0x100;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff80095e0ef8 + 0x108) = *(uint *)(lRamffffff80095e0ef8 + 0x108) & 0xfffffffb;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80095e0ef8 + 0x140) = 1;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80095e0ef8 + 0x144) = 1;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80095e0ef8 + 0x84) = 0xf;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80095e0ef8 + 0xa8) = 0x100;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x1b] = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[1] = 0;
    DataSynchronizationBarrier(3,3,0);
    *puRamffffff80095e0f08 = 1;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x1f] = 1;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[2] = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2100;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2800;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2000;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2800;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2800;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2800;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x20] = 0x2800;
    DataSynchronizationBarrier(3,3,0);
    do {
    } while (((uint)puRamffffff80095e0f08[0x309] >> 0x14 & 1) == 0);
    puRamffffff80095e0f08[0x1f] = 0;
    DataSynchronizationBarrier(3,3,0);
    *puRamffffff80095e0f08 = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x1f] = 0;
    DataSynchronizationBarrier(3,3,0);
    *puRamffffff80095e0f08 = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[1] = 1;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x1b] = 4;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0x27] = 1;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0xd] = 1;
    DataSynchronizationBarrier(3,3,0);
    _pwrap_wacs2_nochk_part_3(1,0x424,8,auStack_c);
    puRamffffff80095e0f08[8] = 8;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[9] = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[10] = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0xb] = 0;
    DataSynchronizationBarrier(3,3,0);
    puRamffffff80095e0f08[0xc] = 0;
    DataSynchronizationBarrier(3,3,0);
    pwrap_write_nochk(0x3a,0xf0);
    pwrap_write_nochk(0x2c,0xf);
    pwrap_write_nochk(0x92,0xf0);
    pwrap_write_nochk(0x408,1);
    pwrap_write_nochk(0x40a,1);
    do {
      do {
      } while ((puRamffffff80095e0f08[0x309] & 0x70000) != 0);
    } while (((uint)puRamffffff80095e0f08[0x309] >> 0x14 & 1) == 0);
    puRamffffff80095e0f08[2] = 1;
    DataSynchronizationBarrier(3,3,0);
    iVar2 = _pwrap_init_sistrobe_isra_4();
    iVar1 = 0x15;
    if (iVar2 == 0) {
      pwrap_write_nochk(0x40e,0xa55a,0x15);
      pwrap_read_nochk(0x40e,auStack_c);
      puRamffffff80095e0f08[0x10] = 0xf4;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x15] = 0x70000;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x36] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x35] = 0x414;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x37] = 0x83;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x11] = 0x42c;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x80] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x81] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x82] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x83] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x84] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x85] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x86] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x87] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x88] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x89] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8a] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8b] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8c] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8d] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8e] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x8f] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x90] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x91] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x92] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x93] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x94] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x95] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x96] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x97] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x98] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x99] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9a] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9b] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9c] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9d] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9e] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x9f] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xa0] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xa1] = 0x10d210ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x79] = 0x111c111c;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x7a] = 0x4000100;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x7b] = 0x10cc10a4;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x7c] = 0x10ca;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x1a] = 0xf;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb4] = 0x402;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb5] = 0x403;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb6] = 0x403;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb7] = 0x40f;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb8] = 0x428;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xb9] = 0x428;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xba] = 0xa5;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xbb] = 0x413;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xbc] = 0x417;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xbd] = 0x417;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xbe] = 0x47c;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xbf] = 0x47c;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xc0] = 0x740;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xc1] = 0x40f;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x1b] = 0x3fd25;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x23] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x27] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x2b] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0xf] = 5;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x3c] = 0xf;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x3d] = 0xffffffff;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x3e] = 0xffffffff;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x3a] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x2d] = 0xffffffff;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x31] = 0xffffffff;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x24] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x28] = 1;
      DataSynchronizationBarrier(3,3,0);
      puRamffffff80095e0f08[0x2c] = 1;
      DataSynchronizationBarrier(3,3,0);
      __iounmap(uRamffffff80095e0f00);
      iVar1 = 0;
    }
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(iVar1);
  }
  return;
}


//// pwrap_read_nochk @ffffff8008514fa0

undefined8 pwrap_read_nochk(uint param_1,undefined8 param_2)

{
  undefined8 uVar1;

  if ((param_1 & 0xffff0000) == 0) {
    uVar1 = _pwrap_wacs2_nochk_part_3(0,param_1,0,param_2);
    return uVar1;
  }
  return 3;
}


//// kpd_get_dts_info @ffffff80087e9570

void kpd_get_dts_info(undefined8 param_1)

{
  int iVar1;

  of_property_read_u32_array(param_1,0xffffff8008ec5ba7,0xffffff80097fb1e0,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5bc1,0xffffff80097fb1e4,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5bd8,0xffffff80097fb1e8,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5bef,0xffffff80097fb1ec,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c06,0xffffff80097fb1f0,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c1d,0xffffff80097fb1f4,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c3a,0xffffff80097fb31c,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c58,0xffffff80097fb320,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c75,0xffffff80097fb324,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5c8d,0xffffff80097fb328,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5ca5,0xffffff80097fb32c,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5cbd,0xffffff80097fb330,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5cda,0xffffff80097fb334,1);
  of_property_read_u32_array(param_1,0xffffff8008ec5cf6,0xffffff80097fb1f8,1);
  iVar1 = of_property_read_u32_array
                    (param_1,0xffffff8008ec5d0e,0xffffff80097fb1fc,uRamffffff80097fb1f8);
  if (iVar1 != 0) {
    if ((iRamffffff80097faf6c != 0) &&
       (iVar1 = ___ratelimit(0xffffff80094ac500,0xffffff8008c37b08), iVar1 != 0)) {
      printk(0xffffff8008ec5d27);
    }
    __memset(0xffffff80097fb1fc,0,0x120);
  }
  if ((iRamffffff80097faf6c != 0) &&
     (iVar1 = ___ratelimit(0xffffff80094ac520,0xffffff8008c37b08), iVar1 != 0)) {
    printk(0xffffff8008ec5d57,uRamffffff80097fb1e0,uRamffffff80097fb1e4,uRamffffff80097fb1e8,
           uRamffffff80097fb1f0,uRamffffff80097fb1ec);
  }
  return;
}


//// kpd_pmic_pwrkey_hal @ffffff80087ea6c0

void kpd_pmic_pwrkey_hal(long param_1)

{
  undefined8 uVar1;
  int iVar2;

  input_event(uRamffffff80097fb178,1,uRamffffff80097fb1e4,(int)param_1 != 0);
  input_event(uRamffffff80097fb178,0,0,0);
  if ((iRamffffff80097faf6c != 0) &&
     (iVar2 = ___ratelimit(0xffffff80094aca28,0xffffff8008c388f8), iVar2 != 0)) {
    uVar1 = 0xffffff8008f25323;
    if (param_1 == 0) {
      uVar1 = 0xffffff8008ec575e;
    }
    printk(0xffffff8008ec6274,uVar1,uRamffffff80097fb1e4);
  }
  aee_powerkey_notify_press(param_1);
  return;
}


//// kpd_pmic_rstkey_hal @ffffff80087ea60c

void kpd_pmic_rstkey_hal(long param_1)

{
  undefined8 uVar1;
  int iVar2;

  if (iRamffffff80097fb1ec != 0) {
    input_event(uRamffffff80097fb178,1,iRamffffff80097fb1ec,(int)param_1 != 0);
    input_event(uRamffffff80097fb178,0,0,0);
    if ((iRamffffff80097faf6c != 0) &&
       (iVar2 = ___ratelimit(0xffffff80094aca08,0xffffff8008c388e0), iVar2 != 0)) {
      uVar1 = 0xffffff8008f25323;
      if (param_1 == 0) {
        uVar1 = 0xffffff8008ec575e;
      }
      printk(0xffffff8008ec6274,uVar1,iRamffffff80097fb1ec);
    }
  }
  return;
}


//// kpd_get_keymap_state @ffffff80087e9e90

void kpd_get_keymap_state(undefined2 *param_1)

{
  int iVar1;

  DataSynchronizationBarrier(3,1,0);
  *param_1 = *(undefined2 *)(lRamffffff80097fb338 + 4);
  DataSynchronizationBarrier(3,1,0);
  param_1[1] = *(undefined2 *)(lRamffffff80097fb338 + 8);
  DataSynchronizationBarrier(3,1,0);
  param_1[2] = *(undefined2 *)(lRamffffff80097fb338 + 0xc);
  DataSynchronizationBarrier(3,1,0);
  param_1[3] = *(undefined2 *)(lRamffffff80097fb338 + 0x10);
  DataSynchronizationBarrier(3,1,0);
  param_1[4] = *(undefined2 *)(lRamffffff80097fb338 + 0x14);
  if (iRamffffff80097faf6c != 0) {
    iVar1 = ___ratelimit(0xffffff80094ac7b8,0xffffff8008c38820);
    if (iVar1 != 0) {
      printk(0xffffff8008ec6026,*param_1,param_1[1],param_1[2],param_1[3],param_1[4]);
    }
    return;
  }
  return;
}


//// pmic_get_auxadc_value @ffffff8008511f48

undefined8 pmic_get_auxadc_value(byte param_1)

{
  undefined8 uVar1;

  if (param_1 < 0xd) {
    uVar1 = mt6357_get_auxadc_value();
  }
  else {
    printk(0xffffff8008e26d46,0xffffff8008be7000);
    uVar1 = 0xffffffea;
  }
  return uVar1;
}
