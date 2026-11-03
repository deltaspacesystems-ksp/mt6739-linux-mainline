//// huawei_ts_chip_register @ffffff80087ed40c

int huawei_ts_chip_register(long param_1)

{
  int iVar1;

  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec6c74);
  }
  if (param_1 == 0) {
    iVar1 = -0x16;
    if ((uRamffffff8008b461e8 & 1) != 0) {
      printk(0xffffff8008ec6ca6,0xffffff8008c38e38);
    }
  }
  else {
    iVar1 = -1;
    if ((cRamffffff80097fb410 == '\x01') && (iRamffffff80097fb838 == 0)) {
      iVar1 = 0;
      if (**(long **)(param_1 + 0xf0) != 0) {
        lRamffffff80097fb890 = param_1;
        iVar1 = (*(code *)**(long **)(param_1 + 0xf0))();
        if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
          printk(0xffffff8008ec6cc8,iVar1);
        }
        if (iVar1 == 0) {
          iRamffffff80097fb838 = 1;
          uRamffffff80097fb880 =
               kthread_create_on_node(ts_thread,0xffffff80097fb810,0xffffffff,0xffffff8008ec6cf8,0);
          if (uRamffffff80097fb880 < 0xfffffffffffff001) {
            DataMemoryBarrier(2,3);
            wake_up_process(uRamffffff80097fb880);
          }
          else {
            if ((uRamffffff8008b461e8 & 1) != 0) {
              printk(0xffffff8008ec6d05);
            }
            ts_destory_client();
            iVar1 = -0x16;
            __memset(0xffffff80097fb810,0,0x4968);
          }
        }
      }
    }
  }
  return iVar1;
}


//// tpd_probe @ffffff80087ed5d0

/* WARNING: Type propagation algorithm not settling */

void tpd_probe(long param_1)

{
  int iVar1;
  undefined8 uVar2;
  undefined *puVar3;
  undefined *puVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  int aiStack_18 [2];
  undefined8 uStack_10;
  long lStack_8;

  plVar10 = (long *)0xffffff80093cf000;
  lVar7 = -0x7ff6805000;
  puVar3 = (undefined *)0xffffff8008b46000;
  lStack_8 = lRamffffff80093cff08;
  uRamffffff80097fb410 = 0;
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec6d8d,0);
  }
  lVar5 = -0x7ff6805000;
  __memset(0xffffff80097fb810,0,0x4968);
  uRamffffff80097fb838 = 0;
  lRamffffff80097fb888 = param_1;
  if (param_1 == 0) {
    puVar4 = &_hwtag_TS_KIT;
    iVar1 = -0xc;
    lVar9 = 0;
    if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed9f4;
    uVar2 = 0xffffff8008ec6dc8;
    do {
      printk(uVar2);
LAB_ffffff80087ed9f4:
      do {
        plVar10 = plVar10 + 0x1e1;
        if (lStack_8 == *plVar10) {
          return;
        }
        __stack_chk_fail(iVar1);
        puVar3 = puVar4;
        lVar6 = lVar5;
        lVar8 = lVar7;
LAB_ffffff80087eda10:
        if (*(long *)(lVar8 + 0x418) == 0) {
          uVar2 = dsm_register_client(lVar9 + 0xc50);
          *(undefined8 *)(lVar8 + 0x418) = uVar2;
        }
        lVar5 = lVar6 + 0x810;
        lVar7 = lVar8 + 0x3f8;
        puVar4 = puVar3 + 0x1e0;
        *(undefined1 *)(lVar8 + 0x410) = 1;
        *(undefined4 *)(lVar6 + 0x848) = 1;
        *(undefined4 *)(lVar6 + 0x84c) = 0;
      } while ((*(uint *)(puVar3 + 0x1e8) >> 2 & 1) == 0);
      uVar2 = 0xffffff8008ec70c9;
    } while( true );
  }
  lVar9 = -0x7ff6b54000;
  aiStack_18[0] = 0;
  aiStack_18[1] = 0;
  lRamffffff80097fb868 = of_find_matching_node_and_match(0,0xffffff80094acca0,0);
  if (lRamffffff80097fb868 == 0) {
    if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
    uVar2 = 0xffffff8008ec6df4;
    goto LAB_ffffff80087ed6a4;
  }
  iVar1 = of_property_read_string(lRamffffff80097fb868,0xffffff8008ec6e1c,&uStack_10);
  if (iVar1 != 0) {
    if ((uRamffffff8008b461e8 & 1) != 0) {
      printk(0xffffff8008ec6e25,iVar1);
    }
    goto LAB_ffffff80087ed6dc;
  }
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec6e4b,uStack_10);
  }
  iVar1 = strcmp(uStack_10,0xffffff8008ee160b);
  if (iVar1 == 0) {
    piRamffffff80097fb878 = (int *)0xffffff80094ad6c8;
LAB_ffffff80087ed740:
    iVar1 = of_property_read_u32_array(lRamffffff80097fb868,0xffffff8008ec6e8a,aiStack_18,1);
    if (iVar1 == 0) {
      piRamffffff80097fb878[1] = aiStack_18[0];
      if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
        printk(0xffffff8008ec6eb2);
      }
      iVar1 = of_property_read_u32_array(lRamffffff80097fb868,0xffffff8008ec6ecb,aiStack_18 + 1,1);
      if (iVar1 == 0) {
        uRamffffff80097fb8f0 = (undefined1)aiStack_18[1];
        uRamffffff80098000f0 = devm_pinctrl_get(lRamffffff80097fb888 + 0x10);
        if ((uRamffffff80098000f0 != 0) && (uRamffffff80098000f0 < 0xfffffffffffff001)) {
          uRamffffff80098000f8 = pinctrl_lookup_state(uRamffffff80098000f0,0xffffff8008ec6f37);
          if ((uRamffffff80098000f8 == 0) || (0xfffffffffffff000 < uRamffffff80098000f8)) {
            if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
            uStack_10 = 0xffffff8008ec6f37;
            uVar2 = 0xffffff8008ec6f49;
            goto LAB_ffffff80087ed7b8;
          }
          uRamffffff8009800108 = pinctrl_lookup_state(uRamffffff80098000f0,0xffffff8008ec6f73);
          if ((uRamffffff8009800108 == 0) || (0xfffffffffffff000 < uRamffffff8009800108)) {
LAB_ffffff80087ed8b0:
            uStack_10 = 0xffffff8008ec6f73;
            if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
          }
          else {
            uRamffffff8009800120 = pinctrl_lookup_state(uRamffffff80098000f0,0xffffff8008ec6fb0);
            if ((uRamffffff8009800120 != 0) && (uRamffffff8009800120 < 0xfffffffffffff001)) {
              uRamffffff8009800110 = pinctrl_lookup_state(uRamffffff80098000f0,0xffffff8008ec6fc2);
              if ((uRamffffff8009800110 != 0) && (uRamffffff8009800110 < 0xfffffffffffff001)) {
                uRamffffff8009800118 = pinctrl_lookup_state(uRamffffff80098000f0,0xffffff8008ec6fd5)
                ;
                if ((uRamffffff8009800118 != 0) && (uRamffffff8009800118 < 0xfffffffffffff001)) {
                  uRamffffff8009800128 =
                       regulator_get(lRamffffff80097fb888 + 0x10,0xffffff8008ec6fe8);
                  if ((uRamffffff8009800128 != 0) && (uRamffffff8009800128 < 0xfffffffffffff001)) {
                    if (*piRamffffff80097fb878 == 0) {
                      if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b461e8 >> 2 & 1) != 0))
                      {
                        printk(0xffffff8008ec7034);
                      }
                      lVar9 = -0x7ff6b53470;
                      iVar1 = i2c_register_driver(0,0xffffff80094ad6f8);
                      lVar6 = lVar5;
                      lVar8 = lVar7;
                      if (iVar1 == 0) goto LAB_ffffff80087eda10;
                      iVar1 = -1;
                      if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed9d8;
                      uVar2 = 0xffffff8008ec7059;
                    }
                    else if (*piRamffffff80097fb878 == 1) {
                      lVar7 = -0x7ff6804c08;
                      iVar1 = -0x16;
                      if ((cRamffffff80097fb3fc == '\0') || ((uRamffffff8008b461e8 >> 2 & 1) == 0))
                      goto LAB_ffffff80087ed9d8;
                      uVar2 = 0xffffff8008ec707d;
                    }
                    else {
                      iVar1 = -0x16;
                      if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed9d8;
                      uVar2 = 0xffffff8008ec6c52;
                    }
                    printk(uVar2);
LAB_ffffff80087ed9d8:
                    puVar4 = &_hwtag_TS_KIT;
                    if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed9f4;
                    printk(0xffffff8008ec70a2,iVar1);
                    goto LAB_ffffff80087ed9f4;
                  }
                  if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
                  uVar2 = 0xffffff8008ec6fef;
                  goto LAB_ffffff80087ed6a4;
                }
              }
              goto LAB_ffffff80087ed8b0;
            }
            if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
            uStack_10 = 0xffffff8008ec6fb0;
          }
          uVar2 = 0xffffff8008ec6f85;
          goto LAB_ffffff80087ed7b8;
        }
        if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
        uVar2 = 0xffffff8008ec6f0a;
      }
      else {
        if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
        uVar2 = 0xffffff8008ec6edd;
      }
    }
    else {
      if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
      uVar2 = 0xffffff8008ec6e91;
    }
LAB_ffffff80087ed6a4:
    printk(uVar2);
  }
  else {
    iVar1 = strcmp(uStack_10,0xffffff8008ebbf14);
    if (iVar1 == 0) {
      piRamffffff80097fb878 = (int *)0xffffff80094ad6e0;
      goto LAB_ffffff80087ed740;
    }
    if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087ed6dc;
    uVar2 = 0xffffff8008ec6e68;
LAB_ffffff80087ed7b8:
    printk(uVar2,uStack_10);
  }
LAB_ffffff80087ed6dc:
  puVar4 = &_hwtag_TS_KIT;
  if ((uRamffffff8008b461e8 & 1) != 0) {
    printk(0xffffff8008ec700e,0xffffffea);
  }
  iVar1 = -0x16;
  goto LAB_ffffff80087ed9f4;
}


//// ts_thread @ffffff80087ef584

/* WARNING: Restarted to delay deadcode elimination for space: ram */

void ts_thread(void)

{
  uint *puVar1;
  int iVar2;
  byte *pbVar3;
  long lVar4;
  byte *pbVar5;
  undefined4 uVar6;
  int iVar7;
  byte bVar8;
  byte bVar9;
  uint uVar10;
  int *piVar11;
  char cVar12;
  undefined2 uVar13;
  int iVar14;
  long lVar15;
  undefined8 *puVar16;
  ulong uVar17;
  undefined8 uVar18;
  byte *pbVar19;
  code *pcVar20;
  long lVar21;
  ulong uVar22;
  long lVar23;
  long lVar24;
  ulong uVar25;
  int *piVar26;
  int *piVar27;
  long lVar28;
  int *piVar29;
  int *unaff_x25;
  ulong unaff_x26;
  long unaff_x27;
  long unaff_x28;
  long lStack_18;
  undefined4 uStack_10;
  undefined4 uStack_c;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  DataMemoryBarrier(2,2);
  lVar23 = -0x7ff74ba000;
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec81fe,uRamffffff8008b461e8,0);
  }
  lVar24 = -0x7ff6805000;
  uRamffffff80097fb830 = 0;
  uRamffffff80097fb834 = 0;
  if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec8216);
  }
  lVar28 = -0x7ff6805000;
  uRamffffff80097fb844 = 10;
  uRamffffff80097fb9b8 = 0;
  uRamffffff80097fb9ba = 0;
  uRamffffff80097fb9a8 = 0;
  uRamffffff80097fb9ac = 0;
  uRamffffff80097fb9b0 = 0;
  uRamffffff80097fb9b4 = 0x14;
  if ((cRamffffff80097fb3fc != '\0') && ((uRamffffff8008b461e8 >> 2 & 1) != 0)) {
    printk(0xffffff8008ec8230);
  }
  wakeup_source_prepare(0xffffff80097fdcc0,0xffffff8008ec825b);
  wakeup_source_add(0xffffff80097fdcc0);
  lStack_18 = 0;
  if ((lRamffffff80097fb868 == 0) ||
     (iVar14 = of_property_read_string(lRamffffff80097fb868,0xffffff8008ebf259,&lStack_18),
     iVar14 == 0)) {
    if (lStack_18 == 0) {
      if ((uRamffffff8008b461e8 & 1) != 0) {
        uVar18 = 0xffffff8008ec828d;
LAB_ffffff80087ef6f4:
        printk(uVar18);
      }
    }
    else {
      iVar14 = of_property_read_u32_array
                         (lRamffffff80097fb868,0xffffff8008ec82ab,lRamffffff80097fb890 + 0x170,1);
      lVar15 = lStack_18;
      if (iVar14 == 0) {
        uRamffffff80097fb810 = 0;
        uRamffffff80097fb818 = 0;
        uRamffffff80097fb820 = 0;
        uRamffffff80097fb828 = 0;
        iVar14 = strlen(lStack_18);
        for (lVar21 = 0; (int)lVar21 < iVar14 && (int)lVar21 < 0x1f; lVar21 = lVar21 + 1) {
          bVar9 = *(byte *)(lVar15 + lVar21);
          bVar8 = bVar9 + 0x20;
          if ((*(byte *)((long)(int)(uint)bVar9 + -0x7ff745de68) & 1) == 0) {
            bVar8 = bVar9;
          }
          *(byte *)((long)&uRamffffff80097fb810 + lVar21) = bVar8;
        }
        if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
          printk(0xffffff8008ec82d3,&uRamffffff80097fb810);
        }
        if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
          printk(0xffffff8008ec8327);
        }
        if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
          printk(0xffffff8008ec8351);
        }
        unaff_x26 = 0xffffff8008c39000;
        uVar22 = sysfs_create_group(lRamffffff80097fb888 + 0x20,0xffffff8008c39668);
        uVar25 = uVar22 & 0xffffffff;
        if ((int)uVar22 == 0) {
          if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
            printk(0xffffff8008ec839f);
          }
          unaff_x27 = -0x7ff7138000;
          lVar15 = proc_mkdir(0xffffff8008ec83c8,0);
          if (lVar15 != 0) {
            proc_create_data(0xffffff8008ec83d4,0x1a4,0,0xffffff8008c38ed0,0);
            proc_create_data(0xffffff8008ec83f4,0x1a4,0,0xffffff8008c38fa8,0);
          }
          if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
            printk(0xffffff8008ec8414);
          }
          uVar22 = sysfs_create_link(0,lRamffffff80097fb888 + 0x20,0xffffff8008ec83c8);
          uVar25 = uVar22 & 0xffffffff;
          if ((int)uVar22 == 0) {
            uVar22 = sysfs_create_link(0xffffff80094380c0,lRamffffff80097fb888 + 0x20,
                                       0xffffff8008ec8466);
            uVar25 = uVar22 & 0xffffffff;
            if ((int)uVar22 == 0) {
              if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
                printk(0xffffff8008ec84dd,uRamffffff80094ad898,uRamffffff80097fb7a0);
              }
              unaff_x25 = (int *)0xffffff8009634868;
              iVar14 = strlen(0xffffff8009634868);
              lVar15 = lRamffffff80097fb890;
              pbVar5 = (byte *)(lRamffffff80097fb890 + 0x3b0);
              strncpy(pbVar5,0xffffff8009634868,0x32);
              if (0x31 < iVar14) {
                iVar14 = 0x31;
              }
              *(undefined1 *)(lVar15 + iVar14 + 0x3b0) = 0;
              pbVar19 = pbVar5;
              do {
                bVar8 = *pbVar19;
                if (bVar8 == 0x5f) break;
                pbVar3 = pbVar19 + 1;
                bVar9 = bVar8 + 0x20;
                if ((*(byte *)((long)(int)(uint)bVar8 + -0x7ff745de68) & 1) == 0) {
                  bVar9 = bVar8;
                }
                pbVar19[0x32] = bVar9;
                pbVar19 = pbVar3;
              } while (pbVar3 != (byte *)(lVar15 + 0x3c3));
              lVar21 = strstr(pbVar5,0xffffff8008ec8520);
              *(bool *)(lVar15 + 0x3f6) = lVar21 != 0;
              lVar21 = strstr(pbVar5,0xffffff8008ecf64f);
              if (lVar21 == 0) {
                uVar18 = 0xffffff8008ec8530;
              }
              else {
                uVar18 = 0xffffff8008ec8526;
              }
              uVar18 = strcpy(lVar15 + 0x3f7,uVar18);
              if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
                printk(0xffffff8008ec853e,pbVar5,lVar15 + 0x3e2,*(undefined1 *)(lVar15 + 0x3f6),
                       uVar18);
              }
              lVar15 = lRamffffff80097fb890;
              if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
                printk(0xffffff8008ec85a1);
              }
              __mutex_init(0xffffff80097fb7a8,0xffffff8008ec85c3);
              if (((*(char *)(lRamffffff80097fb890 + 1) == '\0') &&
                  (pcVar20 = *(code **)(*(long *)(lVar15 + 0xf0) + 0x10), pcVar20 != (code *)0x0))
                 && (iVar14 = (*pcVar20)(), iVar14 != 0)) {
                if ((uRamffffff8008b461e8 & 1) != 0) {
                  printk(0xffffff8008ec85e2);
                }
                if ((uRamffffff8008b461e8 & 1) != 0) {
                  printk(0xffffff8008ec8665,iVar14);
                }
              }
              else {
                lVar21 = lVar15 + 0x1c;
                if (((lVar21 == 0) || (uVar25 = strlen(lVar21), 0xf < uVar25)) ||
                   ((lVar15 = lVar15 + 0x3c, lVar15 == 0 || (uVar25 = strlen(lVar15), 0xf < uVar25))
                   )) {
                  if ((uRamffffff8008b461e8 & 1) != 0) {
                    uVar18 = 0xffffff8008ec8637;
                    goto LAB_ffffff80087efadc;
                  }
                }
                else {
                  lRamffffff80094ad7f0 = lVar21;
                  lRamffffff80094ad7f8 = lVar15;
                  iVar14 = dsm_update_client_vendor_info();
                  if ((iVar14 != 0) && ((uRamffffff8008b461e8 & 1) != 0)) {
                    uVar18 = 0xffffff8008ec8601;
LAB_ffffff80087efadc:
                    printk(uVar18);
                  }
                }
              }
              lVar15 = lRamffffff80097fb890;
              if (*(char *)(lRamffffff80097fb890 + 1) != '\0') {
                *(undefined1 *)(lRamffffff80097fb890 + 0x37d) = 0;
                uRamffffff80097fdd78 = 0;
                uRamffffff80097fdd84 = 0x14;
                uRamffffff80097fdd7c = 0;
                uRamffffff80097fdd80 = 0;
                uRamffffff80097fdd88 = 0;
                uRamffffff80097fdd8a = 0;
                uRamffffff80097fb7d0 = 0xfffffffe0;
                uRamffffff80097fb7d8 = &uRamffffff80097fb7d8;
                uRamffffff80097fb7e0 = &uRamffffff80097fb7d8;
                pcRamffffff80097fb7e8 = tp_init_work_fn;
              }
              if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
                printk(0xffffff8008ec869f);
              }
              *(undefined4 *)(lVar15 + 0x16c) = 0;
              *(long *)(lVar15 + 0x130) = lVar15 + 0x130;
              *(long *)(lVar15 + 0x138) = lVar15 + 0x130;
              uVar22 = ts_kit_register_algo_func(lVar15);
              uVar25 = uVar22 & 0xffffffff;
              if ((int)uVar22 != 0) {
                if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087efb8c;
                uVar18 = 0xffffff8008ec86c2;
                goto LAB_ffffff80087efb88;
              }
              puVar16 = (undefined8 *)input_allocate_device();
              if (puVar16 == (undefined8 *)0x0) {
                uVar25 = 0xfffffff4;
                if ((uRamffffff8008b461e8 & 1) != 0) {
                  printk(0xffffff8008ec86ed);
                }
              }
              else {
                *puVar16 = 0xffffff8008ec8723;
                if (*piRamffffff80097fb878 == 0) {
                  uVar13 = 0x18;
LAB_ffffff80087efc3c:
                  *(undefined2 *)(puVar16 + 3) = uVar13;
                }
                else if (*piRamffffff80097fb878 == 1) {
                  uVar13 = 0x1c;
                  goto LAB_ffffff80087efc3c;
                }
                puVar16[0x45] = lRamffffff80097fb888 + 0x10;
                puVar16[0x39] = ts_input_open;
                puVar16[0x3a] = ts_input_close;
                pcVar20 = *(code **)(*(long *)(lRamffffff80097fb890 + 0xf0) + 0x28);
                puRamffffff80098000e8 = puVar16;
                if (pcVar20 == (code *)0x0) {
LAB_ffffff80087efc7c:
                  unaff_x25 = (int *)&uRamffffff80097fb810;
                  puVar16[0x59] = &uRamffffff80097fb810;
                  lVar15 = -0x7ff74ba000;
                  uVar22 = input_register_device(puVar16);
                  uVar25 = uVar22 & 0xffffffff;
                  if ((int)uVar22 == 0) {
                    unaff_x28 = -0x7ff6800000;
                    pcRamffffff8009800130 = fb_notifier_callback;
                    uVar22 = fb_register_client(&pcRamffffff8009800130);
                    uVar25 = uVar22 & 0xffffffff;
                    if ((int)uVar22 != 0) {
                      if ((uRamffffff8008b461e8 & 1) != 0) {
                        printk(0xffffff8008ec8796,uVar25);
                      }
                      if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087efddc;
                      printk(0xffffff8008ec87ec,uVar25);
                      goto LAB_ffffff80087efddc;
                    }
                    if ((uRamffffff8008b461e8 >> 2 & 1) != 0) {
                      printk(0xffffff8008ec87c7);
                    }
                    unaff_x25 = (int *)0xffffff80097fb000;
                    tpd_gpio_as_int(1);
                    msleep(0x32);
                    uStack_10 = 0;
                    uStack_c = 0;
                    uVar22 = of_property_read_u32_array
                                       (lRamffffff80097fb868,0xffffff8008de83de,&uStack_10,2);
                    uVar6 = uStack_c;
                    uVar25 = uVar22 & 0xffffffff;
                    if ((int)uVar22 == 0) {
                      uVar18 = gpio_to_desc(uStack_10);
                      gpiod_set_debounce(uVar18,uVar6);
                      uVar18 = irq_of_parse_and_map(lRamffffff80097fb868,0);
                      uRamffffff80097fb840 = (undefined4)uVar18;
                      switch(*(undefined4 *)(lRamffffff80097fb890 + 0x178)) {
                      case 0:
                        goto code_r0xffffff80087effb8;
                      case 1:
                        uVar22 = 4;
                        goto LAB_ffffff80087efe58;
                      case 2:
                        uVar22 = 1;
                        goto LAB_ffffff80087efe58;
                      case 3:
                        uVar22 = 2;
                        goto LAB_ffffff80087efe58;
                      default:
                        if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087efed0;
                        printk(0xffffff8008ec8839);
                        goto LAB_ffffff80087efed0;
                      }
                    }
                    uVar22 = 0xffffffea;
                    if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087efe10;
                    uVar18 = 0xffffff8008ec8816;
                    goto LAB_ffffff80087efe0c;
                  }
                  if ((uRamffffff8008b461e8 & 1) != 0) {
                    printk(0xffffff8008ec8731,uVar25);
                  }
                }
                else {
                  uVar22 = (*pcVar20)(puVar16);
                  uVar25 = uVar22 & 0xffffffff;
                  if ((int)uVar22 == 0) goto LAB_ffffff80087efc7c;
                }
                input_free_device(puVar16);
              }
              if ((uRamffffff8008b461e8 & 1) == 0) goto LAB_ffffff80087efb8c;
              uVar18 = 0xffffff8008ec875e;
LAB_ffffff80087efb88:
              printk(uVar18,uVar25);
              goto LAB_ffffff80087efb8c;
            }
            if ((uRamffffff8008b461e8 & 1) != 0) {
              uVar18 = 0xffffff8008ec8473;
              goto LAB_ffffff80087ef920;
            }
          }
          else if ((uRamffffff8008b461e8 & 1) != 0) {
            uVar18 = 0xffffff8008ec8438;
LAB_ffffff80087ef920:
            printk(uVar18,0xffffff8008c39080,uVar25);
          }
          sysfs_remove_group(lRamffffff80097fb888 + 0x20,0xffffff8008c39668);
        }
        else if ((uRamffffff8008b461e8 & 1) != 0) {
          printk(0xffffff8008ec8379);
        }
        platform_device_del(lRamffffff80097fb888);
        platform_device_put(lRamffffff80097fb888);
        if ((uRamffffff8008b461e8 & 1) != 0) {
          printk(0xffffff8008ec84ae,uVar25);
        }
        goto LAB_ffffff80087ef818;
      }
      if ((uRamffffff8008b461e8 & 1) != 0) {
        uVar18 = 0xffffff8008ec82b3;
        goto LAB_ffffff80087ef6f4;
      }
    }
  }
  else if ((uRamffffff8008b461e8 & 1) != 0) {
    printk(0xffffff8008ec8268,iVar14);
  }
  if ((uRamffffff8008b461e8 & 1) != 0) {
    printk(0xffffff8008ec82f8,0xffffffea);
  }
  uVar25 = 0xffffffea;
LAB_ffffff80087ef818:
  do {
    *(undefined4 *)(lVar24 + 0x830) = 0;
    wakeup_source_remove(0xffffff80097fdcc0);
    wakeup_source_drop(0xffffff80097fdcc0);
    lVar21 = lVar24;
    lVar15 = -0x7ff6802340;
    lVar24 = lVar28;
LAB_ffffff80087eff38:
    if ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0) {
      printk(0xffffff8008ec893f,*(undefined4 *)(lVar21 + 0x830));
    }
    if ((int)uVar25 == 0) {
      lVar28 = lVar24 + 0x3f8;
      __memset(lVar24 + 0x5e0,0,0x1c0);
      __memset(lVar24 + 0x420,0,0x1c0);
      DataMemoryBarrier(2,3);
      unaff_x27 = -0x7ff6805000;
      uVar18 = sp_el0;
      sched_setscheduler(uVar18,1,0xffffff8008c39098);
      do {
        if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
          printk(0xffffff8008ec8a8e);
        }
        while( true ) {
          cVar12 = kthread_should_stop();
          if (cVar12 != '\0') {
            if ((*(uint *)(lVar23 + 0x1e8) & 1) != 0) {
              printk(0xffffff8008ec8b0f);
            }
            *(undefined4 *)(lVar21 + 0x830) = 0;
            disable_irq(*(undefined4 *)(lVar21 + 0x840));
            pcVar20 = *(code **)(*(long *)(*(long *)(lVar21 + 0x890) + 0xf0) + 0x58);
            if (pcVar20 != (code *)0x0) {
              (*pcVar20)();
            }
            free_irq(*(undefined4 *)(lVar21 + 0x840));
            iVar14 = fb_unregister_client(&pcRamffffff8009800130);
            if ((iVar14 != 0) && ((*(uint *)(lVar23 + 0x1e8) & 1) != 0)) {
              printk(0xffffff8008ec8901);
            }
            uVar25 = lVar21 + 0x810;
            input_unregister_device(*(undefined8 *)(lVar21 + 0x50e8));
            input_free_device(*(undefined8 *)(lVar21 + 0x50e8));
            sysfs_remove_link(0,0xffffff8008ec83c8);
            sysfs_remove_group(*(long *)(lVar21 + 0x888) + 0x20,0xffffff8008c39668);
            lVar15 = -0x7ff6802340;
            wakeup_source_remove(0xffffff80097fdcc0);
            wakeup_source_drop(0xffffff80097fdcc0);
            platform_device_unregister(*(undefined8 *)(lVar21 + 0x888));
            ts_destory_client();
            if ((*(long *)(lVar21 + 0x5160) != 0) && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
              printk(0xffffff8008ec8b2c);
            }
            __memset(lVar21 + 0x810,0,0x4968);
            unaff_x28 = unaff_x27;
            if ((*(uint *)(lVar23 + 0x1e8) & 1) == 0) goto LAB_ffffff80087eff74;
            uVar18 = 0xffffff8008ec8b62;
            goto LAB_ffffff80087eff70;
          }
          uVar18 = _raw_spin_lock_irqsave(&uRamffffff80097fb9b8);
          DataMemoryBarrier(2,2);
          if (*(int *)(lVar21 + 0x9b0) != 0) break;
          lVar15 = sp_el0;
          *(undefined8 *)(lVar15 + 0x28) = 1;
          DataMemoryBarrier(2,3);
          if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
            printk(0xffffff8008ec8adc);
          }
          _raw_spin_unlock_irqrestore(&uRamffffff80097fb9b8,uVar18);
          schedule();
        }
        lVar15 = sp_el0;
        *(undefined8 *)(lVar15 + 0x28) = 0;
        DataMemoryBarrier(2,3);
        if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
          printk(0xffffff8008ec8aaf);
        }
        _raw_spin_unlock_irqrestore(&uRamffffff80097fb9b8,uVar18);
        unaff_x25 = (int *)(lVar24 + 0x5e0);
        while( true ) {
          uVar18 = _raw_spin_lock_irqsave(&uRamffffff80097fb9b8);
          DataMemoryBarrier(2,2);
          iVar14 = *(int *)(lVar21 + 0x9b0);
          if (iVar14 == 0) break;
          iVar2 = *(int *)(lVar21 + 0x9ac) + 1;
          __memcpy(unaff_x25,lVar21 + (long)*(int *)(lVar21 + 0x9ac) * 0x1c0 + 0x9c0);
          iVar7 = *(int *)(lVar21 + 0x9b4);
          *(int *)(lVar21 + 0x9b0) = iVar14 + -1;
          iVar14 = 0;
          if (iVar7 != 0) {
            iVar14 = iVar2 / iVar7;
          }
          uVar10 = iVar2 - iVar14 * iVar7;
          unaff_x26 = (ulong)uVar10;
          *(uint *)(lVar21 + 0x9ac) = uVar10;
          DataMemoryBarrier(2,3);
          _raw_spin_unlock_irqrestore(&uRamffffff80097fb9b8,uVar18);
          if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
            printk(0xffffff8008ec8a61,*unaff_x25);
          }
          piVar29 = *(int **)(lVar24 + 0x5e8);
          if ((piVar29 == (int *)0x0) || (*piVar29 != 0)) {
            cVar12 = ts_cmd_need_process(unaff_x25);
            if (cVar12 == '\0') {
              if ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0) {
                printk(0xffffff8008ec8996,*unaff_x25);
              }
            }
            else {
              piVar11 = (int *)(lVar24 + 0x420);
              piVar26 = unaff_x25;
LAB_ffffff80087f0574:
              piVar27 = piVar26;
              piVar26 = piVar11;
              *piVar26 = 0xff;
              switch(*piVar27) {
              case 0:
                ts_proc_bottom_half(piVar27,piVar26);
                if (*(int *)(lVar21 + 0x830) == 0) break;
                uVar6 = *(undefined4 *)(lVar21 + 0x840);
                goto code_r0xffffff80087f0188;
              case 1:
                ts_algo_calibrate(piVar27,piVar26);
                break;
              case 2:
                ts_report_input(piVar27,piVar26);
                break;
              case 3:
                ts_power_control(*(undefined4 *)(lVar21 + 0x840),piVar27,piVar26);
                break;
              case 4:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_fw_update_boot(piVar27,piVar26);
                goto code_r0xffffff80087f0240;
              case 5:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_fw_update_sd(piVar27,piVar26);
                goto code_r0xffffff80087f0240;
              case 6:
                ts_oem_info_switch(piVar27,piVar26);
                break;
              case 7:
                ts_get_chip_info(piVar27,piVar26);
                break;
              case 8:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_read_rawdata(piVar27,piVar26);
                goto code_r0xffffff80087f0240;
              case 9:
                ts_calibrate(piVar27,piVar26);
                break;
              case 10:
                ts_calibrate_wakeup_gesture(piVar27,piVar26);
                break;
              case 0xb:
                ts_dsm_debug(piVar27,piVar26);
                break;
              case 0xc:
                ts_kit_charger_switch(piVar27,piVar26);
                break;
              case 0xd:
                ts_glove_switch(piVar27,piVar26);
                break;
              case 0xe:
                ts_hand_detect(piVar27,piVar26);
                break;
              case 0xf:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_stop_wd_timer(lVar21 + 0x810);
                ts_force_reset(piVar27,piVar26);
                goto code_r0xffffff80087f0388;
              case 0x10:
                unaff_x26 = lVar21 + 0x810;
                ts_stop_wd_timer(unaff_x26);
                ts_int_err_process(piVar27,piVar26);
                enable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_start_wd_timer(unaff_x26);
                break;
              case 0x11:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_stop_wd_timer(lVar21 + 0x810);
                ts_err_process(piVar27,piVar26);
code_r0xffffff80087f0388:
                ts_start_wd_timer(lVar21 + 0x810);
                goto code_r0xffffff80087f0240;
              case 0x12:
                ts_check_status(piVar27,piVar26);
                break;
              case 0x13:
                ts_test_cmd(piVar27,piVar26);
                break;
              case 0x14:
                ts_holster_switch(piVar27,piVar26);
                break;
              case 0x15:
                ts_wakeup_gesture_enable_switch(piVar27,piVar26);
                break;
              case 0x16:
                ts_roi_switch(piVar27,piVar26);
                break;
              case 0x17:
                ts_touch_window(piVar27,piVar26);
                break;
              case 0x18:
                ts_palm_switch(piVar27,piVar26);
                break;
              case 0x19:
                ts_chip_regs_operate(piVar27,piVar26);
                break;
              case 0x1a:
                ts_set_info_flag(piVar27,piVar26);
                break;
              case 0x1c:
                ts_get_capacitance_test_type(piVar27,piVar26);
                break;
              case 0x1d:
                ts_special_hardware_test_switch(piVar27,piVar26);
                break;
              case 0x1e:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_read_debug_data(piVar27,piVar26);
                goto code_r0xffffff80087f0240;
              case 0x1f:
                disable_irq(*(undefined4 *)(lVar21 + 0x840));
                ts_read_calibration_data(piVar27,piVar26);
code_r0xffffff80087f0240:
                unaff_x26 = lVar21 + 0x810;
                uVar6 = *(undefined4 *)(lVar21 + 0x840);
code_r0xffffff80087f0188:
                enable_irq(uVar6);
                break;
              case 0x20:
                ts_get_calibration_info(piVar27,piVar26);
                break;
              case 0x21:
                ts_get_brightness_info_cmd();
                break;
              case 0x22:
                queue_work_on(10,uRamffffff80093cff88,lVar24 + 2000);
                break;
              case 0x23:
                ts_touch_switch_cmd();
              }
              if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)
                 ) {
                printk(0xffffff8008ec89bd,*piVar27,0);
              }
              if (*piVar26 != 0xff) {
                piVar11 = piVar27;
                if ((*(char *)(lVar24 + 0x3fc) != '\0') &&
                   ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
                  printk(0xffffff8008ec89ea);
                }
                goto LAB_ffffff80087f0574;
              }
              if (piVar29 != (int *)0x0) {
                DataMemoryBarrier(2,3);
                if ((*(char *)(lVar24 + 0x3fc) != '\0') &&
                   ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
                  printk(0xffffff8008ec8a1a);
                }
                if (*piVar29 == 0) goto LAB_ffffff80087f00d4;
                complete(piVar29 + 2);
              }
            }
          }
          else {
LAB_ffffff80087f00d4:
            kfree(piVar29);
          }
          __memset(unaff_x25,0,0x1c0);
          __memset(lVar24 + 0x420,0,0x1c0);
        }
        if ((*(char *)(lVar24 + 0x3fc) != '\0') && ((*(uint *)(lVar23 + 0x1e8) >> 2 & 1) != 0)) {
          printk(0xffffff8008ec8a44);
        }
        _raw_spin_unlock_irqrestore(&uRamffffff80097fb9b8,uVar18);
      } while( true );
    }
    lVar28 = lVar24;
    if ((*(uint *)(lVar23 + 0x1e8) & 1) != 0) {
      uVar18 = 0xffffff8008ec8978;
LAB_ffffff80087eff70:
      printk(uVar18);
    }
LAB_ffffff80087eff74:
    lVar23 = lVar23 + 0x1e0;
    lVar24 = lVar21 + 0x810;
    platform_device_unregister(*(undefined8 *)(lVar21 + 0x888));
    ts_destory_client();
    gpio_free(*(undefined4 *)(lVar21 + 0x84c));
    gpio_free(*(undefined4 *)(lVar21 + 0x848));
    *(undefined4 *)(lVar21 + 0x830) = 0;
    if (lStack_8 == lRamffffff80093cff08) {
      return;
    }
    uVar18 = __stack_chk_fail(0);
code_r0xffffff80087effb8:
    uVar22 = 8;
LAB_ffffff80087efe58:
    unaff_x25[0x20c] = 2;
    uVar17 = request_threaded_irq
                       (uVar18,ts_irq_handler,0,uVar22 | 0x4000,0xffffff8008ec90ca,unaff_x25 + 0x204
                       );
    uVar22 = uVar17 & 0xffffffff;
    if ((int)uVar17 == 0) {
      if ((*(uint *)(lVar15 + 0x1e8) >> 2 & 1) != 0) {
        printk(0xffffff8008ec8881,unaff_x25[0x210]);
      }
LAB_ffffff80087efed0:
      piVar29 = unaff_x25 + 0x204;
      ts_get_brightness_info();
      uVar18 = __msecs_to_jiffies(*(undefined4 *)(*(long *)(unaff_x25 + 0x224) + 0x180));
      queue_delayed_work_on(10,uRamffffff80093cff88,0xffffff80094ad810,uVar18);
      ts_send_init_cmd();
      if ((**(char **)(unaff_x25 + 0x224) == '\0') && ((*(char **)(unaff_x25 + 0x224))[8] != '\0'))
      {
        ts_kit_status_check_init_part_6();
      }
      lVar4 = lVar15 + 0x1e0;
      puVar1 = (uint *)(lVar15 + 0x1e8);
      lVar21 = lVar24;
      lVar15 = lVar4;
      lVar24 = lVar28;
      unaff_x25 = piVar29;
      if ((*puVar1 >> 2 & 1) != 0) {
        printk(0xffffff8008ec88dc);
      }
      goto LAB_ffffff80087eff38;
    }
    if ((*(uint *)(lVar15 + 0x1e8) & 1) != 0) {
      uVar18 = 0xffffff8008ec885d;
LAB_ffffff80087efe0c:
      printk(uVar18);
    }
LAB_ffffff80087efe10:
    if ((*(uint *)(lVar15 + 0x1e8) & 1) != 0) {
      printk(0xffffff8008ec88b1,uVar22);
    }
    iVar14 = fb_unregister_client(unaff_x28 + 0x130);
    uVar25 = uVar22;
    if ((iVar14 != 0) && ((*(uint *)(lVar15 + 0x1e8) & 1) != 0)) {
      printk(0xffffff8008ec8901);
    }
LAB_ffffff80087efddc:
    input_unregister_device(0);
    input_free_device(0);
LAB_ffffff80087efb8c:
    sysfs_remove_link(0,unaff_x27 + 0x3c8);
    sysfs_remove_group(*(long *)(lVar24 + 0x888) + 0x20,unaff_x26 + 0x668);
    platform_device_del(*(undefined8 *)(lVar24 + 0x888));
    platform_device_put(*(undefined8 *)(lVar24 + 0x888));
  } while( true );
}


//// goodix_ts_module_init @ffffff8008fe9bec

int goodix_ts_module_init(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;

  printk(0xffffff8008ed1de7,0xffffff8008c3aaa8,0x99c,0xffffff8008c3aaa8);
  lRamffffff8009800678 = kmem_cache_alloc_trace(uRamffffff8009597958,0x24080c0,0x4d0);
  if (lRamffffff8009800678 == 0) {
    uVar5 = 0xffffff8008ed1e04;
    uVar6 = 0x9a1;
  }
  else {
    lRamffffff8009800680 = kmem_cache_alloc_trace(uRamffffff8009597958,0x24080c0,0x418);
    if (lRamffffff8009800680 != 0) {
      lVar3 = of_find_compatible_node(0,0,0xffffff8008ec8723);
      if (lVar3 == 0) {
        if ((uRamffffff8008b46278 & 1) != 0) {
          printk(0xffffff8008ecbafe,0xffffff8008c3aaa8);
        }
      }
      else {
        for (lVar4 = of_get_next_child(lVar3,0); lVar4 != 0; lVar4 = of_get_next_child(lVar3,lVar4))
        {
          iVar2 = of_device_is_compatible(lVar4,0xffffff8008ed1e75);
          lVar1 = lRamffffff8009800680;
          if (iVar2 != 0) {
            *(long *)(lRamffffff8009800680 + 0xe8) = lVar4;
            *(undefined8 *)(lVar1 + 0xf0) = 0xffffff80094ae0e8;
            iVar2 = huawei_ts_chip_register();
            if (iVar2 != 0) {
              if ((uRamffffff8008b46278 & 1) != 0) {
                printk(0xffffff8008ecbb63,0xffffff8008c3aaa8,iVar2);
              }
              goto LAB_ffffff8008fe9c68;
            }
            uRamffffff8009800179 = 1;
            lRamffffff80097fb808 = lRamffffff8009800680;
            if ((uRamffffff8008b46278 >> 2 & 1) == 0) {
              uRamffffff8009800179 = 1;
              return 0;
            }
            uVar5 = 0xffffff8008ecbb8f;
            goto LAB_ffffff8008fe9e0c;
          }
        }
        if ((uRamffffff8008b46278 & 1) != 0) {
          printk(0xffffff8008ecbb2e,0xffffff8008c3aaa8,0xffffff8008ed1e75);
        }
      }
      iVar2 = -0x16;
      goto LAB_ffffff8008fe9c68;
    }
    uVar6 = 0x9a9;
    uVar5 = 0xffffff8008ed1e39;
  }
  iVar2 = -0xc;
  printk(uVar5,0xffffff8008c3aaa8,uVar6,0xffffff8008c3aaa8);
LAB_ffffff8008fe9c68:
  if (lRamffffff8009800678 != 0) {
    kfree();
  }
  lRamffffff8009800678 = 0;
  kfree(lRamffffff8009800680);
  lRamffffff8009800680 = 0;
  if ((uRamffffff8008b46278 >> 2 & 1) != 0) {
    uVar5 = 0xffffff8008ecbba8;
LAB_ffffff8008fe9e0c:
    printk(uVar5,0xffffff8008c3aaa8);
  }
  return iVar2;
}


//// mstar_ts_module_init @ffffff8008fe9e28

int mstar_ts_module_init(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;

  if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
    printk(0xffffff8008ec9d57,0xffffff8008c3b788);
  }
  lRamffffff80098008b0 = kmem_cache_alloc_trace(uRamffffff8009597958,0x24080c0,0x418);
  if (lRamffffff80098008b0 == 0) {
    iVar2 = -0xc;
    if ((uRamffffff8008b462c8 & 1) != 0) {
      printk(0xffffff8008ecbace,0xffffff8008c3b788);
    }
  }
  else {
    lVar3 = of_find_compatible_node(0,0,0xffffff8008ec8723);
    if (lVar3 == 0) {
      if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008ecbafe,0xffffff8008c3b788);
      }
    }
    else {
      for (lVar4 = of_get_next_child(lVar3,0); lVar4 != 0; lVar4 = of_get_next_child(lVar3,lVar4)) {
        iVar2 = of_device_is_compatible(lVar4,0xffffff8008ed8ca5);
        lVar1 = lRamffffff80098008b0;
        if (iVar2 != 0) {
          *(long *)(lRamffffff80098008b0 + 0xe8) = lVar4;
          *(undefined8 *)(lVar1 + 0xf0) = 0xffffff80094ae320;
          iVar2 = huawei_ts_chip_register();
          if (iVar2 != 0) {
            if ((uRamffffff8008b462c8 & 1) != 0) {
              printk(0xffffff8008ecbb63,0xffffff8008c3b788,iVar2);
            }
            goto LAB_ffffff8008fe9eac;
          }
          lRamffffff80097fb808 = lRamffffff80098008b0;
          if ((uRamffffff8008b462c8 >> 2 & 1) == 0) {
            return 0;
          }
          uVar5 = 0xffffff8008ecbb8f;
          goto LAB_ffffff8008fe9ffc;
        }
      }
      if ((uRamffffff8008b462c8 & 1) != 0) {
        printk(0xffffff8008ecbb2e,0xffffff8008c3b788,0xffffff8008ed8ca5);
      }
    }
    iVar2 = -0x16;
  }
LAB_ffffff8008fe9eac:
  kfree(lRamffffff80098008b0);
  lRamffffff80098008b0 = 0;
  if ((uRamffffff8008b462c8 >> 2 & 1) != 0) {
    uVar5 = 0xffffff8008ecbba8;
LAB_ffffff8008fe9ffc:
    printk(uVar5,0xffffff8008c3b788);
  }
  return iVar2;
}


//// focal_chip_detect @ffffff80087fdf80

void focal_chip_detect(ulong param_1)

{
  undefined1 *puVar1;
  int iVar2;
  ulong uVar3;
  undefined *unaff_x19;
  undefined *puVar4;
  ulong uVar5;
  long unaff_x21;
  long *plVar6;
  undefined1 uStack_a;
  undefined1 uStack_9;
  long lStack_8;

  plVar6 = (long *)0xffffff80093cf000;
  uVar5 = 0xfffffffb;
  lStack_8 = lRamffffff80093cff08;
  iVar2 = ontim_dev_debug_file_exist(0xffffff80094adfc8,0);
  puVar1 = puRamffffff80098005a8;
  if (iVar2 == 0) goto LAB_ffffff80087fe0dc;
  unaff_x21 = -0x7ff6800000;
  puVar4 = (undefined *)0xffffff8008b46000;
  *(ulong *)(puRamffffff80098005a8 + 0x140) = param_1;
  if ((param_1 == 0) && (lRam0000000000000078 == 0)) {
    uVar5 = 0xfffffff4;
    if ((uRamffffff8008b46228 & 1) != 0) {
      printk(0xffffff8008ecd6a8,0xffffff8008c39e78);
    }
  }
  else {
    puVar1[2] = 0;
    puRamffffff80098005a8[3] = 0;
    *puRamffffff80098005a8 = 0;
    uVar3 = focal_prase_ic_config_dts(*(undefined8 *)(puRamffffff80098005a8 + 0xe8));
    uVar5 = uVar3 & 0xffffffff;
    if ((int)uVar3 == 0) {
      tpd_gpio_output(uVar3,0);
      tpd_gpio_output(1,0);
      focal_power_init();
      msleep(100);
      uVar3 = focal_hardware_reset(2);
      uVar5 = uVar3 & 0xffffffff;
      if ((int)uVar3 == 0) goto LAB_ffffff80087fe0f8;
      unaff_x19 = &_hwtag_TS_KIT;
      if ((uRamffffff8008b46228 & 1) == 0) goto LAB_ffffff80087fe0dc;
      printk(0xffffff8008ecd727,0xffffff8008c39e78,uVar5);
      goto LAB_ffffff80087fe0dc;
    }
    if ((uRamffffff8008b46228 & 1) != 0) {
      printk(0xffffff8008ecd6f5,0xffffff8008c39e78,uVar5);
    }
  }
  do {
    unaff_x19 = puVar4 + 0x220;
    if ((*(uint *)(puVar4 + 0x228) >> 2 & 1) != 0) {
      printk(0xffffff8008ecd836,0xffffff8008c39e78);
    }
LAB_ffffff80087fe0dc:
    while( true ) {
      plVar6 = plVar6 + 0x1e1;
      if (lStack_8 == *plVar6) {
        return;
      }
      __stack_chk_fail(uVar5);
      puVar4 = unaff_x19;
LAB_ffffff80087fe0f8:
      uStack_9 = 0;
      *(short *)(*(long *)(param_1 + 0x60) + 2) =
           (short)*(undefined4 *)(*(long *)(unaff_x21 + 0x5a8) + 0x174);
      uStack_a = 0xa3;
      uVar3 = focal_read(&uStack_a,1,&uStack_9);
      param_1 = uVar3 & 0xffffffff;
      if ((int)uVar3 < 0) break;
      if ((*(uint *)(puVar4 + 0x228) >> 2 & 1) != 0) {
        printk(0xffffff8008ecd785,0xffffff8008c39e90,uStack_9);
      }
      if ((*(uint *)(puVar4 + 0x228) >> 2 & 1) != 0) {
        printk(0xffffff8008ecd7e8,0xffffff8008c39e78);
      }
      unaff_x19 = puVar4 + 0x220;
      strncpy(*(long *)(unaff_x21 + 0x5a8) + 0x1c,0xffffff8008ecbb2a,0x20);
      uRamffffff8009800179 = 1;
      if ((*(uint *)(puVar4 + 0x228) >> 2 & 1) != 0) {
        printk(0xffffff8008ecd80b,0xffffff8008c39e78);
      }
      regester_and_init_ontim_debug(0xffffff80094adfc8);
    }
    if ((*(uint *)(puVar4 + 0x228) & 1) != 0) {
      printk(0xffffff8008ecd754,0xffffff8008c39e90,uVar3 & 0xffffffff,0);
    }
    msleep(0x32);
    if ((*(uint *)(puVar4 + 0x228) & 1) != 0) {
      printk(0xffffff8008ecd7b9,0xffffff8008c39e78,uVar3 & 0xffffffff);
    }
    uVar5 = uVar3 & 0xffffffff;
    focal_regulator_power_switch(0);
  } while( true );
}


//// ts_i2c_read @ffffff80087ec898

void ts_i2c_read(undefined8 param_1,short param_2,long param_3,undefined2 param_4,uint param_5)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long *plVar4;
  undefined2 *puVar5;
  int iVar6;
  undefined8 uVar7;
  undefined2 uStack_28;
  undefined2 uStack_26;
  short sStack_24;
  undefined8 uStack_20;
  undefined2 uStack_18;
  undefined2 uStack_16;
  undefined2 uStack_14;
  long lStack_10;
  long lStack_8;

  plVar4 = (long *)0xffffff80093cf000;
  iVar1 = -1;
  lStack_8 = lRamffffff80093cff08;
  lVar2 = -0x7ff6805000;
  uStack_28 = *(undefined2 *)(lRamffffff80097fb870 + 2);
  uStack_26 = 0;
  uStack_18 = *(undefined2 *)(lRamffffff80097fb870 + 2);
  uStack_16 = 1;
  sStack_24 = param_2;
  uStack_20 = param_1;
  uStack_14 = param_4;
  lStack_10 = param_3;
  if (param_3 == 0) goto LAB_ffffff80087ec940;
  if (param_2 != 0) goto LAB_ffffff80087ec95c;
  puVar5 = &uStack_18;
  uVar7 = 1;
  lVar3 = lVar2;
  do {
    iVar1 = 0;
    lVar2 = lVar3;
    for (iVar6 = 0; iVar6 < (int)(param_5 & 0xff); iVar6 = iVar6 + 1) {
      iVar1 = i2c_transfer(*(undefined8 *)(*(long *)(lVar3 + 0x870) + 0x18),puVar5,uVar7);
      if (-1 < iVar1) {
        iVar1 = 0;
        break;
      }
      if ((uRamffffff8008b461e8 & 1) != 0) {
        printk(0xffffff8008ec6670,iVar1);
      }
    }
LAB_ffffff80087ec940:
    plVar4 = plVar4 + 0x1e1;
    if (lStack_8 == *plVar4) {
      return;
    }
    __stack_chk_fail(iVar1);
LAB_ffffff80087ec95c:
    puVar5 = &uStack_28;
    uVar7 = 2;
    lVar3 = lVar2;
  } while( true );
}
