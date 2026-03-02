###
# decomp-toolkit project generator
# Generates build.ninja and objdiff.json.
#
# This generator is intentionally project-agnostic
# and shared between multiple projects. Any configuration
# specific to a project should be added to `configure.py`.
#
# If changes are made, please submit a PR to
# https://github.com/encounter/dtk-template
###

import io
import json
import math
import os
import platform
import sys
from pathlib import Path
from typing import (
    IO,
    Any,
    Callable,
    Dict,
    Iterable,
    List,
    Optional,
    Set,
    Tuple,
    TypedDict,
    Union,
    cast,
)

from . import ninja_syntax
from .ninja_syntax import serialize_path

if sys.platform == "cygwin":
    sys.exit(
        f"Cygwin/MSYS2 is not supported."
        f"\nPlease use native Windows Python instead."
        f"\n(Current path: {sys.executable})"
    )

Library = Dict[str, Any]


PrecompiledHeader = Dict[str, Any]


class Object:
    def __init__(self, completed: bool, name: str, **options: Any) -> None:
        self.name = name
        self.completed = completed
        self.options: Dict[str, Any] = {
            "add_to_all": None,
            "asflags": None,
            "asm_dir": None,
            "cflags": None,
            "extab_padding": None,
            "extra_asflags": [],
            "extra_cflags": [],
            "extra_clang_flags": [],
            "lib": None,
            "mw_version": None,
            "progress_category": None,
            "scratch_preset_id": None,
            "shift_jis": None,
            "source": name,
            "src_dir": None,
            "strip_prefix": None,
        }
        self.options.update(options)

        # Internal
        self.src_path: Optional[Path] = None
        self.asm_path: Optional[Path] = None
        self.src_obj_path: Optional[Path] = None
        self.asm_obj_path: Optional[Path] = None
        self.ctx_path: Optional[Path] = None

    def resolve(self, config: "ProjectConfig", lib: Library) -> "Object":
        # Use object options, then library options
        obj = Object(self.completed, self.name, **lib)
        for key, value in self.options.items():
            if value is not None or key not in obj.options:
                obj.options[key] = value

        # Use default options from config
        def set_default(key: str, value: Any) -> None:
            if obj.options[key] is None:
                obj.options[key] = value

        set_default("add_to_all", True)
        set_default("asflags", config.asflags)
        set_default("asm_dir", config.asm_dir)
        set_default("extab_padding", None)
        set_default("mw_version", config.linker_version)
        set_default("scratch_preset_id", config.scratch_preset_id)
        set_default("shift_jis", config.shift_jis)
        set_default("src_dir", config.src_dir)

        # Validate progress categories
        def check_category(category: str):
            if not any(category == c.id for c in config.progress_categories):
                sys.exit(
                    f"Progress category '{category}' missing from config.progress_categories"
                )

        progress_category = obj.options["progress_category"]
        if isinstance(progress_category, list):
            for category in progress_category:
                check_category(category)
        elif progress_category is not None:
            check_category(progress_category)

        # Resolve paths
        build_dir = config.out_path()
        source = obj.options["source"]
        if obj.options["strip_prefix"]:
            source = source.removeprefix(obj.options["strip_prefix"])
        obj.src_path = Path(obj.options["src_dir"]) / source
        if obj.options["asm_dir"] is not None:
            obj.asm_path = (
                Path(obj.options["asm_dir"]) / source
            ).with_suffix(".s")
        base_name = Path(self.name).with_suffix("")
        obj.src_obj_path = build_dir / "src" / f"{base_name}.o"
        obj.asm_obj_path = build_dir / "mod" / f"{base_name}.o"
        obj.ctx_path = build_dir / "src" / f"{base_name}.ctx"
        return obj


class ProgressCategory:
    def __init__(self, id: str, name: str) -> None:
        self.id = id
        self.name = name


class ProjectConfig:
    def __init__(self) -> None:
        # Paths
        self.build_dir: Path = Path("build")  # Output build files
        self.src_dir: Path = Path("src")  # C/C++/asm source files
        self.tools_dir: Path = Path("tools")  # Python scripts
        self.asm_dir: Optional[Path] = Path(
            "asm"
        )  # Override incomplete objects (for modding)

        # Tooling
        self.binutils_tag: Optional[str] = None  # Git tag
        self.binutils_path: Optional[Path] = None  # If None, download
        self.dtk_tag: Optional[str] = None  # Git tag
        self.dtk_path: Optional[Path] = None  # If None, download
        self.compilers_tag: Optional[str] = None  # 1
        self.compilers_path: Optional[Path] = None  # If None, download
        self.wibo_tag: Optional[str] = None  # Git tag
        self.wrapper: Optional[Path] = None  # If None, download wibo on Linux
        self.sjiswrap_tag: Optional[str] = None  # Git tag
        self.sjiswrap_path: Optional[Path] = None  # If None, download
        self.ninja_path: Optional[Path] = None  # If None, use system PATH
        self.objdiff_tag: Optional[str] = None  # Git tag
        self.objdiff_path: Optional[Path] = None  # If None, download

        # Project config
        self.non_matching: bool = False
        self.build_rels: bool = True  # Build REL files
        self.check_sha_path: Optional[Path] = None  # Path to version.sha1
        self.config_path: Optional[Path] = None  # Path to config.yml
        self.generate_map: bool = False  # Generate map file(s)
        self.asflags: Optional[List[str]] = None  # Assembler flags
        self.ldflags: Optional[List[str]] = None  # Linker flags
        self.libs: Optional[List[Library]] = None  # List of libraries
        self.precompiled_headers: Optional[List[PrecompiledHeader]] = (
            None  # List of precompiled headers
        )
        self.linker_version: Optional[str] = None  # mwld version
        self.version: Optional[str] = None  # Version name
        self.warn_missing_config: bool = False  # Warn on missing unit configuration
        self.warn_missing_source: bool = False  # Warn on missing source file
        self.rel_strip_partial: bool = True  # Generate PLFs with -strip_partial
        self.rel_empty_file: Optional[str] = (
            None  # Object name for generating empty RELs
        )
        self.shift_jis = (
            True  # Convert source files from UTF-8 to Shift JIS automatically
        )
        self.reconfig_deps: Optional[List[Path]] = (
            None  # Additional re-configuration dependency files
        )
        self.custom_build_rules: Optional[List[Dict[str, Any]]] = (
            None  # Custom ninja build rules
        )
        self.custom_build_steps: Optional[Dict[str, List[Dict[str, Any]]]] = (
            None  # Custom build steps, types are ["pre-compile", "post-compile", "post-link", "post-build"]
        )
        self.generate_compile_commands: bool = (
            True  # Generate compile_commands.json for clangd
        )
        self.extra_clang_flags: List[str] = []  # Extra flags for clangd
        self.scratch_preset_id: Optional[int] = (
            None  # Default decomp.me preset ID for scratches
        )
        self.link_order_callback: Optional[Callable[[int, List[str]], List[str]]] = (
            None  # Callback to add/remove/reorder units within a module
        )
        self.context_exclude_globs: List[
            str
        ] = []  # Globs to exclude from context files
        self.context_defines: List[
            str
        ] = []  # Macros to define at the top of context files

        # Progress output and report.json config
        self.progress = True  # Enable report.json generation and CLI progress output
        self.progress_modules: bool = True  # Include combined "modules" category
        self.progress_each_module: bool = (
            False  # Include individual modules, disable for large numbers of modules
        )
        self.progress_categories: List[ProgressCategory] = []  # Additional categories
        self.print_progress_categories: Union[bool, List[str]] = (
            True  # Print additional progress categories in the CLI progress output
        )
        self.progress_report_args: Optional[List[str]] = (
            None  # Flags to `objdiff-cli report generate`
        )

        # Progress fancy printing
        self.progress_use_fancy: bool = False
        self.progress_code_fancy_frac: int = 0
        self.progress_code_fancy_item: str = ""
        self.progress_data_fancy_frac: int = 0
        self.progress_data_fancy_item: str = ""

    def validate(self) -> None:
        required_attrs = [
            "build_dir",
            "src_dir",
            "tools_dir",
            "check_sha_path",
            "config_path",
            "ldflags",
            "linker_version",
            "libs",
            "version",
        ]
        for attr in required_attrs:
            if getattr(self, attr) is None:
                sys.exit(f"ProjectConfig.{attr} missing")

    # Creates a map of object names to Object instances
    # Options are fully resolved from the library and object
    def objects(self) -> Dict[str, Object]:
        out = {}
        for lib in self.libs or {}:
            objects: List[Object] = lib["objects"]
            for obj in objects:
                if obj.name in out:
                    sys.exit(f"Duplicate object name {obj.name}")
                out[obj.name] = obj.resolve(self, lib)
        return out

    # Gets the output path for build-related files.
    def out_path(self) -> Path:
        return self.build_dir / str(self.version)

    # Gets the path to the compilers directory.
    # Exits the program if neither `compilers_path` nor `compilers_tag` is provided.
    def compilers(self) -> Path:
        if self.compilers_path:
            return self.compilers_path
        elif self.compilers_tag:
            return self.build_dir / "compilers"
        else:
            sys.exit("ProjectConfig.compilers_tag missing")

    # Gets the wrapper to use for compiler commands, if set.
    def compiler_wrapper(self) -> Optional[Path]:
        wrapper = self.wrapper

        if self.use_wibo():
            wrapper = self.build_dir / "tools" / "wibo"
        if not is_windows() and wrapper is None:
            wrapper = Path("wine")

        return wrapper

    # Determines whether or not to use wibo as the compiler wrapper.
    def use_wibo(self) -> bool:
        return (
            self.wibo_tag is not None
            and (sys.platform == "linux" or sys.platform == "darwin")
            and platform.machine() in ("i386", "x86_64", "aarch64", "arm64")
            and self.wrapper is None
        )


def is_windows() -> bool:
    return os.name == "nt"


# On Windows, we need this to use && in commands
CHAIN = "cmd /c " if is_windows() else ""
# Native executable extension
EXE = ".exe" if is_windows() else ""


def file_is_asm(path: Path) -> bool:
    return path.suffix.lower() == ".s"


def file_is_c(path: Path) -> bool:
    return path.suffix.lower() == ".c"


def file_is_cpp(path: Path) -> bool:
    return path.suffix.lower() in (".cc", ".cp", ".cpp", ".cxx", ".pch++")


def file_is_c_cpp(path: Path) -> bool:
    return file_is_c(path) or file_is_cpp(path)


_listdir_cache = {}


def check_path_case(path: Path):
    parts = path.parts
    if path.is_absolute():
        curr = Path(parts[0])
        start = 1
    else:
        curr = Path(".")
        start = 0

    for part in parts[start:]:
        if curr in _listdir_cache:
            entries = _listdir_cache[curr]
        else:
            try:
                entries = os.listdir(curr)
            except (FileNotFoundError, PermissionError):
                sys.exit(f"Cannot access: {curr}")
            _listdir_cache[curr] = entries

        for entry in entries:
            if entry.lower() == part.lower():
                curr = curr / entry
                break
        else:
            sys.exit(f"Cannot resolve: {path}")

    if path != curr:
        print(f"⚠️  Case mismatch: expected={path} actual={curr}")


def make_flags_str(flags: Optional[List[str]]) -> str:
    if flags is None:
        return ""
    return " ".join(flags)


def get_pch_out_path(config: ProjectConfig, pch: PrecompiledHeader) -> str:
    pch_rel_path = Path(pch["output"])
    # Use absolute path as a workaround to allow this target to be matched with absolute paths in depfiles.
    #
    # Without this any object which includes the PCH would depend on the .mch filesystem entry but not the
    # corresponding Ninja task, so the MCH would not be implicitly rebuilt when the PCH is modified.
    return os.path.abspath(config.out_path() / "include" / pch_rel_path)


# Unit configuration
class BuildConfigUnit(TypedDict):
    object: Optional[str]
    name: str
    autogenerated: bool


# Module configuration
class BuildConfigModule(TypedDict):
    name: str
    module_id: int
    ldscript: str
    entry: str
    units: List[BuildConfigUnit]


# Module link configuration
class BuildConfigLink(TypedDict):
    modules: List[str]


# Build configuration generated by decomp-toolkit
class BuildConfig(BuildConfigModule):
    version: str
    modules: List[BuildConfigModule]
    links: List[BuildConfigLink]


# Load decomp-toolkit generated config.json
def load_build_config(
    config: ProjectConfig, build_config_path: Path
) -> Optional[BuildConfig]:
    if not build_config_path.is_file():
        return None

    def versiontuple(v: str) -> Tuple[int, ...]:
        return tuple(map(int, (v.split("."))))

    f = open(build_config_path, "r", encoding="utf-8")
    build_config: BuildConfig = json.load(f)
    config_version = build_config.get("version")
    if config_version is None:
        print("Invalid config.json, regenerating...")
        f.close()
        os.remove(build_config_path)
        return None

    dtk_version = str(config.dtk_tag)[1:]  # Strip v
    if versiontuple(config_version) < versiontuple(dtk_version):
        print("Outdated config.json, regenerating...")
        f.close()
        os.remove(build_config_path)
        return None

    f.close()

    # Apply link order callback
    if config.link_order_callback:
        modules: List[BuildConfigModule] = [build_config, *build_config["modules"]]
        for module in modules:
            unit_names = list(map(lambda u: u["name"], module["units"]))
            unit_names = config.link_order_callback(module["module_id"], unit_names)
            units: List[BuildConfigUnit] = []
            for unit_name in unit_names:
                units.append(
                    # Find existing unit or create a new one
                    next(
                        (u for u in module["units"] if u["name"] == unit_name),
                        {"object": None, "name": unit_name, "autogenerated": False},
                    )
                )
            module["units"] = units

    return build_config


# Generate build.ninja, objdiff.json and compile_commands.json
def generate_build(config: ProjectConfig) -> None:
    config.validate()
    objects = config.objects()
    build_config = load_build_config(config, config.out_path() / "config.json")
    generate_build_ninja(config, objects, build_config)
    generate_objdiff_config(config, objects, build_config)
    generate_compile_commands(config, objects, build_config)


# Generate build.ninja
def generate_build_ninja(
    config: ProjectConfig,
    objects: Dict[str, Object],
    build_config: Optional[BuildConfig],
) -> None:
    out = io.StringIO()
    n = ninja_syntax.Writer(out)
    n.variable("ninja_required_version", "1.3")
    n.newline()

    configure_script = Path(os.path.relpath(os.path.abspath(sys.argv[0])))
    python_lib = Path(os.path.relpath(__file__))
    python_lib_dir = python_lib.parent
    n.comment("The arguments passed to configure.py, for rerunning it.")
    n.variable("configure_args", sys.argv[1:])
    n.variable("python", f'"{sys.executable}"')
    n.newline()

    ###
    # Variables
    ###
    n.comment("Variables")
    n.variable("ldflags", make_flags_str(config.ldflags))
    if config.linker_version is None:
        sys.exit("ProjectConfig.linker_version missing")
    n.variable("mw_version", Path(config.linker_version))
    n.variable("objdiff_report_args", make_flags_str(config.progress_report_args))
    n.newline()

    ###
    # Tooling
    ###
    n.comment("Tooling")

    build_path = config.out_path()
    report_path = build_path / "report.json"
    build_tools_path = config.build_dir / "tools"
    download_tool = config.tools_dir / "download_tool.py"
    n.rule(
        name="download_tool",
        command=f"$python {download_tool} $tool $out --tag $tag",
        description="TOOL $out",
    )

    decompctx = config.tools_dir / "decompctx.py"
    n.rule(
        name="decompctx",
        command=f"$python {decompctx} $in -o $out -d $out.d $includes $excludes $defines",
        description="CTX $in",
        depfile="$out.d",
        deps="gcc",
    )

    cargo_rule_written = False

    def write_cargo_rule():
        nonlocal cargo_rule_written
        if not cargo_rule_written:
            n.pool("cargo", 1)
            n.rule(
                name="cargo",
                command="cargo build --release --manifest-path $in --bin $bin --target-dir $target",
                description="CARGO $bin",
                pool="cargo",
                depfile=Path("$target") / "release" / "$bin.d",
                deps="gcc",
            )
            cargo_rule_written = True

    if config.dtk_path is not None and config.dtk_path.is_file():
        dtk = config.dtk_path
    elif config.dtk_path is not None:
        dtk = build_tools_path / "release" / f"dtk{EXE}"
        write_cargo_rule()
        n.build(
            outputs=dtk,
            rule="cargo",
            inputs=config.dtk_path / "Cargo.toml",
            implicit=config.dtk_path / "Cargo.lock",
            variables={
                "bin": "dtk",
                "target": build_tools_path,
            },
        )
    elif config.dtk_tag:
        dtk = build_tools_path / f"dtk{EXE}"
        n.build(
            outputs=dtk,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "dtk",
                "tag": config.dtk_tag,
            },
        )
    else:
        sys.exit("ProjectConfig.dtk_tag missing")

    if config.objdiff_path is not None and config.objdiff_path.is_file():
        objdiff = config.objdiff_path
    elif config.objdiff_path is not None:
        objdiff = build_tools_path / "release" / f"objdiff-cli{EXE}"
        write_cargo_rule()
        n.build(
            outputs=objdiff,
            rule="cargo",
            inputs=config.objdiff_path / "Cargo.toml",
            implicit=config.objdiff_path / "Cargo.lock",
            variables={
                "bin": "objdiff-cli",
                "target": build_tools_path,
            },
        )
    elif config.objdiff_tag:
        objdiff = build_tools_path / f"objdiff-cli{EXE}"
        n.build(
            outputs=objdiff,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "objdiff-cli",
                "tag": config.objdiff_tag,
            },
        )
    else:
        sys.exit("ProjectConfig.objdiff_tag missing")

    if config.sjiswrap_path:
        sjiswrap = config.sjiswrap_path
    elif config.sjiswrap_tag:
        sjiswrap = build_tools_path / "sjiswrap.exe"
        n.build(
            outputs=sjiswrap,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "sjiswrap",
                "tag": config.sjiswrap_tag,
            },
        )
    else:
        sys.exit("ProjectConfig.sjiswrap_tag missing")

    wrapper = config.compiler_wrapper()
    if wrapper is not None and config.use_wibo():
        n.build(
            outputs=wrapper,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "wibo",
                "tag": config.wibo_tag,
            },
        )

    wrapper_implicit: Optional[Path] = None
    if wrapper is not None and (wrapper.exists() or config.use_wibo()):
        wrapper_implicit = wrapper

    wrapper_cmd = f"{wrapper} " if wrapper else ""

    compilers = config.compilers()
    compilers_implicit: Optional[Path] = None
    if config.compilers_path is None and config.compilers_tag is not None:
        compilers_implicit = compilers
        n.build(
            outputs=compilers,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "compilers",
                "tag": config.compilers_tag,
            },
        )

    binutils_implicit = None
    if config.binutils_path:
        binutils = config.binutils_path
    elif config.binutils_tag:
        binutils = config.build_dir / "binutils"
        binutils_implicit = binutils
        n.build(
            outputs=binutils,
            rule="download_tool",
            implicit=download_tool,
            variables={
                "tool": "binutils",
                "tag": config.binutils_tag,
            },
        )
    else:
        sys.exit("ProjectConfig.binutils_tag missing")

    n.newline()

    ###
    # Helper rule for downloading all tools
    ###
    n.comment("Download all tools")
    n.build(
        outputs="tools",
        rule="phony",
        inputs=[dtk, sjiswrap, wrapper, compilers, binutils, objdiff],
    )
    n.newline()

    ###
    # Build rules
    ###
    compiler_path = compilers / "$mw_version"

    # MWCC
    mwcc = compiler_path / "mwcceppc.exe"
    mwcc_cmd = f"{wrapper_cmd}{mwcc} $cflags -MMD -c $in -o $basedir"
    mwcc_implicit: List[Optional[Path]] = [compilers_implicit or mwcc, wrapper_implicit]

    # MWCC with UTF-8 to Shift JIS wrapper
    mwcc_sjis_cmd = f"{wrapper_cmd}{sjiswrap} {mwcc} $cflags -MMD -c $in -o $basedir"
    mwcc_sjis_implicit: List[Optional[Path]] = [*mwcc_implicit, sjiswrap]

    # MWCC for precompiled headers
    mwcc_pch_cmd = f"{wrapper_cmd}{mwcc} $cflags -MMD -c $in -o $basedir -precompile $basefilestem.mch"
    mwcc_pch_implicit: List[Optional[Path]] = [*mwcc_implicit]

    # MWCC for precompiled headers with UTF-8 to Shift JIS wrapper
    mwcc_pch_sjis_cmd = f"{wrapper_cmd}{sjiswrap} {mwcc} $cflags -MMD -c $in -o $basedir -precompile $basefilestem.mch"
    mwcc_pch_sjis_implicit: List[Optional[Path]] = [*mwcc_implicit, sjiswrap]

    # MWCC with extab post-processing
    mwcc_extab_cmd = (
        f'{CHAIN}{mwcc_cmd} && {dtk} extab clean --padding "$extab_padding" $out $out'
    )
    mwcc_extab_implicit: List[Optional[Path]] = [*mwcc_implicit, dtk]
    mwcc_sjis_extab_cmd = f'{CHAIN}{mwcc_sjis_cmd} && {dtk} extab clean --padding "$extab_padding" $out $out'
    mwcc_sjis_extab_implicit: List[Optional[Path]] = [*mwcc_sjis_implicit, dtk]

    # MWLD
    mwld = compiler_path / "mwldeppc.exe"
    mwld_cmd = f"{wrapper_cmd}{mwld} $ldflags -o $out @$out.rsp"
    mwld_implicit: List[Optional[Path]] = [compilers_implicit or mwld, wrapper_implicit]

    # GNU as
    gnu_as = binutils / f"powerpc-eabi-as{EXE}"
    gnu_as_cmd = (
        f"{CHAIN}{gnu_as} $asflags -o $out $in" + f" && {dtk} elf fixup $out $out"
    )
    gnu_as_implicit = [binutils_implicit or gnu_as, dtk]
    # As a workaround for https://github.com/encounter/dtk-template/issues/51
    # include macros.inc directly as an implicit dependency
    gnu_as_implicit.append(build_path / "include" / "macros.inc")

    if os.name != "nt":
        transform_dep = config.tools_dir / "transform_dep.py"
        mwcc_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_sjis_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_pch_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_pch_sjis_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_extab_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_sjis_extab_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_implicit.append(transform_dep)
        mwcc_sjis_implicit.append(transform_dep)
        mwcc_pch_implicit.append(transform_dep)
        mwcc_pch_sjis_implicit.append(transform_dep)
        mwcc_extab_implicit.append(transform_dep)
        mwcc_sjis_extab_implicit.append(transform_dep)

    n.comment("Link ELF file")
    n.rule(
        name="link",
        command=mwld_cmd,
        description="LINK $out",
        rspfile="$out.rsp",
        rspfile_content="$in_newline",
    )
    n.newline()

    n.comment("Generate DOL")
    n.rule(
        name="elf2dol",
        command=f"{dtk} elf2dol $in $out",
        description="DOL $out",
    )
    n.newline()

    n.comment("MWCC build")
    n.rule(
        name="mwcc",
        command=mwcc_cmd,
        description="MWCC $out",
        depfile="$basefile.d",
        deps="gcc",
    )
    n.newline()

    n.comment("MWCC build (with UTF-8 to Shift JIS wrapper)")
    n.rule(
        name="mwcc_sjis",
        command=mwcc_sjis_cmd,
        description="MWCC $out",
        depfile="$basefile.d",
        deps="gcc",
    )
    n.newline()

    n.comment("MWCC build (with extab post-processing)")
    n.rule(
        name="mwcc_extab",
        command=mwcc_extab_cmd,
        description="MWCC $out",
        depfile="$basefile.d",
        deps="gcc",
    )
    n.newline()

    n.comment("MWCC build (with UTF-8 to Shift JIS wrapper and extab post-processing)")
    n.rule(
        name="mwcc_sjis_extab",
        command=mwcc_sjis_extab_cmd,
        description="MWCC $out",
        depfile="$basefile.d",
        deps="gcc",
    )

    n.comment("Assemble asm")
    n.rule(
        name="as",
        command=gnu_as_cmd,
        description="AS $out",
        # See https://github.com/encounter/dtk-template/issues/51
        # depfile="$out.d",
        # deps="gcc",
    )
    n.newline()

    n.comment("Build precompiled header")
    n.rule(
        name="mwcc_pch",
        command=mwcc_pch_cmd,
        description="PCH $out",
        depfile="$basefile.d",
        deps="gcc",
    )
    n.newline()

    n.comment("Build precompiled header (with UTF-8 to Shift JIS wrapper)")
    n.rule(
        name="mwcc_pch_sjis",
        command=mwcc_pch_sjis_cmd,
        description="PCH $out",
        depfile="$basefile.d",
        deps="gcc",
    )
    n.newline()

    if len(config.custom_build_rules or {}) > 0:
        n.comment("Custom project build rules (pre/post-processing)")
    for rule in config.custom_build_rules or {}:
        n.rule(
            name=cast(str, rule.get("name")),
            command=cast(str, rule.get("command")),
            description=rule.get("description", None),
            depfile=rule.get("depfile", None),
            generator=rule.get("generator", False),
            pool=rule.get("pool", None),
            restat=rule.get("restat", False),
            rspfile=rule.get("rspfile", None),
            rspfile_content=rule.get("rspfile_content", None),
            deps=rule.get("deps", None),
        )
        n.newline()

    def write_custom_step(
        step: str,
        prev_step: Optional[str] = None,
        extra_inputs: Optional[List[str]] = None,
    ) -> None:
        implicit: List[Union[str, Path]] = []
        if config.custom_build_steps and step in config.custom_build_steps:
            n.comment(f"Custom build steps ({step})")
            for custom_step in config.custom_build_steps[step]:
                outputs = cast(List[Union[str, Path]], custom_step.get("outputs"))

                if isinstance(outputs, list):
                    implicit.extend(outputs)
                else:
                    implicit.append(outputs)

                n.build(
                    outputs=outputs,
                    rule=cast(str, custom_step.get("rule")),
                    inputs=custom_step.get("inputs", None),
                    implicit=custom_step.get("implicit", None),
                    order_only=custom_step.get("order_only", None),
                    variables=custom_step.get("variables", None),
                    implicit_outputs=custom_step.get("implicit_outputs", None),
                    pool=custom_step.get("pool", None),
                    dyndep=custom_step.get("dyndep", None),
                )
                n.newline()

        n.build(
            outputs=step,
            rule="phony",
            inputs=implicit,
            order_only=prev_step,
            implicit=extra_inputs,
        )

    # Add all build steps needed before we compile (e.g. processing assets)
    pch_out_names = [
        get_pch_out_path(config, pch) for pch in config.precompiled_headers or []
    ]
    write_custom_step("pre-compile", extra_inputs=pch_out_names)

    ###
    # Source files
    ###
    n.comment("Source files")

    def map_path(path: Path) -> Path:
        return path.parent / (path.name + ".MAP")

    class LinkStep:
        def __init__(self, config: BuildConfigModule) -> None:
            self.name = config["name"]
            self.module_id = config["module_id"]
            self.ldscript: Optional[Path] = Path(config["ldscript"])
            self.entry = config["entry"]
            self.inputs: List[str] = []

        def add(self, obj: Path) -> None:
            self.inputs.append(serialize_path(obj))

        def output(self) -> Path:
            if self.module_id == 0:
                return build_path / f"{self.name}.dol"
            else:
                return build_path / self.name / f"{self.name}.rel"

        def partial_output(self) -> Path:
            if self.module_id == 0:
                return build_path / f"{self.name}.elf"
            else:
                return build_path / self.name / f"{self.name}.plf"

        def write(self, n: ninja_syntax.Writer) -> None:
            n.comment(f"Link {self.name}")
            if self.module_id == 0:
                elf_path = build_path / f"{self.name}.elf"
                elf_ldflags = f"$ldflags -lcf {serialize_path(self.ldscript)}"
                if config.generate_map:
                    elf_map = map_path(elf_path)
                    elf_ldflags += f" -map {serialize_path(elf_map)}"
                else:
                    elf_map = None
                n.build(
                    outputs=elf_path,
                    rule="link",
                    inputs=self.inputs,
                    implicit=[
                        self.ldscript,
                        *mwld_implicit,
                    ],
                    implicit_outputs=elf_map,
                    variables={"ldflags": elf_ldflags},
                    order_only="post-compile",
                )
            else:
                preplf_path = build_path / self.name / f"{self.name}.preplf"
                plf_path = build_path / self.name / f"{self.name}.plf"
                preplf_ldflags = "$ldflags -sdata 0 -sdata2 0 -r"
                plf_ldflags = f"$ldflags -sdata 0 -sdata2 0 -r1 -lcf {serialize_path(self.ldscript)}"
                if self.entry:
                    plf_ldflags += f" -m {self.entry}"
                    # -strip_partial is only valid with -m
                    if config.rel_strip_partial:
                        plf_ldflags += " -strip_partial"
                if config.generate_map:
                    preplf_map = map_path(preplf_path)
                    preplf_ldflags += f" -map {serialize_path(preplf_map)}"
                    plf_map = map_path(plf_path)
                    plf_ldflags += f" -map {serialize_path(plf_map)}"
                else:
                    preplf_map = None
                    plf_map = None
                n.build(
                    outputs=preplf_path,
                    rule="link",
                    inputs=self.inputs,
                    implicit=mwld_implicit,
                    implicit_outputs=preplf_map,
                    variables={"ldflags": preplf_ldflags},
                    order_only="post-compile",
                )
                n.build(
                    outputs=plf_path,
                    rule="link",
                    inputs=self.inputs,
                    implicit=[self.ldscript, preplf_path, *mwld_implicit],
                    implicit_outputs=plf_map,
                    variables={"ldflags": plf_ldflags},
                    order_only="post-compile",
                )
            n.newline()

    link_outputs: List[Path] = []
    if build_config:
        link_steps: List[LinkStep] = []
        used_compiler_versions: Set[str] = set()
        source_inputs: List[Path] = []
        source_added: Set[Path] = set()

        if config.precompiled_headers:
            for pch in config.precompiled_headers:
                src_path = Path(pch["source"])
                pch_out_abs_path = Path(get_pch_out_path(config, pch))
                # Add appropriate language flag if it doesn't exist already
                cflags = pch["cflags"]
                if not any(flag.startswith("-lang") for flag in cflags):
                    if file_is_cpp(src_path):
                        cflags.insert(0, "-lang=c++")
                    else:
                        cflags.insert(0, "-lang=c")

                cflags_str = make_flags_str(cflags)
                shift_jis = pch.get("shift_jis", config.shift_jis)

                n.comment(f"Precompiled header {pch['output']}")
                n.build(
                    outputs=pch_out_abs_path,
                    rule="mwcc_pch_sjis" if shift_jis else "mwcc_pch",
                    inputs=src_path,
                    variables={
                        "mw_version": Path(pch["mw_version"]),
                        "cflags": cflags_str,
                        "basedir": os.path.dirname(pch_out_abs_path),
                        "basefile": pch_out_abs_path.with_suffix(""),
                        "basefilestem": pch_out_abs_path.stem,
                    },
                    implicit=mwcc_pch_sjis_implicit if shift_jis else mwcc_pch_implicit,
                )
                n.newline()

        def c_build(obj: Object, src_path: Path) -> Optional[Path]:
            # Avoid creating duplicate build rules
            if obj.src_obj_path is None or obj.src_obj_path in source_added:
                return obj.src_obj_path
            source_added.add(obj.src_obj_path)

            cflags = obj.options["cflags"]
            extra_cflags = obj.options["extra_cflags"]

            # Add appropriate language flag if it doesn't exist already
            # Added directly to the source so it flows to other generation tasks
            if not any(flag.startswith("-lang") for flag in cflags) and not any(
                flag.startswith("-lang") for flag in extra_cflags
            ):
                # Ensure extra_cflags is a unique instance,
                # and insert into there to avoid modifying shared sets of flags
                extra_cflags = obj.options["extra_cflags"] = list(extra_cflags)
                if file_is_cpp(src_path):
                    extra_cflags.insert(0, "-lang=c++")
                else:
                    extra_cflags.insert(0, "-lang=c")

            all_cflags = cflags + extra_cflags
            cflags_str = make_flags_str(all_cflags)
            used_compiler_versions.add(obj.options["mw_version"])

            # Add MWCC build rule
            lib_name = obj.options["lib"]
            build_rule = "mwcc"
            build_implcit = mwcc_implicit
            variables = {
                "mw_version": Path(obj.options["mw_version"]),
                "cflags": cflags_str,
                "basedir": os.path.dirname(obj.src_obj_path),
                "basefile": obj.src_obj_path.with_suffix(""),
            }

            if obj.options["shift_jis"] and obj.options["extab_padding"] is not None:
                build_rule = "mwcc_sjis_extab"
                build_implcit = mwcc_sjis_extab_implicit
                variables["extab_padding"] = "".join(
                    f"{i:02x}" for i in obj.options["extab_padding"]
                )
            elif obj.options["shift_jis"]:
                build_rule = "mwcc_sjis"
                build_implcit = mwcc_sjis_implicit
            elif obj.options["extab_padding"] is not None:
                build_rule = "mwcc_extab"
                build_implcit = mwcc_extab_implicit
                variables["extab_padding"] = "".join(
                    f"{i:02x}" for i in obj.options["extab_padding"]
                )
            n.comment(f"{obj.name}: {lib_name} (linked {obj.completed})")
            n.build(
                outputs=obj.src_obj_path,
                rule=build_rule,
                inputs=src_path,
                variables=variables,
                implicit=build_implcit,
                order_only="pre-compile",
            )

            # Add ctx build rule
            if obj.ctx_path is not None:
                include_dirs = []
                for flag in all_cflags:
                    if (
                        flag.startswith("-i ")
                        or flag.startswith("-I ")
                        or flag.startswith("-I+")
                    ):
                        include_dirs.append(flag[3:])
                includes = " ".join([f"-I {d}" for d in include_dirs])
                excludes = " ".join([f"-x {d}" for d in config.context_exclude_globs])
                defines = " ".join([f"-D {d}" for d in config.context_defines])

                n.build(
                    outputs=obj.ctx_path,
                    rule="decompctx",
                    inputs=src_path,
                    implicit=decompctx,
                    variables={
                        "includes": includes,
                        "excludes": excludes,
                        "defines": defines,
                    },
                )
            n.newline()

            if obj.options["add_to_all"]:
                source_inputs.append(obj.src_obj_path)

            return obj.src_obj_path

        def asm_build(
            obj: Object, src_path: Path, obj_path: Optional[Path]
        ) -> Optional[Path]:
            if obj.options["asflags"] is None:
                sys.exit("ProjectConfig.asflags missing")
            asflags_str = make_flags_str(obj.options["asflags"])
            if len(obj.options["extra_asflags"]) > 0:
                extra_asflags_str = make_flags_str(obj.options["extra_asflags"])
                asflags_str += " " + extra_asflags_str

            # Avoid creating duplicate build rules
            if obj_path is None or obj_path in source_added:
                return obj_path
            source_added.add(obj_path)

            # Add assembler build rule
            lib_name = obj.options["lib"]
            n.comment(f"{obj.name}: {lib_name} (linked {obj.completed})")
            n.build(
                outputs=obj_path,
                rule="as",
                inputs=src_path,
                variables={"asflags": asflags_str},
                implicit=gnu_as_implicit,
                order_only="pre-compile",
            )
            n.newline()

            if obj.options["add_to_all"]:
                source_inputs.append(obj_path)

            return obj_path

        def add_unit(build_obj: BuildConfigUnit, link_step: LinkStep):
            obj_path, obj_name = build_obj["object"], build_obj["name"]
            obj = objects.get(obj_name)
            if obj is None:
                if config.warn_missing_config and not build_obj["autogenerated"]:
                    print(f"Missing configuration for {obj_name}")
                if obj_path is not None:
                    link_step.add(Path(obj_path))
                return

            link_built_obj = obj.completed
            built_obj_path: Optional[Path] = None
            if obj.src_path is not None and obj.src_path.exists():
                check_path_case(obj.src_path)
                if file_is_c_cpp(obj.src_path):
                    # Add C/C++ build rule
                    built_obj_path = c_build(obj, obj.src_path)
                elif file_is_asm(obj.src_path):
                    # Add assembler build rule
                    built_obj_path = asm_build(obj, obj.src_path, obj.src_obj_path)
                else:
                    sys.exit(f"Unknown source file type {obj.src_path}")
            else:
                if config.warn_missing_source or obj.completed:
                    print(f"Missing source file {obj.src_path}")
                link_built_obj = False

            # Assembly overrides
            if (
                not link_built_obj
                and obj.asm_path is not None
                and obj.asm_path.exists()
            ):
                check_path_case(obj.asm_path)
                link_built_obj = True
                built_obj_path = asm_build(obj, obj.asm_path, obj.asm_obj_path)

            if link_built_obj and built_obj_path is not None:
                # Use the source-built object
                link_step.add(built_obj_path)
            elif obj_path is not None:
                # Use the original (extracted) object
                link_step.add(Path(obj_path))

        # Add DOL link step
        link_step = LinkStep(build_config)
        for unit in build_config["units"]:
            add_unit(unit, link_step)
        link_steps.append(link_step)

        if config.build_rels:
            # Add REL link steps
            for module in build_config["modules"]:
                module_link_step = LinkStep(module)
                for unit in module["units"]:
                    add_unit(unit, module_link_step)
                # Add empty object to empty RELs
                if len(module_link_step.inputs) == 0:
                    if config.rel_empty_file is None:
                        sys.exit("ProjectConfig.rel_empty_file missing")
                    add_unit(
                        {
                            "object": None,
                            "name": config.rel_empty_file,
                            "autogenerated": True,
                        },
                        module_link_step,
                    )
                link_steps.append(module_link_step)
        n.newline()

        # Check if all compiler versions exist
        for mw_version in used_compiler_versions:
            mw_path = compilers / mw_version / "mwcceppc.exe"
            if config.compilers_path and not os.path.exists(mw_path):
                sys.exit(f"Compiler {mw_path} does not exist")

        # Check if linker exists
        mw_path = compilers / str(config.linker_version) / "mwldeppc.exe"
        if config.compilers_path and not os.path.exists(mw_path):
            sys.exit(f"Linker {mw_path} does not exist")

        # Add all build steps needed before we link and after compiling objects
        write_custom_step("post-compile", "pre-compile")

        ###
        # Link
        ###
        for step in link_steps:
            step.write(n)
            link_outputs.append(step.output())
        n.newline()

        # Add all build steps needed after linking and before GC/Wii native format generation
        write_custom_step("post-link", "post-compile")

        ###
        # Generate DOL
        ###
        n.build(
            outputs=link_steps[0].output(),
            rule="elf2dol",
            inputs=link_steps[0].partial_output(),
            implicit=dtk,
            order_only="post-link",
        )

        ###
        # Generate RELs
        ###
        n.comment("Generate REL(s)")
        flags = "-w"
        if len(build_config["links"]) > 1:
            flags += " -q"
        n.rule(
            name="makerel",
            command=f"{dtk} rel make {flags} -c $config $names @$rspfile",
            description="REL",
            rspfile="$rspfile",
            rspfile_content="$in_newline",
        )
        generated_rels: List[str] = []
        for idx, link in enumerate(build_config["links"]):
            # Map module names to link steps
            link_steps_local = list(
                filter(
                    lambda step: step.name in link["modules"],
                    link_steps,
                )
            )
            link_steps_local.sort(key=lambda step: step.module_id)
            # RELs can be the output of multiple link steps,
            # so we need to filter out duplicates
            rels_to_generate = list(
                filter(
                    lambda step: step.module_id != 0
                    and step.name not in generated_rels,
                    link_steps_local,
                )
            )
            if len(rels_to_generate) == 0:
                continue
            generated_rels.extend(map(lambda step: step.name, rels_to_generate))
            rel_outputs = list(
                map(
                    lambda step: step.output(),
                    rels_to_generate,
                )
            )
            rel_names = list(
                map(
                    lambda step: step.name,
                    link_steps_local,
                )
            )
            rel_names_arg = " ".join(map(lambda name: f"-n {name}", rel_names))
            n.build(
                outputs=rel_outputs,
                rule="makerel",
                inputs=list(map(lambda step: step.partial_output(), link_steps_local)),
                implicit=[dtk, config.config_path],
                variables={
                    "config": config.config_path,
                    "rspfile": config.out_path() / f"rel{idx}.rsp",
                    "names": rel_names_arg,
                },
                order_only="post-link",
            )
            n.newline()

        # Add all build steps needed post-build (re-building archives and such)
        write_custom_step("post-build", "post-link")

        ###
        # Helper rule for building all source files
        ###
        n.comment("Build all source files")
        n.build(
            outputs="all_source",
            rule="phony",
            inputs=source_inputs,
        )
        n.newline()

        ###
        # Check hash
        ###
        n.comment("Check hash")
        ok_path = build_path / "ok"
        quiet = "-q " if len(link_steps) > 3 else ""
        n.rule(
            name="check",
            command=f"{dtk} shasum {quiet} -c $in -o $out",
            description="CHECK $in",
        )
        n.build(
            outputs=ok_path,
            rule="check",
            inputs=config.check_sha_path,
            implicit=[dtk, *link_outputs],
            order_only="post-build",
        )
        n.newline()

        ###
        # Calculate progress
        ###
        n.comment("Calculate progress")
        n.rule(
            name="progress",
            command=f"$python {configure_script} $configure_args progress",
            description="PROGRESS",
        )
        n.build(
            outputs="progress",
            rule="progress",
            implicit=[
                ok_path,
                configure_script,
                python_lib,
                report_path,
            ],
            order_only="post-build",
        )

        ###
        # Generate progress report
        ###
        n.comment("Generate progress report")
        n.rule(
            name="report",
            command=f"{objdiff} report generate $objdiff_report_args -o $out",
            description="REPORT",
        )
        n.build(
            outputs=report_path,
            rule="report",
            implicit=[objdiff, "objdiff.json", "all_source"],
            order_only="post-build",
        )

        n.comment("Phony edge that will always be considered dirty by ninja.")
        n.comment(
            "This can be used as an implicit to a target that should always be rerun, ignoring file modified times."
        )
        n.build(
            outputs="always",
            rule="phony",
        )
        n.newline()

        ###
        # Regression test progress reports
        ###
        report_baseline_path = build_path / "baseline.json"
        report_changes_path = build_path / "report_changes.json"
        changes_fmt = config.tools_dir / "changes_fmt.py"
        regressions_md = build_path / "regressions.md"
        n.comment(
            "Create a baseline progress report for later match regression testing"
        )
        n.build(
            outputs=report_baseline_path,
            rule="report",
            implicit=[objdiff, "all_source", "always"],
            order_only="post-build",
        )
        n.build(
            outputs="baseline",
            rule="phony",
            inputs=report_baseline_path,
        )
        n.comment("Check for any match regressions against the baseline")
        n.comment("Will fail if no baseline has been created")
        n.rule(
            name="report_changes",
            command=f"{objdiff} report changes --format json-pretty {report_baseline_path} $in -o $out",
            description="CHANGES",
        )
        n.build(
            outputs=report_changes_path,
            rule="report_changes",
            inputs=report_path,
            implicit=[objdiff, "always"],
        )
        n.rule(
            name="changes_fmt",
            command=f"$python {changes_fmt} $args $in",
            description="CHANGESFMT",
        )
        n.build(
            outputs="changes",
            rule="changes_fmt",
            inputs=report_changes_path,
            implicit=changes_fmt,
        )
        n.build(
            outputs="changes_all",
            rule="changes_fmt",
            inputs=report_changes_path,
            implicit=changes_fmt,
            variables={"args": "--all"},
        )
        n.rule(
            name="changes_md",
            command=f"$python {changes_fmt} $in -o $out",
            description="CHANGESFMT $out",
        )
        n.build(
            outputs=regressions_md,
            rule="changes_md",
            inputs=report_changes_path,
            implicit=changes_fmt,
        )
        n.newline()

        ###
        # Helper tools
        ###
        # TODO: make these rules work for RELs too
        dol_link_step = link_steps[0]
        dol_elf_path = dol_link_step.partial_output()
        n.comment("Check for mismatching symbols")
        n.rule(
            name="dol_diff",
            command=f"{dtk} -L error dol diff $in",
            description=f"DIFF {dol_elf_path}",
        )
        n.build(
            inputs=[config.config_path, dol_elf_path],
            outputs="dol_diff",
            rule="dol_diff",
        )
        n.build(
            outputs="diff",
            rule="phony",
            inputs="dol_diff",
        )
        n.newline()

        n.comment("Apply symbols from linked ELF")
        n.rule(
            name="dol_apply",
            command=f"{dtk} dol apply $in",
            description=f"APPLY {dol_elf_path}",
        )
        n.build(
            inputs=[config.config_path, dol_elf_path],
            outputs="dol_apply",
            rule="dol_apply",
            implicit=[ok_path],
        )
        n.build(
            outputs="apply",
            rule="phony",
            inputs="dol_apply",
        )
        n.newline()

    ###
    # Split DOL
    ###
    build_config_path = build_path / "config.json"
    n.comment("Split DOL into relocatable objects")
    n.rule(
        name="split",
        command=f"{dtk} dol split $in $out_dir",
        description="SPLIT $in",
        depfile="$out_dir/dep",
        deps="gcc",
    )
    n.build(
        inputs=config.config_path,
        outputs=build_config_path,
        rule="split",
        implicit=dtk,
        variables={"out_dir": build_path},
    )
    n.newline()

    ###
    # Regenerate on change
    ###
    n.comment("Reconfigure on change")
    n.rule(
        name="configure",
        command=f"$python {configure_script} $configure_args",
        generator=True,
        description=f"RUN {configure_script}",
    )
    n.build(
        outputs=["build.ninja", "objdiff.json"],
        rule="configure",
        implicit=[
            build_config_path,
            configure_script,
            python_lib,
            python_lib_dir / "ninja_syntax.py",
            *(config.reconfig_deps or []),
        ],
    )
    n.newline()

    ###
    # Default rule
    ###
    n.comment("Default rule")
    if build_config:
        if config.non_matching:
            n.default(link_outputs)
        elif config.progress:
            n.default("progress")
        else:
            n.default(ok_path)
    else:
        n.default(build_config_path)

    # Write build.ninja
    with open("build.ninja", "w", encoding="utf-8") as f:
        f.write(out.getvalue())
    out.close()


# Generate objdiff.json
def generate_objdiff_config(
    config: ProjectConfig,
    objects: Dict[str, Object],
    build_config: Optional[BuildConfig],
) -> None:
    if build_config is None:
        return

    # Load existing objdiff.json
    existing_units = {}
    if Path("objdiff.json").is_file():
        with open("objdiff.json", "r", encoding="utf-8") as r:
            existing_config = json.load(r)
            existing_units = {unit["name"]: unit for unit in existing_config["units"]}

    if config.ninja_path:
        ninja = str(config.ninja_path.absolute())
    else:
        ninja = "ninja"

    objdiff_config: Dict[str, Any] = {
        "min_version": "2.0.0-beta.5",
        "custom_make": ninja,
        "build_target": False,
        "watch_patterns": [
            "*.c",
            "*.cc",
            "*.cp",
            "*.cpp",
            "*.cxx",
            "*.c++",
            "*.h",
            "*.hh",
            "*.hp",
            "*.hpp",
            "*.hxx",
            "*.h++",
            "*.pch",
            "*.pch++",
            "*.inc",
            "*.py",
            "*.yml",
            "*.txt",
            "*.json",
        ],
        "units": [],
        "progress_categories": [],
    }

    # decomp.me compiler name mapping
    COMPILER_MAP = {
        "GC/1.0": "mwcc_233_144",
        "GC/1.1": "mwcc_233_159",
        "GC/1.1p1": "mwcc_233_159p1",
        "GC/1.2.5": "mwcc_233_163",
        "GC/1.2.5e": "mwcc_233_163e",
        "GC/1.2.5n": "mwcc_233_163n",
        "GC/1.3": "mwcc_242_53",
        "GC/1.3.2": "mwcc_242_81",
        "GC/1.3.2r": "mwcc_242_81r",
        "GC/2.0": "mwcc_247_92",
        "GC/2.0p1": "mwcc_247_92p1",
        "GC/2.5": "mwcc_247_105",
        "GC/2.6": "mwcc_247_107",
        "GC/2.7": "mwcc_247_108",
        "GC/3.0a3": "mwcc_41_51213",
        "GC/3.0a3.2": "mwcc_41_60126",
        "GC/3.0a3.3": "mwcc_41_60209",
        "GC/3.0a3.4": "mwcc_42_60308",
        "GC/3.0a5": "mwcc_42_60422",
        "GC/3.0a5.2": "mwcc_41_60831",
        "GC/3.0": "mwcc_41_60831",
        "Wii/1.0RC1": "mwcc_42_140",
        "Wii/0x4201_127": "mwcc_42_142",
        "Wii/1.0a": "mwcc_42_142",
        "Wii/1.0": "mwcc_43_145",
        "Wii/1.1": "mwcc_43_151",
        "Wii/1.3": "mwcc_43_172",
        "Wii/1.5": "mwcc_43_188",
        "Wii/1.6": "mwcc_43_202",
        "Wii/1.7": "mwcc_43_213",
    }

    def add_unit(
        build_obj: BuildConfigUnit, module_name: str, progress_categories: List[str]
    ) -> None:
        obj_path, obj_name = build_obj["object"], build_obj["name"]
        base_object = Path(obj_name).with_suffix("")
        name = str(Path(module_name) / base_object).replace(os.sep, "/")
        unit_config: Dict[str, Any] = {
            "name": name,
            "target_path": obj_path,
            "base_path": None,
            "scratch": None,
            "metadata": {
                "complete": None,
                "reverse_fn_order": None,
                "source_path": None,
                "progress_categories": progress_categories,
                "auto_generated": build_obj["autogenerated"],
            },
            "symbol_mappings": None,
        }

        # Preserve existing symbol mappings
        existing_unit = existing_units.get(name)
        if existing_unit is not None:
            unit_config["symbol_mappings"] = existing_unit.get("symbol_mappings")

        obj = objects.get(obj_name)
        if obj is None:
            objdiff_config["units"].append(unit_config)
            return

        src_exists = obj.src_path is not None and obj.src_path.exists()
        if src_exists:
            unit_config["base_path"] = obj.src_obj_path
            unit_config["metadata"]["source_path"] = obj.src_path

        # Filter out include directories
        def keep_flag(flag):
            return (
                not flag.startswith("-i ")
                and not flag.startswith("-i-")
                and not flag.startswith("-I ")
                and not flag.startswith("-I+")
                and not flag.startswith("-I-")
            )

        all_cflags = list(
            filter(keep_flag, obj.options["cflags"] + obj.options["extra_cflags"])
        )
        reverse_fn_order = False
        for flag in all_cflags:
            if not flag.startswith("-inline "):
                continue
            for value in flag.split(" ")[1].split(","):
                if value == "deferred":
                    reverse_fn_order = True
                elif value == "nodeferred":
                    reverse_fn_order = False

        compiler_version = COMPILER_MAP.get(obj.options["mw_version"])
        if compiler_version is None:
            print(f"Missing scratch compiler mapping for {obj.options['mw_version']}")
        else:
            cflags_str = make_flags_str(all_cflags)
            unit_config["scratch"] = {
                "platform": "gc_wii",
                "compiler": compiler_version,
                "c_flags": cflags_str,
                "preset_id": obj.options["scratch_preset_id"],
            }
            if src_exists:
                unit_config["scratch"].update(
                    {
                        "ctx_path": obj.ctx_path,
                        "build_ctx": True,
                    }
                )
        category_opt: Union[List[str], str] = obj.options["progress_category"]
        if isinstance(category_opt, list):
            progress_categories.extend(category_opt)
        elif category_opt is not None:
            progress_categories.append(category_opt)
        unit_config["metadata"].update(
            {
                "complete": obj.completed if src_exists else None,
                "reverse_fn_order": reverse_fn_order,
                "progress_categories": progress_categories,
            }
        )
        objdiff_config["units"].append(unit_config)

    # Add DOL units
    for unit in build_config["units"]:
        progress_categories = []
        # Only include a "dol" category if there are any modules
        # Otherwise it's redundant with the global report measures
        if len(build_config["modules"]) > 0:
            progress_categories.append("dol")
        add_unit(unit, build_config["name"], progress_categories)

    # Add REL units
    for module in build_config["modules"]:
        for unit in module["units"]:
            progress_categories = []
            if config.progress_modules:
                progress_categories.append("modules")
            if config.progress_each_module:
                progress_categories.append(module["name"])
            add_unit(unit, module["name"], progress_categories)

    # Add progress categories
    def add_category(id: str, name: str):
        objdiff_config["progress_categories"].append(
            {
                "id": id,
                "name": name,
            }
        )

    if len(build_config["modules"]) > 0:
        add_category("dol", "DOL")
        if config.progress_modules:
            add_category("modules", "Modules")
        if config.progress_each_module:
            for module in build_config["modules"]:
                add_category(module["name"], module["name"])
    for category in config.progress_categories:
        add_category(category.id, category.name)

    def cleandict(d):
        if isinstance(d, dict):
            return {k: cleandict(v) for k, v in d.items() if v is not None}
        elif isinstance(d, list):
            return [cleandict(v) for v in d]
        else:
            return d

    # Write objdiff.json
    with open("objdiff.json", "w", encoding="utf-8") as w:

        def unix_path(input: Any) -> str:
            return str(input).replace(os.sep, "/") if input else ""

        json.dump(cleandict(objdiff_config), w, indent=2, default=unix_path)


def generate_compile_commands(
    config: ProjectConfig,
    objects: Dict[str, Object],
    build_config: Optional[BuildConfig],
) -> None:
    if build_config is None or not config.generate_compile_commands:
        return

    # The following code attempts to convert mwcc flags to clang flags
    # for use with clangd.

    # Flags to ignore explicitly
    CFLAG_IGNORE: Set[str] = {
        # Search order modifier
        # Has a different meaning to Clang, and would otherwise
        # be picked up by the include passthrough prefix
        "-I-",
        "-i-",
    }
    CFLAG_IGNORE_PREFIX: Tuple[str, ...] = (
        # Recursive includes are not supported by modern compilers
        "-ir ",
    )

    # Flags to replace
    CFLAG_REPLACE: Dict[str, str] = {}
    CFLAG_REPLACE_PREFIX: Tuple[Tuple[str, str], ...] = (
        # Includes
        ("-i ", "-I"),
        ("-I ", "-I"),
        ("-I+", "-I"),
        # Defines
        ("-d ", "-D"),
        ("-D ", "-D"),
        ("-D+", "-D"),
    )

    # Flags with a finite set of options
    CFLAG_REPLACE_OPTIONS: Tuple[Tuple[str, Dict[str, Tuple[str, ...]]], ...] = (
        # Exceptions
        (
            "-Cpp_exceptions",
            {
                "off": ("-fno-cxx-exceptions",),
                "on": ("-fcxx-exceptions",),
            },
        ),
        # RTTI
        (
            "-RTTI",
            {
                "off": ("-fno-rtti",),
                "on": ("-frtti",),
            },
        ),
        # Language configuration
        (
            "-lang",
            {
                "c": ("--language=c", "--std=c99"),
                "c99": ("--language=c", "--std=c99"),
                "c++": ("--language=c++", "--std=c++98"),
                "cplus": ("--language=c++", "--std=c++98"),
            },
        ),
        # Enum size
        (
            "-enum",
            {
                "min": ("-fshort-enums",),
                "int": ("-fno-short-enums",),
            },
        ),
        # Common BSS
        (
            "-common",
            {
                "off": ("-fno-common",),
                "on": ("-fcommon",),
            },
        ),
    )

    # Flags to pass through
    CFLAG_PASSTHROUGH: Set[str] = set()
    CFLAG_PASSTHROUGH_PREFIX: Tuple[str, ...] = (
        "-I",  # includes
        "-D",  # defines
    )

    clangd_config = []

    def add_unit(build_obj: BuildConfigUnit) -> None:
        obj = objects.get(build_obj["name"])
        if obj is None:
            return

        # Skip unresolved objects
        if (
            obj.src_path is None
            or obj.src_obj_path is None
            or not file_is_c_cpp(obj.src_path)
        ):
            return

        # Gather cflags for source file
        cflags: list[str] = []

        def append_cflags(flags: Iterable[str]) -> None:
            # Match a flag against either a set of concrete flags, or a set of prefixes.
            def flag_match(
                flag: str, concrete: Set[str], prefixes: Tuple[str, ...]
            ) -> bool:
                if flag in concrete:
                    return True

                for prefix in prefixes:
                    if flag.startswith(prefix):
                        return True

                return False

            # Determine whether a flag should be ignored.
            def should_ignore(flag: str) -> bool:
                return flag_match(flag, CFLAG_IGNORE, CFLAG_IGNORE_PREFIX)

            # Determine whether a flag should be passed through.
            def should_passthrough(flag: str) -> bool:
                return flag_match(flag, CFLAG_PASSTHROUGH, CFLAG_PASSTHROUGH_PREFIX)

            # Attempts replacement for the given flag.
            def try_replace(flag: str) -> bool:
                replacement = CFLAG_REPLACE.get(flag)
                if replacement is not None:
                    cflags.append(replacement)
                    return True

                for prefix, replacement in CFLAG_REPLACE_PREFIX:
                    if flag.startswith(prefix):
                        cflags.append(flag.replace(prefix, replacement, 1))
                        return True

                for prefix, options in CFLAG_REPLACE_OPTIONS:
                    if not flag.startswith(prefix):
                        continue

                    # "-lang c99" and "-lang=c99" are both generally valid option forms
                    option = flag.removeprefix(prefix).removeprefix("=").lstrip()
                    replacements = options.get(option)
                    if replacements is not None:
                        cflags.extend(replacements)

                    return True

                return False

            for flag in flags:
                # Ignore flags first
                if should_ignore(flag):
                    continue

                # Then find replacements
                if try_replace(flag):
                    continue

                # Pass flags through last
                if should_passthrough(flag):
                    cflags.append(flag)
                    continue

        append_cflags(obj.options["cflags"])
        append_cflags(obj.options["extra_cflags"])
        cflags.extend(config.extra_clang_flags)
        cflags.extend(obj.options["extra_clang_flags"])

        unit_config = {
            "directory": Path.cwd(),
            "file": obj.src_path,
            "output": obj.src_obj_path,
            "arguments": [
                "clang",
                "-nostdinc",
                "-fno-builtin",
                "--target=powerpc-eabi",
                *cflags,
                "-c",
                obj.src_path,
                "-o",
                obj.src_obj_path,
            ],
        }
        clangd_config.append(unit_config)

    # Add DOL units
    for unit in build_config["units"]:
        add_unit(unit)

    # Add REL units
    for module in build_config["modules"]:
        for unit in module["units"]:
            add_unit(unit)

    # Write compile_commands.json
    with open("compile_commands.json", "w", encoding="utf-8") as w:

        def default_format(o):
            if isinstance(o, Path):
                return o.resolve().as_posix()
            return str(o)

        json.dump(clangd_config, w, indent=2, default=default_format)


# Print progress information from objdiff report
def calculate_progress(config: ProjectConfig) -> None:
    config.validate()
    out_path = config.out_path()
    report_path = out_path / "report.json"
    if not report_path.is_file():
        sys.exit(f"Report file {report_path} does not exist")

    report_data: Dict[str, Any] = {}
    with open(report_path, "r", encoding="utf-8") as f:
        report_data = json.load(f)

    # Convert string numbers (u64) to int
    def convert_numbers(data: Dict[str, Any]) -> None:
        for key, value in data.items():
            if isinstance(value, str) and value.isdigit():
                data[key] = int(value)

    convert_numbers(report_data["measures"])
    for category in report_data.get("categories", []):
        convert_numbers(category["measures"])

    # Output to GitHub Actions job summary, if available
    summary_path = os.getenv("GITHUB_STEP_SUMMARY")
    summary_file: Optional[IO[str]] = None
    if summary_path:
        summary_file = open(summary_path, "a", encoding="utf-8")
        summary_file.write("```\n")

    def progress_print(s: str) -> None:
        print(s)
        if summary_file:
            summary_file.write(s + "\n")

    # Print human-readable progress
    progress_print("Progress:")

    def print_category(name: str, measures: Dict[str, Any]) -> None:
        total_code = measures.get("total_code", 0)
        matched_code = measures.get("matched_code", 0)
        matched_code_percent = measures.get("matched_code_percent", 0)
        total_data = measures.get("total_data", 0)
        matched_data = measures.get("matched_data", 0)
        matched_data_percent = measures.get("matched_data_percent", 0)
        total_functions = measures.get("total_functions", 0)
        matched_functions = measures.get("matched_functions", 0)
        complete_code_percent = measures.get("complete_code_percent", 0)
        total_units = measures.get("total_units", 0)
        complete_units = measures.get("complete_units", 0)

        progress_print(
            f"  {name}: {matched_code_percent:.2f}% matched, {complete_code_percent:.2f}% linked ({complete_units} / {total_units} files)"
        )
        progress_print(
            f"    Code: {matched_code} / {total_code} bytes ({matched_functions} / {total_functions} functions)"
        )
        progress_print(
            f"    Data: {matched_data} / {total_data} bytes ({matched_data_percent:.2f}%)"
        )

    print_category("All", report_data["measures"])
    for category in report_data.get("categories", []):
        if config.print_progress_categories is True or (
            isinstance(config.print_progress_categories, list)
            and category["id"] in config.print_progress_categories
        ):
            print_category(category["name"], category["measures"])

    if config.progress_use_fancy:
        measures = report_data["measures"]
        total_code = measures.get("total_code", 0)
        total_data = measures.get("total_data", 0)
        if total_code == 0 or total_data == 0:
            return
        code_frac = measures.get("complete_code", 0) / total_code
        data_frac = measures.get("complete_data", 0) / total_data

        progress_print(
            "\nYou have {} out of {} {} and {} out of {} {}.".format(
                math.floor(code_frac * config.progress_code_fancy_frac),
                config.progress_code_fancy_frac,
                config.progress_code_fancy_item,
                math.floor(data_frac * config.progress_data_fancy_frac),
                config.progress_data_fancy_frac,
                config.progress_data_fancy_item,
            )
        )

    # Finalize GitHub Actions job summary
    if summary_file:
        summary_file.write("```\n")
        summary_file.close()
