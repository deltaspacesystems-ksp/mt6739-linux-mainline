//// spm_mtcmos_ctrl_mfg @ffffff80088f8d94

undefined8 spm_mtcmos_ctrl_mfg(int param_1)

{
  uint *puVar1;

  if (param_1 == 0) {
    *(uint *)(lRamffffff80098423a0 + 0x200) = *(uint *)(lRamffffff80098423a0 + 0x200) & 0xffffff7f;
    DataSynchronizationBarrier(3,3,0);
    do {
      if ((*puRamffffff80098423a8 >> 0x18 & 1) != 0) break;
    } while ((*puRamffffff80098423a8 | 0xffff3fff) != 0xffff3fff);
    *(uint *)(lRamffffff80098423a0 + 0x200) = *(uint *)(lRamffffff80098423a0 + 0x200) | 2;
    DataSynchronizationBarrier(3,3,0);
    *(undefined4 *)(lRamffffff80098423a0 + 0x2a0) = 0x600000;
    DataSynchronizationBarrier(3,3,0);
    spm_topaxi_protect_ready1();
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) | 0x20;
    DataSynchronizationBarrier(3,3,0);
    puVar1 = (uint *)(lRamffffff8009842398 + 0x338);
    do {
    } while ((*puVar1 >> 6 & 1) == 0);
    *puVar1 = *puVar1 | 2;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) | 0x10;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xfffffffe;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xfffffffb;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xfffffff7;
    DataSynchronizationBarrier(3,3,0);
    do {
      do {
      } while ((*(uint *)(lRamffffff8009842398 + 0x180) >> 4 & 1) != 0);
    } while ((*(uint *)(lRamffffff8009842398 + 0x184) >> 4 & 1) != 0);
  }
  else {
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) | 4;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) | 8;
    DataSynchronizationBarrier(3,3,0);
    do {
      do {
      } while ((*(uint *)(lRamffffff8009842398 + 0x180) >> 4 & 1) == 0);
    } while ((*(uint *)(lRamffffff8009842398 + 0x184) >> 4 & 1) == 0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xffffffef;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xfffffffd;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) | 1;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x338) = *(uint *)(lRamffffff8009842398 + 0x338) & 0xffffffdf;
    DataSynchronizationBarrier(3,3,0);
    do {
    } while ((*(uint *)(lRamffffff8009842398 + 0x338) >> 6 & 1) != 0);
    *(undefined4 *)(lRamffffff80098423a0 + 0x2a4) = 0x600000;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff80098423a0 + 0x200) = *(uint *)(lRamffffff80098423a0 + 0x200) & 0xfffffffd;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff80098423a0 + 0x200) = *(uint *)(lRamffffff80098423a0 + 0x200) | 0x80;
    DataSynchronizationBarrier(3,3,0);
    do {
      if ((*puRamffffff80098423a8 >> 0x18 & 1) != 0) {
        return 0;
      }
    } while ((*puRamffffff80098423a8 | 0xffff3fff) != 0xffff3fff);
  }
  return 0;
}


//// spm_mtcmos_ctrl_mfg_core0 @ffffff80088f9390

undefined8 spm_mtcmos_ctrl_mfg_core0(int param_1)

{
  uint *puVar1;

  if (param_1 == 0) {
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) | 0x20;
    DataSynchronizationBarrier(3,3,0);
    puVar1 = (uint *)(lRamffffff8009842398 + 0x33c);
    do {
    } while ((*puVar1 >> 6 & 1) == 0);
    *puVar1 = *puVar1 | 2;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) | 0x10;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xfffffffe;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xfffffffb;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xfffffff7;
    DataSynchronizationBarrier(3,3,0);
    do {
      do {
      } while (*(int *)(lRamffffff8009842398 + 0x180) < 0);
    } while (*(int *)(lRamffffff8009842398 + 0x184) < 0);
  }
  else {
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) | 4;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) | 8;
    DataSynchronizationBarrier(3,3,0);
    do {
      do {
      } while (-1 < *(int *)(lRamffffff8009842398 + 0x180));
    } while (-1 < *(int *)(lRamffffff8009842398 + 0x184));
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xffffffef;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xfffffffd;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) | 1;
    DataSynchronizationBarrier(3,3,0);
    *(uint *)(lRamffffff8009842398 + 0x33c) = *(uint *)(lRamffffff8009842398 + 0x33c) & 0xffffffdf;
    DataSynchronizationBarrier(3,3,0);
    do {
    } while ((*(uint *)(lRamffffff8009842398 + 0x33c) >> 6 & 1) != 0);
  }
  return 0;
}


//// MTKDisablePowerDomain @ffffff8008672ae0

void MTKDisablePowerDomain(void)

{
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
