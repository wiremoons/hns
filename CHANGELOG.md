## v0.5.10 - 12 Aug 2023
- Remove use of CMake `fetchContent` fall back to simplify - only use `vcpkg` instead.

## v0.5.9 - 09 Aug 2023
- Fix typo in application Github repo url in 'project' section of `CMakeLists.txt`.
- remove the use of `std::move()` for function returns as they are not required as explicit statements.
- remove any comments that state the obvious based on the source code they were supporting.

## v0.5.8 - 08 Aug 2023
- add `CHANGELOG.md` to project.
- update / correct several comments for clarity.
- add 'TODO' for application name parsing.
- remove two debug ouputs in `main()` as available in application version output anyway.
- move declaration of `base_url` to section at start of `main()`.
