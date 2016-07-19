import os
import distutils.core

EXTRA_COMPILE_ARGS = None
if "DEBUG" in os.environ:
    EXTRA_COMPILE_ARGS = ["-O0", "-pg"]


distutils.core.setup(
    name="pump",
    version="1.0",
    ext_modules=[
        distutils.core.Extension(
            "pump",
            sources = ['pump/pump.c'],
            include_dirs = ['pump', 'pump/utils', 'pump/serializers'],
            extra_compile_args=EXTRA_COMPILE_ARGS
        )
     ]
)
