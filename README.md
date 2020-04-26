# Coral Visual Studio C++ Solution
Minimal Visual Studio C++ solution template for Google Coral development on Windows.

To compile the console application, make sure you hydrate the TensorFlow submodule and create an NTFS symlink to the tensorflow/tensorflow directory.

On the repository root:
```
git submodule update --init --recursive
wsl ./third-party/tensorflow/tensorflow/lite/tools/make/download_dependencies.sh
make-ntfs-junctions.bat
```
