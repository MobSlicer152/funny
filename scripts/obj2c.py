import argparse
import io
import os
import sys

from typing import Any, Callable


ObjVertex = tuple[float, float, float, float]
TextureCoordinate = tuple[float, float, float]
Normal = tuple[float, float, float]
ObjFace = tuple[tuple[int, int, int], tuple[int, int, int], tuple[int, int, int]]
Vertex = tuple[ObjVertex, TextureCoordinate, Normal]
Face = tuple[Vertex, Vertex, Vertex]


class OBJ:
    def __interpret(
        self, line: list[str]
    ) -> ObjVertex | TextureCoordinate | Normal | ObjFace:
        match line[0]:
            case "v":
                self.vertices.append(OBJ.__vertex(line))
            case "vt":
                self.texcoords.append(OBJ.__texcoord(line))
            case "vn":
                self.normals.append(OBJ.__normal(line))
            case "f":
                self.faces.append(OBJ.__faces(line))
            case "#":
                pass
            case _:
                print(f"ignoring unsupported command {line}")

    # TODO: make less redundant?

    def __vertex(line: list[str]) -> ObjVertex:
        match len(line):
            case 4:
                return (float(line[1]), float(line[2]), float(line[3]), 1)
            case 5:
                return (float(line[1]), float(line[2]), float(line[3]), float(line[4]))
            case _:
                raise f"malformed v entry {line}"

    def __texcoord(line: list[str]) -> TextureCoordinate:
        match len(line):
            case 2:
                return (float(line[1]), 0, 0)
            case 3:
                return (float(line[1]), float(line[2]), 0)
            case 4:
                return (float(line[1]), float(line[2]), float(line[3]))
            case _:
                raise f"malformed vt entry {line}"

    def __normal(line: list[str]) -> Normal:
        match len(line):
            case 4:
                return (float(line[1]), float(line[2]), float(line[3]))
            case _:
                raise f"malformed vn entry {line}"

    def __faces(line: list[str]) -> ObjFace:
        match len(line):
            case 4:
                # vertices start at 1
                return tuple(
                    [[int(i) - 1 for i in line[n].split("/")] for n in range(1, 4)]
                )
            case _:
                raise f"malformed f entry {line}"

    def __init__(self, name):
        lines = []
        with open(name, "rb") as f:
            # read lines, decode, split lines, and remove empties
            try:
                lines = list(
                    filter(
                        None,
                        [
                            list(filter(None, line.decode().split()))
                            for line in f.readlines()
                        ],
                    )
                )
            except ValueError:
                raise "malformed OBJ mesh"

        self.vertices = []
        self.texcoords = []
        self.normals = []
        self.faces = []
        [self.__interpret(line) for line in lines]


def clean_name(name: str) -> str:
    return "".join(filter(str.isidentifier, name))


def write_data(
    f: io.TextIOWrapper,
    list_var: str,
    count_var: str | None,
    data: list[Any],
    line_length: int,
    format: Callable[[Any], str],
):
    f.write(
        (f"const {count_var} = {len(data)};\n" if count_var else f"")
        + f"const {list_var} =\n"
        + f"{{\n"
        + f"\t"
    )

    text = ""
    current_length = 0
    for datum in data:
        current_length += 1
        text += f"{format(datum)},"
        if current_length >= line_length:
            text += "\n"
            current_length = 0
        else:
            text += " "

    f.write(text.strip())

    f.write(f"\n" + f"}};\n" + f"\n")


def main(argc: int, argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        prog="obj2c", description="convert OBJ meshes into C sources"
    )
    parser.add_argument("input")
    parser.add_argument("-f", "--force", action="store_true")
    parser.add_argument("-s", "--symbol")
    parser.add_argument("-o", "--output")

    args = parser.parse_args()

    mesh = args.input
    symbol = (
        args.symbol
        if args.symbol != None
        else clean_name(os.path.splitext(os.path.basename(mesh))[0]).upper()
    )
    output = (
        args.output
        if args.output != None
        else f"{os.path.join(os.path.dirname(mesh), symbol.lower())}.h"
    )
    force = args.force == True

    if os.path.exists(output):
        print(f"{output} will be overwritten", end="\n" if force else "")
        if not force:
            choice = input(', type "yes" to continue: ')
            if len(choice) >= 1 and choice[0].lower() != "y":
                print("not overwriting")
                exit(1)

    obj = OBJ(mesh)

    faces = []
    for face in obj.faces:
        faces.append(
            (
                (
                    obj.vertices[face[0][0]],
                    obj.texcoords[face[0][1]],
                    obj.normals[face[0][2]],
                ),
                (
                    obj.vertices[face[1][0]],
                    obj.texcoords[face[1][1]],
                    obj.normals[face[1][2]],
                ),
                (
                    obj.vertices[face[2][0]],
                    obj.texcoords[face[2][1]],
                    obj.normals[face[2][2]],
                ),
            )
        )

    f = open(output, "w")
    f.write(
        f"/* generated from {mesh} by bmp2c.py */\n"
        + f"\n"
        + f"#pragma once\n"
        + f"\n"
        + f'#include "kernel/math/vector.h"\n'
        + f'#include "kernel/raster/raster.h"\n'
        + f"\n"
        + f"#ifdef {symbol}_DEFINE_DATA\n"
        + f"extern const unsigned int {symbol}_FACE_COUNT;\n"
        + f"extern const Face_t {symbol}_FACE[];\n"
        + f"#else\n"
    )

    write_data(
        f,
        f"Face_t {symbol}_FACES[]",
        f"unsigned int {symbol}_FACE_COUNT",
        faces,
        1,
        lambda f: f"\t{{\n"
        + f"\t\t[0] = {{.pos = {{{f[0][0][0]}, {f[0][0][1]}, {f[0][0][2]}, {f[0][0][3]}}}, .uv = {{{f[0][1][0]}, {f[0][1][1]}}}, .nrm = {{{f[0][2][0]}, {f[0][2][1]}, {f[0][2][2]}}}}},\n"
        + f"\t\t[1] = {{.pos = {{{f[1][0][0]}, {f[1][0][1]}, {f[1][0][2]}, {f[1][0][3]}}}, .uv = {{{f[1][1][0]}, {f[1][1][1]}}}, .nrm = {{{f[1][2][0]}, {f[1][2][1]}, {f[1][2][2]}}}}},\n"
        + f"\t\t[2] = {{.pos = {{{f[2][0][0]}, {f[2][0][1]}, {f[2][0][2]}, {f[2][0][3]}}}, .uv = {{{f[2][1][0]}, {f[2][1][1]}}}, .nrm = {{{f[2][2][0]}, {f[2][2][1]}, {f[2][2][2]}}}}},\n"
        + f"\t}}",
    )

    f.write(f"#endif\n")
    f.close()

    return 0


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
