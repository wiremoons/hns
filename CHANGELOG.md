# Change Log for HNS

## v0.5.11 - 17 May 2024
- Update copyright year range in the `LICENSE` file.
- Update `vcpkg.json` to latest baseline and package versions.
- Improve with `std::optional` the usage of `getMaxID()` function.
- Update **stats** output to show the number of articles remaining to scan.
- Add *clangd* `.cache` direcory to `.gitignore` file.
- Show scanned article total count on *Last Checked* loop output.

## v0.5.10 - 12 Aug 2023
- Remove use of CMake `fetchContent` fall back - now only use `vcpkg`.

## v0.5.9 - 09 Aug 2023
- Fix typo in application Github repo url in 'project' section of `CMakeLists.txt`.
- remove the use of `std::move()` for function returns as they are not required as explicit statements.
- remove any comments that state the obvious for the source code they were supporting.

## v0.5.8 - 08 Aug 2023
- add `CHANGELOG.md` to project.
- update / correct several comments for clarity.
- add 'TODO' for application name parsing.
- remove two debug ouputs in `main()` as available in application version output anyway.
- move declaration of `base_url` to section at start of `main()`.
