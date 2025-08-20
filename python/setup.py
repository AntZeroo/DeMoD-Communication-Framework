from setuptools import setup

setup(
    name='dcf-sdk-python',
    version='5.0.0',
    packages=['dcf'],
    install_requires=[
        'grpcio',
        'grpcio-tools',
        'protobuf',
        'curses'
    ],
    entry_points={
        'console_scripts': [
            'dcf = dcf.cli:main',
        ],
    },
)
