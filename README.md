# Switch server plugin

## Usage
To use this plugin, you need to build it.

Requirements:
* CMake
* Photon
* Libuv

Make sure that the CMake file is including and linking to the libraries. You are supposed to link against the Photon shared library that you
built.

Once built, put the shared library (.dll on Windows) (.so on Linux) inside the Photon plugins folder and start/restart the proxy.
If all works correctly, it is supposed to say "Loaded server switch plugin!" once Photon starts.