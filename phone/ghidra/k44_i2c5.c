//// __mt_i2c_transfer @ffffff80088395d0

void __mt_i2c_transfer(long param_1,short *param_2,int param_3)

{
  undefined1 *puVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  ulong uVar5;
  bool bVar6;
  char cVar7;
  undefined4 uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  ulong uVar12;
  short *psVar13;
  ushort *puVar14;
  byte *pbVar15;
  byte *pbVar16;
  undefined8 uVar17;
  long lVar18;
  undefined2 *puVar19;
  ushort uVar20;
  short sVar21;
  undefined8 uVar22;
  undefined2 uVar23;
  long lVar24;
  undefined1 *puVar25;
  short *psVar26;
  int iVar27;
  undefined4 auStack_30 [6];
  undefined8 ****ppppuStack_18;
  undefined8 ****ppppuStack_10;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  iVar4 = param_3;
LAB_ffffff8008839624:
  do {
    psVar26 = param_2;
    iVar27 = iVar4 + -1;
    if (iVar4 == 0) {
LAB_ffffff800883a660:
      while (lStack_8 != lRamffffff80093cff08) {
        __stack_chk_fail(param_3);
LAB_ffffff800883a680:
        DataSynchronizationBarrier(3,2,0);
        uVar23 = 1;
        puVar19 = (undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x38);
LAB_ffffff800883a484:
        *puVar19 = uVar23;
        DataSynchronizationBarrier(3,1,0);
        printk(0xffffff8008ee1f8f,*(undefined2 *)(*(long *)(param_1 + 0x3a8) + 4));
        DataSynchronizationBarrier(3,2,0);
        lVar18 = *(long *)(param_1 + 0x3a8);
        *(undefined2 *)(lVar18 + 8) = 0;
        uVar23 = *(undefined2 *)(lVar18 + 0xc);
        DataSynchronizationBarrier(3,1,0);
        *(undefined2 *)(param_1 + 0x3f8) = uVar23;
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0xc) = uVar23;
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x50) = 1;
        bVar6 = *(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02';
        if (*(char *)(param_1 + 0x3f2) == '\0') {
          uVar11 = 0x40;
          if (bVar6) {
            uVar11 = 0x34;
          }
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) = 3;
        }
        else {
          uVar11 = 0x40;
          if (bVar6) {
            uVar11 = 0x34;
          }
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) = 0;
        }
        if ((*(char *)(param_1 + 0x3f1) != '\0') &&
           (*(char *)(*(long *)(param_1 + 0x470) + 6) != '\x02')) {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x54) = 0;
        }
        if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
          DataSynchronizationBarrier(3,2,0);
          *(ushort *)(*(long *)(param_1 + 0x3a8) + 0x20) = *(ushort *)(param_1 + 0x42c) | 1;
        }
        else {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x20) = *(undefined2 *)(param_1 + 0x42c);
        }
        if (*(char *)(*(long *)(param_1 + 0x470) + 4) != '\0') {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x2c) = *(undefined2 *)(param_1 + 0x42e);
        }
        uVar11 = 0x48;
        if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
          uVar11 = 0x30;
        }
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) =
             *(undefined2 *)(param_1 + 0x430);
        DataSynchronizationBarrier(3,2,0);
        *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0xc) = 1;
        __const_udelay(0x53e3);
        DataSynchronizationBarrier(3,1,0);
        if (*(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 8) != 0) {
          dev_err(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee16d9);
          dump_dma_regs();
          warn_slowpath_null(0xffffff8008ee16e9,0x1c1);
        }
        if (*(int *)(param_1 + 0x5c4) != 0) {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x24) = 2;
        }
        param_3 = -0x6e;
        if (((ulong)psVar26 & 1) != 0) {
          param_3 = -5;
          dev_err(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1fab);
        }
      }
      return;
    }
    if (((0x1000 < (ushort)psVar26[2]) || (*psVar26 == 0)) || (*(long *)(psVar26 + 4) == 0)) {
LAB_ffffff80088397c4:
      param_3 = -0x16;
      goto LAB_ffffff800883a660;
    }
    *(short *)(param_1 + 0x42a) = *psVar26;
    *(short *)(param_1 + 0x41a) = psVar26[2];
    *(undefined2 *)(param_1 + 0x428) = 0;
    if ((iVar4 == param_3) || (cVar7 = mt_i2c_should_batch(psVar26 + -8,psVar26), cVar7 == '\0')) {
      *(short *)(param_1 + 0x418) = psVar26[2];
      uVar8 = 2;
      if ((psVar26[1] & 1U) == 0) {
        uVar8 = 1;
      }
      *(undefined4 *)(param_1 + 0x414) = uVar8;
    }
    else {
      *(short *)(param_1 + 0x418) = *(short *)(param_1 + 0x418) + psVar26[2];
    }
    if ((psVar26[1] & 1U) == 0) {
      __memcpy(*(long *)(param_1 + 0x3c0) +
               ((ulong)*(ushort *)(param_1 + 0x418) - (ulong)(ushort)psVar26[2]),
               *(undefined8 *)(psVar26 + 4));
    }
    if (iVar27 < 1) {
LAB_ffffff8008839728:
      bVar6 = true;
      *(undefined1 *)(param_1 + 0x410) = 0;
      *(undefined2 *)(param_1 + 0x3f8) = 0;
      iVar4 = *(int *)(param_1 + 0x40);
      if (*(ushort *)(param_1 + 0x418) < 9) {
        bVar6 = 8 < *(ushort *)(param_1 + 0x428);
      }
      if ((*(char *)(param_1 + 0x460) == '\0') || (uVar11 = *(uint *)(param_1 + 0x464), uVar11 == 0)
         ) {
        uVar11 = *(uint *)(param_1 + 0x3fc);
      }
      if (*(char *)(param_1 + 0x462) == '\0') {
        *(undefined4 *)(param_1 + 0x5c4) = *(undefined4 *)(param_1 + 0x5c0);
        *(undefined4 *)(param_1 + 0x5cc) = *(undefined4 *)(param_1 + 0x5c8);
      }
      else {
        *(uint *)(param_1 + 0x5c4) = (uint)*(ushort *)(param_1 + 0x468);
        *(uint *)(param_1 + 0x5cc) = (uint)*(ushort *)(param_1 + 0x46a);
        if (*(ushort *)(param_1 + 0x468) == 0) {
          _dev_info(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1f1d);
          *(undefined4 *)(param_1 + 0x5c4) = *(undefined4 *)(param_1 + 0x5d4);
        }
      }
      uVar12 = clk_get_rate(*(undefined8 *)(param_1 + 0x3d0));
      uVar5 = 0;
      if ((ulong)*(uint *)(param_1 + 0x400) != 0) {
        uVar5 = uVar12 / *(uint *)(param_1 + 0x400);
      }
      iVar9 = i2c_set_speed(param_1,uVar5);
      if (iVar9 != 0) {
        uVar17 = 0xffffff8008ee16aa;
LAB_ffffff80088397bc:
        dev_err(*(undefined8 *)(param_1 + 0x388),uVar17);
        goto LAB_ffffff80088397c4;
      }
      if ((*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') &&
         (*(short *)(param_1 + 0x42e) != 0)) {
        uVar12 = clk_get_rate(*(undefined8 *)(param_1 + 0x3d0));
        uVar5 = 0;
        if ((ulong)*(uint *)(param_1 + 0x400) != 0) {
          uVar5 = uVar12 / *(uint *)(param_1 + 0x400);
        }
        iVar9 = (*(ushort *)(param_1 + 0x42e) & 0x3f) + 1;
        uVar10 = iVar9 + (*(ushort *)(param_1 + 0x42e) >> 6 & 7) * iVar9;
        uVar23 = 0;
        if (uVar10 != 0) {
          uVar23 = (undefined2)(((uVar10 - 1) + (int)(uVar5 / 1000) * 2) / uVar10);
        }
        sVar21 = 0xfff;
        if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
          sVar21 = 0x4c;
        }
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)
         (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21)) =
             uVar23;
      }
      if (*(char *)(*(long *)(param_1 + 0x470) + 2) != '\0') {
        if (8 < *(uint *)(param_1 + 0x400)) {
          uVar17 = 0xffffff8008ee1f45;
          goto LAB_ffffff80088397bc;
        }
        sVar21 = 0x70;
        if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
          sVar21 = 0x48;
        }
        DataSynchronizationBarrier(3,2,0);
        *(short *)(*(long *)(param_1 + 0x3a8) +
                  (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21)) =
             ((short)*(uint *)(param_1 + 0x400) + -1) * 0x101;
      }
      if ((*(char *)(param_1 + 0x3f0) != '\0') &&
         (*(char *)(*(long *)(param_1 + 0x470) + 6) != '\x02')) {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)
         (*(long *)(param_1 + 0x3a8) +
         (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x60)) = 1;
      }
      uVar10 = 0x32c;
      if (!bVar6) {
        uVar10 = 0x28;
      }
      uVar3 = uVar10 | 2;
      if (uVar11 < 0x61a81) {
        uVar3 = uVar10;
      }
      uVar10 = uVar3 | 0x12;
      if (*(int *)(param_1 + 0x414) != 3) {
        uVar10 = uVar3;
      }
      DataSynchronizationBarrier(3,2,0);
      *(short *)(*(long *)(param_1 + 0x3a8) +
                (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x10)) = (short)uVar10;
      sVar21 = (short)*(undefined4 *)(param_1 + 0x5c4);
      if (uVar11 < 0x186a1) {
        DataSynchronizationBarrier(3,2,0);
        psVar13 = (short *)(*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(sVar21 + 0x28));
        sVar21 = -0x7fff;
      }
      else {
        uVar20 = sVar21 + 0x28;
        sVar21 = *(short *)(*(long *)(param_1 + 0x470) + 8);
        if (sVar21 == 0) {
          DataSynchronizationBarrier(3,2,0);
          psVar13 = (short *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar20);
          sVar21 = 0x1800;
        }
        else {
          DataSynchronizationBarrier(3,2,0);
          psVar13 = (short *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar20);
        }
      }
      *psVar13 = sVar21;
      if ((uVar10 >> 1 & 1) == 0) {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)
         (*(long *)(param_1 + 0x3a8) +
         (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x1c)) = 2;
      }
      uVar20 = 0;
      if ((*(char *)(param_1 + 0x3f2) == '\0') &&
         (uVar20 = 3, *(char *)(*(long *)(param_1 + 0x470) + 5) != '\0')) {
        uVar20 = (ushort)((*(uint *)(param_1 + 0x404) & 0xfff) << 4) | 3;
      }
      sVar21 = 0x40;
      if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
        sVar21 = 0x34;
      }
      DataSynchronizationBarrier(3,2,0);
      *(ushort *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21)) = uVar20
      ;
      if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
        uVar20 = *(ushort *)(param_1 + 0x42c) | 1;
      }
      else {
        uVar20 = *(ushort *)(param_1 + 0x42c);
      }
      DataSynchronizationBarrier(3,2,0);
      *(ushort *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x20)
       ) = uVar20;
      if (*(char *)(*(long *)(param_1 + 0x470) + 4) != '\0') {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)
         (*(long *)(param_1 + 0x3a8) +
         (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x2c)) =
             *(undefined2 *)(param_1 + 0x42e);
      }
      sVar21 = 0x48;
      if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
        sVar21 = 0x30;
      }
      DataSynchronizationBarrier(3,2,0);
      *(undefined2 *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21)) =
           *(undefined2 *)(param_1 + 0x430);
      if ((*(char *)(param_1 + 0x3f1) != '\0') &&
         (*(char *)(*(long *)(param_1 + 0x470) + 6) != '\x02')) {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)
         (*(long *)(param_1 + 0x3a8) +
         (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x54)) = 7;
      }
      uVar2 = *(short *)(param_1 + 0x42a) << 1;
      uVar20 = uVar2 | 1;
      if (*(int *)(param_1 + 0x414) != 2) {
        uVar20 = uVar2;
      }
      DataSynchronizationBarrier(3,2,0);
      *(ushort *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 4)) =
           uVar20;
      uVar20 = 0x12f;
      if (*(char *)(*(long *)(param_1 + 0x470) + 6) != '\x02') {
        uVar20 = 0xf;
      }
      uVar2 = uVar20 & 0xfff9;
      if (*(int *)(param_1 + 0x5c4) == 0) {
        uVar2 = uVar20;
      }
      DataSynchronizationBarrier(3,2,0);
      *(undefined2 *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)((short)*(int *)(param_1 + 0x5c4) + 0xc)) =
           0x1ff;
      if (*(int *)(param_1 + 0x5c4) == 0) {
        DataSynchronizationBarrier(3,2,0);
        uVar23 = 1;
        puVar19 = (undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x38);
      }
      else {
        DataSynchronizationBarrier(3,2,0);
        puVar19 = (undefined2 *)
                  (*(long *)(param_1 + 0x3a8) +
                  (ulong)(ushort)((short)*(int *)(param_1 + 0x5c4) + 0x38));
        uVar23 = 5;
      }
      *puVar19 = uVar23;
      DataSynchronizationBarrier(3,2,0);
      *(ushort *)
       (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 8)) =
           uVar2;
      sVar21 = (short)*(undefined4 *)(param_1 + 0x5c4);
      if (*(int *)(param_1 + 0x414) == 3) {
        if ((*(char *)(param_1 + 0x3f3) == '\0') ||
           (*(char *)(*(long *)(param_1 + 0x470) + 1) == '\0')) {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(sVar21 + 0x14)) =
               *(undefined2 *)(param_1 + 0x41a);
          sVar21 = 0x6c;
          if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
            sVar21 = 0x44;
          }
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)
           (*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21)) =
               *(undefined2 *)(param_1 + 0x428);
        }
        else {
          DataSynchronizationBarrier(3,2,0);
          *(ushort *)(*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(sVar21 + 0x14)) =
               (*(ushort *)(param_1 + 0x428) & 0x1f) << 8 | (ushort)*(byte *)(param_1 + 0x41a);
        }
        DataSynchronizationBarrier(3,2,0);
        puVar14 = (ushort *)
                  (*(long *)(param_1 + 0x3a8) +
                  (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x18));
        uVar20 = 2;
      }
      else if (*(int *)(param_1 + 0x414) == 4) {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(sVar21 + 0x14U)) =
             *(undefined2 *)(param_1 + 0x41a);
        uVar20 = 0;
        if (*(ushort *)(param_1 + 0x41a) != 0) {
          uVar20 = *(ushort *)(param_1 + 0x418) / *(ushort *)(param_1 + 0x41a);
        }
        DataSynchronizationBarrier(3,2,0);
        puVar14 = (ushort *)
                  (*(long *)(param_1 + 0x3a8) +
                  (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x18));
      }
      else {
        DataSynchronizationBarrier(3,2,0);
        *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)(ushort)(sVar21 + 0x14U)) =
             *(undefined2 *)(param_1 + 0x41a);
        DataSynchronizationBarrier(3,2,0);
        puVar14 = (ushort *)
                  (*(long *)(param_1 + 0x3a8) +
                  (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x18));
        uVar20 = 1;
      }
      *puVar14 = uVar20;
      iVar9 = *(int *)(param_1 + 0x414);
      if ((bVar6) && (*(char *)(param_1 + 0x46d) == '\0')) {
        if (iVar9 != 2) {
          if (iVar9 != 1 && iVar9 != 4) {
            DataSynchronizationBarrier(3,2,0);
            *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc)) = 0;
            DataSynchronizationBarrier(3,2,0);
            *(uint *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x18) =
                 (uint)(iVar9 == 1 || iVar9 == 4);
            DataSynchronizationBarrier(3,2,0);
            *(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x1c) =
                 (int)*(undefined8 *)(param_1 + 0x3c8);
            DataSynchronizationBarrier(3,2,0);
            *(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x20) =
                 (int)*(undefined8 *)(param_1 + 0x3c8);
            if (1 < **(byte **)(param_1 + 0x470)) {
              DataSynchronizationBarrier(3,2,0);
              *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x54)
                   = *(undefined4 *)(param_1 + 0x3cc);
              DataSynchronizationBarrier(3,2,0);
              *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x58)
                   = *(undefined4 *)(param_1 + 0x3cc);
            }
            DataSynchronizationBarrier(3,2,0);
            *(uint *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x24) =
                 (uint)*(ushort *)(param_1 + 0x41a);
            uVar20 = *(ushort *)(param_1 + 0x428);
            goto LAB_ffffff8008839c74;
          }
          DataSynchronizationBarrier(3,2,0);
          *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc)) = 0;
          DataSynchronizationBarrier(3,2,0);
          *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x18) = 0
          ;
          DataSynchronizationBarrier(3,2,0);
          *(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x1c) =
               (int)*(undefined8 *)(param_1 + 0x3c8);
          if (1 < **(byte **)(param_1 + 0x470)) {
            DataSynchronizationBarrier(3,2,0);
            *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x54) =
                 *(undefined4 *)(param_1 + 0x3cc);
          }
          uVar20 = *(ushort *)(param_1 + 0x418);
          DataSynchronizationBarrier(3,2,0);
          lVar18 = (ulong)*(uint *)(param_1 + 0x5cc) + 0x24;
        }
        else {
          DataSynchronizationBarrier(3,2,0);
          *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc)) = 0;
          DataSynchronizationBarrier(3,2,0);
          *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x18) = 1
          ;
          DataSynchronizationBarrier(3,2,0);
          *(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x20) =
               (int)*(undefined8 *)(param_1 + 0x3c8);
          if (1 < **(byte **)(param_1 + 0x470)) {
            DataSynchronizationBarrier(3,2,0);
            *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x58) =
                 *(undefined4 *)(param_1 + 0x3cc);
          }
          uVar20 = *(ushort *)(param_1 + 0x41a);
LAB_ffffff8008839c74:
          DataSynchronizationBarrier(3,2,0);
          lVar18 = (ulong)*(uint *)(param_1 + 0x5cc) + 0x28;
        }
        *(uint *)(*(long *)(param_1 + 0x3b0) + lVar18) = (uint)uVar20;
        lVar18 = *(long *)(param_1 + 0x3b0);
        lVar24 = (long)*(int *)(param_1 + 0x3bc) * 0x50;
        *(long *)(lVar24 + -0x7ff67e4b50) = lVar18;
        DataSynchronizationBarrier(3,1,0);
        uVar11 = *(uint *)(param_1 + 0x5cc);
        *(undefined4 *)(lVar24 + -0x7ff67e4b48) =
             *(undefined4 *)(lVar18 + (ulong)*(uint *)(param_1 + 0x5cc));
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b44) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)uVar11 + 4);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b40) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 8);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b3c) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0xc);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b38) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x10);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b34) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x14);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b30) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x18);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b2c) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x1c);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b28) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x20);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b24) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x24);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b20) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x28);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b1c) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x38);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b18) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x50);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b14) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x54);
        DataSynchronizationBarrier(3,1,0);
        *(undefined4 *)((long)*(int *)(param_1 + 0x3bc) * 0x50 + -0x7ff67e4b10) =
             *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0x58);
        DataSynchronizationBarrier(3,3,0);
        DataSynchronizationBarrier(3,2,0);
        *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 8) = 1;
      }
      else if ((iVar9 != 2) && (*(char *)(param_1 + 0x46d) == '\0')) {
        uVar20 = *(ushort *)(param_1 + 0x418);
        pbVar15 = *(byte **)(param_1 + 0x3c0);
        pbVar16 = pbVar15;
        while (pbVar16 != pbVar15 + uVar20) {
          DataSynchronizationBarrier(3,2,0);
          *(ushort *)(*(long *)(param_1 + 0x3a8) + (ulong)*(ushort *)(param_1 + 0x5c4)) =
               (ushort)*pbVar16;
          pbVar16 = pbVar16 + 1;
        }
      }
      if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
        if (*(char *)(param_1 + 0x46d) == '\0') {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)
           (*(long *)(param_1 + 0x3a8) +
           (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x40)) = 1;
          goto LAB_ffffff800883a104;
        }
        _dev_info(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1f56);
      }
      else {
LAB_ffffff800883a104:
        DataSynchronizationBarrier(3,3,0);
        if (*(char *)(param_1 + 0x46c) == '\0') {
          DataSynchronizationBarrier(3,2,0);
          *(undefined2 *)
           (*(long *)(param_1 + 0x3a8) +
           (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x24)) = 1;
          lVar24 = (long)iVar4;
          lVar18 = lVar24;
          if ((*(byte *)(param_1 + 0x410) & lVar24 == 0) != 0) {
            lVar18 = 1;
          }
          if (*(byte *)(param_1 + 0x410) == 0 && lVar18 != 0) {
            auStack_30[0] = 0;
            ppppuStack_18 = &ppppuStack_18;
            ppppuStack_10 = &ppppuStack_18;
            while( true ) {
              prepare_to_wait_event(param_1 + 0x390,auStack_30,2);
              if ((*(byte *)(param_1 + 0x410) & lVar24 == 0) != 0) {
                lVar24 = 1;
              }
              if (*(byte *)(param_1 + 0x410) != 0 || lVar24 == 0) break;
              lVar24 = schedule_timeout(lVar24);
            }
            finish_wait(param_1 + 0x390,auStack_30);
            lVar18 = lVar24;
          }
          DataSynchronizationBarrier(3,1,0);
          lVar24 = param_1 + (-(ulong)(*(uint *)(param_1 + 0x478) >> 0x1f) & 0xffffffe000000000 |
                             (ulong)*(uint *)(param_1 + 0x478) << 5);
          *(uint *)(lVar24 + 0x480) =
               (uint)*(ushort *)
                      (*(long *)(param_1 + 0x3a8) +
                      (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 4));
          *(uint *)(lVar24 + 0x484) = (uint)*(ushort *)(param_1 + 0x3f8);
          DataSynchronizationBarrier(3,1,0);
          *(uint *)(lVar24 + 0x488) =
               (uint)*(ushort *)
                      (*(long *)(param_1 + 0x3a8) +
                      (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) + 0x10));
          sVar21 = 0x30;
          if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
            sVar21 = 0xf4;
          }
          DataSynchronizationBarrier(3,1,0);
          *(uint *)(lVar24 + 0x48c) =
               (uint)*(ushort *)
                      (*(long *)(param_1 + 0x3a8) +
                      (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21));
          sVar21 = 100;
          if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
            sVar21 = 0xe4;
          }
          DataSynchronizationBarrier(3,1,0);
          *(uint *)(lVar24 + 0x490) =
               (uint)*(ushort *)
                      (*(long *)(param_1 + 0x3a8) +
                      (ulong)(ushort)(*(short *)(param_1 + 0x5c4) + sVar21));
          *(int *)(lVar24 + 0x494) = (int)lVar18;
          uVar17 = sched_clock();
          *(undefined8 *)(lVar24 + 0x498) = uVar17;
          iVar4 = *(int *)(param_1 + 0x478) + 1;
          if (iVar4 == 10) {
            iVar4 = 0;
          }
          *(int *)(param_1 + 0x478) = iVar4;
          if ((int)lVar18 == 0) {
            dev_err(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1f73,
                    *(undefined2 *)(param_1 + 0x42a));
            psVar26 = (short *)(ulong)*(ushort *)
                                       (*(long *)(param_1 + 0x3a8) +
                                       (ulong)(ushort)((short)*(undefined4 *)(param_1 + 0x5c4) +
                                                      0x24));
            DataSynchronizationBarrier(3,1,0);
            i2c_dump_info(param_1);
            mt_irq_dump_status(*(undefined4 *)(param_1 + 0x3b8));
            dump_cg_regs(param_1);
            if (*(int *)(param_1 + 0x5c4) == 0) goto LAB_ffffff800883a680;
            DataSynchronizationBarrier(3,2,0);
            puVar19 = (undefined2 *)
                      (*(long *)(param_1 + 0x3a8) +
                      (ulong)(ushort)((short)*(int *)(param_1 + 0x5c4) + 0x38));
            uVar23 = 5;
            goto LAB_ffffff800883a484;
          }
          if ((*(ushort *)(param_1 + 0x3f8) & 0x126) != 0) {
            if ((*(ushort *)(param_1 + 0x3f8) >> 5 & 1) == 0) {
              uVar17 = 0xffffff8008ee1fdb;
            }
            else {
              uVar17 = 0xffffff8008ee1fbe;
            }
            _dev_info(*(undefined8 *)(param_1 + 0x388),uVar17,*(undefined2 *)(param_1 + 0x42a));
            if (*(int *)(param_1 + 0x5c4) == 0) {
              DataSynchronizationBarrier(3,2,0);
              uVar23 = 1;
              puVar19 = (undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x38);
            }
            else {
              DataSynchronizationBarrier(3,2,0);
              puVar19 = (undefined2 *)
                        (*(long *)(param_1 + 0x3a8) +
                        (ulong)(ushort)((short)*(int *)(param_1 + 0x5c4) + 0x38));
              uVar23 = 5;
            }
            *puVar19 = uVar23;
            if ((*(char *)(param_1 + 0x460) == '\0') || (*(char *)(param_1 + 0x461) == '\0')) {
              i2c_dump_info(param_1);
            }
            if ((((*(ushort *)(param_1 + 0x3f8) & 1) == 0) || (*(int *)(param_1 + 0x5c4) == 0)) ||
               ((*(ushort *)(param_1 + 0x3f8) >> 8 & 1) != 0)) {
              DataSynchronizationBarrier(3,2,0);
              lVar18 = *(long *)(param_1 + 0x3a8);
              *(undefined2 *)(lVar18 + 8) = 0;
              uVar23 = *(undefined2 *)(lVar18 + 0xc);
              DataSynchronizationBarrier(3,1,0);
              *(undefined2 *)(param_1 + 0x3f8) = uVar23;
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0xc) = uVar23;
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x50) = 1;
              bVar6 = *(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02';
              if (*(char *)(param_1 + 0x3f2) == '\0') {
                uVar11 = 0x40;
                if (bVar6) {
                  uVar11 = 0x34;
                }
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) = 3;
              }
              else {
                uVar11 = 0x40;
                if (bVar6) {
                  uVar11 = 0x34;
                }
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) = 0;
              }
              if ((*(char *)(param_1 + 0x3f1) != '\0') &&
                 (*(char *)(*(long *)(param_1 + 0x470) + 6) != '\x02')) {
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x54) = 0;
              }
              if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
                DataSynchronizationBarrier(3,2,0);
                *(ushort *)(*(long *)(param_1 + 0x3a8) + 0x20) = *(ushort *)(param_1 + 0x42c) | 1;
              }
              else {
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x20) =
                     *(undefined2 *)(param_1 + 0x42c);
              }
              if (*(char *)(*(long *)(param_1 + 0x470) + 4) != '\0') {
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x2c) =
                     *(undefined2 *)(param_1 + 0x42e);
              }
              uVar11 = 0x48;
              if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
                uVar11 = 0x30;
              }
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) =
                   *(undefined2 *)(param_1 + 0x430);
              DataSynchronizationBarrier(3,2,0);
              *(undefined4 *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 0xc)
                   = 1;
              __const_udelay(0x53e3);
              DataSynchronizationBarrier(3,1,0);
              if (*(int *)(*(long *)(param_1 + 0x3b0) + (ulong)*(uint *)(param_1 + 0x5cc) + 8) != 0)
              {
                dev_err(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee16d9);
                dump_dma_regs();
                warn_slowpath_null(0xffffff8008ee16e9,0x1c1);
              }
              param_3 = -0x79;
              if (*(int *)(param_1 + 0x5c4) != 0) {
                DataSynchronizationBarrier(3,2,0);
                *(undefined2 *)(*(long *)(param_1 + 0x3a8) + 0x24) = 2;
              }
            }
            else {
              param_3 = -0x79;
              _dev_info(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1ff9,
                        *(undefined2 *)(param_1 + 0x42a));
            }
            goto LAB_ffffff800883a660;
          }
          if (!(bool)(bVar6 | *(int *)(param_1 + 0x414) == 1)) {
            if (*(int *)(param_1 + 0x5c4) == 0) {
              uVar11 = 0x30;
              if (*(char *)(*(long *)(param_1 + 0x470) + 6) == '\x02') {
                uVar11 = 0xf4;
              }
              DataSynchronizationBarrier(3,1,0);
              uVar11 = *(ushort *)(*(long *)(param_1 + 0x3a8) + (ulong)uVar11) >> 4 & 0xf;
            }
            else if (*(int *)(param_1 + 0x414) == 3) {
              uVar11 = (uint)*(ushort *)(param_1 + 0x428);
            }
            else {
              uVar11 = (uint)*(ushort *)(param_1 + 0x41a);
            }
            puVar25 = *(undefined1 **)(param_1 + 0x3c0);
            puVar1 = puVar25 + (int)uVar11;
            for (; puVar25 != puVar1; puVar25 = puVar25 + 1) {
              DataSynchronizationBarrier(3,1,0);
              *puVar25 = (char)*(undefined2 *)
                                (*(long *)(param_1 + 0x3a8) + (ulong)*(ushort *)(param_1 + 0x5c4));
            }
          }
        }
        else {
          dev_err(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1f65);
        }
      }
      if (*(int *)(param_1 + 0x414) == 3) {
        if ((psVar26[9] & 1U) != 0) {
          sVar21 = psVar26[10];
          uVar22 = *(undefined8 *)(param_1 + 0x3c0);
          uVar17 = *(undefined8 *)(psVar26 + 0xc);
LAB_ffffff800883a2ec:
          __memcpy(uVar17,uVar22,sVar21);
        }
      }
      else if ((psVar26[1] & 1U) != 0) {
        sVar21 = psVar26[2];
        uVar22 = *(undefined8 *)(param_1 + 0x3c0);
        uVar17 = *(undefined8 *)(psVar26 + 4);
        goto LAB_ffffff800883a2ec;
      }
      param_2 = psVar26 + 8;
      iVar4 = iVar27;
      if ((0 < iVar27) && (param_2 = psVar26 + 0x10, *(int *)(param_1 + 0x414) != 3)) {
        param_2 = psVar26 + 8;
      }
      goto LAB_ffffff8008839624;
    }
    param_2 = psVar26 + 8;
    cVar7 = mt_i2c_should_batch(psVar26,param_2);
    if (cVar7 == '\0') {
      if ((((ushort)psVar26[10] < 0x1000) && (*psVar26 == psVar26[8])) &&
         (((psVar26[1] & 1U) == 0 && ((psVar26[9] & 1U) != 0)))) {
        *(short *)(param_1 + 0x428) = psVar26[10];
        iVar27 = iVar4 + -2;
        *(undefined4 *)(param_1 + 0x414) = 3;
      }
      goto LAB_ffffff8008839728;
    }
    *(undefined4 *)(param_1 + 0x414) = 4;
    iVar4 = iVar27;
  } while( true );
}


//// i2c_set_speed @ffffff8008837790

void i2c_set_speed(long param_1,undefined4 param_2)

{
  uint uVar1;
  ushort uVar2;
  uint uVar3;
  undefined8 uVar4;
  long extraout_x13;
  long extraout_x13_00;
  long extraout_x13_01;
  long extraout_x13_02;
  long lVar5;
  uint extraout_w14;
  uint uVar6;
  uint extraout_w14_00;
  ulong extraout_x14;
  long extraout_x15;
  long extraout_x15_00;
  long lVar7;
  long extraout_x15_01;
  long extraout_x15_02;
  undefined8 extraout_x18;
  long extraout_x18_00;
  undefined8 extraout_x18_01;
  uint uStack_18;
  uint uStack_14;
  ushort auStack_10 [2];
  uint uStack_c;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  if ((*(char *)(param_1 + 0x460) == '\0') || (uVar6 = *(uint *)(param_1 + 0x464), uVar6 == 0)) {
    uVar6 = *(uint *)(param_1 + 0x3fc);
  }
  lVar7 = param_1 + 0x470;
  if (uVar6 < 0x61a81) goto LAB_ffffff800883788c;
  if (*(char *)(param_1 + 0x3f6) != '\0') goto LAB_ffffff800883788c;
  uVar4 = mtk_i2c_calculate_speed_isra_0(lVar7,param_2,400000,auStack_10,&uStack_c);
  lVar7 = extraout_x15;
  if ((-1 < (int)uVar4) &&
     (uVar4 = mtk_i2c_calculate_speed_isra_0
                        (extraout_x18,param_2,extraout_x14 & 0xffffffff,&uStack_18,&uStack_14),
     lVar7 = extraout_x15_00, -1 < (int)uVar4)) {
    uVar2 = (ushort)((uStack_14 & 7) << 0xc);
    *(ushort *)(extraout_x13 + 0x430) = uVar2 | (ushort)((uStack_18 & 7) << 8) | 3;
    *(ushort *)(extraout_x13 + 0x42c) = (ushort)((uStack_c & 7) << 8) | auStack_10[0] & 0x3f;
    uVar4 = 0;
    if (*(char *)(*(long *)(extraout_x13 + 0x470) + 4) != '\0') {
      *(ushort *)(extraout_x13 + 0x42e) =
           uVar2 | (ushort)((uStack_18 & 7) << 9) | auStack_10[0] | (short)uStack_c << 6;
    }
  }
LAB_ffffff8008837874:
  do {
    if (lStack_8 == *(long *)(lVar7 + 0xf08)) {
      return;
    }
    __stack_chk_fail(uVar4);
    param_1 = extraout_x13_00;
    lVar7 = extraout_x18_00;
    uVar6 = extraout_w14;
LAB_ffffff800883788c:
    uVar3 = 0x32;
    if ((uVar6 - 0x186a1 < 300000) &&
       (uVar3 = 0x32, *(char *)(*(long *)(param_1 + 0x470) + 4) != '\0')) {
      uVar3 = 0x2d;
    }
    uVar1 = 0;
    if (uVar3 != 0) {
      uVar1 = (uVar6 * 0x32) / uVar3;
    }
    uVar4 = mtk_i2c_calculate_speed_isra_0(lVar7,param_2,uVar1,&uStack_18,&uStack_14);
    lVar7 = extraout_x15_01;
  } while ((int)uVar4 < 0);
  *(ushort *)(extraout_x13_01 + 0x42c) = (ushort)((uStack_14 & 7) << 8) | (ushort)uStack_18 & 0x3f;
  lVar5 = extraout_x13_01;
  if (*(char *)(*(long *)(extraout_x13_01 + 0x470) + 4) != '\0') goto code_r0xffffff8008837900;
  goto LAB_ffffff8008837938;
code_r0xffffff8008837900:
  uVar6 = 0;
  if (100 - uVar3 != 0) {
    uVar6 = extraout_w14_00 / (100 - uVar3);
  }
  uVar4 = mtk_i2c_calculate_speed_isra_0(extraout_x18_01,param_2,uVar6,auStack_10,&uStack_c);
  lVar7 = extraout_x15_02;
  if (-1 < (int)uVar4) {
    *(ushort *)(extraout_x13_02 + 0x42e) = (ushort)((uStack_c & 7) << 6) | auStack_10[0] & 0x3f;
    lVar5 = extraout_x13_02;
LAB_ffffff8008837938:
    *(undefined2 *)(lVar5 + 0x430) = 0;
    uVar4 = 0;
  }
  goto LAB_ffffff8008837874;
}


//// mt_i2c_clock_enable @ffffff8008837994

int mt_i2c_clock_enable(long param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;

  lVar2 = *(long *)(param_1 + 0x3d8);
  iVar1 = clk_prepare(lVar2);
  if (iVar1 != 0) {
    return iVar1;
  }
  iVar1 = clk_enable(lVar2);
  if (iVar1 != 0) goto LAB_ffffff80088379d0;
  lVar2 = *(long *)(param_1 + 1000);
  if (lVar2 != 0) {
    iVar1 = clk_prepare(lVar2);
    if (iVar1 != 0) {
      return iVar1;
    }
    iVar1 = clk_enable(lVar2);
    if (iVar1 != 0) goto LAB_ffffff80088379d0;
  }
  uVar3 = *(undefined8 *)(param_1 + 0x3d0);
  iVar1 = clk_prepare(uVar3);
  if (iVar1 == 0) {
    iVar1 = clk_enable(uVar3);
    if (iVar1 == 0) {
      if (*(char *)(param_1 + 0x3f0) == '\0') {
LAB_ffffff8008837aac:
        iVar1 = -5;
        _raw_spin_lock(param_1 + 0x408);
        if (*(char *)(param_1 + 0x46e) == '\0') {
          iVar1 = 0;
          *(int *)(param_1 + 0x40c) = *(int *)(param_1 + 0x40c) + 1;
        }
        _raw_spin_unlock(param_1 + 0x408);
        if (iVar1 == 0) {
          return 0;
        }
        _dev_info(*(undefined8 *)(param_1 + 0x388),0xffffff8008ee1326);
        if (*(char *)(param_1 + 0x3f0) != '\0') {
          uVar3 = *(undefined8 *)(param_1 + 0x3e0);
          clk_disable(uVar3);
LAB_ffffff8008837a0c:
          clk_unprepare(uVar3);
        }
      }
      else {
        uVar3 = *(undefined8 *)(param_1 + 0x3e0);
        iVar1 = clk_prepare(uVar3);
        if (iVar1 == 0) {
          iVar1 = clk_enable(uVar3);
          if (iVar1 == 0) goto LAB_ffffff8008837aac;
          goto LAB_ffffff8008837a0c;
        }
      }
      uVar3 = *(undefined8 *)(param_1 + 0x3d0);
      clk_disable(uVar3);
    }
    clk_unprepare(uVar3);
  }
  lVar2 = *(long *)(param_1 + 1000);
  if (lVar2 != 0) {
    clk_disable(lVar2);
    clk_unprepare(lVar2);
  }
  lVar2 = *(long *)(param_1 + 0x3d8);
  clk_disable(lVar2);
LAB_ffffff80088379d0:
  clk_unprepare(lVar2);
  return iVar1;
}
