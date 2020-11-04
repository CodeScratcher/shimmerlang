#!/bin/sh
build/shimmerlang to_dump.shmr | xxd > result.txt
build/shimmerlang to_dump.shmr | xxd > result2.txt
diff result.txt result2.txt