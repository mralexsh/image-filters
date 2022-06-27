# Simple algorithms for image filtering.
It takes a raw image as an array of pixels. Each pixel consists of three bytes. It can be one byte per pixel too, but it's not tested. In order to make the code simple, the input file must be [.ppm](http://davis.lbl.gov/Manuals/NETPBM/doc/ppm.html) format. But it can be tedious to make .pmm files every time you want to apply the filters. [ImageMagick](https://imagemagick.org/) helps with conversion automation.  Also the code was tested only with the GCC compiler. You must install [mingw](https://www.mingw-w64.org/) on Windows.
## Quick start
```
  $ sudo apt install imagemagick 
  $ git clone https://github.com/mralexsh/image-filters
  $ cd image-filters
  $ make
  $ cp some_file.jpg .
  $ ./run.sh some_file.jpg
```
The result may looks like this
```
$ ./run.sh 123.jpg
Filter #1. Execution time: 0.203000
Filter #2. Execution time: 0.187000
Filter #3. Execution time: 1.922000
Filter #4. Execution time: 0.156000
Filter #5. Execution time: 0.172000
Filter #6. Execution time: 0.172000
Total time spent:2.812000
```
It generates ```filtered_image[1..6].jpg``` files

## TODO: Describe each filter
