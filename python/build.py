"""Script to build C extensions with poetry."""

import os

from setuptools.command.develop import develop
from setuptools.errors import CCompilerError, ExecError, PlatformError


class CustomDevelop(develop):
    def run(self):
        self.run_command("build_clib")
        super().run()


def build(setup_kwargs):
    setup_kwargs.update(
        {
            "cmdclass": {"develop": CustomDevelop},
        }
    )
