#!/bin/bash

input="$1"
face_size=512

directory="${input%.*}"
mkdir "$directory"
magick "$input" -crop ${face_size}x${face_size}+$((face_size))+$((face_size)) +repage "${directory}/front.jpg"
magick "$input" -crop ${face_size}x${face_size}+0+$((face_size)) +repage "${directory}/left.jpg"
magick "$input" -crop ${face_size}x${face_size}+$((face_size))+0 +repage "${directory}/top.jpg"
magick "$input" -crop ${face_size}x${face_size}+$((face_size))+$((2*face_size)) +repage "${directory}/bottom.jpg"
magick "$input" -crop ${face_size}x${face_size}+$((3*face_size))+$((face_size)) +repage "${directory}/back.jpg"
magick "$input" -crop ${face_size}x${face_size}+$((2*face_size))+$((face_size)) +repage "${directory}/right.jpg"
