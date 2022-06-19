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

## Build

A C++ compiler is required to build `hns`. 

`cmake` is recommended to perform the build. 

If the computer the build is being performed on has `vcpkg` available, then it will be 
used for building the library dependencies. Otherwise `cmake` will use its 
*fecthContent* feature instead.

In order to build using `vcpkg` or `cmake` directly, clone this repo and then run:

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The resulting program is in located the `./bin/` sub-directory.

## License

The `hns` application is provided under the _MIT open source license_. A copy
of the MIT license file is [here](./LICENSE).

The [Hacker News API](https://github.com/HackerNews/API) is provided under the
_MIT_ open source license. A copy of the license file is
[here](https://github.com/HackerNews/API/blob/665205f324b95f60bc7889b543978f728c274c4a/LICENSE).