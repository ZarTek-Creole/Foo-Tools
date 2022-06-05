<span class="badge-opencollective"><a href="https://github.com/ZarTek-Creole/DONATE" title="Donate to this project"><img src="https://img.shields.io/badge/open%20collective-donate-yellow.svg" alt="Open Collective donate button" /></a></span>
[![CC BY 4.0][cc-by-shield]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg
- added date/time in foo-pre.log
- added mod_audiosort

For more details see [CHANGELOG](src/CHANGES)

## Documentation

- [README.1st](README.1st) Original README file
- [src/CHANGES](src/CHANGES) CHANGELOG
- [src/README.1st](src/README.1st) Building instructions
- [doc/HOWTO](doc/HOWTO) Notes about compiling
- [doc](doc) Documentation on installing/running the programs
- [README.mp3genre](src/pre/README.mp3genre) mp3genre specific README (install/configure)

### mp3genre

mp3genre for foo-pre; adds mp3 genre in PRE output without module

check README on how to install/configure

### Modules

Addons for foo-pre; modules add functions but are not build into foo-pre binary

- [src/pre/README.modules](src/pre/README.modules)
- [src/pre/modules/README.mod_audiosort](src/pre/modules/README.mod_audiosort)
- [src/pre/modules/README.mod_chmod](src/pre/modules/README.mod_chmod)
- [src/pre/modules/README.mod_idmp3](src/pre/modules/README.mod_idmp3)
- [src/pre/modules/README.mod_nfohandler](src/pre/modules/README.mod_nfohandler)
- [src/pre/modules/README.mod_sitenfoadd](src/pre/modules/README.mod_sitenfoadd)
- [src/pre/modules/README.mod_symlink](src/pre/modules/README.mod_symlink)

## TODO

### foo-pre

- [ ] replace pre/mp3genre by pzs-ng code or other id3 lib instead
- [ ] FLAC support?
- [X] [#5](https://github.com/glftpd/foo-tools/issues/5) add option to force using mv external 
- [X] add audiosort after pre
- [ ] prebw after pre (mod_prebw)

### spy

- [X] look into libhttpd.h since it's LGPL now (http://www.hughes.com.au/products/libhttpd)

