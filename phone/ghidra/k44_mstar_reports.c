//// mstar_mutual_parse_packet @ffffff8008812fe8

/* WARNING: Restarted to delay deadcode elimination for space: ram */

void mstar_mutual_parse_packet(char *param_1,uint param_2,byte *param_3,undefined4 *param_4)

{
  long lVar1;
  int iVar2;
  byte bVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  char cVar8;
  byte *pbVar9;
  undefined4 *puVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  uint uVar14;
  uint *puVar15;
  uint uVar16;
  ulong uVar17;
  uint uVar18;
  long lVar19;
  uint *puVar20;
  undefined8 uStack_368;
  uint *puStack_360;
  undefined8 uStack_358;
  uint uStack_350;
  uint uStack_34c;
  short sStack_348;
  short sStack_344;
  short sStack_338;
  short sStack_334;
  short sStack_330;
  short sStack_32c;
  short sStack_328;
  short sStack_324;
  short sStack_320;
  short sStack_31c;
  undefined2 uStack_210;
  undefined1 uStack_20e;
  undefined4 uStack_208;
  long lStack_8;

  puVar15 = &uStack_350;
  param_2 = param_2 & 0xffff;
  uVar17 = (ulong)param_2;
  lStack_8 = lRamffffff80093cff08;
  __memset(puVar15,0,0x140);
  lVar12 = -0x7ff6800000;
  lVar13 = -0x7ff6805000;
  *(undefined1 *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0x109) = 0xff;
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ecd4d0,0xffffff8008c3b7f8);
  }
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ed8e71);
  }
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ed8eb0,*param_1,param_1[1],param_1[2],param_1[3],param_1[4],param_1[5],
           param_1[6],param_1[7],param_1[8]);
  }
  if ((cRamffffff8009800900 != '\0') && (*param_1 == ',')) {
    if (lRamffffff8009800908 == 0) goto LAB_ffffff8008813114;
    puVar15 = &uStack_208;
    __memset(puVar15,0,0x200);
    uStack_210 = 0;
    uStack_20e = 0;
    strcat(puVar15,0xffffff8008ed8f41);
    for (lVar12 = 0; (uint)lVar12 < param_2; lVar12 = lVar12 + 1) {
      sprintf(&uStack_210,0xffffff8008eba9d3,param_1[lVar12]);
      strcat(puVar15,&uStack_210);
    }
    uStack_368 = 0xffffff8008ed8f48;
    uStack_358 = 0;
    puStack_360 = puVar15;
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed8f5a,puVar15);
    }
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed8f76,param_1);
    }
    lVar12 = -0x7ff67ff780;
    uVar4 = kobject_uevent_env(lRamffffff8009800908,2,&uStack_368);
    param_4 = (undefined4 *)0xffffff8008eba9d3;
    if (cRamffffff80097fb3fc == '\0') goto LAB_ffffff8008813114;
    if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008813114;
    printk(0xffffff8008ed8f99,uVar4);
    param_4 = (undefined4 *)0xffffff8008eba9d3;
    goto LAB_ffffff8008813114;
  }
  cVar8 = '\0';
  for (lVar19 = 0; (uint)lVar19 < param_2 - 1; lVar19 = lVar19 + 1) {
    cVar8 = cVar8 + param_1[lVar19];
  }
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ed8fdc,param_1[uVar17 - 1],-cVar8);
  }
  if (param_1[uVar17 - 1] != -cVar8) {
    if ((uRamffffff8008b462c8 & 1) != 0) {
      printk(0xffffff8008ed9004);
    }
    iVar5 = dsm_client_ocuppy(uRamffffff80097fb418);
    if (iVar5 != 0) goto LAB_ffffff8008813114;
    dsm_client_record(uRamffffff80097fb418,0xffffff8008ed9021);
    dsm_client_notify(uRamffffff80097fb418,0x3731b323);
    goto LAB_ffffff8008813114;
  }
  if ((*(char *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0xc0) != '\x01') ||
     (cRamffffff8009800910 != '\x01')) {
    cVar8 = *param_1;
    if (cRamffffff80094ae4f0 == '\0') {
      if (cVar8 != 'Z') goto LAB_ffffff8008813a60;
LAB_ffffff8008813abc:
      pbVar9 = (byte *)(param_1 + 2);
      uVar17 = 3;
      puVar15 = (uint *)0x0;
      puVar10 = param_4;
      uVar16 = 0;
      while (uVar16 < uRamffffff8009800b1c) {
        iVar5 = (int)uVar17;
        bVar3 = param_1[iVar5 - 2];
        if (((bVar3 != 0xff) || (param_1[iVar5 - 1] != -1)) || (param_1[uVar17] != -1)) {
          uVar14 = (bVar3 & 0xf0) << 4 | (uint)*pbVar9;
          puVar15 = (uint *)(ulong)(uVar16 + 1);
          uVar18 = (bVar3 & 0xf) << 8 | (uint)pbVar9[1];
          bVar3 = *param_3;
          *(short *)(param_3 + (ulong)bVar3 * 8 + 4) =
               (short)(uVar14 * *(int *)(lRamffffff80098008b0 + 0x18c) >> 0xb);
          *(short *)(param_3 + (ulong)bVar3 * 8 + 6) =
               (short)(uVar18 * *(int *)(lRamffffff80098008b0 + 400) >> 0xb);
          *(ushort *)(param_3 + (ulong)bVar3 * 8 + 8) = (ushort)pbVar9[2];
          *(short *)(param_3 + (ulong)bVar3 * 8 + 2) = (short)uVar16;
          *puVar10 = 1;
          puVar10[1] = uVar14 * *(int *)(lRamffffff80098008b0 + 0x18c) >> 0xb;
          puVar10[2] = uVar18 * *(int *)(lRamffffff80098008b0 + 400) >> 0xb;
          puVar10[4] = (uint)pbVar9[2];
          *param_3 = *param_3 + 1;
        }
        uVar17 = (ulong)(iVar5 + 4);
        pbVar9 = pbVar9 + 4;
        puVar10 = puVar10 + 10;
        uVar16 = uVar16 + 1;
      }
      if ((cRamffffff80094ae4f0 == '\0') || (lRamffffff8009800908 == 0)) goto LAB_ffffff8008813ba4;
      __memset(&uStack_208,0,0x200);
      uStack_210 = 0;
      uStack_20e = 0;
      strcat(&uStack_208,0xffffff8008ed8f41);
      for (lVar12 = 0; (uint)lVar12 < param_2; lVar12 = lVar12 + 1) {
        sprintf(&uStack_210,0xffffff8008eba9d3,param_1[lVar12]);
        strcat(&uStack_208,&uStack_210);
      }
      uStack_368 = 0xffffff8008ed93f7;
      uStack_358 = 0;
      puStack_360 = &uStack_208;
      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
        printk(0xffffff8008ed8f5a,&uStack_208);
      }
      lVar12 = -0x7ff67ff780;
      uVar4 = kobject_uevent_env(lRamffffff8009800908,2,&uStack_368);
      if ((cRamffffff80097fb3fc == '\0') || ((uRamffffff8008b462c8 >> 2 & 1) == 0))
      goto LAB_ffffff8008813ba4;
      uVar7 = 0xffffff8008ed9413;
    }
    else {
      if ((sRamffffff80094ae4f2 == sRamffffff8009800b1a) && (cVar8 != 'Z')) {
        if (cRamffffff80097fb3fc == '\0') goto LAB_ffffff8008813114;
LAB_ffffff8008813a60:
        if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008813114;
        uVar7 = 0xffffff8008ed93ac;
LAB_ffffff8008813864:
        printk(uVar7);
        goto LAB_ffffff8008813114;
      }
      if (sRamffffff80094ae4f4 == sRamffffff8009800b1a) {
        if (cVar8 != -0x59) {
          if (param_1[3] != ' ') {
            if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008813114;
            uVar7 = 0xffffff8008ed93d1;
            goto LAB_ffffff8008813864;
          }
          goto LAB_ffffff8008813bac;
        }
      }
      else {
LAB_ffffff8008813bac:
        if (cVar8 == 'Z') goto LAB_ffffff8008813abc;
      }
      puVar15 = (uint *)0x0;
      if ((cVar8 != -0x59) || (param_1[3] != ' ')) goto LAB_ffffff8008813ba4;
      pbVar9 = (byte *)(param_1 + 6);
      uVar11 = 6;
      puVar10 = param_4;
      for (lVar19 = 0; lVar1 = lRamffffff8009800908, (uint)lVar19 < (uint)uRamffffff8009800b1c;
          lVar19 = lVar19 + 1) {
        iVar5 = (int)uVar11;
        bVar3 = param_1[iVar5 - 1];
        if (((bVar3 != 0xff) || (param_1[uVar11] != -1)) || (param_1[iVar5 + 1] != -1)) {
          puVar15 = (uint *)(ulong)((uint)lVar19 + 1);
          uVar16 = (bVar3 & 0xf0) << 4 | (uint)*pbVar9;
          uVar14 = (bVar3 & 0xf) << 8 | (uint)pbVar9[1];
          bVar3 = *param_3;
          *(short *)(param_3 + (ulong)bVar3 * 8 + 4) =
               (short)(uVar16 * *(int *)(lRamffffff80098008b0 + 0x18c) >> 0xb);
          iVar2 = *(int *)(lRamffffff80098008b0 + 400);
          *(short *)(param_3 + (ulong)bVar3 * 8 + 2) = (short)lVar19;
          (param_3 + (ulong)bVar3 * 8 + 8)[0] = 1;
          (param_3 + (ulong)bVar3 * 8 + 8)[1] = 0;
          *(short *)(param_3 + (ulong)bVar3 * 8 + 6) = (short)(uVar14 * iVar2 >> 0xb);
          *puVar10 = 1;
          puVar10[1] = uVar16 * *(int *)(lRamffffff80098008b0 + 0x18c) >> 0xb;
          puVar10[2] = uVar14 * *(int *)(lRamffffff80098008b0 + 400) >> 0xb;
          puVar10[4] = (uint)(byte)param_1[lVar19 * 4 + 4];
          *param_3 = *param_3 + 1;
        }
        pbVar9 = pbVar9 + 3;
        puVar10 = puVar10 + 10;
        uVar11 = (ulong)(iVar5 + 3);
      }
      if (lRamffffff8009800908 == 0) goto LAB_ffffff8008813ba4;
      __memcpy(0xffffff8009800b20,0xffffff8009801020,uVar17);
      uStack_368 = 0xffffff8008ed9460;
      puStack_360 = (uint *)0x0;
      uVar4 = kobject_uevent_env(lVar1,2,&uStack_368);
      if ((cRamffffff80097fb3fc == '\0') || ((uRamffffff8008b462c8 >> 2 & 1) == 0))
      goto LAB_ffffff8008813ba4;
      uVar7 = 0xffffff8008ed947d;
    }
    printk(uVar7,uVar4);
LAB_ffffff8008813ba4:
    param_4[100] = (int)puVar15;
    do {
      uVar7 = 0;
      while( true ) {
        if (lStack_8 == lRamffffff80093cff08) {
          return;
        }
        __stack_chk_fail(uVar7);
code_r0xffffff8008813784:
        if ((*(char *)(lVar13 + 0x3fc) != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
          printk(0xffffff8008ed91d9);
        }
        if (*(int *)(*(long *)(lVar12 + 0x8b0) + 0x100) != 0) break;
LAB_ffffff8008813114:
        uVar7 = 0xffffffff;
      }
      uVar4 = 0x44;
LAB_ffffff80088137f0:
      iVar5 = 6;
LAB_ffffff8008813634:
      lVar19 = *(long *)(*(long *)(lVar12 + 0x8b0) + 0x140);
      uVar6 = jiffies_to_msecs(0x5dc);
      __pm_wakeup_event(lVar19 + 0x24b0,uVar6);
      mutex_lock(lVar12 + 0x888);
      lVar19 = *(long *)(lVar12 + 0x8b0);
      if (*(int *)(lVar19 + 0xfc) == 1) {
        *(undefined4 *)(lVar19 + 0xfc) = 0;
        if (iVar5 == 2) {
          puVar15 = puVar15 + 1;
          iVar5 = 0;
          do {
            uVar16 = puVar15[-1];
            uVar14 = *puVar15;
            if ((*(char *)(lVar13 + 0x3fc) != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
              printk(0xffffff8008ed932f,0xffffff8008c3b818,iVar5,(int)(short)uVar16,
                     (int)(short)uVar14);
            }
            *(int *)(*(long *)(lVar12 + 0x8b0) + (long)iVar5 * 4 + 0x114) =
                 (int)(short)uVar14 | (int)(short)uVar16 << 0x10;
            if ((*(char *)(lVar13 + 0x3fc) != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
              printk(0xffffff8008ecbebb,iVar5);
            }
            iVar5 = iVar5 + 1;
            puVar15 = puVar15 + 2;
          } while (iVar5 != 2);
        }
        else {
          *(int *)(lVar19 + 0x114) = (int)(short)uStack_34c | (int)(short)uStack_350 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ed9367);
          }
          *(int *)(*(long *)(lVar12 + 0x8b0) + 0x118) = (int)sStack_344 | (int)sStack_348 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ecbf70);
          }
          *(int *)(*(long *)(lVar12 + 0x8b0) + 0x11c) = (int)sStack_334 | (int)sStack_338 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ecbfb2);
          }
          *(int *)(*(long *)(lVar12 + 0x8b0) + 0x120) = (int)sStack_32c | (int)sStack_330 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ecbffc);
          }
          *(int *)(*(long *)(lVar12 + 0x8b0) + 0x124) = (int)sStack_324 | (int)sStack_328 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ecc04d);
          }
          *(int *)(*(long *)(lVar12 + 0x8b0) + 0x128) = (int)sStack_31c | (int)sStack_320 << 0x10;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            printk(0xffffff8008ecc092);
          }
        }
        cVar8 = *(char *)(lVar13 + 0x3fc);
        param_4[0x65] = uVar4;
        if ((cVar8 != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
          printk(0xffffff8008ecc0da,0xffffff8008c3b7f8,uVar4);
        }
      }
      mutex_unlock(lVar12 + 0x888);
    } while( true );
  }
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ed8e71);
  }
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ed904b,*param_1,param_1[1],param_1[2],param_1[3],param_1[4],param_1[5]);
  }
  if (*param_1 != -0x59) {
    uStack_208._0_2_ = CONCAT11((char)((uint)uRamffffff80094ae4e8 >> 8),0x58);
    uStack_208 = (uint)CONCAT12((char)uRamffffff80094ae4e8,(undefined2)uStack_208);
    iVar5 = mstar_iic_write_data(uRamffffff80094ae4e0,&uStack_208,3);
    if (iVar5 < 1) goto LAB_ffffff8008813114;
    lVar12 = 0x1a;
    lVar13 = 0x418958;
    while (lVar12 = lVar12 + -1, lVar12 != 0) {
      __const_udelay(0x418958);
    }
    lVar12 = -0x7ff67ff780;
    uRamffffff8009800b18 = 1;
    if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008813114;
    uVar7 = 0xffffff8008ed92cb;
    lVar12 = -0x7ff67ff780;
    goto LAB_ffffff8008813864;
  }
  if (param_1[1] != '\0') {
LAB_ffffff80088138ec:
    if ((uRamffffff8008b462c8 & 1) == 0) goto LAB_ffffff8008813114;
    uVar7 = 0xffffff8008ed92f7;
    goto LAB_ffffff8008813864;
  }
  if (param_1[2] == '\x06') {
    if (param_1[3] != 'P') goto LAB_ffffff80088138ec;
    uVar16 = (uint)(byte)param_1[4];
  }
  else {
    if ((param_1[2] != -0x80) || (param_1[3] != 'R')) goto LAB_ffffff80088138ec;
    uVar16 = (uint)(byte)param_1[4];
    lVar19 = 0;
    do {
      *(uint *)(lVar19 * 4 + -0x7ff67ff6e8) = (uint)(byte)param_1[lVar19];
      lVar19 = lVar19 + 1;
    } while (lVar19 != 6);
    pbVar9 = (byte *)(param_1 + 6);
    puVar20 = (uint *)0xffffff8009800930;
    lVar19 = 0;
    do {
      bVar3 = *pbVar9;
      if ((bVar3 == 0xff && pbVar9[1] == 0xff) && pbVar9[2] == 0xff) {
        uVar18 = 0;
        uVar14 = 0;
        if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
          printk(0xffffff8008ed90b0);
        }
      }
      else {
        uVar14 = ((bVar3 & 0xf0) << 4 | (uint)pbVar9[1]) * *(int *)(lRamffffff80098008b0 + 0x18c) >>
                 0xb;
        uVar18 = ((bVar3 & 0xf) << 8 | (uint)pbVar9[2]) * *(int *)(lRamffffff80098008b0 + 400) >>
                 0xb;
      }
      *(uint *)((long)puVar15 + lVar19) = uVar14;
      lVar1 = lVar19 + 8;
      *puVar20 = uVar14;
      puVar20[1] = uVar18;
      pbVar9 = pbVar9 + 3;
      *(uint *)((long)&uStack_34c + lVar19) = uVar18;
      puVar20 = puVar20 + 2;
      lVar19 = lVar1;
    } while (lVar1 != 0x140);
    uVar14 = ((byte)param_1[0xc] & 0xf0) << 4 | (uint)(byte)param_1[0xd];
    uVar18 = ((byte)param_1[0xc] & 0xf) << 8 | (uint)(byte)param_1[0xe];
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed90e1,uVar14,uVar18);
    }
    if (((param_1[0xc] == -1) && (param_1[0xd] == -1)) && (param_1[0xe] == -1)) {
      uVar18 = 0;
      uVar14 = 0;
    }
    else {
      uVar14 = uVar14 * *(int *)(lRamffffff80098008b0 + 0x18c) >> 0xb;
      uVar18 = uVar18 * *(int *)(lRamffffff80098008b0 + 400) >> 0xb;
      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
        printk(0xffffff8008ed9115,uVar14,uVar18);
      }
    }
    uRamffffff8009800a70 = (uint)(byte)param_1[0x7e];
    uRamffffff8009800a74 = (uint)(byte)param_1[0x7f];
    uRamffffff8009800940 = uVar14;
    uRamffffff8009800944 = uVar18;
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed9148,0x58);
    }
  }
  iVar5 = uVar16 - 0x58;
  if (cRamffffff80097fb3fc == '\0') {
    switch(iVar5) {
    case 0:
      goto code_r0xffffff8008813608;
    default:
LAB_ffffff8008813850:
      if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008813114;
      uVar7 = 0xffffff8008ed9275;
      goto LAB_ffffff8008813864;
    case 0xc:
      goto code_r0xffffff8008813784;
    case 0xd:
      goto code_r0xffffff80088137bc;
    case 0xe:
      goto code_r0xffffff80088137f8;
    case 0xf:
      goto code_r0xffffff8008813818;
    }
  }
  if ((uRamffffff8008b462c8 >> 2 & 1) == 0) {
    switch(iVar5) {
    case 0:
      goto code_r0xffffff80088135e8;
    default:
      goto LAB_ffffff8008813114;
    case 0xc:
      goto code_r0xffffff8008813784;
    case 0xd:
      goto code_r0xffffff80088137bc;
    case 0xe:
      goto code_r0xffffff80088137f8;
    case 0xf:
      goto code_r0xffffff8008813818;
    }
  }
  printk(0xffffff8008ed917a,uVar16);
  switch(iVar5) {
  case 0:
code_r0xffffff80088135e8:
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed919b);
    }
code_r0xffffff8008813608:
    if (*(int *)(lRamffffff80098008b0 + 0x100) == 0) goto LAB_ffffff8008813114;
    uVar4 = 0x3b;
    iVar5 = 2;
    hwlog_to_jank(0x34,3,0xffffff8008ecbd95);
    goto LAB_ffffff8008813634;
  default:
    goto LAB_ffffff8008813850;
  case 0xc:
    goto code_r0xffffff8008813784;
  case 0xd:
    goto code_r0xffffff80088137bc;
  case 0xe:
code_r0xffffff80088137f8:
    if ((cRamffffff80097fb3fc == '\0') || ((uRamffffff8008b462c8 >> 2 & 1) == 0))
    goto code_r0xffffff80088137dc;
    uVar7 = 0xffffff8008ed9241;
    break;
  case 0xf:
code_r0xffffff8008813818:
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008ed9241);
    }
    if (*(int *)(lRamffffff80098008b0 + 0x100) == 0) goto LAB_ffffff8008813114;
    uVar4 = 0x43;
    goto LAB_ffffff80088137f0;
  }
code_r0xffffff80088137d8:
  printk(uVar7);
code_r0xffffff80088137dc:
  uVar4 = 0x42;
  if (*(int *)(lRamffffff80098008b0 + 0x100) != 0) goto LAB_ffffff80088137f0;
  goto LAB_ffffff8008813114;
code_r0xffffff80088137bc:
  if ((cRamffffff80097fb3fc == '\0') || ((uRamffffff8008b462c8 >> 2 & 1) == 0))
  goto code_r0xffffff80088137dc;
  uVar7 = 0xffffff8008ed920d;
  goto code_r0xffffff80088137d8;
}


//// mstar_get_touch_packet_addr @ffffff80088171d4

void mstar_get_touch_packet_addr(short *param_1,short *param_2)

{
  int iVar1;
  long lVar2;
  int iVar3;
  undefined1 auStack_18 [8];
  undefined4 uStack_10;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ecd4d0,0xffffff8008c3ba78);
  }
  if ((((ushort)(sRamffffff80098008c8 + 0xdee9U) < 2 || (ushort)(sRamffffff80098008c8 + 0xdee0U) < 2
       ) || (sRamffffff80098008c8 == 0x85 || sRamffffff80098008c8 == 0xbf)) ||
     ((sRamffffff80098008c8 == 0x7a && (cRamffffff8009801674 != '\0')))) {
    auStack_18[0] = 5;
    uStack_10 = 0;
    mutex_lock(0xffffff8009801520);
    if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
      printk(0xffffff8008eda3fd,0xffffff8008c3ba78);
    }
    iVar3 = 5;
    do {
      lVar2 = 0x15;
      while (lVar2 = lVar2 + -1, lVar2 != 0) {
        __const_udelay(0x418958);
      }
      iVar1 = mstar_iic_write_data(uRamffffff80094ae4e0,auStack_18,1);
      if ((0 < iVar1) && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
        printk(0xffffff8008eda854);
      }
      lVar2 = 0x15;
      while (lVar2 = lVar2 + -1, lVar2 != 0) {
        __const_udelay(0x418958);
      }
      iVar1 = mstar_iic_read_data(uRamffffff80094ae4e0,&uStack_10,4);
      if (0 < iVar1) {
        if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
          printk(0xffffff8008eda89a);
        }
        goto LAB_ffffff800881737c;
      }
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    if ((uRamffffff8008b462c8 & 1) != 0) {
      printk(0xffffff8008eda8df,iVar1);
    }
    if (iVar1 < 0) {
      uRamffffff8009801646 = 0;
    }
    else {
LAB_ffffff800881737c:
      *param_1 = (ushort)(byte)uStack_10 * 0x100 + (ushort)uStack_10._1_1_;
      *param_2 = (ushort)uStack_10._2_1_ * 0x100 + (ushort)uStack_10._3_1_;
      uRamffffff8009801646 = 1;
      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
        printk(0xffffff8008eda916,*param_1);
      }
      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
        printk(0xffffff8008eda942,*param_2);
      }
    }
    if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
      printk(0xffffff8008eda48b,0xffffff8008c3ba78);
    }
    mutex_unlock(0xffffff8009801520);
  }
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail();
  }
  return;
}


//// mstar_iic_read_data @ffffff8008813e70

int mstar_iic_read_data(uint param_1,undefined8 param_2,undefined2 param_3)

{
  int iVar1;
  undefined8 uVar2;

  param_1 = param_1 & 0xff;
  if (lRamffffff80098008c0 == 0) {
    if ((uRamffffff8008b462c8 & 1) == 0) {
      return 0;
    }
    uVar2 = 0xffffff8008ed8da6;
  }
  else {
    if (((((sRamffffff80098008c8 != 0x85 && sRamffffff80098008c8 != 0xbf) &&
          sRamffffff80098008c8 != 0x2117) &&
          ((sRamffffff80098008c8 == 0x85 || sRamffffff80098008c8 == 0xbf) ||
          sRamffffff80098008c8 != 0x2118)) || (param_1 != uRamffffff80094ae4e0)) ||
       (cRamffffff80098008ca == '\0')) {
      *(short *)(*(long *)(*(long *)(lRamffffff80098008b0 + 0x140) + 0x60) + 2) = (short)param_1;
      iVar1 = tpkit_i2c_read(param_2,param_3);
      if (-1 < iVar1) {
        return iVar1;
      }
      if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008ed94cb,iVar1,param_1,param_3);
      }
      uRamffffff80098008f8 = 0xff;
      return iVar1;
    }
    if ((uRamffffff8008b462c8 & 1) == 0) {
      return 0;
    }
    uVar2 = 0xffffff8008ed8d19;
  }
  printk(uVar2);
  return 0;
}


//// mstar_init_chip @ffffff80088176e8

void mstar_init_chip(void)

{
  undefined4 *puVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  undefined8 uVar5;
  undefined *puVar6;
  undefined *puVar7;
  long *plVar8;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  long lStack_8;

  plVar8 = (long *)0xffffff80093cf000;
  puVar6 = (undefined *)0xffffff8008b46000;
  lStack_8 = lRamffffff80093cff08;
  *(undefined1 *)(lRamffffff80098008b0 + 5) = 0;
  lVar2 = lRamffffff80098008b0;
  puVar1 = (undefined4 *)(lRamffffff80098008b0 + 0x100);
  lVar4 = *(long *)(lRamffffff80098008b0 + 0x140);
  *(undefined4 *)(lRamffffff80098008b0 + 0xf8) = 0;
  *(undefined4 *)(lVar2 + 0xfc) = 0;
  *puVar1 = 0;
  *(undefined4 *)(lVar2 + 0x104) = 0;
  *(undefined4 *)(lVar2 + 0x108) = 0;
  *(undefined4 *)(lVar2 + 0x10c) = 0;
  *(undefined1 *)(lVar4 + 0x99) = 0;
  *(undefined1 *)(*(long *)(lVar2 + 0x140) + 0xcc) = 0;
  iVar3 = mstar_parse_dts(*(undefined8 *)(*(long *)(*(long *)(lVar2 + 0x140) + 0x78) + 0x238));
  puVar7 = puVar6;
  if (iVar3 == 0) {
    mstar_apknode_create_procfs();
    mstar_apknode_create_jni_msg();
    iVar3 = mstar_chip_type_init();
    if (iVar3 == -0x13) {
      mstar_unregister_device();
      iVar3 = -0x13;
      goto LAB_ffffff80088177a8;
    }
    uStack_20 = 0;
    if ((sRamffffff80098008c8 == 0x85 || sRamffffff80098008c8 == 0xbf) ||
        (sRamffffff80098008c8 == 0x2117 || sRamffffff80098008c8 == 0x2118)) {
      uRamffffff8009800b1a = 0;
      uRamffffff80094ae532 = 0xff;
      uRamffffff80094ae4f2 = 0;
      uRamffffff80094ae4f4 = 1;
      uRamffffff8009801630 = 0x2b;
      uRamffffff8009801688 = 0x500;
      uRamffffff8009800b1c = 10;
    }
    uStack_18 = 0;
    uStack_10 = 0;
    mstar_dbbus_enter_serial_debug();
    mstar_dbbus_stop_mcu();
    mstar_dbbus_iic_use_bus();
    mstar_dbbus_iic_reshape();
    mstar_mp_read_flash((undefined1)sRamffffff80098008c8,0x780,2,0x10,&uStack_18);
    strncpy(0xffffff8009801690,&uStack_18,10);
    uRamffffff800980169a = 0;
    if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
      printk(0xffffff8008eda9c2,0xffffff8009801690);
    }
    if (iRamffffff800980169c != 0) {
      uRamffffff800981b388 = uStack_10._3_1_;
      if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
        printk(0xffffff8008eda9df,uStack_10._2_1_);
      }
    }
    lVar2 = lRamffffff80098008b0;
    *(undefined1 *)(lRamffffff80098008b0 + 0xe0) = uStack_10._4_1_;
    *(undefined1 *)(lVar2 + 0xe3) = uStack_10._5_1_;
    if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
      printk(0xffffff8008edaa01);
    }
    mstar_mp_read_flash((undefined1)sRamffffff80098008c8,0x7a0,2,4,&uStack_18);
    uRamffffff80098016a0 = uStack_18._2_1_;
    uRamffffff80098016a1 = uStack_18._3_1_;
    if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
      printk(0xffffff8008edaa34);
    }
    mstar_dbbus_iic_not_use_bus();
    mstar_dbbus_not_stop_mcu();
    mstar_dbbus_exit_serial_debug();
    uRamffffff80098016a2 = mstar_get_swid(2);
    mstar_dev_hw_reset();
    iVar3 = mstar_get_vendor_name_from_dts(0xffffff8009801690,&uStack_20,8);
    if (iVar3 != 0) {
      if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008edaa63,0xffffff8008c3bad8);
      }
      if ((uRamffffff80098016a2 & 0xbfff) != 8) goto LAB_ffffff8008817a1c;
      strncpy(&uStack_20,0xffffff8008e109d0,8);
    }
    strncpy(lRamffffff80098008b0 + 0x3c,&uStack_20,0x1f);
    if (iVar3 != 0) goto LAB_ffffff8008817a44;
    puVar6 = &_hwtag_TS_KIT;
    if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008817a00;
    uVar5 = 0xffffff8008ecba7a;
    goto LAB_ffffff80088179fc;
  }
  if ((uRamffffff8008b462c8 & 1) == 0) goto LAB_ffffff80088177a8;
  uVar5 = 0xffffff8008ecb910;
  do {
    printk(uVar5,0xffffff8008c3bac8,iVar3);
    puVar7 = puVar6;
LAB_ffffff80088177a8:
    do {
      puVar6 = puVar7 + 0x2c0;
      if ((*(uint *)(puVar7 + 0x2c8) & 1) != 0) {
        uVar5 = 0xffffff8008edaa98;
LAB_ffffff80088179fc:
        printk(uVar5,0xffffff8008c3bac8);
      }
LAB_ffffff8008817a00:
      plVar8 = plVar8 + 0x1e1;
      if (lStack_8 == *plVar8) {
        return;
      }
      __stack_chk_fail(iVar3);
LAB_ffffff8008817a1c:
      if ((*(uint *)(puVar6 + 0x2c8) & 1) != 0) {
        printk(0xffffff8008ecb9c1,0xffffff8008c3bad8,iVar3);
      }
LAB_ffffff8008817a44:
      puVar7 = puVar6;
    } while ((*(uint *)(puVar6 + 0x2c8) & 1) == 0);
    uVar5 = 0xffffff8008ecba51;
  } while( true );
}
