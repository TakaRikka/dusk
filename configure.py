#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GZ2E01",    # GCN USA
    "GZ2P01",    # GCN PAL
    "GZ2J01",    # GCN JPN
    "RZDE01_00", # Wii USA Rev 0
    "RZDE01_02", # Wii USA Rev 2
    "RZDP01",    # Wii PAL
    "RZDJ01",    # Wii JPN
    "RZDK01",    # Wii KOR
    "DZDE01",    # Wii USA Kiosk Demo
    "DZDP01",    # Wii PAL Kiosk Demo
    "Shield",    # Shield
    "ShieldP",   # Shield Production
    "ShieldD",   # Shield Debug
]

# Versions to disable until properly configured
DISABLED_VERSIONS = [
    7,  # Wii KOR
    8,  # Wii USA Kiosk Demo
    9,  # Wii PAL Kiosk Demo
    11, # Shield Production
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
parser.add_argument(
    "--reghio",
    action="store_true",
    help="build with regHIO support",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

if version_num in DISABLED_VERSIONS:
    print(f"Version {VERSIONS[version_num]} is disabled. Using default")
    version_num = DEFAULT_VERSION
    config.version = VERSIONS[DEFAULT_VERSION]

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.7.1"
config.objdiff_tag = "v3.5.0"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.0-beta.5"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"-I assets/{config.version}",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = 69 # Twilight Princess (DOL)

# Globs to exclude from context files
# *.mch excludes precompiled header output (which cannot be parsed)
config.context_exclude_globs = ["*.mch"]

# Macro definitions to inject into context files
config.context_defines = ["DECOMPCTX"]

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-i include",
    f"-i build/{config.version}/include",
    f"-i assets/{config.version}",
    "-i src",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/Runtime/Inc",
    "-i src/PowerPC_EABI_Support/MetroTRK",
    "-i include/dolphin",
    f"-DVERSION={version_num}",
    "-D__GEKKO__",
]

if config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01", "Shield", "ShieldD"]:
    cflags_base.extend(["-enc SJIS"])
else:
    cflags_base.extend(["-multibyte"])

USE_REVOLUTION_SDK_VERSIONS = [
    "RZDE01_00", # Wii USA Rev 0
    "RZDE01_02", # Wii USA Rev 2
    "RZDP01",    # Wii PAL
    "RZDJ01",    # Wii JPN
    "RZDK01",    # Wii KOR
    "DZDE01",    # Wii USA Kiosk Demo
    "DZDP01",    # Wii PAL Kiosk Demo
    "Shield",    # Shield
    "ShieldP",   # Shield Production
    "ShieldD",   # Shield Debug
]

if config.version in USE_REVOLUTION_SDK_VERSIONS:
    cflags_base.extend(["-D__REVOLUTION_SDK__"])

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
elif config.version == "ShieldD":
    cflags_base.extend(["-DDEBUG=1"])

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-common off",
    "-char signed",
    "-func_align 4",
    "-DMSL_USE_INLINES=1",
]

if config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01", "ShieldD", "Shield"]:
    cflags_runtime.extend(["-ipa file", "-fp_contract off"])
else:
    cflags_runtime.extend(["-inline deferred,auto"])

cflags_trk = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-rostr",
    "-str reuse",
    "-gccinc",
    "-common off",
    "-inline deferred,auto",
    "-char signed",
    "-sdata 0",
    "-sdata2 0",
]

# Dolphin library flags
cflags_dolphin = [
    *cflags_base,
    "-ir src/dolphin",
    "-fp_contract off",
    "-char unsigned",
    "-sym on",
    "-DSDK_REVISION=2",
]

# Revolution library flags
cflags_revolution_base = [
    *cflags_base,
    "-ir src/revolution",
    "-fp_contract off",
    "-sym on",
    "-inline auto",
    "-ipa file",
    "-i include/revolution",
    "-D__REVOLUTION_SDK__",
]

cflags_revolution_retail = [
    *cflags_revolution_base,
    "-O4,p",
]

cflags_revolution_debug = [
    *cflags_revolution_base,
    "-opt off",
    "-inline off",
]

# Framework flags
cflags_framework = [
    *cflags_base,
    "-use_lmw_stmw off",
    "-schedule off",
    "-sym on",
    "-fp_contract off",
]

# for specific sdk libs
cflags_noopt = cflags_base[:]
if config.version != "ShieldD":
    cflags_noopt.remove("-O4,p")


if config.version == "ShieldD":
    cflags_framework.extend(["-O0,p", "-inline off", "-RTTI on", "-DDEBUG=1", "-DWIDESCREEN_SUPPORT=1"])
elif config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01", "Shield"]:
    cflags_framework.extend(["-ipa file", "-RTTI on", "-DWIDESCREEN_SUPPORT=1"])

if config.version in ["RZDE01_00", "ShieldD"] or args.debug or args.reghio:
    cflags_framework.extend(["-DENABLE_REGHIO=1"])

if config.version != "ShieldD":
    if config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01"]:
        # TODO: whats the correct inlining flag? deferred looks better in some places, others not. something else wrong?
        cflags_framework.extend(["-inline noauto", "-O4,s", "-sym on"])
    elif config.version in ["Shield"]:
        # TODO: whats the correct inlining flag? deferred looks better in some places, others not. something else wrong?
        cflags_framework.extend(["-inline noauto", "-O4,p", "-sym on"])
    else:
        cflags_framework.extend(["-inline noauto", "-O3,s", "-sym on", "-str reuse,pool,readonly"])

if config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01"]:
    cflags_framework.extend(["-DSDK_SEP2006"])


# REL flags
cflags_rel = [
    *cflags_framework,
    "-sdata 0",
    "-sdata2 0",
]

cflags_dolzel_framework = [
    *cflags_framework,
]

cflags_dolzel_rel = [
    *cflags_rel,
]

def MWVersion(cfg_version: str | None) -> str:
    match cfg_version:
        case "GZ2E01":
            return "GC/2.7"
        case "GZ2P01":
            return "GC/2.7"
        case "GZ2J01":
            return "GC/2.7"
        case "RZDE01_00" | "RZDE01_02" | "RZDP01" | "RZDJ01":
            # NOTE: we use a modified version of GC/3.0a3 to be able to handle multi-char constants.
            # This was probably a change made in some compiler version in the early days of transitioning GC to Wii development,
            # but we don't have that version. GC/3.0a3 appears to have the best overall codegen of any available GC/Wii compiler
            # However GC/3.0a5 is required for the linker version, GC/3.0a3 won't work.
            return "GC/3.0a3p1"
        case "ShieldD":
            return "Wii/1.0"
        case "Shield":
            return "Wii/1.0"
        case _:
            return "GC/2.7"

# Wii versions specifically need linker GC/3.0a5
if config.version in ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01"]:
    config.linker_version = "GC/3.0a5"
else:
    config.linker_version = MWVersion(config.version)


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "objects": objects,
    }

def RevolutionLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    if config.version == "ShieldD":
        return {
            "lib": lib_name,
            "mw_version": "Wii/1.0",
            "cflags": [*cflags_revolution_debug, "-DSDK_AUG2010"],
            "progress_category": "sdk",
            "objects": objects,
        }
    elif config.version == "Shield":
        return {
            "lib": lib_name,
            "mw_version": "Wii/1.0",
            "cflags": [*cflags_revolution_retail, "-DSDK_AUG2010"],
            "progress_category": "sdk",
            "objects": objects,
        }
    elif config.version == "RZDE01_00":
        return {
            "lib": lib_name,
            "mw_version": "GC/3.0a3",
            "cflags": [*cflags_revolution_retail, "-DSDK_SEP2006", "-DNW4HBM_DEBUG"],
            "progress_category": "sdk",
            "objects": objects,
        }
    else:
        return {
            "lib": lib_name,
            "mw_version": "GC/3.0a3",
            "cflags": [*cflags_revolution_retail, "-DSDK_SEP2006"],
            "progress_category": "sdk",
            "objects": objects,
        }

# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": MWVersion(config.version),
        "cflags": [*cflags_rel],
        "progress_category": "game",
        "objects": objects,
    }

# Helper function for actor RELs
def ActorRel(status: bool, rel_name: str, extra_cflags: List[str]=[]) -> Dict[str, Any]:
    return Rel(rel_name, [Object(status, f"d/actor/{rel_name}.cpp", extra_cflags=extra_cflags, scratch_preset_id=70)])


# Helper function for JSystem libraries
def JSystemLib(lib_name: str, objects: List[Object], progress_category: str="third_party") -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": MWVersion(config.version),
        "cflags": [*cflags_framework],
        "progress_category": progress_category,
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


ALL_GCN = ["GZ2E01", "GZ2P01", "GZ2J01"]
ALL_WII = ["RZDE01_00", "RZDE01_02", "RZDP01", "RZDJ01"] # , "RZDK01"]
ALL_DEMO = ["DZDE01", "DZDP01"]
ALL_SHIELD = ["Shield", "ShieldD"] # , "ShieldP"]
ALL = ALL_GCN + ALL_WII + ALL_SHIELD


# Object is only matching for specific versions
def MatchingFor(*versions) -> bool:
    fullset = set()
    for vers in versions:
        if isinstance(vers, (list, tuple, set)):
            fullset.update(vers)
        else:
            fullset.add(vers)

    return config.version in fullset


config.warn_missing_config = True
config.warn_missing_source = False
config.precompiled_headers = [
    {
        "source": "d/dolzel.pch",
        "mw_version": MWVersion(config.version),
        "cflags": ["-lang=c++", *cflags_dolzel_framework],
    },
    {
        "source": "d/dolzel_rel.pch",
        "mw_version": MWVersion(config.version),
        "cflags": ["-lang=c++", *cflags_dolzel_rel],
    },
    {
        "source": "JSystem/JSystem.pch",
        "mw_version": MWVersion(config.version),
        "cflags": ["-lang=c++", *cflags_framework],
    },
]
config.libs = [
    {
        "lib": "machine",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_main.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_printf.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_audio.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_controller_pad.cpp"),
            Object(NonMatching, "m_Do/m_Re_controller_pad.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_graphic.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_machine.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_mtx.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_ext.cpp"),
            Object(NonMatching, "m_Do/m_Do_ext2.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_lib.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_Reset.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_dvd_thread.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_DVDError.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_MemCard.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_MemCardRWmng.cpp"),
            Object(MatchingFor(ALL_GCN), "m_Do/m_Do_machine_exception.cpp"),
            Object(NonMatching, "m_Do/m_Do_hostIO.cpp"),
        ],
    },
    {
        "lib": "c",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "game",
        "host": True,
        "objects": [
            Object(MatchingFor(ALL_GCN), "c/c_damagereaction.cpp"),
            Object(MatchingFor(ALL_GCN), "c/c_dylink.cpp"),
        ],
    },
    {
        "lib": "framework",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            # f_ap
            Object(MatchingFor(ALL_GCN), "f_ap/f_ap_game.cpp"),

            # f_op
            Object(MatchingFor(ALL_GCN, "Shield"), "f_op/f_op_actor.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_actor_iter.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_actor_tag.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_camera.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_actor_map.cpp"),
            Object(MatchingFor(ALL_GCN), "f_op/f_op_actor_mng.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_WII, "Shield"), "f_op/f_op_camera_mng.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_overlap.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_WII, "Shield"), "f_op/f_op_overlap_mng.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_WII, "ShieldD"), "f_op/f_op_overlap_req.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_WII, "Shield"), "f_op/f_op_scene.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "f_op/f_op_scene_iter.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_scene_mng.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_scene_req.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_scene_tag.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_view.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_kankyo.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_msg.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_WII, "Shield"), "f_op/f_op_kankyo_mng.cpp"),
            Object(MatchingFor(ALL_GCN), "f_op/f_op_msg_mng.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_draw_iter.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_draw_tag.cpp"),
            Object(MatchingFor(ALL), "f_op/f_op_scene_pause.cpp"),

            # f_pc
            Object(MatchingFor(ALL_GCN, ALL_WII), "f_pc/f_pc_base.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_create_iter.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "f_pc/f_pc_create_req.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_create_tag.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_creator.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_delete_tag.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "f_pc/f_pc_deletor.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_draw_priority.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_executor.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "f_pc/f_pc_layer.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_leaf.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_layer_iter.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_layer_tag.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_line.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_load.cpp"),
            Object(MatchingFor(ALL_GCN), "f_pc/f_pc_manager.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_method.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_node.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "f_pc/f_pc_node_req.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "f_pc/f_pc_priority.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_profile.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_searcher.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_line_tag.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_line_iter.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_method_iter.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_method_tag.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_pause.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_draw.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_fstcreate_req.cpp"),
            Object(MatchingFor(ALL), "f_pc/f_pc_stdcreate_req.cpp"),
            Object(MatchingFor("ShieldD"), "f_pc/f_pc_debug_sv.cpp"),
        ],
    },
    {
        "lib": "dolzel2",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "game",
        "host": True,
        "objects": [
            Object(NonMatching, "d/d_home_button.cpp"),
            Object(NonMatching, "d/d_cursor_mng.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_stage.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_map.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_com_inf_game.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_com_static.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_com_inf_actor.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_bomb.cpp"), # debug weak func order
            Object(MatchingFor(ALL_GCN), "d/d_lib.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_save.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "d/d_save_init.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_jnt_col.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_a_obj.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_a_itembase_static.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_a_item_static.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_a_shop_item_static.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_a_horse_static.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_demo.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_door_param2.cpp"), # debug weak func order
            Object(MatchingFor(ALL_GCN), "d/d_resorce.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_map_path.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_map_path_fmap.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_map_path_dmap.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_event.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_event_data.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_event_manager.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_event_lib.cpp"), # debug weak func order
            Object(Equivalent, "d/d_event_debug.cpp"), # RTTI
            Object(MatchingFor(ALL_GCN), "d/d_simple_model.cpp"),
            Object(Equivalent, "d/d_particle.cpp"), # weak func order
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_particle_name.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_particle_copoly.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_path.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_drawlist.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kankyo_data.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kankyo_wether.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kankyo_rain.cpp"),
            Object(NonMatching, "d/d_kankyo_debug.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_vibration.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_vib_pattern.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_attention.cpp"),
            Object(NonMatching, "d/d_bg_parts.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "d/d_bg_pc.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "d/d_att_dist.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_bg_plc.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_acch.cpp"),
            Object(NonMatching, "d/d_bg_s_capt_poly.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_bg_s_chk.cpp"),
            Object(NonMatching, "d/d_bg_s_func.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_gnd_chk.cpp"), # debug weak func order
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_grp_pass_chk.cpp"), # debug weak func order
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_lin_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_movebg_actor.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "d/d_bg_s_sph_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_spl_grp_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_s_poly_pass_chk.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "d/d_bg_s_roof_chk.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "d/d_bg_s_wtr_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_w.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_w_base.cpp"),
            Object(NonMatching, "d/d_bg_w_deform.cpp"),
            Object(NonMatching, "d/d_bg_w_hf.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_w_kcol.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bg_w_sv.cpp"),
            Object(Equivalent, "d/d_cc_d.cpp"), # weak func order (cCcD_ShapeAttr::GetCoCP)
            Object(MatchingFor(ALL_GCN), "d/d_cc_mass_s.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_cc_s.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_cc_uty.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_cam_param.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_ev_camera.cpp"),
            Object(NonMatching, "d/d_jcam_editor.cpp"),
            Object(NonMatching, "d/d_jpreviewer.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_spline_path.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_item_data.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_item.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_tresure.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_model.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_eye_hl.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_error_msg.cpp"),
            Object(Equivalent, "d/d_debug_viewer.cpp"), # debug weak func order
            Object(NonMatching, "d/d_debug_pad.cpp"),
            Object(NonMatching, "d/d_debug_camera.cpp"),
            Object(Equivalent, "d/actor/d_a_alink.cpp"), # weak func order, vtable order
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_itembase.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_no_chg_room.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_npc.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_npc_cd.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_npc_cd2.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_obj_item.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_insect.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_obj_ss_base.cpp"),
            Object(MatchingFor(ALL_GCN), "d/actor/d_a_player.cpp"),
            Object(Equivalent, "d/d_camera.cpp"), # weak func order
            Object(MatchingFor(ALL_GCN), "d/d_envse.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_file_select.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_file_sel_warning.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_file_sel_info.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_bright_check.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_scope.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_select_cursor.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_select_icon.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_shop_camera.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_shop_item_ctrl.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_shop_system.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_gameover.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kankyo.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kyeff.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_kyeff2.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_ky_thunder.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_kantera_icon_meter.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_calibration.cpp"),
            Object(NonMatching, "d/d_menu_collect.cpp"), # weak func order (dMenu_Collect2D_c::draw())
            Object(MatchingFor(ALL_GCN), "d/d_menu_dmap.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_dmap_map.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_map_common.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_fishing.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_fmap.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_fmap_map.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_fmap2D.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_insect.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_item_explain.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_letter.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_option.cpp"),
            Object(NonMatching, "d/d_menu_quit.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_ring.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_save.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_skill.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_menu_window_HIO.cpp"),
            Object(Equivalent, "d/d_menu_window.cpp"), # weak func order (dDlst_MENU_CAPTURE_c::draw)
            Object(MatchingFor(ALL_GCN), "d/d_meter_HIO.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter_button.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter_haihai.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter_hakusha.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter_map.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter_string.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter2_draw.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter2_info.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_meter2.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_out_font.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_class.cpp"),
            Object(Equivalent, "d/d_msg_object.cpp"), # weak func order
            Object(MatchingFor(ALL_GCN), "d/d_msg_unit.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_3select.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_arrow.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_base.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_boss.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_explain.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_item.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_howl.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_jimaku.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_kanban.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_light.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_place.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_staff.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_talk.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_scrn_tree.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_string_base.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_string.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_msg_flow.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_name.cpp"),
            Object(NonMatching, "d/d_npc.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_npc_lib.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_ovlp_fade.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_ovlp_fade2.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_ovlp_fade3.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_pane_class.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_pane_class_alpha.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_pane_class_ex.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_s_logo.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "d/d_s_menu.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_s_name.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_s_play.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_s_room.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_s_title.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_save_HIO.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_timer.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_k_wmark.cpp"),
            Object(MatchingFor(ALL_GCN), "d/d_k_wpillar.cpp"),
        ],
    },
    {
        "lib": "DynamicLink",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            Object(MatchingFor(ALL_GCN), "DynamicLink.cpp"),
        ],
    },
    {
        "lib": "CaptureScreen",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            Object(NonMatching, "CaptureScreen.cpp"),
        ],
    },
    {
        "lib": "SSystem",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "third_party",
        "host": True,
        "objects": [
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_malloc.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_API.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_API_controller_pad.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_API_graphic.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_cc_d.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_cc_s.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_counter.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_list.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_list_iter.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_node.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_node_iter.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_tree.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_tree_iter.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_phase.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_request.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_tag.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "SSystem/SComponent/c_tag_iter.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_xyz.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_sxyz.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_math.cpp"), # debug weak literal order
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_bg_s_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_bg_s_gnd_chk.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_bg_s_lin_chk.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_bg_s_shdw_draw.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_bg_s_poly_info.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_bg_w.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_m2d.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d_g_aab.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "SSystem/SComponent/c_m3d_g_cir.cpp", extra_cflags=['-pragma "nosyminline on"']), # PCH fixes need for nosyminline - maybe SSystem has one?
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d_g_cps.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d_g_cyl.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_m3d_g_lin.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "SSystem/SComponent/c_m3d_g_pla.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d_g_sph.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_m3d_g_tri.cpp"), # debug weak func order
            Object(MatchingFor("ShieldD"), "SSystem/SComponent/c_m3d_g_vtx.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_lib.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SComponent/c_angle.cpp"),
            Object(MatchingFor(ALL_GCN), "SSystem/SStandard/s_basic.cpp"),
        ],
    },
    JSystemLib(
        "JFramework",
        [
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JFramework/JFWSystem.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JFramework/JFWDisplay.cpp"),
        ],
    ),
    JSystemLib(
        "J3DU",
        [
            Object(NonMatching, "JSystem/J3DU/J3DUPerf.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DU/J3DUClipper.cpp"),
            Object(NonMatching, "JSystem/J3DU/J3DUMotion.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/J3DU/J3DUDL.cpp"),
            Object(NonMatching, "JSystem/J3DU/J3DUFur.cpp"),
            Object(NonMatching, "JSystem/J3DU/J3DUShadow.cpp"),
            Object(NonMatching, "JSystem/J3DU/J3DUMtxCache.cpp"),
        ],
    ),
    JSystemLib(
        "JParticle",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAResourceManager.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAResource.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPABaseShape.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAExtraShape.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAChildShape.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAExTexShape.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPADynamicsBlock.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAFieldBlock.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JParticle/JPAKeyBlock.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JParticle/JPATexture.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAResourceLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAEmitterManager.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAEmitter.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAParticle.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JParticle/JPAMath.cpp"),
        ],
    ),
    JSystemLib(
        "JStage",
        [
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JStage/JSGActor.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JStage/JSGAmbientLight.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JStage/JSGCamera.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStage/JSGFog.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JStage/JSGLight.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JStage/JSGObject.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "JSystem/JStage/JSGSystem.cpp"),
        ],
    ),
    JSystemLib(
        "JStudio",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/ctb.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "JSystem/JStudio/JStudio/ctb-data.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/functionvalue.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/fvb.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JStudio/JStudio/fvb-data.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JStudio/JStudio/fvb-data-parse.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/jstudio-control.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JStudio/JStudio/jstudio-data.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/jstudio-math.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/jstudio-object.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "JSystem/JStudio/JStudio/object-id.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/stb.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JStudio/JStudio/stb-data-parse.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio/stb-data.cpp"),
        ],
    ),
    JSystemLib(
        "JStudio_JStage",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/control.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object-actor.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object-ambientlight.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object-camera.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object-fog.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JStage/object-light.cpp"),
        ],
    ),
    JSystemLib(
        "JStudio_JAudio2",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JAudio2/control.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JAudio2/object-sound.cpp"),
        ],
    ),
    JSystemLib(
        "JStudio_JParticle",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JParticle/control.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JStudio/JStudio_JParticle/object-particle.cpp"),
        ],
    ),
    JSystemLib(
        "JStudioCameraEditor",
        [
            Object(NonMatching, "JSystem/JStudio/JStudioCameraEditor/control.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioCameraEditor/controlset-csb-valueset.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioCameraEditor/csb.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioCameraEditor/csb-data.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioCameraEditor/sequence.cpp"),
        ],
    ),
    JSystemLib(
        "JStudioPreviewer",
        [
            Object(NonMatching, "JSystem/JStudio/JStudioPreviewer/control.cpp"),
        ],
    ),
    JSystemLib(
        "JStudioToolLibrary",
        [
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/anchor.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/console.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/controlset.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/controlset-anchor.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/controlset-preview.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/interface.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/scroll.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/visual.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/xml.cpp"),
            Object(NonMatching, "JSystem/JStudio/JStudioToolLibrary/jstudio-controlset-transform.cpp"),
        ],
    ),
    JSystemLib(
        "JAudio2",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASCalc.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASTaskThread.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JASDvdThread.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JASCallback.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASHeapCtrl.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASResArcLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASProbe.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASReport.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JASWaveFile.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JASWaveFileWav.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASCmdStack.cpp"),
            Object(Equivalent, "JSystem/JAudio2/JASTrack.cpp"), # weak func order
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASTrackPort.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASRegisterParam.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASSeqCtrl.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASSeqParser.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASSeqReader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASAramStream.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JASMidi.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASBank.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASBasicBank.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JASVoiceBank.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASBasicInst.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASDrumSet.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASBasicWaveBank.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASSimpleWaveBank.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JASInstRand.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASWSParser.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASBNKParser.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASWaveArcLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASChannel.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASLfo.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASOscillator.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASAiCtrl.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASAudioThread.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASAudioReseter.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JASDSPChannel.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASDSPInterface.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JASDriverIF.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JAudio2/JASSoundParams.cpp"), # debug weak func order
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/dspproc.cpp", extra_cflags=["-O4", "-func_align 32"]),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/dsptask.cpp", extra_cflags=["-O4", "-func_align 32"]),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/osdsp.cpp", extra_cflags=["-O4", "-func_align 32", "-str nopool"]),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/osdsp_task.cpp", extra_cflags=["-O4", "-func_align 32"]),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JAIAudible.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAIAudience.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISe.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISeMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISeq.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISeqDataMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISeqMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISound.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JAISoundChild.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISoundHandles.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISoundInfo.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISoundParams.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAISoundStarter.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAIStream.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAIStreamDataMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAIStreamMgr.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JAUAudience.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUAudioArcInterpreter.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUAudioArcLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUAudioMgr.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JAudio2/JAUBankTable.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUClusterSound.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUInitializer.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUSectionHeap.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUSeqCollection.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUSeqDataBlockMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUSoundAnimator.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JAUSoundMgr.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JAUSoundObject.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUSoundTable.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JAUStdSoundInfo.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JAudio2/JAUStreamFileTable.cpp"),
            Object(NonMatching, "JSystem/JAudio2/JAUWaveFilePlayer.cpp"),
        ],
    ),
    JSystemLib(
        "JAWExtSystem",
        [
            Object(Matching, "JSystem/JAWExtSystem/JAWSystem.cpp"),
            Object(NonMatching, "JSystem/JAWExtSystem/JAWWindow.cpp"),
            Object(Matching, "JSystem/JAWExtSystem/JAWWindow3D.cpp"),
            Object(NonMatching, "JSystem/JAWExtSystem/JAWGraphContext.cpp"),
            Object(NonMatching, "JSystem/JAWExtSystem/JAWExtSystem.cpp"),
        ],
    ),
    JSystemLib(
        "JAWWinLib",
        [
            Object(NonMatching, "JSystem/JAWWinLib/JAWSeqEdit.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWSoundTable.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWBankView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWHioBankEdit.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWHioReceiver.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWMidiTerm.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWSysMemView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWChView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWVolume.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWReportView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWPlayerChView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWPlaySeView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWEntrySeView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWTrackView.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWSpeAna.cpp"),
            Object(NonMatching, "JSystem/JAWWinLib/JAWSectionHeapView.cpp"),
        ],
    ),
    JSystemLib(
        "JMessage",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JMessage/control.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/JMessage/data.cpp"),
            Object(NonMatching, "JSystem/JMessage/data-parse.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JMessage/processor.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JMessage/resource.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JMessage/locale.cpp"),
        ],
    ),
    JSystemLib(
        "JAHostIO",
        [
            Object(Matching, "JSystem/JAHostIO/JAHioMessage.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHioMgr.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHioNode.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHioUtil.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHFrameNode.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHVirtualNode.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHUAsnData.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHUTableEdit.cpp"),
            Object(NonMatching, "JSystem/JAHostIO/JAHUFx.cpp"),
        ],
    ),
    JSystemLib(
        "JAHNodeLib",
        [
            Object(NonMatching, "JSystem/JAHNodeLib/JAHAudienceSettingNode.cpp"),
            Object(NonMatching, "JSystem/JAHNodeLib/JAHConductNode.cpp"),
            Object(NonMatching, "JSystem/JAHNodeLib/JAHIDSwapNode.cpp"),
            Object(NonMatching, "JSystem/JAHNodeLib/JAHSoundPlayerNode.cpp"),
            Object(NonMatching, "JSystem/JAHNodeLib/JAHViewerSys.cpp"),
            Object(NonMatching, "JSystem/JAHNodeLib/JAHViewerApp.cpp"),
        ],
    ),
    {
        "lib": "Z2AudioLib",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2Calc.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2AudioArcLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2Param.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundStarter.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundHandles.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SeMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SeqMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2StatusMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SceneMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2FxLineMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundInfo.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2Audience.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundObject.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SoundObjMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2Creature.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2LinkMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2EnvSeMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2WolfHowlMgr.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2SpeechMgr2.cpp"),
            Object(MatchingFor(ALL_GCN), "Z2AudioLib/Z2AudioMgr.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2DebugSys.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2F1TestWindow.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2SoundPlayer.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2WaveArcLoader.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2SeView.cpp"),
            Object(NonMatching, "Z2AudioLib/Z2TrackView.cpp"),
        ],
    },
    {
        "lib": "Z2AudioCSD",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": True,
        "objects": [
            Object(NonMatching, "Z2AudioCSD/SpkSpeakerCtrl.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkSystem.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkMixingBuffer.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkWave.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkTable.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkData.cpp"),
            Object(NonMatching, "Z2AudioCSD/SpkSound.cpp"),
            Object(NonMatching, "Z2AudioCSD/Z2AudioCS.cpp"),
        ],
    },
    {
        "lib": "gf",
        "mw_version": MWVersion(config.version),
        "cflags": [*cflags_noopt, "-O3"],
        "progress_category": "sdk",
        "objects": [
            Object(MatchingFor(ALL_GCN), "dolphin/gf/GFGeometry.cpp"),
            Object(MatchingFor(ALL_GCN), "dolphin/gf/GFLight.cpp"),
            Object(MatchingFor(ALL_GCN), "dolphin/gf/GFPixel.cpp"),
            Object(MatchingFor(ALL_GCN), "dolphin/gf/GFTev.cpp"),
        ],
    },
    JSystemLib(
        "JKernel",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRHeap.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRStdHeap.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRExpHeap.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRSolidHeap.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAssertHeap.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JKernel/JKRDisposer.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRThread.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAram.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAramHeap.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAramBlock.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAramPiece.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAramStream.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRFileLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRFileFinder.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRFileCache.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRArchivePub.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRArchivePri.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRMemArchive.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRAramArchive.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRDvdArchive.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRCompArchive.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRFile.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRDvdFile.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRRelocation.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRDvdRipper.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRDvdAramRipper.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JKernel/JKRDecomp.cpp"),
        ],
    ),
    JSystemLib(
        "JSupport",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JSupport/JSUList.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JSupport/JSUInputStream.cpp"),
            Object(MatchingFor(ALL_SHIELD), "JSystem/JSupport/JSUOutputStream.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JSupport/JSUMemoryStream.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JSupport/JSUFileStream.cpp"),
        ],
    ),
    JSystemLib(
        "JGadget",
        [
            Object(NonMatching, "JSystem/JGadget/define.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JGadget/binary.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JGadget/linklist.cpp"),
            Object(NonMatching, "JSystem/JGadget/search.cpp"),
            Object(NonMatching, "JSystem/JGadget/textreader.cpp"),
            Object(NonMatching, "JSystem/JGadget/xml-scanner.cpp"),
            Object(NonMatching, "JSystem/JGadget/std-list.cpp"),
            Object(NonMatching, "JSystem/JGadget/std-stream.cpp"),
            Object(NonMatching, "JSystem/JGadget/std-streambuf.cpp"),
            Object(NonMatching, "JSystem/JGadget/std-string.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JGadget/std-vector.cpp"),
            Object(NonMatching, "JSystem/JGadget/dolphin-stream-JORFile.cpp"),
        ],
    ),
    JSystemLib(
        "JUtility",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTCacheFont.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTResource.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTTexture.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTPalette.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTNameTab.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTGraphFifo.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTFont.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTResFont.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTDbPrint.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTGamePad.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTException.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTDirectPrint.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTAssert.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTVideo.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTXfb.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTFader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTProcBar.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTConsole.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JUtility/JUTDirectFile.cpp"),
            Object(MatchingFor(ALL_GCN, ALL_SHIELD), "JSystem/JUtility/JUTFontData_Ascfont_fix12.cpp"),
            Object(NonMatching, "JSystem/JUtility/JUTFontData_Ascfont_fix16.cpp"),
        ],
    ),
    JSystemLib(
        "J2DGraph",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DGrafContext.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DOrthoGraph.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DTevs.cpp"),
            Object(Equivalent, "JSystem/J2DGraph/J2DMaterial.cpp"), # weak func order
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DMatBlock.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DMaterialFactory.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DPrint.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DPane.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DScreen.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DWindow.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DPicture.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DTextBox.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DWindowEx.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DPictureEx.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DTextBoxEx.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DAnmLoader.cpp"),
            Object(NonMatching, "JSystem/J2DGraph/J2DAnmSaver.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DAnimation.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J2DGraph/J2DManage.cpp"),
        ],
    ),
    JSystemLib(
        "J3DGraphBase",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DGD.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DSys.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DVertex.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DTransform.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DTexture.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DPacket.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DShapeMtx.cpp"),
            Object(MatchingFor(ALL_GCN, "ShieldD"), "JSystem/J3DGraphBase/J3DShapeDraw.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DShape.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DMaterial.cpp"),
            Object(Equivalent, "JSystem/J3DGraphBase/J3DMatBlock.cpp"), # weak func order (J3DTevBlock)
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DTevs.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DDrawBuffer.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphBase/J3DStruct.cpp"),
        ],
    ),
    JSystemLib(
        "J3DGraphAnimator",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DShapeTable.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DJointTree.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DModelData.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DMtxBuffer.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DModel.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DAnimation.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DMaterialAnm.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DSkinDeform.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DCluster.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DJoint.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphAnimator/J3DMaterialAttach.cpp"),
        ],
    ),
    JSystemLib(
        "J3DGraphLoader",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DMaterialFactory.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DMaterialFactory_v21.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DClusterLoader.cpp"),
            Object(NonMatching, "JSystem/J3DGraphLoader/J3DBinaryFormat.cpp"),
            Object(NonMatching, "JSystem/J3DGraphLoader/J3DSaverUtility.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DModelLoader.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DModelLoaderCalcSize.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DJointFactory.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DShapeFactory.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/J3DGraphLoader/J3DAnmLoader.cpp"),
            Object(NonMatching, "JSystem/J3DGraphLoader/J3DModelSaver.cpp"),
        ],
    ),
    JSystemLib(
        "JMath",
        [
            Object(MatchingFor(ALL_GCN), "JSystem/JMath/JMath.cpp"),
            Object(MatchingFor(ALL_GCN, "Shield"), "JSystem/JMath/random.cpp"),
            Object(MatchingFor(ALL_GCN), "JSystem/JMath/JMATrigonometric.cpp"),
        ],
    ),
    JSystemLib(
        "JHostIO",
        [
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHIComm.cpp"),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHICommonMem.cpp"),
            Object(Equivalent, "JSystem/JHostIO/JORServer.cpp"), # debug weak func order
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JOREntry.cpp", extra_cflags=["-sym off"]), # debug weak func order
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JORFile.cpp", extra_cflags=["-sym off"]),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JORMessageBox.cpp"),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JORHostInfo.cpp", extra_cflags=["-sym off"]), # debug weak func order
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JORShellExecute.cpp"),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHIMemBuf.cpp"),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHIhioASync.cpp", extra_cflags=["-sym off"]),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHIMccBuf.cpp", extra_cflags=["-sym off"]),
            Object(MatchingFor("ShieldD"), "JSystem/JHostIO/JHIRMcc.cpp"),
        ],
    ),
    DolphinLib(
        "base",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/os/__start.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OS.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSAlarm.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSAlloc.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSArena.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSAudioSystem.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSCache.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSContext.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSError.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSExec.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSFont.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSInterrupt.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSLink.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSMessage.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSMemory.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSMutex.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSReboot.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSReset.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSResetSW.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSRtc.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSSync.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSThread.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/OSTime.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/os/__ppc_eabi_init.cpp"),
        ],
    ),
    {
        "lib": "exi",
        "mw_version": "GC/1.2.5n",
        "cflags": [*cflags_noopt, "-ir src/dolphin"],
        "progress_category": "sdk",
        "objects": [
            Object(MatchingFor(ALL_GCN), "dolphin/exi/EXIBios.c", extra_cflags=["-O3,p"]),
            Object(MatchingFor(ALL_GCN), "dolphin/exi/EXIUart.c", extra_cflags=["-O4,p"]),
        ],
    },
    DolphinLib(
        "si",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/si/SIBios.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/si/SISamplingRate.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/db/db.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/mtx/mtx.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/mtx/mtxvec.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/mtx/mtx44.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/mtx/vec.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/mtx/quat.c"),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvdlow.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvdfs.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvd.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvdqueue.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvderror.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvdidutils.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/dvdFatal.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/dvd/fstload.c", extra_cflags=["-char signed"]),
        ],
    ),
    DolphinLib(
        "vi",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/vi/vi.c"),
        ],
    ),
    DolphinLib(
        "pad",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/pad/Padclamp.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/ar/ar.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/dsp/dsp.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/dsp/dsp_debug.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "card",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDBios.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDUnlock.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDRdwr.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDBlock.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDDir.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDCheck.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDMount.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDFormat.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDOpen.c", extra_cflags=["-char signed"]),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDCreate.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDRead.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDWrite.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDStat.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/card/CARDNet.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXInit.c", extra_cflags=["-opt nopeephole"]),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXFifo.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXAttr.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXMisc.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXGeometry.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXFrameBuf.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXLight.c", extra_cflags=["-fp_contract off"]),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXTexture.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXBump.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXTev.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXPixel.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXDisplayList.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXTransform.c", extra_cflags=["-fp_contract off"]),
            Object(MatchingFor(ALL_GCN), "dolphin/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "gd",
        [
            Object(MatchingFor(ALL_GCN), "dolphin/gd/GDBase.c"),
            Object(MatchingFor(ALL_GCN), "dolphin/gd/GDGeometry.c"),
        ],
    ),
    RevolutionLib(
        "gf",
        [
            Object(NonMatching, "revolution/gf/GFGeometry.cpp"),
            Object(NonMatching, "revolution/gf/GFLight.cpp"),
            Object(NonMatching, "revolution/gf/GFPixel.cpp"),
            Object(NonMatching, "revolution/gf/GFTev.cpp"),
        ],
    ),
    RevolutionLib(
        "aralt",
        [
            Object(NonMatching, "revolution/aralt/aralt.c"),
        ],
    ),
    RevolutionLib(
        "base",
        [
            Object(NonMatching, "revolution/base/PPCArch.c"),
        ],
    ),
    RevolutionLib(
        "os",
        [
            Object(NonMatching, "revolution/os/__start.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OS.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSAddress.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSAlarm.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSAlloc.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSArena.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSAudioSystem.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSCache.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSContext.c"),
            Object(NonMatching, "revolution/os/OSError.c"),
            Object(NonMatching, "revolution/os/OSExec.c"),
            Object(NonMatching, "revolution/os/OSFatal.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSFont.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSInterrupt.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSLink.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSMessage.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSMemory.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSMutex.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSReboot.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSReset.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSRtc.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSStopwatch.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSSync.c"),
            Object(NonMatching, "revolution/os/OSThread.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSTime.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSUtf.c"),
            Object(MatchingFor("ShieldD", "RZDE01_00"), "revolution/os/OSIpc.c"),
            Object(NonMatching, "revolution/os/OSStateTM.c"),
            Object(NonMatching, "revolution/os/OSPlayRecord.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSStateFlags.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSNet.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSNandbootInfo.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSPlayTime.c"),
            Object(MatchingFor("ShieldD"), "revolution/os/OSLaunch.c"),
            Object(NonMatching, "revolution/os/__ppc_eabi_init.cpp"),
        ],
    ),
    RevolutionLib(
        "exi",
        [
            Object(NonMatching, "revolution/exi/EXIBios.c"),
            Object(NonMatching, "revolution/exi/EXIUart.c"),
            Object(NonMatching, "revolution/exi/EXICommon.c"),
        ],
    ),
    RevolutionLib(
        "si",
        [
            Object(NonMatching, "revolution/si/SIBios.c"),
            Object(NonMatching, "revolution/si/SISamplingRate.c"),
        ],
    ),
    RevolutionLib(
        "vi",
        [
            Object(NonMatching, "revolution/vi/vi.c"),
            Object(NonMatching, "revolution/vi/i2c.c"),
            Object(NonMatching, "revolution/vi/vi3in1.c"),
        ],
    ),
    RevolutionLib(
        "mtx",
        [
            Object(NonMatching, "revolution/mtx/mtx.c"),
            Object(NonMatching, "revolution/mtx/mtxvec.c"),
            Object(NonMatching, "revolution/mtx/mtx44.c"),
            Object(NonMatching, "revolution/mtx/vec.c"),
            Object(NonMatching, "revolution/mtx/quat.c"),
        ],
    ),
    RevolutionLib(
        "gx",
        [
            Object(NonMatching, "revolution/gx/GXInit.c", extra_cflags=["-opt nopeephole"]),
            Object(NonMatching, "revolution/gx/GXFifo.c"),
            Object(NonMatching, "revolution/gx/GXAttr.c"),
            Object(NonMatching, "revolution/gx/GXMisc.c"),
            Object(NonMatching, "revolution/gx/GXGeometry.c"),
            Object(NonMatching, "revolution/gx/GXFrameBuf.c"),
            Object(NonMatching, "revolution/gx/GXLight.c", extra_cflags=["-fp_contract off"]),
            Object(NonMatching, "revolution/gx/GXTexture.c"),
            Object(NonMatching, "revolution/gx/GXBump.c"),
            Object(NonMatching, "revolution/gx/GXTev.c"),
            Object(NonMatching, "revolution/gx/GXPixel.c"),
            Object(NonMatching, "revolution/gx/GXDraw.c"),
            Object(NonMatching, "revolution/gx/GXDisplayList.c"),
            Object(NonMatching, "revolution/gx/GXVert.c"),
            Object(NonMatching, "revolution/gx/GXTransform.c", extra_cflags=["-fp_contract off"]),
            Object(NonMatching, "revolution/gx/GXVerify.c"),
            Object(NonMatching, "revolution/gx/GXVerifXF.c"),
            Object(NonMatching, "revolution/gx/GXVerifRAS.c"),
            Object(NonMatching, "revolution/gx/GXSave.c"),
            Object(NonMatching, "revolution/gx/GXPerf.c"),
        ],
    ),
    RevolutionLib(
        "dvd",
        [
            Object(NonMatching, "revolution/dvd/dvdfs.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvd.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvdqueue.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvderror.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvdidutils.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvdFatal.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvdDeviceError.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/dvd/dvd_broadway.c", extra_cflags=["-char signed"]),
        ],
    ),
    RevolutionLib(
        "ai",
        [
            Object(NonMatching, "revolution/ai/ai.c"),
        ],
    ),
    RevolutionLib(
        "dsp",
        [
            Object(NonMatching, "revolution/dsp/dsp.c"),
            Object(NonMatching, "revolution/dsp/dsp_debug.c"),
            Object(NonMatching, "revolution/dsp/dsp_task.c"),
        ],
    ),
    RevolutionLib(
        "card",
        [
            Object(NonMatching, "revolution/card/CARDBios.c"),
            Object(NonMatching, "revolution/card/CARDUnlock.c"),
            Object(NonMatching, "revolution/card/CARDRdwr.c"),
            Object(NonMatching, "revolution/card/CARDBlock.c"),
            Object(NonMatching, "revolution/card/CARDDir.c"),
            Object(NonMatching, "revolution/card/CARDCheck.c"),
            Object(NonMatching, "revolution/card/CARDMount.c"),
            Object(NonMatching, "revolution/card/CARDFormat.c"),
            Object(NonMatching, "revolution/card/CARDOpen.c", extra_cflags=["-char signed"]),
            Object(NonMatching, "revolution/card/CARDCreate.c"),
            Object(NonMatching, "revolution/card/CARDRead.c"),
            Object(NonMatching, "revolution/card/CARDWrite.c"),
            Object(NonMatching, "revolution/card/CARDStat.c"),
            Object(NonMatching, "revolution/card/CARDNet.c"),
        ],
    ),
    RevolutionLib(
        "hio2",
        [
            Object(NonMatching, "revolution/hio2/hio2.c"),
        ],
    ),
    RevolutionLib(
        "nand",
        [
            Object(NonMatching, "revolution/nand/nand.c"),
            Object(NonMatching, "revolution/nand/NANDOpenClose.c"),
            Object(NonMatching, "revolution/nand/NANDCore.c"),
            Object(NonMatching, "revolution/nand/NANDCheck.c"),
            Object(NonMatching, "revolution/nand/NANDLogging.c"),
            Object(NonMatching, "revolution/nand/NANDErrorMessage.c"),
        ],
    ),
    RevolutionLib(
        "sc",
        [
            Object(NonMatching, "revolution/sc/scsystem.c"),
            Object(NonMatching, "revolution/sc/scapi.c"),
            Object(NonMatching, "revolution/sc/scapi_prdinfo.c"),
        ],
    ),
    RevolutionLib(
        "wenc",
        [
            Object(NonMatching, "revolution/wenc/wenc.c"),
        ],
    ),
    RevolutionLib(
        "esp",
        [
            Object(NonMatching, "revolution/esp/esp.c"),
        ],
    ),
    RevolutionLib(
        "ipc",
        [
            Object(NonMatching, "revolution/ipc/ipcMain.c"),
            Object(NonMatching, "revolution/ipc/ipcclt.c"),
            Object(NonMatching, "revolution/ipc/memory.c"),
            Object(NonMatching, "revolution/ipc/ipcProfile.c"),
        ],
    ),
    RevolutionLib(
        "fs",
        [
            Object(NonMatching, "revolution/fs/fs.c"),
        ],
    ),
    RevolutionLib(
        "pad",
        [
            Object(NonMatching, "revolution/pad/Padclamp.c"),
            Object(NonMatching, "revolution/pad/Pad.c"),
        ],
    ),
    RevolutionLib(
        "wpad",
        [
            Object(NonMatching, "revolution/wpad/WPAD.c"),
            Object(NonMatching, "revolution/wpad/WUD.c"),
        ],
    ),
    RevolutionLib(
        "euart",
        [
            Object(NonMatching, "revolution/euart/euart.c"),
        ],
    ),
    RevolutionLib(
        "gd",
        [
            Object(NonMatching, "revolution/gd/GDBase.c"),
            Object(NonMatching, "revolution/gd/GDGeometry.c"),
        ],
    ),
    RevolutionLib(
        "homebuttonLib",
        [
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/db/db_assert.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/db/db_console.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/db/db_DbgPrintBase.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/db/db_directPrint.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/db/db_mapFile.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_animation.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_arcResourceAccessor.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_bounding.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_common.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_drawInfo.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_group.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_layout.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_material.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_pane.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_picture.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_resourceAccessor.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_textBox.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/lyt/lyt_window.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/math/math_triangular.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/snd/snd_SoundArchivePlayer.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/snd/snd_SoundHandle.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/snd/snd_SoundPlayer.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/snd/snd_SoundStartable.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_binaryFileFormat.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_CharStrmReader.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_CharWriter.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_Font.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_LinkList.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_list.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_ResFont.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_ResFontBase.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_TagProcessorBase.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/nw4hbm/ut/ut_TextWriterBase.cpp"),

            Object(NonMatching, "revolution/homebuttonLib/HBMBase.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/HBMAnmController.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/HBMFrameController.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/HBMGUIManager.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/HBMController.cpp"),
            Object(NonMatching, "revolution/homebuttonLib/HBMRemoteSpk.cpp"),
        ],
    ),
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/GCN_mem_alloc.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/__mem.c"),
            Object(MatchingFor(ALL_GCN, "Shield"), "PowerPC_EABI_Support/Runtime/Src/__va_arg.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/CPlusLibPPC.cp"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/NMWException.cp", extra_cflags=["-Cpp_exceptions on"]),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/ptmf.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/runtime.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/Gecko_ExceptionPPC.cp"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/Runtime/Src/GCN_Mem_Alloc.c", extra_cflags=["-str reuse,nopool,readonly"]),
        ],
    },
    {
        "lib": "MSL_C",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/abort_exit.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/alloc.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/errno.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ansi_files.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/ansi_fp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/math_sun.c"),
            Object(MatchingFor(ALL_GCN, "Shield"), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/arith.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/buffer_io.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/char_io.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/critical_regions.gamecube.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ctype.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/locale.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/direct_io.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/file_io.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/FILE_POS.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mbstring.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem_funcs.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/math_api.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/misc_io.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/printf.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/scanf.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/float.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/signal.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/string.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtold.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wctype.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtoul.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wstring.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wchar_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/secure_error.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/math_double.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/uart_console_io_gcn.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_exp.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_fmod.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log10.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ceil.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_exp.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_fmod.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log10.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/abort_exit_ppc_eabi.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/math_ppc.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.c"),
            Object(MatchingFor(ALL_GCN), "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/extras.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_trk,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            # debugger
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mainloop.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubevent.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubinit.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msgbuf.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/serpoll.c", extra_cflags=["-sdata 8"]),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/string_TRK.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/usr_put.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/dispatch.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msghndlr.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/support.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mutex_TRK.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mem_TRK.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/targimpl.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Export/targsupp.s"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/exception.s"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/main_TRK.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/targcont.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/target_options.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Export/mslsupp.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/UDP_Stubs.c"),

            # gamedev
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_DDH_GCN/main.c", extra_cflags=["-sdata 8"]),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/CircleBuffer.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN/main.c", extra_cflags=["-sdata 8"]),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/MWTrace.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/gc/cc_gdev.c"),
            Object(MatchingFor(ALL_GCN), "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/gc/MWCriticalSection_gc.c"),
        ],
    },
    {
        "lib": "amcstubs",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(MatchingFor(ALL_GCN), "amcstubs/AmcExi2Stubs.c"),
        ],
    },
    {
        "lib": "odemuexi2",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(MatchingFor(ALL_GCN), "odemuexi2/DebuggerDriver.c"),
        ],
    },
    {
        "lib": "odenotstub",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(MatchingFor(ALL_GCN), "odenotstub/odenotstub.c"),
        ],
    },
    {
        "lib": "NdevExi2AD",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(NonMatching, "NdevExi2AD/DebuggerDriver.c"),
            Object(NonMatching, "NdevExi2AD/exi2.c"),
        ],
    },
    {
        "lib": "lingcod",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_framework,
        "progress_category": "third_party",
        "host": False,
        "objects": [
            Object(MatchingFor("Shield"), "lingcod/LingcodPatch.c"),
        ],
    },

    # Begin RELs
    {
        "lib": "REL",
        "mw_version": MWVersion(config.version),
        "cflags": cflags_rel,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(MatchingFor(ALL_GCN), "REL/executor.c"),
            Object(
                MatchingFor(ALL_GCN),
                "REL/global_destructor_chain.c",
                source="PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c",
            ),
        ],
    },
    Rel("f_pc_profile_lst", [Object(MatchingFor(ALL_GCN, ALL_SHIELD), "f_pc/f_pc_profile_lst.cpp")]),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_andsw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_bg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_bg_obj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_dmidna"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_dbdoor00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_knob00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_shutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_spiral"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_dshutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_ep"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_hitobj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag04"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_kytag17"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_brakeeff"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_burnbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_carry"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ito"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_movebox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swpush"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_timer"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_path_line"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_scene_exit"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_set_bgobj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_swhit0"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_allmato"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_camera"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_chkpoint"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_event"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_evt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_evtarea"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_evtmsg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_howl"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_kmsg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_lantern"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_mist"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_msg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_push"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_telop"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tbox2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_vrbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_vrbox2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_arrow"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_boomerang"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_crod"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_demo00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_disappear"),
    ActorRel(Equivalent, "d_a_mg_rod"), # weak func order
    ActorRel(MatchingFor(ALL_GCN), "d_a_midna"),
    ActorRel(Equivalent, "d_a_nbomb"), # weak func order
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_life_container"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_yousei"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_spinner"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_suspend"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_attention"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_alldie"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_andsw2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_bd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_canoe"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_cstaF"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_demo_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_bossL1"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_dn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_fm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ga"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_nest"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_rd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_econt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_fr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_grass"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_kytag05"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag10"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag11"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag14"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_mg_fish"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_besu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_fairy_seirei"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_fish"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_henna"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kakashi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kkri"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kolin"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_maro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_taro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_tkj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bhashi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bkdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bosswarp"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cboard"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_digplace"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_eff"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fmobj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gpTaru"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hhashi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kanban2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kbacket"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kgate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_klift00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ktOnFire"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ladder"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv2Candle"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_magne_arm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_metalbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mgate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_nameplate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ornament_cloth"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rope_bridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sWallShutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stick"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stoneMark"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swpropeller"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swpush5"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_yobikusa"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_scene_exit2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_shop_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_sq"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_swc00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_CstaSw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_ajnot"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_attack_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_gstart"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_hinit"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_hjump"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_hstop"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_lv2prchk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_magne"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_mhint"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_mstop"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_spring"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_statue_evt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_ykgr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_L7demo_dr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_L7low_dr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_L7op_demo_dr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_bh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_bq"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_dr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_dre"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_ds"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_gg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_gm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_gnd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_go"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_gos"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_mgn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_ob"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_oh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_oh2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_tn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_yo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_yo_ice"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_zant"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_zant_magic"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_zant_mobile"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_b_zant_sima"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_balloon_2D"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_bullet"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_coach_2D"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_coach_fire"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_cow"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_cstatue"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_do"), # Z2SoundObjSimple dtor
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_boss"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_bossL5"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_mbossL1"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_door_push"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ai"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_arrow"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ba"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bee"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bi_leaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_bug"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_cr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_cr_egg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_db"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_db_leaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_dd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_df"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_dk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_dt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_fb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_fk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_fs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_fz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_gb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_gi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_gm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_gob"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_gs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hb_leaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hp"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_hzelda"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_is"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_kg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_kk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_kr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_md"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mk_bo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_mm_mt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ms"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_nz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_oc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_oct_bg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ot"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ph"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_pm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_po"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_pz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_rb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_rdb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_rdy"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_s1"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sm2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_st"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_st_line"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_sw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_th"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_th_ball"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_tk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_tk2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_tk_ball"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_tt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_vt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_warpappear"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_wb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ws"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ww"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yd_leaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ym"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_e_ym_tag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_ymb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_yr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_zh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_zm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_e_zs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_formation_mng"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_guard_mng"),
    ActorRel(Equivalent, "d_a_horse"), # weak func order (J3DMtxCalcNoAnm)
    ActorRel(Equivalent, "d_a_hozelda"), # weak func order (J3DMtxCalcNoAnm)
    ActorRel(MatchingFor(ALL_GCN), "d_a_izumi_gate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kago"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag01"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag02"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag03"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag06"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag07"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag08"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag09"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag12"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag13"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag15"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_kytag16"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_mant"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_mg_fshop"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_mirror"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_movie_player"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_myna"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_ni"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_aru"),
    ActorRel(Equivalent, "d_a_npc_ash"), # weak func order (sinShort)
    ActorRel(Equivalent, "d_a_npc_ashB"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_bans"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_blue_ns"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_bou"),
    ActorRel(Equivalent, "d_a_npc_bouS"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_cdn3"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_chat"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_chin"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_clerka"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_clerkb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_clerkt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_coach"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_df"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_doc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_doorboy"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_drainSol"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_du"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_fairy"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_fguard"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_gnd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_gra"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grd"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grmc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_gro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_grz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_guard"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_gwolf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_hanjo"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_npc_henna0"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_hoz"),
    ActorRel(Equivalent, "d_a_npc_impal"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_inko"),
    ActorRel(Equivalent, "d_a_npc_ins"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_jagar"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kasi_hana"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kasi_kyu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kasi_mich"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_npc_kdk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_knj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kolinb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_ks"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_kyury"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_len"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_lf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_lud"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_midp"),
    ActorRel(MatchingFor(ALL_GCN, ALL_SHIELD), "d_a_npc_mk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_moi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_moir"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_myna2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_ne"),
    ActorRel(MatchingFor(ALL_GCN, ALL_SHIELD), "d_a_npc_p2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_pachi_besu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_pachi_maro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_pachi_taro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_passer"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_passer2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_post"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_pouya"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_prayer"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_raca"),
    ActorRel(Equivalent, "d_a_npc_rafrel"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_saru"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seib"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seic"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seid"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seira"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seira2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_seirei"),
    ActorRel(Equivalent, "d_a_npc_shad"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_shaman"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_shoe"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_shop0"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_shop_maro"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_sola"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_soldierA"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_soldierB"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_sq"),
    ActorRel(Equivalent, "d_a_npc_the"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_theB"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_tk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_tkc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_tkj2"),
    ActorRel(Equivalent, "d_a_npc_tks"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_toby"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_tr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_uri"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_worm"),
    ActorRel(Equivalent, "d_a_npc_wrestler"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_yamid"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_yamis"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_yamit"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_yelia"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_ykm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_ykw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zanb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zant"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zelR"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zelRo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zelda"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zra"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zrc"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_npc_zrz"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_Lv5Key"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_Turara"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_TvCdlst"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_Y_taihou"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_amiShutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ari"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_automata"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_avalanche"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_balloon"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_barDesk"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_batta"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bed"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bemos"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bhbridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bk_leaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bky_rock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bmWindow"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bmshutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bombf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_boumato"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_brg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bsGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_bubblePilar"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_catdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cb"),
    ActorRel(Equivalent, "d_a_obj_cblock"), # weird weak data issue - needs a ...data pool, but no functions use it
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_chandelier"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_chest"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cho"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cowdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crope"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvfence"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvgate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvhahen"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvlh_down"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvlh_up"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crvsteel"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_crystal"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_cwall"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_obj_damCps"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_dan"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_digholl"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_digsnow"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_dmelevator"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_drop"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_dust"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_enemy_create"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fallobj"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fan"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fchain"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fireWood"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fireWood2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_firepillar"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_firepillar2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_flag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_flag2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_flag3"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_food"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_fw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gadget"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ganonwall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ganonwall2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_geyser"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_glowSphere"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_goGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gomikabe"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gra2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_graWall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_gra_rock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_grave_stone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_groundwater"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_grz_rock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_h_saku"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hakai_brl"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hakai_ftr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hasu2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hata"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hb"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hbombkoya"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_heavySw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hfuta"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_hsTarget"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ice_l"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ice_s"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_iceblock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_iceleaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ihasi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ikada"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_inobone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ita"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_itamato"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kabuto"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kage"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kago"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kaisou"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kamakiri"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kantera"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_katatsumuri"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kazeneko"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_key"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_keyhole"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ki"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kiPot"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kita"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kjgjs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kkanban"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_knBullet"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kshutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kuwagata"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kwheel00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kwheel01"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_kznkarm"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_laundry"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_laundry_rope"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lp"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv1Candle00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv1Candle01"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3Candle"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3Water"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3Water2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3WaterB"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3saka00"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv3waterEff"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4CandleDemoTag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4CandleTag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4EdShutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4Gate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4HsTarget"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4PoGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4RailWall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4SlideWall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4bridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4chandelier"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4digsand"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4floor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4gear"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4prelvtr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4prwall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv4sand"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv5FloorBoard"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv5IceWall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv5SwIce"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv5ychndlr"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv5yiblltray"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6ChangeGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6FurikoTrap"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6Lblock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6SwGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6SzGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6Tenbin"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6TogeRoll"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6TogeTrap"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6bemos"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6bemos2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6egate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6elevta"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv6swturn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv7BsGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv7PropellerY"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv7bridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv8KekkaiTrap"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv8Lift"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv8OptiLift"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv8UdFloor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_lv9SwShutter"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_magLift"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_magLiftRot"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_maki"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_master_sword"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mato"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mhole"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mie"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mirror_6pole"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mirror_chain"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mirror_sand"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mirror_screw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mirror_table"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_msima"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_mvstair"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_myogan"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_nagaisu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_nan"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ndoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_nougu"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_octhashi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_oiltubo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_onsen"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_onsenFire"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_onsenTaru"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pdtile"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pdwall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_picture"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pillar"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pleaf"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_poCandle"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_poFire"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_poTbox"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_prop"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_pumpkin"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rcircle"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rfHole"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rgate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_riverrock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rotBridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rotTrap"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_roten"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rstair"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_rw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_saidan"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sakuita"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sakuita_rope"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_scannon"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_scannon_crs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_scannon_ten"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sekidoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sekizo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sekizoa"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_shield"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sm_door"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_smallkey"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_smgdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_smoke"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_smtile"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_smw_stone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_snowEffTag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_snow_soup"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_so"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_spinLift"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ss_drink"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ss_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stairBlock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stopper"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_stopper2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_suisya"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swBallA"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swBallB"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swBallC"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swLight"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swchain"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swhang"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_sword"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swpush2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swspinner"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_swturn"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_syRock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_szbridge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_taFence"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_table"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_takaraDai"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tatigi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ten"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_testcube"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tgake"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_thashi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_thdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_timeFire"),
    ActorRel(Equivalent, "d_a_obj_tks"), # weak func order (sinShort)
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tmoon"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_toaru_maki"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_toby"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tobyhouse"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_togeTrap"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tombo"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tornado"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tornado2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_tp"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_treesh"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_twGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_udoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_usaku"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_vground"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_volcball"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_volcbom"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_warp_kbrg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_warp_obrg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_waterGate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_waterPillar"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_waterfall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wchain"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wdStick"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_web0"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_web1"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_well_cover"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wflag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wind_stone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_window"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wood_pendulum"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wood_statue"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_wsword"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_yel_bag"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_ystone"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zcloth"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zdoor"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zrTurara"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zrTuraraRock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zraMark"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zra_freeze"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_obj_zra_rock"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_passer_mng"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_arena"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_peru"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_ppolamp"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_skip_2D"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_startAndGoal"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_swBall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_swLBall"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_swTime"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_Lv6Gate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_Lv7Gate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_Lv8Gate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_TWgate"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_lv6CstaSw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_assistance"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_bottle_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_chgrestart"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_csw"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_escape"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_firewall"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_gra"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_guard"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_instruction"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_kago_fall"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_lightball"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_lv5soup"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_mmsg"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_mwait"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_myna2"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_myna_light"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_pachi"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_poFire"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_qs"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_ret_room"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_river_back"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_rmbit_sw"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_schedule"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tag_setBall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_setrestart"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_shop_camera"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_shop_item"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_smk_emt"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_spinner"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_sppath"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_ss_drink"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_stream"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_theB_hint"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_wara_howl"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_watchge"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_waterfall"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_wljump"), # debug weak func order
    ActorRel(MatchingFor(ALL_GCN), "d_a_tag_yami"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_talk"),
    ActorRel(MatchingFor(ALL_GCN, "Shield"), "d_a_tboxSw"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_title"),
    ActorRel(MatchingFor(ALL_GCN), "d_a_warp_bug"),
]


# Define our custom asset processing scripts
config.custom_build_rules = [
    {
        "name": "convert_matDL",
        "command": "$python tools/converters/matDL_dis.py $in $out --symbol $symbol --scope $scope",
        "description": "CONVERT $symbol",
    },
]
config.custom_build_steps = {}

# Grab the specific GameID so we can format our strings properly
version = VERSIONS[version_num]
out_dir = config.build_dir / version


# This generates the build steps needed for preprocessing
def emit_build_rule(asset: Dict[str, Any]) -> None:
    assert config.custom_build_steps is not None
    steps = config.custom_build_steps.setdefault("pre-compile", [])
    custom_data = asset.get("custom_data") or {}

    match asset.get("custom_type"):
        case None:
            return

        case "matDL":
            steps.append(
                {
                    "rule": "convert_matDL",
                    "inputs": out_dir / "bin" / asset["binary"],
                    "outputs": out_dir / "include" / asset["header"],
                    "variables": {
                        "symbol": asset["symbol"],
                        "scope": custom_data.get("scope", "local")
                    },
                    "implicit": Path("tools/converters/matDL_dis.py"),
                }
            )

        case _:
            print("Unknown asset type: " + asset["custom_type"])


# Parse the config and create the build rules for all our assets
config_path = out_dir / "config.json"
if config_path.exists():
    config_data = json.load(open(config_path))
    for asset in config_data.get("extract", []):
        emit_build_rule(asset)
    for module in config_data.get("modules", []):
        for asset in module.get("extract", []):
            emit_build_rule(asset)

# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback

# Optional extra categories for progress tracking
config.progress_categories = [
    ProgressCategory("game", "TP Game Code"),
    ProgressCategory("core", "Core Engine"),
    ProgressCategory("sdk", "SDK"),
    ProgressCategory("third_party", "Third Party"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
