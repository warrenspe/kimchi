import os
import distutils.core

DEBUG_COMPILE_ARGS = None
VERSION = "1.0"

if "DEBUG" in os.environ:
    DEBUG_COMPILE_ARGS = ['-O0', '-g', '-pedantic-errors', '-Wall', '-Wextra', '-Wmissing-prototypes',
                          '-Wstrict-prototypes', '-Wold-style-definition']


distutils.core.setup(
    name="pump",
    version=VERSION,
    description="An object serialization utility.",
    author="Warren Spencer",
    author_email="warrenspencer27@gmail.com",
    url="https://github.com/warrenspe/pump",
    download_url="https://github.com/warrenspe/pump/tarball/%s" % VERSION,
    keywords=['serialize', 'pickle'],
    classifiers=[],
    ext_modules=[
        distutils.core.Extension(
            "pump",
            sources = ['pump/pump.c'],
            include_dirs = ['pump', 'pump/utils', 'pump/serializers'],
            extra_compile_args=DEBUG_COMPILE_ARGS
        )
     ]
)
