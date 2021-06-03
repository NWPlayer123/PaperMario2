import os, subprocess

full_size = 0
for root, dirs, files in os.walk("build/objects"):
    for entry in files:
        if entry.endswith(".o"):
            path = "/".join([root, entry])
            output = subprocess.check_output(["readelf", "-S", path])
            output = output.split(b"\r\n")
            sections = output[5:-6]
            for section in sections:
                section = section.strip()
                section_size = int(section[55:61], 16)
                section_name = section[5:22].strip()
                if section_name in [b".text", b".rodata", b".data", b".sdata", b".sdata2"]:
                    full_size += section_size

#does not include SDK or MusyX
full_retail_sections = [
    [".text", 0x8025854C - 0x800055E0],
    [".rodata", 0x80301B78 - 0x802C0F20],
    [".data", 0x803B7360 - 0x80302C20],
    [".sdata", 0x80418198 - 0x8040F260],
    [".sdata2", 0x80422620 - 0x80419380]
    ]

full_retail_size = 0
for entry in full_retail_sections:
    full_retail_size += entry[1]

print("0x%X / 0x%X" % (full_size, full_retail_size))
print("~%f%% done" % (float(full_size) / float(full_retail_size) * 100.0))

'''
output = subprocess.check_output(["readelf", "-S", "evtmgr_cmd.o"])
output = output.split("\r\n")
sections = output[5:-6]
full_size = 0
for section in sections:
    section = section.strip()
    section_size = int(section[55:61], 16)
    section_name = section[5:22].strip()
    if section_name in [".text", ".rodata", ".data", ".sdata", ".sdata2"]:
        full_size += section_size
        
print("0x%08X" % full_size)'''
