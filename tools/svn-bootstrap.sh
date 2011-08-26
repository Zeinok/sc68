#! /bin/sh
#
# Run this script after fresh update. It should do all autotools and
# buddies required before you can configure and compile this package.
#
# $Id$

me="svn-bootstrap.sh"
ln_s="ln -sf --"
cp_r="cp -R --"
 
# Display informationnal message
msg() {
    echo "$@"
}

# Display error message
error() {
    echo "$me: $*" 1>&2
    return 1
}

# Display error message and exit
fatal() {
    error "$@"
    exit 127
}

# $1: file  $2: action
testfile() {
    local file="$1" action="$2"
    msg "Checking for file '${file}'"
    if ! test -f "${file}"; then
	${action:-error} "Missing file '${file}'"
	return 1
    fi
    return 0
}

# $1: dir  $2: action
testdir() {
    local dir="$1" action="$2"
    msg "Checking for directory '${dir}'"
    if ! (cd "${dir}"); then
	${action:-error} "Invalid directory '${dir}'"
	return 1
    fi
    return 0
}

# $1: source  $2: destination
ln_or_cp() {
    $ln_s "$1" "$2" ||
    $cp_r "$1" "$2"
}

# $1: dir  $2: --force
bootstrap_dir() {
    local dir="$1" m4file=ac_package_extra.m4 force=''

    test "$2" = '--force' && force='yes'

    testfile "${dir}/../m4/${m4file}" || return 1
    testfile "${dir}/configure.ac"    || return 1
    testfile "${dir}/Makefile.am"     || return 1
    
    if test -e "${dir}/m4"; then
	if testdir "${dir}/m4" 2>/dev/null; then
	    if test -z "$force"; then
		msg "Directory '${dir}/m4' already exists; skipping"
		return 0
	    fi
	else
	    error "'${dir}/m4' exists but is not a valid"
	    return 1
	fi
    fi
    ( cd "${dir}" && ln_or_cp ../m4 m4 )
}

######################################################################

err=0

# Test for aclocal68 directory
testdir aclocal68 fatal

# In the top source dir create a m4 directory filled with invidual
# symbolic links to aclocal68 files. All sub directories only link
# this m4 directory.
mkdir -p m4 && testdir m4 fatal
(
    cd m4 &&
    for l in ../aclocal68/*.m4; do
	ln_or_cp "$l" $(basename "$l")
    done
)

# Bootstrap all sub-directories.
dirs="as68 desa68 file68 info68 unice68 file68 sourcer68 libsc68"
dirs="$dirs sc68 cdbg68 mksc68 sc68-audacious sc68-doc"
for dir in ${dirs}; do
    bootstrap_dir $dir "$@" || err=$((err+1))
done

if test ${err} -ne 0; then
    fatal "$err error(s): Not running autoreconf"
fi

# No error runs autoreconf to create missing files.
autoreconf -vifs
