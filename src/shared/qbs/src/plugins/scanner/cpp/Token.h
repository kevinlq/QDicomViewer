/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef CPLUSPLUS_TOKEN_H
#define CPLUSPLUS_TOKEN_H

#include "CPlusPlusForwardDeclarations.h"

namespace CPlusPlus {

enum Kind {
    T_EOF_SYMBOL = 0,
    T_ERROR,

    T_CPP_COMMENT,
    T_CPP_DOXY_COMMENT,
    T_COMMENT,
    T_DOXY_COMMENT,
    T_IDENTIFIER,

    T_FIRST_LITERAL,
    T_NUMERIC_LITERAL = T_FIRST_LITERAL,
    T_CHAR_LITERAL,
    T_WIDE_CHAR_LITERAL,
    T_STRING_LITERAL,
    T_WIDE_STRING_LITERAL,
    T_AT_STRING_LITERAL,
    T_ANGLE_STRING_LITERAL,
    T_LAST_LITERAL = T_ANGLE_STRING_LITERAL,

    T_FIRST_OPERATOR,
    T_AMPER = T_FIRST_OPERATOR,
    T_AMPER_AMPER,
    T_AMPER_EQUAL,
    T_ARROW,
    T_ARROW_STAR,
    T_CARET,
    T_CARET_EQUAL,
    T_COLON,
    T_COLON_COLON,
    T_COMMA,
    T_SLASH,
    T_SLASH_EQUAL,
    T_DOT,
    T_DOT_DOT_DOT,
    T_DOT_STAR,
    T_EQUAL,
    T_EQUAL_EQUAL,
    T_EXCLAIM,
    T_EXCLAIM_EQUAL,
    T_GREATER,
    T_GREATER_EQUAL,
    T_GREATER_GREATER,
    T_GREATER_GREATER_EQUAL,
    T_LBRACE,
    T_LBRACKET,
    T_LESS,
    T_LESS_EQUAL,
    T_LESS_LESS,
    T_LESS_LESS_EQUAL,
    T_LPAREN,
    T_MINUS,
    T_MINUS_EQUAL,
    T_MINUS_MINUS,
    T_PERCENT,
    T_PERCENT_EQUAL,
    T_PIPE,
    T_PIPE_EQUAL,
    T_PIPE_PIPE,
    T_PLUS,
    T_PLUS_EQUAL,
    T_PLUS_PLUS,
    T_POUND,
    T_POUND_POUND,
    T_QUESTION,
    T_RBRACE,
    T_RBRACKET,
    T_RPAREN,
    T_SEMICOLON,
    T_STAR,
    T_STAR_EQUAL,
    T_TILDE,
    T_TILDE_EQUAL,
    T_LAST_OPERATOR = T_TILDE_EQUAL,

    T_FIRST_KEYWORD,
    T_ASM = T_FIRST_KEYWORD,
    T_AUTO,
    T_BOOL,
    T_BREAK,
    T_CASE,
    T_CATCH,
    T_CHAR,
    T_CLASS,
    T_CONST,
    T_CONST_CAST,
    T_CONTINUE,
    T_DEFAULT,
    T_DELETE,
    T_DO,
    T_DOUBLE,
    T_DYNAMIC_CAST,
    T_ELSE,
    T_ENUM,
    T_EXPLICIT,
    T_EXPORT,
    T_EXTERN,
    T_FALSE,
    T_FLOAT,
    T_FOR,
    T_FRIEND,
    T_GOTO,
    T_IF,
    T_INLINE,
    T_INT,
    T_LONG,
    T_MUTABLE,
    T_NAMESPACE,
    T_NEW,
    T_OPERATOR,
    T_PRIVATE,
    T_PROTECTED,
    T_PUBLIC,
    T_REGISTER,
    T_REINTERPRET_CAST,
    T_RETURN,
    T_SHORT,
    T_SIGNED,
    T_SIZEOF,
    T_STATIC,
    T_STATIC_CAST,
    T_STRUCT,
    T_SWITCH,
    T_TEMPLATE,
    T_THIS,
    T_THROW,
    T_TRUE,
    T_TRY,
    T_TYPEDEF,
    T_TYPEID,
    T_TYPENAME,
    T_UNION,
    T_UNSIGNED,
    T_USING,
    T_VIRTUAL,
    T_VOID,
    T_VOLATILE,
    T_WCHAR_T,
    T_WHILE,

    T___ATTRIBUTE__,
    T___TYPEOF__,

    // obj c++ @ keywords
    T_FIRST_OBJC_AT_KEYWORD,

    T_AT_CATCH = T_FIRST_OBJC_AT_KEYWORD,
    T_AT_CLASS,
    T_AT_COMPATIBILITY_ALIAS,
    T_AT_DEFS,
    T_AT_DYNAMIC,
    T_AT_ENCODE,
    T_AT_END,
    T_AT_FINALLY,
    T_AT_IMPLEMENTATION,
    T_AT_INTERFACE,
    T_AT_NOT_KEYWORD,
    T_AT_OPTIONAL,
    T_AT_PACKAGE,
    T_AT_PRIVATE,
    T_AT_PROPERTY,
    T_AT_PROTECTED,
    T_AT_PROTOCOL,
    T_AT_PUBLIC,
    T_AT_REQUIRED,
    T_AT_SELECTOR,
    T_AT_SYNCHRONIZED,
    T_AT_SYNTHESIZE,
    T_AT_THROW,
    T_AT_TRY,

    T_LAST_OBJC_AT_KEYWORD = T_AT_TRY,

    T_FIRST_QT_KEYWORD,

    // Qt keywords
    T_SIGNAL = T_FIRST_QT_KEYWORD,
    T_SLOT,
    T_Q_SIGNAL,
    T_Q_SLOT,
    T_Q_SIGNALS,
    T_Q_SLOTS,
    T_Q_FOREACH,
    T_Q_D,
    T_Q_Q,
    T_Q_INVOKABLE,
    T_Q_PROPERTY,
    T_Q_INTERFACES,
    T_Q_ENUMS,
    T_Q_FLAGS,
    T_Q_PRIVATE_SLOT,
    T_Q_DECLARE_INTERFACE,
    T_Q_OBJECT,
    T_Q_GADGET,
    T_Q_NAMESPACE,
    T_LAST_KEYWORD = T_Q_NAMESPACE,

    // aliases
    T_OR = T_PIPE_PIPE,
    T_AND = T_AMPER_AMPER,
    T_NOT = T_EXCLAIM,
    T_XOR = T_CARET,
    T_BITOR = T_PIPE,
    T_COMPL = T_TILDE,
    T_OR_EQ = T_PIPE_EQUAL,
    T_AND_EQ = T_AMPER_EQUAL,
    T_BITAND = T_AMPER,
    T_NOT_EQ = T_EXCLAIM_EQUAL,
    T_XOR_EQ = T_CARET_EQUAL,

    T___ASM = T_ASM,
    T___ASM__ = T_ASM,

    T_TYPEOF = T___TYPEOF__,
    T___TYPEOF = T___TYPEOF__,

    T___INLINE = T_INLINE,
    T___INLINE__ = T_INLINE,

    T___CONST = T_CONST,
    T___CONST__ = T_CONST,

    T___VOLATILE = T_VOLATILE,
    T___VOLATILE__ = T_VOLATILE,

    T___ATTRIBUTE = T___ATTRIBUTE__
};

class CPLUSPLUS_EXPORT Token
{
public:
    Token();
    ~Token();

    inline bool is(unsigned k) const    { return f.kind == k; }
    inline bool isNot(unsigned k) const { return f.kind != k; }
#ifndef CPLUSPLUS_NO_PARSER
    const char *spell() const;
#endif
    void reset();

    inline unsigned kind() const { return f.kind; }
    inline bool newline() const { return f.newline; }
    inline bool whitespace() const { return f.whitespace; }
    inline bool joined() const { return f.joined; }
    inline bool expanded() const { return f.expanded; }
    inline bool generated() const { return f.generated; }
    inline unsigned length() const { return f.length; }

    inline unsigned begin() const
    { return offset; }

    inline unsigned end() const
    { return offset + f.length; }

    inline bool isLiteral() const
    { return f.kind >= T_FIRST_LITERAL && f.kind <= T_LAST_LITERAL; }

    inline bool isOperator() const
    { return f.kind >= T_FIRST_OPERATOR && f.kind <= T_LAST_OPERATOR; }

    inline bool isKeyword() const
    { return f.kind >= T_FIRST_KEYWORD && f.kind < T_FIRST_QT_KEYWORD; }

    inline bool isComment() const
    { return f.kind == T_COMMENT || f.kind == T_DOXY_COMMENT ||
      f.kind == T_CPP_COMMENT || f.kind == T_CPP_DOXY_COMMENT; }

    inline bool isObjCAtKeyword() const
    { return f.kind >= T_FIRST_OBJC_AT_KEYWORD && f.kind <= T_LAST_OBJC_AT_KEYWORD; }

    static const char *name(int kind);

public:
    struct Flags {
        unsigned kind       : 8;
        unsigned newline    : 1;
        unsigned whitespace : 1;
        unsigned joined     : 1;
        unsigned expanded   : 1;
        unsigned generated  : 1;
        unsigned pad        : 3;
        unsigned length     : 16;
    };
    union {
        unsigned flags;
        Flags f;
    };

    unsigned offset;

    union {
        void *ptr;
#ifndef CPLUSPLUS_NO_PARSER
        const Literal *literal;
        const NumericLiteral *number;
        const StringLiteral *string;
        const Identifier *identifier;
#endif
        unsigned close_brace;
        unsigned lineno;
    };
};

} // end of namespace CPlusPlus


#endif // CPLUSPLUS_TOKEN_H
