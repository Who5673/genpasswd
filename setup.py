from setuptools import setup, find_packages

setup(
    name="genpasswd",
    version="1.0.6",
    author="who5673",
    author_email="windows7modrenedition@gmail.com",
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
        "Programming Language :: Python :: 3",
        "Operating System :: POSIX :: Linux",
        "Environment :: Console",
        "Topic :: Security :: Cryptography",
    ],
    platforms=["Linux"],
    python_requires=">=3.6",
    install_requires=[
        "rich>=13.0.0",
    ],
)
