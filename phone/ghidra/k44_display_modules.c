//// ddp_get_module_pa @ffffff80085bbdbc

undefined8 ddp_get_module_pa(ulong param_1)

{
  int iVar1;
  undefined8 uVar2;

  param_1 = param_1 & 0xffffffff;
  iVar1 = is_ddp_module_has_reg_info();
  if (iVar1 == 0) {
    iVar1 = ddp_debug_dbg_log_level();
    if ((iVar1 != 0) && (dprec_logger_pr(2,0xffffff8008e4d2af,param_1), iRamffffff8009639114 != 0))
    {
      printk(0xffffff8008e4d2a3,param_1);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined8 *)(param_1 * 0x48 + -0x7ff6b99530);
  }
  return uVar2;
}


//// ddp_get_module_va @ffffff80085bc034

undefined8 ddp_get_module_va(ulong param_1)

{
  int iVar1;
  undefined8 uVar2;

  iVar1 = is_ddp_module_has_reg_info();
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined8 *)((param_1 & 0xffffffff) * 0x48 + -0x7ff6b99520);
  }
  return uVar2;
}


//// ddp_get_module_irq @ffffff80085bc074

ulong ddp_get_module_irq(ulong param_1)

{
  ulong uVar1;

  uVar1 = is_ddp_module_has_reg_info();
  if ((int)uVar1 != 0) {
    uVar1 = (ulong)*(uint *)((param_1 & 0xffffffff) * 0x48 + -0x7ff6b99518);
  }
  return uVar1;
}


//// ddp_get_module_name @ffffff80085bbb50

undefined8 ddp_get_module_name(uint param_1)

{
  if (param_1 < 0x31) {
    return *(undefined8 *)((ulong)param_1 * 0x48 + -0x7ff6b99550);
  }
  dprec_logger_pr(0,0xffffff8008e4d19b,param_1);
  printk(0xffffff8008e4d189,param_1);
  return 0xffffff8008e4af7b;
}
