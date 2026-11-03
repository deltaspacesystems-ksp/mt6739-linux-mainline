//// mtk_pinctrl_get_gpio_value @ffffff80083b6684

void mtk_pinctrl_get_gpio_value(long param_1,undefined8 param_2,long param_3,int *param_4)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  undefined8 extraout_x1;
  int *piVar4;
  ulong uVar5;
  uint uStack_c;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  uVar5 = 0;
  do {
    piVar4 = param_4;
    if ((int)uVar5 < (int)param_3) goto LAB_ffffff80083b66d4;
    uVar3 = 0xffffffff;
    while( true ) {
      if (lStack_8 == lRamffffff80093cff08) {
        return;
      }
      param_3 = lStack_8;
      __stack_chk_fail(uVar3);
      param_2 = extraout_x1;
      piVar4 = param_4;
LAB_ffffff80083b66d4:
      param_4 = piVar4 + 3;
      if (*piVar4 != (int)param_2) break;
      uVar3 = 0xffffffff;
      if (piVar4 != (int *)0x0) {
        param_4 = (int *)((ulong)*(byte *)((long)piVar4 + 10) + 2);
        bVar1 = *(byte *)((long)piVar4 + 9);
        bVar2 = *(byte *)(piVar4 + 2);
        regmap_read(*(undefined8 *)(param_1 + (long)param_4 * 8),piVar4[1],&uStack_c);
        uVar3 = uStack_c >> (ulong)(bVar2 & 0x1f) &
                ((uint)(-1L << ((ulong)bVar1 & 0x3f)) ^ 0xffffffff);
      }
    }
    uVar5 = (ulong)((int)uVar5 + 1);
  } while( true );
}


//// mtk_gpio_get_direction @ffffff80083b67f4

void mtk_gpio_get_direction(long param_1,ulong param_2)

{
  int iVar1;
  undefined8 *puVar2;
  ulong uVar3;
  long lVar4;
  long *plVar5;
  undefined1 auVar6 [16];
  uint uStack_c;
  long lStack_8;

  plVar5 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  uStack_c = 0;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 8) + 0xa0);
  lVar4 = puVar2[0x1d];
  if (*(long *)(lVar4 + 0xc0) == 0) goto LAB_ffffff80083b6854;
  iVar1 = mtk_pinctrl_get_gpio_value(puVar2,param_2,*(undefined4 *)(lVar4 + 200));
  uVar3 = (ulong)(iVar1 == 0);
  while( true ) {
    plVar5 = plVar5 + 0x1e1;
    if (lStack_8 == *plVar5) break;
    lVar4 = lStack_8;
    auVar6 = __stack_chk_fail(uVar3);
    param_2 = auVar6._8_8_;
    puVar2 = auVar6._0_8_;
LAB_ffffff80083b6854:
    if (*(code **)(lVar4 + 0x168) == (code *)0x0) {
      regmap_read(*puVar2,*(int *)(lVar4 + 0x1e8) +
                          (int)(((ulong)*(byte *)(lVar4 + 0x211) & (param_2 & 0xffffffff) >> 4) <<
                               ((ulong)*(byte *)(lVar4 + 0x210) & 0x3f)),&uStack_c);
      uVar3 = (ulong)((uStack_c & (uint)(1L << ((uint)param_2 & 0xf))) == 0);
    }
    else {
      uVar3 = (**(code **)(lVar4 + 0x168))((uint)param_2 | 0x80000000);
    }
  }
  return;
}


//// mtk_gpio_get_out.isra.12 @ffffff80083b68b8

void mtk_gpio_get_out_isra_12(long param_1,ulong param_2)

{
  ulong uVar1;
  undefined8 uVar2;
  ulong extraout_x1;
  ulong uVar3;
  long *plVar4;
  uint uStack_c;
  long lStack_8;

  plVar4 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  uStack_c = 0;
  uVar3 = *(ulong *)(*(long *)(param_1 + 0xa0) + 0xe8);
  if (*(long *)(uVar3 + 0xa0) == 0) goto LAB_ffffff80083b690c;
  uVar1 = mtk_pinctrl_get_gpio_value
                    (*(long *)(param_1 + 0xa0),param_2,*(undefined4 *)(uVar3 + 0xa8));
  while( true ) {
    plVar4 = plVar4 + 0x1e1;
    if (lStack_8 == *plVar4) break;
    __stack_chk_fail(uVar1);
    param_2 = extraout_x1;
LAB_ffffff80083b690c:
    uVar3 = (ulong)(uint)(*(int *)(uVar3 + 0x200) +
                         (int)(((ulong)*(byte *)(uVar3 + 0x211) & (param_2 & 0xffffffff) >> 4) <<
                              ((ulong)*(byte *)(uVar3 + 0x210) & 0x3f)));
    uVar2 = mtk_get_regmap();
    regmap_read(uVar2,uVar3 & 0xffffffff,&uStack_c);
    uVar1 = (ulong)((uStack_c & (uint)(1L << ((uint)param_2 & 0xf))) != 0);
  }
  return;
}


//// mtk_gpio_get_in.isra.13 @ffffff80083b72b4

void mtk_gpio_get_in_isra_13(long param_1,ulong param_2)

{
  ulong uVar1;
  undefined8 uVar2;
  ulong extraout_x1;
  ulong uVar3;
  long *plVar4;
  uint uStack_c;
  long lStack_8;

  plVar4 = (long *)0xffffff80093cf000;
  lStack_8 = lRamffffff80093cff08;
  uStack_c = 0;
  uVar3 = *(ulong *)(*(long *)(param_1 + 0xa0) + 0xe8);
  if (*(long *)(uVar3 + 0xb0) == 0) goto LAB_ffffff80083b7308;
  uVar1 = mtk_pinctrl_get_gpio_value
                    (*(long *)(param_1 + 0xa0),param_2,*(undefined4 *)(uVar3 + 0xb8));
  while( true ) {
    plVar4 = plVar4 + 0x1e1;
    if (lStack_8 == *plVar4) break;
    __stack_chk_fail(uVar1);
    param_2 = extraout_x1;
LAB_ffffff80083b7308:
    uVar3 = (ulong)(uint)(*(int *)(uVar3 + 0x204) +
                         (int)(((ulong)*(byte *)(uVar3 + 0x211) & (param_2 & 0xffffffff) >> 4) <<
                              ((ulong)*(byte *)(uVar3 + 0x210) & 0x3f)));
    uVar2 = mtk_get_regmap();
    regmap_read(uVar2,uVar3 & 0xffffffff,&uStack_c);
    uVar1 = (ulong)((uStack_c & (uint)(1L << ((uint)param_2 & 0xf))) != 0);
  }
  return;
}


//// mtk_pinctrl_get_gpio_mode_for_eint @ffffff80083b87b4

void mtk_pinctrl_get_gpio_mode_for_eint(undefined4 param_1)

{
  mtk_pinctrl_get_gpio_value
            (lRamffffff80095bf4d8,param_1,
             *(undefined4 *)(*(long *)(lRamffffff80095bf4d8 + 0xe8) + 0x38),
             *(undefined8 *)(*(long *)(lRamffffff80095bf4d8 + 0xe8) + 0x30));
  return;
}


//// mtk_pinctrl_probe @ffffff80083b843c

void mtk_pinctrl_probe(undefined8 param_1)

{
  printk(0xffffff8008de8972);
  mtk_pctrl_init(param_1,0xffffff8008bac100,0);
  return;
}


//// mtk_pctrl_init @ffffff80083b7c24

ulong mtk_pctrl_init(long param_1,ulong param_2,ulong param_3)

{
  undefined4 uVar1;
  ulong *puVar2;
  ulong *puVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  undefined4 *puVar7;
  ulong uVar8;
  ulong uVar9;
  long lVar10;
  undefined8 *puVar11;
  long *plVar12;
  long lVar13;
  undefined8 *puVar14;
  ulong uVar15;
  long lVar16;
  long lVar17;
  uint uVar18;

  uVar8 = param_1 + 0x10;
  lVar17 = *(long *)(param_1 + 0x238);
  puRamffffff80095bf4d8 = (ulong *)devm_kmalloc(uVar8,0x128,0x24080c0);
  if (puRamffffff80095bf4d8 == (ulong *)0x0) {
LAB_ffffff80083b7c70:
    uVar15 = 0xfffffff4;
    goto LAB_ffffff80083b7c74;
  }
  *(ulong **)(param_1 + 0xb0) = puRamffffff80095bf4d8;
  lVar5 = of_find_property(lVar17,0xffffff8008de8723,0);
  if (lVar5 == 0) {
    uVar6 = 0xffffff8008de8735;
LAB_ffffff80083b7cb4:
    dev_err(uVar8,uVar6);
  }
  else {
    if (*(int *)(param_2 + 0x214) == 0) {
      lVar5 = of_parse_phandle(lVar17,0xffffff8008de875c,0);
      puVar2 = puRamffffff80095bf4d8;
      if (lVar5 == 0) {
        if (param_3 == 0) {
          uVar6 = 0xffffff8008de8771;
          goto LAB_ffffff80083b7cb4;
        }
        *puRamffffff80095bf4d8 = param_3;
      }
      else {
        uVar15 = syscon_node_to_regmap();
        *puVar2 = uVar15;
        if (0xfffffffffffff000 < *puRamffffff80095bf4d8) {
          uVar15 = *puRamffffff80095bf4d8 & 0xffffffff;
          goto LAB_ffffff80083b7c74;
        }
      }
      lVar5 = of_parse_phandle(lVar17,0xffffff8008de875c,1);
      puVar2 = puRamffffff80095bf4d8;
      if (lVar5 != 0) {
        uVar15 = syscon_node_to_regmap();
        puVar2[1] = uVar15;
        uVar15 = puRamffffff80095bf4d8[1];
        if (0xfffffffffffff000 < uVar15) goto LAB_ffffff80083b7c74;
      }
    }
    else {
      for (uVar18 = 0; uVar18 < *(uint *)(param_2 + 0x214); uVar18 = uVar18 + 1) {
        lVar5 = of_parse_phandle(lVar17,0xffffff8008de875c,uVar18);
        puVar2 = puRamffffff80095bf4d8;
        if (lVar5 != 0) {
          uVar15 = syscon_node_to_regmap();
          puVar2[(long)(int)uVar18 + 2] = uVar15;
          uVar15 = puRamffffff80095bf4d8[(long)(int)uVar18 + 2];
          if (0xfffffffffffff000 < uVar15) goto LAB_ffffff80083b7c74;
        }
      }
    }
    puRamffffff80095bf4d8[0x1d] = param_2;
    lVar16 = *(long *)(param_1 + 0xb0);
    uVar18 = *(uint *)(*(long *)(lVar16 + 0xe8) + 8);
    *(uint *)(lVar16 + 0xd0) = uVar18;
    lVar5 = devm_kmalloc(uVar8,(ulong)uVar18 * 0x18,0x24080c0);
    *(long *)(lVar16 + 0xc0) = lVar5;
    if (lVar5 != 0) {
      lVar5 = devm_kmalloc(uVar8,(ulong)*(uint *)(lVar16 + 0xd0) << 3,0x24080c0);
      *(long *)(lVar16 + 0xd8) = lVar5;
      if (lVar5 != 0) {
        for (uVar18 = 0; uVar18 < *(uint *)(*(long **)(lVar16 + 0xe8) + 1); uVar18 = uVar18 + 1) {
          lVar10 = **(long **)(lVar16 + 0xe8);
          lVar13 = *(long *)(lVar16 + 0xc0);
          lVar5 = lVar10 + (long)(int)uVar18 * 0x28;
          *(undefined8 *)(lVar13 + (long)(int)uVar18 * 0x18) = *(undefined8 *)(lVar5 + 8);
          *(undefined4 *)(lVar13 + (long)(int)uVar18 * 0x18 + 0x10) =
               *(undefined4 *)(lVar10 + (long)(int)uVar18 * 0x28);
          *(undefined8 *)(*(long *)(lVar16 + 0xd8) + (long)(int)uVar18 * 8) =
               *(undefined8 *)(lVar5 + 8);
        }
        uVar15 = devm_kmalloc(uVar8,(ulong)*(uint *)(puRamffffff80095bf4d8[0x1d] + 8) * 0x18,
                              0x24080c0);
        if (uVar15 == 0) goto LAB_ffffff80083b7c70;
        for (uVar18 = 0; puVar2 = puRamffffff80095bf4d8,
            plVar12 = (long *)puRamffffff80095bf4d8[0x1d], uVar18 < *(uint *)(plVar12 + 1);
            uVar18 = uVar18 + 1) {
          puVar14 = (undefined8 *)(uVar15 + (long)(int)uVar18 * 0x18);
          puVar11 = (undefined8 *)(*plVar12 + (long)(int)uVar18 * 0x28);
          uVar6 = puVar11[1];
          *puVar14 = *puVar11;
          puVar14[1] = uVar6;
          puVar14[2] = puVar11[2];
        }
        uVar9 = *(ulong *)(param_1 + 0x60);
        if (uVar9 == 0) {
          uVar9 = *(ulong *)(param_1 + 0x20);
        }
        puRamffffff80095bf4d8[0xc] = uVar9;
        puVar2[0xd] = uVar15;
        puVar2[0x12] = 0;
        *(int *)(puVar2 + 0xe) = (int)plVar12[1];
        puVar2[0x11] = 0xffffff8008babeb0;
        puVar2[0x16] = uVar8;
        puVar2[0xf] = 0xffffff8008babef8;
        puVar2[0x10] = 0xffffff8008babf28;
        uVar15 = pinctrl_register(puVar2 + 0xc,uVar8,puVar2);
        puVar2[0x1c] = uVar15;
        puVar2 = puRamffffff80095bf4d8;
        if (0xfffffffffffff000 < puRamffffff80095bf4d8[0x1c]) {
          dev_err(uVar8,0xffffff8008de8798);
          uVar15 = (ulong)(uint)puRamffffff80095bf4d8[0x1c];
          goto LAB_ffffff80083b7c74;
        }
        uVar15 = devm_kmalloc(uVar8,0xd0,0x24080c0);
        puVar2[0x17] = uVar15;
        if (puRamffffff80095bf4d8[0x17] == 0) {
          uVar15 = 0xfffffff4;
LAB_ffffff80083b80f0:
          pinctrl_unregister(puRamffffff80095bf4d8[0x1c]);
          uVar6 = 0xffffff8008de8884;
        }
        else {
          __memcpy(puRamffffff80095bf4d8[0x17],0xffffff8008babf78,0xd0);
          puVar2 = puRamffffff80095bf4d8;
          *(short *)(puRamffffff80095bf4d8[0x17] + 0x8c) =
               (short)*(undefined4 *)(puRamffffff80095bf4d8[0x1d] + 8);
          lVar5 = *(long *)(param_1 + 0x60);
          if (lVar5 == 0) {
            lVar5 = *(long *)(param_1 + 0x20);
          }
          *(long *)puVar2[0x17] = lVar5;
          puVar2 = puRamffffff80095bf4d8;
          *(ulong *)(puRamffffff80095bf4d8[0x17] + 8) = uVar8;
          *(undefined4 *)(puVar2[0x17] + 0x88) = 0xffffffff;
          iVar4 = gpiochip_add(puVar2[0x17]);
          if (iVar4 != 0) {
            uVar15 = 0xffffffea;
            goto LAB_ffffff80083b80f0;
          }
          lVar5 = *(long *)(param_1 + 0x60);
          if (lVar5 == 0) {
            lVar5 = *(long *)(param_1 + 0x20);
          }
          uVar9 = gpiochip_add_pin_range
                            (puRamffffff80095bf4d8[0x17],lVar5,0,0,
                             *(undefined4 *)(puRamffffff80095bf4d8[0x1d] + 8));
          uVar15 = uVar9 & 0xffffffff;
          if ((int)uVar9 != 0) goto LAB_ffffff80083b8084;
          if ((uVar8 == 0) || (iVar4 = device_create_file(uVar8,0xffffff8009428230), iVar4 != 0)) {
            printk(0xffffff8008de87ba);
          }
          lVar5 = of_find_property(lVar17,0xffffff8008de87dc,0);
          if (lVar5 != 0) {
            lVar5 = platform_get_resource(param_1,0x200,0);
            puVar2 = puRamffffff80095bf4d8;
            if (lVar5 == 0) {
              uVar6 = 0xffffff8008de880f;
LAB_ffffff80083b807c:
              dev_err(uVar8,uVar6);
LAB_ffffff80083b8084:
              uVar15 = 0xffffffea;
            }
            else {
              uVar9 = devm_ioremap_resource(uVar8,lVar5);
              puVar3 = puRamffffff80095bf4d8;
              puVar2[0x1e] = uVar9;
              if (0xfffffffffffff000 < puVar3[0x1e]) goto LAB_ffffff80083b8084;
              lVar5 = (ulong)*(byte *)(puVar3[0x1d] + 0x269) << 2;
              uVar9 = devm_kmalloc(uVar8,lVar5,0x24080c0);
              puVar3[0x23] = uVar9;
              puVar2 = puRamffffff80095bf4d8;
              if (puRamffffff80095bf4d8[0x23] != 0) {
                uVar9 = devm_kmalloc(uVar8,lVar5,0x24080c0);
                puVar3 = puRamffffff80095bf4d8;
                puVar2[0x24] = uVar9;
                if (puVar3[0x24] != 0) {
                  uVar9 = devm_kmalloc(uVar8,(ulong)*(uint *)(puVar3[0x1d] + 0x270) << 2,0x24080c0);
                  puVar3[0x20] = uVar9;
                  puVar2 = puRamffffff80095bf4d8;
                  if (puRamffffff80095bf4d8[0x20] != 0) {
                    uVar9 = devm_kmalloc(uVar8,(ulong)*(uint *)(puRamffffff80095bf4d8[0x1d] + 0x270)
                                               * 0x30,0x24080c0);
                    puVar2[0x19] = uVar9;
                    puVar2 = puRamffffff80095bf4d8;
                    if (puRamffffff80095bf4d8[0x19] != 0) {
                      uVar9 = devm_kmalloc(uVar8,(ulong)*(uint *)(puRamffffff80095bf4d8[0x1d] +
                                                                 0x270) << 2,0x24080c0);
                      puVar2[0x21] = uVar9;
                      puVar2 = puRamffffff80095bf4d8;
                      if (puRamffffff80095bf4d8[0x21] != 0) {
                        uVar9 = devm_kmalloc(uVar8,(ulong)*(uint *)(puRamffffff80095bf4d8[0x1d] +
                                                                   0x270) << 2,0x24080c0);
                        puVar2[0x22] = uVar9;
                        if (puRamffffff80095bf4d8[0x22] != 0) {
                          iVar4 = irq_of_parse_and_map(lVar17,0);
                          puVar2 = puRamffffff80095bf4d8;
                          if (iVar4 == 0) {
                            uVar6 = 0xffffff8008de882f;
                            goto LAB_ffffff80083b807c;
                          }
                          lVar16 = *(long *)(puRamffffff80095bf4d8[0x1d] + 0x278);
                          uVar1 = *(undefined4 *)(puRamffffff80095bf4d8[0x1d] + 0x270);
                          lVar5 = lVar17 + 0x20;
                          if (lVar16 == 0) {
                            lVar16 = -0x7ff74abd60;
                          }
                          if (lVar17 == 0) {
                            lVar5 = 0;
                          }
                          uVar9 = __irq_domain_add(lVar5,uVar1,uVar1,0,lVar16,0);
                          puVar3 = puRamffffff80095bf4d8;
                          puVar2[0x1f] = uVar9;
                          if (puVar3[0x1f] != 0) {
                            puVar7 = (undefined4 *)
                                     (puVar3[0x1e] + (ulong)*(uint *)(puVar3[0x1d] + 600));
                            for (uVar18 = 0; uVar18 < *(uint *)(puVar3[0x1d] + 0x270);
                                uVar18 = uVar18 + 0x20) {
                              DataSynchronizationBarrier(3,2,0);
                              *puVar7 = 0xffffffff;
                              puVar7 = puVar7 + 1;
                            }
                            for (uVar18 = 0; uVar18 < *(uint *)(puRamffffff80095bf4d8[0x1d] + 0x270)
                                ; uVar18 = uVar18 + 1) {
                              uVar8 = irq_create_mapping(puRamffffff80095bf4d8[0x1f],
                                                         (long)(int)uVar18);
                              irq_set_chip_and_handler_name
                                        (uVar8,0xffffff8009428250,handle_level_irq,0);
                              irq_set_chip_data(uVar8 & 0xffffffff,puRamffffff80095bf4d8);
                            }
                            irq_set_chained_handler_and_data(iVar4,mtk_eint_irq_handler);
                            uVar6 = 0xffffff8008de8869;
                            goto LAB_ffffff80083b8040;
                          }
                          dev_err(uVar8,0xffffff8008de884b);
                        }
                      }
                    }
                  }
                }
              }
              uVar15 = 0xfffffff4;
            }
            gpiochip_remove(puRamffffff80095bf4d8[0x17]);
            goto LAB_ffffff80083b80f0;
          }
          uVar6 = 0xffffff8008de87f1;
        }
LAB_ffffff80083b8040:
        printk(uVar6);
        goto LAB_ffffff80083b7c74;
      }
    }
    dev_err(uVar8,0xffffff8008de88a1,0xfffffff4);
  }
  uVar15 = 0xffffffea;
LAB_ffffff80083b7c74:
  return uVar15 & 0xffffffff;
}


//// mt_i2c_probe @ffffff8008837e54

/* WARNING: Removing unreachable block (ram,0xffffff800883859c) */

void mt_i2c_probe(long param_1)

{
  long lVar1;
  char cVar2;
  undefined2 uVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  undefined8 *puVar7;
  long lVar8;
  ulong uVar9;
  char *pcVar10;
  undefined8 *puVar11;
  code *pcVar12;
  undefined8 uVar13;
  ulong uVar14;
  long lStack_10;
  long lStack_8;

  lVar1 = param_1 + 0x10;
  lStack_8 = lRamffffff80093cff08;
  puVar7 = (undefined8 *)devm_kmalloc(lVar1,0x5e0,0x24080c0);
  if (puVar7 == (undefined8 *)0x0) {
LAB_ffffff800883861c:
    uVar14 = 0xfffffff4;
    goto LAB_ffffff8008838710;
  }
  uVar13 = *(undefined8 *)(param_1 + 0x238);
  *(undefined4 *)((long)puVar7 + 0x3fc) = 100000;
  of_property_read_u32_array(uVar13,0xffffff8008db65c1,(long)puVar7 + 0x3fc,1);
  of_property_read_u32_array(uVar13,0xffffff8008ee14e3,puVar7 + 0x80,1);
  of_property_read_u32_array(uVar13,0xffffff8008ecc3c0,(long)puVar7 + 0x3bc,1);
  of_property_read_u32_array(uVar13,0xffffff8008ee14f1,puVar7 + 0xb8,1);
  of_property_read_u32_array(uVar13,0xffffff8008ee14ed,puVar7 + 0xb9,1);
  of_property_read_u32_array(uVar13,0xffffff8008eb53fe,(long)puVar7 + 0x404,1);
  iVar5 = of_property_read_u32_array(uVar13,0xffffff8008ee1503,(long)puVar7 + 0x5d4,1);
  *(bool *)((long)puVar7 + 0x5d1) = -1 < iVar5;
  lVar8 = of_find_property(uVar13,0xffffff8008ee1511,0);
  *(bool *)(puVar7 + 0x7e) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee1524,0);
  *(bool *)((long)puVar7 + 0x3f1) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee1536,0);
  *(bool *)((long)puVar7 + 0x3f2) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee154d,0);
  *(bool *)((long)puVar7 + 0x3f3) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee1560,0);
  *(bool *)((long)puVar7 + 0x3f4) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee1574,0);
  *(bool *)((long)puVar7 + 0x3f5) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee158d,0);
  *(bool *)((long)puVar7 + 0x3f6) = lVar8 != 0;
  lVar8 = of_find_property(uVar13,0xffffff8008ee159e,0);
  *(bool *)(puVar7 + 0xba) = lVar8 != 0;
  printk(0xffffff8008ee15b5,*(undefined4 *)((long)puVar7 + 0x3bc),
         *(undefined4 *)((long)puVar7 + 0x3fc),*(undefined4 *)(puVar7 + 0x80),
         *(undefined4 *)(puVar7 + 0xb8),*(undefined4 *)(puVar7 + 0xb9));
  if (*(int *)(puVar7 + 0x80) == 0) goto LAB_ffffff8008838090;
  uVar13 = platform_get_resource(param_1,0x200,0);
  uVar14 = devm_ioremap_resource(lVar1,uVar13);
  puVar7[0x75] = uVar14;
  if (uVar14 < 0xfffffffffffff001) {
    if (*(int *)((long)puVar7 + 0x3bc) < 10) {
      *(undefined8 **)((long)*(int *)((long)puVar7 + 0x3bc) * 8 + -0x7ff67e4ba0) = puVar7;
    }
    uVar13 = platform_get_resource(param_1,0x200,1);
    uVar14 = devm_ioremap_resource(lVar1,uVar13);
    puVar7[0x76] = uVar14;
    if (uVar14 < 0xfffffffffffff001) {
      iVar5 = platform_get_irq(param_1,0);
      *(int *)(puVar7 + 0x77) = iVar5;
      if (0 < iVar5) {
        __init_waitqueue_head(puVar7 + 0x72,0xffffff8008ee15fd,0xffffff800981b4b0);
        uVar9 = devm_request_threaded_irq
                          (lVar1,*(undefined4 *)(puVar7 + 0x77),mt_i2c_irq,0,0x4000,
                           0xffffff8008ee1608,puVar7);
        uVar14 = uVar9 & 0xffffffff;
        if ((int)uVar9 < 0) {
          dev_err(lVar1,0xffffff8008ee160f,*(undefined4 *)(puVar7 + 0x77));
          goto LAB_ffffff8008838710;
        }
        lVar8 = of_match_node(0xffffff8008c487e0,*(undefined8 *)(param_1 + 0x238));
        if (lVar8 == 0) goto LAB_ffffff8008838090;
        pcVar10 = *(char **)(lVar8 + 0xc0);
        puVar7[0x8e] = pcVar10;
        puVar7[0x4e] = *(undefined8 *)(param_1 + 0x238);
        *(undefined4 *)(puVar7 + 8) = 600;
        *(undefined4 *)((long)puVar7 + 0x44) = 1;
        *(undefined2 *)(puVar7 + 0x81) = 0;
        *(undefined2 *)((long)puVar7 + 0x40a) = 0;
        *puVar7 = 0;
        puVar7[2] = 0xffffff8008c49140;
        puVar7[3] = 0;
        puVar7[9] = lVar1;
        puVar7[0x71] = puVar7 + 9;
        *(undefined4 *)(puVar7 + 0x5d) = *(undefined4 *)((long)puVar7 + 0x3bc);
        cVar2 = *pcVar10;
        if (cVar2 == '\x02') {
          if (lVar1 == 0) {
            lVar8 = -0x7ff6c1f550;
          }
          else {
            lVar8 = -0x7ff6c1f550;
            if (*(long *)(param_1 + 0x220) != 0) {
              lVar8 = *(long *)(param_1 + 0x220);
            }
          }
          pcVar12 = *(code **)(lVar8 + 0x70);
          uVar13 = 0x1ffffffff;
          if (pcVar12 == (code *)0x0) {
            if (*(long *)(param_1 + 0x1e8) != 0) {
              if ((lVar1 == 0) || (lVar8 = *(long *)(param_1 + 0x220), lVar8 == 0)) {
                lVar8 = -0x7ff6c1f550;
              }
              if ((*(code **)(lVar8 + 0x68) == (code *)0x0) ||
                 (iVar5 = (**(code **)(lVar8 + 0x68))(lVar1,0x1ffffffff), iVar5 != 0)) {
                puVar11 = *(undefined8 **)(param_1 + 0x1e8);
                uVar13 = 0x1ffffffff;
LAB_ffffff80088381d4:
                *puVar11 = uVar13;
                goto LAB_ffffff8008838244;
              }
            }
          }
          else {
LAB_ffffff8008838238:
            iVar5 = (*pcVar12)(lVar1,uVar13);
            if (iVar5 == 0) goto LAB_ffffff8008838244;
          }
LAB_ffffff80088381f8:
          uVar13 = 0xffffff8008e09784;
        }
        else {
          if (cVar2 == '\x03') {
            if (lVar1 == 0) {
              lVar8 = -0x7ff6c1f550;
            }
            else {
              lVar8 = -0x7ff6c1f550;
              if (*(long *)(param_1 + 0x220) != 0) {
                lVar8 = *(long *)(param_1 + 0x220);
              }
            }
            pcVar12 = *(code **)(lVar8 + 0x70);
            if (pcVar12 != (code *)0x0) {
              uVar13 = 0xfffffffff;
              goto LAB_ffffff8008838238;
            }
            if (*(long *)(param_1 + 0x1e8) != 0) {
              if ((lVar1 == 0) || (lVar8 = *(long *)(param_1 + 0x220), lVar8 == 0)) {
                lVar8 = -0x7ff6c1f550;
              }
              if ((*(code **)(lVar8 + 0x68) == (code *)0x0) ||
                 (iVar5 = (**(code **)(lVar8 + 0x68))(lVar1,0xfffffffff), iVar5 != 0)) {
                uVar13 = 0xfffffffff;
                puVar11 = *(undefined8 **)(param_1 + 0x1e8);
                goto LAB_ffffff80088381d4;
              }
            }
            goto LAB_ffffff80088381f8;
          }
LAB_ffffff8008838244:
          uVar14 = devm_clk_get(lVar1,0xffffff8008e09993);
          puVar7[0x7a] = uVar14;
          if (0xfffffffffffff000 < uVar14) {
            dev_err(lVar1,0xffffff8008ee1628);
            uVar14 = (ulong)*(uint *)(puVar7 + 0x7a);
            goto LAB_ffffff8008838710;
          }
          uVar14 = devm_clk_get(lVar1,0xffffff8008e2cc8d);
          puVar7[0x7b] = uVar14;
          if (0xfffffffffffff000 < uVar14) {
            dev_err(lVar1,0xffffff8008ee163f);
            uVar14 = (ulong)*(uint *)(puVar7 + 0x7b);
            goto LAB_ffffff8008838710;
          }
          uVar14 = devm_clk_get(lVar1,0xffffff8008dbaf8f);
          if (0xfffffffffffff000 < uVar14) {
            uVar14 = 0;
          }
          puVar7[0x7d] = uVar14;
          if (*(char *)(puVar7 + 0x7e) == '\0') {
            uVar14 = puVar7[0x7a];
          }
          else {
            uVar14 = devm_clk_get(lVar1,0xffffff8008f116b1);
            puVar7[0x7c] = uVar14;
            if (0xfffffffffffff000 < uVar14) {
              dev_err(lVar1,0xffffff8008ee1655);
              uVar14 = (ulong)*(uint *)(puVar7 + 0x7c);
              goto LAB_ffffff8008838710;
            }
          }
          uVar9 = clk_get_rate(uVar14);
          uVar14 = 0;
          if ((ulong)*(uint *)(puVar7 + 0x80) != 0) {
            uVar14 = uVar9 / *(uint *)(puVar7 + 0x80);
          }
          puVar7[0xbb] = uVar14 & 0xffffffff;
          dev_warn(lVar1,0xffffff8008ee166c,*(undefined4 *)((long)puVar7 + 0x3bc),puVar7[0x7a],
                   uVar14 & 0xffffffff);
          strlcpy((long)puVar7 + 0x2ec,0xffffff8008ee1608,0x30);
          __mutex_init(puVar7 + 0x87,0xffffff8008ee169a,0xffffff800981b4b0);
          iVar5 = i2c_set_speed(puVar7,uVar14 & 0xffffffff);
          if (iVar5 == 0) {
            uVar9 = mt_i2c_clock_enable(puVar7);
            uVar14 = uVar9 & 0xffffffff;
            if ((int)uVar9 != 0) {
              dev_err(lVar1,0xffffff8008ee16c3);
              goto LAB_ffffff8008838710;
            }
            DataSynchronizationBarrier(3,2,0);
            lVar8 = puVar7[0x75];
            *(undefined2 *)(lVar8 + 8) = 0;
            uVar3 = *(undefined2 *)(lVar8 + 0xc);
            DataSynchronizationBarrier(3,1,0);
            *(undefined2 *)(puVar7 + 0x7f) = uVar3;
            DataSynchronizationBarrier(3,2,0);
            *(undefined2 *)(puVar7[0x75] + 0xc) = uVar3;
            DataSynchronizationBarrier(3,2,0);
            *(undefined2 *)(puVar7[0x75] + 0x50) = 1;
            bVar4 = *(char *)(puVar7[0x8e] + 6) != '\x02';
            if (*(char *)((long)puVar7 + 0x3f2) == '\0') {
              uVar6 = 0x34;
              if (bVar4) {
                uVar6 = 0x40;
              }
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(puVar7[0x75] + (ulong)uVar6) = 3;
            }
            else {
              uVar6 = 0x34;
              if (bVar4) {
                uVar6 = 0x40;
              }
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(puVar7[0x75] + (ulong)uVar6) = 0;
            }
            if ((*(char *)((long)puVar7 + 0x3f1) != '\0') && (*(char *)(puVar7[0x8e] + 6) != '\x02')
               ) {
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(puVar7[0x75] + 0x54) = 0;
            }
            if (*(char *)(puVar7[0x8e] + 6) == '\x02') {
              DataSynchronizationBarrier(3,2,0);
              *(ushort *)(puVar7[0x75] + 0x20) = *(ushort *)((long)puVar7 + 0x42c) | 1;
            }
            else {
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(puVar7[0x75] + 0x20) = *(undefined2 *)((long)puVar7 + 0x42c);
            }
            if (*(char *)(puVar7[0x8e] + 4) != '\0') {
              DataSynchronizationBarrier(3,2,0);
              *(undefined2 *)(puVar7[0x75] + 0x2c) = *(undefined2 *)((long)puVar7 + 0x42e);
            }
            uVar6 = 0x30;
            if (*(char *)(puVar7[0x8e] + 6) != '\x02') {
              uVar6 = 0x48;
            }
            DataSynchronizationBarrier(3,2,0);
            *(undefined2 *)(puVar7[0x75] + (ulong)uVar6) = *(undefined2 *)(puVar7 + 0x86);
            DataSynchronizationBarrier(3,2,0);
            *(undefined4 *)(puVar7[0x76] + (ulong)*(uint *)((long)puVar7 + 0x5cc) + 0xc) = 1;
            __const_udelay(0x53e3);
            DataSynchronizationBarrier(3,1,0);
            if (*(int *)(puVar7[0x76] + (ulong)*(uint *)((long)puVar7 + 0x5cc) + 8) != 0) {
              dev_err(puVar7[0x71],0xffffff8008ee16d9);
              dump_dma_regs();
              warn_slowpath_null(0xffffff8008ee16e9,0x1c1);
            }
            mt_i2c_clock_disable(puVar7);
            if ((lVar1 == 0) ||
               (puVar11 = *(undefined8 **)(param_1 + 0x220), puVar11 == (undefined8 *)0x0)) {
              puVar11 = (undefined8 *)0xffffff80093e0ab0;
            }
            iVar5 = dma_alloc_from_coherent(lVar1,0x1000,puVar7 + 0x79,&lStack_10);
            if (iVar5 == 0) {
              lStack_10 = 0;
              if ((code *)*puVar11 != (code *)0x0) {
                lStack_10 = (*(code *)*puVar11)(lVar1,0x1000,puVar7 + 0x79,0x24000c0,0);
              }
            }
            puVar7[0x78] = lStack_10;
            if (lStack_10 != 0) {
              puVar7[0x1d] = puVar7;
              uVar9 = i2c_add_numbered_adapter(puVar7);
              uVar14 = uVar9 & 0xffffffff;
              if ((int)uVar9 == 0) {
                *(undefined8 **)(param_1 + 0xb0) = puVar7;
                iVar5 = map_cg_regs(puVar7);
                if (iVar5 == 0) {
                  printk(0xffffff8008ee1777);
                }
              }
              else {
                dev_err(lVar1,0xffffff8008ee1754);
                free_i2c_dma_bufs(puVar7);
              }
              goto LAB_ffffff8008838710;
            }
            dev_err(lVar1,0xffffff8008f1e7c3);
            goto LAB_ffffff800883861c;
          }
          uVar13 = 0xffffff8008ee16aa;
        }
        dev_err(lVar1,uVar13);
      }
LAB_ffffff8008838090:
      uVar14 = 0xffffffea;
      goto LAB_ffffff8008838710;
    }
  }
  uVar14 = uVar14 & 0xffffffff;
LAB_ffffff8008838710:
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(uVar14);
  }
  return;
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


//// mt_i2c_set_speed: brak symbolu
//// mt_get_cust_led_list @ffffff80084ae308

void mt_get_cust_led_list(void)

{
  get_cust_led_dtsi();
  return;
}


//// mt65xx_leds_probe @ffffff80084adb04

int mt65xx_leds_probe(long param_1)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *puVar3;
  long lVar4;
  undefined8 *puVar5;
  long lVar6;
  long lVar7;
  int iVar8;

  puVar3 = (undefined8 *)mt_get_cust_led_list();
  if (puVar3 == (undefined8 *)0x0) {
    iVar2 = -1;
    if (iRamffffff800944cf3c != 0) {
      printk(0xffffff8008e184a2,0xffffff8008bd5878);
    }
  }
  else {
    mt_led_get_gpio_info(param_1);
    if (iRamffffff800944cf3c != 0) {
      printk(0xffffff8008e1833c,0xffffff8008bd5878);
    }
    lVar4 = mt_get_div_array();
    lVar7 = 0;
    do {
      if (iRamffffff800944cf3c != 0) {
        printk(0xffffff8008e184b7,*(undefined4 *)(lVar4 + lVar7));
      }
      lVar7 = lVar7 + 4;
    } while (lVar7 != 0x20);
    lVar7 = 0;
    do {
      lVar4 = lVar7 * 8;
      if (*(int *)(puVar3 + 1) == 0) {
        *(undefined8 *)(lVar4 + -0x7ff6a27890) = 0;
      }
      else {
        puVar5 = (undefined8 *)kmem_cache_alloc_trace(uRamffffff8009597948,0x24080c0,0x1c8);
        *(undefined8 **)(lVar4 + -0x7ff6a27890) = puVar5;
        if (puVar5 == (undefined8 *)0x0) {
          iVar2 = -0xc;
LAB_ffffff80084add04:
          if ((int)lVar7 == 0) {
            return iVar2;
          }
          iVar8 = (int)lVar7 + -1;
          do {
            lVar7 = (long)iVar8;
            if (*(long *)(lVar7 * 8 + -0x7ff6a27890) != 0) {
              led_classdev_unregister();
              cancel_work_sync(*(long *)(lVar7 * 8 + -0x7ff6a27890) + 400);
              kfree(*(undefined8 *)(lVar7 * 8 + -0x7ff6a27890));
              *(undefined8 *)(lVar7 * 8 + -0x7ff6a27890) = 0;
            }
            iVar8 = iVar8 + -1;
          } while (iVar8 != -1);
          return iVar2;
        }
        *(undefined4 *)(puVar5 + 0x2d) = *(undefined4 *)(puVar3 + 1);
        puVar5[0x2e] = puVar3[2];
        puVar5[0x2c] = *puVar3;
        *puVar5 = *puVar3;
        lVar6 = *(long *)(lVar4 + -0x7ff6a27890);
        uVar1 = puVar3[4];
        *(undefined8 *)(lVar6 + 0x178) = puVar3[3];
        *(undefined8 *)(lVar6 + 0x180) = uVar1;
        *(undefined4 *)(lVar6 + 0x188) = *(undefined4 *)(puVar3 + 5);
        *(code **)(lVar6 + 0x18) = mt65xx_led_set;
        *(code **)(*(long *)(lVar4 + -0x7ff6a27890) + 0x30) = mt65xx_blink_set;
        lVar4 = *(long *)(lVar4 + -0x7ff6a27890);
        *(undefined8 *)(lVar4 + 400) = 0xfffffffe0;
        *(long *)(lVar4 + 0x198) = lVar4 + 0x198;
        *(long *)(lVar4 + 0x1a0) = lVar4 + 0x198;
        *(code **)(lVar4 + 0x1a8) = mt_mt65xx_led_work;
        iVar2 = led_classdev_register(param_1 + 0x10);
        if (iVar2 != 0) goto LAB_ffffff80084add04;
      }
      lVar7 = lVar7 + 1;
      puVar3 = puVar3 + 6;
    } while (lVar7 != 7);
    uRamffffff80095d8760 = 0;
    uRamffffff80095d8764 = 0;
    uRamffffff80095d8768 = 0;
    uRamffffff800944cf38 = 0xff;
    iVar2 = 0;
    if (iRamffffff800944cf3c != 0) {
      iVar2 = 0;
      printk(0xffffff8008e184d6,0,0xff,0,0);
    }
  }
  return iVar2;
}


//// mt65xx_led_set_cust @ffffff80084ad454

undefined8 mt65xx_led_set_cust(long param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;

  mutex_lock(0xffffff800944cf10);
  uVar1 = param_2;
  uVar2 = param_2;
  if ((iRamffffff80095d8764 != 0) && (uVar1 = uRamffffff80095d8768, uRamffffff800944cf38 < param_2))
  {
    uVar2 = uRamffffff800944cf38;
  }
  uRamffffff80095d8768 = uVar1;
  uRamffffff80095d8760 = param_2;
  mutex_unlock(0xffffff800944cf10);
  if (*(int *)(param_1 + 8) == 5) {
    uVar2 = (int)(uVar2 * 0x3ff + 0x7f) / 0xff;
  }
  mt_mt65xx_led_set_cust(param_1,uVar2);
  return 0xffffffff;
}


//// mt_backlight_set_pwm @ffffff80084ae774

undefined8 mt_backlight_set_pwm(undefined8 param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;

  if (param_4[1] != 0) {
    param_3 = param_4[1];
  }
  iVar1 = *param_4;
  printk(0xffffff8008e1888d,0x100,param_2,0xff);
  printk(0xffffff8008e188bf,0x100,iVar1 == 0,param_3);
  if (param_2 - 1U < 0xff) {
    printk(0xffffff8008e188f7,0x100,param_2,0xff);
  }
  else {
    printk(0xffffff8008e18940,0x100);
  }
  return 0;
}
