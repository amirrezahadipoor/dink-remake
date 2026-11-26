#!/usr/bin/env python3
"""ffextract.py — Dink Smallwood dir.ff (FastFile) archive tool.

Format (little-endian, derived from engine/src/fastfile.cpp):
  uint32            entry_count (N)
  N x { uint32 off; char name[13]; }   # name NUL-terminated, 12 usable chars
  blob data...                         # entry i length = off(i+1) - off(i)

The last entry is conventionally a sentinel pointing at EOF (often empty name).

Commands:
  list    <archive.ff>
  extract <archive.ff> <outdir>
  pack    <indir> <archive.ff>         # packs files in manifest order
  selftest <graphics-root>             # roundtrip-test every dir.ff found
"""
import os
import struct
import sys
import tempfile

ENTRY = struct.Struct("<I13s")


def read_archive(path):
    """Return (entries, blob) where entries = [(name, off)], blob = raw file."""
    with open(path, "rb") as f:
        raw = f.read()
    (n,) = struct.unpack_from("<I", raw, 0)
    entries = []
    pos = 4
    for _ in range(n):
        off, name = ENTRY.unpack_from(raw, pos)
        pos += ENTRY.size
        name = name.split(b"\0", 1)[0].decode("latin-1")
        entries.append((name, off))
    return entries, raw


def entry_spans(entries, total_len):
    """Yield (name, start, end) for real (non-sentinel) entries."""
    for i, (name, off) in enumerate(entries):
        end = entries[i + 1][1] if i + 1 < len(entries) else total_len
        if end < off:
            end = off
        yield name, off, end


def cmd_list(archive):
    entries, raw = read_archive(archive)
    print(f"{archive}: {len(entries)} entries, {len(raw)} bytes")
    for name, start, end in entry_spans(entries, len(raw)):
        print(f"  {name:<12} {end - start:>8} bytes @ {start}")


def cmd_extract(archive, outdir):
    entries, raw = read_archive(archive)
    os.makedirs(outdir, exist_ok=True)
    manifest = []
    for name, start, end in entry_spans(entries, len(raw)):
        manifest.append(name)
        if name:  # sentinel entries have empty names
            with open(os.path.join(outdir, name), "wb") as f:
                f.write(raw[start:end])
    with open(os.path.join(outdir, ".manifest"), "w", encoding="latin-1") as f:
        f.write("\n".join(manifest))
    print(f"extracted {len(manifest)} entries -> {outdir}")


def cmd_pack(indir, archive):
    with open(os.path.join(indir, ".manifest"), encoding="latin-1") as f:
        names = f.read().split("\n")
    header_len = 4 + ENTRY.size * len(names)
    offs, blobs = [], []
    pos = header_len
    for name in names:
        offs.append(pos)
        if name:
            with open(os.path.join(indir, name), "rb") as f:
                data = f.read()
            blobs.append(data)
            pos += len(data)
    out = bytearray(struct.pack("<I", len(names)))
    for name, off in zip(names, offs):
        out += ENTRY.pack(off, name.encode("latin-1"))
    for data in blobs:
        out += data
    with open(archive, "wb") as f:
        f.write(out)
    print(f"packed {len(names)} entries -> {archive} ({len(out)} bytes)")


def cmd_selftest(root):
    archives = []
    for dirpath, _dirs, files in os.walk(root):
        for fn in files:
            if fn.lower().endswith(".ff"):
                archives.append(os.path.join(dirpath, fn))
    archives.sort()
    if not archives:
        print("no .ff archives found", file=sys.stderr)
        return 1
    byte_ok = sem_ok = 0
    for a in archives:
        with tempfile.TemporaryDirectory() as td:
            out = os.path.join(td, "x")
            repack = os.path.join(td, "re.ff")
            cmd_extract_quiet(a, out)
            cmd_pack_quiet(out, repack)
            orig = open(a, "rb").read()
            new = open(repack, "rb").read()
            if orig == new:
                byte_ok += 1
                sem_ok += 1
            elif same_contents(a, repack):
                sem_ok += 1
                print(f"  note: {a} byte-differs (layout) but contents identical")
            else:
                print(f"FAIL: {a} roundtrip corrupted", file=sys.stderr)
                return 1
    print(f"selftest OK: {len(archives)} archives | byte-identical: {byte_ok} | content-identical: {sem_ok}")
    return 0


def cmd_extract_quiet(a, out):
    entries, raw = read_archive(a)
    os.makedirs(out, exist_ok=True)
    manifest = []
    for name, start, end in entry_spans(entries, len(raw)):
        manifest.append(name)
        if name:
            with open(os.path.join(out, name), "wb") as f:
                f.write(raw[start:end])
    with open(os.path.join(out, ".manifest"), "w", encoding="latin-1") as f:
        f.write("\n".join(manifest))


def cmd_pack_quiet(indir, archive):
    stdout = sys.stdout
    sys.stdout = open(os.devnull, "w")
    try:
        cmd_pack(indir, archive)
    finally:
        sys.stdout.close()
        sys.stdout = stdout


def same_contents(a, b):
    ea, ra = read_archive(a)
    eb, rb = read_archive(b)
    da = {n: ra[s:e] for n, s, e in entry_spans(ea, len(ra)) if n}
    db = {n: rb[s:e] for n, s, e in entry_spans(eb, len(rb)) if n}
    return da == db


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 2
    cmd = sys.argv[1]
    if cmd == "list":
        return cmd_list(sys.argv[2]) or 0
    if cmd == "extract":
        return cmd_extract(sys.argv[2], sys.argv[3]) or 0
    if cmd == "pack":
        return cmd_pack(sys.argv[2], sys.argv[3]) or 0
    if cmd == "selftest":
        return cmd_selftest(sys.argv[2])
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main())
