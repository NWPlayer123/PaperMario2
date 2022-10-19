import os, subprocess
from struct import unpack

#does not include SDK or MusyX
base_size_data = [
    [".text", 0x8025854C - 0x800055E0],
    [".rodata", 0x80301B78 - 0x802C0F20],
    [".data", 0x803B7360 - 0x80302C20],
    [".sdata", 0x80418198 - 0x8040F260],
    [".sdata2", 0x80422620 - 0x80419380]
    ]

rel_size_data = [
    ["aaa.rel", 0x384, 0x2720],
    ["aji.rel", 0x10DAC, 0x45388],
    ["bom.rel", 0x616C, 0x102D4],
    ["dig.rel", 0x60C, 0xE45],
    ["dmo.rel", 0x1D54, 0x238F],
    ["dou.rel", 0x3BA0, 0x1F88C],
    ["eki.rel", 0x86F8, 0x173F8],
    ["end.rel", 0x2530, 0x2C47],
    ["gon.rel", 0x3CC8, 0x22E78],
    ["gor.rel", 0x12378, 0x591E1],
    ["gra.rel", 0x1368, 0x11DFE],
    ["hei.rel", 0x5DE8, 0x21410],
    ["hom.rel", 0x728, 0x49CF],
    ["jin.rel", 0x7020, 0x23741],
    ["jon.rel", 0x6F74, 0x740E4],
    ["kpa.rel", 0x1378, 0x8BAD],
    ["las.rel", 0x11858, 0x67721],
    ["moo.rel", 0x3020, 0xCAF3],
    ["mri.rel", 0x20254, 0x42713],
    ["muj.rel", 0xD8C0, 0x4442C],
    ["nok.rel", 0x988, 0xBB2C],
    ["pik.rel", 0x132C, 0x125A9],
    ["qiz.rel", 0xB0, 0xC7],
    ["rsh.rel", 0xF9D0, 0x23AAD],
    ["sys.rel", 0x1834, 0x917],
    ["tik.rel", 0x7D50, 0x36AE1],
    ["tou.rel", 0x8864, 0x37880],
    ["tou2.rel", 0x9520, 0x648FC],
    ["usu.rel", 0x3734, 0x147E8],
    ["win.rel", 0x3640, 0x1C57C],
    ["yuu.rel", 0x13F30, 0x7235]]

base_code_size = 0
base_data_size = 0
#have to walk the objects since most decompiled data doesn't get linked in yet
for root, dirs, files in os.walk("build/objects/base"):
    for entry in files:
        if entry.endswith(".o"): #double check for object files
            path = "/".join([root, entry])
            output = subprocess.check_output(["readelf", "-S", path])
            output = output.split(b"\n")
            sections = output[5:-6]
            for section in sections: #only get sections with actual code/data
                section = section.strip()
                section_size = int(section[55:61], 16)
                section_name = section[5:22].strip()
                if section_name in [b".text"]:
                    base_code_size += section_size
                elif section_name in [b".rodata", b".data", b".sdata", b".sdata2"]:
                    base_data_size += section_size

full_base_code_size = base_size_data[0][1]
full_base_data_size = 0
for entry in base_size_data[1:]:
    full_base_data_size += entry[1]

print("[Progress]")
print("MarioSt.dol:")
print("\tCode sections: 0x%X / 0x%X (~%f%% done)" % (base_code_size, full_base_code_size, (float(base_code_size) / float(full_base_code_size)) * 100.0))
print("\tData sections: 0x%X / 0x%X (~%f%% done)\n" % (base_data_size, full_base_data_size, (float(base_data_size) / float(full_base_data_size)) * 100.0))

total_code_size = base_code_size
total_data_size = base_data_size
full_total_code_size = full_base_code_size
full_total_data_size = full_base_data_size

#print out each rel's total
for this_data in rel_size_data:
    full_total_code_size += this_data[1]
    full_total_data_size += this_data[2]
    
    filename = "build/module/%s" % this_data[0]
    if os.path.exists(filename): #if exists, read section data and compare
        with open(filename, "rb") as f:
            f.seek(0xC)
            section_count, section_offset = unpack(">2I", f.read(8))
            f.seek(0x1C)
            assert unpack(">I", f.read(4))[0] == 3 #rel version 3
            f.seek(section_offset)
            assert unpack(">2I", f.read(8)) == (0, 0) #first entry null

            this_code_size = 0
            this_data_size = 0
            for i in range(section_count - 1): #already read 1
                this_offset, this_size = unpack(">2I", f.read(8))
                if this_offset != 0 and this_size != 0:
                    if this_offset & 1: #code section
                        this_code_size += this_size
                    else:
                        this_data_size += this_size

        total_code_size += this_code_size
        total_data_size += this_data_size

        print("%s:" % this_data[0])
        print("\tCode sections: 0x%X / 0x%X (~%f%% done)" % (this_code_size, this_data[1], (float(this_code_size) / float(this_data[1])) * 100.0))
        print("\tData sections: 0x%X / 0x%X (~%f%% done)\n" % (this_data_size, this_data[2], (float(this_data_size) / float(this_data[2])) * 100.0))

print("[Totals]")
print("DOL Finished: 0x%X / 0x%X (~%f%% done)" % (base_code_size + base_data_size, full_base_code_size + full_base_data_size,
                                              (float(base_code_size + base_data_size) / float(full_base_code_size + full_base_data_size)) * 100.0))
print("Game Finished: 0x%X / 0x%X (~%f%% done)" % (total_code_size + total_data_size, full_total_code_size + full_total_data_size,
                                                   (float(total_code_size + total_data_size) / float(full_total_code_size + full_total_data_size)) * 100.0))

