[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/hyperium/hyper/master/LICENSE) [![CodeQL](https://github.com/wiremoons/hns/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/wiremoons/hns/actions/workflows/codeql-analysis.yml) [![hns-build](https://github.com/wiremoons/hns/actions/workflows/hns-build.yml/badge.svg)](https://github.com/wiremoons/hns/actions/workflows/hns-build.yml)

# hns
CLI app to monitor and extract the latest Hacker News stories.

The official web site for [Hacker News](https://news.ycombinator.com/news) can
also show the latest new articles published if preferred.

## Executing the program

The application is command line application that is run from a terminal program on *macOS*, *Linux*, or *Windows*. Example of the basic output from executing the program `nhs` is as below. If supported by the terminal, colour output is included.

```console
./bin/hns 

Starting Hacker News Stream (hns)...
Starting article ID: 31546834
Waiting for new HN stories... checking every 2 minutes

    Title:      'UN Warns of ‘Total Societal Collapse’ Due to Breaching of Planetary Boundaries'
    NH Link:     https://news.ycombinator.com/item?id=31546839
    Story URL:   https://bylinetimes.com/2022/05/26/un-warns-of-total-societal-collapse-due-to-breaching-of-planetary-boundaries/
    Posted by:  'chewymouse' at 'Sun 29 May 2022 @ 07:08'
    Stats:      '1' displayed. '4' omitted. '5' total scanned.

    Title:      'AWS Graviton3 Hits GA with 3 Sockets per Motherboard Designs'
    NH Link:     https://news.ycombinator.com/item?id=31546847
    Story URL:   https://www.servethehome.com/amazon-aws-graviton3-hits-ga-with-3-sockets-per-motherboard-designs-tri-socket-arm/
    Posted by:  'mpweiher' at 'Sun 29 May 2022 @ 07:10'
    Stats:      '2' displayed. '11' omitted. '13' total scanned.

    Title:      'Distribution and diversity of Central African hunter-gatherers'
    NH Link:     https://news.ycombinator.com/item?id=31546870
    Story URL:   https://www.pnas.org/doi/10.1073/pnas.2113936119
    Posted by:  'bryanrasmussen' at 'Sun 29 May 2022 @ 07:14'
    Stats:      '3' displayed. '33' omitted. '36' total scanned.

Last check: 10:09:23
```

Example output with run with either `-v` or `--version`:
```console
% hns --version

'./hns' version is: '0.5.4'
Compiled on: 'Jul 23 2022 @ 15:35:52'.
Copyright (c) 2022 Simon Rowe.

C++ source built as 'Release' using compiler 'clang 13.1.6 (clang-1316.0.21.2.5)'.

Included library versions:
- cpr version: '1.9.0'
- Curl library version: '7.84.0-DEV'
- fmt version: '80101'
- nlohmann_json version: '3.10.5'
- spdlog version: '11000'

For licenses and further information visit:
- Hacker News Stream (hns):   https://github.com/wiremoons/hns
- argparse:                   https://github.com/p-ranav/argparse
- curl:                       https://github.com/curl/curl
- Curl for People (cpr):      https://github.com/libcpr/cpr
- fmt:                        https://github.com/fmtlib/fmt
- nlohmann_json:              https://github.com/nlohmann/json
- spdlog:                     https://github.com/gabime/spdlog

```

Example output with run with either `-h` or `--help`:
```console
% hns -h
Usage: hns [options] 

Hacker News Stream (HNS) obtains the latest stories from the Hacker News API.

Optional arguments:
-h --help       shows help message and exits
-v --version    prints version information and exits


```

## Build

A C++ compiler is required to build `hns`. 

- `cmake` is recommended to perform the build.
- `vcpkg` is recommended to perform the config and package management. 

If the computer the build is being performed on has `vcpkg` available, then it will be 
used for building the library dependencies. Otherwise `cmake` will use its 
*fecthContent* feature instead.

In order to build using `vcpkg` or `cmake` directly, clone this repo and then run:

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The resulting program is in located the `./bin/` sub-directory.

To detect and use `vcpkg` the `cmake` process looks for the environment variable `VCPKG_ROOT`. Set this to the location of `vcpkg` installation. For assistance with installing `vcpkg` see: [Get started with vcpkg](https://vcpkg.io/en/getting-started.html).
```
# Most Unix (Linux, macOS, etc):
export VCPKG_ROOT=~/.vcpkg
```

### Fedora Asahi - Build Issue

When excuting the build as decribed above on *Fedora Asahi* for *aarch64* the `vcpkg` build fails on the *OpenSSL* library. To fix it addtional *Perl* modules are needed, that can be installed with the command:

```
sudo dnf install perl-IPC-Cmd perl-FindBin
```

## Library Dependencies

The following libraries are used to build `hns`. They are included statically by default, and the dependency 
management is managed at build time either via `vcpkg` or `cmake`.

- argparse: https://github.com/p-ranav/argparse
- curl: https://github.com/curl/curl
- Curl for People (cpr): https://github.com/libcpr/cpr
- fmt: https://github.com/fmtlib/fmt
- nlohmann_json: https://github.com/nlohmann/json
- spdlog: https://github.com/gabime/spdlog

Thank you to the authors of the above libraries for making them available as open source.

## License

The `hns` application is provided under the _MIT open source license_. A copy
of the MIT license file is [here](./LICENSE).

The [Hacker News API](https://github.com/HackerNews/API) is provided under the
_MIT_ open source license. A copy of the license file is
[here](https://github.com/HackerNews/API/blob/665205f324b95f60bc7889b543978f728c274c4a/LICENSE).
