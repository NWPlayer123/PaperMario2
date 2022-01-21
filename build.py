import os, re, sys, subprocess, time, argparse, shutil
import regex #https://pypi.org/project/regex/
import colorama #https://pypi.org/project/colorama/
from colorama import Fore, Style

#customizable data
project_name = "G8MJ01"
project_type = "relocatable" #standard, relocatable
C_BUILD_FLAGS = ["-O4,p", "-fp_contract", "on", "-use_lmw_stmw", "on", "-multibyte", "-rostr", "-sdata", "48", "-sdata2", "6",
                 "-opt", "nodeadcode", "-w", "all", "-w", "nonotused", "-w", "nounusedarg", "-Iinclude/", "-once"] #"-inline", "deferred"
C_BUILD_FLAGS_REL = ["-O4,p", "-fp_contract", "on", "-use_lmw_stmw", "on", "-multibyte", "-rostr", "-sdata", "0", "-sdata2", "0",
                 "-opt", "nodeadcode", "-w", "all", "-w", "nonotused", "-w", "nounusedarg", "-Iinclude/", "-once"] #"-inline", "deferred"
C_LINK_FLAGS = ["-map", "build/%s.map" % project_name]

def iconv_check(source_file):
    if source_file in [b"source/event.c", b"source/data/item_data.c", b"source/mario_pouch.c"]:#b"source/data/npc_data.c"]:
        with open("build/temp/iconv.c", "wb") as o:
            subprocess.run(["iconv.exe", "-f", "UTF-8", "-t", "SJIS", source_file], stdout=o)
        print("running iconv")
        return "build/temp/iconv.c", True
    else:
        return source_file, False

def iconv_delete():
    os.remove("build/temp/iconv.c")

#-----------------------------------------------------------------------------------------------------------------------------
#generators
def check_env():
    paths = {}
    
    if not os.getenv("SDK_BASE_PATH"):
        print("SDK_BASE_PATH not defined. Please point it to your SDK root directory.")
        sys.exit(0)
    paths["SDK_BASE_PATH"] = os.getenv("SDK_BASE_PATH")
    paths["SDK_LIB_PATH"] = paths["SDK_BASE_PATH"] + "/HW2/lib/"
    paths["SDK_INC_PATH"] = paths["SDK_BASE_PATH"] + "/include/"
    paths["SDK_BIN_PATH"] = paths["SDK_BASE_PATH"] + "/X86/bin/"
    
    if not os.getenv("MW_BASE_PATH"):
        print("MW_BASE_PATH not defined. Please point it to your Metrowerks CodeWarrior root directory.")
        sys.exit(0)
    paths["MW_BASE_PATH"] = os.getenv("MW_BASE_PATH")
    paths["MW_LIB_PATH"] = paths["MW_BASE_PATH"] + "/PowerPC_EABI_Support/"
    paths["MW_INC_PATH"] = paths["MW_BASE_PATH"] + "/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/"
    paths["MW_BIN_PATH"] = paths["MW_BASE_PATH"] + "/PowerPC_EABI_Tools/Command_Line_Tools/"
    
    if not os.getenv("MUSYX_BASE_PATH"):
        print("MUSYX_BASE_PATH not defined. Please point it to your MusyX root directory.")
        sys.exit(0)
    paths["MUSYX_BASE_PATH"] = os.getenv("MUSYX_BASE_PATH")
    paths["MUSYX_LIB_PATH"] = paths["MUSYX_BASE_PATH"] + "/HW2/lib/"
    paths["MUSYX_INC_PATH"] = paths["MUSYX_BASE_PATH"] + "/include/"

    if not os.path.exists(paths["MW_BIN_PATH"] + "/mwcceppc.exe"):
        print("Couldn't find mwcceppc.exe in MW_BASE_PATH. Please ensure you're pointing to the right directory.")
        sys.exit(0)

    if not os.path.exists(paths["MW_BIN_PATH"] + "/mwldeppc.exe"):
        print("Couldn't find mwldeppc.exe in MW_BASE_PATH. Please ensure you're pointing to the right directory.")
        sys.exit(0)
        
    return paths

def generate_libraries(paths, build_type):
    array = []
    
    #SDK libraries
    lib_names = ["ai", "am", "amcnotstub", "amcstubs", "ar", "ax", "axart", "axfx", "base", "card", "db", "demo",
                 "dsp", "dtk", "dvd", "exi", "fileCache", "G2D", "gd", "gx", "hio", "mcc", "mix", "mtx", "odemustubs",
                 "odenotstub", "os", "pad", "perf", "seq", "si", "sp", "support", "syn", "texPalette", "vi"]
    for entry in lib_names:
        if build_type.lower() == "debug":
            entry += "D.a"
        else:
            entry += ".a"
        array.append("%s/%s" % (paths["SDK_LIB_PATH"], entry))

    #Metrowerks libraries
    array.append("%s/%s" % (paths["MW_LIB_PATH"], "MSL/MSL_C/PPC_EABI/Lib/MSL_C.PPCEABI.bare.H.a"))
    array.append("%s/%s" % (paths["MW_LIB_PATH"], "MetroTRK/TRK_Minnow_Dolphin.a"))
    array.append("%s/%s" % (paths["MW_LIB_PATH"], "Runtime/Lib/Runtime.PPCEABI.H.a"))

    #MusyX library
    array.append("%s/%s" % (paths["MUSYX_LIB_PATH"], "musyx.a"))
    
    return array

def generate_MWCIncludes(paths):
    string = ""
    string += "%s/%s;" % (paths["MW_BASE_PATH"], "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include")
    string += "%s/%s;" % (paths["MW_BASE_PATH"], "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include")
    string += "%s/%s;" % (paths["SDK_BASE_PATH"], "include")
    return string

#-----------------------------------------------------------------------------------------------------------------------------
#cache, includes
def find_exists(filename):
    filename = filename.decode("ASCII")
    if os.path.exists("include/%s" % filename):
        return b"include/%s" % filename.encode("ASCII")
    for module in os.listdir("module"):
        if os.path.exists("module/%s/include/%s" % (module, filename)):
            return b"module/%s/include/%s" % (module.encode("ASCII"), filename.encode("ASCII"))
    return b""

def find_all_include(filename, dependencies):
    data = open(filename, "rb").read()
    for entry in regex.findall(b'#[[:blank:]]*include[[:blank:]]*\"\K([\w.\/]+)(?=\")', data): #change \" to ["|>] to include system
        entry = find_exists(entry)
        if entry not in dependencies: #recurse into each include
            dependencies.append(entry)
            dependencies = find_all_include(entry, dependencies)
    return dependencies

def build_cache_files(object_depends, cache_times, build_flags, link_flags):
    all_paths = []
    with open("build/cache/cached_data.txt", "wb") as o:
        for object_file in object_depends.keys():
            if object_file not in [b"", ""] and object_depends[object_file] != []:
                if object_file not in all_paths:
                    all_paths.append(object_file)
                try: o.write(object_file.encode("ASCII"))
                except: o.write(object_file)
                o.write(b"\r\n")
                for include_file in object_depends[object_file]:
                    if include_file not in all_paths:
                        all_paths.append(include_file)
                    o.write(include_file + b"\r\n")
                o.write(b"\r\n") #extra to separate entries
    
    with open("build/cache/cache_times.txt", "wb") as o:
        for path in all_paths:
            try: o.write(path.encode("ASCII"))
            except: o.write(path)
            o.write(b" " + str(os.path.getmtime(path)).encode("ASCII") + b"\r\n")

    with open("build/cache/cache_build_flags.txt", "wb") as o:
        for entry in build_flags:
            o.write(entry.strip().encode("ASCII") + b"\r\n")

    with open("build/cache/cache_link_flags.txt", "wb") as o:
        for entry in link_flags:
            o.write(entry.strip().encode("ASCII") + b"\r\n")

def build_module_cache_files(build_flags):
    with open("build/cache/cache_module_build_flags.txt", "wb") as o:
        for entry in build_flags:
            o.write(entry.strip().encode("ASCII") + b"\r\n")

#-----------------------------------------------------------------------------------------------------------------------------
#build helpers
def compile_object(source_file, output_file, flag_array, paths, MWCIncludes):
    arguments = [paths["MW_BIN_PATH"] + "/mwcceppc.exe", "-o", output_file, "-c", source_file] + flag_array
    return subprocess.run(arguments, env={"MWCIncludes": MWCIncludes})

def generate_preplf(object_files, output_file, flag_array, paths):
    arguments = [paths["MW_BIN_PATH"] + "/mwldeppc.exe", "-r", "-o", output_file] + flag_array + object_files
    return subprocess.run(arguments)

def generate_linker_command_file(module_list, elf_path):
    arguments = [paths["SDK_BIN_PATH"] + "makerel.exe"] + module_list
    ret = subprocess.run(arguments, cwd=os.getcwd()+"/build/module") #run makerel with all preplf to get import.lst in cwd
    with open("build/temp/base.lcf", "wb") as o:
        with open(paths["SDK_INC_PATH"] + "/dolphin/eppc.HW2.lcf", "rb") as f:
            o.write(f.read())
        o.write(b"\r\nFORCEACTIVE {\r\n")
        with open("build/module/import.lst", "rb") as f:
            o.write(f.read())
        o.write(b"\tOSLink\r\n")
        o.write(b"\tOSUnlink\r\n")
        o.write(b"}\r\n")

    with open("build/temp/module.lcf", "wb") as o:
        with open(paths["SDK_INC_PATH"] + "/dolphin/eppc.lcf", "rb") as f:
            o.write(f.read())
        o.write(b"\r\nFORCEACTIVE {\r\n")
        with open("build/module/import.lst", "rb") as f:
            o.write(f.read())
        o.write(b"\t_prolog\r\n")
        o.write(b"\t_epilog\r\n")
        o.write(b"\t_unresolved\r\n")
        o.write(b"}\r\n")
        o.write(b"EXCLUDEFILES {\r\n")
        o.write(b"\t%s\r\n" % elf_path)
        o.write(b"}\r\n")

def partial_link_module(object_files, output_file, flag_array, paths):
    arguments = [paths["MW_BIN_PATH"] + "/mwldeppc.exe", "-r2", "-strip_partial", "-o", output_file] + flag_array + object_files + ["build/module/G8MJ01.elf"]
    return subprocess.run(arguments)

def link_executable(object_files, output_file, flag_array, paths):
    arguments = [paths["MW_BIN_PATH"] + "/mwldeppc.exe", "-o", output_file] + flag_array + object_files
    return subprocess.run(arguments)

def create_dol(source_file, output_file, paths, do_verbose=False):
    arguments = [paths["SDK_BIN_PATH"] + "makedol.exe", "-f", source_file, "-d", output_file]
    if do_verbose:
	    arguments.append("-v")
    return subprocess.run(arguments)

#-----------------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='CodeWarrior Project Build System')
    parser.add_argument("build_type", nargs="?", default="release", choices=["debug", "release", "clean"])
    args = parser.parse_args()
    build_type = args.build_type

    if build_type == "clean":
        try: shutil.rmtree("build/cache/")
        except FileNotFoundError: pass
        try: shutil.rmtree("build/objects/")
        except FileNotFoundError: pass
        try: shutil.rmtree("build/temp/")
        except FileNotFoundError: pass

    if build_type in ["debug", "release"]:
        #generators
        paths = check_env()
        libraries = generate_libraries(paths, build_type)
        MWCIncludes = generate_MWCIncludes(paths)
        if not os.path.exists("build/cache/"):
            os.makedirs("build/cache/")
        if not os.path.exists("build/objects/"):
            os.makedirs("build/objects/")
        if not os.path.exists("build/temp/"):
            os.makedirs("build/temp/")
        if project_type == "relocatable":
            if not os.path.exists("build/module/"):
                os.makedirs("build/module/")

        #standard defines
        if build_type.lower() == "debug":
            HW2_Prefix = paths["MW_LIB_PATH"] + "/Include/Prefix/HW2_Debug_Prefix.h"
        else:
            HW2_Prefix = paths["MW_LIB_PATH"] + "/Include/Prefix/HW2_Release_Prefix.h"
        C_BASE_FLAGS = ["-gccinc", "-proc", "gekko", "-fp", "hard", "-enum", "int"]
        C_BASE_INCLUDE_FLAGS = ["-I" + paths["SDK_INC_PATH"], "-I" + paths["MW_INC_PATH"],
                                "-I" + paths["MUSYX_INC_PATH"], "-include", HW2_Prefix]
        C_BASE_FLAGS_DEBUG = ["-Og"]
        C_BASE_FLAGS_RELEASE = ["-O2"]
        C_BASE_LINK_FLAGS = ["-proc", "gekko", "-fp", "hard", "-nostdlib"]
        C_BASE_LINK_INC_FLAGS = ["-l,"] + libraries

        full_build_flags = C_BASE_FLAGS + C_BASE_INCLUDE_FLAGS + C_BUILD_FLAGS
        if project_type == "relocatable":
            full_module_build_flags = C_BASE_FLAGS + C_BASE_INCLUDE_FLAGS + C_BUILD_FLAGS_REL
            for module in os.listdir("module"):
                full_module_build_flags.append("-Imodule/%s/include/" % module)

        #find all source files
        if project_type == "relocatable":
            #base dol, put all objects in base/ subfolder
            file_list = []
            for root, dirs, files in os.walk("source"):
                new_root = root.replace("\\", "/").encode("ASCII")
                for filename in files:
                    filename = filename.encode("ASCII")
                    if filename.endswith(b".c"):
                        source_file = b"%s/%s" % (new_root, filename)
                        output_file = b"build/objects/base/%s/%s" % (b"/".join(new_root.split(b"/")[1:]), b".".join(filename.split(b".")[:-1]) + b".o")
                        output_path = b"build/objects/base/%s" % (b"/".join(new_root.split(b"/")[1:]))
                        file_list.append([source_file, output_file, output_path])

            #put objects in <modulename>/
            module_file_list = {}
            for module in os.listdir("module"):
                module_file_list[module] = []
                for root, dirs, files in os.walk("module/%s/source" % module):
                    new_root = root.replace("\\", "/").encode("ASCII")
                    for filename in files:
                        filename = filename.encode("ASCII")
                        if filename.endswith(b".c"):
                            source_file = b"%s/%s" % (new_root, filename)
                            output_file = b"build/objects/%s/%s" % (b"/".join(new_root.split(b"/")[1:-1]), b".".join(filename.split(b".")[:-1]) + b".o")
                            output_path = b"build/objects/%s" % (b"/".join(new_root.split(b"/")[1:-1]))
                            module_file_list[module].append([source_file, output_file, output_path, module])

        else:
            file_list = []
            for root, dirs, files in os.walk("source"):
                new_root = root.replace("\\", "/").encode("ASCII")
                for filename in files:
                    filename = filename.encode("ASCII")
                    if filename.endswith(b".c"):
                        source_file = b"%s/%s" % (new_root, filename)
                        output_file = b"build/objects/%s/%s" % (b"/".join(new_root.split(b"/")[1:]), b".".join(filename.split(b".")[:-1]) + b".o")
                        output_path = b"build/objects/%s" % (b"/".join(new_root.split(b"/")[1:]))
                        file_list.append([source_file, output_file, output_path])

        #get cache data
        REBUILD_ALL = False
        old_build_flags = []
        if os.path.exists("build/cache/cache_build_flags.txt"):
            with open("build/cache/cache_build_flags.txt", "rb") as f:
                for line in f.readlines():
                    old_build_flags.append(line.strip().decode("ASCII"))
            if old_build_flags != full_build_flags:
                REBUILD_ALL = True #if flags were changed, rebuild
                
        if project_type == "relocatable":
            old_module_build_flags = []
            if os.path.exists("build/cache/cache_module_build_flags.txt"):
                with open("build/cache/cache_module_build_flags.txt", "rb") as f:
                    for line in f.readlines():
                        old_module_build_flags.append(line.strip().decode("ASCII"))
                if old_module_build_flags != full_module_build_flags:
                    REBUILD_ALL = True #if flags were changed, rebuild

        #TODO: separate for rel link?
        RELINK_ALL = False
        old_link_flags = []
        if os.path.exists("build/cache/cache_link_flags.txt"):
            with open("build/cache/cache_link_flags.txt", "rb") as f:
                for line in f.readlines():
                    old_link_flags.append(line.strip().decode("ASCII"))
            if old_link_flags != (C_BASE_LINK_FLAGS + C_LINK_FLAGS):
                RELINK_ALL = True
        
        object_depends = {}
        if os.path.exists("build/cache/cached_data.txt"):
            data = open("build/cache/cached_data.txt", "rb").read()
            for object_file in data.split(b"\r\n\r\n"): #object path, include paths, double return
                lines = object_file.split(b"\r\n")
                object_depends[lines[0]] = lines[1:]

        cache_times = {}
        if os.path.exists("build/cache/cache_times.txt"):
            with open("build/cache/cache_times.txt", "rb") as f:
                for line in f.readlines():
                    obj_time = float(line.split(b" ")[-1])
                    obj_path = b" ".join(line.split(b" ")[:-1])
                    cache_times[obj_path] = obj_time

        
        object_files = []
        do_compile = []
        for current_file in file_list:
            if not REBUILD_ALL and current_file[0] in object_depends.keys(): #we have it cached, check if it needs rebuilt
                if os.path.getmtime(current_file[0]) > cache_times[current_file[0]]: #C file updated, needs rebuild
                    do_compile.append(current_file)
                else:
                    for include in object_depends[current_file[0]]:
                        if os.path.getmtime(include) > cache_times[include]: #header file updated, needs rebuild
                            do_compile.append(current_file)
                            break #out of include loop
            else: #default to building
                do_compile.append(current_file)
            object_files.append(current_file[1])

        if project_type == "relocatable":
            #process each module
            module_object_files = {}
            rel_do_compile = []
            for module in module_file_list.keys():
                module_object_files[module] = []
                for current_file in module_file_list[module]:
                    if not REBUILD_ALL and current_file[0] in object_depends.keys(): #we have it cached, check if it needs rebuilt
                        if os.path.getmtime(current_file[0]) > cache_times[current_file[0]]: #C file updated, needs rebuild
                            rel_do_compile.append(current_file)
                        else:
                            for include in object_depends[current_file[0]]:
                                if os.path.getmtime(include) > cache_times[include]: #header file updated, needs rebuild
                                    rel_do_compile.append(current_file)
                                    break #out of include loop
                    else: #default to building
                        rel_do_compile.append(current_file)
                    module_object_files[module].append(current_file[1])

        #compile all files
        colorama.init() #setup colors
        if len(do_compile) > 0: #only print header if there's actually stuff to compile
            print(Fore.GREEN + "Building Static Files" + Style.RESET_ALL)
            for i in range(len(do_compile)):
                #print, initialization
                current_file = do_compile[i] #helper
                percent = float(i) / float(len(do_compile)) * 100.0
                percent = "[%.0f%%] " % percent
                print(Fore.GREEN + percent + "Building " + current_file[0].decode("ASCII") + Style.RESET_ALL)
                print
                if not os.path.exists(current_file[2]):
                    os.makedirs(current_file[2])

                #get cache data
                dependencies = find_all_include(current_file[0], [])
                dependencies.sort(key=lambda x: x.lower())
                object_depends[current_file[0]] = [] #reset that entry
                for entry in dependencies:
                    object_depends[current_file[0]].append(entry)

                #compile file
                source_file, iconv_removeme = iconv_check(current_file[0]) #custom step, remove for other projects

                ret = compile_object(source_file, current_file[1], full_build_flags, paths, MWCIncludes)
                if ret.returncode != 0: #file didn't compile correctly, abort
                    del object_depends[current_file[0]] #remove current entry so it will rebuild, save all others
                    build_cache_files(object_depends, cache_times, full_build_flags, C_BASE_LINK_FLAGS+C_LINK_FLAGS) #store new cache
                    if project_type == "relocatable":
                        build_module_cache_files(full_module_build_flags)
                    sys.exit(0)

                if iconv_removeme: iconv_delete() #custom step, remove for other projects

        if project_type == "relocatable":
            if len(rel_do_compile) > 0: #only print header if there's actually stuff to compile
                print("\r\n" + Fore.GREEN + "Building Relocatable Files" + Style.RESET_ALL)
                for i in range(len(rel_do_compile)):
                    #print, initialization
                    current_file = rel_do_compile[i] #helper
                    percent = float(i) / float(len(rel_do_compile)) * 100.0
                    percent = "[%.0f%%] " % percent
                    print(Fore.GREEN + percent + "Building " + current_file[0].decode("ASCII") + Style.RESET_ALL)
                    print
                    if not os.path.exists(current_file[2]):
                        os.makedirs(current_file[2])

                    #get cache data
                    dependencies = find_all_include(current_file[0], [])
                    dependencies.sort(key=lambda x: x.lower())
                    object_depends[current_file[0]] = [] #reset that entry
                    for entry in dependencies:
                        object_depends[current_file[0]].append(entry)

                    #compile file
                    source_file, iconv_removeme = iconv_check(current_file[0]) #custom step, remove for other projects

                    ret = compile_object(source_file, current_file[1], full_module_build_flags, paths, MWCIncludes)
                    if ret.returncode != 0: #file didn't compile correctly, abort
                        del object_depends[current_file[0]] #remove current entry so it will rebuild, save all others
                        build_cache_files(object_depends, cache_times, full_build_flags, C_BASE_LINK_FLAGS+C_LINK_FLAGS) #store new cache
                        build_module_cache_files(full_module_build_flags)
                        sys.exit(0)

                    if iconv_removeme: iconv_delete() #custom step, remove for other projects

        if project_type == "relocatable":
            #pre-link all modules
            print("\r\n" + Fore.GREEN + "Pre-Linking Modules" + Style.RESET_ALL)
            modules = []
            for module in module_file_list.keys():
                output_path = "build/module/%s.preplf" % module
                link_object_files = [] #build object list for this specific module
                for entry in module_file_list[module]:
                    link_object_files.append(entry[1])

                print(Fore.GREEN + "Linking " + output_path + Style.RESET_ALL)
                generate_preplf(link_object_files, output_path, C_BASE_LINK_FLAGS, paths)
                modules.append("%s.preplf" % module)

            print(Fore.GREEN + "Generating Import List" + Style.RESET_ALL)
            generate_linker_command_file(modules, b"%s.elf" % project_name.encode("ASCII"))
            print(Fore.GREEN + "Linking Base Executable" + Style.RESET_ALL)
            link_executable(object_files, "build/%s.elf" % project_name, C_BASE_LINK_INC_FLAGS + C_BASE_LINK_FLAGS + C_LINK_FLAGS + ["-lcf", "build/temp/base.lcf"], paths)
            
            #generate finished binaries
            print(Fore.GREEN + "Full Linking Modules" + Style.RESET_ALL)
            modules = []
            for module in module_file_list.keys():
                output_path = "build/module/%s.plf" % module
                link_object_files = [] #build object list for this specific module
                for entry in module_file_list[module]:
                    link_object_files.append(entry[1])

                print(Fore.GREEN + "Linking " + output_path + Style.RESET_ALL)
                partial_link_module(link_object_files, output_path, C_BASE_LINK_FLAGS + ["-lcf", "build/temp/module.lcf"], paths)
                modules.append("%s.preplf" % module)
                
            
                
        '''print(Fore.GREEN + Style.BRIGHT + "[100%%] Linking build/%s.elf" % project_name + Style.RESET_ALL)
        link_executable(object_files, "build/%s.elf" % project_name, C_BASE_LINK_INC_FLAGS + C_BASE_LINK_FLAGS + C_LINK_FLAGS, paths)'''
        
        build_cache_files(object_depends, cache_times, full_build_flags, C_BASE_LINK_FLAGS+C_LINK_FLAGS) #store new cache
        build_module_cache_files(full_module_build_flags)

        '''#compile needed files
        init() #setup colors
        for i in range(len(do_compile)):
            #print, initialization
            current_file = do_compile[i]
            percent = float(i) / float(len(do_compile)) * 100.0
            percent = "[%.0f%%] " % percent
            print(Fore.GREEN + percent + "Building " + current_file[0].decode("ASCII") + Style.RESET_ALL)
            print
            if not os.path.exists(current_file[2]):
                os.makedirs(current_file[2])

            #get cache data
            fuckit = find_all_include(current_file[0], b".", [])
            fuckit.sort(key=lambda x: x.lower())
            object_depends[current_file[0]] = [] #reset that entry
            for entry in fuckit:
                object_depends[current_file[0]].append(b"include/" + entry)
                
            #compile file
            source_file, iconv_removeme = iconv_check(current_file[0]) #custom step, remove for other projects
            
            ret = compile_object(source_file, current_file[1], C_BASE_FLAGS+C_BASE_INCLUDE_FLAGS+C_BUILD_FLAGS, paths, MWCIncludes)
            if ret.returncode != 0: #file didn't compile correctly, abort
                del object_depends[current_file[0]] #remove current entry so it will rebuild, save all others
                build_cache_files(object_depends, cache_times, C_BASE_FLAGS+C_BUILD_FLAGS, C_BASE_LINK_FLAGS+C_LINK_FLAGS) #store new cache
                sys.exit(0)

            if iconv_removeme: iconv_delete() #custom step, remove for other projects

        #build executable files
        print(Fore.GREEN + Style.BRIGHT + "[100%%] Linking build/%s.elf" % project_name + Style.RESET_ALL)
        link_executable(object_files, "build/%s.elf" % project_name, C_BASE_LINK_INC_FLAGS + C_BASE_LINK_FLAGS + C_LINK_FLAGS, paths)
        create_dol("build/%s.elf" % project_name, "build/%s.dol" % project_name, paths)
        build_cache_files(object_depends, cache_times, C_BASE_FLAGS+C_BUILD_FLAGS, C_BASE_LINK_FLAGS+C_LINK_FLAGS) #store new cache'''
