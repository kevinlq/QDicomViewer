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

#ifndef QBS_VALUE_H
#define QBS_VALUE_H

#include "forward_decls.h"
#include <tools/codelocation.h>
#include <QtCore/qvariant.h>

namespace qbs {
namespace Internal {
class Item;
class ValueHandler;

class Value
{
public:
    enum Type
    {
        JSSourceValueType,
        ItemValueType,
        VariantValueType
    };

    Value(Type t, bool createdByPropertiesBlock);
    Value(const Value &other);
    virtual ~Value();

    Type type() const { return m_type; }
    virtual void apply(ValueHandler *) = 0;
    virtual ValuePtr clone() const = 0;
    virtual CodeLocation location() const { return CodeLocation(); }

    Item *definingItem() const;
    virtual void setDefiningItem(Item *item);

    ValuePtr next() const;
    void setNext(const ValuePtr &next);

    bool createdByPropertiesBlock() const { return m_createdByPropertiesBlock; }

private:
    Type m_type;
    Item *m_definingItem;
    ValuePtr m_next;
    const bool m_createdByPropertiesBlock;
};

class ValueHandler
{
public:
    virtual void handle(JSSourceValue *value) = 0;
    virtual void handle(ItemValue *value) = 0;
    virtual void handle(VariantValue *value) = 0;
};

class JSSourceValue : public Value
{
    friend class ItemReaderASTVisitor;
    JSSourceValue(bool createdByPropertiesBlock);
    JSSourceValue(const JSSourceValue &other);

    enum Flag
    {
        NoFlags = 0x00,
        SourceUsesBase = 0x01,
        SourceUsesOuter = 0x02,
        SourceUsesOriginal = 0x04,
        HasFunctionForm = 0x08,
        ExclusiveListValue = 0x10,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

public:
    static JSSourceValuePtr QBS_AUTOTEST_EXPORT create(bool createdByPropertiesBlock = false);
    ~JSSourceValue();

    void apply(ValueHandler *handler) { handler->handle(this); }
    ValuePtr clone() const;

    void setSourceCode(const QStringRef &sourceCode) { m_sourceCode = sourceCode; }
    const QStringRef &sourceCode() const { return m_sourceCode; }
    QString sourceCodeForEvaluation() const;

    void setLocation(int line, int column);
    int line() const { return m_line; }
    int column() const { return m_column; }
    CodeLocation location() const;

    void setFile(const FileContextPtr &file) { m_file = file; }
    const FileContextPtr &file() const { return m_file; }

    void setSourceUsesBaseFlag() { m_flags |= SourceUsesBase; }
    bool sourceUsesBase() const { return m_flags.testFlag(SourceUsesBase); }
    bool sourceUsesOuter() const { return m_flags.testFlag(SourceUsesOuter); }
    bool sourceUsesOriginal() const { return m_flags.testFlag(SourceUsesOriginal); }
    bool hasFunctionForm() const { return m_flags.testFlag(HasFunctionForm); }
    void setHasFunctionForm(bool b);
    void setIsExclusiveListValue() { m_flags |= ExclusiveListValue; }
    bool isExclusiveListValue() { return m_flags.testFlag(ExclusiveListValue); }

    const JSSourceValuePtr &baseValue() const { return m_baseValue; }
    void setBaseValue(const JSSourceValuePtr &v) { m_baseValue = v; }

    struct Alternative
    {
        Alternative() { }
        Alternative(const QString &c, const QString &o, const JSSourceValuePtr &v)
            : condition(c), overrideListProperties(o), value(v) {}
        Alternative clone() const
        {
            return Alternative(condition, overrideListProperties,
                               std::static_pointer_cast<JSSourceValue>(value->clone()));
        }

        QString condition;
        QString overrideListProperties;
        JSSourceValuePtr value;
    };

    const QList<Alternative> &alternatives() const { return m_alternatives; }
    void setAlternatives(const QList<Alternative> &alternatives) { m_alternatives = alternatives; }
    void addAlternative(const Alternative &alternative) { m_alternatives.append(alternative); }

    void setDefiningItem(Item *item);

private:
    QStringRef m_sourceCode;
    int m_line;
    int m_column;
    FileContextPtr m_file;
    Flags m_flags;
    JSSourceValuePtr m_baseValue;
    QList<Alternative> m_alternatives;
};


class ItemValue : public Value
{
    ItemValue(Item *item, bool createdByPropertiesBlock);
public:
    static ItemValuePtr create(Item *item, bool createdByPropertiesBlock = false);

    Item *item() const { return m_item; }
    void setItem(Item *item) { m_item = item; }

private:
    void apply(ValueHandler *handler) override { handler->handle(this); }
    ValuePtr clone() const override;

    Item *m_item;
};


class VariantValue : public Value
{
    VariantValue(const QVariant &v);
public:
    static VariantValuePtr create(const QVariant &v = QVariant());

    void apply(ValueHandler *handler) { handler->handle(this); }
    ValuePtr clone() const;

    void setValue(const QVariant &v) { m_value = v; }
    const QVariant &value() const { return m_value; }

private:
    QVariant m_value;
};

} // namespace Internal
} // namespace qbs

#endif // QBS_VALUE_H
