import os
import sys

def main(argc, argv):
    if argc < 3:
        print(f"Usage: {argv[0]} <output dir> <output image>")
        exit(1)

    outdir = argv[1]
    imagefile = argv[2]

    image = b""

    with open(os.path.join(outdir, "boot.bin"), "rb") as bootbin:
        image += bootbin.read()

    with open(os.path.join(outdir, "kernel.bin"), "rb") as kernelbin:
        image += kernelbin.read()

    with open(imagefile, "wb") as output:
        output.write(image)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
