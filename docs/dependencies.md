# Dependencies

These are the dependencies used by Regus Core.
You can find installation instructions in the `build-*.md` file for your platform.
"Runtime" and "Version Used" are both in reference to the release binaries.

| Dependency | Minimum required |
| --- | --- |
| [Autoconf](https://www.gnu.org/software/autoconf/)
| [Automake](https://www.gnu.org/software/automake/)
| [Clang](https://clang.llvm.org)
| [GCC](https://gcc.gnu.org)
| [Python](https://www.python.org) (scripts, tests)
| [systemtap](https://sourceware.org/systemtap/) ([tracing](tracing.md))

## Required

| Dependency | Releases | Version used | Minimum required | Runtime |
| --- | --- | --- | --- | --- |
| [Boost](../depends/packages/boost.mk) | [link](https://www.boost.org/users/download/)
| [libevent](../depends/packages/libevent.mk) | [link](https://github.com/libevent/libevent/releases)
| glibc | [link](https://www.gnu.org/software/libc/)
| Linux Kernel | [link](https://www.kernel.org/)

## Optional

### GUI
| Dependency | Releases | Version used | Minimum required | Runtime |
| --- | --- | --- | --- | --- |
| [Fontconfig](../depends/packages/fontconfig.mk) | [link](https://www.freedesktop.org/wiki/Software/fontconfig/)
| [FreeType](../depends/packages/freetype.mk) | [link](https://freetype.org)
| [qrencode](../depends/packages/qrencode.mk) | [link](https://fukuchi.org/works/qrencode/)
| [Qt](../depends/packages/qt.mk) | [link](https://download.qt.io/official_releases/qt/)

### Networking
| Dependency | Releases | Version used | Minimum required | Runtime |
| --- | --- | --- | --- | --- |
| [libnatpmp](../depends/packages/libnatpmp.mk) | [link](https://github.com/miniupnp/libnatpmp/)
| [MiniUPnPc](../depends/packages/miniupnpc.mk) | [link](https://miniupnp.tuxfamily.org/)

### Notifications
| Dependency | Releases | Version used | Minimum required | Runtime |
| --- | --- | --- | --- | --- |
| [ZeroMQ](../depends/packages/zeromq.mk) | [link](https://github.com/zeromq/libzmq/releases)

### Wallet
| Dependency | Releases | Version used | Minimum required | Runtime |
| --- | --- | --- | --- | --- |
| [SQLite](../depends/packages/sqlite.mk) | [link](https://sqlite.org)
