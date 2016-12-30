# xmonitor

Simple utility deamon that listens to `XRRScreenChangeNotifyEvent` and runs scripts when external monitors are attached or detached.

Scripts are searched for in `~/.screenlayout/attach.d` or `~/.screenlayout/detach.d`; these directories are run with `run-parts`.

# Usage

After you git-clone this repository run

``` shell
make
```

Place your scripts in `~/.screenlayout/<event>.d` (where `<event>` is either `attach` or `detach`), then run

```bash
./xmonitor
```
