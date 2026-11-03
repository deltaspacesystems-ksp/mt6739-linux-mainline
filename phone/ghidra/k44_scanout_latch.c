//// ovl_layer_config @ffffff800857c80c

undefined8
ovl_layer_config(undefined4 param_1,uint param_2,int param_3,long param_4,int *param_5,int *param_6,
                long param_7)

{
  uint *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  long lVar4;
  bool bVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  long lVar12;
  ulong uVar13;
  long lVar14;
  long lVar15;
  ulong uVar16;
  ulong uVar17;
  long lVar18;
  undefined8 uVar19;
  long lVar20;
  uint uVar21;
  int iVar22;
  int iVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  int iStack_c;

  iVar10 = *(int *)(param_4 + 0x90);
  lVar12 = ovl_base_addr();
  bVar5 = iVar10 != -1;
  lVar18 = 0;
  if (bVar5) {
    lVar18 = 0x300;
  }
  lVar20 = 0;
  if (bVar5) {
    lVar20 = 0x134;
  }
  lVar4 = 0;
  if (bVar5) {
    lVar4 = 0x70;
  }
  uVar13 = (ulong)(param_2 << 5);
  uVar17 = uVar13;
  if (bVar5) {
    uVar17 = ((ulong)*(uint *)(param_4 + 0x90) & 0x7ffffff) << 5;
  }
  lVar18 = uVar17 + lVar12 + lVar18;
  uVar21 = param_2;
  if (iVar10 != -1) {
    uVar21 = *(uint *)(param_4 + 0x90);
  }
  lVar20 = ((ulong)uVar21 & 0x3fffffff) * 4 + lVar12 + lVar20;
  lVar15 = lVar12 + uVar13;
  if (iVar10 != -1) {
    uVar13 = ((ulong)*(uint *)(param_4 + 0x90) & 0x3fffffff) << 2;
  }
  iStack_c = *(int *)(param_4 + 0x2c);
  iVar23 = *(int *)(param_4 + 0x10);
  uVar26 = *(uint *)(param_4 + 0x28);
  uVar21 = *(uint *)(param_4 + 0x3c);
  iVar10 = *(int *)(param_4 + 0x40);
  if (param_5 == (int *)0x0) {
    uVar24 = *(uint *)(param_4 + 0x44);
    uVar25 = *(uint *)(param_4 + 0x48);
  }
  else {
    uVar7 = *param_6 - *param_5;
    uVar26 = (*param_6 + uVar26) - uVar21;
    uVar24 = param_6[2];
    iVar22 = param_6[1] - param_5[1];
    iStack_c = (param_6[1] + iStack_c) - iVar10;
    uVar25 = param_6[3];
    iVar6 = ddp_debug_dbg_log_level();
    iVar10 = iVar22;
    uVar21 = uVar7;
    if ((iVar6 != 0) &&
       (dprec_logger_pr(2,0xffffff8008e457b6,*(undefined4 *)(param_4 + 0x28),
                        *(undefined4 *)(param_4 + 0x2c),*(undefined4 *)(param_4 + 0x3c),
                        *(undefined4 *)(param_4 + 0x40),*(undefined4 *)(param_4 + 0x44),
                        *(undefined4 *)(param_4 + 0x48),uVar26,iStack_c,uVar7,iVar22,uVar24,uVar25),
       iRamffffff8009639114 != 0)) {
      printk(0xffffff8008e457ab,*(undefined4 *)(param_4 + 0x28),*(undefined4 *)(param_4 + 0x2c),
             *(undefined4 *)(param_4 + 0x3c),*(undefined4 *)(param_4 + 0x40),
             *(undefined4 *)(param_4 + 0x44),*(undefined4 *)(param_4 + 0x48),uVar26,iStack_c,uVar7,
             iVar22,uVar24,uVar25);
    }
  }
  if (0xfff < uVar24) {
    printk(0xffffff8008e4576b,0xffffff8008e457f2,0x142);
    warn_slowpath_null(0xffffff8008e457f2,0x142);
  }
  if (0xfff < uVar25) {
    printk(0xffffff8008e4576b,0xffffff8008e457f2,0x144);
    warn_slowpath_null(0xffffff8008e457f2,0x144);
  }
  if ((*(long *)(param_4 + 0x18) == 0) && (*(int *)(param_4 + 0xc) == 0)) {
    dprec_logger_pr(0,0xffffff8008e45886);
    printk(0xffffff8008e45875);
    printk(0xffffff8008e4576b,0xffffff8008e457f2,0x148);
    warn_slowpath_null(0xffffff8008e457f2,0x148);
    return 0xffffffff;
  }
  iVar6 = is_slt_test();
  puVar1 = (uint *)(lVar12 + 0x24);
  iVar22 = 0;
  if (iVar6 == 0) {
    if (param_7 == 0) {
      uVar7 = *puVar1 | 0x4000000;
      goto LAB_ffffff800857cabc;
    }
    do {
      uVar17 = ddp_get_module_va(iVar22);
      if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar14 = ddp_get_module_pa(iVar22);
        lVar14 = ((ulong)puVar1 & 0xfff) + lVar14;
        goto LAB_ffffff800857ce98;
      }
      iVar22 = iVar22 + 1;
    } while (iVar22 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar14 = 0;
LAB_ffffff800857ce98:
    uVar19 = 0x4000000;
LAB_ffffff800857cde0:
    cmdqRecWrite(param_7,lVar14,uVar19,0x4000000);
  }
  else {
    if (param_7 != 0) {
      do {
        uVar17 = ddp_get_module_va(iVar22);
        if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
          lVar14 = ddp_get_module_pa(iVar22);
          lVar14 = ((ulong)puVar1 & 0xfff) + lVar14;
          goto LAB_ffffff800857cdd8;
        }
        iVar22 = iVar22 + 1;
      } while (iVar22 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar14 = 0;
LAB_ffffff800857cdd8:
      uVar19 = 0;
      goto LAB_ffffff800857cde0;
    }
    uVar7 = *puVar1 & 0xfbffffff;
LAB_ffffff800857cabc:
    *puVar1 = uVar7;
    DataSynchronizationBarrier(3,3,0);
  }
  if ((*(int *)(param_4 + 0xc) == 1) && (iVar23 = 0xb00606, *(int *)(param_4 + 0x54) == 0)) {
    dprec_logger_pr(0,0xffffff8008e458bb,param_2);
    printk(0xffffff8008e458aa,param_2);
  }
  iVar6 = ufmt_get_Bpp(iVar23);
  uVar7 = ufmt_get_byteswap(iVar23);
  uVar8 = ufmt_get_format(iVar23);
  iVar9 = ufmt_get_rgb(iVar23);
  puVar1 = (uint *)(lVar18 + 0x3c);
  iVar22 = 0;
  if (param_7 == 0) {
    *puVar1 = uVar21 | iVar10 << 0x10;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    do {
      uVar17 = ddp_get_module_va(iVar22);
      if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar14 = ddp_get_module_pa(iVar22);
        lVar14 = ((ulong)puVar1 & 0xfff) + lVar14;
        goto LAB_ffffff800857cf40;
      }
      iVar22 = iVar22 + 1;
    } while (iVar22 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar14 = 0;
LAB_ffffff800857cf40:
    cmdqRecWrite(param_7,lVar14,uVar21 | iVar10 << 0x10,0xffffffff);
  }
  puVar1 = (uint *)(lVar18 + 0x4c);
  if ((iVar23 == 0x201012 || iVar23 == 0x201213) || (iVar23 == 0x201414 || iVar23 == 0x201615)) {
    uVar21 = uVar26 & 1;
    if ((uVar26 & 1) != 0) {
      uVar26 = uVar26 - 1;
      uVar24 = uVar24 + 1;
    }
    if ((uVar26 + uVar24 & 1) != 0) {
      uVar24 = uVar24 + 1;
      uVar21 = uVar21 | 0x100;
    }
    iVar10 = 0;
    if (param_7 == 0) {
      *puVar1 = uVar21;
      goto LAB_ffffff800857cbb4;
    }
    do {
      uVar17 = ddp_get_module_va(iVar10);
      if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar14 = ddp_get_module_pa(iVar10);
        lVar14 = ((ulong)puVar1 & 0xfff) + lVar14;
        goto LAB_ffffff800857cffc;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar14 = 0;
LAB_ffffff800857cffc:
    cmdqRecWrite(param_7,lVar14,uVar21,0xffffffff);
  }
  else {
    iVar10 = 0;
    if (param_7 != 0) {
      do {
        uVar17 = ddp_get_module_va(iVar10);
        if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
          lVar14 = ddp_get_module_pa(iVar10);
          lVar14 = ((ulong)puVar1 & 0xfff) + lVar14;
          goto LAB_ffffff800857d0b0;
        }
        iVar10 = iVar10 + 1;
      } while (iVar10 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar14 = 0;
LAB_ffffff800857d0b0:
      uVar21 = 0;
      goto LAB_ffffff800857cffc;
    }
    *puVar1 = 0;
LAB_ffffff800857cbb4:
    DataSynchronizationBarrier(3,3,0);
  }
  iVar10 = *(int *)(param_4 + 0x80);
  iVar22 = 6;
  if (((iVar10 != 1) && (iVar22 = 4, iVar10 != 0)) && (iVar22 = 7, iVar10 != 2)) {
    dprec_logger_pr(0,0xffffff8008e4667f);
    iVar22 = 4;
    printk(0xffffff8008e458e2,*(undefined4 *)(param_4 + 0x80));
  }
  iVar10 = 0;
  puVar2 = (undefined4 *)(lVar15 + 0xc0);
  if (param_7 == 0) {
    *puVar2 = 1;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    do {
      uVar17 = ddp_get_module_va(iVar10);
      if (uVar17 == ((ulong)puVar2 & 0xfffffffffffff000)) {
        lVar15 = ddp_get_module_pa(iVar10);
        lVar15 = ((ulong)puVar2 & 0xfff) + lVar15;
        goto LAB_ffffff800857d158;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar2);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar2);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar15 = 0;
LAB_ffffff800857d158:
    cmdqRecWrite(param_7,lVar15,1,0xffffffff);
  }
  uVar21 = (*(uint *)(param_4 + 0xc) & 3) << 0x1c | (*(uint *)(param_4 + 0x54) & 1) << 8 |
           (*(uint *)(param_4 + 0x4c) & 1) << 0x1e | (uint)*(byte *)(param_4 + 0x58) |
           (uVar8 & 0xf) << 0xc | (uVar7 & 1) << 0x18;
  iVar10 = ufmt_is_old_fmt(iVar23);
  if (iVar10 != 0) {
    if ((iVar23 == 0xc00e2a || iVar23 == 0xc00f2b) || (iVar23 == 0xc00d2c || iVar23 == 0xc00c2d)) {
      uVar7 = ufmt_get_rgbswap(iVar23);
      uVar21 = uVar21 | (uVar7 & 1) << 0x19;
    }
    uVar21 = uVar21 | 0x800000;
  }
  puVar1 = (uint *)(lVar18 + 0x30);
  iVar10 = 0;
  uVar7 = uVar21 | iVar22 << 0x10;
  if (iVar9 != 0) {
    uVar7 = uVar21;
  }
  if (param_7 == 0) {
    *puVar1 = uVar7;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lVar20 + 0x25c) = 0xff000000;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lVar18 + 0x38) = uVar24 | uVar25 << 0x10;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    do {
      uVar17 = ddp_get_module_va(iVar10);
      if (uVar17 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar15 = ddp_get_module_pa(iVar10);
        lVar15 = ((ulong)puVar1 & 0xfff) + lVar15;
        goto LAB_ffffff800857d200;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar15 = 0;
LAB_ffffff800857d200:
    cmdqRecWrite(param_7,lVar15,uVar7,0xffffffff);
    iVar10 = 0;
    uVar17 = lVar20 + 0x25c;
    do {
      uVar16 = ddp_get_module_va(iVar10);
      if (uVar16 == (uVar17 & 0xfffffffffffff000)) {
        lVar20 = ddp_get_module_pa(iVar10);
        lVar20 = (uVar17 & 0xfff) + lVar20;
        goto LAB_ffffff800857d240;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar17);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar17);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar20 = 0;
LAB_ffffff800857d240:
    uVar17 = lVar18 + 0x38;
    iVar10 = 0;
    cmdqRecWrite(param_7,lVar20,0xff000000,0xffffffff);
    do {
      uVar16 = ddp_get_module_va(iVar10);
      if (uVar16 == (uVar17 & 0xfffffffffffff000)) {
        lVar20 = ddp_get_module_pa(iVar10);
        lVar20 = (uVar17 & 0xfff) + lVar20;
        goto LAB_ffffff800857d27c;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,uVar17);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar17);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar20 = 0;
LAB_ffffff800857d27c:
    cmdqRecWrite(param_7,lVar20,uVar24 | uVar25 << 0x10,0xffffffff);
  }
  iVar10 = *(int *)(param_4 + 0x38);
  uVar21 = iVar10 * iStack_c + uVar26 * iVar6;
  piVar3 = (int *)(uVar13 + lVar4 + lVar12 + 0xf40);
  if (param_3 == 0) {
    iVar10 = 0;
    if (param_7 == 0) {
      *piVar3 = uVar21 + (int)*(undefined8 *)(param_4 + 0x18);
      DataSynchronizationBarrier(3,3,0);
LAB_ffffff800857cd3c:
      *(undefined4 *)(lVar18 + 0x34) = *(undefined4 *)(param_4 + 0x50);
      DataSynchronizationBarrier(3,3,0);
      goto LAB_ffffff800857cd4c;
    }
    do {
      uVar13 = ddp_get_module_va(iVar10);
      if (uVar13 == ((ulong)piVar3 & 0xfffffffffffff000)) {
        lVar12 = ddp_get_module_pa(iVar10);
        lVar12 = ((ulong)piVar3 & 0xfff) + lVar12;
        goto LAB_ffffff800857d3f4;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,piVar3);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,piVar3);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar12 = 0;
LAB_ffffff800857d3f4:
    cmdqRecWrite(param_7,lVar12,uVar21 + (int)*(undefined8 *)(param_4 + 0x18),0xffffffff);
  }
  else {
    iVar23 = 0;
    uVar11 = module_to_m4u_port(param_1);
    if (*(int *)(param_4 + 0x7c) == 2) {
      do {
        uVar13 = ddp_get_module_va(iVar23);
        if (uVar13 == ((ulong)piVar3 & 0xfffffffffffff000)) {
          lVar12 = ddp_get_module_pa(iVar23);
          lVar12 = ((ulong)piVar3 & 0xfff) + lVar12;
          goto LAB_ffffff800857d5b8;
        }
        iVar23 = iVar23 + 1;
      } while (iVar23 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,piVar3);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,piVar3);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar12 = 0;
LAB_ffffff800857d5b8:
      uVar19 = 1;
      lVar20 = *(long *)(param_4 + 0x18);
    }
    else {
      do {
        uVar13 = ddp_get_module_va(iVar23);
        if (uVar13 == ((ulong)piVar3 & 0xfffffffffffff000)) {
          lVar12 = ddp_get_module_pa(iVar23);
          lVar12 = ((ulong)piVar3 & 0xfff) + lVar12;
          goto LAB_ffffff800857d504;
        }
        iVar23 = iVar23 + 1;
      } while (iVar23 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,piVar3);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,piVar3);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar12 = 0;
LAB_ffffff800857d504:
      uVar19 = 2;
      lVar20 = *(long *)(param_4 + 0x18) + (ulong)uVar21;
      uVar21 = 0;
    }
    cmdqRecWriteSecure(param_7,lVar12,uVar19,lVar20,uVar21,uVar24 * iVar6 + (uVar25 - 1) * iVar10,
                       uVar11);
    if (param_7 == 0) goto LAB_ffffff800857cd3c;
  }
  uVar13 = lVar18 + 0x34;
  iVar10 = 0;
  do {
    uVar17 = ddp_get_module_va(iVar10);
    if (uVar17 == (uVar13 & 0xfffffffffffff000)) {
      lVar12 = ddp_get_module_pa(iVar10);
      lVar12 = (uVar13 & 0xfff) + lVar12;
      goto LAB_ffffff800857d434;
    }
    iVar10 = iVar10 + 1;
  } while (iVar10 != 0x31);
  dprec_logger_pr(0,0xffffff8008e456bf,uVar13);
  printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar13);
  printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
  warn_slowpath_null(0xffffff8008e456e8,0xbb);
  lVar12 = 0;
LAB_ffffff800857d434:
  cmdqRecWrite(param_7,lVar12,*(undefined4 *)(param_4 + 0x50),0xffffffff);
LAB_ffffff800857cd4c:
  uVar21 = *(uint *)(param_4 + 0x60);
  puVar1 = (uint *)(lVar18 + 0x44);
  uVar26 = (uVar21 & 3) << 0x10 | (uVar21 & 3) << 0x12 |
           (uint)*(ushort *)(param_4 + 0x38) | uVar21 << 0x1f |
           (*(uint *)(param_4 + 100) & 3) << 0x14 | (*(uint *)(param_4 + 100) & 3) << 0x16;
  iVar10 = 0;
  uVar21 = uVar26 | 0x10000000;
  if (*(int *)(param_4 + 0x88) == 0) {
    uVar21 = uVar26;
  }
  if (param_7 == 0) {
    *puVar1 = uVar21;
    DataSynchronizationBarrier(3,3,0);
  }
  else {
    do {
      uVar13 = ddp_get_module_va(iVar10);
      if (uVar13 == ((ulong)puVar1 & 0xfffffffffffff000)) {
        lVar18 = ddp_get_module_pa(iVar10);
        lVar18 = ((ulong)puVar1 & 0xfff) + lVar18;
        goto LAB_ffffff800857d6c8;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 != 0x31);
    dprec_logger_pr(0,0xffffff8008e456bf,puVar1);
    printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,puVar1);
    printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
    warn_slowpath_null(0xffffff8008e456e8,0xbb);
    lVar18 = 0;
LAB_ffffff800857d6c8:
    cmdqRecWrite(param_7,lVar18,uVar21,0xffffffff);
  }
  return 0;
}


//// ovl_config_l @ffffff800857e784

undefined8 ovl_config_l(ulong param_1,char *param_2,undefined8 param_3)

{
  if (param_2[4] != '\0') {
    ovl_roi(param_1,*(undefined4 *)(param_2 + 0x95c),*(undefined4 *)(param_2 + 0x960),0xff000000,
            param_3);
  }
  if (*param_2 != '\0') {
    ovl_config_l_part_2(param_1 & 0xffffffff,param_2,param_3);
  }
  return 0;
}


//// ovl_config_l.part.2 @ffffff800857e134

void ovl_config_l_part_2(int param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  ulong uVar7;
  uint uVar8;
  int iVar9;
  int *piVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int *piVar14;
  int *piStack_108;
  int iStack_f0;
  int iStack_ec;
  int iStack_e8;
  int iStack_e4;
  undefined4 uStack_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  undefined1 auStack_d0 [200];
  long lStack_8;

  piStack_108 = (int *)(param_2 + 0x18);
  lStack_8 = lRamffffff80093cff08;
  if (*(int *)(param_2 + 0xc) == 0) goto LAB_ffffff800857e314;
  do {
    piVar14 = piStack_108;
    uVar2 = setup_ovl_sec(param_1,param_2,param_3);
    piVar10 = (int *)(param_2 + 0x440);
    uVar12 = 0;
    uVar11 = 0;
    uVar13 = 0;
    piStack_108 = (int *)0xffffff8008e45c5c;
    do {
      iVar3 = piVar14[2];
      if (iVar3 != 0) {
        if ((((*(long *)(piVar14 + 6) == 0) && (piVar14[3] == 0)) || (piVar14[0x11] == 0)) ||
           (piVar14[0x12] == 0)) {
          dprec_logger_pr(0,0xffffff8008e45c6d,*(long *)(piVar14 + 6),piVar14[0x11],piVar14[0x12]);
          printk(0xffffff8008e45c5c,*(undefined8 *)(piVar14 + 6),piVar14[0x11],piVar14[0x12]);
          printk(0xffffff8008e4576b,0xffffff8008e457f2,0x261);
          warn_slowpath_null(0xffffff8008e457f2,0x261);
          strcpy(auStack_d0,0xffffff8008e45c9d);
          aee_kernel_warning_api(0xffffff8008e457f2,0x3bf,0x20000,auStack_d0,0xffffff8008e45ca1);
          printk(0xffffff8008e45cbc);
        }
        else if (param_1 == *piVar14) {
          if (*(char *)(param_2 + 1) == '\0') {
            print_layer_config_args(param_1,piVar14[0x25],piVar14,0);
            ovl_layer_config(param_1,piVar14[0x25],uVar2,piVar14,0,0,param_3);
            iVar9 = iVar3;
          }
          else {
            iStack_f0 = piVar14[0xf];
            iStack_ec = piVar14[0x10];
            uStack_e0 = 0;
            uStack_dc = 0;
            uStack_d8 = 0;
            uStack_d4 = 0;
            iStack_e8 = piVar14[0x11];
            iStack_e4 = piVar14[0x12];
            iVar4 = rect_intersect(&iStack_f0,piVar10,&uStack_e0);
            iVar9 = 0;
            if (iVar4 != 0) {
              print_layer_config_args(param_1,piVar14[0x25],piVar14,&uStack_e0);
              ovl_layer_config(param_1,piVar14[0x25],uVar2,piVar14,piVar10,&uStack_e0,param_3);
              iVar9 = iVar3;
            }
          }
          uVar8 = piVar14[0x24];
          if (uVar8 == 0xffffffff) {
            uVar12 = uVar12 | iVar9 << (ulong)(piVar14[0x25] & 0x1f);
          }
          else {
            uVar13 = uVar13 | iVar9 << (ulong)(uVar8 & 0x1f);
            uVar11 = uVar11 | piVar14[0x25] << (ulong)((uVar8 + 4) * 4 & 0x1f);
          }
        }
      }
      piVar14 = piVar14 + 0x26;
    } while (piVar10 != piVar14);
    iVar3 = ddp_debug_dbg_log_level();
    if (iVar3 != 0) {
      dprec_logger_pr(2,0xffffff8008e45cef,param_1,uVar12,uVar13,(int)uVar11 >> 0x10);
      if (iRamffffff8009639114 != 0) {
        printk(0xffffff8008e45ce4,param_1,uVar12,uVar13,(int)uVar11 >> 0x10);
      }
    }
    if (param_3 == 0) {
      lVar6 = ovl_base_addr(param_1);
      *(uint *)(lVar6 + 0x2c) = uVar12;
      DataSynchronizationBarrier(3,3,0);
      lVar6 = ovl_base_addr(param_1);
      *(uint *)(lVar6 + 0x324) = uVar13 | uVar11;
      DataSynchronizationBarrier(3,3,0);
    }
    else {
      lVar6 = ovl_base_addr(param_1);
      uVar1 = lVar6 + 0x2c;
      param_2 = uVar1 & 0xfffffffffffff000;
      iVar3 = 0;
      do {
        uVar7 = ddp_get_module_va(iVar3);
        if (uVar7 == param_2) {
          lVar6 = ddp_get_module_pa(iVar3);
          lVar6 = (uVar1 & 0xfff) + lVar6;
          goto LAB_ffffff800857e590;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,uVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar1);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar6 = 0;
LAB_ffffff800857e590:
      cmdqRecWrite(param_3,lVar6,uVar12,0xffffffff);
      lVar6 = ovl_base_addr(param_1);
      uVar1 = lVar6 + 0x324;
      param_1 = 0;
      do {
        uVar7 = ddp_get_module_va(param_1);
        if (uVar7 == (uVar1 & 0xfffffffffffff000)) {
          lVar6 = ddp_get_module_pa(param_1);
          lVar6 = (uVar1 & 0xfff) + lVar6;
          goto LAB_ffffff800857e5d4;
        }
        param_1 = param_1 + 1;
      } while (param_1 != 0x31);
      dprec_logger_pr(0,0xffffff8008e456bf,uVar1);
      printk(0xffffff8008e456a9,0xffffff8008c00538,0xba,uVar1);
      printk(0xffffff8008e4576b,0xffffff8008e456e8,0xbb);
      warn_slowpath_null(0xffffff8008e456e8,0xbb);
      lVar6 = 0;
LAB_ffffff800857e5d4:
      cmdqRecWrite(param_3,lVar6,uVar13 | uVar11,0xffffffff);
    }
    if (lStack_8 == lRamffffff80093cff08) {
      return;
    }
    __stack_chk_fail();
LAB_ffffff800857e314:
    piVar10 = (int *)(param_2 + 0x18);
    iVar3 = 0;
    uVar13 = 0;
    uVar12 = 0;
    uVar11 = 0xffffffff;
    iVar9 = param_1;
    do {
      piVar10[0x24] = -1;
      piVar10[0x25] = 0;
      *(uint *)(param_2 + 0xc) = *(uint *)(param_2 + 0xc) | 1 << (ulong)(uVar12 & 0x1f);
      if (piVar10[2] == 0) {
        uVar13 = uVar13 - 1;
      }
      else {
        iVar4 = is_DAL_Enabled();
        if ((iVar4 == 0) ||
           (iVar4 = piVar10[1], iVar5 = primary_display_get_option(0xffffff8008e45bbd),
           iVar4 != iVar5)) {
          iVar4 = disp_helper_get_option(0x2a);
          uVar8 = uVar13;
          if (iVar4 == 0) {
LAB_ffffff800857e40c:
            iVar4 = -1;
            uVar11 = uVar8;
          }
          else {
            uVar8 = piVar10[0x23];
            if (uVar8 == 0xffffffff) {
              uVar11 = uVar11 + 1;
              iVar4 = -1;
            }
            else {
              if (uVar8 != uVar11) {
                dprec_logger_pr(0,0xffffff8008e45bdb,uVar12,uVar11,uVar8);
                printk(0xffffff8008e45bca,uVar12,uVar11,piVar10[0x23]);
                uVar8 = uVar11 + 1;
                goto LAB_ffffff800857e40c;
              }
              iVar4 = iVar3;
              iVar3 = iVar3 + 1;
            }
          }
          if (iVar9 == 0) {
            if (3 < uVar11) {
              iVar3 = 0;
              uVar11 = 0;
              uVar13 = 0;
              iVar4 = -1;
              iVar9 = 2;
            }
          }
          else {
            dprec_logger_pr(0,0xffffff8008e45692,iVar9);
            iVar3 = 0;
            printk(0xffffff8008e45681,iVar9);
            uVar11 = 0;
            uVar13 = 0;
            iVar4 = -1;
            dprec_logger_pr(0,0xffffff8008e45c11,iVar9);
            printk(0xffffff8008e45c00,iVar9);
          }
          *piVar10 = iVar9;
          piVar10[0x24] = iVar4;
          piVar10[0x25] = uVar11;
          iVar5 = ddp_debug_dbg_log_level();
          if (iVar5 != 0) {
            uVar8 = uVar11;
            if (iVar4 < 0) {
              uVar8 = piVar10[0x23];
            }
            dprec_logger_pr(2,0xffffff8008e45c30,uVar12,iVar9,uVar11,iVar4,uVar8);
            if (iRamffffff8009639114 != 0) {
              uVar8 = uVar11;
              if (iVar4 < 0) {
                uVar8 = piVar10[0x23];
              }
              printk(0xffffff8008e45c25,uVar12,iVar9,uVar11,iVar4,uVar8);
            }
          }
        }
        else {
          *piVar10 = 0;
          piVar10[0x25] = 3;
        }
      }
      uVar12 = uVar12 + 1;
      uVar13 = uVar13 + 1;
      piVar10 = piVar10 + 0x26;
    } while (uVar12 != 7);
  } while( true );
}


//// m4u_config_port @ffffff8008487c74

undefined8 m4u_config_port(uint *param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  uint uVar9;
  uint uVar10;
  byte bVar11;
  uint uVar12;
  ulong uVar13;
  uint uVar14;

  uVar9 = *param_1;
  if (((int)uVar9 < 0) || ((int)uRamffffff800944a22c <= (int)uVar9)) {
    printk(0xffffff8008e0b4b9,0xffffff8008bd0f50,uVar9);
    uVar12 = uRamffffff800944a22c;
  }
  else {
    uVar12 = *(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc8) & 3;
  }
  uVar5 = ~uVar9 >> 0x1f;
  bVar11 = 0xff;
  if ((uVar5 & (int)uVar9 < (int)uRamffffff800944a22c) != 0) {
    bVar11 = *(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc8) >> 4;
  }
  if (uVar12 == 0) {
    lVar6 = sched_clock();
    larb_clock_on_isra_2(bVar11);
    lVar7 = sched_clock();
    if (((50000000 < (ulong)(lVar7 - lVar6)) && (iRamffffff800944a974 < 3)) &&
       (iRamffffff800944a970 < 3)) {
      printk(0xffffff8008e0b7bd,bVar11,1);
    }
  }
  uVar2 = param_1[1];
  uVar3 = param_1[2];
  uVar10 = param_1[4];
  uVar4 = param_1[5];
  if (((int)uVar9 < 0) || ((int)uRamffffff800944a22c <= (int)uVar9)) {
    printk(0xffffff8008e0b4b9,0xffffff8008bd0f50,uVar9);
    uVar14 = uRamffffff800944a22c;
  }
  else {
    uVar14 = *(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc8) & 3;
  }
  if (((uVar2 == 0 || uVar3 == 1) && (iRamffffff800944a974 < 3)) && (iRamffffff800944a970 < 3)) {
    if ((uVar5 & (int)uVar9 < (int)uRamffffff800944a22c) == 0) {
      uVar8 = 0xffffff8008e0b204;
    }
    else {
      uVar8 = *(undefined8 *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dd0);
    }
    printk(0xffffff8008e0b7ea,uVar8,uVar2,uVar3);
  }
  if (uVar4 == 0 && uVar10 == 1) {
    m4u_invalid_prog_dist_by_id(uVar9);
  }
  else {
    m4u_config_prog_dist(uVar9,uVar4,uVar10,1,0,0);
  }
  if (uVar2 == 0) {
    m4u_invalid_prog_dist_by_id(uVar9);
  }
  _raw_spin_lock(0xffffff80095cd240);
  if (uVar14 == 0) {
    uVar13 = 0xff;
    if ((uVar5 & (int)uVar9 < (int)uRamffffff800944a22c) != 0) {
      uVar13 = (ulong)(*(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc8) >> 4);
    }
    if (uVar9 < 0x1d) {
      uVar10 = (uint)*(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc7);
    }
    else {
      printk(0xffffff8008e0b4b9,0xffffff8008bd0f68,uVar9);
      uVar10 = uRamffffff800944a22c;
    }
    puVar1 = (uint *)((ulong)(uVar10 * 4 + 0x380) + *(long *)(uVar13 * 8 + -0x7ff6a32db8));
    DataSynchronizationBarrier(3,1,0);
    *puVar1 = (uint)(uVar2 != 0) | *puVar1 & 0xfffffffe;
    DataSynchronizationBarrier(3,3,0);
    DataSynchronizationBarrier(3,1,0);
    if ((uVar2 != (*puVar1 & 1)) && (iRamffffff800944a974 < 3)) {
      if (iRamffffff800944a970 < 3) {
        if ((uVar5 & (int)uVar9 < (int)uRamffffff800944a22c) == 0) {
          uVar8 = 0xffffff8008e0b204;
        }
        else {
          uVar8 = *(undefined8 *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dd0);
        }
        DataSynchronizationBarrier(3,1,0);
        printk(0xffffff8008e0b80a,uVar8,uVar2,*puVar1 & 1,*puVar1,1);
      }
      else {
        DataSynchronizationBarrier(3,1,0);
      }
    }
  }
  else {
    if (uVar9 < 0x1d) {
      uVar9 = (uint)*(byte *)((long)(int)uVar9 * 0x30 + -0x7ff6bb5dc7);
    }
    else {
      printk(0xffffff8008e0b4b9,0xffffff8008bd0f68,uVar9);
      uVar9 = uRamffffff800944a22c;
    }
    DataSynchronizationBarrier(3,1,0);
    *(uint *)(lRamffffff80095cd260 + 0x208) =
         *(uint *)(lRamffffff80095cd260 + 0x208) & (1 << (ulong)(uVar9 & 0x1f) ^ 0xffffffffU) |
         (uint)(uVar2 != 0) << (ulong)(uVar9 & 0x1f);
    DataSynchronizationBarrier(3,3,0);
  }
  _raw_spin_unlock(0xffffff80095cd240);
  if (uVar12 == 0) {
    lVar6 = sched_clock();
    larb_clock_off_isra_3(bVar11);
    lVar7 = sched_clock();
    if (((50000000 < (ulong)(lVar7 - lVar6)) && (iRamffffff800944a974 < 3)) &&
       (iRamffffff800944a970 < 3)) {
      printk(0xffffff8008e0b7bd,bVar11,0);
    }
  }
  return 0;
}


//// m4u_config_port_ext @ffffff80084880f0

void m4u_config_port_ext(void)

{
  m4u_config_port();
  return;
}


//// m4u_hw_init @ffffff80084895e4

void m4u_hw_init(long param_1,int param_2)

{
  ulong uVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined4 uVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  long *plVar9;
  ulong uVar10;
  ulong uVar11;
  long lVar12;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  long lStack_8;

  plVar9 = (long *)0xffffff80093cf000;
  uVar10 = (ulong)param_2;
  lStack_8 = lRamffffff80093cff08;
  printk(0xffffff8008e0c3ec,uRamffffff80095cd5f8);
  *(undefined8 *)(uVar10 * 8 + -0x7ff6a32dc8) = *(undefined8 *)(param_1 + uVar10 * 8 + 0x68);
  lVar3 = kmem_cache_alloc_trace(uRamffffff8009597940,0x24080c0,0x100);
  lVar7 = -0x7ff6a33000;
  lVar4 = -0x7ff6a69000;
  if (lVar3 != 0) goto LAB_ffffff800848968c;
  printk(0xffffff8008e0c409);
  lVar8 = -0x7ff6a33000;
  uVar11 = uVar10;
  lVar12 = -0x7ff6a69000;
LAB_ffffff800848966c:
  do {
    uVar5 = 0xffffffff;
    while( true ) {
      plVar9 = plVar9 + 0x1e1;
      if (lStack_8 == *plVar9) {
        return;
      }
      lVar3 = __stack_chk_fail(uVar5);
      lVar7 = lVar8;
      uVar10 = uVar11;
      lVar4 = lVar12;
LAB_ffffff800848968c:
      uVar1 = lVar3 + 0x7f;
      if ((uVar1 >> 0x26 & 1) == 0) {
        uVar6 = (uVar1 & 0xffffffffffffff80) - lRamffffff80093cff48;
      }
      else {
        uVar6 = (uVar1 & 0x3fffffff80) + lRamffffff80093cff40;
      }
      uVar11 = uVar10;
      if ((uVar6 & 0x7f) != 0) {
        printk(0xffffff8008e0c431,uVar6);
        lVar8 = lVar7;
        lVar12 = lVar4;
        goto LAB_ffffff800848966c;
      }
      lVar12 = lVar4 + 0x900;
      printk(0xffffff8008e0c45c,uVar1 & 0xffffffffffffff80,uVar6);
      lVar8 = lVar7 + 0x238;
      lVar4 = kmem_cache_alloc_trace(*(undefined8 *)(lVar4 + 0x948),0x24080c0,400);
      *(long *)(lVar7 + 0x268) = lVar4;
      if (lVar4 == 0) break;
      *(undefined2 *)(lVar7 + 0x240) = 0;
      *(undefined2 *)(lVar7 + 0x242) = 0;
      m4u_reg_init(lVar7 + 0x578,uVar6,uVar10 & 0xffffffff);
      init_timer_key(lVar7 + 0x5c8,0,0,0);
      *(code **)(lVar7 + 0x5e0) = m4u_isr_restart;
      lVar8 = 0xfffff;
      m4u_monitor_start(uVar10 & 0xffffffff);
      mau_start_monitor(0,0,0,1,1,0,0,0,0xfffff,0xffffffff,0xffffffff);
      mau_start_monitor(0,0,1,0,1,0,0,0,0xfffff,0xffffffff,0xffffffff);
      if ((int)uVar10 == 0) {
        uStack_1c = 1;
        uStack_18 = 0;
        uStack_14 = 0;
        uStack_10 = 1;
        uStack_20 = 3;
        uStack_c = 0;
        m4u_config_port(&uStack_20);
        uStack_20 = 4;
        m4u_config_port(&uStack_20);
        uStack_20 = 5;
        m4u_config_port(&uStack_20);
      }
      uVar11 = param_1 + uVar10 * 4;
      iVar2 = request_threaded_irq
                        (*(undefined4 *)(uVar11 + 0x70),MTK_M4U_isr,0,0,0xffffff8008e0c4b8,0);
      if (iVar2 == 0) {
        printk(0xffffff8008e0c4e9,*(undefined4 *)(uVar11 + 0x70));
        uVar5 = 0;
      }
      else {
        printk(0xffffff8008e0c4bc,uVar10 & 0xffffffff);
        uVar5 = 0xffffffed;
        aee_kernel_exception_api
                  (0xffffff8008e0b40e,0x88b,0,0xffffff8008e58642,0xffffff8008e0c4be,
                   uVar10 & 0xffffffff);
      }
    }
    printk(0xffffff8008e0c488,400);
  } while( true );
}
