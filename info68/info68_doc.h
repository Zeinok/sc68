/* This file exists for documentation purpose only. */
/**
@defgroup  info68_prg  info68 program
@ingroup   usertools68

Get and format information on sc68 files.

@section info68_author AUTHOR

  Written by Benjamin Gerard


@section info68_licence LICENCE

  Copyright (c) 1998-2014 Benjamin Gerard

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


@section info68_synopsis SYNOPSIS

  info68 [OPTION] [--] \<URI\> ([Track-List] Format)...


@section info68_description DESCRIPTION

  info68 is a program to display information on an sc68 file in
  a free format.


@section info68_options OPTIONS

 @subsection info68_generaloption General options

    @arg @b --                  break option parsing
    @arg @b -h --help           display this message and exit
    @arg @b -V --version        display version and exit
    @arg @b -o --output=\<URI\> change output to file (- is stdout)

 @subsection info68_sc68options sc68 options

    These options are parsed by the file68 library.
    @ref sc68_options More about it ...


@section info68_uri URI

    Location of the input sc68 file.

    @ref sc68uri More about it ...


@section info68_tracklist TRACK-LIST

  @subsection info68_syntax Syntax

    -DIGIT[-DIGIT][,DIGIT[-DIGIT]...]

    track-list executes the next format string for all listed tracks
    one after another  in the given order.  First track is number 1.
    0 is replaced by the last track number (number of track).

    @e e.g: "-1,5,4-6" works with tracks 1,5,4,5 and 6 in this order.


@section info68_format FORMAT

    A string with special @ref commands commands sequence. Each of
    these commands start with a percent '\%' char followed by a command
    identifer.


@subsection info68_diskcommands DISK-COMMANDS

  Commands to display disk information.

  @arg @b \%#     Number of tracks
  @arg @b \%?     Default track
  @arg @b \%N     Disk name
  @arg @b \%A     Default track author name
  @arg @b \%C     Default track composer name
  @arg @b \%P     Default track ripper name
  @arg @b \%V     Default track converter name
  @arg @b \%T     Disk time in sec
  @arg @b \%Y     Formated disk time. Format "TT MM:SS"
  @arg @b \%H     All tracks ORed hardware flags (see -h)


@subsection info68_trackcommands TRACK-COMMANDS

  Commands applied to current track.

  @arg @b \%%     Track number
  @arg @b \%n     Track name
  @arg @b \%a     Author name
  @arg @b \%c     Composer name
  @arg @b \%r     Replay name
  @arg @b \%t     Time in sec
  @arg @b \%y     Formated time. Format "TT MM:SS"
  @arg @b \%f     Replay frequency
  @arg @b \%@     Load address
  @arg @b \%h     Hardware flags [YSA] uppercase means activated.
                  Y:YM-2149, S:STE, A:Amiga


@subsection info68_misccommand MISC-COMMANDS

  @arg @b \%L        Display a newline character
  @arg @b \%-STRING  Display "-STRING"
  @arg @b STRING     Display "STRING"


@section bugs BUGS

  Report to <benjihan -4t- users.sourceforge -d0t- net>.
  

@section seealso SEE ALSO

  @see unice68_prg, sc68_prg

*/
