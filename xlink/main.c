/*  Copyright 2008-2017 Carsten Elton Sorensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ctype.h>
#include <stdarg.h>

#include "asmotor.h"
#include "file.h"
#include "str.h"

#include "amiga.h"
#include "assign.h"
#include "commodore.h"
#include "gameboy.h"
#include "group.h"
#include "image.h"
#include "mapfile.h"
#include "object.h"
#include "patch.h"
#include "section.h"
#include "sega.h"
#include "smart.h"
#include "xlink.h"

typedef enum {
    TARGET_BINARY,
    TARGET_GAMEBOY,
    TARGET_AMIGA_EXECUTABLE,
    TARGET_AMIGA_LINK_OBJECT,
    TARGET_COMMODORE64_PRG,
    TARGET_COMMODORE128_PRG,
    TARGET_COMMODORE264_PRG,
    TARGET_MEGA_DRIVE,
    TARGET_MASTER_SYSTEM,
} TargetType;

bool
target_SupportsReloc(TargetType type) {
    return type == TARGET_AMIGA_EXECUTABLE || type == TARGET_AMIGA_LINK_OBJECT;
}

bool
target_SupportsOnlySectionRelativeReloc(TargetType type) {
    return type == TARGET_AMIGA_EXECUTABLE;
}

bool
target_SupportsImports(TargetType type) {
    return type == TARGET_AMIGA_LINK_OBJECT;
}

NORETURN (void error(const char* fmt, ...));

void
error(const char* fmt, ...) {
    va_list list;

    va_start(list, fmt);

    printf("ERROR: ");
    vprintf(fmt, list);
    printf("\n");

    va_end(list);

    exit(EXIT_FAILURE);
}

static void
printUsage(void) {
    printf("xlink (ASMotor v" ASMOTOR_VERSION ")\n"
           "\n"
           "Usage: xlink [options] file1 [file2 [... filen]]\n"
           "Options: (a forward slash (/) can be used instead of the dash (-))\n"
           "\t-h\t\tThis text\n"
           "\t-m<mapfile>\tWrite a mapfile\n"
           "\t-o<output>\tWrite output to file <output>\n"
           "\t-s<symbol>\tPerform smart linking starting with <symbol>\n"
           "\t-t\t\tOutput target\n"
           "\t    -ta\t\tAmiga executable\n"
           "\t    -tb\t\tAmiga link object\n"
           "\t    -tc64\tCommodore 64 .prg\n"
           "\t    -tc128\tCommodore 128 unbanked .prg\n"
           "\t    -tc128f\tCommodore 128 Function ROM (32 KiB)\n"
           "\t    -tc128fl\tCommodore 128 Function ROM Low (16 KiB)\n"
           "\t    -tc128fh\tCommodore 128 Function ROM High (16 KiB)\n"
           "\t    -tc264\tCommodore 264 series .prg\n"
           "\t    -tg\t\tGame Boy ROM image\n"
           "\t    -ts\t\tGame Boy small mode (32 KiB)\n"
           "\t    -tmd\tSega Mega Drive\n"
           "\t    -tms8\tSega Master System (8 KiB)\n"
           "\t    -tms16\tSega Master System (16 KiB)\n"
           "\t    -tms32\tSega Master System (32 KiB)\n"
           "\t    -tms48\tSega Master System (48 KiB)\n"
           "\t    -tmsb\tSega Master System banked (64+ KiB)\n"
//			"\t    -tm<mach>\tUse file <mach>\n"
    );
    exit(EXIT_SUCCESS);
}

int
main(int argc, char* argv[]) {
    int argn = 1;
    bool targetDefined = false;
    TargetType targetType = TARGET_BINARY;
    int binaryPad = -1;

    const char* outputFilename = NULL;
    const char* smartlink = NULL;
    const char* mapFilename = NULL;

    if (argc == 1)
        printUsage();

    while (argn < argc && (argv[argn][0] == '-' || argv[argn][0] == '/')) {
        switch (tolower((unsigned char) argv[argn][1])) {
            case '?':
            case 'h':
                ++argn;
                printUsage();
                break;
            case 'm':
                /* MapFile */
                if (argv[argn][2] != 0) {
                    mapFilename = &argv[argn++][2];
                } else {
                    error("option \"m\" needs an argument");
                }
                break;
            case 'o':
                /* Output filename */
                if (argv[argn][2] != 0) {
                    outputFilename = &argv[argn][2];
                    ++argn;
                } else {
                    error("option \"o\" needs an argument");
                }
                break;
            case 's':
                /* Smart linking */
                if (argv[argn][2] != 0) {
                    smartlink = &argv[argn][2];
                    ++argn;
                } else {
                    error("option \"s\" needs an argument");
                }
                break;
            case 't':
                /* Target */
                if (argv[argn][2] != 0 && !targetDefined) {
                    string* target = str_ToLower(str_Create(&argv[argn][2]));
                    if (str_EqualConst(target, "a")) {
                        /* Amiga executable */
                        group_SetupAmiga();
                        targetDefined = true;
                        targetType = TARGET_AMIGA_EXECUTABLE;
                        ++argn;
                    } else if (str_EqualConst(target, "b")) {
                        /* Amiga link object */
                        group_SetupAmiga();
                        targetDefined = true;
                        targetType = TARGET_AMIGA_LINK_OBJECT;
                        ++argn;
                    } else if (str_EqualConst(target, "g")) {
                        /* Gameboy ROM image */
                        group_SetupGameboy();
                        targetDefined = true;
                        targetType = TARGET_GAMEBOY;
                        ++argn;
                    } else if (str_EqualConst(target, "s")) {
                        /* Gameboy small mode ROM image */
                        group_SetupSmallGameboy();
                        targetDefined = true;
                        targetType = TARGET_GAMEBOY;
                        ++argn;
                    } else if (str_EqualConst(target, "c64")) {
                        /* Commodore 64 .prg */
                        group_SetupCommodore64();
                        targetDefined = true;
                        targetType = TARGET_COMMODORE64_PRG;
                        ++argn;
                    } else if (str_EqualConst(target, "c128")) {
                        /* Commodore 128 .prg */
                        group_SetupUnbankedCommodore128();
                        targetDefined = true;
                        targetType = TARGET_COMMODORE128_PRG;
                        ++argn;
                    } else if (str_EqualConst(target, "c128f")) {
                        /* Commodore 128 Function ROM */
                        group_SetupCommodore128FunctionROM();
                        targetDefined = true;
                        targetType = TARGET_BINARY;
                        binaryPad = 0x8000;
                        ++argn;
                    } else if (str_EqualConst(target, "c128fl")) {
                        /* Commodore 128 Function ROM Low */
                        group_SetupCommodore128FunctionROMLow();
                        targetDefined = true;
                        targetType = TARGET_BINARY;
                        binaryPad = 0x4000;
                        ++argn;
                    } else if (str_EqualConst(target, "c128fh")) {
                        /* Commodore 128 Function ROM High */
                        group_SetupCommodore128FunctionROMHigh();
                        targetDefined = true;
                        targetType = TARGET_BINARY;
                        binaryPad = 0x4000;
                        ++argn;
                    } else if (str_EqualConst(target, "c264")) {
                        /* Commodore 264 series .prg */
                        group_SetupCommodore264();
                        targetDefined = true;
                        targetType = TARGET_COMMODORE264_PRG;
                        ++argn;
                    } else if (str_EqualConst(target, "md")) {
                        /* Sega Mega Drive/Genesis */
                        group_SetupSegaMegaDrive();
                        targetDefined = true;
                        targetType = TARGET_MEGA_DRIVE;
                        ++argn;
                    } else if (str_EqualConst(target, "ms8")) {
                        /* Sega Master System 8 KiB */
                        group_SetupSegaMasterSystem(0x2000);
                        targetDefined = true;
                        targetType = TARGET_MASTER_SYSTEM;
                        binaryPad = 0x2000;
                        ++argn;
                    } else if (str_EqualConst(target, "ms16")) {
                        /* Sega Master System 16 KiB */
                        group_SetupSegaMasterSystem(0x4000);
                        targetDefined = true;
                        targetType = TARGET_MASTER_SYSTEM;
                        binaryPad = 0x4000;
                        ++argn;
                    } else if (str_EqualConst(target, "ms32")) {
                        /* Sega Master System 32 KiB */
                        group_SetupSegaMasterSystem(0x8000);
                        targetDefined = true;
                        targetType = TARGET_MASTER_SYSTEM;
                        binaryPad = 0x8000;
                        ++argn;
                    } else if (str_EqualConst(target, "ms48")) {
                        /* Sega Master System 48 KiB */
                        group_SetupSegaMasterSystem(0xC000);
                        targetDefined = true;
                        targetType = TARGET_MASTER_SYSTEM;
                        binaryPad = 0xC000;
                        ++argn;
                    } else if (str_EqualConst(target, "msb")) {
                        /* Sega Master System 64+ KiB */
                        group_SetupSegaMasterSystemBanked();
                        targetDefined = true;
                        targetType = TARGET_MASTER_SYSTEM;
                        binaryPad = 0;
                        ++argn;
                    } else {
                        error("Unknown target \"%s\"", str_String(target));
                    }
#if 0
                    case 'm':
                    {
                        /* Use machine def file */
                        targetDefined = 1;
                        Error("option \"tm\" not implemented yet");
                        ++argn;
                        break;
                    }
#endif
                    str_Free(target);
                } else {
                    error("more than one target (option \"t\") defined");
                }

                break;
            default:
                printf("Unknown option \"%s\", ignored\n", argv[argn]);
                ++argn;
                break;
        }
    }

    if (!targetDefined) {
        error("No target format defined");
    }

    while (argn < argc && argv[argn]) {
        obj_Read(argv[argn++]);
    }

    smart_Process(smartlink);

    if (!target_SupportsReloc(targetType))
        assign_Process();

    patch_Process(target_SupportsReloc(targetType), target_SupportsOnlySectionRelativeReloc(targetType),
                  target_SupportsImports(targetType));

    if (outputFilename != NULL) {
        switch (targetType) {
            case TARGET_MEGA_DRIVE:
                sega_WriteMegaDriveImage(outputFilename);
                break;
            case TARGET_MASTER_SYSTEM:
                sega_WriteMasterSystemImage(outputFilename, binaryPad);
                break;
            case TARGET_GAMEBOY:
                gameboy_WriteImage(outputFilename);
                break;
            case TARGET_BINARY:
                image_WriteBinary(outputFilename, binaryPad);
                break;
            case TARGET_COMMODORE64_PRG:
                commodore_WritePrg(outputFilename, 0x0801);
                break;
            case TARGET_COMMODORE128_PRG:
                commodore_WritePrg(outputFilename, 0x1C01);
                break;
            case TARGET_COMMODORE264_PRG:
                commodore_WritePrg(outputFilename, 0x1001);
                break;
            case TARGET_AMIGA_EXECUTABLE:
                amiga_WriteExecutable(outputFilename, false);
                break;
            case TARGET_AMIGA_LINK_OBJECT:
                amiga_WriteLinkObject(outputFilename, false);
                break;
        }
    }

    if (mapFilename != NULL) {
        if (!target_SupportsReloc(targetType)) {
            sect_ResolveUnresolved();
            sect_SortSections();
            map_Write(mapFilename);
        } else {
            error("Output format does not support producing a mapfile");
        }
    }
        

    return EXIT_SUCCESS;
}
