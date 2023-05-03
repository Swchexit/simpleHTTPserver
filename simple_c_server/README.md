



# How to compile?

```shell
cd source											# cd into directory

mkdir build										# create build directory and cd into it
cd build
cmake ../											# use camke to generate makefile

make													# make project
./introCN_Proj								# Then the server is listening on default port 6969
```

預設port為8000，可以改動main.c main function第一行來換一個port。
