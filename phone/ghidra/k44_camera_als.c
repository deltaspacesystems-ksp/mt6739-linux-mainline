//// ltr577_i2c_probe @ffffff800849a0d0

void ltr577_i2c_probe(long param_1)

{
  long *plVar1;
  int iVar2;
  int iVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long unaff_x24;
  long unaff_x25;
  long *plVar10;
  undefined8 *puVar11;
  undefined1 uStack_d1;
  code *pcStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  code *pcStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  code *pcStack_a0;
  code *pcStack_98;
  code *pcStack_90;
  code *pcStack_88;
  code *pcStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  ulong uStack_60;
  code *pcStack_58;
  code *pcStack_50;
  code *pcStack_48;
  code *pcStack_40;
  code *pcStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  ulong uStack_10;
  long lStack_8;

  lVar8 = -0x7ff6bb6000;
  plVar10 = (long *)0xffffff80093cf000;
  iVar3 = -5;
  pcStack_58 = (code *)0x0;
  pcStack_50 = (code *)0x0;
  lStack_8 = lRamffffff80093cff08;
  pcStack_d0 = (code *)0x0;
  uStack_c8 = 0;
  pcStack_a0 = (code *)0x0;
  pcStack_98 = (code *)0x0;
  pcStack_48 = (code *)0x0;
  pcStack_40 = (code *)0x0;
  pcStack_90 = (code *)0x0;
  pcStack_88 = (code *)0x0;
  pcStack_80 = (code *)0x0;
  uStack_78 = 0;
  pcStack_38 = (code *)0x0;
  uStack_30 = 0;
  uStack_70 = 0;
  uStack_68 = 0;
  pcStack_b8 = (code *)0x0;
  uStack_b0 = 0;
  uStack_28 = 0;
  uStack_20 = 0;
  uStack_c0 = 0;
  uStack_a8 = 0;
  uStack_18 = 0;
  uStack_10 = 0;
  uStack_60 = 0;
  iVar2 = ontim_dev_debug_file_exist(0xffffff800944b100,0);
  if (iVar2 == 0) goto LAB_ffffff800849a770;
  unaff_x24 = -0x7ff742e000;
  printk(0xffffff8008e11d52,0xffffff8008bd2c40);
  unaff_x25 = -0x7ff6a29000;
  plVar4 = (long *)kmem_cache_alloc_trace(uRamffffff8009597940,0x24080c0,0xb0);
  if (plVar4 == (long *)0x0) goto LAB_ffffff800849a78c;
  __memset(plVar4,0,0xb0);
  *plVar4 = -0x7ff6a288a8;
  plVar4[1] = param_1;
  plVar4[2] = 0xfffffffe0;
  plVar4[3] = (long)(plVar4 + 3);
  plVar4[4] = (long)(plVar4 + 3);
  plVar4[5] = (long)ltr577_eint_work;
  *(long **)(param_1 + 0xc0) = plVar4;
  *(undefined4 *)((long)plVar4 + 0x3c) = 300;
  *(undefined4 *)(plVar4 + 8) = 0;
  *(undefined4 *)((long)plVar4 + 0x44) = 0;
  *(undefined4 *)((long)plVar4 + 0x4c) = 300;
  *(undefined4 *)(plVar4 + 10) = 0;
  *(undefined4 *)((long)plVar4 + 0x54) = 0;
  lVar6 = *plVar4;
  *(undefined4 *)(plVar4 + 9) = 0;
  *(undefined4 *)(plVar4 + 7) = 0;
  plRamffffff80095d7718 = plVar4;
  *(undefined4 *)(plVar4 + 0x10) = *(undefined4 *)(lVar6 + 0x218);
  *(undefined4 *)((long)plVar4 + 0x84) = *(undefined4 *)(lVar6 + 0x21c);
  *(undefined4 *)(plVar4 + 0x13) = *(undefined4 *)(lVar6 + 0x1fc);
  *(undefined4 *)(plVar4 + 0x12) = *(undefined4 *)(lVar6 + 0x220);
  *(undefined4 *)((long)plVar4 + 0x94) = *(undefined4 *)(lVar6 + 0x224);
  *(undefined4 *)((long)plVar4 + 0x74) = 0;
  plVar4[0x14] = 0;
  plVar4[0x15] = 0;
  *(undefined2 *)(plVar4 + 6) = 0x10;
  __memcpy(0xffffff80095d79d8,lVar6 + 0x1c,0x1b0);
  lVar6 = *plVar4;
  uRamffffff80095d79a8 = *(undefined8 *)(lVar6 + 0x1cc);
  uRamffffff80095d79b0 = *(undefined8 *)(lVar6 + 0x1d4);
  uRamffffff80095d79b8 = *(undefined8 *)(lVar6 + 0x1dc);
  uRamffffff80095d79c0 = *(undefined8 *)(lVar6 + 0x1e4);
  uRamffffff80095d79c8 = *(undefined8 *)(lVar6 + 0x1ec);
  uRamffffff80095d79d0 = *(undefined8 *)(lVar6 + 500);
  *(undefined4 *)((long)plVar4 + 0x34) = 3;
  clear_bit(1,plVar4 + 0x14);
  clear_bit(2,plVar4 + 0x14);
  printk(0xffffff8008e11b42);
  plVar1 = plRamffffff80095d7718;
  lVar6 = 0x259;
  lVar9 = plRamffffff80095d7718[1];
  lRamffffff80095d79a0 = param_1;
  while (lVar6 = lVar6 + -1, lVar6 != 0) {
    __const_udelay(0x418958);
  }
  iVar2 = ltr577_master_recv(plRamffffff80095d7718[1],6,0xffffff800944af44,1);
  if (iVar2 < 0) {
    printk(0xffffff8008e108b4,iVar2);
  }
  uStack_d1 = 0x10;
  iVar2 = ltr577_master_send_constprop_30(lVar9,2,&uStack_d1);
  if (iVar2 < 0) {
    uVar5 = 0xffffff8008e11b6d;
LAB_ffffff800849a31c:
    printk(uVar5);
  }
  else {
    if (cRamffffff800944af44 == -0x4f) {
      lRamffffff80095d7bc0 = -0x7ff6bb4ee8;
    }
    uStack_d1 = 0x36;
    iVar2 = ltr577_master_send_constprop_30(lVar9,1,&uStack_d1);
    if (iVar2 < 0) {
      uVar5 = 0xffffff8008e11205;
      goto LAB_ffffff800849a31c;
    }
    uStack_d1 = 0x5c;
    iVar2 = ltr577_master_send_constprop_30(lVar9,3,&uStack_d1);
    if (iVar2 < 0) {
      uVar5 = 0xffffff8008e11236;
      goto LAB_ffffff800849a31c;
    }
    if (*(int *)(*plVar1 + 0x10) == 0) {
      ltr577_ps_set_thres();
      uStack_d1 = 1;
      iVar2 = ltr577_master_send_constprop_30(lVar9,0x19,&uStack_d1);
      if (-1 < iVar2) {
        uStack_d1 = 2;
        iVar2 = ltr577_master_send_constprop_30(lVar9,0x1a,&uStack_d1);
        if (-1 < iVar2) goto LAB_ffffff800849a40c;
      }
    }
    else {
LAB_ffffff800849a40c:
      iRamffffff80095d7b90 = 9;
      printk(0xffffff8008e11268,0xffffff8008bd2c58,0x836);
      if (iRamffffff80095d7b90 == 6) {
        uStack_d1 = 2;
      }
      else if (iRamffffff80095d7b90 < 7) {
        if (iRamffffff80095d7b90 == 1) {
          uStack_d1 = 0;
        }
        else {
LAB_ffffff800849a448:
          uStack_d1 = 1;
        }
      }
      else if (iRamffffff80095d7b90 == 9) {
        uStack_d1 = 3;
      }
      else {
        if (iRamffffff80095d7b90 != 0x12) goto LAB_ffffff800849a448;
        uStack_d1 = 4;
      }
      ltr577_master_send_constprop_30(lVar9,5,&uStack_d1);
      uStack_d1 = 0x22;
      ltr577_master_send_constprop_30(lVar9,4,&uStack_d1);
      printk(0xffffff8008e11295,0xffffff8008bd2c58,0x85b,0x22);
      iVar2 = ltr577_setup_eint(lVar9);
      if (iVar2 == 0) {
        printk(0xffffff8008e11ccc);
        iVar3 = misc_register(0xffffff800944af88);
        lVar6 = lRamffffff80095d7bc0;
        if (iVar3 == 0) {
          uStack_10._0_4_ = (uint)(uint3)uStack_10;
          uStack_60._0_4_ = (uint)(uint3)uStack_60;
          if (lRamffffff80095d7bc0 != 0) {
            lVar9 = strlen(lRamffffff80095d7bc0);
            strncpy(0xffffff800944b130,lVar6,lVar9 + 1);
            printk(0xffffff8008e11bf7,0xffffff800944b130);
          }
          lVar6 = lRamffffff800944af80 + 0x28;
          if (lVar6 == 0) {
            iVar3 = -0x16;
LAB_ffffff800849a5d4:
            uVar7 = 0x974;
            uVar5 = 0xffffff8008e11cf1;
          }
          else {
            lVar9 = 0;
            do {
              puVar11 = *(undefined8 **)(lVar9 + -0x7ff742d578);
              iVar3 = driver_create_file(lVar6,puVar11);
              if (iVar3 != 0) {
                printk(0xffffff8008e11c18,0xffffff8008bd2c70,0x59a,*puVar11,iVar3);
                goto LAB_ffffff800849a5d4;
              }
              lVar9 = lVar9 + 8;
            } while (lVar9 != 0x58);
            uStack_10 = uStack_10 & 0xffffffffffff0000;
            uRamffffff80095d7738 = *(undefined4 *)(*plVar4 + 0x21c);
            pcStack_58 = als_open_report_data;
            pcStack_50 = als_enable_nodata;
            pcStack_48 = als_set_delay;
            pcStack_40 = ltr_als_batch;
            pcStack_38 = ltr_als_flush;
            iVar3 = als_register_control_path(&pcStack_58);
            if (iVar3 == 0) {
              pcStack_d0 = als_get_data;
              uStack_c0 = CONCAT44(uStack_c0._4_4_,100);
              iVar3 = als_register_data_path(&pcStack_d0);
              if (iVar3 == 0) {
                pcStack_a0 = ps_open_report_data;
                pcStack_98 = ps_enable_nodata;
                pcStack_90 = ps_set_delay;
                pcStack_88 = ltr_ps_batch;
                pcStack_80 = ltr_ps_flush;
                uStack_60 = uStack_60 & 0xffffffffffff0000;
                iVar3 = ps_register_control_path(&pcStack_a0);
                if (iVar3 == 0) {
                  uStack_a8 = CONCAT44(uStack_a8._4_4_,100);
                  pcStack_b8 = ps_get_data;
                  iVar3 = ps_register_data_path(&pcStack_b8);
                  if (iVar3 == 0) {
                    lVar8 = -0x7ff6bb5208;
                    regester_and_init_ontim_debug(0xffffff800944b100);
                    uRamffffff800944aef0 = 0;
                    printk(0xffffff8008e11c98,0xffffff8008bd2c88);
                    uRamffffff80095cc40a = 1;
                    goto LAB_ffffff800849a770;
                  }
                  uVar7 = 0x9b2;
                  uVar5 = 0xffffff8008e11c70;
                  goto LAB_ffffff800849a674;
                }
                uVar7 = 0x9a9;
              }
              else {
                uVar7 = 0x995;
              }
            }
            else {
              uVar7 = 0x98c;
            }
            uVar5 = 0xffffff8008e11c49;
          }
LAB_ffffff800849a674:
          printk(uVar5,0xffffff8008bd2c40,uVar7,iVar3);
        }
        else {
          printk(0xffffff8008e11bc5,0xffffff8008bd2c40,0x966);
        }
        misc_deregister(0xffffff800944af88);
        goto LAB_ffffff800849a340;
      }
      printk(0xffffff8008e11ba2,0xffffff8008bd2c58,0x85f,iVar2);
    }
  }
  iVar3 = 1;
  printk(0xffffff8008e112d7,0xffffff8008bd2c58,0x866,iVar2);
LAB_ffffff800849a340:
  kfree(plVar4);
  lVar6 = lVar8;
  lVar9 = unaff_x25;
  while( true ) {
    unaff_x25 = lVar9 + 0x718;
    lVar8 = lVar6 + 0xdf8;
    *(undefined8 *)(lVar9 + 0x9a0) = 0;
    printk(0xffffff8008e11cab,unaff_x24 + 0xc40,0x9d0,unaff_x24 + 0xc88,iVar3);
    *(undefined4 *)(lVar6 + 0xef0) = 0xffffffff;
LAB_ffffff800849a770:
    plVar10 = plVar10 + 0x1e1;
    if (lStack_8 == *plVar10) break;
    __stack_chk_fail(iVar3);
LAB_ffffff800849a78c:
    iVar3 = -0xc;
    lVar6 = lVar8;
    lVar9 = unaff_x25;
  }
  return;
}


//// ltr577_als_read @ffffff800849767c

void ltr577_als_read(ulong param_1,undefined2 *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  ulong uVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  undefined4 uVar8;
  long lVar9;
  ulong unaff_x19;
  long lVar10;
  long *plVar11;
  uint uVar12;
  byte bStack_10;
  byte bStack_f;
  byte bStack_e;
  long lStack_8;

  uVar4 = uRamffffff80098008b0;
  plVar11 = (long *)0xffffff80093cf000;
  lVar10 = -0x7ff6a29000;
  lStack_8 = lRamffffff80093cff08;
  lVar9 = lVar10;
  if (iRamffffff80095d7724 == 0) {
    iRamffffff80095d7724 = 1;
    unaff_x19 = uRamffffff80098005a8;
    if (uRamffffff80098005a8 != 0) goto LAB_ffffff8008497830;
    unaff_x19 = uRamffffff8009800680;
    if (uRamffffff8009800680 != 0) goto LAB_ffffff8008497830;
    unaff_x19 = uRamffffff80098008b0;
    if (uRamffffff80098008b0 != 0) goto LAB_ffffff8008497830;
    printk(0xffffff8008e10920,0xffffff8008bd2868,0x371);
    unaff_x19 = uVar4;
  }
  do {
    iVar7 = ltr577_master_recv(param_1,0xd,&bStack_10,3);
    if (iVar7 < 0) {
      printk(0xffffff8008e108b4,iVar7);
    }
    uVar1 = (uint)bStack_10 + ((uint)bStack_f + (uint)bStack_e * 0x100) * 0x100;
    printk(0xffffff8008e109f7);
    iVar7 = ltr577_master_recv(param_1,10,&bStack_10,3);
    if (iVar7 < 0) {
      printk(0xffffff8008e108b4,iVar7);
    }
    uVar2 = (uint)bStack_10 + ((uint)bStack_f + (uint)bStack_e * 0x100) * 0x100;
    param_1 = (ulong)uVar2;
    printk(0xffffff8008e10a33);
    uVar12 = 0;
    lVar10 = lVar9;
    if (uVar1 != 0) {
      uVar3 = 0;
      if (uVar1 != 0) {
        uVar3 = (uVar2 * 10) / uVar1;
      }
      bVar6 = cRamffffff800944af44 == -0x4f;
      *(uint *)(lVar9 + 0x72c) = uVar3;
      if (bVar6) {
        if (uVar3 < 0x97) {
          bVar5 = SBORROW4(uVar3,0x1e);
          iVar7 = uVar3 - 0x1e;
          bVar6 = uVar3 == 0x1e;
          goto LAB_ffffff8008497930;
        }
LAB_ffffff80084977d0:
        iVar7 = *piRamffffff800944af20;
        uVar8 = 2;
LAB_ffffff80084977e8:
        *(undefined4 *)(lVar9 + 0x730) = uVar8;
      }
      else {
        if (0x3a < uVar3) goto LAB_ffffff80084977d0;
        bVar5 = SBORROW4(uVar3,0xf);
        iVar7 = uVar3 - 0xf;
        bVar6 = uVar3 == 0xf;
LAB_ffffff8008497930:
        if (!bVar6 && iVar7 < 0 == bVar5) {
          iVar7 = piRamffffff800944af20[1];
          uVar8 = 1;
          goto LAB_ffffff80084977e8;
        }
        iVar7 = piRamffffff800944af20[2];
        *(undefined4 *)(lVar9 + 0x730) = 0;
      }
      uVar12 = (int)(uVar1 * iVar7) / 300;
      unaff_x19 = (ulong)uVar12;
      lVar10 = lVar9 + 0x718;
      printk(0xffffff8008e10a77,unaff_x19,uVar3,uVar2,uVar1,*(undefined4 *)(lVar9 + 0x728));
    }
    *param_2 = (short)uVar12;
    plVar11 = plVar11 + 0x1e1;
    if (lStack_8 == *plVar11) {
      return;
    }
    __stack_chk_fail(uVar12);
LAB_ffffff8008497830:
    unaff_x19 = unaff_x19 + 0x3c;
    printk(0xffffff8008e10973,unaff_x19);
    lVar9 = strstr(unaff_x19,0xffffff8008e10995);
    if (lVar9 == 0) {
      lVar9 = strstr(unaff_x19,0xffffff8008e109c8);
      if (lVar9 == 0) {
        lVar9 = strstr(unaff_x19,0xffffff8008e109d0);
        if (lVar9 != 0) {
          printk(0xffffff8008e1099b,unaff_x19);
          *(undefined4 *)(lVar10 + 0x728) = 2;
          piRamffffff800944af20 = (int *)0xffffff800944af48;
          if (cRamffffff800944af44 != -0x4f) {
            piRamffffff800944af20 = (int *)0xffffff800944af58;
          }
        }
      }
      else {
        printk(0xffffff8008e1099b,unaff_x19);
        *(undefined4 *)(lVar10 + 0x728) = 1;
        piRamffffff800944af20 = (int *)0xffffff800944af38;
      }
    }
    else {
      printk(0xffffff8008e1099b,unaff_x19);
      *(undefined4 *)(lVar10 + 0x728) = 0;
      piRamffffff800944af20 = (int *)0xffffff800944af28;
    }
    printk(0xffffff8008e109d4,*(undefined4 *)(lVar10 + 0x728));
    lVar9 = lVar10;
  } while( true );
}


//// ltr577_ps_read @ffffff80084974a4

void ltr577_ps_read(undefined8 param_1,ushort *param_2)

{
  ushort uVar1;
  int iVar2;
  byte bStack_10;
  byte bStack_f;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  iVar2 = ltr577_master_recv(param_1,8,&bStack_10,2);
  if (iVar2 < 0) {
    printk(0xffffff8008e108b4,iVar2);
  }
  uVar1 = (bStack_f & 7) << 8 | (ushort)bStack_10;
  *param_2 = uVar1;
  printk(0xffffff8008e108ce,uVar1);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar1);
  }
  return;
}


//// ltr577_als_enable @ffffff8008498b48

void ltr577_als_enable(ulong param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 extraout_x1;
  undefined8 extraout_x1_00;
  long lVar5;
  long lVar6;
  long *plVar7;
  byte bStack_9;
  long lStack_8;

  plVar7 = (long *)0xffffff80093cf000;
  lVar5 = -0x7ff6a29000;
  lStack_8 = lRamffffff80093cff08;
  uVar4 = 0;
  bStack_9 = 0;
  lVar6 = lVar5;
  if (param_2 == 0) {
    if (iRamffffff80095d7b88 != 0) goto LAB_ffffff8008498bd4;
    uVar3 = 0xffffff8008e10fcd;
    goto LAB_ffffff8008498bb4;
  }
  if (iRamffffff80095d7b88 != 1) goto LAB_ffffff8008498bd4;
  printk(0xffffff8008e10fab,0);
  do {
    lVar6 = lVar5;
    param_2 = 0;
LAB_ffffff8008498bb8:
    while( true ) {
      plVar7 = plVar7 + 0x1e1;
      if (lStack_8 == *plVar7) {
        return;
      }
      __stack_chk_fail(param_2);
LAB_ffffff8008498bd4:
      iVar1 = ltr577_master_recv(param_1,0,&bStack_9,1);
      uVar4 = extraout_x1;
      if (iVar1 < 0) {
        printk(0xffffff8008e108b4,iVar1);
        uVar4 = extraout_x1_00;
      }
      bStack_9 = bStack_9 & 0xef;
      if (param_2 != 0) break;
      if ((cRamffffff800944af44 != -0x4f) || (*(int *)(lVar6 + 0xb8c) != 1)) {
        printk(0xffffff8008e1103e);
        bStack_9 = bStack_9 & 0xfd;
        goto LAB_ffffff8008498c20;
      }
      uVar3 = 0xffffff8008e11015;
LAB_ffffff8008498bb4:
      printk(uVar3,uVar4);
    }
    printk(0xffffff8008e10ff0);
    bStack_9 = bStack_9 | 2;
LAB_ffffff8008498c20:
    uVar2 = ltr577_master_send_constprop_30(param_1,0,&bStack_9);
    param_1 = (ulong)uVar2;
    if ((int)uVar2 < 0) {
      printk(0xffffff8008e11064,0xffffff8008bd2af8,0x34c,uVar2,param_2);
      param_2 = uVar2;
      goto LAB_ffffff8008498bb8;
    }
    lVar5 = 0xb;
    while (lVar5 = lVar5 + -1, lVar5 != 0) {
      __const_udelay(0x418958);
    }
    lVar5 = lVar6 + 0x718;
    param_1 = 0;
    if (param_2 == 0) {
      *(undefined4 *)(lVar6 + 0xb88) = 0;
      lVar6 = lVar5;
      goto LAB_ffffff8008498bb8;
    }
    *(undefined4 *)(lVar6 + 0xb88) = 1;
  } while( true );
}


//// ltr577_ps_enable @ffffff8008498f10

void ltr577_ps_enable(ulong param_1,uint param_2)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  char cVar5;
  bool bVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  undefined8 uVar10;
  ulong uVar11;
  undefined8 uVar12;
  code *pcVar13;
  long lVar14;
  long lVar15;
  long *plVar16;
  long *plVar17;
  char cStack_13;
  char cStack_12;
  byte bStack_11;
  undefined4 uStack_10;
  ushort uStack_c;
  long lStack_8;

  plVar16 = (long *)0xffffff80093cf000;
  lVar14 = -0x7ff6a29000;
  lStack_8 = lRamffffff80093cff08;
  printk(0xffffff8008e11143,0);
  lVar9 = -0x7ff6a288e8;
  if (param_2 == 0) goto LAB_ffffff8008498f98;
  lVar15 = lVar14;
  if (iRamffffff80095d7b8c != 1) goto LAB_ffffff8008498fb0;
  iVar7 = 0;
  printk(0xffffff8008e1116c);
LAB_ffffff8008498f7c:
  do {
    while( true ) {
      plVar16 = plVar16 + 0x1e1;
      if (lStack_8 == *plVar16) {
        return;
      }
      lVar9 = __stack_chk_fail(iVar7);
LAB_ffffff8008498f98:
      lVar15 = lVar14;
      if (*(int *)(lVar9 + 0x474) != 0) break;
      printk(0xffffff8008e1118d);
      iVar7 = 0;
    }
LAB_ffffff8008498fb0:
    iVar7 = ltr577_master_recv(param_1,2,&cStack_13,1);
    if (iVar7 < 0) {
      printk(0xffffff8008e108b4,iVar7);
    }
    iVar7 = ltr577_master_recv(param_1,0x19,&cStack_12,1);
    if (iVar7 < 0) {
      printk(0xffffff8008e108b4,iVar7);
    }
    if ((cStack_13 == '\b') || (cStack_12 != '\x01')) {
      printk(0xffffff8008e111af);
      plVar17 = *(long **)(lVar15 + 0x718);
      lVar9 = 0xb;
      lVar14 = plVar17[1];
      while (lVar9 = lVar9 + -1, lVar9 != 0) {
        __const_udelay(0x418958);
      }
      bStack_11 = 0x10;
      iVar7 = ltr577_master_send_constprop_30(lVar14,2,&bStack_11);
      if (iVar7 < 0) {
        uVar10 = 0xffffff8008e111d1;
LAB_ffffff8008499070:
        printk(uVar10);
LAB_ffffff8008499074:
        uVar12 = 0x7e8;
        uVar10 = 0xffffff8008e112d7;
      }
      else {
        bStack_11 = 0x36;
        iVar7 = ltr577_master_send_constprop_30(lVar14,1,&bStack_11);
        if (iVar7 < 0) {
          uVar10 = 0xffffff8008e11205;
          goto LAB_ffffff8008499070;
        }
        bStack_11 = 0x5c;
        iVar7 = ltr577_master_send_constprop_30(lVar14,3,&bStack_11);
        if (iVar7 < 0) {
          uVar10 = 0xffffff8008e11236;
          goto LAB_ffffff8008499070;
        }
        if (*(int *)(*plVar17 + 0x10) == 0) {
          ltr577_ps_set_thres();
          bStack_11 = 1;
          iVar7 = ltr577_master_send_constprop_30(lVar14,0x19,&bStack_11);
          if (-1 < iVar7) {
            bStack_11 = 2;
            iVar7 = ltr577_master_send_constprop_30(lVar14,0x1a,&bStack_11);
            if (-1 < iVar7) goto LAB_ffffff8008499100;
          }
          goto LAB_ffffff8008499074;
        }
LAB_ffffff8008499100:
        *(undefined4 *)(lVar15 + 0xb90) = 9;
        printk(0xffffff8008e11268,0xffffff8008bd2b58,0x7be);
        iVar7 = *(int *)(lVar15 + 0xb90);
        if (iVar7 == 6) {
          bStack_11 = 2;
        }
        else if (iVar7 < 7) {
          if (iVar7 == 1) {
            bStack_11 = 0;
          }
          else {
LAB_ffffff800849913c:
            bStack_11 = 1;
          }
        }
        else if (iVar7 == 9) {
          bStack_11 = 3;
        }
        else {
          if (iVar7 != 0x12) goto LAB_ffffff800849913c;
          bStack_11 = 4;
        }
        ltr577_master_send_constprop_30(lVar14,5,&bStack_11);
        bStack_11 = 0x22;
        ltr577_master_send_constprop_30(lVar14,4,&bStack_11);
        uVar10 = 0xffffff8008e11295;
        iVar7 = 0x22;
        uVar12 = 0x7e3;
      }
      printk(uVar10,0xffffff8008bd2b58,uVar12,iVar7);
    }
    iVar7 = ltr577_master_recv(param_1,0,&bStack_11,1);
    if (iVar7 < 0) {
      printk(0xffffff8008e108b4,iVar7);
    }
    bStack_11 = bStack_11 & 0xef;
    if (param_2 == 0) {
      printk(0xffffff8008e11318);
      bStack_11 = bStack_11 & 0xfe;
    }
    else {
      printk(0xffffff8008e112f8);
      bStack_11 = bStack_11 | 1;
    }
    iVar7 = ltr577_master_send_constprop_30(param_1,0,&bStack_11);
    if (-1 < iVar7) {
      lVar9 = 0xb;
      while (lVar9 = lVar9 + -1, lVar9 != 0) {
        __const_udelay(0x418958);
      }
      iVar7 = ltr577_master_recv(param_1,0,&bStack_11,1);
      if (iVar7 < 0) {
        printk(0xffffff8008e108b4,iVar7);
      }
      bVar6 = param_2 == 0;
      uVar1 = *(uint *)(**(long **)(lVar15 + 0x718) + 0x10);
      if (bVar6 || uVar1 != 0) {
        param_2 = param_2 | uVar1;
        if (param_2 == 0) {
          *(undefined4 *)(lVar15 + 0xb8c) = 0;
        }
      }
      else {
        uStack_c = 0;
        lVar9 = sp_el0;
        *(undefined8 *)(lVar15 + 0xb98) = *(undefined8 *)(lVar9 + 8);
        uStack_10 = 0;
        *(undefined8 *)(lVar9 + 8) = 0xffffffffffffffff;
        printk(0xffffff8008e1136b);
        uVar11 = filp_open(0xffffff800944b000,0,0);
        if ((0xfffffffffffff000 < uVar11) || (*(long *)(uVar11 + 0x28) == 0)) {
          printk(0xffffff8008e1137c,0xffffff8008bd2b88,0x227);
          uVar11 = 0;
        }
        *(ulong *)(lVar15 + 0xba0) = uVar11;
        if (uVar11 == 0) {
          printk(0xffffff8008e113b7,0xffffff8008bd2bb0,0x26a,0xffffff8008bd2b98,0xffffff800944b000);
LAB_ffffff8008499360:
          lVar9 = sp_el0;
          *(undefined8 *)(lVar9 + 8) = *(undefined8 *)(lVar15 + 0xb98);
        }
        else {
          printk(0xffffff8008e113ed,0xffffff800944b000);
          lVar9 = *(long *)(*(long *)(lVar15 + 0xba0) + 0x28);
          if (((lVar9 == 0) || (pcVar13 = *(code **)(lVar9 + 8), pcVar13 == (code *)0x0)) ||
             (iVar8 = (*pcVar13)(*(long *)(lVar15 + 0xba0),0xd2,0), iVar8 < 0)) {
            printk(0xffffff8008e1141d,0xffffff8008bd2bb0,0x271,0xffffff8008bd2b98,0xffffff800944b000
                  );
LAB_ffffff8008499420:
            filp_close(*(undefined8 *)(lVar15 + 0xba0),0);
            goto LAB_ffffff8008499360;
          }
          lVar9 = *(long *)(lVar15 + 0xba0);
          uStack_c = 0;
          uStack_10 = 0;
          if ((lVar9 == 0) || (*(long *)(lVar9 + 0x28) == 0)) {
            iVar8 = -1;
LAB_ffffff800849955c:
            printk(0xffffff8008e114a2,0xffffff8008bd2bb0,0x279,0xffffff8008bd2b98,iVar8);
            goto LAB_ffffff8008499420;
          }
          iVar8 = __vfs_read(lVar9,&uStack_10,6,lVar9 + 0x70);
          if (iVar8 < 1) goto LAB_ffffff800849955c;
          printk(0xffffff8008e11454,0xffffff8008bd2b98,&uStack_10);
          filp_close(*(undefined8 *)(lVar15 + 0xba0),0);
          lVar9 = sp_el0;
          *(undefined8 *)(lVar9 + 8) = *(undefined8 *)(lVar15 + 0xb98);
          uVar1 = uStack_10 >> 8 & 0xff | (uStack_10 & 0xff) << 8;
          uVar3 = (uint)(uStack_10._2_2_ >> 8) | (uStack_10._2_2_ & 0xff00ff) << 8;
          uVar4 = (uint)(uStack_c >> 8) | (uStack_c & 0xff00ff) << 8;
          param_1 = (ulong)uVar3;
          printk(0xffffff8008e11475,0xffffff8008bd2b98,uVar1,param_1,uVar4);
          if ((uVar1 + 8 < uVar3) && (uVar3 + 8 < uVar4)) {
            *(uint *)(lVar15 + 0xba8) = uVar1;
            *(uint *)(lVar15 + 0xbac) = uVar3;
            *(uint *)(lVar15 + 0xbb0) = uVar4;
            printk(0xffffff8008e114cc,0xffffff8008bd2b98,uVar1,uVar3,uVar4);
          }
          else {
            *(undefined4 *)(lVar15 + 0xbac) = 0x28;
            *(undefined4 *)(lVar15 + 0xbb0) = 0x46;
            *(undefined4 *)(lVar15 + 0xba8) = 10;
            printk(0xffffff8008e11501,0xffffff8008bd2bb0,0x291,0xffffff8008bd2b98,uVar1);
          }
        }
        cVar5 = cRamffffff800944af44;
        param_2 = 0x944adf8;
        *(int *)(lVar15 + 0x744) = *(int *)(lVar15 + 0xbb0) - *(int *)(lVar15 + 0xbac);
        *(int *)(lVar15 + 0x73c) = *(int *)(lVar15 + 0xba8);
        *(int *)(lVar15 + 0x734) = *(int *)(lVar15 + 0xbac) - *(int *)(lVar15 + 0xba8);
        printk(0xffffff8008e1153d,0xffffff8008bd2b70,0x2fc,cVar5);
        if (cRamffffff800944af44 == -0x4f) {
          *(undefined4 *)(lVar15 + 0x740) = 0;
          *(undefined4 *)(lVar15 + 0x748) = 0;
          *(undefined4 *)(lVar15 + 0x74c) = 1;
        }
        else {
          iVar8 = *(int *)(**(long **)(lVar15 + 0x718) + 0x21c);
          *(int *)(lVar15 + 0x738) = iVar8;
          iVar8 = iVar8 - *(int *)(lVar15 + 0x734);
          *(int *)(lVar15 + 0x748) = iVar8;
          *(int *)(lVar15 + 0x740) = iVar8 + -1;
        }
        if (((*(int *)(lVar15 + 0x744) == 0) && (*(int *)(lVar15 + 0x734) == 0)) &&
           (*(int *)(lVar15 + 0x73c) == 0)) {
          uVar2 = *(undefined4 *)(**(long **)(lVar15 + 0x718) + 0x218);
          *(undefined4 *)(lVar15 + 0x734) = *(undefined4 *)(**(long **)(lVar15 + 0x718) + 0x21c);
          *(undefined4 *)(lVar15 + 0x744) = uVar2;
          *(undefined4 *)(lVar15 + 0x73c) = 10;
        }
        *(short *)(lVar15 + 0x720) = (short)*(undefined4 *)(lVar15 + 0x740);
        *(short *)(lVar15 + 0x722) = (short)*(undefined4 *)(lVar15 + 0x748);
        printk(0xffffff8008e1155f,0xffffff8008bd2bc8,*(undefined4 *)(lVar15 + 0x744),
               *(undefined4 *)(lVar15 + 0x734),*(undefined4 *)(lVar15 + 0x73c));
        printk(0xffffff8008e115a8,0xffffff8008bd2bc8,*(undefined2 *)(lVar15 + 0x720),
               *(undefined2 *)(lVar15 + 0x722));
        *(undefined4 *)(lVar15 + 0xb8c) = 1;
        ltr577_ps_set_thres();
      }
      lVar14 = lVar15 + 0x718;
      if (!bVar6 && *(int *)(lVar15 + 0x750) == 1) {
        *(undefined4 *)(lVar15 + 0x750) = 2;
      }
      goto LAB_ffffff8008498f7c;
    }
    printk(0xffffff8008e11339,0xffffff8008bd2b70,0x2ed,iVar7,param_2);
    lVar14 = lVar15;
  } while( true );
}


//// imgsensor_hw_power_sequence @ffffff8008694264

undefined8
imgsensor_hw_power_sequence(long param_1,ulong param_2,int param_3,long *param_4,undefined8 param_5)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  long *plVar4;
  long *plVar5;
  long *plVar6;
  ulong uVar7;
  undefined8 *puVar8;

  param_2 = param_2 & 0xffffffff;
  plVar4 = param_4 + 0xf8;
  while( true ) {
    if (*param_4 == 0) {
      return 0xffffffff;
    }
    if ((param_4 == plVar4) || (iVar2 = strcmp(*param_4,param_5), iVar2 == 0)) break;
    param_4 = param_4 + 0x1f;
  }
  iVar2 = 0;
  for (plVar4 = param_4 + 1; (iVar1 = (int)*plVar4, iVar1 != 0 && (plVar4 < param_4 + 0x1f));
      plVar4 = (long *)((long)plVar4 + 0x14)) {
    if (param_3 == 1 && iVar1 != -1) {
      puVar8 = *(undefined8 **)
                (param_1 + (long)*(int *)(param_1 + (param_2 * 0xe + (long)iVar1) * 4 + 0x20) * 8);
      pcVar3 = (code *)puVar8[3];
      if (pcVar3 != (code *)0x0) {
        (*pcVar3)(*puVar8,param_2,iVar1,*(int *)((long)plVar4 + 4));
      }
      for (uVar7 = (ulong)*(uint *)(plVar4 + 1); uVar7 != 0; uVar7 = uVar7 - 1) {
        __const_udelay(0x418958);
      }
    }
    iVar2 = iVar2 + 1;
  }
  if (param_3 == 0) {
    plVar6 = plVar4;
    while (plVar5 = plVar6, plVar5 != (long *)((long)plVar4 + (long)iVar2 * -0x14)) {
      plVar6 = (long *)((long)plVar5 + -0x14);
      if (*(int *)plVar6 != -1) {
        puVar8 = *(undefined8 **)
                  (param_1 +
                  (long)*(int *)(param_1 + (param_2 * 0xe + (long)*(int *)plVar6) * 4 + 0x20) * 8);
        for (uVar7 = (ulong)*(uint *)((long)plVar5 + -0xc); uVar7 != 0; uVar7 = uVar7 - 1) {
          __const_udelay(0x418958);
        }
        pcVar3 = (code *)puVar8[3];
        if (pcVar3 != (code *)0x0) {
          (*pcVar3)(*puVar8,param_2,*(int *)plVar6,(int)plVar5[-1]);
        }
      }
    }
  }
  else if (param_3 == 1) {
    __const_udelay(0x147aeb8);
  }
  return 0;
}


//// imgsensor_hw_power @ffffff8008694538

undefined8 imgsensor_hw_power(undefined8 param_1,long param_2,undefined8 param_3,undefined4 param_4)

{
  uint uVar1;

  uVar1 = *(uint *)(param_2 + 0xc);
  imgsensor_hw_power_sequence
            (param_1,uVar1,param_4,0xffffff800969bb98,
             *(undefined8 *)((ulong)uVar1 * 8 + -0x7ff6b86230));
  imgsensor_hw_power_sequence(param_1,uVar1,param_4,0xffffff800947ca70,param_3);
  return 0;
}
