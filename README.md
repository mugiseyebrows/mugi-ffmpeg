# mugi-ffmpeg

Gui for ffmpeg to simplify transcoding and embeding audio / subtitles in mkv videos

![mugi-ffmpeg](https://mugiseyebrows.github.io/img/mugi-ffmpeg-enqueue.png)
![mugi-ffmpeg](https://mugiseyebrows.github.io/img/mugi-ffmpeg-run.png)

## Build 

### Windows

1) Install qt5 from [www.qt.io](https://www.qt.io/download) ([direct link](http://master.qt.io/archive/qt/5.11/5.11.3/qt-opensource-windows-x86-5.11.3.exe))
2) Include compiler and qt path to `%PATH%` env variable
3) In cmd run

```bash
git clone git@github.com:mugiseyebrows/mugi-ffmpeg.git
cd mugi-ffmpeg
qmake
make release
```

### Linux (Ubuntu)

In terminal run

```bash
sudo apt install build-essential qtbase5-dev git
git clone git@github.com:mugiseyebrows/mugi-ffmpeg.git
cd mugi-ffmpeg
qmake -qt=5 'CONFIG+=release'
make
make clean
sudo make install # installs to /usr/local/bin
python desktop.py # copies icons, creates desktop file
```