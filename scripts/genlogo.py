import sys
import struct
import zlib

SIZE = 200
SS = 4

THEME_WALL_MID = 0x152A52
THEME_ACCENT = 0x4C8DFF
HIGHLIGHT = 0xBFD6FF


def mix(a, b, t):
    out = 0
    for sh in (16, 8, 0):
        ca = (a >> sh) & 0xFF
        cb = (b >> sh) & 0xFF
        cv = (ca * (256 - t) + cb * t) >> 8
        out |= (cv & 0xFF) << sh
    return out


def rgb(c):
    return ((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF)


BODY = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 110))
GLOW = rgb(mix(THEME_WALL_MID, HIGHLIGHT, 140))
RING = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 80))


def coverage(px, py, cx, cy, r):
    hit = 0
    for sx in range(SS):
        for sy in range(SS):
            fx = px + (sx + 0.5) / SS
            fy = py + (sy + 0.5) / SS
            dx = fx - cx
            dy = fy - cy
            if dx * dx + dy * dy <= r * r:
                hit += 1
    return hit / (SS * SS)


def ring_cov(px, py, cx, cy, r_out, r_in):
    hit = 0
    for sx in range(SS):
        for sy in range(SS):
            fx = px + (sx + 0.5) / SS
            fy = py + (sy + 0.5) / SS
            d2 = (fx - cx) ** 2 + (fy - cy) ** 2
            if r_in * r_in <= d2 <= r_out * r_out:
                hit += 1
    return hit / (SS * SS)


def over(dst, src, a):
    return tuple(int(s * a + d * (1 - a) + 0.5) for s, d in zip(src, dst))


def build():
    cx = cy = SIZE / 2.0
    px = bytearray(SIZE * SIZE * 4)
    for y in range(SIZE):
        for x in range(SIZE):
            col = (0, 0, 0)
            alpha = 0.0

            cr = ring_cov(x, y, cx, cy, 92, 88)
            if cr > 0:
                col = over(col, RING, cr) if alpha > 0 else RING
                alpha = max(alpha, cr)

            cb = coverage(x, y, cx, cy, 56)
            if cb > 0:
                col = over(col, BODY, cb)
                alpha = alpha * (1 - cb) + cb

            cg = coverage(x, y, cx - 18, cy - 18, 18)
            if cg > 0:
                col = over(col, GLOW, cg)
                alpha = alpha * (1 - cg) + cg

            i = (y * SIZE + x) * 4
            px[i] = col[0]
            px[i + 1] = col[1]
            px[i + 2] = col[2]
            px[i + 3] = int(alpha * 255 + 0.5)
    return px


def write_png(path, w, h, rgba):
    rows = bytearray()
    stride = w * 4
    for y in range(h):
        rows.append(0)
        rows += rgba[y * stride:(y + 1) * stride]
    comp = zlib.compress(bytes(rows), 9)

    def chunk(tag, payload):
        out = struct.pack(">I", len(payload)) + tag + payload
        out += struct.pack(">I", zlib.crc32(tag + payload) & 0xFFFFFFFF)
        return out

    ihdr = struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", ihdr))
        f.write(chunk(b"IDAT", comp))
        f.write(chunk(b"IEND", b""))


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else "assets/orbit_logo.png"
    px = build()
    write_png(out, SIZE, SIZE, px)
    print("wrote", out, SIZE, "x", SIZE)


if __name__ == "__main__":
    main()
