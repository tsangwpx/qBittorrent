/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2023  Mike Tzou (Chocobo1)
 * Copyright (C) 2014  sledgehammer999 <hammered999@gmail.com>
 * Copyright (C) 2006  Christophe Dumez <chris@qbittorrent.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 */

#include "os.h"

#ifdef Q_OS_MACOS
#include <CoreServices/CoreServices.h>
#endif // Q_OS_MACOS

#include <QString>

#include "base/global.h"
#include "base/path.h"

#ifdef Q_OS_MACOS
namespace
{
    const CFStringRef torrentExtension = CFSTR("torrent");
    const CFStringRef magnetUrlScheme = CFSTR("magnet");
}

bool Utils::OS::isTorrentFileAssocSet()
{
    bool isSet = false;
    const CFStringRef torrentId = ::UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, torrentExtension, NULL);
    if (torrentId != NULL)
    {
        const CFStringRef defaultHandlerId = ::LSCopyDefaultRoleHandlerForContentType(torrentId, kLSRolesViewer);
        if (defaultHandlerId != NULL)
        {
            const CFStringRef myBundleId = ::CFBundleGetIdentifier(::CFBundleGetMainBundle());
            if (myBundleId != NULL)
                isSet = ::CFStringCompare(myBundleId, defaultHandlerId, 0) == kCFCompareEqualTo;
            ::CFRelease(defaultHandlerId);
        }
        ::CFRelease(torrentId);
    }
    return isSet;
}

void Utils::OS::setTorrentFileAssoc()
{
    if (isTorrentFileAssocSet())
        return;

    const CFStringRef torrentId = ::UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, torrentExtension, NULL);
    if (torrentId != NULL)
    {
        const CFStringRef myBundleId = ::CFBundleGetIdentifier(::CFBundleGetMainBundle());
        if (myBundleId != NULL)
            ::LSSetDefaultRoleHandlerForContentType(torrentId, kLSRolesViewer, myBundleId);
        ::CFRelease(torrentId);
    }
}

bool Utils::OS::isMagnetLinkAssocSet()
{
    bool isSet = false;
    const CFStringRef defaultHandlerId = ::LSCopyDefaultHandlerForURLScheme(magnetUrlScheme);
    if (defaultHandlerId != NULL)
    {
        const CFStringRef myBundleId = ::CFBundleGetIdentifier(::CFBundleGetMainBundle());
        if (myBundleId != NULL)
            isSet = ::CFStringCompare(myBundleId, defaultHandlerId, 0) == kCFCompareEqualTo;
        ::CFRelease(defaultHandlerId);
    }
    return isSet;
}

void Utils::OS::setMagnetLinkAssoc()
{
    if (isMagnetLinkAssocSet())
        return;

    const CFStringRef myBundleId = ::CFBundleGetIdentifier(::CFBundleGetMainBundle());
    if (myBundleId != NULL)
        ::LSSetDefaultHandlerForURLScheme(magnetUrlScheme, myBundleId);
}
#endif // Q_OS_MACOS
