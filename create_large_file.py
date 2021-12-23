#! /usr/bin/env/ python3

with open("MyF.txt", "w") as out:
	out.seek(( 1024 * 1024 *2) - 1)
	out.write('\0')

