#!/bin/sh
#
# archivers/xz and devel/git must be installed
#
if [[ -z "$1" ]]; then
   echo usage: './archive.sh nnnnn using the HEAD svn revision number' && exit
fi
#
# delete any old tarballs
rm -r /tmp/codeblocks-20.03pl* 2> /dev/null
# archive tar
git archive --prefix=codeblocks-20.03pl$1/ -o /tmp/codeblocks-20.03pl$1.tar HEAD
# compress, move to distfiles
/usr/local/bin/xz /tmp/codeblocks-20.03pl$1.tar && doas mv /tmp/codeblocks-20.03pl$1.tar.xz /usr/ports/distfiles
