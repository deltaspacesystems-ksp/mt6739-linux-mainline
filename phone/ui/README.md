# MICROFW desktop — native C/C++ DE

Target: a compact English desktop for DURA-L21, ordinary overlapping windows, fullscreen applications by default, standard compositor decorations on restore, edge gestures and multitouch. No phone home-screen grid. Current native runtime is written in C; C++ can use the public C gesture interface. Python is used only for PC/build utilities.

## Implemented and tested on PC

`--desktop` runs the native shell against **labwc/wlroots**, with three real layer-shell surfaces: desktop background, status bar, bottom gesture area. It subscribes to foreign-toplevel management, tracks actual application titles/state/lifetime, requests initial fullscreen, and activates/restores/minimizes/closes real windows. Overview lists actual windows and offers activation/close. Standard maximize/minimize/close controls belong to the compositor, not a painted toolbar. Settings is another native C xdg-shell application; Terminal currently launches weston-terminal on the PC. Files is still a placeholder; general application discovery/launching is pending.

The regular xdg-shell mode remains a rendering/transport harness and Settings application. It is not the desktop compositor. labwc is the existing C stacking compositor for this stage; the project does not yet contain its own wlroots compositor implementation.

- Compact default scale1.0: 14px clock, 18px status icon canvas, 30px desktop shortcut glyphs. `--scale 0.85` reduces everything further. English labels throughout.
- All17 original project SVG files retained unchanged in assets/icons. Seven are translated from their original M/L/Q/Z curves into Cairo C; no runtime SVG interpreter is needed. Source was D:\y5_linux_work\guisrc\symbols; final4.4 status geometry is preserved at smaller scale.
- Cairo/Pango/pixman software path; shared-memory buffers, release tracking and per-surface frame callbacks. No idle60fps loop. Occluded background callbacks do not prevent raising the overview: commit the layer-role change before waiting for its presentation callback.
- Native tracker handles arbitrary contact IDs, multiple simultaneous contacts, cancellation and timestamp rollover. Bottom swipe -> desktop/minimize applications; slower bottom swipe or three-finger swipe up -> overview; two-finger downward swipe/top-left downward edge swipe -> leave fullscreen. Top-right edge opens Control Center. These bindings are preliminary.
- Edge gestures are received on shell-owned surfaces, with coordinates translated from the bar into output coordinates. Gestures starting inside another app require compositor input integration; a client cannot intercept another client's wl_touch stream. Physical multitouch is not tested: mainline touch/pinctrl/I2C remain hardware dependencies.
- Status and Settings read microfw sysfs. Unknown capacity stays unknown, never a fake voltage-derived percentage. `--demo` explicitly uses sample data for previews/PC tests only. Control Center sliders/toggles are currently visual, not hardware controls.

## Build/run

Working copy D:\y5_linux_work\microfw-ui; repo phone/ui. Dependencies: C compiler, pkg-config, Cairo, Pango, Wayland development files, wayland-scanner, wayland-protocols, Python3 for asset generation. Protocol XML sources and their original licenses are retained in protocols/. Build generates bindings and icon paths into build/.

```sh
make
make previews
make test                         # gesture UBSan + real headless Weston xdg-shell
make test-desktop                 # real layer-shell + applications on headless labwc
./build/microfw-shell --desktop   # inside a running labwc/wlroots session
./build/microfw-shell --windowed --page settings
./build/microfw-shell --demo --preview build/desktop.png --width 720 --height 1440
```

The current executable is x86_64/glibc for PC verification, **not** Alpine/aarch64. No UI/compositor has yet been installed or activated on the phone.

## Measured integration result

Full build with -Wall -Wextra -Werror PASS. UBSan contact tests PASS. Weston/pixman:720x1440, xdg configure, shared buffers and3frame callbacks PASS. labwc0.20.2/wlroots0.20.2/pixman: three layer surfaces and two real clients (native Settings and weston-terminal), server-acknowledged fullscreen -> overview actually presented above occluded apps -> restore -> minimize -> activate -> maximize -> unmaximize -> close PASS. Evidence: phone/ui-desktop-integration.txt. This test uses protocol acknowledgements and actual client destruction, not pictures of fake windows. Standard-titlebar button clicking and physical finger gestures remain manual tests.

PC dependencies: wayland1.26.0, cairo1.18.6, pango1.58.2, wayland-protocols1.49, weston15.0.1, labwc/wlroots0.20.2, DejaVu. Stale mirror404s for glycin/librsvg were resolved using the exact package versions and detached signatures from the official Arch archive, followed by normal pacman signature/integrity checks. No dependency verification was disabled and no blanket system upgrade was performed. Reproduction helper: archive/pc_work/fetch_librsvg_pc.py; package blobs/signatures stay outside Git.

## Next

Package for Alpine/aarch64; test real display recovery and fonts. Connect application discovery and file manager, real capture thumbnails, keyboard behavior, settings actuators and animation. Add compositor input policy for gestures within application content and transient-window rules (the bootstrap policy currently requests fullscreen for every new toplevel). Review status/home overlay insets: fullscreen clients may draw underneath the bars, while maximized clients respect exclusive zones. Test hot-unplug/cancel and surface lifecycle under real input. Resolve touch hardware and GPU/BVNC separately; no speculative GPU power/clock changes.

C/C++ ABI smoke test also passes: a C++17 consumer links to and calls the actual C model and contact implementation. No Python is involved in the UI runtime.

## Telemetry responsiveness (2026-10-25)
Phone-native mfw_sample blocks for50-56ms; periodic reads now run in a C pthread, outside the snapshot mutex. The UI copies telemetry only; window, gesture and Cairo/Wayland state remain on the UI thread. Initial startup sample remains synchronous. Run `make test-sampler` (UBSan), `make test-desktop`. Native worker test PASS, cached snapshot200reads avg347ns/max1616ns (not end-to-end UI latency). Deployed on v57 without reboot; physical responsiveness test pending. Preserve `build/microfw-shell.pre-async` on phone for rollback. Animations and compositor-wide application gestures still unfinished. Current Alpine/aarch64 deployment and physical touch supersede the older Next section above.

Physical owner feedback after async telemetry on v57: clearly better, gestures work. Six contact down/up sequences were logged; RDMA underflow/abnormal remained0 during this sample. This confirms this trial, not GPU acceleration, global gestures or elimination of all tearing.

## Page motion (PC stage)

NativeC cached420ms critically damped slide/parallax/dim with rounded incoming card. Monotonic time, frame callbacks, no idle animation, interruption continues from current composite; resize/cleanup bounded. Wired to xdg-shell page changes and desktop base layer, not foreign window opening/closing. Finger-driven progress, cancellation, direction-aware back transitions, actual compositor window transforms, blur/refraction remain pending. `make test-motion`: ASan/UBSan endpoint/retarget pixel continuity/resize/cleanup PASS. Weston/labwc protocol regressions PASS. No phone deployment or phone FPS measurement yet.
