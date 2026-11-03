//// mstar_irq_bottom_half @ffffff8008814c5c

void mstar_irq_bottom_half(undefined8 param_1,undefined4 *param_2)

{
  byte bVar1;
  ushort uVar2;
  undefined4 uVar3;
  char cVar4;
  int iVar5;
  undefined8 uVar6;
  uint uVar7;
  undefined8 uVar8;
  undefined8 extraout_x1;
  undefined8 extraout_x1_00;
  long lVar9;
  long lVar10;
  long lVar11;
  undefined2 uVar12;
  long *plVar13;
  ulong auStack_68 [10];
  undefined2 uStack_18;
  undefined4 uStack_10;
  long lStack_8;

  plVar13 = (long *)0xffffff80093cf000;
  lVar9 = -0x7ff6800000;
  lStack_8 = lRamffffff80093cff08;
  uVar8 = 0;
  *param_2 = 1;
  cVar4 = cRamffffff80097fb3fc;
  param_2[4] = *(undefined4 *)(lRamffffff80098008b0 + 0x170);
  if ((cVar4 != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ecc1f2,0xffffff8008c3b848);
    uVar8 = extraout_x1;
  }
  lVar11 = -0x7ff67ff780;
  lVar10 = lVar9;
  if (iRamffffff8009801600 != 0) goto LAB_ffffff8008814d08;
  if (cRamffffff8009801604 != '\x01') goto LAB_ffffff8008814d88;
  if ((uRamffffff8008b462c8 >> 2 & 1) == 0) goto LAB_ffffff8008814d08;
  uVar6 = 0xffffff8008ed9861;
  do {
    printk(uVar6,uVar8);
    lVar10 = lVar9;
LAB_ffffff8008814d08:
    do {
      while( true ) {
        if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
          printk(0xffffff8008ed9bc4,0xffffff8008c3b848,*(undefined4 *)(lVar10 + 0x164c));
        }
        lVar11 = lVar10 + 0x880;
        uVar8 = _raw_spin_lock_irqsave(lVar10 + 0x1650);
        if ((*(int *)(lVar10 + 0x164c) == 0) && (*(int *)(lVar10 + 0x1600) == 0)) {
          *(undefined4 *)(lVar10 + 0x164c) = 1;
        }
        lVar9 = lVar10 + 0x880;
        plVar13 = plVar13 + 0x1e1;
        _raw_spin_unlock_irqrestore(lVar10 + 0x1650,uVar8);
        if (lStack_8 == *plVar13) {
          return;
        }
        __stack_chk_fail(0);
        uVar8 = extraout_x1_00;
LAB_ffffff8008814d88:
        lVar10 = lVar9;
        if ((*(char *)(lVar11 + 0xd85) != '\0') || (*(char *)(lVar11 + 0x298) != '\0')) break;
        mutex_lock(lVar11 + 0xca0);
        mutex_lock(lVar11 + 0xd88);
        auStack_68[0] = 0;
        auStack_68[1] = 0;
        auStack_68[2] = 0;
        auStack_68[3] = 0;
        auStack_68[4] = 0;
        auStack_68[5] = 0;
        auStack_68[6] = 0;
        auStack_68[7] = 0;
        auStack_68[8] = 0;
        auStack_68[9] = 0;
        uStack_18 = 0;
        if ((cRamffffff80094ae4f0 == '\0') ||
           (uVar2 = *(ushort *)(lVar11 + 0x29a), uRamffffff80094ae4f2 == uVar2)) {
          if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
            printk(0xffffff8008ed98f1);
          }
          lVar11 = lVar9 + 0x1548;
          uVar12 = *(undefined2 *)(lVar9 + 0x1630);
LAB_ffffff8008814e94:
          if (*(char *)(lVar9 + 0x910) == '\x01') {
            if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
              printk(0xffffff8008ed9994);
            }
            uVar12 = 0x80;
            lVar11 = lVar9 + 0x1580;
            msleep(0x78);
          }
          if (cRamffffff80094ae4f0 == '\0') {
            uVar7 = mstar_iic_read_data(uRamffffff80094ae4e0,lVar11,uVar12);
            if ((int)uVar7 < 0) {
              if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                uVar8 = 0xffffff8008ed9b60;
                goto LAB_ffffff8008814e84;
              }
            }
            else {
LAB_ffffff8008815178:
              iVar5 = mstar_mutual_parse_packet(lVar11,uVar12,auStack_68,param_2 + 5);
              if (((iVar5 == 0) && (cRamffffff80097fb3fc != '\0')) &&
                 ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
                printk(0xffffff8008ed9b93,auStack_68[0] & 0xff,0);
              }
            }
          }
          else {
            uVar2 = *(ushort *)(lVar9 + 0xb1a);
            if (uRamffffff80094ae4f2 == uVar2) {
              uVar7 = mstar_iic_read_data(uRamffffff80094ae4e0,lVar11,uVar12);
              if (-1 < (int)uVar7) goto LAB_ffffff8008815178;
              if ((uRamffffff8008b462c8 & 1) != 0) {
                uVar8 = 0xffffff8008ed99c3;
                goto LAB_ffffff8008814e84;
              }
            }
            else {
              if (uRamffffff80094ae4f4 != uVar2) goto LAB_ffffff8008814f28;
              if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                printk(0xffffff8008ed99f6,*(undefined2 *)(lVar9 + 0x1642));
              }
              if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                printk(0xffffff8008ed9a2a,*(undefined2 *)(lVar9 + 0x1644));
              }
              if (*(char *)(lVar9 + 0x1646) == '\0') {
                if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                  uVar8 = 0xffffff8008ed9a5e;
LAB_ffffff8008814fa4:
                  printk(uVar8);
                }
              }
              else if (*(char *)(lVar9 + 0x1604) == '\x01') {
                if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                  uVar8 = 0xffffff8008ed9861;
                  goto LAB_ffffff8008814fa4;
                }
              }
              else {
                uStack_10 = uStack_10 & 0xff000000;
                if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
                  printk(0xffffff8008ecd4d0,0xffffff8008c3b860);
                }
                uStack_10._0_2_ =
                     CONCAT11((char)((ushort)*(undefined2 *)(lVar9 + 0x1644) >> 8),0x53);
                uStack_10 = CONCAT22(CONCAT11(uStack_10._3_1_,(char)*(undefined2 *)(lVar9 + 0x1644))
                                     ,(undefined2)uStack_10);
                mstar_iic_write_data(uRamffffff80094ae4e0,&uStack_10,3);
                mstar_iic_read_data(uRamffffff80094ae4e0,lVar9 + 0x1648,2);
                bVar1 = *(byte *)(lVar9 + 0x1648);
                if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                  printk(0xffffff8008ed9aac,(bVar1 & 2) != 0);
                }
                if ((bVar1 & 2) == 0) {
                  uVar7 = mstar_ii2c_segment_read_smbus
                                    (*(undefined2 *)(lVar9 + 0x1642),lVar11,uVar12,0xfa);
                  uStack_10 = 0;
                  if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                    printk(0xffffff8008ecd4d0,0xffffff8008c3b888);
                  }
                  uStack_10._0_3_ = CONCAT12((char)*(undefined2 *)(lVar9 + 0x1644),0x52);
                  uVar3 = CONCAT13(*(undefined1 *)(lVar9 + 0x1648),(undefined3)uStack_10);
                  uStack_10._0_2_ =
                       CONCAT11((char)((ushort)*(undefined2 *)(lVar9 + 0x1644) >> 8),0x52);
                  uStack_10 = CONCAT22((short)((uint)uVar3 >> 0x10),(undefined2)uStack_10) |
                              0x2000000;
                  mstar_iic_write_data(uRamffffff80094ae4e0,&uStack_10,4);
                  if (-1 < (int)uVar7) goto LAB_ffffff8008815178;
                  if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                    uVar8 = 0xffffff8008ed9b22;
                    goto LAB_ffffff8008814e84;
                  }
                }
                else if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
                  uVar8 = 0xffffff8008ed9ac4;
                  goto LAB_ffffff8008814fa4;
                }
              }
            }
          }
        }
        else if (uRamffffff80094ae4f4 == uVar2) {
          if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b462c8 >> 2 & 1) != 0)) {
            printk(0xffffff8008ed9917);
          }
          uVar7 = (uint)*(byte *)(lVar9 + 0x163b);
          if (*(byte *)(lVar9 + 0x163b) == 0xa7) {
            uVar12 = *(undefined2 *)(lVar9 + 0x1640);
            lVar11 = lVar9 + 0x1020;
            goto LAB_ffffff8008814e94;
          }
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            uVar8 = 0xffffff8008ed993e;
            goto LAB_ffffff8008814e84;
          }
        }
        else {
LAB_ffffff8008814f28:
          uVar7 = (uint)uVar2;
          if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
            uVar8 = 0xffffff8008ed996b;
LAB_ffffff8008814e84:
            printk(uVar8,uVar7);
          }
        }
        mutex_unlock(lVar9 + 0x1608);
        mutex_unlock(lVar9 + 0x1520);
      }
      *(undefined1 *)(lVar9 + 0xb18) = 0;
    } while ((uRamffffff8008b462c8 & 1) == 0);
    uVar6 = 0xffffff8008ed98b8;
  } while( true );
}
