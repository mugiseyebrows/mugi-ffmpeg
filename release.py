
from __future__ import print_function, unicode_literals

from mugidelivery import Manager

import os

plugins = [p for p in [
    'C:\\Qt5\\5.11.1\\mingw53_32\\plugins',
    'D:\\qt\\Qt5.11.3\\5.11.3\\mingw53_32\\plugins'
] if os.path.exists(p)]

conf = {
    'AppName' : 'mugi-ffmpeg',
    'AppVersion' : '1.1',
    'Binaries' : [{'Source': 'D:\\dev\\mugi-ffmpeg\\release\\mugi-ffmpeg.exe', 'Dest': ''}],
    'Data' : [],
    'BuildDir' : 'D:\\dev\\mugi-ffmpeg',
    'BuildActions': [{
        'cmds': [
            ['qmake'],
            ['mingw32-make.exe', 'clean'],
            ['mingw32-make.exe', 'release', '-j4']
        ],
        'cwd': 'D:\\dev\\mugi-ffmpeg'
    }],
    'Arch': 'win32',
    'VersionUpdater': 'QtVersionUpdater',
    'Builder': 'QtBuilder',
    'Packer': 'SimplePacker',
    'VersionHeader': 'D:\\dev\\mugi-ffmpeg\\src\\version.h',
    'QtPlugins': ['qwindows','qwindowsvistastyle'],
    'QtPluginsSource': plugins[0]
}

if os.path.exists('D:\\Qt\\Qt5.11.3\\5.11.3\\mingw53_32\\bin'):
    os.environ['PATH'] = 'D:\\Qt\\Qt5.11.3\\5.11.3\\mingw53_32\\bin;D:\\Qt\\Qt5.11.3\\Tools\\mingw530_32\\bin;C:\\qwt6\\lib;C:\\lib-x86;C:\\windows\\system32'

m = Manager(conf)
#m.updateVersion()
m.build()
m.pack()
m.release()