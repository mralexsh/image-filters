#!/bin/sh

mogrify -format ppm $1
./image-filters ${1%.*}.ppm

for f in $(ls filtered_image*.ppm); 
do 
	mogrify -format jpg $f
done;

rm filtered_image*.ppm
rm ${1%.*}.ppm
