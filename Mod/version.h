﻿#pragma once

#define VERSION_MAJOR 1
#define VERSION_MINOR 8
#define VERSION_REVISION 1
#define VERSION_BUILD 0

#define PLUGIN_NAME "Mod"
#define PLUGIN_INTRODUCTION "Minecraft Windows Win10 Mod"
#define PLUGIN_AUTHOR "CNGEGE"

#define VS_FLAG_DEBUG 0x1L
#define VS_FLAG_RELEASE 0x0L

#define __TO_VERSION_STRING(ver) #ver
#define TO_VERSION_STRING(ver) __TO_VERSION_STRING(ver)
#define FILE_VERSION_STRING TO_VERSION_STRING(VERSION_MAJOR.VERSION_MINOR.VERSION_REVISION.VERSION_BUILD)

#ifdef _DEBUG
 #define FILE_VERSION_FLAG VS_FLAG_DEBUG
#else
 #define FILE_VERSION_FLAG VS_FLAG_RELEASE
#endif

#define VERSION_FLAG_STR_REPAIR  " - Repair"
#define VERSION_FLAG_STR_DEBUG   " - Debug"
#define VERSION_FLAG_STR_RELEASE " - Release"

#ifdef _REPAIR
 #define VERSION_FLAG_STR VERSION_FLAG_STR_REPAIR
#elif _DEBUG
 #define VERSION_FLAG_STR VERSION_FLAG_STR_DEBUG
#else
 #define VERSION_FLAG_STR VERSION_FLAG_STR_RELEASE
#endif

#define FILE_VERSION_COMPANY_NAME PLUGIN_AUTHOR
#define FILE_VERSION_LEGAL_COPYRIGHT "Copyright (C) 2022"
#define FILE_VERSION_FILE_VERSION_STRING FILE_VERSION_STRING
#define FILE_VERSION_INTERNAL_NAME PLUGIN_NAME
#define FILE_VERSION_ORIGINAL_FILENAME  PLUGIN_NAME ".dll"
#define FILE_VERSION_PRODUCT_NAME FILE_VERSION_INTERNAL_NAME
#define FILE_VERSION_PRODUCT_VERSION_STRING FILE_VERSION_STRING VERSION_FLAG_STR
#define FILE_VERSION_FILE_DESCRIPTION PLUGIN_INTRODUCTION
#define FILE_VERSION_FILE_VERSION VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define FILE_VERSION_PRODUCT_VERSION FILE_VERSION_FILE_VERSION


