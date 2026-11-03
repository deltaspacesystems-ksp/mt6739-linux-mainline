//// RGXDevVersionString @ffffff8008644960

void RGXDevVersionString(long param_1,long *param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined1 auStack_40 [56];
  long lStack_8;

  uVar4 = 3;
  lStack_8 = lRamffffff80093cff08;
  if (param_1 != 0 && param_2 != (long *)0x0) {
    lVar5 = *(long *)(param_1 + 0x148);
    if (*(long *)(lVar5 + 0x70) == 0) {
      OSSNPrintf(auStack_40,0x32,0xffffff8008e6bd86,*(undefined4 *)(lVar5 + 0x18),
                 *(undefined4 *)(lVar5 + 0x1c),*(undefined4 *)(lVar5 + 0x20),
                 *(undefined4 *)(lVar5 + 0x24));
      lVar1 = OSStringLength(auStack_40);
      lVar2 = OSAllocMem(lVar1 + 1U & 0xffffffff);
      *(long *)(lVar5 + 0x70) = lVar2;
      uVar4 = 1;
      if (lVar2 == 0) goto LAB_ffffff8008644a38;
      __memcpy(lVar2,auStack_40,lVar1 + 1U,1);
    }
    lVar1 = OSStringLength(*(undefined8 *)(lVar5 + 0x70));
    lVar2 = OSStringLength(0xffffff8008e7948c);
    lVar3 = OSAllocZMem(lVar1 + lVar2 & 0xffffffff);
    *param_2 = lVar3;
    uVar4 = 1;
    if (lVar3 != 0) {
      OSSNPrintf(lVar3,lVar1 + lVar2,0xffffff8008e7948c,*(undefined8 *)(lVar5 + 0x70));
      uVar4 = 0;
    }
  }
LAB_ffffff8008644a38:
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar4);
  }
  return;
}


//// RGXParseBVNCList.constprop.11 @ffffff8008644a68

void RGXParseBVNCList_constprop_11
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               uint param_5)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  char *pcVar4;
  long *plVar5;
  undefined1 auVar6 [12];
  char acStack_d0 [200];
  long lStack_8;

  plVar5 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  if (param_5 == 0) {
    pcVar4 = acStack_d0;
    acStack_d0[0] = '\0';
    iVar1 = pvr_apphint_get_string(0x1c,pcVar4,0xc1);
    if (iVar1 == 0) {
      for (lVar2 = 0; (*pcVar4 != '\0' && (lVar2 != 0x10)); lVar2 = lVar2 + 1) {
        *(char **)(lVar2 * 8 + -0x7ff69a3880) = pcVar4;
        for (; *pcVar4 != ','; pcVar4 = pcVar4 + 1) {
          if (*pcVar4 == '\0') goto LAB_ffffff8008644b68;
        }
        *pcVar4 = '\0';
        pcVar4 = pcVar4 + 1;
LAB_ffffff8008644b68:
      }
      iRamffffff800965c778 = (int)lVar2;
    }
    else {
      iRamffffff800965c778 = 0;
    }
  }
  auVar6._8_4_ = iRamffffff800965c778;
  auVar6._0_8_ = 0xffffff800965c778;
  lVar2 = lRamffffff800965c780;
  if (iRamffffff800965c778 != 1) {
    if (iRamffffff800965c778 != 0) goto LAB_ffffff8008644ba4;
    goto LAB_ffffff8008644b88;
  }
  do {
    if (lVar2 == 0) goto LAB_ffffff8008644b88;
    iVar1 = OSVSScanf(lVar2,0xffffff8008e794ed,param_1,param_2,param_3,param_4);
    if ((iVar1 != 4) &&
       (iVar1 = OSVSScanf(lVar2,0xffffff8008e79524,param_1,param_2,param_3,param_4), iVar1 != 4))
    goto LAB_ffffff8008644b88;
    PVRSRVReleasePrintf(0xffffff8008e79501,lVar2);
    uVar3 = 0;
    while( true ) {
      plVar5 = plVar5 + 0x1e1;
      if (lStack_8 == *plVar5) {
        return;
      }
      auVar6 = __stack_chk_fail(uVar3);
LAB_ffffff8008644ba4:
      if (param_5 < auVar6._8_4_) break;
      PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e794a3,0xffffff8008c0ba08);
LAB_ffffff8008644b88:
      uVar3 = 0x143;
    }
    lVar2 = *(long *)(auVar6._0_8_ + 8 + (ulong)param_5 * 8);
  } while( true );
}


//// rgx_bvnc_packed @ffffff800865cfe0

void rgx_bvnc_packed(ulong *param_1,char *param_2,uint param_3,long param_4,char *param_5,
                    ulong param_6,ulong param_7)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;

  *param_1 = (param_6 & 0xffffffff) << 0x10 | param_7 & 0xffffffff | param_4 << 0x20;
  if (param_2 != (char *)0x0) {
    pcVar2 = param_5;
    if (param_5 == (char *)0x0) {
LAB_ffffff800865d000:
      pcVar2 = param_2;
      do {
        pcVar3 = pcVar2 + 1;
        *pcVar2 = '\0';
        pcVar2 = pcVar3;
      } while (pcVar3 != param_2 + (ulong)param_3 + 1);
      return;
    }
    for (; *pcVar2 != '\0'; pcVar2 = pcVar2 + 1) {
    }
    for (; param_3 != 0; param_3 = param_3 - 1) {
      cVar1 = *param_5;
      if (cVar1 == '\0') goto LAB_ffffff800865d000;
      if ((byte)(cVar1 - 0x30U) < 10) {
        *param_2 = cVar1;
        param_2 = param_2 + 1;
      }
      param_5 = param_5 + 1;
    }
    *param_2 = '\0';
  }
  return;
}


//// rgx_bvnc_pack_hw @ffffff800865d060

void rgx_bvnc_pack_hw(undefined8 *param_1,long param_2,undefined1 *param_3,long param_4,
                     ulong param_5,ulong param_6,ulong param_7)

{
  byte bVar1;
  undefined1 *puVar2;
  long extraout_x1;
  char *pcVar4;
  long lVar5;
  uint uVar6;
  undefined1 *puVar7;
  long *plVar8;
  undefined1 uStack_10;
  undefined1 auStack_f [2];
  char cStack_d;
  char acStack_c [4];
  long lStack_8;
  undefined1 *puVar3;

  plVar8 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  puVar7 = (undefined1 *)((param_6 & 0xffffffff) << 0x10 | param_7 & 0xffffffff | param_4 << 0x20);
  *param_1 = puVar7;
  if (param_2 == 0) goto LAB_ffffff800865d10c;
  puVar7 = &uStack_10;
  uVar6 = (uint)param_5;
  if (9999 < uVar6) goto LAB_ffffff800865d124;
  pcVar4 = acStack_c;
  if ((uVar6 < 1000) && (pcVar4 = &cStack_d, uVar6 < 100)) {
    puVar3 = puVar7;
    if (9 < uVar6) {
      puVar3 = auStack_f;
    }
    pcVar4 = puVar3 + 1;
  }
  do {
    *pcVar4 = '\0';
    pcVar4[-1] = '0';
    while( true ) {
      pcVar4 = pcVar4 + -1;
      if ((uint)param_5 == 0) break;
      uVar6 = (uint)param_5 / 10;
      *pcVar4 = (char)param_5 + (char)uVar6 * -10 + '0';
      param_5 = (ulong)uVar6;
    }
    lVar5 = 0;
LAB_ffffff800865d0e4:
    puVar3 = (undefined1 *)(param_2 + lVar5);
    if ((int)param_3 == 0) {
      *puVar3 = 0;
    }
    else {
      bVar1 = puVar7[lVar5];
      param_5 = (ulong)bVar1;
      if (bVar1 != 0) break;
      param_3 = puVar3 + ((ulong)param_3 & 0xffffffff) + 1;
      do {
        puVar2 = puVar3 + 1;
        *puVar3 = 0;
        puVar3 = puVar2;
      } while (puVar2 != param_3);
    }
LAB_ffffff800865d10c:
    plVar8 = plVar8 + 0x1e1;
    if (lStack_8 == *plVar8) {
      return;
    }
    __stack_chk_fail();
    param_2 = extraout_x1;
LAB_ffffff800865d124:
    pcVar4 = puVar7 + 5;
  } while( true );
  *(byte *)(param_2 + lVar5) = bVar1;
  param_3 = (undefined1 *)(ulong)((int)param_3 - 1);
  lVar5 = lVar5 + 1;
  goto LAB_ffffff800865d0e4;
}
