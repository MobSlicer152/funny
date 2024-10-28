import os
import sys

def main(argc, argv):
    if argc < 3:
        print(f"Usage: {argv[0]} <bootsector> <kernel binary> <output image>")
        exit(1)

    boot = argv[1]
    kernel = argv[2]
    imagefile = argv[3]

    image = b""

    with open(boot, "rb") as bootbin:
        image += bootbin.read()

    with open(kernel, "rb") as kernelbin:
        image += kernelbin.read()

    image += b"\x00" * (0x6200 - len(image))

    with open(imagefile, "wb") as output:
        output.write(image)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
