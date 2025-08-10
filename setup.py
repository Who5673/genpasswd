from setuptools import setup, find_packages
from subprocess import check_output as c

# ./bin -> /bin
# ./usr/bin -> /usr/bin
# ./man -> /usr/share/man/man1
# ./<module> -> /usr/lib/python3.X/site-packages/<module>
# In ./<module>:
# __init__.py -> To scan that it was a module
# __main__.py -> Python logic
# <module>.so -> Shared Object (Shared Library)

user = c("whoami", shell=True)
if (user != "root"): raise PermissionError("This setup.py needs to run as root for establishing genpasswd app into your system")
setup(
    name="genpasswd",
    version="1.0.6",
    author="who5673",
    author_email="windows7modrenedition@gmail.com",
    data_files=[
        ('bin', ['bin/genpasswd']),
        ('bin', ['bin/genpasswdtui']),
        ('share/man/man1', ['man/genpasswdtui.1.gz']),
    ],
    include_packages_data=True,
    description="Generate strong passwords without using the Internet. Use the file system to save generated passwords.",

long_description=open("README.md").read(),
    long_description_content_type="text/markdown",

url="https://github.com/users/Who5673/genpasswdtui",
    packages=find_packages(),
    entry_points={
        'console_scripts': [
            'genpasswd = genpasswd.__main__:main'
            'genpasswdtui = genpasswdtui.__main__:main'
        ]
    },
    classifiers=[
        "Programming Language :: C",
        "Programming Language :: Python :: 3",
        "Operating System :: POSIX :: Linux",
        "Environment :: Console",
        "Development Status :: 1 - Planning",
        "Topic :: Utilities",
        "License :: OSI Approved :: MIT License",
    ],
    platforms=["Linux"],
    python_requires=">=3.6",
    install_requires=[
        "rich>=13.0.0",
    ],
)
