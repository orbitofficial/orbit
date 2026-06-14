import sys
import struct
import zlib

W = 1024
H = 768
SS = 3

THEME_WALL_TOP = 0x0A1228
THEME_WALL_MID = 0x152A52
THEME_WALL_BOTTOM = 0x0B1A33
THEME_ACCENT = 0x4C8DFF


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


BODY = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 60))
GLOW = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 100))
RING1 = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 90))
RING2 = rgb(mix(THEME_WALL_MID, THEME_ACCENT, 45))


def disc_cov(px, py, cx, cy, r):
    hit = 0
    for sx in range(SS):
        for sy in range(SS):
            fx = px + (sx + 0.5) / SS
            fy = py + (sy + 0.5) / SS
            if (fx - cx) ** 2 + (fy - cy) ** 2 <= r * r:
                hit += 1
    return hit / (SS * SS)


def ring_cov(px, py, cx, cy, ro, ri):
    hit = 0
    for sx in range(SS):
        for sy in range(SS):
            fx = px + (sx + 0.5) / SS
            fy = py + (sy + 0.5) / SS
            d2 = (fx - cx) ** 2 + (fy - cy) ** 2
            if ri * ri <= d2 <= ro * ro:
                hit += 1
    return hit / (SS * SS)


def over(dst, src, a):
    return tuple(int(s * a + d * (1 - a) + 0.5) for s, d in zip(src, dst))


def build():
    cx = W / 2.0
    cy = H / 2.0 - 40
    px = bytearray(W * H * 4)
    for y in range(H):
        if y < H // 2:
            base = rgb(mix(THEME_WALL_TOP, THEME_WALL_MID, (y * 256) // (H // 2)))
        else:
            base = rgb(mix(THEME_WALL_MID, THEME_WALL_BOTTOM, ((y - H // 2) * 256) // (H // 2)))
        for x in range(W):
            col = base
            c = ring_cov(x, y, cx, cy, 150, 148)
            if c > 0:
                col = over(col, RING2, c)
            c = ring_cov(x, y, cx, cy, 122, 117)
            if c > 0:
                col = over(col, RING1, c)
            c = disc_cov(x, y, cx, cy, 70)
            if c > 0:
                col = over(col, BODY, c)
            c = disc_cov(x, y, cx - 22, cy - 22, 24)
            if c > 0:
                col = over(col, GLOW, c)
            i = (y * W + x) * 4
            px[i] = col[0]
            px[i + 1] = col[1]
            px[i + 2] = col[2]
            px[i + 3] = 255
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
    out = sys.argv[1] if len(sys.argv) > 1 else "assets/orbit_wallpaper.png"
    write_png(out, W, H, build())
    print("wrote", out, W, "x", H)


if __name__ == "__main__":
    main()
