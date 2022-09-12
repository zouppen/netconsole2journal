# netconsole2journal

Reads
[netconsole](https://www.kernel.org/doc/Documentation/networking/netconsole.txt)
output and writes to
[systemd-journald](https://www.freedesktop.org/software/systemd/man/systemd-journald.service.html).

## Motivation

Logging of network booting workstations.

## Requirements

- Linux
- glib
- cmake
- systemd

## Building

Install dependencies. In Debian based systems for example:

```sh
sudo apt install cmake libglib2.0-dev
```

And build:

```sh
mkdir build
cd build
cmake ..
```

## Usage

You can try by running it via test wrapper and listen to connections from port 1234:

```sh
systemd-socket-activate -d -l 1234 ./netconsole2journal
```

Then write to there using nc:

```sh
nc -u localhost 1234
```

And follow logs while you do it, with some filtering:

```sh
journalctl -fn0 -o export -t netconsole2journal
```

To deploy this properly, use
[systemd.socket](https://www.freedesktop.org/software/systemd/man/systemd.socket.html)
for activating it.

## Status

Reads datagrams via socket activation and writes to systemd-journald. Consider it usable.

## Author

Zouppen / Joel Lehtonen
