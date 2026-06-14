import sys
import struct
import zlib

MAGIC = 0x4C41574F


def read_png(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not a PNG")
    pos = 8
    w = h = depth = ctype = 0
    idat = bytearray()
    while pos < len(data):
        length = struct.unpack(">I", data[pos:pos + 4])[0]
        tag = data[pos + 4:pos + 8]
        payload = data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if tag == b"IHDR":
            w, h, depth, ctype = struct.unpack(">IIBB", payload[:10])
        elif tag == b"IDAT":
            idat += payload
        elif tag == b"IEND":
            break
    if depth != 8 or ctype != 6:
        raise ValueError("expected 8-bit RGBA")
    raw = zlib.decompress(bytes(idat))
    stride = w * 4
    out = bytearray(w * h * 4)
    prev = bytearray(stride)
    p = 0
    for y in range(h):
        ft = raw[p]
        p += 1
        line = bytearray(raw[p:p + stride])
        p += stride
        if ft == 1:
            for i in range(4, stride):
                line[i] = (line[i] + line[i - 4]) & 0xFF
        elif ft == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif ft == 3:
            for i in range(stride):
                a = line[i - 4] if i >= 4 else 0
                line[i] = (line[i] + ((a + prev[i]) >> 1)) & 0xFF
        elif ft == 4:
            for i in range(stride):
                a = line[i - 4] if i >= 4 else 0
                b = prev[i]
                c = prev[i - 4] if i >= 4 else 0
                pp = a + b - c
                pa, pb, pc = abs(pp - a), abs(pp - b), abs(pp - c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 0xFF
        out[y * stride:(y + 1) * stride] = line
        prev = line
    return w, h, out


def main():
    src = sys.argv[1] if len(sys.argv) > 1 else "assets/orbit_wallpaper.png"
    dst = sys.argv[2] if len(sys.argv) > 2 else "assets/orbit_wallpaper.bin"
    w, h, rgba = read_png(src)
    header = struct.pack("<III", MAGIC, w, h)
    header += b"\x00" * (512 - len(header))
    pixels = bytearray(w * h * 4)
    for i in range(w * h):
        r, g, b, a = rgba[i * 4], rgba[i * 4 + 1], rgba[i * 4 + 2], rgba[i * 4 + 3]
        struct.pack_into("<I", pixels, i * 4, (0xFF << 24) | (r << 16) | (g << 8) | b)
    pad = (-len(pixels)) % 512
    pixels += b"\x00" * pad
    with open(dst, "wb") as f:
        f.write(header)
        f.write(pixels)
    print("wrote", dst, w, "x", h, len(header) + len(pixels), "bytes")


if __name__ == "__main__":
    main()
