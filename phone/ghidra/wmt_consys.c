//// consys_soc_chipid_get @00102700

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

UINT32 consys_soc_chipid_get(void)

{
  return 0x6739;
}


//// consys_soc_get_emi_phy_add @00102708

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

P_CONSYS_EMI_ADDR_INFO consys_soc_get_emi_phy_add(void)

{
  return (P_CONSYS_EMI_ADDR_INFO)PTR_mtk_wcn_emi_addr_info_00102710;
}


//// consys_emi_set_remapping_reg @00102ed8

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

UINT32 consys_emi_set_remapping_reg(void)

{
  undefined *puVar1;
  
                    /* Unresolved local var: UINT32 addrPhy@[???]
                       Unresolved local var: char[29] __func__@[???] */
  puVar1 = PTR_conn_reg_00102f50;
  *(uint *)(*(long *)(PTR_conn_reg_00102f50 + 0x10) + 0x1380) =
       (uint)(*(ulong *)PTR_gConEmiPhyBase_00102f58 >> 0x15) & 0x1fff |
       *(uint *)(*(long *)(PTR_conn_reg_00102f50 + 0x10) + 0x1380) | 0x2000;
  DataSynchronizationBarrier(3,3,0);
  if (1 < *(uint *)PTR_wmtPlatLogLvl_00102f60) {
                    /* Unresolved local var: UINT32 addrPhy@[???]
                       Unresolved local var: char[29] __func__@[???] */
    printk(PTR_DAT_00102f70,PTR___func___00102f68 + 0xb0,
           *(undefined4 *)(*(long *)(puVar1 + 0x10) + 0x1380));
    return 0;
  }
  return 0;
}


//// consys_emi_mpu_set_region_protection @00102ac0

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

INT32 consys_emi_mpu_set_region_protection(void)

{
  if (1 < *(uint *)PTR_wmtPlatLogLvl_00102b20) {
    printk(PTR_DAT_00102b30,PTR___func___00102b28 + 0x58);
  }
  emi_mpu_set_region_protection
            (*(long *)PTR_gConEmiPhyBase_00102b38 + 0x80000,
             *(long *)PTR_gConEmiPhyBase_00102b38 + *(long *)PTR_gConEmiSize_00102b40 + -1,0x16,
             0x4b6da28);
  return 0;
}


//// consys_hw_reset_bit_set @00102e80

void consys_hw_reset_bit_set(MTK_WCN_BOOL enable)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  
  if (enable == 0) {
    lVar2 = *(long *)(PTR_conn_reg_00102ed0 + 8);
    uVar1 = *(uint *)(lVar2 + 0x18) & 0x77ffefff;
    uVar3 = 0x88000000;
  }
  else {
    lVar2 = *(long *)(PTR_conn_reg_00102ed0 + 8);
    uVar3 = 0x88001000;
    uVar1 = *(uint *)(lVar2 + 0x18);
  }
  *(uint *)(lVar2 + 0x18) = uVar1 | uVar3;
  DataSynchronizationBarrier(3,3,0);
  return;
}


//// consys_hw_spm_clk_gating_enable @00103668

void consys_hw_spm_clk_gating_enable(void)

{
  return;
}


//// consys_hw_power_ctrl @00102f78

INT32 consys_hw_power_ctrl(MTK_WCN_BOOL enable)

{
  uint uVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)]
                       Unresolved local var: char[21] __func__@[???] */
  puVar2 = PTR_wmtPlatLogLvl_00103048;
  if (enable == 0) {
    uVar5 = *(undefined8 *)(PTR_reg_VCN18_00103040 + 0x20);
    clk_disable(uVar5);
    clk_unprepare(uVar5);
    uVar1 = *(uint *)puVar2;
    iVar3 = -1;
    puVar2 = PTR_DAT_00103060;
    goto joined_r0x00103028;
  }
  uVar5 = *(undefined8 *)(PTR_reg_VCN18_00103040 + 0x20);
                    /* Unresolved local var: int ret@[DW_OP_reg19(x19)] */
  iVar3 = clk_prepare(uVar5);
  if (iVar3 == 0) {
    iVar4 = clk_enable(uVar5);
    iVar3 = 0;
    if (iVar4 != 0) {
      clk_unprepare(uVar5);
      iVar3 = iVar4;
      goto LAB_00102fc0;
    }
  }
  else {
LAB_00102fc0:
    printk(PTR_DAT_00103068,PTR___func___00103050 + 0xd0,0x1dd,iVar3);
  }
  uVar1 = *(uint *)puVar2;
  puVar2 = PTR_DAT_00103058;
joined_r0x00103028:
  if (2 < uVar1) {
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)]
                       Unresolved local var: char[21] __func__@[???] */
    printk(puVar2,PTR___func___00103050 + 0xd0);
  }
  return iVar3;
}


//// consys_clk_get_from_dts @00103270

/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 consys_clk_get_from_dts(platform_device *pdev)

{
  undefined *puVar1;
  ulong uVar2;
  INT32 IVar3;
  
                    /* Unresolved local var: char[24] __func__@[???] */
  uVar2 = devm_clk_get(&pdev->dev,PTR_DAT_001032f0);
  puVar1 = PTR_reg_VCN18_001032f8;
  *(ulong *)(PTR_reg_VCN18_001032f8 + 0x20) = uVar2;
  if (uVar2 < 0xfffffffffffff001) {
    IVar3 = 0;
    if (2 < *(uint *)PTR_wmtPlatLogLvl_00103310) {
      printk(PTR_DAT_00103318,PTR___func___00103300 + 0x128,uVar2);
    }
  }
  else {
                    /* Unresolved local var: char[24] __func__@[???] */
    printk(PTR_DAT_00103308,PTR___func___00103300 + 0x128,0x15b);
    IVar3 = *(INT32 *)(puVar1 + 0x20);
  }
  return IVar3;
}


//// polling_consys_chipid @00102cf8

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

INT32 polling_consys_chipid(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  
  puVar3 = PTR_DAT_00102e50;
                    /* Unresolved local var: UINT32 retry@[???]
                       Unresolved local var: UINT32 consysHwChipId@[???] */
  puVar2 = PTR_conn_reg_00102e30;
  puVar1 = PTR___func___00102e40 + 0x98;
  iVar5 = 0;
  iVar4 = 10;
  do {
    iVar4 = iVar4 + -1;
    if (iVar4 == -1) {
LAB_00102d68:
      if (iVar4 == 0 || iVar5 == 0) {
        puVar1 = PTR___func___00102e40 + 0x98;
        printk(PTR_DAT_00102e58,puVar1,0x291,iVar5);
        puVar2 = PTR_conn_reg_00102e30;
        printk(PTR_DAT_00102e60,puVar1,0x293,
               *(undefined4 *)(*(long *)(PTR_conn_reg_00102e30 + 8) + 0x18));
        printk(PTR_DAT_00102e68,puVar1,0x295,*(undefined4 *)(*(long *)(puVar2 + 0x18) + 0x180));
        printk(PTR_DAT_00102e70,puVar1,0x297,*(undefined4 *)(*(long *)(puVar2 + 0x18) + 0x184));
        printk(PTR_DAT_00102e78,puVar1,0x299,*(undefined4 *)(*(long *)(puVar2 + 0x18) + 0x32c));
      }
      return 0;
    }
    iVar5 = *(int *)(*(long *)puVar2 + 8);
    if (iVar5 == 0x699) {
      if (1 < *(uint *)PTR_wmtPlatLogLvl_00102e38) {
        printk(PTR_DAT_00102e48,PTR___func___00102e40 + 0x98,iVar4,0x699);
      }
      goto LAB_00102d68;
    }
    printk(puVar3,puVar1,0x28c,iVar5);
    msleep(0x14);
  } while( true );
}


//// mtk_wcn_consys_hw_init @00107368

/* WARNING: Unknown calling convention -- yet parameter storage is locked */
/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 mtk_wcn_consys_hw_init(void)

{
  undefined *puVar1;
  int iVar2;
  P_WMT_CONSYS_IC_OPS p_Var3;
  
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)] */
  puVar1 = PTR_wmt_consys_ic_ops_001073c8;
  if (*(long *)PTR_wmt_consys_ic_ops_001073c8 == 0) {
    p_Var3 = mtk_wcn_get_consys_ic_ops();
    *(P_WMT_CONSYS_IC_OPS *)puVar1 = p_Var3;
  }
  iVar2 = __platform_driver_register(PTR_gps_lna_pin_num_001073d8 + 8,PTR___this_module_001073d0);
  if (iVar2 != 0) {
    printk(PTR_DAT_001073e8,PTR___func___30614_001073e0 + 0x168,0x20f,iVar2);
  }
  return iVar2;
}


//// mtk_wcn_consys_hw_reg_ctrl @00106a70

INT32 mtk_wcn_consys_hw_reg_ctrl(UINT32 on,UINT32 co_clock_type)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  code *pcVar4;
  
  puVar3 = PTR_wmt_consys_ic_ops_00106d10;
                    /* Unresolved local var: INT32 iRet@[???] */
  puVar2 = PTR_wmtPlatLogLvl_00106cf0;
  if (*(int *)PTR_wmtPlatLogLvl_00106cf0 == 0) {
    if (on == 0) goto LAB_00106c80;
  }
  else {
    puVar1 = PTR___func___30614_00106cf8 + 0x90;
    printk(PTR_DAT_00106d00,puVar1,on);
    if (on == 0) {
LAB_00106c80:
      if (*(code **)(*(long *)puVar3 + 0x20) != (code *)0x0) {
        (**(code **)(*(long *)puVar3 + 0x20))(0);
      }
      if (*(code **)(*(long *)puVar3 + 0x18) != (code *)0x0) {
        (**(code **)(*(long *)puVar3 + 0x18))(0);
      }
      if (co_clock_type == 0) {
        pcVar4 = (code *)(*(undefined8 **)puVar3)[0xb];
        if (pcVar4 != (code *)0x0) {
          (*pcVar4)(0);
        }
        pcVar4 = *(code **)(*(long *)puVar3 + 0x60);
      }
      else {
        pcVar4 = (code *)**(undefined8 **)puVar3;
      }
      if (pcVar4 != (code *)0x0) {
        (*pcVar4)(0);
      }
      if (*(code **)(*(long *)puVar3 + 0x50) != (code *)0x0) {
        (**(code **)(*(long *)puVar3 + 0x50))(0);
      }
      if (*(code **)(*(long *)puVar3 + 0xf8) != (code *)0x0) {
        (**(code **)(*(long *)puVar3 + 0xf8))(0);
      }
      goto LAB_00106c00;
    }
    if (2 < *(uint *)puVar2) {
      printk(PTR_DAT_00106d08,puVar1);
    }
  }
  if (*(code **)(*(long *)puVar3 + 0xf8) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0xf8))(1);
  }
  if (*(code **)(*(long *)puVar3 + 0x50) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x50))(1);
  }
  __const_udelay(0x9d49a);
  if (co_clock_type == 0) {
    if (*(code **)(*(long *)puVar3 + 0x58) != (code *)0x0) {
      (**(code **)(*(long *)puVar3 + 0x58))(1);
    }
    if (*(code **)(*(long *)puVar3 + 0x60) != (code *)0x0) {
      (**(code **)(*(long *)puVar3 + 0x60))(1);
    }
  }
  else {
    if (1 < *(uint *)puVar2) {
      printk(PTR_DAT_00106d18,PTR___func___30614_00106cf8 + 0x90,co_clock_type);
    }
    if ((code *)**(undefined8 **)puVar3 != (code *)0x0) {
      (*(code *)**(undefined8 **)puVar3)(1);
    }
    if (*(code **)(*(long *)puVar3 + 0x58) != (code *)0x0) {
      (**(code **)(*(long *)puVar3 + 0x58))(0);
    }
  }
  if (*(code **)(*(long *)puVar3 + 8) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 8))(1);
  }
  if (*(code **)(*(long *)puVar3 + 0x10) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x10))();
  }
  if (*(code **)(*(long *)puVar3 + 0x18) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x18))(1);
  }
  __const_udelay(0xa7c6);
  if (*(code **)(*(long *)puVar3 + 0x20) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x20))(1);
  }
  if (*(code **)(*(long *)puVar3 + 0x28) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x28))();
  }
  if (*(code **)(*(long *)puVar3 + 0x30) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x30))();
  }
  if (*(code **)(*(long *)puVar3 + 0x40) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x40))();
  }
  if (*(code **)(*(long *)puVar3 + 0x48) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 0x48))();
  }
  if (*(code **)(*(long *)puVar3 + 8) != (code *)0x0) {
    (**(code **)(*(long *)puVar3 + 8))(0);
  }
  msleep(0x14);
LAB_00106c00:
  if (*(int *)puVar2 != 0) {
    printk(PTR_DAT_00106d20,PTR___func___30614_00106cf8 + 0x90,on);
  }
  return 0;
}


//// consys_read_reg_from_dts @00102850

/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 consys_read_reg_from_dts(platform_device *pdev)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined8 uVar3;
  INT32 IVar4;
  device_node *pdVar5;
  
                    /* Unresolved local var: INT32 iRet@[???]
                       Unresolved local var: device_node * node@[???] */
  pdVar5 = (pdev->dev).of_node;
  if (pdVar5 == (device_node *)0x0) {
    IVar4 = -1;
    printk(PTR_DAT_001029a0,PTR___func___00102978 + 0x18,0x440);
  }
  else {
    uVar3 = of_iomap(pdVar5,0);
    puVar2 = PTR_wmtPlatLogLvl_00102970;
    puVar1 = PTR_conn_reg_00102968;
    *(undefined8 *)PTR_conn_reg_00102968 = uVar3;
    if (2 < *(uint *)puVar2) {
      printk(PTR_DAT_00102980,PTR___func___00102978 + 0x18,uVar3);
    }
    uVar3 = of_iomap(pdVar5,1);
    *(undefined8 *)(puVar1 + 8) = uVar3;
    if (2 < *(uint *)puVar2) {
      printk(PTR_DAT_00102988,PTR___func___00102978 + 0x18,uVar3);
    }
    uVar3 = of_iomap(pdVar5,2);
    *(undefined8 *)(puVar1 + 0x10) = uVar3;
    if (2 < *(uint *)puVar2) {
      printk(PTR_DAT_00102990,PTR___func___00102978 + 0x18,uVar3);
    }
    uVar3 = of_iomap(pdVar5,3);
    *(undefined8 *)(puVar1 + 0x18) = uVar3;
    IVar4 = 0;
    if (2 < *(uint *)puVar2) {
      printk(PTR_DAT_00102998,PTR___func___00102978 + 0x18,uVar3);
    }
  }
  return IVar4;
}


//// consys_pmic_get_from_dts @001029a8

/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 consys_pmic_get_from_dts(platform_device *pdev)

{
  device *pdVar1;
  undefined *puVar2;
  long lVar3;
  
  pdVar1 = &pdev->dev;
  lVar3 = regulator_get(pdVar1,PTR_s_vcn18_00102a70);
  puVar2 = PTR_reg_VCN18_00102a78;
  *(long *)PTR_reg_VCN18_00102a78 = lVar3;
  if (lVar3 == 0) {
    printk(PTR_DAT_00102a88,PTR___func___00102a80 + 0x38,0x174);
  }
  lVar3 = regulator_get(pdVar1,PTR_s_vcn28_00102a90);
  *(long *)(puVar2 + 8) = lVar3;
  if (lVar3 == 0) {
    printk(PTR_DAT_00102a98,PTR___func___00102a80 + 0x38,0x177);
  }
  lVar3 = regulator_get(pdVar1,PTR_s_vcn33_bt_00102aa0);
  *(long *)(puVar2 + 0x10) = lVar3;
  if (lVar3 == 0) {
    printk(PTR_DAT_00102aa8,PTR___func___00102a80 + 0x38,0x17a);
  }
  lVar3 = regulator_get(pdVar1,PTR_s_vcn33_wifi_00102ab0);
  *(long *)(puVar2 + 0x18) = lVar3;
  if (lVar3 == 0) {
    printk(PTR_DAT_00102ab8,PTR___func___00102a80 + 0x38,0x17d);
  }
  return 0;
}


//// consys_read_irq_info_from_dts @00103148

/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 consys_read_irq_info_from_dts(platform_device *pdev,PINT32 irq_num,PUINT32 irq_flag)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  long lVar4;
  INT32 IVar5;
  device_node *pdVar6;
  UINT32 irq_info [3];
  
                    /* Unresolved local var: device_node * node@[???]
                       Unresolved local var: INT32 iret@[???]
                       Unresolved local var: char[30] __func__@[???] */
  puVar1 = PTR___stack_chk_guard_00103238;
  pdVar6 = (pdev->dev).of_node;
  lVar4 = *(long *)PTR___stack_chk_guard_00103238;
  irq_info[0] = 0;
  irq_info[1] = 0;
  irq_info[2] = 0;
  if (pdVar6 == (device_node *)0x0) {
    printk(PTR_DAT_00103268,PTR___func___00103248 + 0x108,0x427);
  }
  else {
    iVar3 = irq_of_parse_and_map(pdVar6,0);
    *irq_num = iVar3;
    iVar3 = of_property_read_u32_array(pdVar6,PTR_s_interrupts_00103240,irq_info,3);
    puVar2 = PTR_wmtPlatLogLvl_00103258;
    if (iVar3 == 0) {
      IVar5 = 0;
      *irq_flag = irq_info[2];
      if (1 < *(uint *)puVar2) {
        printk(PTR_DAT_00103260,PTR___func___00103248 + 0x108,*irq_num);
      }
      goto LAB_001031f0;
    }
    printk(PTR_DAT_00103250,PTR___func___00103248 + 0x108,0x420);
  }
                    /* Unresolved local var: device_node * node@[???]
                       Unresolved local var: INT32 iret@[???]
                       Unresolved local var: char[30] __func__@[???] */
  IVar5 = -1;
LAB_001031f0:
  if (lVar4 != *(long *)puVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return IVar5;
}


//// consys_hw_vcn18_ctrl @00102b48

/* WARNING: Enum "anon_enum_32": Some values do not have unique names */

INT32 consys_hw_vcn18_ctrl(MTK_WCN_BOOL enable)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_reg_VCN18_00102cb8;
  if (enable == 0) {
    connectivity_export_pmic_set_register_value(0xc2f,1);
    connectivity_export_pmic_set_register_value(0xc31,1);
    if (*(long *)(puVar1 + 0x10) != 0) {
      regulator_disable();
    }
    connectivity_export_pmic_set_register_value(0xc2f,1);
    connectivity_export_pmic_set_register_value(0xc31,1);
    if (*(long *)puVar1 != 0) {
      iVar2 = regulator_disable();
      if (iVar2 == 0) {
        if (2 < *(uint *)PTR_wmtPlatLogLvl_00102cd0) {
          printk(PTR_DAT_00102cf0,PTR___func___00102cc0 + 0x80);
        }
      }
      else {
        printk(PTR_DAT_00102ce8,PTR___func___00102cc0 + 0x80,0x322);
      }
    }
  }
  else {
    connectivity_export_pmic_set_register_value(0xc2f,1);
    connectivity_export_pmic_set_register_value(0xc31,1);
    if (*(long *)puVar1 != 0) {
      regulator_set_voltage(*(long *)puVar1,1800000);
      iVar2 = regulator_enable(*(undefined8 *)puVar1);
      if (iVar2 == 0) {
        if (2 < *(uint *)PTR_wmtPlatLogLvl_00102cd0) {
          printk(PTR_DAT_00102cd8,PTR___func___00102cc0 + 0x80);
        }
      }
      else {
        printk(PTR_DAT_00102cc8,PTR___func___00102cc0 + 0x80,0x2fa);
      }
    }
    if (*(long *)(puVar1 + 0x10) != 0) {
      regulator_set_voltage(*(long *)(puVar1 + 0x10),3300000);
      iVar2 = regulator_enable(*(undefined8 *)(puVar1 + 0x10));
      if (iVar2 != 0) {
        printk(PTR_DAT_00102ce0,PTR___func___00102cc0 + 0x80,0x303);
      }
    }
    connectivity_export_pmic_set_register_value(0xd03,0);
    connectivity_export_pmic_set_register_value(0xd08,0);
  }
  return 0;
}


//// consys_hw_vcn28_ctrl @00103560

INT32 consys_hw_vcn28_ctrl(UINT32 enable)

{
  uint uVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  
  puVar3 = PTR_wmtPlatLogLvl_00103620;
                    /* Unresolved local var: char[21] __func__@[???] */
  puVar2 = PTR_reg_VCN18_00103608;
  if (enable == 0) {
    if (*(long *)(PTR_reg_VCN18_00103608 + 8) != 0) {
      regulator_disable();
    }
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103630;
  }
  else {
    if (*(long *)(PTR_reg_VCN18_00103608 + 8) != 0) {
      regulator_set_voltage(*(long *)(PTR_reg_VCN18_00103608 + 8),2800000);
      iVar4 = regulator_enable(*(undefined8 *)(puVar2 + 8));
      if (iVar4 != 0) {
        printk(PTR_DAT_00103618,PTR___func___00103610 + 0x178,0x34b);
      }
    }
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103628;
  }
  if (1 < uVar1) {
    printk(puVar2,PTR___func___00103610 + 0x178);
  }
  return 0;
}


//// consys_hw_bt_vcn33_ctrl @00103350

INT32 consys_hw_bt_vcn33_ctrl(UINT32 enable)

{
  uint uVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  
  puVar3 = PTR_wmtPlatLogLvl_00103440;
                    /* Unresolved local var: char[24] __func__@[???] */
  puVar2 = PTR_reg_VCN18_00103428;
  if (enable == 0) {
    connectivity_export_pmic_set_register_value(0xd03,0);
    connectivity_export_pmic_set_register_value(0xd08,0);
    if (*(long *)(puVar2 + 0x10) != 0) {
      regulator_disable();
    }
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103450;
  }
  else {
    if (*(long *)(PTR_reg_VCN18_00103428 + 0x10) != 0) {
      regulator_set_voltage(*(long *)(PTR_reg_VCN18_00103428 + 0x10),3300000);
      iVar4 = regulator_enable(*(undefined8 *)(puVar2 + 0x10));
      if (iVar4 != 0) {
        printk(PTR_DAT_00103438,PTR___func___00103430 + 0x140,0x38d);
      }
    }
    connectivity_export_pmic_set_register_value(0xd03,1);
    connectivity_export_pmic_set_register_value(0xd08,0);
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103448;
  }
  if (2 < uVar1) {
                    /* Unresolved local var: char[24] __func__@[???] */
    printk(puVar2,PTR___func___00103430 + 0x140);
  }
  return 0;
}


//// consys_hw_wifi_vcn33_ctrl @00103458

INT32 consys_hw_wifi_vcn33_ctrl(UINT32 enable)

{
  uint uVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  
  puVar3 = PTR_wmtPlatLogLvl_00103548;
                    /* Unresolved local var: char[26] __func__@[???] */
  puVar2 = PTR_reg_VCN18_00103530;
  if (enable == 0) {
    connectivity_export_pmic_set_register_value(0xd03,0);
    connectivity_export_pmic_set_register_value(0xd08,0);
    if (*(long *)(puVar2 + 0x18) != 0) {
      regulator_disable();
    }
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103558;
  }
  else {
    if (*(long *)(PTR_reg_VCN18_00103530 + 0x18) != 0) {
      regulator_set_voltage(*(long *)(PTR_reg_VCN18_00103530 + 0x18),3300000);
      iVar4 = regulator_enable(*(undefined8 *)(puVar2 + 0x18));
      if (iVar4 != 0) {
        printk(PTR_DAT_00103540,PTR___func___00103538 + 0x158,0x3c1);
      }
    }
    connectivity_export_pmic_set_register_value(0xd03,1);
    connectivity_export_pmic_set_register_value(0xd08,0);
    uVar1 = *(uint *)puVar3;
    puVar2 = PTR_DAT_00103550;
  }
  if (2 < uVar1) {
                    /* Unresolved local var: char[26] __func__@[???] */
    printk(puVar2,PTR___func___00103538 + 0x158);
  }
  return 0;
}


//// consys_vcn28_hw_mode_ctrl @00103320

void consys_vcn28_hw_mode_ctrl(UINT32 enable)

{
  connectivity_export_pmic_set_register_value(0xd3b,enable != 0);
  connectivity_export_pmic_set_register_value(0xd41,0);
  return;
}


//// consys_clock_buffer_ctrl @00103638

INT32 consys_clock_buffer_ctrl(MTK_WCN_BOOL enable)

{
  if ((enable == 1) || (enable == 0)) {
    connectivity_export_clk_buf_ctrl(1,enable);
  }
  return 0;
}


//// consys_acr_reg_setting @001026b0

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void consys_acr_reg_setting(void)

{
  *(uint *)(*(long *)PTR_conn_reg_001026d0 + 0x110) =
       *(uint *)(*(long *)PTR_conn_reg_001026d0 + 0x110) | 0x40000;
  DataSynchronizationBarrier(3,3,0);
  return;
}


//// consys_afe_reg_setting @001026d8

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void consys_afe_reg_setting(void)

{
  return;
}


//// mtk_wcn_consys_hw_pwr_on @00107018

INT32 mtk_wcn_consys_hw_pwr_on(UINT32 co_clock_type)

{
  undefined *puVar1;
  INT32 IVar2;
  INT32 IVar3;
  int iVar4;
  
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)] */
  puVar1 = PTR_wmtPlatLogLvl_001070d8;
  if (1 < *(uint *)PTR_wmtPlatLogLvl_001070d8) {
    printk(PTR_DAT_001070e8,PTR___func___30614_001070e0 + 0xf0);
  }
  if (*(long *)PTR_gConEmiPhyBase_001070f0 == 0) {
    puVar1 = PTR___func___30614_001070e0 + 0xf0;
    printk(PTR_DAT_001070f8,puVar1,0x1bf);
    iVar4 = -1;
    printk(PTR_DAT_00107100,puVar1,0x1c0);
  }
  else {
    IVar2 = mtk_wcn_consys_hw_reg_ctrl(1,co_clock_type);
    IVar3 = mtk_wcn_consys_hw_gpio_ctrl(1);
    iVar4 = IVar2 + IVar3;
    mtk_wcn_consys_jtag_set_for_mcu();
    if (1 < *(uint *)puVar1) {
      printk(PTR_DAT_00107108,PTR___func___30614_001070e0 + 0xf0,iVar4);
    }
  }
  return iVar4;
}


//// mtk_wcn_consys_hw_pwr_off @00107110

INT32 mtk_wcn_consys_hw_pwr_off(UINT32 co_clock_type)

{
  undefined *puVar1;
  INT32 IVar2;
  INT32 IVar3;
  
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)] */
  puVar1 = PTR_wmtPlatLogLvl_00107190;
  if (1 < *(uint *)PTR_wmtPlatLogLvl_00107190) {
    printk(PTR_DAT_001071a0,PTR___func___30614_00107198 + 0x110);
  }
  IVar2 = mtk_wcn_consys_hw_reg_ctrl(0,co_clock_type);
  IVar3 = mtk_wcn_consys_hw_gpio_ctrl(0);
  if (1 < *(uint *)puVar1) {
    printk(PTR_DAT_001071a8,PTR___func___30614_00107198 + 0x110,IVar2 + IVar3);
  }
  return IVar2 + IVar3;
}


//// mtk_wcn_consys_stp_btif_open @00142bd8

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

INT32 mtk_wcn_consys_stp_btif_open(void)

{
  uint uVar1;
  INT32 IVar2;
  
                    /* Unresolved local var: INT32 iRet@[DW_OP_reg19(x19)] */
  uVar1 = mtk_wcn_btif_open(PTR_s_CONSYS_STP_00142c60,*(undefined8 *)PTR_pBtifRef_00142c58);
  if (uVar1 == 0) {
    if (2 < *(uint *)PTR_gWmtDbgLvl_00142c68) {
      osal_warn_print(PTR_s__STP_BTIF__D__s_STP_open_bitf_OK_00142c80,
                      PTR___func___28303_00142c70 + 0x20);
    }
    mtk_wcn_stp_register_if_tx
              (STP_BTIF_IF_TX,(MTK_WCN_STP_IF_TX)PTR_mtk_wcn_consys_stp_btif_tx_00142c88);
    IVar2 = 0;
  }
  else {
    if (*(uint *)PTR_gWmtDbgLvl_00142c68 != 0) {
      osal_warn_print(PTR_s__STP_BTIF__W__s_STP_open_btif_fa_00142c78,
                      PTR___func___28303_00142c70 + 0x20,(ulong)uVar1);
    }
    IVar2 = -1;
  }
  return IVar2;
}


//// mtk_wcn_consys_stp_btif_tx @001429c8

INT32 mtk_wcn_consys_stp_btif_tx(PUINT8 pBuf,UINT32 len,PUINT32 written_len)

{
  long lVar1;
  undefined *puVar2;
  uint uVar3;
  uint uVar4;
  ulong uVar5;
  uint uVar6;
  
                    /* Unresolved local var: INT32 retry_left@[???]
                       Unresolved local var: INT32 wr_count@[???]
                       Unresolved local var: INT32 written@[???]
                       Unresolved local var: char[27] __func__@[???] */
  puVar2 = PTR_stpBtifId_00142b88;
  if (*(long *)PTR_stpBtifId_00142b88 == 0) {
    uVar3 = 0xffffffff;
    if (*(int *)PTR_gWmtDbgLvl_00142b90 != 0) {
      osal_warn_print(PTR_s__STP_BTIF__W__s_NULL_BTIF_ID_ref_00142ba0,PTR___func___28303_00142b98);
    }
  }
  else {
    *written_len = 0;
    if (len == 0) {
      uVar3 = 0;
      if (1 < *(uint *)PTR_gWmtDbgLvl_00142b90) {
        osal_warn_print(PTR_s__STP_BTIF__I__s_special_case_for_00142ba8,PTR___func___28303_00142b98,
                        pBuf);
      }
    }
    else {
                    /* Unresolved local var: INT32 retry_left@[???]
                       Unresolved local var: INT32 wr_count@[DW_OP_reg19(x19)]
                       Unresolved local var: INT32 written@[???]
                       Unresolved local var: char[27] __func__@[???] */
      if (len < 0x7d1) {
        uVar3 = mtk_wcn_btif_write(*(undefined8 *)puVar2,pBuf,len);
        if ((int)uVar3 < 0) {
          uVar5 = (ulong)uVar3;
          uVar3 = 0xfffffffd;
          osal_err_print(PTR_s__STP_BTIF__E__s__d__mtk_wcn_btif_00142bb8,PTR___func___28303_00142b98
                         ,0x9e,uVar5);
          *written_len = 0;
        }
        else {
          uVar6 = 10;
          if (len != uVar3) {
            while( true ) {
              uVar6 = uVar6 - 1;
              if ((uVar6 == 0xffffffff) || (len <= uVar3)) goto LAB_00142b24;
              osal_sleep_ms(5);
              uVar4 = mtk_wcn_btif_write(*(undefined8 *)puVar2,pBuf + (int)uVar3,len - uVar3);
              if ((int)uVar4 < 0) break;
              uVar3 = uVar3 + uVar4;
            }
            osal_err_print(PTR_s__STP_BTIF__E__s__d__mtk_wcn_btif_00142bc0,
                           PTR___func___28303_00142b98,0xac,(ulong)uVar4);
LAB_00142b24:
            if (len != uVar3) {
                    /* Unresolved local var: ulong sp_el0@[???] */
              lVar1 = sp_el0;
              osal_err_print(PTR_s__STP_BTIF__E__s__d__stp_btif_wri_00142bd0,
                             PTR___func___28303_00142b98,0xba,(ulong)len,(ulong)uVar3,(ulong)uVar6,
                             (ulong)*(uint *)(lVar1 + 0x488),lVar1 + 0x658);
              *written_len = 0;
              return -uVar3;
            }
            if (1 < *(uint *)PTR_gWmtDbgLvl_00142b90) {
              osal_warn_print(PTR_s__STP_BTIF__I__s_recovered_len__d_00142bc8,
                              PTR___func___28303_00142b98,(ulong)uVar3,(ulong)uVar6);
            }
          }
          *written_len = uVar3;
        }
      }
      else {
        uVar3 = 0xfffffffe;
        if (*(int *)PTR_gWmtDbgLvl_00142b90 != 0) {
                    /* Unresolved local var: ulong sp_el0@[???] */
          lVar1 = sp_el0;
          osal_warn_print(PTR_s__STP_BTIF__W__s_abnormal_pacage_l_00142bb0,
                          PTR___func___28303_00142b98,(ulong)len,(ulong)*(uint *)(lVar1 + 0x488),
                          lVar1 + 0x658);
        }
      }
    }
  }
  return uVar3;
}


