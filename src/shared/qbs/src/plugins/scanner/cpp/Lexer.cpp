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

#include "Lexer.h"
#include <cctype>

namespace CPlusPlus {

Lexer::Lexer(const char *firstChar, const char *lastChar)
    : _state(State_Default),
      _flags(0),
      _currentLine(1)
{
    setSource(firstChar, lastChar);
}

Lexer::~Lexer()
{ }

void Lexer::setSource(const char *firstChar, const char *lastChar)
{
    _firstChar = firstChar;
    _lastChar = lastChar;
    _currentChar = _firstChar - 1;
    _tokenStart = _currentChar;
    _yychar = '\n';
}

void Lexer::setStartWithNewline(bool enabled)
{
    if (enabled)
        _yychar = '\n';
    else
        _yychar = ' ';
}

int Lexer::state() const
{ return _state; }

void Lexer::setState(int state)
{ _state = state; }

bool Lexer::qtMocRunEnabled() const
{ return f._qtMocRunEnabled; }

void Lexer::setQtMocRunEnabled(bool onoff)
{ f._qtMocRunEnabled = onoff; }

bool Lexer::cxx0xEnabled() const
{ return f._cxx0xEnabled; }

void Lexer::setCxxOxEnabled(bool onoff)
{ f._cxx0xEnabled = onoff; }

bool Lexer::objCEnabled() const
{ return f._objCEnabled; }

void Lexer::setObjCEnabled(bool onoff)
{ f._objCEnabled = onoff; }

bool Lexer::isIncremental() const
{ return f._isIncremental; }

void Lexer::setIncremental(bool isIncremental)
{ f._isIncremental = isIncremental; }

bool Lexer::scanCommentTokens() const
{ return f._scanCommentTokens; }

void Lexer::setScanCommentTokens(bool onoff)
{ f._scanCommentTokens = onoff; }

void Lexer::setScanAngleStringLiteralTokens(bool onoff)
{ f._scanAngleStringLiteralTokens = onoff; }

void Lexer::pushLineStartOffset()
{
    ++_currentLine;
}

unsigned Lexer::tokenOffset() const
{ return _tokenStart - _firstChar; }

unsigned Lexer::tokenLength() const
{ return _currentChar - _tokenStart; }

const char *Lexer::tokenBegin() const
{ return _tokenStart; }

const char *Lexer::tokenEnd() const
{ return _currentChar; }

unsigned Lexer::currentLine() const
{ return _currentLine; }

void Lexer::scan(Token *tok)
{
    tok->reset();
    scan_helper(tok);
    tok->f.length = _currentChar - _tokenStart;
}

void Lexer::scan_helper(Token *tok)
{
  _Lagain:
    while (_yychar && std::isspace(_yychar)) {
        if (_yychar == '\n') {
            tok->f.joined = false;
            tok->f.newline = true;
        } else {
            tok->f.whitespace = true;
        }
        yyinp();
    }

    tok->lineno = _currentLine;
    _tokenStart = _currentChar;
    tok->offset = _currentChar - _firstChar;

    if (_state == State_MultiLineComment || _state == State_MultiLineDoxyComment) {
        const int originalState = _state;

        if (! _yychar) {
            tok->f.kind = T_EOF_SYMBOL;
            return;
        }

        while (_yychar) {
            if (_yychar != '*')
                yyinp();
            else {
                yyinp();
                if (_yychar == '/') {
                    yyinp();
                    _state = State_Default;
                    break;
                }
            }
        }

        if (! f._scanCommentTokens)
            goto _Lagain;

        else if (originalState == State_MultiLineComment)
            tok->f.kind = T_COMMENT;
        else
            tok->f.kind = T_DOXY_COMMENT;
        return; // done
    }

    if (! _yychar) {
        tok->f.kind = T_EOF_SYMBOL;
        return;
    }

    unsigned char ch = _yychar;
    yyinp();

    switch (ch) {
    case '\\':
        while (_yychar != '\n' && std::isspace(_yychar))
            yyinp();
        // ### assert(! _yychar || _yychar == '\n');
        if (_yychar == '\n') {
            tok->f.joined = true;
            tok->f.newline = false;
            yyinp();
        }
        goto _Lagain;

    case '"': case '\'': {
        const char quote = ch;

        tok->f.kind = quote == '"'
            ? T_STRING_LITERAL
            : T_CHAR_LITERAL;

        while (_yychar && _yychar != quote) {
            if (_yychar == '\n')
                break;
            else if (_yychar != '\\')
                yyinp();
            else {
                yyinp(); // skip `\\'

                if (_yychar)
                    yyinp();
            }
        }
        // assert(_yychar == quote);

        if (_yychar == quote)
            yyinp();
    } break;

    case '{':
        tok->f.kind = T_LBRACE;
        break;

    case '}':
        tok->f.kind = T_RBRACE;
        break;

    case '[':
        tok->f.kind = T_LBRACKET;
        break;

    case ']':
        tok->f.kind = T_RBRACKET;
        break;

    case '#':
        if (_yychar == '#') {
            tok->f.kind = T_POUND_POUND;
            yyinp();
        } else {
            tok->f.kind = T_POUND;
        }
        break;

    case '(':
        tok->f.kind = T_LPAREN;
        break;

    case ')':
        tok->f.kind = T_RPAREN;
        break;

    case ';':
        tok->f.kind = T_SEMICOLON;
        break;

    case ':':
        if (_yychar == ':') {
            yyinp();
            tok->f.kind = T_COLON_COLON;
        } else {
            tok->f.kind = T_COLON;
        }
        break;

    case '.':
        if (_yychar == '*') {
            yyinp();
            tok->f.kind = T_DOT_STAR;
        } else if (_yychar == '.') {
            yyinp();
            // ### assert(_yychar);
            if (_yychar == '.') {
                yyinp();
                tok->f.kind = T_DOT_DOT_DOT;
            } else {
                tok->f.kind = T_ERROR;
            }
        } else if (std::isdigit(_yychar)) {
            do {
                if (_yychar == 'e' || _yychar == 'E') {
                    yyinp();
                    if (_yychar == '-' || _yychar == '+') {
                        yyinp();
                        // ### assert(std::isdigit(_yychar));
                    }
                } else if (std::isalnum(_yychar) || _yychar == '.') {
                    yyinp();
                } else {
                    break;
                }
            } while (_yychar);
            tok->f.kind = T_NUMERIC_LITERAL;
        } else {
            tok->f.kind = T_DOT;
        }
        break;

    case '?':
        tok->f.kind = T_QUESTION;
        break;

    case '+':
        if (_yychar == '+') {
            yyinp();
            tok->f.kind = T_PLUS_PLUS;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_PLUS_EQUAL;
        } else {
            tok->f.kind = T_PLUS;
        }
        break;

    case '-':
        if (_yychar == '-') {
            yyinp();
            tok->f.kind = T_MINUS_MINUS;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_MINUS_EQUAL;
        } else if (_yychar == '>') {
            yyinp();
            if (_yychar == '*') {
                yyinp();
                tok->f.kind = T_ARROW_STAR;
            } else {
                tok->f.kind = T_ARROW;
            }
        } else {
            tok->f.kind = T_MINUS;
        }
        break;

    case '*':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_STAR_EQUAL;
        } else {
            tok->f.kind = T_STAR;
        }
        break;

    case '/':
        if (_yychar == '/') {
            yyinp();

            bool doxy = false;

            if (_yychar == '/' || _yychar == '!') {
                yyinp();

                if (_yychar == '<')
                    yyinp();

                if (_yychar != '\n' && std::isspace(_yychar))
                    doxy = true;
            }

            while (_yychar && _yychar != '\n')
                yyinp();

            if (! f._scanCommentTokens)
                goto _Lagain;

            tok->f.kind = doxy ? T_CPP_DOXY_COMMENT : T_CPP_COMMENT;

        } else if (_yychar == '*') {
            yyinp();

            bool doxy = false;

            if (_yychar == '*' || _yychar == '!') {
                const char ch = _yychar;

                yyinp();

                if (ch == '*' && _yychar == '/')
                    goto _Ldone;

                if (_yychar == '<')
                    yyinp();

                if (! _yychar || std::isspace(_yychar))
                    doxy = true;
            }

            while (_yychar) {
                if (_yychar != '*') {
                    yyinp();
                } else {
                    yyinp();
                    if (_yychar == '/')
                        break;
                }
            }

        _Ldone:
            if (_yychar)
                yyinp();
            else
                _state = doxy ? State_MultiLineDoxyComment : State_MultiLineComment;

            if (! f._scanCommentTokens)
                goto _Lagain;

            tok->f.kind = doxy ? T_DOXY_COMMENT : T_COMMENT;

        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_SLASH_EQUAL;
        } else {
            tok->f.kind = T_SLASH;
        }
        break;

    case '%':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_PERCENT_EQUAL;
        } else {
            tok->f.kind = T_PERCENT;
        }
        break;

    case '^':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_CARET_EQUAL;
        } else {
            tok->f.kind = T_CARET;
        }
        break;

    case '&':
        if (_yychar == '&') {
            yyinp();
            tok->f.kind = T_AMPER_AMPER;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_AMPER_EQUAL;
        } else {
            tok->f.kind = T_AMPER;
        }
        break;

    case '|':
        if (_yychar == '|') {
            yyinp();
            tok->f.kind = T_PIPE_PIPE;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_PIPE_EQUAL;
        } else {
            tok->f.kind = T_PIPE;
        }
        break;

    case '~':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_TILDE_EQUAL;
        } else {
            tok->f.kind = T_TILDE;
        }
        break;

    case '!':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_EXCLAIM_EQUAL;
        } else {
            tok->f.kind = T_EXCLAIM;
        }
        break;

    case '=':
        if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_EQUAL_EQUAL;
        } else {
            tok->f.kind = T_EQUAL;
        }
        break;

    case '<':
        if (f._scanAngleStringLiteralTokens) {
            //const char *yytext = _currentChar;
            while (_yychar && _yychar != '>')
                yyinp();
            //int yylen = _currentChar - yytext;
            // ### assert(_yychar == '>');
            if (_yychar == '>')
                yyinp();
            tok->f.kind = T_ANGLE_STRING_LITERAL;
        } else if (_yychar == '<') {
            yyinp();
            if (_yychar == '=') {
                yyinp();
                tok->f.kind = T_LESS_LESS_EQUAL;
            } else
                tok->f.kind = T_LESS_LESS;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_LESS_EQUAL;
        } else {
            tok->f.kind = T_LESS;
        }
        break;

    case '>':
        if (_yychar == '>') {
            yyinp();
            if (_yychar == '=') {
                yyinp();
                tok->f.kind = T_GREATER_GREATER_EQUAL;
            } else
                tok->f.kind = T_LESS_LESS;
            tok->f.kind = T_GREATER_GREATER;
        } else if (_yychar == '=') {
            yyinp();
            tok->f.kind = T_GREATER_EQUAL;
        } else {
            tok->f.kind = T_GREATER;
        }
        break;

    case ',':
        tok->f.kind = T_COMMA;
        break;

    default: {
        if (f._objCEnabled) {
            if (ch == '@' && _yychar >= 'a' && _yychar <= 'z') {
                //const char *yytext = _currentChar;

                do {
                    yyinp();
                    if (! (isalnum(_yychar) || _yychar == '_' || _yychar == '$'))
                        break;
                } while (_yychar);

                // const int yylen = _currentChar - yytext;
                //tok->f.kind = classifyObjCAtKeyword(yytext, yylen);		 /// ### FIXME
                break;
            } else if (ch == '@' && _yychar == '"') {
                // objc @string literals
                ch = _yychar;
                yyinp();
                tok->f.kind = T_AT_STRING_LITERAL;

                //const char *yytext = _currentChar;

                while (_yychar && _yychar != '"') {
                    if (_yychar != '\\')
                        yyinp();
                    else {
                        yyinp(); // skip `\\'

                        if (_yychar)
                            yyinp();
                    }
                }
                // assert(_yychar == '"');

                //int yylen = _currentChar - yytext;

                if (_yychar == '"')
                    yyinp();

                break;
            }
        }

        if (ch == 'L' && (_yychar == '"' || _yychar == '\'')) {
            // wide char/string literals
            ch = _yychar;
            yyinp();

            const char quote = ch;

            tok->f.kind = quote == '"'
                ? T_WIDE_STRING_LITERAL
                : T_WIDE_CHAR_LITERAL;

            //const char *yytext = _currentChar;

            while (_yychar && _yychar != quote) {
                if (_yychar != '\\')
                    yyinp();
                else {
                    yyinp(); // skip `\\'

                    if (_yychar)
                        yyinp();
                }
            }
            // assert(_yychar == quote);

            //int yylen = _currentChar - yytext;

            if (_yychar == quote)
                yyinp();

        } else if (std::isalpha(ch) || ch == '_' || ch == '$') {
            //const char *yytext = _currentChar - 1;
            while (std::isalnum(_yychar) || _yychar == '_' || _yychar == '$')
                yyinp();
            //int yylen = _currentChar - yytext;
            tok->f.kind = T_IDENTIFIER;
            break;
        } else if (std::isdigit(ch)) {
            //const char *yytext = _currentChar - 1;
            while (_yychar) {
                if (_yychar == 'e' || _yychar == 'E') {
                    yyinp();
                    if (_yychar == '-' || _yychar == '+') {
                        yyinp();
                        // ### assert(std::isdigit(_yychar));
                    }
                } else if (std::isalnum(_yychar) || _yychar == '.') {
                    yyinp();
                } else {
                    break;
                }
            }
            //int yylen = _currentChar - yytext;
            tok->f.kind = T_NUMERIC_LITERAL;
            break;
        } else {
            tok->f.kind = T_ERROR;
            break;
        }
    } // default

    } // switch
}

}
