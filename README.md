# mt6739-linux-mainline: native Linux 7.3 on the Huawei Y5 2018 (MT6739, DURA-L21)

Goal: get rid of Android and run **mainline Linux 7.3 with a custom userland** (Alpine) on this phone, and eventually a
modern iOS/macOS-style user interface written in C, plus modem, GPS, sensors, LEDs, flashlight and IMU. Work is done
remotely: a cloud agent controls the PC and the phone through our own signed bridge.

> **Documentation language:** this README is in English. Most documents under `docs/` and `phone/NOTES.md`, as well as
> many code comments, are still in Polish and are being translated. New text is written in English.

> **If you are a new agent (e.g. Codex): start with [`docs/HANDOFF.md`](docs/HANDOFF.md).** It is the full hand-over:
> current state, procedures, pitfalls and the list of next steps. Build details: [`docs/BUILD_AND_FLASH.md`](docs/BUILD_AND_FLASH.md).
> Bridge keys: [`docs/KLUCZE_I_ZMIENNE.md`](docs/KLUCZE_I_ZMIENNE.md) (keys and variables).

## Status as of 2026-10-24 (evening)

This table is a snapshot from that date. The repository has not been updated for later work yet.

| Area | State |
|---|---|
| Kernel 7.3 | Boots from the `boot` partition (LK -> kernel -> Alpine initramfs), reaches init, stable |
| Display | Text works on the panel: own renderer `y5txt` (writes to the scan-out buffer, manual DSI trigger) |
| eMMC | **Works**: `mmcblk0` 14.6 GiB, partitions p1..p18 (HS200) |
| USB | **Works**: MUSB (PIO mode) + t-phy, configfs RNDIS gadget, Windows picks the driver automatically |
| Network over USB | v41: phone `192.168.7.2/24`, DHCP for the PC, telnet (testing only) on that link |
| Root filesystem on flash | Not yet (currently an initramfs in RAM) |
| UI, modem, GPS, sensors, LED, touch | Not on 7.3 yet (a UI prototype ran in an Android 4.4 chroot) |

## Remote-work architecture

```
cloud agent --HTTPS--> Cloudflare --> nginx (PC A) --> bridge_server.py (PC B with the phone) --USB--> phone
                                                  \--> pc_agent.py (PowerShell on the PC, only with -Pc)
```

- Client: [`bridge/y5sh_cloud.py`](bridge/y5sh_cloud.py) (`run`, `push`, `pull`, `ps`, `job`, `job-status`, `pc-put`, `pc-get`, `pc-info`).
- Server: [`bridge/bridge_server.py`](bridge/bridge_server.py), PC agent: [`bridge/pc_agent.py`](bridge/pc_agent.py), start script: `bridge/start_bridge_signed.ps1`.
- Requests are HMAC-signed, carry a Bearer token and are protected against replay. Environment variables and key rotation: `docs/KLUCZE_I_ZMIENNE.md`.
- Cloudflare limit: **100 s** per request. Long jobs always run in the background (`nohup`, log to a file).

## Repository map

| Path | Contents |
|---|---|
| `bridge/` | The cloud -> PC -> phone bridge (server, PC agent, client, nginx) |
| `docs/` | `HANDOFF.md`, `BUILD_AND_FLASH.md`, `KLUCZE_I_ZMIENNE.md` |
| `phone/NOTES.md` | Findings log (chronological: every breakthrough and every dead end) |
| `phone/kernel/` | **Patch against a clean linux-7.3** (`y5-kernel-7.3.patch`), `config-7.3-y5`, `bld.sh` |
| `phone/stub/` | Image building (`bm_wd*.py`), DTB (`extras_v*.dtsi`, `build_dtbv*.sh`), userland tools (`y5txt.c`, `y5cal.c`, `wdt3.c`, `mkfont.py`, `build_tools.sh`) |
| `phone/ghidra/`, `phone/*_44.txt` | Decompilation and data from the 4.4 kernel (clock tables, kallsyms, Ghidra output `k44_dec*.c`) |
| `phone/y5-ui`, `phone/*.py` | UI prototype and fixes for the Android/chroot setup (the old track, Python/GTK) |
| `archive/pc_work/` | **All PC-side scripts** from the whole project (historical; contains no secrets and no device data) |
| `tools/` | Helper tools (e.g. `pack_secrets.ps1`) |

## What is NOT in the repository (and must not be)
Images (`*.img`), partition dumps (`backup*`, they contain the IMEI, calibration data and MAC addresses), keys in clear
text, vendor binaries (GPU libraries and firmware). Phone backups live on the development PC, outside the repository.

## Safety rules (critical)
1. Write **only** to the `boot` partition (and only through BROM). Never to `nvdata`, `nvram`, `nvcfg`, `proinfo`, `protect*`, `preloader`, `lk`.
2. **Do not run `nvram_daemon`** before a verified NVRAM backup exists: it writes the IMEI and calibration data.
3. A restorable `boot` backup must exist before every test (see `docs/HANDOFF.md`, section "Recovery").
4. Never paste secrets into a chat or an issue.

## License
Our own code is released under the MIT License (see [`LICENSE`](LICENSE)). Patches against the Linux kernel remain
under GPL-2.0, and third-party components keep their own licenses.
