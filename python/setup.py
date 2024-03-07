import os

from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            "python.speakeasy2._speakeasy2",
            sources=["python/speakeasy2/_speakeasy2.c"]
            + [
                os.path.join("src/speakeasy2", src)
                for src in os.listdir("src/speakeasy2")
                if src.endswith(".c")
            ],
            include_dirs=[
                "include",
                "build/include",
                "src/speakeasy2",
                "vendor/igraph/include",
                "build/vendor/igraph/include",
                "vendor/python-igraph/src/_igraph",
            ],
            libraries=["igraph"],
            extra_compile_args=["-fopenmp"],
            extra_link_args=["-fopenmp", "-Lbuild/vendor/igraph/src"],
        )
    ]
)
