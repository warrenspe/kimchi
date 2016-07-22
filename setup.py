import os
import distutils.core

DEBUG_COMPILE_ARGS = None
if "DEBUG" in os.environ:
    DEBUG_COMPILE_ARGS = ['-O0', '-g', '-pedantic-errors', '-Wall', '-Wextra', '-Wmissing-prototypes',
                          '-Wstrict-prototypes', '-Wold-style-definition']


distutils.core.setup(
    name="pump",
    version="1.0",
    ext_modules=[
        distutils.core.Extension(
            "pump",
            sources = ['pump/pump.c'],
            include_dirs = ['pump', 'pump/utils', 'pump/serializers'],
            extra_compile_args=DEBUG_COMPILE_ARGS
        )
     ]
)
