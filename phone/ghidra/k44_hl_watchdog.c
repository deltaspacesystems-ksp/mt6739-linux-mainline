//// charging_reset_watch_dog_timer @ffffff80088555c4

void charging_reset_watch_dog_timer(void)

{
  int iVar1;
  char cStack_9;
  long lStack_8;

  lStack_8 = lRamffffff80093cff08;
  cStack_9 = '\0';
  fan54005_reg_config_interface(6,0x7b);
  fan54005_reg_config_interface(6,0x7b);
  fan54005_read_byte(6,&cStack_9);
  if ((cStack_9 != '{') && (iVar1 = chr_get_debug_level(), 0 < iVar1)) {
    printk(0xffffff8008eec8e3,0xffffff8008c4afa8,0x3ff);
  }
  fan54005_set_tmr_rst(1);
  if (lStack_8 != lRamffffff80093cff08) {
    __stack_chk_fail(0);
  }
  return;
}


//// fan54005_set_tmr_rst @ffffff80088555a0

void fan54005_set_tmr_rst(undefined4 param_1)

{
  fan54005_config_interface(0,param_1,1,7);
  return;
}


//// fan54005_config_interface @ffffff80088552a0

void fan54005_config_interface(char param_1,uint param_2,char param_3,uint param_4)

{
  long lVar1;
  uint uVar2;
  long *plVar3;

  lVar1 = lRamffffff80093cff08;
  plVar3 = (long *)0xffffff80093cf000;
  fan54005_read_byte(param_1);
  uVar2 = (param_2 & 0xff) << (ulong)(param_4 & 0x1f) & 0xff;
  if (param_1 != '\x04' || (param_2 & 0xff) != 1) goto LAB_ffffff8008855340;
  if (param_3 != '\x01' || (param_4 & 0xff) != 7) goto LAB_ffffff8008855348;
  while( true ) {
    plVar3 = plVar3 + 0x1e1;
    fan54005_write_byte(param_1,uVar2 & 0xff);
    if (lVar1 == *plVar3) break;
    uVar2 = __stack_chk_fail();
LAB_ffffff8008855340:
    if (param_1 == '\x04') {
LAB_ffffff8008855348:
      uVar2 = uVar2 & 0x7f;
    }
  }
  return;
}
