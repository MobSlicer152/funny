import os
import struct
import sys


class BMP:
    MAGIC: bytes = b"BM"
    HEADER_SIZE: int = 40

    def __init__(self, name):
        with open(name, "rb") as f:
            size = f.seek(0, os.SEEK_END)
            if size < BMP.HEADER_SIZE:
                raise ValueError(
                    f"file is only {size} bytes, expected at least {BMP.HEADER_SIZE} bytes"
                )

            f.seek(0, os.SEEK_SET)

            self.magic: bytes = f.read(2)
            if self.magic != BMP.MAGIC:
                raise ValueError(
                    f"invalid magic, expected {BMP.MAGIC}, got {self.magic}"
                )

            self.size: int = struct.unpack("<I", f.read(4))[0]
            self.reserved1: int = struct.unpack("<H", f.read(2))[0]
            self.reserved2: int = struct.unpack("<H", f.read(2))[0]
            self.image_offset: int = struct.unpack("<I", f.read(4))[0]
            self.header_size: int = struct.unpack("<I", f.read(4))[0]
            self.width: int = struct.unpack("<i", f.read(4))[0]
            self.height: int = struct.unpack("<i", f.read(4))[0]
            self.color_planes: int = struct.unpack("<H", f.read(2))[0]
            self.color_depth: int = struct.unpack("<H", f.read(2))[0]
            if self.color_depth != 8:
                raise ValueError(f"unsupported color depth {self.color_depth[0]}")
            self.compression_method: int = struct.unpack("<I", f.read(4))[0]
            if self.compression_method != 0:  # BI_RGB, uncompressed
                raise ValueError(
                    f"unsupported compression method {self.compression_method[0]}, only uncompressed (BI_RGB) images are supported"
                )
            self.image_size: int = struct.unpack("<I", f.read(4))[0]
            self.horizontal_resolution: int = struct.unpack("<i", f.read(4))[0]
            self.vertical_resolution: int = struct.unpack("<i", f.read(4))[0]
            self.color_count: int = struct.unpack("<i", f.read(4))[0]
            self.important_colors: int = struct.unpack("<i", f.read(4))[0]

            # skip the palette
            f.seek(self.image_offset, os.SEEK_SET)
            self.image_data: bytes = f.read(self.image_size)

    def get_pixel(self, x: int, y: int) -> int:
        return 0


def clean_name(name: str) -> str:
    return ''.join(filter(str.isidentifier, name))


def main(argc: int, argv: list[str]) -> int:
    if argc < 2:
        print(f"usage: {argv[0]} <image> [output] [symbol prefix]")
        exit(1)

    image = argv[1]
    output = argv[2] if argc >= 3 else f"{image}.c"
    symbol = argv[3] if argc >= 4 else clean_name(image).upper()

    if os.path.exists(output):
        choice = input(f'{output} will be overridden, type "yes" if this is ok: ')
        if choice[0].lower() != "y":
            print("not overwriting")
            exit(1)

    f = open(output, "w")
    bmp = BMP(image)

    f.write(
        f"/* generated from {image} by bmp2c.py */\n" +
        f"\n" +
        f"#pragma once\n" +
        f"\n" +
        f"const unsigned int {symbol}_WIDTH = {bmp.width};\n" +
        f"const unsigned int {symbol}_HEIGHT = {bmp.height};\n" +
        f"\n" +
        f"const unsigned char {symbol}_DATA[{bmp.width * bmp.height}] =\n" +
        f"{{\n"
    )

    for y in range(0, bmp.height):
        for x in range(0, bmp.width):
            bmp.get_pixel(x, y)

    return 0


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
