import argparse
import math
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
                raise ValueError(f"unsupported color depth {self.color_depth}")
            self.compression_method: int = struct.unpack("<I", f.read(4))[0]
            if self.compression_method != 0:  # BI_RGB, uncompressed
                raise ValueError(
                    f"unsupported compression method {self.compression_method}, only uncompressed (BI_RGB) images are supported"
                )
            self.image_size: int = struct.unpack("<I", f.read(4))[0]
            self.horizontal_resolution: int = struct.unpack("<i", f.read(4))[0]
            self.vertical_resolution: int = struct.unpack("<i", f.read(4))[0]
            self.color_count: int = struct.unpack("<i", f.read(4))[0]
            self.important_colors: int = struct.unpack("<i", f.read(4))[0]

            # align to dword
            self.row_size: int = math.ceil(self.width / 4.0, ) * 4

            # skip the palette
            f.seek(self.image_offset, os.SEEK_SET)
            self.image_data: bytes = f.read(self.image_size)

    def get_pixel(self, x: int, y: int) -> int:
        return self.image_data[y * self.row_size + x]

def clean_name(name: str) -> str:
    return ''.join(filter(str.isidentifier, name))


def main(argc: int, argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        prog="bmp2c",
        description="convert indexed BMP images into C sources"
    )
    parser.add_argument("input")
    parser.add_argument("-f", "--force", action="store_true")
    parser.add_argument("-s", "--symbol")
    parser.add_argument("-o", "--output")

    args = parser.parse_args()

    image = args.input
    symbol = args.symbol if args.symbol != None else clean_name(os.path.splitext(os.path.basename(image))[0]).upper()
    output = args.output if args.output != None else f"{os.path.join(os.path.dirname(image), symbol.lower())}.h"
    force = args.force == True

    if os.path.exists(output):
        print(f"{output} will be overwritten", end="\n" if force else "")
        if not force: 
            choice = input(", type \"yes\" to continue: ")
            if len(choice) >= 1 and choice[0].lower() != "y":
                print("not overwriting")
                exit(1)

    bmp = BMP(image)

    f = open(output, "w")
    f.write(
        f"/* generated from {image} by bmp2c.py */\n" +
        f"\n" +
        f"#pragma once\n" +
        f"\n" +
        f"#ifndef {symbol}_DEFINE_DATA\n" +
        f"extern const unsigned int {symbol}_WIDTH;\n" +
        f"extern const unsigned int {symbol}_HEIGHT;\n" +
        f"extern const unsigned char {symbol}_DATA[{bmp.width * bmp.height}];\n" +
        f"#else\n" +
        f"const unsigned int {symbol}_WIDTH = {bmp.width};\n" +
        f"const unsigned int {symbol}_HEIGHT = {bmp.height};\n" +
        f"\n" +
        f"const unsigned char {symbol}_DATA[{bmp.width * bmp.height}] =\n" +
        f"{{\n" +
        f"\t"
    )

    line_length = 80 / len("0x00, ")
    pixels = ""
    current_length = 0
    for y in range(bmp.height - 1, 0, -1):
        for x in range(0, bmp.width):
            pixel = bmp.get_pixel(x, y)
            current_length += 1
            pixels += f"0x{pixel:02X},"
            if current_length >= line_length:
                pixels += "\n\t"
                current_length = 0
            else:
                pixels += " "

    pixels = pixels.strip()
    f.write(pixels)

    f.write(
        f"\n" +
        f"}};\n" +
        f"#endif\n"
    )

    return 0


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
