import os
import distutils.core

DEBUG_COMPILE_ARGS = None
VERSION = "2.0.1"
NAME = "kimchi"

if "DEBUG" in os.environ:
    DEBUG_COMPILE_ARGS = ['-O0', '-g', '-pedantic-errors', '-Wall', '-Wextra', '-Wmissing-prototypes',
                          '-Wstrict-prototypes', '-Wold-style-definition']


distutils.core.setup(
    name=NAME,
    version=VERSION,
    description="A built-in object serialization utility.",
    author="Warren Spencer",
    author_email="warrenspencer27@gmail.com",
    url="https://github.com/warrenspe/%s" % NAME,
    download_url="https://github.com/warrenspe/%s/tarball/%s" % (NAME, VERSION),
    keywords=['serialize', 'pickle'],
    classifiers=[],
    license="https://www.gnu.org/licenses/gpl-3.0.html",
    platforms=["Linux", "Windows"],
    ext_modules=[
        distutils.core.Extension(
            NAME,
            sources = ['{0}/{0}.c'.format(NAME)],
            include_dirs = [NAME],
            extra_compile_args=DEBUG_COMPILE_ARGS
        )
     ]
)
