/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LIB4_H
#define LIB4_H
#include <stdio.h>

#ifdef TEST_LIB
#   if defined(_WIN32) || defined(WIN32)
#       define LIB_EXPORT __declspec(dllexport)
#       define LIB_NO_EXPORT
#   else
#       define LIB_EXPORT __attribute__((visibility("default")))
#   endif
#else
#   define LIB_EXPORT
#endif

class LIB_EXPORT TestMe
{
public:
    TestMe();
    void hello1() const;
    inline void hello2() const { hello2Impl(); }
private:
    void hello2Impl() const;
};

#endif // LIB4_H
