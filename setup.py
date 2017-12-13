import os
import distutils.core

DEBUG_COMPILE_ARGS = None
if "DEBUG" in os.environ:
    DEBUG_COMPILE_ARGS = ['-O0', '-g', '-pedantic-errors', '-Wall', '-Wextra', '-Wmissing-prototypes',
                          '-Wstrict-prototypes', '-Wold-style-definition']

NAME = "kimchi"
VERSION = "2.0.2"

AUTHOR = "Warren Spencer"
EMAIL = "warrenspencer27@gmail.com"
URL = "https://github.com/warrenspe/%s" % NAME
LICENSE = "https://opensource.org/licenses/MIT"

PLATFORMS = ["Linux", "Windows"]
DESCRIPTION = "Package for serializing built in python objects"
KEYWORDS = ["filestore", "file", "storage", "db", "kimchi"]



distutils.core.setup(
    name=NAME,
    version=VERSION,
    description=DESCRIPTION,
    author=AUTHOR,
    author_email=EMAIL,
    url=URL,
    download_url="%s/tarball/%s" % (URL, VERSION),
    keywords=KEYWORDS,
    classifiers=[],
    license=LICENSE,
    platforms=PLATFORMS,
    ext_modules=[
        distutils.core.Extension(
            NAME,
            sources = ['{0}/{0}.c'.format(NAME)],
            include_dirs = [NAME],
            extra_compile_args=DEBUG_COMPILE_ARGS
        )
     ]
)
