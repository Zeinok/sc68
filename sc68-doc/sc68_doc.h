#error "FOR DOCUMENTATION PURPOSE ONLY"

/*

GROUPS HIERARCHY

sc68_doc
  sc68_usr
    sc68_cfg
    sc68_prg
      sc68_prg_dev
      sc68_prg_usr
  sc68_dev
    sc68_dev_lib
*/

/**
 @mainpage sc68 documentation

 @section Copyleft

   Copyright (C) 1993-2013 Benjamin Gerard

   Permission is granted to copy, distribute and/or modify this
   document under the terms of the GNU Free Documentation License,
   Version 1.3 or any later version published by the Free Software
   Foundation; with no Invariant Sections, no Front-Cover Texts, and
   no Back-Cover Texts.  A copy of the license is included in the
   section entitled "GNU Free Documentation License".

   The full license is at http://www.gnu.org/licenses/fdl-1.3.en.html

 @section Content

   The @ref sc68_usr "sc68 user documentation" contains documentation for
   - @ref sc68_cli "Using programs and utilities"
   - @ref sc68_cfg "Configuration"

   The sc68 developer documentation contains documentation for
   - Building sc68
   - Using sc68 library

*/

/**
 @defgroup  sc68_doc  All sc68 documentations.
*/

/**
 @defgroup  sc68_usr  End user's documentation
 @ingroup   sc68_doc
 @brief     Configure and use sc68 programs.
*/

/**
 @defgroup  sc68_dev  Developer's documentation
 @ingroup   sc68_doc
 @brief     How to use sc68 libraries in your programs.
*/


/**
 @defgroup  sc68_prg  sc68 programs
 @ingroup   sc68_usr
*/

/**
 @defgroup  sc68_prg_dev  sc68 programs for developers
 @ingroup   sc68_prg
*/

/**
 @defgroup  sc68_prg_usr  sc68 programs for end users
 @ingroup   sc68_prg
*/

/**
 @defgroup  sc68_dev_lib sc68  libraries
 @ingroup   sc68_dev

   Documents sc68 and buddies libraries.

   This documention is for developers that want to use sc68 or its
   component in their own programs.

   The @ref sc68_lib is the higher level component. It depends on @ref
   file68_lib which main purpose is to handle sc68 resources (player,
   files, config ...). @ref unice68_lib is optionnaly used for
   depacking ICE! file a common compressor format in the Atari ST
   world.  Finally @ref desa68_lib is used by extra programs such as
   @ref sourcer68_prg or @ref debug68_prg for disassemble M68000
   machine code.

*/

