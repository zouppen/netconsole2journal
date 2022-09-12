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

## Status

WIP

## Author

Zouppen / Joel Lehtonen
