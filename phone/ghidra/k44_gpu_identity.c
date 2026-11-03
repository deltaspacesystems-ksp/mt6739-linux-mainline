//// RGXInit @ffffff800864d9d8

void RGXInit(long param_1)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  ulong uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  long lVar15;
  uint uVar16;
  ulong uVar17;
  ulong uVar18;
  ulong uVar19;
  ulong uVar20;
  ulong uVar21;
  ulong uVar22;
  uint uVar23;
  undefined8 uStack_100;
  uint uStack_f8;
  undefined4 uStack_f4;
  uint uStack_f0;
  undefined4 uStack_ec;
  uint uStack_e8;
  int iStack_e4;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined1 auStack_c8 [4];
  int iStack_c4;
  undefined1 auStack_c0 [4];
  undefined1 auStack_bc [4];
  undefined1 auStack_b8 [4];
  undefined1 auStack_b4 [4];
  undefined1 auStack_b0 [4];
  undefined1 auStack_ac [4];
  int iStack_a8;
  undefined1 auStack_a4 [4];
  undefined1 auStack_a0 [4];
  int iStack_9c;
  int iStack_98;
  int iStack_94;
  undefined1 auStack_90 [8];
  undefined1 auStack_88 [4];
  undefined1 auStack_84 [4];
  int iStack_80;
  undefined1 auStack_7c [4];
  undefined1 auStack_78 [4];
  undefined1 auStack_74 [4];
  undefined1 auStack_70 [4];
  undefined4 uStack_6c;
  uint uStack_68;
  int iStack_64;
  int iStack_60;
  undefined1 auStack_5c [4];
  uint uStack_58;
  uint uStack_54;
  uint uStack_50;
  undefined1 auStack_4c [4];
  undefined1 auStack_48 [4];
  uint uStack_44;
  uint uStack_40;
  uint uStack_3c;
  uint uStack_38;
  undefined1 auStack_30 [8];
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined1 auStack_10 [8];
  long lStack_8;

  uVar18 = 0xffffff8008c0bc80;
  uVar20 = 2;
  uStack_18 = 0;
  lStack_8 = lRamffffff80093cff08;
  uStack_20 = 0;
  uStack_28 = 2;
  uStack_24 = 7;
  __memset(auStack_c8,0,0x9c);
  uVar19 = *(ulong *)(param_1 + 0x148);
  uStack_e0 = uRamffffff8008c0bc90;
  uStack_d8 = uRamffffff8008c0bc98;
  uStack_d0 = uRamffffff8008c0bca0;
  OSSNPrintf(auStack_10,7,0xffffff8008eb088e,*(undefined4 *)(uVar19 + 0x1c));
  rgx_bvnc_packed(&uStack_20,&uStack_18,uStack_24,*(undefined4 *)(uVar19 + 0x18),auStack_10,
                  *(undefined4 *)(uVar19 + 0x20),*(undefined4 *)(uVar19 + 0x24));
  uVar17 = *(ulong *)(uVar19 + 8);
  uVar21 = *(ulong *)(uVar19 + 0x10);
  pvr_apphint_get_uint32(0x22,&uStack_f8);
  uVar22 = (ulong)uStack_f8;
  pvr_apphint_get_uint32(0x23,&uStack_e8);
  pvr_apphint_get_uint32(2,&uStack_f0);
  iVar6 = HTBConfigure(param_1,0xffffff8008e7c25f,uStack_f0);
  if (iVar6 == 0) goto LAB_ffffff800864de7c;
  uVar11 = PVRSRVGetErrorStringKM();
  lVar15 = -0x7ff73f4358;
  uVar13 = 0xffffff8008e7c26b;
  do {
    PVRSRVDebugPrintf(uVar20,0xffffff8008dc29e3,0,0xffffff8008e62f9d,uVar13,uVar11,lVar15);
    do {
      pvr_apphint_get_uint32(0x10,auStack_c8);
      pvr_apphint_get_bool(0x36,&iStack_c4);
      uVar18 = uVar17 >> 4 & 1;
      pvr_apphint_get_bool(7,auStack_c0);
      uVar20 = uVar21 >> 0x24 & 1;
      pvr_apphint_get_uint32(8,auStack_bc);
      pvr_apphint_get_bool(0x2d,auStack_b8);
      pvr_apphint_get_bool(0x2f,auStack_b4);
      pvr_apphint_get_bool(9,auStack_b0);
      pvr_apphint_get_bool(10,auStack_ac);
      pvr_apphint_get_bool(0x34,&iStack_a8);
      pvr_apphint_get_uint32(0x33,&uStack_f0);
      uStack_40 = uStack_f0;
      pvr_apphint_get_bool(0xb,auStack_a0);
      pvr_apphint_get_uint32(0xc,auStack_7c);
      pvr_apphint_get_uint32(0xd,auStack_74);
      pvr_apphint_get_bool(0x30,auStack_90);
      pvr_apphint_get_uint32(0xe,&uStack_f0);
      uStack_38 = uStack_f0;
      pvr_apphint_get_uint32(0x11,&uStack_f0);
      uStack_44 = uStack_f0;
      pvr_apphint_get_uint32(0x12,auStack_78);
      pvr_apphint_get_bool(0x13,auStack_84);
      pvr_apphint_get_uint32(0x15,&uStack_6c);
      pvr_apphint_get_uint32(0x14,auStack_70);
      pvr_apphint_get_uint32(0x1b,auStack_48);
      pvr_apphint_get_bool(0x37,auStack_a4);
      pvr_apphint_get_uint64(0x24,&uStack_e8);
      uStack_68 = uStack_e8;
      iStack_64 = iStack_e4;
      pvr_apphint_get_uint32(0x25,&iStack_60);
      pvr_apphint_get_uint32(0x2c,auStack_5c);
      pvr_apphint_get_uint32(0,&uStack_f0);
      uStack_58 = uStack_f0;
      if (99999 < uStack_f0) {
        uStack_58 = 99999;
      }
      if (((uint)uVar21 >> 0x15 & 1) != 0) {
        pvr_apphint_get_uint32(0x16,&uStack_f0);
        if (((uStack_f0 & 0x30) - 0x10 & 0xffffffe0) == 0) {
          uStack_f0 = uStack_f0 | 0x30;
        }
        uStack_54 = uStack_f0;
      }
      if (((uint)(uVar17 >> 5) & 1 & (uint)uVar20) != 0) {
        pvr_apphint_get_bool(0x17,auStack_88);
      }
      if ((uVar17 & 0x100) != 0 || (int)uVar18 != 0) {
        pvr_apphint_get_uint32(0x18,auStack_4c);
      }
      pvr_apphint_get_uint32(0x19,&uStack_f0);
      uStack_3c = uStack_f0 & 3;
      pvr_apphint_get_bool(0x35,&iStack_80);
      pvr_apphint_get_uint32(0x1d,auStack_30);
      if (iStack_98 == 0) {
        if ((uStack_68 != 0) || (iStack_64 != 0)) {
          iStack_98 = 1;
        }
      }
      else if ((uStack_68 == 0) && (iStack_64 == 0)) {
        uStack_68 = 0xffffffff;
        iStack_64 = -1;
      }
      if (iStack_9c != 0) {
        if (iStack_98 == 0) {
          uStack_68 = 0x3fbff80;
          iStack_64 = 0x300;
        }
        else {
          iStack_64 = -1;
          uStack_68 = 0xffffffff;
        }
      }
      if (iStack_94 == 0) {
        if (iStack_60 != 0) {
          iStack_94 = 1;
        }
      }
      else if (iStack_60 == 0) {
        iStack_60 = -1;
      }
      pvr_apphint_get_uint32(0x31,&uStack_e8);
      uVar16 = uStack_e8;
      pvr_apphint_get_uint32(0x32,&uStack_f0);
      iVar6 = iStack_80;
      uVar1 = uStack_e8 | 1;
      if (uStack_f0 == 2) {
        uVar7 = uStack_e8 | 3;
        uStack_e8 = uVar1;
        if (uVar16 == 0) {
LAB_ffffff800864df0c:
          uStack_e8 = uVar7;
        }
      }
      else {
        if (uStack_f0 == 1) {
          if (uVar16 == 0) {
            uVar1 = uStack_e8 | 3;
          }
          uStack_e8 = uVar1;
          uVar7 = uStack_e8 & 0xfffffffe;
          goto LAB_ffffff800864df0c;
        }
        uStack_e8 = uVar1;
        if (uStack_f0 == 0) {
          uStack_e8 = 0;
        }
      }
      uStack_50 = uStack_e8 | 0x643;
      bVar2 = iStack_c4 != 0;
      bVar3 = iStack_a8 != 0;
      bVar4 = iStack_94 != 0;
      bVar5 = iStack_9c != 0;
      uVar12 = InitFirmware(param_1,auStack_c8,&uStack_28,&uStack_100,&uStack_f8,&uStack_f0,
                            &uStack_e8);
      uVar1 = uStack_44;
      uVar17 = uVar12 & 0xffffffff;
      if ((int)uVar12 == 0) {
        uStack_d0 = 0xffffff800965d750;
        if (uVar19 == 0) {
          uVar20 = 0;
          iVar10 = 0;
          uVar21 = 0;
          uVar22 = 0;
          uVar19 = 0;
          uVar7 = 0;
          uVar23 = 0;
          uVar16 = 0;
          uVar18 = 0;
LAB_ffffff800864e2f8:
          uVar13 = 8;
        }
        else {
          uVar17 = *(ulong *)(uVar19 + 0x10);
          uVar18 = (ulong)*(uint *)(uVar19 + 0x34);
          iVar10 = *(int *)(uVar19 + 0x54);
          uVar16 = (uint)(uVar17 >> 0x15) & 1;
          uVar23 = (uint)(uVar17 >> 0x28) & 1;
          uVar21 = uVar17 >> 0x12 & 1;
          uVar22 = uVar17 >> 0x1e & 1;
          uVar20 = uVar17 >> 0xc & 1;
          uVar7 = (uint)(*(ulong *)(uVar19 + 8) >> 0xb) & 1;
          uVar19 = (ulong)((*(ulong *)(uVar19 + 8) & 0xf) != 0);
          if (((uint)uVar17 >> 0xe & 1) == 0) goto LAB_ffffff800864e2f8;
          uVar13 = 9;
        }
        iVar8 = ScriptDBGReadRGXReg(&uStack_e0,uVar13,0x18,0xffffff8008e7c2b1);
        if ((((((iVar8 == 0) ||
               (iVar8 = ScriptDBGReadRGXReg(&uStack_e0,8,0x20,0xffffff8008e7c2d2), iVar8 == 0)) ||
              (iVar8 = ScriptDBGReadRGXReg(&uStack_e0,8,0x28,0xffffff8008e7c2f3), iVar8 == 0)) ||
             ((iVar8 = ScriptDBGReadRGXReg(&uStack_e0,8,0x30,0xffffff8008e7c314), iVar8 == 0 ||
              (iVar8 = ScriptDBGReadRGXReg(&uStack_e0,9,0x40,0xffffff8008e7c335), iVar8 == 0)))) ||
            ((iVar8 = (int)uVar18, iVar8 != 0 &&
             (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xac8,0xffffff8008e7c377), iVar9 == 0)))) ||
           (((((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0,0xffffff8008e7c356), iVar9 == 0 ||
               (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,8,0xffffff8008e7c398), iVar9 == 0)) ||
              (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xd200,0xffffff8008e7c3b9), iVar9 == 0)) ||
             (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xd208,0xffffff8008e7c3da), iVar9 == 0)) ||
            ((((uVar16 != 0 &&
               ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x80,0xffffff8008e7c41c), iVar9 == 0 ||
                (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x88,0xffffff8008e7c43d), iVar9 == 0))))
              || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x130,0xffffff8008e7c3fb), iVar9 == 0))
             || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x160,0xffffff8008e7c45e), iVar9 == 0)))))
           ) {
LAB_ffffff800864e324:
          PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7d325);
        }
        else {
          if (uVar16 == 0) {
            iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x12b0,0xffffff8008e7c4e2);
            if (((iVar9 != 0) &&
                (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x12b8,0xffffff8008e7c503), iVar9 != 0))
               && (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x12c0,0xffffff8008e7c524), iVar9 != 0))
            {
              uVar11 = 0x12c8;
              uVar13 = 0xffffff8008e7c545;
              goto LAB_ffffff800864e0d0;
            }
            goto LAB_ffffff800864e324;
          }
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xe150,0xffffff8008e7c47f);
          if (iVar9 == 0) goto LAB_ffffff800864e324;
          uVar13 = 0xffffff8008e7c4a0;
          uVar11 = 0xe158;
LAB_ffffff800864e0d0:
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,uVar11,uVar13);
          if (((iVar9 == 0) ||
              (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x12d0,0xffffff8008e7c4c1), iVar9 == 0)) ||
             ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1290,0xffffff8008e7c566), iVar9 == 0 ||
              (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1288,0xffffff8008e7c587), iVar9 == 0))))
          goto LAB_ffffff800864e324;
          if (uVar16 == 0) {
            if (((uVar23 != 0) ||
                (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1358,0xffffff8008e7c62c), iVar9 != 0
                  && (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1320,0xffffff8008e7c64d),
                     iVar9 != 0)) &&
                 (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1328,0xffffff8008e7c66e), iVar9 != 0)))
                ) && ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1350,0xffffff8008e7c60b),
                      iVar9 != 0 &&
                      (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1338,0xffffff8008e7c68f),
                      iVar9 != 0)))) {
              uVar11 = 0x1330;
              uVar13 = 0xffffff8008e7c6b0;
              goto LAB_ffffff800864e17c;
            }
            goto LAB_ffffff800864e324;
          }
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xf098,0xffffff8008e7c5a8);
          if ((iVar9 == 0) ||
             (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xf0a0,0xffffff8008e7c5c9), iVar9 == 0))
          goto LAB_ffffff800864e324;
          uVar13 = 0xffffff8008e7c5ea;
          uVar11 = 0xf0a8;
LAB_ffffff800864e17c:
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,uVar11,uVar13);
          if (((iVar9 == 0) ||
              ((uVar7 != 0 && (iVar9 = ScriptDBGString(&uStack_e0,0xffffff8008e7c6d1), iVar9 == 0)))
              ) || (((int)uVar19 != 0 &&
                    ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4d8,0xffffff8008e7c6ee), iVar9 == 0
                     || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4e0,0xffffff8008e7c70f),
                        iVar9 == 0)))))) goto LAB_ffffff800864e324;
          if ((int)uVar22 != 0) {
            iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xf078,0xffffff8008e7c730);
            if ((((iVar9 != 0) &&
                 (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xf080,0xffffff8008e7c751), iVar9 != 0))
                && (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xf088,0xffffff8008e7c772), iVar9 != 0)
                ) && (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xf090,0xffffff8008e7c793),
                     iVar9 != 0)) {
              uVar13 = 0xffffff8008e7c7b4;
              uVar11 = 0xf210;
              goto LAB_ffffff800864e504;
            }
            goto LAB_ffffff800864e324;
          }
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1240,0xffffff8008e7c7f6);
          if ((((iVar9 == 0) ||
               (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1200,0xffffff8008e7c817), iVar9 == 0)) ||
              ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1208,0xffffff8008e7c838), iVar9 == 0 ||
               ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1210,0xffffff8008e7c859), iVar9 == 0 ||
                (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1218,0xffffff8008e7c87a), iVar9 == 0))))
              )) || ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1220,0xffffff8008e7c89b),
                     iVar9 == 0 ||
                     ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1228,0xffffff8008e7c8bc),
                      iVar9 == 0 ||
                      (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1230,0xffffff8008e7c8dd),
                      iVar9 == 0)))))) goto LAB_ffffff800864e324;
          uVar11 = 0x1238;
          uVar13 = 0xffffff8008e7c8fe;
LAB_ffffff800864e504:
          iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,uVar11,uVar13);
          if ((((((((((((iVar9 == 0) ||
                       (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x12a0,0xffffff8008e7c7d5),
                       iVar9 == 0)) ||
                      (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x12a8,0xffffff8008e7c91f),
                      iVar9 == 0)) ||
                     ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1248,0xffffff8008e7c940),
                      iVar9 == 0 ||
                      (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1250,0xffffff8008e7c961),
                      iVar9 == 0)))) ||
                    (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1260,0xffffff8008e7c982), iVar9 == 0
                    )) || ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1268,0xffffff8008e7c9a3),
                           iVar9 == 0 ||
                           (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1270,0xffffff8008e7c9c4),
                           iVar9 == 0)))) ||
                  (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1280,0xffffff8008e7c9e5), iVar9 == 0
                    || (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6008,0xffffff8008e7ca06),
                         iVar9 == 0 ||
                         (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6020,0xffffff8008e7ca27),
                         iVar9 == 0)) ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6010,0xffffff8008e7ca48),
                        iVar9 == 0)))) ||
                   (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6028,0xffffff8008e7ca69),
                     iVar9 == 0 ||
                     (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6038,0xffffff8008e7ca8a),
                     iVar9 == 0)) ||
                    (iVar9 = ScriptDBGCalc(&uStack_e0,10,0x6020,0x6028,0x6038,0xffffff8008e7caab),
                    iVar9 == 0)))))) ||
                 ((((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6018,0xffffff8008e7cacc), iVar9 == 0
                    || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x6030,0xffffff8008e7caed),
                       iVar9 == 0)) ||
                   ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x338,0xffffff8008e7cb0e), iVar9 == 0
                    || (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xf08,0xffffff8008e7cb2f),
                         iVar9 == 0 ||
                         (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x178,0xffffff8008e7cb50),
                         iVar9 == 0)) ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x1740,0xffffff8008e7cb71),
                        iVar9 == 0)))))) ||
                  ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x430,0xffffff8008e7cb92), iVar9 == 0
                   || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x438,0xffffff8008e7cbb3),
                      iVar9 == 0)))))) ||
                ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x440,0xffffff8008e7cbd4), iVar9 == 0 ||
                 (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x448,0xffffff8008e7cbf5), iVar9 == 0
                   || (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x450,0xffffff8008e7cc16),
                      iVar9 == 0)) ||
                  ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x458,0xffffff8008e7cc37), iVar9 == 0
                   || ((((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x460,0xffffff8008e7cc58),
                         iVar9 == 0 ||
                         (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xf38,0xffffff8008e7cc79),
                         iVar9 == 0)) ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x1038,0xffffff8008e7cc9a),
                        iVar9 == 0)) ||
                       ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xb98,0xffffff8008e7ccbb),
                        iVar9 == 0 ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xba0,0xffffff8008e7ccdc),
                        iVar9 == 0)))))))))))) ||
               ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xba8,0xffffff8008e7ccfd), iVar9 == 0 ||
                (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xb00,0xffffff8008e7cd1e), iVar9 == 0 ||
                  (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xbb0,0xffffff8008e7cd3f), iVar9 == 0))
                 || (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x4a0,0xffffff8008e7cd60),
                      iVar9 == 0 ||
                      (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4a8,0xffffff8008e7cd81),
                        iVar9 == 0 ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4b0,0xffffff8008e7cda2),
                        iVar9 == 0)) ||
                       (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4b8,0xffffff8008e7cdc3),
                       iVar9 == 0)))) ||
                     (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4c0,0xffffff8008e7cde4), iVar9 == 0
                     )))))))) ||
              ((((int)uVar19 != 0 &&
                ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4d8,0xffffff8008e7c6ee), iVar9 == 0 ||
                 (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x4e0,0xffffff8008e7c70f), iVar9 == 0))))
               || (((int)uVar21 != 0 &&
                   (((((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xc5d8,0xffffff8008e7ce05),
                       iVar9 == 0 ||
                       (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xc5c8,0xffffff8008e7ce26),
                       iVar9 == 0)) ||
                      ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xc5d0,0xffffff8008e7ce47),
                       iVar9 == 0 ||
                       (((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xd520,0xffffff8008e7ce68),
                         iVar9 == 0 ||
                         (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xd528,0xffffff8008e7ce89),
                         iVar9 == 0)) ||
                        (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xd530,0xffffff8008e7ceaa),
                        iVar9 == 0)))))) ||
                     ((iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0xd538,0xffffff8008e7cecb),
                      iVar9 == 0 ||
                      (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xd540,0xffffff8008e7ceec),
                      iVar9 == 0)))) ||
                    (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0xd548,0xffffff8008e7cf0d), iVar9 == 0
                    )))))))) ||
             (((uVar16 != 0 &&
               (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x8328,0xffffff8008e7cf4f), iVar9 == 0)) ||
              (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x3c8,0xffffff8008e7cf2e), iVar9 == 0))))
          goto LAB_ffffff800864e324;
          if (uVar16 == 0) {
            iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x3898,0xffffff8008e7cf70);
            if (((((iVar9 != 0) &&
                  (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,8,0x3820,0xffffff8008e7cf91), iVar9 != 0))
                 && (iVar9 = ScriptDBGReadRGXReg(&uStack_e0,9,0x3870,0xffffff8008e7cfb2), iVar9 != 0
                    )) && ((iVar10 == 0 ||
                           (iVar10 = ScriptDBGReadRGXReg(&uStack_e0,9,0x3908,0xffffff8008e7cff4),
                           iVar10 != 0)))) &&
               (iVar10 = ScriptDBGReadRGXReg(&uStack_e0,8,0x3828,0xffffff8008e7cfd3), iVar10 != 0))
            {
              uVar13 = 0xffffff8008e7d015;
              uVar14 = 0x3800;
              uVar11 = 9;
              goto LAB_ffffff800864eb0c;
            }
            goto LAB_ffffff800864e324;
          }
          iVar10 = ScriptDBGReadRGXReg(&uStack_e0,8,0xe228,0xffffff8008e7d036);
          if ((iVar10 == 0) ||
             (iVar10 = ScriptDBGReadRGXReg(&uStack_e0,9,0xe220,0xffffff8008e7d057), iVar10 == 0))
          goto LAB_ffffff800864e324;
          uVar14 = 0xe248;
          uVar13 = 0xffffff8008e7d078;
          uVar11 = 8;
LAB_ffffff800864eb0c:
          iVar10 = ScriptDBGReadRGXReg(&uStack_e0,uVar11,uVar14,uVar13);
          if ((((iVar10 == 0) ||
               ((iVar8 != 0 &&
                ((((iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x4800000,0xffffff8008e7d099),
                   iVar10 == 0 ||
                   (iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x4800010,0xffffff8008e7d0ba),
                   iVar10 == 0)) ||
                  (iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x48000a0,0xffffff8008e7d0db),
                  iVar10 == 0)) ||
                 (((iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,5,0xffffff8008e7d0fc), iVar10 == 0
                   || (iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,0x15,0xffffff8008e7d11d),
                      iVar10 == 0)) ||
                  (iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,3,0xffffff8008e7d13e), iVar10 == 0))
                 )))))) ||
              (((iVar8 - 1U < 2 &&
                (((iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x4801000,0xffffff8008e7d15f),
                  iVar10 == 0 ||
                  (iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x4801010,0xffffff8008e7d180),
                  iVar10 == 0)) ||
                 ((iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x48010a0,0xffffff8008e7d1a1),
                  iVar10 == 0 ||
                  (((iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,0x1005,0xffffff8008e7d1c2),
                    iVar10 == 0 ||
                    (iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,0x1015,0xffffff8008e7d1e3),
                    iVar10 == 0)) ||
                   (iVar10 = ScriptDBGReadMetaCoreReg(&uStack_e0,0x1003,0xffffff8008e7d204),
                   iVar10 == 0)))))))) ||
               ((uVar1 != 0 && iVar8 != 0 &&
                ((iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x480ffe0,0xffffff8008e7d225),
                 iVar10 == 0 ||
                 (iVar10 = ScriptDBGReadMetaRegThroughSP(&uStack_e0,0x480ffe8,0xffffff8008e7d246),
                 iVar10 == 0)))))))) ||
             (((int)uVar20 != 0 &&
              (iVar10 = ScriptDBGReadRGXReg(&uStack_e0,8,0x8d0,0xffffff8008e7d267), iVar10 == 0))))
          goto LAB_ffffff800864e324;
        }
        iVar10 = ScriptHalt(&uStack_e0);
        if (iVar10 == 0) {
          PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7d288);
        }
        uVar12 = PVRSRVRGXInitReleaseFWInitResourcesKM
                           (param_1,uStack_100,CONCAT44(uStack_f4,uStack_f8),
                            CONCAT44(uStack_ec,uStack_f0),CONCAT44(iStack_e4,uStack_e8));
        uVar17 = uVar12 & 0xffffffff;
        if ((int)uVar12 != 0) {
          uVar12 = uVar12 & 0xffffffff;
          uVar13 = 0xffffff8008e7d2bf;
          goto LAB_ffffff800864de48;
        }
        uVar12 = PVRSRVRGXInitDevPart2KM
                           (param_1,uStack_d0,
                            iVar6 != 0 | bVar2 << 3 | bVar4 << 4 | bVar3 << 2 | bVar5 << 1,uStack_6c
                            ,iStack_60,uStack_40);
        uVar17 = uVar12 & 0xffffffff;
        if ((int)uVar12 != 0) {
          uVar12 = uVar12 & 0xffffffff;
          uVar13 = 0xffffff8008e7d2f8;
          goto LAB_ffffff800864de48;
        }
      }
      else {
        uVar12 = uVar12 & 0xffffffff;
        uVar13 = 0xffffff8008e7c28f;
LAB_ffffff800864de48:
        PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,uVar13,uVar12);
      }
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      __stack_chk_fail(uVar17);
LAB_ffffff800864de7c:
    } while (((int)uVar22 == 0) ||
            (iVar6 = HTBControl(param_1,1,&uStack_f8,0,0,1,uStack_e8), iVar6 == 0));
    uVar11 = PVRSRVGetErrorStringKM();
    lVar15 = uVar18 + 0x28;
    uVar13 = 0xffffff8008e7c27e;
  } while( true );
}


//// RGXRegisterDevice @ffffff8008645ee4

void RGXRegisterDevice(long param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  long lVar5;
  long *plVar6;
  ulong uVar7;
  undefined8 uVar8;
  uint uVar9;
  code *pcVar10;
  uint uVar11;
  ulong uVar12;
  ulong uVar13;
  long *plVar14;
  long lStack_28;
  long lStack_20;
  long lStack_18;
  ulong uStack_10;
  long lStack_8;

  plVar14 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  *(undefined4 *)(param_1 + 0x20) = 0;
  RGXMMUInit_Register(param_1,0xffffff80093cff08,0);
  *(code **)(param_1 + 0xa8) = RGXMMUCacheInvalidate;
  *(code **)(param_1 + 0xb0) = RGXMMUCacheInvalidateKick;
  PVRSRVRegisterCmdCompleteNotify(param_1 + 0x270,RGXScheduleProcessQueuesKM,param_1);
  *(undefined4 *)(param_1 + 0x298) = 0;
  *(code **)(param_1 + 0x118) = RGXDevInitCompatCheck;
  *(code **)(param_1 + 0x1e8) = RGXRegisterMemoryContext;
  *(code **)(param_1 + 0x1f0) = RGXUnregisterMemoryContext;
  *(code **)(param_1 + 0x1f8) = RGXAllocUFOBlock;
  *(code **)(param_1 + 0x200) = RGXFreeUFOBlock;
  *(code **)(param_1 + 200) = RGXUpdateHealthStatus;
  *(code **)(param_1 + 0xd8) = RGXHWPerfDataStoreCB;
  *(code **)(param_1 + 0xe0) = RGXDevVersionString;
  *(code **)(param_1 + 0xe8) = RGXDevClockSpeed;
  *(code **)(param_1 + 0xf0) = RGXSoftReset;
  *(code **)(param_1 + 0xd0) = RGXResetHWRLogs;
  *(code **)(param_1 + 0x100) = RGXCheckFeatureSupported;
  *(code **)(param_1 + 0x108) = RGXGetSupportedFeatureValue;
  *(undefined4 *)(param_1 + 0x2a8) = 0;
  *(undefined4 *)(param_1 + 0x2ac) = 0;
  *(undefined8 *)(param_1 + 0x2b0) = 0xbadbad00bad;
  lVar5 = OSAllocMem(0x28);
  *(long *)(param_1 + 0x2a0) = lVar5;
  if (lVar5 == 0) goto LAB_ffffff8008646ac0;
  __mutex_init(lVar5,0xffffff8008e79e29,0xffffff800965c800);
  plVar6 = (long *)OSAllocZMem(0x5b8);
  if (plVar6 == (long *)0x0) {
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e79e54);
    goto LAB_ffffff800864605c;
  }
  lVar5 = OSAllocMem(0x28);
  plVar6[0x8f] = lVar5;
  if (lVar5 == 0) {
    iVar2 = 1;
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a2c9,0xffffff8008c0ba50);
    goto LAB_ffffff8008646278;
  }
  __init_rwsem(lVar5,0xffffff8008e79e89,0xffffff800965c800);
  lVar5 = OSAllocMem(0x28);
  plVar6[0x90] = lVar5;
  if (lVar5 != 0) {
    __init_rwsem(lVar5,0xffffff8008e79ead,0xffffff800965c800);
    lVar5 = OSAllocMem(0x28);
    plVar6[0x91] = lVar5;
    if (lVar5 != 0) {
      __init_rwsem(lVar5,0xffffff8008e79ed2,0xffffff800965c800);
      lVar5 = OSAllocMem(0x28);
      plVar6[0x92] = lVar5;
      if (lVar5 != 0) {
        __init_rwsem(lVar5,0xffffff8008e79ef8,0xffffff800965c800);
        lVar5 = OSAllocMem(0x28);
        plVar6[0x93] = lVar5;
        if (lVar5 != 0) {
          __init_rwsem(lVar5,0xffffff8008e79f19,0xffffff800965c800);
          lVar5 = OSAllocMem(0x28);
          plVar6[0x95] = lVar5;
          if (lVar5 != 0) {
            __init_rwsem(lVar5,0xffffff8008e79f3f,0xffffff800965c800);
            lVar5 = OSAllocMem(0x28);
            plVar6[0x94] = lVar5;
            if (lVar5 != 0) {
              __init_rwsem(lVar5,0xffffff8008e79f65,0xffffff800965c800);
              plVar6[0x96] = (long)(plVar6 + 0x96);
              plVar6[0x97] = (long)(plVar6 + 0x96);
              plVar6[0x98] = (long)(plVar6 + 0x98);
              plVar6[0x99] = (long)(plVar6 + 0x98);
              plVar6[0x9a] = (long)(plVar6 + 0x9a);
              plVar6[0x9b] = (long)(plVar6 + 0x9a);
              plVar6[0x9c] = (long)(plVar6 + 0x9c);
              plVar6[0x9d] = (long)(plVar6 + 0x9c);
              plVar6[0x9e] = (long)(plVar6 + 0x9e);
              plVar6[0x9f] = (long)(plVar6 + 0x9e);
              plVar6[0xa0] = (long)(plVar6 + 0xa0);
              plVar6[0xa1] = (long)(plVar6 + 0xa0);
              plVar6[0xa2] = (long)(plVar6 + 0xa2);
              plVar6[0xa3] = (long)(plVar6 + 0xa2);
              plVar6[0xa4] = (long)(plVar6 + 0xa4);
              plVar6[0xa5] = (long)(plVar6 + 0xa4);
              *(undefined4 *)(plVar6 + 0xa7) = 1;
              lVar5 = OSAllocMem(0x28);
              plVar6[0xa6] = lVar5;
              if (lVar5 != 0) {
                __init_rwsem(lVar5,0xffffff8008e79f89,0xffffff800965c800);
                plVar6[0x8d] = (long)(plVar6 + 0x8d);
                plVar6[0x8e] = (long)(plVar6 + 0x8d);
                lVar5 = OSAllocMem(0x4600);
                plVar6[0x2a] = lVar5;
                if (lVar5 != 0) {
                  *plVar6 = param_1;
                  plVar6[0x1a] = *(long *)(param_1 + 0x130);
                  lVar5 = OSMapPhysToLin(*(undefined8 *)(*(long *)(param_1 + 0x110) + 0x20),
                                         *(undefined4 *)(*(long *)(param_1 + 0x110) + 0x28),0);
                  plVar6[0x12] = lVar5;
                  iVar2 = iRamffffff800965c800;
                  if (lVar5 != 0) {
                    *(long **)(param_1 + 0x148) = plVar6;
                    uStack_10 = 0;
                    lStack_28 = 0;
                    lStack_20 = 0;
                    lStack_18 = 0;
                    iVar2 = RGXParseBVNCList_constprop_11
                                      (&lStack_28,&lStack_20,&lStack_18,&uStack_10,iVar2);
                    if (iVar2 != 0) {
                      lStack_28 = 0x16;
                      lStack_18 = 0x15;
                      uStack_10 = 0x10;
                      iVar2 = OSVSScanf(0xffffff8008db75f7,0xffffff8008dd0532,&lStack_20);
                      if ((iVar2 != 1) &&
                         (iVar2 = OSVSScanf(0xffffff8008db75f7,0xffffff8008e7a000,&lStack_20),
                         iVar2 != 1)) {
                        lStack_20 = 0;
                      }
                      PVRSRVReleasePrintf(0xffffff8008e7a012,0xffffff8008c0ba68,0xffffff8008e7a006);
                    }
                    uVar11 = 0;
                    uVar12 = lStack_18 << 0x10 | lStack_28 << 0x30 | uStack_10;
                    uVar1 = 0x23;
                    while (uVar9 = uVar1, uVar11 < uVar9) {
                      uVar1 = uVar9 + uVar11 >> 1;
                      lVar5 = (ulong)(uVar1 * 3) * 8;
                      if (uVar12 == *(ulong *)(lVar5 + -0x7ff6b89bc0)) {
                        if ((ulong)(uVar1 * 3) != 0xffed71378) {
                          plVar6[2] = *(long *)(lVar5 + -0x7ff6b89bb8);
                          uVar13 = *(ulong *)(lVar5 + -0x7ff6b89bb0);
                          lVar5 = *(long *)(param_1 + 0x148);
                          uVar12 = uVar13 >> 0x26 & 7;
                          if (uVar12 < 6) {
                            iVar2 = *(int *)(uVar12 * 4 + -0x7ff6b89878);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a074);
                            iVar2 = 0;
                          }
                          *(int *)(lVar5 + 0x5c) = iVar2 << 10;
                          uVar3 = uRamffffff80094767a0;
                          if ((uVar13 & 1) != 0) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a0ba);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x2c) = uVar3;
                          uVar12 = uVar13 >> 3 & 3;
                          if (uVar12 == 3) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a0e0);
                            uVar3 = 0;
                          }
                          else {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89858);
                          }
                          *(undefined4 *)(lVar5 + 0x30) = uVar3;
                          uVar12 = uVar13 >> 8 & 3;
                          if (uVar12 < 2) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89848);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a0ff);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x38) = uVar3;
                          uVar12 = uVar13 >> 10 & 7;
                          if (uVar12 < 5) {
                            iVar2 = *(int *)(uVar12 * 4 + -0x7ff6b89840);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a11e);
                            iVar2 = 0;
                          }
                          *(int *)(lVar5 + 0x3c) = iVar2 << 10;
                          uVar3 = uRamffffff80094767d8;
                          if (((uint)uVar13 >> 0xd & 1) != 0) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a13c);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x40) = uVar3;
                          if (((uint)*(undefined8 *)(lVar5 + 0x10) >> 0xc & 1) == 0) {
                            uVar12 = uVar13 >> 5 & 7;
                            if (uVar12 < 4) {
                              uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89820);
                            }
                            else {
                              PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                                0xffffff8008e7a15f);
                              uVar3 = 0;
                            }
                            *(undefined4 *)(lVar5 + 0x34) = uVar3;
                          }
                          else {
                            *(undefined4 *)(lVar5 + 0x34) = 0;
                          }
                          uVar12 = uVar13 >> 0xe & 0xf;
                          if (uVar12 < 8) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89810);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a170);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x28) = uVar3;
                          uVar12 = uVar13 >> 0x12 & 0xf;
                          if (uVar12 < 0xc) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b897f0);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a189);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x44) = uVar3;
                          uVar12 = uVar13 >> 0x18 & 7;
                          if (uVar12 < 4) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b897c0);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a1a7);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x48) = uVar3;
                          uVar12 = uVar13 >> 0x22 & 3;
                          if (uVar12 == 3) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a1c2);
                            uVar3 = 0;
                          }
                          else {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b897b0);
                          }
                          *(undefined4 *)(lVar5 + 0x54) = uVar3;
                          uVar12 = uVar13 >> 0x24 & 3;
                          if (uVar12 < 2) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b897a0);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a1d8);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x58) = uVar3;
                          uVar12 = uVar13 >> 0x1b & 3;
                          if (uVar12 == 3) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a1fd);
                            uVar3 = 0;
                          }
                          else {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89798);
                          }
                          *(undefined4 *)(lVar5 + 0x4c) = uVar3;
                          uVar12 = uVar13 >> 0x1d & 7;
                          if (uVar12 < 4) {
                            uVar3 = *(undefined4 *)(uVar12 * 4 + -0x7ff6b89788);
                          }
                          else {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a21a);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x50) = uVar3;
                          uVar3 = uRamffffff8009476888;
                          if ((uVar13 >> 0x2b & 1) != 0) {
                            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a092,
                                              0xffffff8008e7a233);
                            uVar3 = 0;
                          }
                          *(undefined4 *)(lVar5 + 0x60) = uVar3;
                          lVar5 = *(long *)(param_1 + 0x148);
                          *(undefined4 *)(lVar5 + 100) = 5;
                          *(undefined4 *)(lVar5 + 0x68) = 6;
                          if (((uint)*(undefined8 *)(lVar5 + 0x10) >> 0x12 & 1) != 0) {
                            *(undefined4 *)(lVar5 + 100) = 7;
                            *(undefined4 *)(lVar5 + 0x68) = 8;
                          }
                          uVar11 = *(uint *)(lVar5 + 0x28);
                          if (uVar11 != 0) {
                            uVar1 = uVar11 >> 1;
                            if (uVar11 < 2) {
                              uVar1 = 1;
                            }
                            *(uint *)(lVar5 + 0x6c) = uVar1;
                          }
                          uVar11 = 0;
                          uVar12 = lStack_20 << 0x20 | lStack_28 << 0x30 |
                                   uStack_10 | lStack_18 << 0x10;
                          uVar1 = 0x30;
                          goto LAB_ffffff8008646668;
                        }
                        break;
                      }
                      if (*(ulong *)(lVar5 + -0x7ff6b89bc0) <= uVar12) {
                        uVar11 = uVar1 + 1;
                        uVar1 = uVar9;
                      }
                    }
                    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a038,0xffffff8008c0ba68);
                    goto LAB_ffffff80086463f8;
                  }
                  iVar2 = 8;
                  PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e79fd6,0xffffff8008c0ba50);
                  goto LAB_ffffff80086462e8;
                }
                iVar2 = 1;
                PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e79fac,0xffffff8008c0ba50);
                goto LAB_ffffff8008646218;
              }
              iVar2 = 1;
              PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a410,0xffffff8008c0ba50);
              goto LAB_ffffff8008646224;
            }
            iVar2 = 1;
            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a3e2,0xffffff8008c0ba50);
            goto LAB_ffffff8008646230;
          }
          iVar2 = 1;
          PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a3b1,0xffffff8008c0ba50);
          goto LAB_ffffff800864623c;
        }
        iVar2 = 1;
        PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a381,0xffffff8008c0ba50);
        goto LAB_ffffff8008646248;
      }
      iVar2 = 1;
      PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a356,0xffffff8008c0ba50);
      goto LAB_ffffff8008646254;
    }
    iVar2 = 1;
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a326,0xffffff8008c0ba50);
    goto LAB_ffffff8008646260;
  }
  iVar2 = 1;
  PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a2f7,0xffffff8008c0ba50);
  goto LAB_ffffff800864626c;
LAB_ffffff8008646668:
  uVar9 = uVar1;
  if (uVar11 < uVar9) {
    uVar1 = uVar9 + uVar11 >> 1;
    uVar13 = (ulong)(uVar9 + uVar11) & 0xfffffffe;
    lVar5 = uVar13 * 8;
    uVar7 = *(ulong *)(lVar5 + -0x7ff6b89770);
    if (uVar12 == uVar7) {
      if (uVar13 != 0xffed712ee) {
        plVar6[1] = *(long *)(lVar5 + -0x7ff6b89768);
        *(int *)(plVar6 + 3) = (int)lStack_28;
        *(int *)((long)plVar6 + 0x1c) = (int)lStack_20;
        *(int *)(plVar6 + 4) = (int)lStack_18;
        *(int *)((long)plVar6 + 0x24) = (int)uStack_10;
        iRamffffff800965c800 = iRamffffff800965c800 + 1;
        iVar2 = RGXInitHeaps(plVar6,param_1 + 0x120,param_1 + 0x2ac);
        if (iVar2 != 0) goto LAB_ffffff80086469f8;
        iVar2 = RGXHWPerfInit(plVar6);
        if (iVar2 != 0) {
          uVar8 = PVRSRVGetErrorStringKM();
          PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e62f9d,0xffffff8008e7a296,uVar8,
                            0xffffff8008c0ba50);
          goto LAB_ffffff80086469f8;
        }
        iVar4 = PVRSRVRegisterDbgRequestNotify(plVar6 + 0x11,param_1,RGXDebugRequestNotify,2,plVar6)
        ;
        if (iVar4 != 0) {
          uVar8 = PVRSRVGetErrorStringKM();
          PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e62f9d,0xffffff8008e64d71,uVar8,
                            0xffffff8008c0ba50);
        }
        if (((uint)plVar6[2] >> 0xc & 1) != 0) {
          RGXMipsMMUInit_Register(param_1);
        }
        uVar8 = 0;
        if ((plVar6[1] & 0x802200000U) == 0) {
          uVar8 = 0x8000000000;
        }
        *(undefined8 *)(param_1 + 0x138) = uVar8;
        pcVar10 = *(code **)(*(long *)(param_1 + 0x110) + 0xa0);
        if (pcVar10 != (code *)0x0) {
          (*pcVar10)(*(long *)(param_1 + 0x110),plVar6[2]);
        }
        goto LAB_ffffff8008646aa4;
      }
      goto LAB_ffffff8008646670;
    }
    if (uVar7 <= uVar12) {
      uVar11 = uVar1 + 1;
      uVar1 = uVar9;
    }
    goto LAB_ffffff8008646668;
  }
LAB_ffffff8008646670:
  PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a25a,0xffffff8008c0ba68);
  plVar6[1] = 0;
LAB_ffffff80086463f8:
  iVar2 = 0x142;
  PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a43e,0xffffff8008c0ba50);
LAB_ffffff80086469f8:
  OSUnMapPhysToLin(plVar6[0x12],(int)plVar6[0x15],0);
LAB_ffffff80086462e8:
  OSFreeMem(plVar6[0x2a]);
  plVar6[0x2a] = 0;
LAB_ffffff8008646218:
  OSFreeMem(plVar6[0xa6]);
  plVar6[0xa6] = 0;
LAB_ffffff8008646224:
  OSFreeMem(plVar6[0x94]);
  plVar6[0x94] = 0;
LAB_ffffff8008646230:
  OSFreeMem(plVar6[0x95]);
  plVar6[0x95] = 0;
LAB_ffffff800864623c:
  OSFreeMem(plVar6[0x93]);
  plVar6[0x93] = 0;
LAB_ffffff8008646248:
  OSFreeMem(plVar6[0x92]);
  plVar6[0x92] = 0;
LAB_ffffff8008646254:
  OSFreeMem(plVar6[0x91]);
  plVar6[0x91] = 0;
LAB_ffffff8008646260:
  OSFreeMem(plVar6[0x90]);
  plVar6[0x90] = 0;
LAB_ffffff800864626c:
  OSFreeMem(plVar6[0x8f]);
  plVar6[0x8f] = 0;
LAB_ffffff8008646278:
  OSFreeMem(plVar6);
  OSFreeMem(*(undefined8 *)(param_1 + 0x2a0));
  *(undefined8 *)(param_1 + 0x2a0) = 0;
LAB_ffffff8008646aa4:
  while (plVar14 = plVar14 + 0x1e1, lStack_8 != *plVar14) {
    __stack_chk_fail(iVar2);
LAB_ffffff8008646ac0:
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e7a2a4,0xffffff8008c0ba50);
LAB_ffffff800864605c:
    iVar2 = 1;
  }
  return;
}


//// RGXDevInitCompatCheck @ffffff80086442a4

void RGXDevInitCompatCheck(long param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  long lVar4;
  bool bVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  ulong uVar11;
  undefined8 uVar12;
  long lVar13;
  long *plVar14;
  uint uVar15;
  uint uStack_4c;
  long lStack_48;
  undefined8 uStack_40;
  ulong uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  ulong uStack_20;
  undefined8 uStack_18;
  undefined1 auStack_10 [8];
  long lStack_8;

  plVar14 = (long *)0xffffff80093cf000;
  lStack_48 = 0;
  lStack_8 = lRamffffff80093cff08;
  lVar13 = *(long *)(param_1 + 0x148);
  piVar10 = (int *)PVRSRVGetPVRSRVData(param_1,0xffffff80093cff08,0);
  if (*piVar10 < 1) goto LAB_ffffff800864430c;
LAB_ffffff80086442ec:
  iVar6 = 0;
LAB_ffffff80086442f0:
  while( true ) {
    plVar14 = plVar14 + 0x1e1;
    if (lStack_8 == *plVar14) {
      return;
    }
    __stack_chk_fail(iVar6);
LAB_ffffff800864430c:
    piVar10 = (int *)PVRSRVGetPVRSRVData();
    if (*piVar10 == 1) break;
    iVar6 = DevmemAcquireCpuVirtAddr(*(undefined8 *)(lVar13 + 0x250),&lStack_48);
    if (iVar6 == 0) {
      uVar7 = OSClockus();
      uVar9 = uVar7 + 1;
      bVar5 = true;
      uVar15 = 0;
LAB_ffffff800864437c:
      if (999999 < (uVar15 - uVar7) + uVar9) {
        if (!bVar5) goto LAB_ffffff8008644390;
        bVar5 = false;
      }
      if (*(int *)(lStack_48 + 0x198) != 0) goto LAB_ffffff8008644390;
      OSWaitus(0x32);
      uVar9 = OSClockus();
      if (uVar9 < uVar7) {
        uVar15 = ~uVar7;
        uVar7 = 0;
      }
      goto LAB_ffffff800864437c;
    }
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e78f11,0xffffff8008c0b9b8,iVar6);
  }
  goto LAB_ffffff80086442ec;
LAB_ffffff8008644390:
  uStack_4c = 0;
  if (*(int *)(lVar13 + 0x34) != 0) {
    iVar8 = RGXReadMETAAddr(lVar13,0x4800000,&uStack_4c);
    if (iVar8 != 0) {
      PVRSRVReleasePrintf(0xffffff8008e79117,0xffffff8008c0b9b8,iVar8);
      iVar6 = iVar8;
      goto LAB_ffffff80086443d4;
    }
    if ((uStack_4c & 1) == 0) {
      iVar6 = 0x119;
      PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e79163,0xffffff8008c0b9b8,
                        *(undefined4 *)(lStack_48 + 0x198),0x119);
      goto LAB_ffffff80086443d4;
    }
  }
  if (*(int *)(lStack_48 + 0x198) == 0) {
    iVar6 = 9;
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e791a9,0xffffff8008c0b9b8,9);
    goto LAB_ffffff80086443d4;
  }
  if (lStack_48 != 0) {
    uVar9 = *(uint *)(lStack_48 + 0x194) & 0xffff;
    if (uVar9 != 0x10) {
      uVar9 = uVar9 ^ 0x10;
      uVar15 = uVar9 & 0x10;
      if ((uVar9 >> 4 & 1) == 0) {
        uVar15 = *(uint *)(lStack_48 + 0x194) & 0x10 & uVar9;
        if (uVar15 == 0) goto LAB_ffffff80086445c8;
        uVar12 = 0xffffff8008e79270;
      }
      else {
        uVar12 = 0xffffff8008e791d5;
      }
      PVRSRVReleasePrintf(uVar12,uVar15);
      iVar6 = 0x1c;
      goto LAB_ffffff80086443d4;
    }
LAB_ffffff80086445c8:
    uVar9 = *(uint *)(lStack_48 + 0x18c);
    if (uVar9 != 0x10009) {
      iVar6 = 0x1a;
      PVRSRVReleasePrintf(0xffffff8008e7930b,1,9,uVar9 >> 0x10,uVar9 & 0xffff);
      goto LAB_ffffff80086443d4;
    }
    if (*(int *)(lStack_48 + 400) != 0x4aab9b) {
      PVRSRVReleasePrintf(0xffffff8008e79372);
    }
    lVar4 = lStack_48;
    uStack_20 = 0;
    uStack_18 = 0;
    uStack_28 = 0x700000002;
    uVar1 = *(undefined4 *)(lVar13 + 0x18);
    uVar2 = *(undefined4 *)(lVar13 + 0x20);
    uVar3 = *(undefined4 *)(lVar13 + 0x24);
    OSSNPrintf(auStack_10,8,0xffffff8008eb088e,*(undefined4 *)(lVar13 + 0x1c));
    rgx_bvnc_packed(&uStack_20,&uStack_18,uStack_28._4_4_,uVar1,auStack_10,uVar2,uVar3);
    if (lVar4 != 0) {
      if ((((int)uStack_28 == *(int *)(lVar4 + 0x170)) &&
          (uStack_28._4_4_ == *(uint *)(lVar4 + 0x174))) && (uStack_20 == *(ulong *)(lVar4 + 0x178))
         ) {
        auStack_10[(ulong)uStack_28._4_4_ - 8] = 0;
        *(undefined1 *)(lVar4 + (ulong)uStack_28._4_4_ + 0x180) = 0;
        OSStringCompare(&uStack_18,lVar4 + 0x180);
      }
      lVar4 = lStack_48;
      uStack_20 = 0;
      uStack_30 = 0;
      uStack_28 = 0x700000002;
      uStack_18 = 0;
      uStack_38 = 0;
      uVar1 = *(undefined4 *)(lVar13 + 0x18);
      uStack_40 = 0x700000002;
      uVar2 = *(undefined4 *)(lVar13 + 0x20);
      uVar9 = *(uint *)(lVar13 + 0x24);
      OSSNPrintf(auStack_10,8,0xffffff8008eb088e,*(undefined4 *)(lVar13 + 0x1c));
      rgx_bvnc_packed(&uStack_38,&uStack_30,uStack_40._4_4_,uVar1,auStack_10,uVar2,uVar9);
      bVar5 = (*(ulong *)(lVar13 + 8) & 8) == 0;
      if ((!bVar5 && 8 < uVar9) && (bVar5 || uVar9 != 9)) {
        PVRSRVReleasePrintf(0xffffff8008e7944e,0xffffff8008dc29e3,0xffffff8008dc29e3,
                            0xffffff8008dc29e3,0xffffff8008ef463f);
        uVar11 = 0xffffffff0000;
      }
      else {
        uVar11 = 0xffffffffffff;
      }
      if (lVar4 != 0) {
        uStack_28 = *(undefined8 *)(lVar4 + 0x158);
        uStack_18 = *(undefined8 *)(lVar4 + 0x168);
        uStack_20 = uVar11 & *(ulong *)(lVar4 + 0x160);
        uStack_38 = uVar11 & uStack_38;
        bVar5 = (int)uStack_40 == (int)uStack_28;
        if (bVar5) {
          uStack_28._4_4_ = (uint)((ulong)uStack_28 >> 0x20);
          bVar5 = uStack_40._4_4_ == uStack_28._4_4_;
          if ((bVar5) && (uStack_20 == uStack_38)) {
            *(undefined1 *)((long)&uStack_30 + (ulong)uStack_40._4_4_) = 0;
            auStack_10[(ulong)uStack_40._4_4_ - 8] = 0;
            OSStringCompare(&uStack_30);
          }
        }
        lVar4 = lStack_48;
        if (((uint)*(undefined8 *)(lVar13 + 0x10) >> 0xc & 1) == 0) {
          if (*(int *)(lVar13 + 0x34) == 0) {
            PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e793de,0xffffff8008c0b9d0);
            iVar8 = 0;
            uVar12 = 0;
          }
          else {
            uVar12 = 0xffffff8008e7a16b;
            switch(*(int *)(lVar13 + 0x34)) {
            case 1:
              iVar8 = 0x19;
              break;
            case 2:
              iVar8 = 0x1e;
              break;
            case 3:
              iVar8 = 0x1c;
              break;
            case 4:
              iVar8 = 0x1f;
              uVar12 = 0xffffff8008e7a16b;
              break;
            default:
              PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e793de,0xffffff8008c0b9d0);
              iVar8 = 0;
              uVar12 = 0xffffff8008e7a16b;
            }
          }
        }
        else {
          iVar8 = 1;
          uVar12 = 0xffffff8008e7ad7a;
        }
        if (lVar4 != 0) {
          if (*(int *)(lVar4 + 0x188) != iVar8) {
            iVar6 = 0x1e;
            PVRSRVReleasePrintf(0xffffff8008e793fd,uVar12,iVar8);
          }
          goto LAB_ffffff80086443d4;
        }
      }
    }
  }
  iVar6 = 3;
LAB_ffffff80086443d4:
  DevmemReleaseCpuVirtAddr(*(undefined8 *)(lVar13 + 0x250));
  goto LAB_ffffff80086442f0;
}


//// mt_gpufreq_pdrv_probe @ffffff80084483e4

void mt_gpufreq_pdrv_probe(long param_1)

{
  int *piVar1;
  char cVar2;
  bool bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  long lVar9;
  undefined8 uVar10;
  long lVar11;
  ulong uVar12;
  long lVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  undefined1 auVar17 [16];
  undefined4 auStack_10 [2];
  long lStack_8;

  lVar14 = -0x7ff743f000;
  plVar15 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  auStack_10[0] = 99;
  lVar9 = of_find_matching_node_and_match(0,0xffffff8008bc1ba0,0);
  if (lVar9 == 0) {
    printk(0xffffff8008dfa377,0xffffff8008bc1d30);
  }
  lVar16 = -0x7ff6a69000;
  auVar17 = kmem_cache_alloc_trace(uRamffffff8009597938,0x24080c0,0x18);
  uVar12 = 0xfffffff4;
  lVar9 = -0x7ff743e710;
  if (auVar17._0_8_ == 0) goto LAB_ffffff8008448534;
  uVar5 = get_devinfo_with_index(0x1d,auVar17._8_8_,0xfffffff4);
  lVar9 = -0x7ff6a3e000;
  if ((uVar5 & 0x300000) != 0x300000) goto LAB_ffffff80084485dc;
  iVar6 = get_devinfo_with_index(0x1e);
  if (iVar6 != 0x80) goto LAB_ffffff8008448550;
LAB_ffffff8008448494:
  uVar7 = get_devinfo_with_index(0x1e);
  uVar8 = get_devinfo_with_index(0x1d);
  printk(0xffffff8008dfa3ab,lVar14 + 0xd30,uVar7,uVar8);
  *(undefined4 *)(lVar9 + 0xe98) = 1;
  lVar13 = lVar9;
  do {
    lVar14 = lVar13 + 0xd88;
    param_1 = param_1 + 0x10;
    lVar16 = kmem_cache_alloc_trace(*(undefined8 *)(lVar16 + 0x938),0x24080c0,0x18);
    *(long *)(lVar13 + 0xde8) = lVar16;
    uVar10 = regulator_get(param_1,0xffffff8008e260c6);
    *(undefined8 *)(lVar16 + 0x10) = uVar10;
    lVar9 = lVar13;
    if (*(ulong *)(*(long *)(lVar13 + 0xde8) + 0x10) < 0xfffffffffffff001) {
      lVar16 = -0x7ff6bc6000;
      *(undefined4 *)(lVar13 + 0xe58) = *(undefined4 *)(lVar13 + 0xe98);
      mt_spower_init();
      aee_rr_rec_gpu_dvfs_vgpu(0xffffffff);
      aee_rr_rec_gpu_dvfs_oppidx(0xffffffff);
      aee_rr_rec_gpu_dvfs_status(0xfffffffc);
      printk(0xffffff8008dfa43b);
      if (*(int *)(lVar13 + 0xe58) == 1) {
        uVar10 = 0xffffff800943ad50;
      }
      else {
        uVar10 = 0xffffff800943ad10;
      }
      mt_setup_gpufreqs_table_constprop_21(uVar10);
      lVar14 = lVar13 + 0xd88;
      *(undefined4 *)(lVar13 + 0xde4) = 1;
      uVar10 = of_find_compatible_node(0,0,0xffffff8008dfa463);
      lVar11 = of_iomap(uVar10,0);
      *(long *)(lVar13 + 0xdd8) = lVar11;
      if (lVar11 == 0) {
        printk(0xffffff8008dfa474);
        uVar12 = 0xfffffffe;
      }
      else {
        param_1 = -0x7ff6bc5480;
        mutex_lock(0xffffff800943ab90);
        bVar4 = aee_rr_curr_gpu_dvfs_status();
        aee_rr_rec_gpu_dvfs_status(bVar4 | 1);
        uVar7 = _mt_gpufreq_get_cur_volt();
        uVar10 = _mt_gpufreq_get_cur_freq();
        uRamffffff800943ab80 = 0;
        mt_gpufreq_set(uVar10,**(undefined4 **)(lVar13 + 0xdb8),uVar7,
                       (*(undefined4 **)(lVar13 + 0xdb8))[1]);
        lVar11 = *(long *)(lVar13 + 0xdb8) + (ulong)uRamffffff800943ab80 * 0x10;
        uRamffffff800943ab84 =
             *(undefined4 *)(*(long *)(lVar13 + 0xdb8) + (ulong)uRamffffff800943ab80 * 0x10);
        uRamffffff800943ab8c = *(undefined4 *)(lVar11 + 4);
        uRamffffff800943ab88 = *(undefined4 *)(lVar11 + 0xc);
        aee_rr_rec_gpu_dvfs_oppidx();
        uVar5 = aee_rr_curr_gpu_dvfs_status();
        aee_rr_rec_gpu_dvfs_status(uVar5 & 0xfe);
        mutex_unlock(0xffffff800943ab90);
        uVar7 = _mt_gpufreq_get_cur_freq();
        printk(0xffffff8008dfa4a9,uVar7);
        uVar5 = _mt_gpufreq_get_cur_volt();
        printk(0xffffff8008dfa4da,uVar5 / 100);
        printk(0xffffff8008dfa502,uRamffffff800943ab84,uRamffffff800943ab8c);
        printk(0xffffff8008dfa53e,uRamffffff800943ab88,uRamffffff800943ab80);
        *(undefined1 *)(lVar13 + 0xde1) = 1;
        uVar12 = kthread_create_on_node(mt_gpufreq_input_boost_task,0,0xffffffff,0xffffff8008dfa57b)
        ;
        *(ulong *)(lVar13 + 0xe60) = uVar12;
        if (uVar12 < 0xfffffffffffff001) {
          sched_setscheduler_nocheck(uVar12,1,auStack_10);
          piVar1 = (int *)(*(long *)(lVar13 + 0xe60) + 0x38);
          Hint_Prefetch(piVar1,2,0,1);
          do {
            cVar2 = '\x01';
            bVar3 = (bool)ExclusiveMonitorPass(piVar1,0x10);
            if (bVar3) {
              *piVar1 = *piVar1 + 1;
              cVar2 = ExclusiveMonitorsStatus();
            }
          } while (cVar2 != '\0');
          input_register_handler(0xffffff800943ac68);
          uVar12 = 0;
          while (iVar6 = (int)uVar12, iVar6 != *(int *)(lVar13 + 0xd88)) {
            if (*(int *)(*(long *)(lVar13 + 0xdb8) +
                        (-(uVar12 >> 0x1f) & 0xfffffff000000000 | uVar12 << 4)) == 290000) {
              *(int *)(lVar13 + 0xed0) = iVar6;
              break;
            }
            uVar12 = (ulong)(iVar6 + 1);
          }
          register_low_battery_notify(mt_gpufreq_low_batt_volt_callback,2);
          uVar12 = 0;
          while (iVar6 = (int)uVar12, iVar6 != *(int *)(lVar13 + 0xd88)) {
            if (*(int *)(*(long *)(lVar13 + 0xdb8) +
                        (-(uVar12 >> 0x1f) & 0xfffffff000000000 | uVar12 << 4)) == 450000) {
              *(int *)(lVar13 + 0xec8) = iVar6;
              break;
            }
            uVar12 = (ulong)(iVar6 + 1);
          }
          register_battery_percent_notify(mt_gpufreq_low_batt_volume_callback,2);
          uVar12 = 0;
          while (iVar6 = (int)uVar12, iVar6 != *(int *)(lVar13 + 0xd88)) {
            if (*(int *)(*(long *)(lVar13 + 0xdb8) +
                        (-(uVar12 >> 0x1f) & 0xfffffff000000000 | uVar12 << 4)) == 290000) {
              *(int *)(lVar13 + 0xec0) = iVar6;
              break;
            }
            uVar12 = (ulong)(iVar6 + 1);
          }
          register_battery_oc_notify(mt_gpufreq_oc_callback,2);
          lVar9 = lVar13 + 0xdf8;
          *(undefined8 *)(lVar13 + 0xdf8) = 0xfffffffe0;
          *(long *)(lVar13 + 0xe00) = lVar13 + 0xe00;
          *(long *)(lVar13 + 0xe08) = lVar13 + 0xe00;
          *(code **)(lVar13 + 0xe10) = mt_gpufreq_notify_pbm_gpuoff;
          init_timer_key(lVar13 + 0xe18,0x300000,0,0);
          uVar12 = 0;
          *(code **)(lVar13 + 0xe30) = delayed_work_timer_fn;
          *(long *)(lVar13 + 0xe38) = lVar9;
        }
      }
    }
    else {
      dev_err(param_1,0xffffff8008dfa429);
      uVar12 = (ulong)*(uint *)(*(long *)(lVar13 + 0xde8) + 0x10);
    }
LAB_ffffff8008448534:
    plVar15 = plVar15 + 0x1e1;
    if (lStack_8 == *plVar15) {
      return;
    }
    __stack_chk_fail(uVar12 & 0xffffffff);
LAB_ffffff8008448550:
    iVar6 = get_devinfo_with_index(0x1e);
    if ((((((iVar6 == 0x88) || (iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0x90)) ||
          (iVar6 = get_devinfo_with_index(0x1e), iVar6 == 8)) ||
         ((iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0 ||
          (iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0x40)))) ||
        ((iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0x48 ||
         ((iVar6 = get_devinfo_with_index(0x1e), iVar6 == 200 ||
          (iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0xc0)))))) ||
       (iVar6 = get_devinfo_with_index(0x1e), iVar6 == 0xd0)) goto LAB_ffffff8008448494;
LAB_ffffff80084485dc:
    uVar7 = get_devinfo_with_index(0x1e);
    uVar8 = get_devinfo_with_index(0x1d);
    printk(0xffffff8008dfa3eb,lVar14 + 0xd30,uVar7,uVar8);
    *(undefined4 *)(lVar9 + 0xe98) = 0;
    lVar13 = lVar9;
  } while( true );
}


//// MTKInitHWAPM @ffffff8008672358

undefined8 MTKInitHWAPM(void)

{
  if (lRamffffff8009662228 == 0) {
    lRamffffff8009662228 = OSMapPhysToLin(uRamffffff8009662230,0x1000,0);
  }
  if (iRamffffff8009662238 != 0) {
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e8468d,lRamffffff8009662228);
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844da,
                      *(undefined4 *)(lRamffffff8009662228 + 0x1c));
  }
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x504) = 0x1a80000;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x508) = 0x80010;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x50c) = 0x100008;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x510) = 0xb800c8;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x514) = 0xb000c0;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x518) = 0xc000c8;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x51c) = 0xb000b8;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x520) = 0xd000d0;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x524) = 0xd000d0;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x528) = 0xd00000;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x24) = 0x9004001b;
  DataSynchronizationBarrier(3,2,0);
  DataSynchronizationBarrier(3,2,0);
  *(undefined4 *)(lRamffffff8009662228 + 0x24) = 0x8004001b;
  DataSynchronizationBarrier(3,2,0);
  if (iRamffffff8009662238 != 0) {
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x504),
                      *(undefined4 *)(lRamffffff8009662228 + 0x504));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x508),
                      *(undefined4 *)(lRamffffff8009662228 + 0x508));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x50c),
                      *(undefined4 *)(lRamffffff8009662228 + 0x50c));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x510),
                      *(undefined4 *)(lRamffffff8009662228 + 0x510));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x514),
                      *(undefined4 *)(lRamffffff8009662228 + 0x514));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x518),
                      *(undefined4 *)(lRamffffff8009662228 + 0x518));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x51c),
                      *(undefined4 *)(lRamffffff8009662228 + 0x51c));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x520),
                      *(undefined4 *)(lRamffffff8009662228 + 0x520));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x524),
                      *(undefined4 *)(lRamffffff8009662228 + 0x524));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e844f1,
                      (undefined4 *)(lRamffffff8009662228 + 0x528),
                      *(undefined4 *)(lRamffffff8009662228 + 0x528));
    DataSynchronizationBarrier(3,1,0);
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e84510,
                      *(undefined4 *)(lRamffffff8009662228 + 0x1c));
  }
  return 0;
}
