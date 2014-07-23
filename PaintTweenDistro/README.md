# PaintTween

This project is provided by the authors of paper <a href="http://graphics.pixar.com/library/ByExampleStylization/index.html" target=_blank>Stylizing Animation By Example (SIGGRAPH 2013)</a>.

I migrated it to newer APIs on OS X 10.8 & 10.9.

## What I modified comparing to original project
Last update: *2014/07/22*
### Compatible with CUDA >= 5
The <a href="http://developer.download.nvidia.com/compute/cuda/5_0/rel/docs/CUDA_Toolkit_Release_Notes_And_Errata.txt" target=_blank>CUDA 5 release notes</a> read:

> The use of a character string to indicate a device symbol, which was possible with certain API functions, is no longer supported. Instead, the symbol should be used directly.

The constructor of `CudaImagePyramidHost` and `CudaTextureHost` was modified to suit this new feature and the declaration of `CUDA_IMAGE_PYRAMID`, `CUDA_IMAGE_PYRAMID_LAYERED` and `CUDA_TEXTURE` was modified based on the modification of the constructor of 2 hosts above.

The member `_texture_name` was modified to `void *`, and storing the pointer to corresponding texture variable.

The function `allocateGaussianKernel` in `convolution.cu` was also modified: the parameter `device_name` was changed to `void *`.

All usages of a character string to indicate a device symbol were modified to use the symbol directly.

## Dependency

### Hardware

Name | Model | My Model |
--- | --- | --- |
CPU | x64 | Intel Core i7-4850HQ @ 2.3GHz |
GPU | NVIDIA CUDA6-Enabled GPUs, compute capability >= 2.0 | NVIDIA GeForce GT 750M @ 2GB GDDR5 |

### Software

Name | Version | My Version |
--- | --- | --- |
OS X | >=10.8 | 10.9.4 |
CMake | All is well | 2.8.12.2 |
GCC | 4.7.x | Homebrew gcc47 4.7.3 |
CUDA 6 Production Release | >=6.0 | 6.0.37 |
Qt4 | >=4.7 | 4.8.6 |
OpenEXR | >=1.6 | 2.1.0 |

#### Notes

* **Do not use `clang`** and use `gcc` instead. For CUDA6 compatibility, gcc4.7 is recommended.
* **Do not use Qt5** and use Qt4 instead. The Qt provided in Homebrew work.
* **Do not use OpenEXR provided in Homebrew**. You must download from openexr.com and compile it using gcc47.

#### Details
##### Install gcc47
```sh
brew install gcc47
```

After a long time, it will be installed in `/usr/local/Cellar/gcc47` and there will be links in `/usr/local/bin`. For next step, you can use the following environment variables:

```sh
export CC=/usr/local/bin/gcc-4.7
export CXX=/usr/local/bin/g++-4.7
```

##### Install Qt4
```sh
brew install qt
```

For the moment, it will install Qt 4.8.6 in `/usr/local/Cellar/qt`.

If you want to use the `qmake`  in Qt Creator, you should install Qt Creator and do some settings.

Luckily, thanks to `cmake`, we can compile the whole project without complex configurations in Qt Creator.

##### Install OpenEXR
1. Using the environment variables of `CC` and `CXX` provided before.
2. Download openexr-2.1.0 and ilmbase-2.1.0 from openexr.com .
3. Unzip them.
4. `cd ilmbase-2.1.0; ./configure; make; make install`
5. `cd openexr-2.1.0; ./configure; make; make install`

## Compile and run
```sh
export CC=/usr/local/bin/gcc-4.7
export CXX=/usr/local/bin/g++-4.7
export DYLD_LIBRARY_PATH=/usr/local/cuda/lib:$DYLD_LIBRARY_PATH
export PATH=/usr/local/cuda/bin:$PATH
cmake .
make
./bin/PaintTweenGUI
```
