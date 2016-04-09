include(../common.pri)

SOURCES += \
$$PWD/filesystem.cpp \
$$PWD/timing.cpp \
$$PWD/tinythread.cpp\
$$PWD/platform.cpp\
$$PWD/utf8.cpp\
$$PWD/version.cpp\
$$PWD/re2/bitstate.cc\
$$PWD/re2/compile.cc\
$$PWD/re2/dfa.cc\
$$PWD/re2/filtered_re2.cc\
$$PWD/re2/mimics_pcre.cc\
$$PWD/re2/nfa.cc\
$$PWD/re2/onepass.cc\
$$PWD/re2/parse.cc\
$$PWD/re2/perl_groups.cc\
$$PWD/re2/prefilter.cc\
$$PWD/re2/prefilter_tree.cc\
$$PWD/re2/prog.cc\
$$PWD/re2/re2.cc\
$$PWD/re2/regexp.cc\
$$PWD/re2/set.cc\
$$PWD/re2/simplify.cc\
$$PWD/re2/stringpiece.cc\
$$PWD/re2/tostring.cc\
$$PWD/re2/unicode_casefold.cc\
$$PWD/re2/unicode_groups.cc\
$$PWD/re2/util/hash.cc\
$$PWD/re2/util/stringprintf.cc\
$$PWD/re2/util/rune.cc\
$$PWD/re2/util/strutil.cc\
$$PWD/re2/util/valgrind.cc


HEADERS += \
    $$PWD/filesystem.h \
    $$PWD/timing.h \
    $$PWD/tinythread.h \
    $$PWD/platform.h \
    $$PWD/utf8.h \
    $$PWD/version.h \
    $$PWD/re2/filtered_re2.h \
    $$PWD/re2/prefilter.h \
    $$PWD/re2/prefilter_tree.h \
    $$PWD/re2/prog.h \
    $$PWD/re2/regexp.h \
    $$PWD/re2/set.h \
    $$PWD/re2/stringpiece.h \
    $$PWD/re2/unicode_casefold.h \
    $$PWD/re2/unicode_groups.h \
    $$PWD/re2/util/valgrind.h

