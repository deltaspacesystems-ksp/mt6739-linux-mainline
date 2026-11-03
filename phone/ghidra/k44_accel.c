//// MC3XXX_ValidateSensorIC @ffffff80084a16e8

undefined8 MC3XXX_ValidateSensorIC(byte *param_1,byte *param_2)

{
  uint uVar1;
  byte bVar2;
  uint uVar3;
  ulong uVar4;
  
  printk(0xffffff8008e1416c,0xffffff8008bd3e90,*param_1,*param_2);
  bVar2 = *param_2;
  if ((byte)(bVar2 - 3) < 0xd || bVar2 == 1) {
    if ((*param_1 + 0x78 & 0xf7) == 0 || *param_1 == 0x19) {
      return 0;
    }
  }
  else {
    uVar4 = 1;
    if (bVar2 < 0x22) {
      uVar4 = ~(0x2ffff0004U >> ((ulong)bVar2 & 0x3f));
    }
    if ((uVar4 & 1) == 0) {
      bVar2 = *param_1;
      uVar1 = bVar2 + 0x78 & 0xff;
      uVar3 = 1;
      if (uVar1 < 0x31) {
        uVar3 = ~(uint)(0x1000100000101 >> ((ulong)uVar1 & 0x3f));
      }
      if (((bVar2 & 0xffffffdf) == 0x19 || bVar2 == 0x29) || (uVar3 & 1) != 1) {
        return 0;
      }
    }
    else if ((byte)(bVar2 + 0x40) < 0x10) {
      bVar2 = *param_1;
      *param_1 = bVar2 & 0x71;
      if (((bVar2 & 0x71) - 0x30 & 0xef) == 0) {
        return 0;
      }
    }
    else if ((byte)(bVar2 + 0x60) < 0x10) {
      bVar2 = *param_1;
      *param_1 = bVar2 & 0xf1;
      if ((bVar2 & 0xf1) == 0x20) {
        uRamffffff80095d8350 = 0xffffff800944bc18;
        return 0;
      }
    }
    else if (bVar2 == 0x20 || (byte)(bVar2 - 0x22) < 0xe) {
      bVar2 = *param_1;
      *param_1 = bVar2 & 0xf1;
      if ((byte)((bVar2 & 0x31) - 0x20) < 2 || (byte)((bVar2 & 0x71) - 0x10) < 2) {
        return 0;
      }
    }
  }
  return 0xfffffffa;
}


//// _mc3xxx_i2c_auto_probe @ffffff80084a3c24

void _mc3xxx_i2c_auto_probe(long param_1)

{
  char cVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined4 uVar6;
  int aiStack_30 [6];
  ushort auStack_18 [4];
  ushort auStack_10 [4];
  long lStack_8;
  
  lStack_8 = lRamffffff80093cff08;
  auStack_18[0] = 0;
  auStack_10[0] = 0;
  aiStack_30[0] = 0;
  aiStack_30[1] = 0;
  aiStack_30[2] = 0;
  aiStack_30[3] = 0;
  aiStack_30[4] = 0;
  aiStack_30[5] = 0;
LAB_ffffff80084a3c78:
  lVar3 = 0;
  uRamffffff80095d83a9 = 0;
  uRamffffff80095d83b8 = 0;
  lVar4 = -0x7ff6a28000;
  do {
    *(undefined2 *)(param_1 + 2) = *(undefined2 *)(lVar3 * 2 + -0x7ff742bef8);
    auStack_18[0] = auStack_18[0] & 0xff00;
    iVar2 = MC3XXX_i2c_read_block(param_1,0x3b,auStack_18,1);
    lVar5 = lVar4;
    if (-1 < iVar2) {
      iVar2 = aiStack_30[lVar3];
      cVar1 = (char)auStack_18[0];
      aiStack_30[lVar3] = iVar2 + 1;
      if (cVar1 == '\0') {
        if (iVar2 + 1 == 1) break;
      }
      else {
        auStack_10[0] = auStack_10[0] & 0xff00;
        MC3XXX_i2c_read_block(param_1,0x18,auStack_10,1);
        iVar2 = MC3XXX_ValidateSensorIC(auStack_18,auStack_10);
        if (iVar2 == 0) {
          lVar5 = lVar4 + 800;
          lVar3 = lVar4 + 0x430;
          *(char *)(lVar4 + 0x3a9) = (char)auStack_18[0];
          *(undefined1 *)(lVar4 + 0x3b8) = (undefined1)auStack_10[0];
          printk(0xffffff8008e14891,0xffffff8008bd4118);
          MC3XXX_i2c_read_block(param_1,0x21,lVar3,3);
          MC3XXX_i2c_read_block(param_1,0x24,lVar4 + 0x433,3);
          printk(0xffffff8008e14edb,*(undefined1 *)(lVar4 + 0x430),*(undefined1 *)(lVar4 + 0x431),
                 *(undefined1 *)(lVar4 + 0x432),*(undefined1 *)(lVar4 + 0x433),
                 *(undefined1 *)(lVar4 + 0x434),*(undefined1 *)(lVar4 + 0x435));
          uVar6 = 0;
          goto LAB_ffffff80084a3d80;
        }
      }
    }
    while (lVar3 = lVar3 + 1, lVar4 = lVar5, lVar3 == 6) {
      uVar6 = 0xffffffff;
LAB_ffffff80084a3d80:
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      __stack_chk_fail(uVar6);
    }
  } while( true );
  MC3XXX_reset(param_1);
  msleep(3);
  goto LAB_ffffff80084a3c78;
}


//// MC3XXX_ReadData @ffffff80084a1be0

void MC3XXX_ReadData(long param_1,short *param_2)

{
  int iVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  byte bVar9;
  int iVar10;
  undefined8 uVar11;
  long lVar12;
  undefined8 uVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  undefined4 uStack_10;
  short sStack_c;
  long lStack_8;
  
  plVar15 = (long *)0xffffff80093cf000;
  sStack_c = 0;
  lStack_8 = lRamffffff80093cff08;
  uStack_10 = 0;
  if (param_1 != 0) goto LAB_ffffff80084a1c58;
  uVar11 = 0xffffff8008e143bd;
  uVar13 = 0x67f;
  do {
    printk(uVar11,0xffffff8008bd3ef8,uVar13);
    uVar11 = 0xfffffffe;
    while( true ) {
      plVar15 = plVar15 + 0x1e1;
      if (lStack_8 == *plVar15) {
        return;
      }
      __stack_chk_fail(uVar11);
LAB_ffffff80084a1c58:
      lVar16 = *(long *)(param_1 + 0xc0);
      if (lVar16 == 0) break;
      if ((*(uint *)(lVar16 + 0x20) >> 4 & 1) != 0) {
        printk(0xffffff8008e14413,0xffffff8008bd3ef8,0);
      }
      if (cRamffffff80095d83a8 == '\x01') {
        iVar10 = MC3XXX_i2c_read_block(param_1,0,&uStack_10,3);
        uVar11 = 0x692;
        if (iVar10 == 0) {
          *param_2 = (short)(char)uStack_10;
          param_2[1] = (short)uStack_10._1_1_;
          param_2[2] = (short)uStack_10._2_1_;
          if ((*(uint *)(lVar16 + 0x20) >> 4 & 1) != 0) {
            printk(0xffffff8008e1446f,0xffffff8008bd3ef8);
          }
          _MC3XXX_LowResFilter(0,param_2);
          _MC3XXX_LowResFilter(1,param_2);
          _MC3XXX_LowResFilter(2,param_2);
          goto LAB_ffffff80084a1d30;
        }
LAB_ffffff80084a1e94:
        printk(0xffffff8008e1443a,0xffffff8008bd3ef8,uVar11);
        uVar11 = 0xffffffff;
      }
      else {
        if (cRamffffff80095d83a8 == '\x02') {
          iVar10 = MC3XXX_i2c_read_block(param_1,0xd,&uStack_10,6);
          if (iVar10 != 0) {
            uVar11 = 0x6a8;
            goto LAB_ffffff80084a1e94;
          }
          *param_2 = (short)uStack_10;
          param_2[1] = uStack_10._2_2_;
          param_2[2] = sStack_c;
          if ((*(uint *)(lVar16 + 0x20) >> 4 & 1) != 0) {
            printk(0xffffff8008e1449a,0xffffff8008bd3ef8);
          }
        }
LAB_ffffff80084a1d30:
        if ((*(uint *)(lVar16 + 0x20) >> 4 & 1) != 0) {
          printk(0xffffff8008e144c6,(long)*param_2,(long)param_2[1],(long)param_2[2]);
        }
        lVar14 = *(long *)(param_1 + 0xc0);
        if (((*(int *)(lVar14 + 0x2c) != 0) && (*(int *)(lVar14 + 0x4c) != 0)) &&
           (*(int *)(lVar14 + 0x24) == 0)) {
          iVar10 = *(int *)(lVar14 + 0x48);
          iVar1 = *(int *)(lVar14 + 0x11c);
          if (iVar1 < iVar10) {
            lVar12 = lVar14 + (long)iVar1 * 6;
            *(short *)(lVar12 + 0x50) = *param_2;
            *(short *)(lVar12 + 0x52) = param_2[1];
            *(short *)(lVar12 + 0x54) = param_2[2];
            *(int *)(lVar14 + 0x110) = (int)*param_2 + *(int *)(lVar14 + 0x110);
            *(int *)(lVar14 + 0x114) = (int)param_2[1] + *(int *)(lVar14 + 0x114);
            *(int *)(lVar14 + 0x118) = (int)param_2[2] + *(int *)(lVar14 + 0x118);
            if ((*(uint *)(lVar14 + 0x20) & 1) != 0) {
              printk(0xffffff8008e144ec,iVar1,(long)*(short *)(lVar12 + 0x50),
                     (long)*(short *)(lVar12 + 0x52),(long)*(short *)(lVar12 + 0x54));
            }
            *(int *)(lVar14 + 0x11c) = *(int *)(lVar14 + 0x11c) + 1;
            *(int *)(lVar14 + 0x120) = *(int *)(lVar14 + 0x120) + 1;
          }
          else {
            iVar1 = *(int *)(lVar14 + 0x120);
            iVar6 = 0;
            if (iVar10 != 0) {
              iVar6 = iVar1 / iVar10;
            }
            iVar4 = iVar1 - iVar6 * iVar10;
            lVar12 = lVar14 + (long)iVar4 * 6;
            *(int *)(lVar14 + 0x110) = *(int *)(lVar14 + 0x110) - (int)*(short *)(lVar12 + 0x50);
            iVar7 = *(int *)(lVar14 + 0x114) - (int)*(short *)(lVar12 + 0x52);
            *(int *)(lVar14 + 0x114) = iVar7;
            iVar8 = *(int *)(lVar14 + 0x118) - (int)*(short *)(lVar12 + 0x54);
            *(int *)(lVar14 + 0x118) = iVar8;
            *(short *)(lVar12 + 0x50) = *param_2;
            *(short *)(lVar12 + 0x52) = param_2[1];
            *(short *)(lVar12 + 0x54) = param_2[2];
            iVar6 = (int)*param_2 + *(int *)(lVar14 + 0x110);
            *(int *)(lVar14 + 0x110) = iVar6;
            sVar2 = 0;
            if (iVar10 != 0) {
              sVar2 = (short)(iVar6 / iVar10);
            }
            *(int *)(lVar14 + 0x114) = param_2[1] + iVar7;
            sVar3 = param_2[2];
            *(int *)(lVar14 + 0x120) = iVar1 + 1;
            *(int *)(lVar14 + 0x118) = sVar3 + iVar8;
            *param_2 = sVar2;
            sVar3 = 0;
            if (iVar10 != 0) {
              sVar3 = (short)(*(int *)(lVar14 + 0x114) / iVar10);
            }
            param_2[1] = sVar3;
            sVar5 = 0;
            if (iVar10 != 0) {
              sVar5 = (short)(*(int *)(lVar14 + 0x118) / iVar10);
            }
            param_2[2] = sVar5;
            if ((*(uint *)(lVar14 + 0x20) & 1) != 0) {
              printk(0xffffff8008e14522,iVar4,(long)*(short *)(lVar12 + 0x50),
                     (long)*(short *)(lVar12 + 0x52),(long)*(short *)(lVar12 + 0x54),
                     *(undefined4 *)(lVar14 + 0x110),*(undefined4 *)(lVar14 + 0x114),
                     *(undefined4 *)(lVar14 + 0x118),(int)sVar2,(int)sVar3,(int)sVar5);
            }
          }
        }
        if ((*(uint *)(lVar16 + 0x20) >> 4 & 1) != 0) {
          printk(0xffffff8008e14568,(long)*param_2,(long)param_2[1],(long)param_2[2]);
        }
        bVar9 = bRamffffff80095d83aa;
        if (cRamffffff80095d83a9 == -0x78) {
          sVar2 = *param_2;
          *param_2 = param_2[1];
          param_2[1] = -sVar2;
          printk(0xffffff8008e1458e,0xffffff8008bd3ef8);
          uVar11 = 0;
        }
        else {
          if ((bRamffffff80095d83aa & 1) != 0) {
            *param_2 = -*param_2;
          }
          uVar11 = 0;
          if ((bVar9 >> 1 & 1) != 0) {
            param_2[1] = -param_2[1];
          }
        }
      }
    }
    uVar13 = 0x686;
    uVar11 = 0xffffff8008e143e4;
  } while( true );
}


//// MC3XXX_SetPowerMode @ffffff80084a2c70

void MC3XXX_SetPowerMode(long param_1,char param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  undefined2 uStack_18;
  undefined2 auStack_10 [4];
  long lStack_8;
  
  uStack_18 = 0;
  lStack_8 = lRamffffff80093cff08;
  auStack_10[0] = 0;
  lVar4 = *(long *)(param_1 + 0xc0);
  if (cRamffffff80095d83d4 == param_2) {
    printk(0xffffff8008e149fc);
  }
  iVar2 = MC3XXX_i2c_read_block(param_1,7,&uStack_18,1);
  if (iVar2 == 0) goto LAB_ffffff80084a2d30;
  printk(0xffffff8008e14a38,0xffffff8008bd4078,0x811);
  do {
    iVar2 = -1;
    cVar1 = cRamffffff80095d83d4;
    while( true ) {
      cRamffffff80095d83d4 = cVar1;
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      __stack_chk_fail(iVar2);
LAB_ffffff80084a2d30:
      printk(0xffffff8008e14a6a,(undefined1)uStack_18);
      if (param_2 == '\0') {
        uStack_18 = CONCAT11(uStack_18._1_1_,0x43);
        iVar3 = MC3XXX_i2c_write_block(param_1,7,&uStack_18,1);
      }
      else {
        uStack_18 = CONCAT11(uStack_18._1_1_,0x41);
        iVar3 = MC3XXX_i2c_write_block(param_1,7,&uStack_18,1);
        MC3XXX_i2c_read_block(param_1,7,auStack_10,1);
        printk(0xffffff8008e14aa2,(undefined1)auStack_10[0]);
      }
      if (iVar3 < 0) break;
      cVar1 = param_2;
      if ((*(uint *)(lVar4 + 0x20) >> 4 & 1) != 0) {
        printk(0xffffff8008e14b09,uStack_18._1_1_);
        cVar1 = param_2;
      }
    }
    printk(0xffffff8008e14ae1);
  } while( true );
}


//// MC3XXX_Init.isra.2 @ffffff80084a3dc8

void MC3XXX_Init_isra_2(long param_1)

{
  char cVar1;
  byte bVar2;
  bool bVar3;
  byte bVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  ushort auStack_20 [4];
  undefined2 uStack_18;
  undefined2 uStack_10;
  long lStack_8;
  
  lVar8 = -0x7ff742d000;
  plVar10 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  auStack_20[0] = 0;
  printk(0xffffff8008e14891,0xffffff8008bd4138);
  auStack_20[0] = CONCAT11(auStack_20[0]._1_1_,0x43);
  MC3XXX_i2c_write_block(param_1,7,auStack_20,1);
  printk(0xffffff8008e14891,0xffffff8008bd4148);
  puVar6 = (undefined8 *)0xffffff80095d8000;
  if (bRamffffff80095d83a9 == 0x39) {
LAB_ffffff80084a3e78:
    uRamffffff80095d83a8 = 1;
  }
  else {
    if (bRamffffff80095d83a9 < 0x3a) {
      if (bRamffffff80095d83a9 != 0x20) {
        if (bRamffffff80095d83a9 < 0x21) {
          if (bRamffffff80095d83a9 == 0x10) goto LAB_ffffff80084a3f98;
          bVar3 = bRamffffff80095d83a9 == 0x19;
        }
        else {
          if (bRamffffff80095d83a9 == 0x29) goto LAB_ffffff80084a3e78;
          bVar3 = bRamffffff80095d83a9 == 0x30;
        }
LAB_ffffff80084a3e74:
        if (bVar3) goto LAB_ffffff80084a3e78;
LAB_ffffff80084a3fa4:
        printk(0xffffff8008e14f30,0xffffff8008bd4148,0x86f);
        goto LAB_ffffff80084a3e84;
      }
    }
    else if (bRamffffff80095d83a9 != 0x88) {
      if (bRamffffff80095d83a9 < 0x89) {
        if (bRamffffff80095d83a9 != 0x40) {
          bVar3 = bRamffffff80095d83a9 == 0x60;
          goto LAB_ffffff80084a3e74;
        }
      }
      else if (((bRamffffff80095d83a9 != 0xa8) && (bRamffffff80095d83a9 != 0xb8)) &&
              (bRamffffff80095d83a9 != 0x90)) goto LAB_ffffff80084a3fa4;
    }
LAB_ffffff80084a3f98:
    uRamffffff80095d83a8 = 2;
  }
LAB_ffffff80084a3e84:
  printk(0xffffff8008e14f62,0xffffff8008bd4148,uRamffffff80095d83a8);
  uStack_18 = 0;
  printk(0xffffff8008e14891,0xffffff8008bd4160);
  uStack_18._1_1_ = 0;
  uStack_18._0_1_ = 8;
  puVar7 = puVar6;
  lVar9 = lVar8;
  if ((0xf < (byte)(cRamffffff80095d83b8 + 0x40U)) &&
     ((cRamffffff80095d83b8 != ' ' && 0xc < (byte)(cRamffffff80095d83b8 - 0x22U)) &&
      (cRamffffff80095d83b8 == ' ' || (byte)(cRamffffff80095d83b8 - 0x22U) != 0xd))) {
    uStack_18._0_1_ = 5;
    if (0xf < (byte)(cRamffffff80095d83b8 + 0x60U)) {
      uStack_18._0_1_ = 0;
    }
    goto LAB_ffffff80084a3fdc;
  }
  uStack_10._0_1_ = 0x2a;
  uStack_10._1_1_ = 0;
  MC3XXX_i2c_read_block(param_1,0x2a,&uStack_10,1);
  printk(0xffffff8008e14f86,0xffffff8008bd4160,(byte)uStack_10);
  bVar2 = (byte)uStack_10 & 0xc0;
  bVar4 = (byte)uStack_10 & 0xc0;
  uStack_10 = CONCAT11(uStack_10._1_1_,(byte)uStack_10) & 0xffc0;
  if (bVar4 == 0x40) goto LAB_ffffff80084a41f8;
  if (bVar4 < 0x41) {
    if (bVar2 == 0) {
      uStack_18._0_1_ = 0;
      goto LAB_ffffff80084a3fdc;
    }
  }
  else {
    if (bVar4 == 0x80) {
      uStack_18._0_1_ = 9;
      goto LAB_ffffff80084a3fdc;
    }
    if (bVar4 == 0xc0) {
      uStack_18._0_1_ = 10;
      goto LAB_ffffff80084a3fdc;
    }
  }
  printk(0xffffff8008e14fab,0xffffff8008bd4160,0x894);
LAB_ffffff80084a3fdc:
  do {
    MC3XXX_i2c_write_block(param_1,8,&uStack_18,1);
    uStack_10 = 0xf;
    if (*(char *)(puVar7 + 0x75) == '\x01') {
      uStack_10 = 0x32;
    }
    cVar1 = *(char *)(puVar7 + 0x77);
    if (((byte)(cVar1 + 0x40U) < 0x10) || (cVar1 == ' ' || (byte)(cVar1 - 0x22U) < 0xe)) {
      bVar4 = 2;
      if (*(char *)(puVar7 + 0x75) != '\x01') {
        bVar4 = 0x25;
      }
LAB_ffffff80084a4050:
      uStack_10 = (ushort)bVar4;
    }
    else if ((byte)(cVar1 + 0x60U) < 0x10) {
      bVar4 = 0x29;
      goto LAB_ffffff80084a4050;
    }
    iVar5 = MC3XXX_i2c_write_block(param_1,0x20,&uStack_10,1);
    if (iVar5 < 0) {
      printk(0xffffff8008e14fea,lVar9 + 0x1178,0x8bb);
    }
    printk(0xffffff8008e1501b,lVar9 + 0x1178,uStack_10._1_1_);
    *(undefined2 *)((long)puVar7 + 0x3c4) = 0x400;
    *(undefined2 *)((long)puVar7 + 0x3c2) = 0x400;
    *(undefined2 *)(puVar7 + 0x78) = 0x400;
    if (*(char *)(puVar7 + 0x75) == '\x01') {
      cVar1 = *(char *)(puVar7 + 0x77);
      *(undefined2 *)((long)puVar7 + 0x3c4) = 0x56;
      *(undefined2 *)((long)puVar7 + 0x3c2) = 0x56;
      *(undefined2 *)(puVar7 + 0x78) = 0x56;
      if (((byte)(cVar1 + 0x40U) < 0x10) || (cVar1 == ' ' || (byte)(cVar1 - 0x22U) < 0xe)) {
        *(undefined2 *)((long)puVar7 + 0x3c4) = 0x40;
        *(undefined2 *)((long)puVar7 + 0x3c2) = 0x40;
        *(undefined2 *)(puVar7 + 0x78) = 0x40;
      }
    }
    if ((byte)(*(char *)(puVar7 + 0x77) + 0x60U) < 0x10) {
      *(undefined2 *)((long)puVar7 + 0x3c4) = 0x1000;
      *(undefined2 *)((long)puVar7 + 0x3c2) = 0x1000;
      *(undefined2 *)(puVar7 + 0x78) = 0x1000;
    }
    puVar6 = puVar7 + 100;
    lVar8 = lVar9 + 0xe80;
    plVar10 = plVar10 + 0x1e1;
    printk(0xffffff8008e15036,lVar9 + 0x1190,*(undefined2 *)(puVar7 + 0x78),
           *(undefined2 *)((long)puVar7 + 0x3c2),*(undefined2 *)((long)puVar7 + 0x3c4));
    auStack_20[0] = auStack_20[0] & 0xff00;
    MC3XXX_i2c_write_block(param_1,9,auStack_20,1);
    auStack_20[0] = auStack_20[0] & 0xff00;
    MC3XXX_i2c_write_block(param_1,6,auStack_20,1);
    auStack_20[0] = auStack_20[0] & 0xff00;
    MC3XXX_i2c_read_block(param_1,0x2a,auStack_20,1);
    *(byte *)((long)puVar7 + 0x3aa) = (byte)auStack_20[0] & 3;
    __memset(*(long *)(param_1 + 0xc0) + 0x50,0,0xd4);
    *puVar6 = 0;
    puVar7[0x65] = 0;
    puVar7[0x66] = 0;
    puVar7[0x67] = 0;
    *(undefined2 *)(puVar7 + 0x69) = 0;
    puVar7[0x68] = 0;
    printk(0xffffff8008e1505b,lVar9 + 0x1138);
    if (lStack_8 == *plVar10) {
      return;
    }
    __stack_chk_fail(0);
LAB_ffffff80084a41f8:
    uStack_18._0_1_ = 8;
    puVar7 = puVar6;
    lVar9 = lVar8;
  } while( true );
}


