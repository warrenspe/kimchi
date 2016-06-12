from distutils.core import setup, Extension
setup(
    name="pump",
    version="1.0",
    ext_modules=[
        Extension(
            "pump",
            sources = ['pump/pump.c'],
            include_dirs = ['pump', 'pump/utils', 'pump/serializers'],
            extra_compile_args=["-O0", "-g"] # TODO
        )
     ]
)
