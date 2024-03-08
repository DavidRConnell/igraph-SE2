"""Script to build C extensions with poetry."""

import os

from setuptools import Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.develop import develop
from setuptools.errors import CCompilerError, ExecError, PlatformError

extensions = [
    Extension(
        "speakeasy2._speakeasy2",
        sources=["speakeasy2/_speakeasy2.c"]
        + [
            os.path.join("../src/speakeasy2", src)
            for src in os.listdir("../src/speakeasy2")
            if src.endswith(".c")
        ],
        include_dirs=[
            "../include",
            "../build/include",
            "../src/speakeasy2",
            "../vendor/igraph/include",
            "../build/vendor/igraph/include",
            "../vendor/python-igraph/src/_igraph",
        ],
        libraries=["igraph"],
        extra_compile_args=["-fopenmp"],
        extra_link_args=["-fopenmp", "-L../build/vendor/igraph/src"],
    )
]


class ExtBuilder(build_ext):
    def run(self):
        try:
            build_ext.run(self)
        except FileNotFoundError:
            print("Failed to build C extension.")

    def build_extension(self, ext):
        try:
            build_ext.build_extension(self, ext)
        except (
            CCompilerError,
            ExecError,
            PlatformError,
            ValueError,
        ):
            print("Failed to build C extension.")


class CustomDevelop(develop):
    def run(self):
        self.run_command("build_clib")
        super().run()


def build(setup_kwargs):
    setup_kwargs.update(
        {
            "ext_modules": extensions,
            "cmdclass": {"develop": CustomDevelop},
        }
    )
