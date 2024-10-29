import os
import sys

# 1 sector for boot, 80h for kernel
IMAGE_SIZE = 0x200 * 0x81

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

    image += b"\x00" * (IMAGE_SIZE - len(image))

    with open(imagefile, "wb") as output:
        output.write(image)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
