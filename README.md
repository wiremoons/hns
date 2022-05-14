[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/hyperium/hyper/master/LICENSE)

# hns
CLI app to monitor and extract the latest Hacker News stories.

The official web site for [Hacker News](https://news.ycombinator.com/news) can
also show the latest new articles published if preferred.

## Build

A C++ compiler is required to build `hns`. 

`cmake` is recommended to perform the build. 

In order to build using `cmake`, clone this repo and then run:

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The resulting program is in the `./bin/` sub-directory.

## License

The `hns` application is provided under the _MIT open source license_. A copy
of the MIT license file is [here](./LICENSE).

The [Hacker News API](https://github.com/HackerNews/API) is provided under the
_MIT_ open source license. A copy of the license file is
[here](https://github.com/HackerNews/API/blob/665205f324b95f60bc7889b543978f728c274c4a/LICENSE).