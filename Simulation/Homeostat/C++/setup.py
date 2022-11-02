from pybind11.setup_helpers import Pybind11Extension, build_ext

from setuptools import setup

setup(
    name="homeostat",

    ext_modules=[Pybind11Extension("homeostat", ["main.cpp"])],
    cmdclass={"build_ext": build_ext}
)

