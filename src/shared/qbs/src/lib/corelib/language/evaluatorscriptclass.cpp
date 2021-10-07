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

#include "evaluatorscriptclass.h"

#include "evaluationdata.h"
#include "evaluator.h"
#include "filecontext.h"
#include "item.h"
#include "scriptengine.h"
#include "propertydeclaration.h"
#include "value.h"
#include <logging/translator.h>
#include <tools/fileinfo.h>
#include <tools/qbsassert.h>
#include <tools/scripttools.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>
#include <QtCore/qsettings.h>

#include <QtScript/qscriptclasspropertyiterator.h>
#include <QtScript/qscriptstring.h>
#include <QtScript/qscriptvalue.h>

namespace qbs {
namespace Internal {

class SVConverter : ValueHandler
{
    EvaluatorScriptClass * const scriptClass;
    ScriptEngine * const engine;
    QScriptContext * const scriptContext;
    const QScriptValue * const object;
    const ValuePtr &valuePtr;
    const Item * const itemOfProperty;
    const QScriptString * const propertyName;
    const EvaluationData * const data;
    QScriptValue * const result;
    char pushedScopesCount;

public:

    SVConverter(EvaluatorScriptClass *esc, const QScriptValue *obj, const ValuePtr &v,
                const Item *_itemOfProperty, const QScriptString *propertyName, const EvaluationData *data,
                QScriptValue *result)
        : scriptClass(esc)
        , engine(static_cast<ScriptEngine *>(esc->engine()))
        , scriptContext(esc->engine()->currentContext())
        , object(obj)
        , valuePtr(v)
        , itemOfProperty(_itemOfProperty)
        , propertyName(propertyName)
        , data(data)
        , result(result)
        , pushedScopesCount(0)
    {
    }

    void start()
    {
        valuePtr->apply(this);
    }

private:
    void setupConvenienceProperty(const QString &conveniencePropertyName, QScriptValue *extraScope,
                                  const QScriptValue &scriptValue)
    {
        if (!extraScope->isObject())
            *extraScope = engine->newObject();
        const PropertyDeclaration::Type type
                = itemOfProperty->propertyDeclaration(propertyName->toString()).type();
        const bool isArray = type == PropertyDeclaration::StringList
                || type == PropertyDeclaration::PathList
                || type == PropertyDeclaration::Variant;
        QScriptValue valueToSet = scriptValue;
        if (isArray) {
            if (!valueToSet.isValid() || valueToSet.isUndefined())
                valueToSet = engine->newArray();
        } else if (!valueToSet.isValid()) {
            valueToSet = engine->undefinedValue();
        }
        extraScope->setProperty(conveniencePropertyName, valueToSet);
    }

    void pushScope(const QScriptValue &scope)
    {
        if (scope.isObject()) {
            scriptContext->pushScope(scope);
            ++pushedScopesCount;
        }
    }

    void pushItemScopes(const Item *item)
    {
        const Item *scope = item->scope();
        if (scope) {
            pushItemScopes(scope);
            pushScope(data->evaluator->scriptValue(scope));
        }
    }

    void popScopes()
    {
        for (; pushedScopesCount; --pushedScopesCount)
            scriptContext->popScope();
    }

    void handle(JSSourceValue *value)
    {
        const Item *conditionScopeItem = 0;
        QScriptValue conditionScope;
        QScriptValue conditionFileScope;
        Item *outerItem = data->item->outerItem();
        for (int i = 0; i < value->alternatives().count(); ++i) {
            const JSSourceValue::Alternative *alternative = 0;
            alternative = &value->alternatives().at(i);
            const Evaluator::FileContextScopes fileCtxScopes
                    = data->evaluator->fileContextScopes(value->file());
            if (!conditionScopeItem) {
                // We have to differentiate between module instances and normal items here.
                //
                // The module instance case:
                // Product {
                //     property bool something: true
                //     Properties {
                //         condition: something
                //         cpp.defines: ["ABC"]
                //     }
                // }
                //
                // data->item points to cpp and the condition's scope chain must contain cpp's
                // scope, which is the item where cpp is instantiated. The scope chain must not
                // contain data->item itself.
                //
                // The normal item case:
                // Product {
                //     property bool something: true
                //     property string value: "ABC"
                //     Properties {
                //         condition: something
                //         value: "DEF"
                //     }
                // }
                //
                // data->item points to the product and the condition's scope chain must contain
                // the product item.
                conditionScopeItem = data->item->type() == ItemType::ModuleInstance
                        ? data->item->scope() : data->item;
                conditionScope = data->evaluator->scriptValue(conditionScopeItem);
                QBS_ASSERT(conditionScope.isObject(), return);
                conditionFileScope = fileCtxScopes.fileScope;
            }
            scriptContext->pushScope(conditionFileScope);
            pushItemScopes(conditionScopeItem);
            if (alternative->value->definingItem())
                pushItemScopes(alternative->value->definingItem());
            scriptContext->pushScope(conditionScope);
            const QScriptValue &theImportScope = fileCtxScopes.importScope;
            if (theImportScope.isError()) {
                scriptContext->popScope();
                scriptContext->popScope();
                popScopes();
                *result = theImportScope;
                return;
            }
            scriptContext->pushScope(theImportScope);
            const QScriptValue cr = engine->evaluate(alternative->condition);
            const QScriptValue overrides = engine->evaluate(alternative->overrideListProperties);
            scriptContext->popScope();
            scriptContext->popScope();
            scriptContext->popScope();
            popScopes();
            if (engine->hasErrorOrException(cr)) {
                *result = engine->lastErrorValue(cr);
                return;
            }
            if (cr.toBool()) {
                // condition is true, let's use the value of this alternative
                if (alternative->value->sourceUsesOuter() && !outerItem) {
                    // Clone value but without alternatives.
                    JSSourceValuePtr outerValue = JSSourceValue::create();
                    outerValue->setFile(value->file());
                    outerValue->setHasFunctionForm(value->hasFunctionForm());
                    outerValue->setSourceCode(value->sourceCode());
                    outerValue->setBaseValue(value->baseValue());
                    if (value->sourceUsesBase())
                        outerValue->setSourceUsesBaseFlag();
                    outerValue->setLocation(value->line(), value->column());
                    outerItem = Item::create(data->item->pool(), ItemType::Outer);
                    outerItem->setProperty(propertyName->toString(), outerValue);
                }
                if (overrides.toBool())
                    value->setIsExclusiveListValue();
                value = alternative->value.get();
                break;
            }
        }

        QScriptValue extraScope;
        if (value->sourceUsesBase()) {
            QScriptValue baseValue;
            if (value->baseValue()) {
                SVConverter converter(scriptClass, object, value->baseValue(), itemOfProperty,
                                      propertyName, data, &baseValue);
                converter.start();
            }
            setupConvenienceProperty(QLatin1String("base"), &extraScope, baseValue);
        }
        if (value->sourceUsesOuter() && outerItem) {
            const QScriptValue v = data->evaluator->property(outerItem, *propertyName);
            if (engine->hasErrorOrException(v)) {
                *result = engine->lastErrorValue(v);
                return;
            }
            setupConvenienceProperty(QLatin1String("outer"), &extraScope, v);
        }
        if (value->sourceUsesOriginal()) {
            QScriptValue originalValue;
            if (data->item->propertyDeclaration(propertyName->toString()).isScalar()) {
                const Item *item = itemOfProperty;
                while (item->type() == ItemType::ModuleInstance)
                    item = item->prototype();
                SVConverter converter(scriptClass, object, item->property(*propertyName), item,
                                      propertyName, data, &originalValue);
                converter.start();
            } else {
                originalValue = engine->newArray(0);
            }
            setupConvenienceProperty(QLatin1String("original"), &extraScope, originalValue);
        }

        const Evaluator::FileContextScopes fileCtxScopes
                = data->evaluator->fileContextScopes(value->file());
        pushScope(fileCtxScopes.fileScope);
        pushItemScopes(data->item);
        if (itemOfProperty->type() != ItemType::ModuleInstance) {
            // Own properties of module instances must not have the instance itself in the scope.
            pushScope(*object);
        }
        if (value->definingItem())
            pushItemScopes(value->definingItem());
        pushScope(extraScope);
        const QScriptValue &theImportScope = fileCtxScopes.importScope;
        if (theImportScope.isError()) {
            *result = theImportScope;
        } else {
            pushScope(theImportScope);
            *result = engine->evaluate(value->sourceCodeForEvaluation(), value->file()->filePath(),
                                       value->line());
        }
        popScopes();
    }

    void handle(ItemValue *value)
    {
        *result = data->evaluator->scriptValue(value->item());
        if (!result->isValid())
            qDebug() << "SVConverter returned invalid script value.";
    }

    void handle(VariantValue *variantValue)
    {
        *result = engine->toScriptValue(variantValue->value());
    }
};

bool debugProperties = false;

enum QueryPropertyType
{
    QPTDefault,
    QPTParentProperty
};

EvaluatorScriptClass::EvaluatorScriptClass(ScriptEngine *scriptEngine, const Logger &logger)
    : QScriptClass(scriptEngine)
    , m_logger(logger)
    , m_valueCacheEnabled(false)
{
    Q_UNUSED(m_logger);
}

QScriptClass::QueryFlags EvaluatorScriptClass::queryProperty(const QScriptValue &object,
                                                             const QScriptString &name,
                                                             QScriptClass::QueryFlags flags,
                                                             uint *id)
{
    Q_UNUSED(flags);

    // We assume that it's safe to save the result of the query in a member of the scriptclass.
    // It must be cleared in the property method before doing any further lookup.
    QBS_ASSERT(m_queryResult.isNull(), return QueryFlags());

    if (debugProperties)
        qDebug() << "[SC] queryProperty " << object.objectId() << " " << name;

    EvaluationData *const data = attachedPointer<EvaluationData>(object);
    const QString nameString = name.toString();
    if (nameString == QLatin1String("parent")) {
        *id = QPTParentProperty;
        m_queryResult.data = data;
        return QScriptClass::HandlesReadAccess;
    }

    *id = QPTDefault;
    if (!data) {
        if (debugProperties)
            qDebug() << "[SC] queryProperty: no data attached";
        return QScriptClass::QueryFlags();
    }

    return queryItemProperty(data, nameString);
}

QScriptClass::QueryFlags EvaluatorScriptClass::queryItemProperty(const EvaluationData *data,
                                                                 const QString &name,
                                                                 bool ignoreParent)
{
    for (const Item *item = data->item; item; item = item->prototype()) {
        m_queryResult.value = item->ownProperty(name);
        if (m_queryResult.value) {
            m_queryResult.data = data;
            m_queryResult.itemOfProperty = item;
            return HandlesReadAccess;
        }
    }

    if (!ignoreParent && data->item && data->item->parent()) {
        if (debugProperties)
            qDebug() << "[SC] queryProperty: query parent";
        EvaluationData parentdata = *data;
        parentdata.item = data->item->parent();
        const QueryFlags qf = queryItemProperty(&parentdata, name, true);
        if (qf.testFlag(HandlesReadAccess)) {
            m_queryResult.foundInParent = true;
            m_queryResult.data = data;
            return qf;
        }
    }

    if (debugProperties)
        qDebug() << "[SC] queryProperty: no such property";
    return QScriptClass::QueryFlags();
}

QString EvaluatorScriptClass::resultToString(const QScriptValue &scriptValue)
{
    return (scriptValue.isObject()
        ? QLatin1String("[Object: ")
            + QString::number(scriptValue.objectId(), 16) + QLatin1Char(']')
        : scriptValue.toVariant().toString());
}

void EvaluatorScriptClass::collectValuesFromNextChain(const EvaluationData *data, QScriptValue *result,
        const QString &propertyName, const ValuePtr &value)
{
    QScriptValueList lst;
    Set<Value *> oldNextChain = m_currentNextChain;
    for (ValuePtr next = value; next; next = next->next())
        m_currentNextChain.insert(next.get());

    for (ValuePtr next = value; next; next = next->next()) {
        QScriptValue v = data->evaluator->property(next->definingItem(), propertyName);
        ScriptEngine * const se = static_cast<ScriptEngine *>(engine());
        if (se->hasErrorOrException(v)) {
            *result = se->lastErrorValue(v);
            return;
        }
        if (v.isUndefined())
            continue;
        lst << v;
        if (next->type() == Value::JSSourceValueType
                && std::static_pointer_cast<JSSourceValue>(next)->isExclusiveListValue()) {
            lst = lst.mid(lst.length() - 2);
            break;
        }
    }
    m_currentNextChain = oldNextChain;

    *result = engine()->newArray();
    quint32 k = 0;
    for (int i = 0; i < lst.count(); ++i) {
        const QScriptValue &v = lst.at(i);
        QBS_ASSERT(!v.isError(), continue);
        if (v.isArray()) {
            const quint32 vlen = v.property(QStringLiteral("length")).toInt32();
            for (quint32 j = 0; j < vlen; ++j)
                result->setProperty(k++, v.property(j));
        } else {
            result->setProperty(k++, v);
        }
    }
}

static QString overriddenSourceDirectory(const Item *item)
{
    const VariantValuePtr v = item->variantProperty(QLatin1String("_qbs_sourceDir"));
    return v ? v->value().toString() : QString();
}

static void makeTypeError(const ErrorInfo &error, QScriptValue &v)
{
    v = v.engine()->currentContext()->throwError(QScriptContext::TypeError,
                                                 error.toString());
}

static void makeTypeError(const PropertyDeclaration &decl, const CodeLocation &location,
                          QScriptValue &v)
{
    const ErrorInfo error(Tr::tr("Value assigned to property '%1' does not have type '%2'.")
                          .arg(decl.name(), decl.typeString()), location);
    makeTypeError(error, v);
}

static void convertToPropertyType(const Item *item, const PropertyDeclaration& decl,
                                  const Value *value, QScriptValue &v)
{
    if (value->type() == Value::VariantValueType && v.isUndefined() && !decl.isScalar()) {
        v = v.engine()->newArray(); // QTBUG-51237
        return;
    }

    if (v.isUndefined() || v.isError())
        return;
    QString srcDir;
    const CodeLocation &location = value->location();
    switch (decl.type()) {
    case PropertyDeclaration::UnknownType:
    case PropertyDeclaration::Variant:
        break;
    case PropertyDeclaration::Boolean:
        if (!v.isBool())
            v = v.toBool();
        break;
    case PropertyDeclaration::Integer:
        if (!v.isNumber())
            makeTypeError(decl, location, v);
        break;
    case PropertyDeclaration::Path:
    {
        if (!v.isString()) {
            makeTypeError(decl, location, v);
            break;
        }
        const QString srcDir = overriddenSourceDirectory(item);
        if (!srcDir.isEmpty())
            v = v.engine()->toScriptValue(FileInfo::resolvePath(srcDir, v.toString()));
        break;
    }
    case PropertyDeclaration::String:
        if (!v.isString())
            makeTypeError(decl, location, v);
        break;
    case PropertyDeclaration::PathList:
        srcDir = overriddenSourceDirectory(item);
        // Fall-through.
    case PropertyDeclaration::StringList:
    {
        if (!v.isArray()) {
            QScriptValue x = v.engine()->newArray(1);
            x.setProperty(0, v);
            v = x;
        }
        const quint32 c = v.property(QLatin1String("length")).toUInt32();
        for (quint32 i = 0; i < c; ++i) {
            QScriptValue elem = v.property(i);
            if (elem.isUndefined()) {
                ErrorInfo error(Tr::tr("Element at index %1 of list property '%2' is undefined. "
                                       "String expected.").arg(i).arg(decl.name()), location);
                makeTypeError(error, v);
                break;
            }
            if (elem.isNull()) {
                ErrorInfo error(Tr::tr("Element at index %1 of list property '%2' is null. "
                                       "String expected.").arg(i).arg(decl.name()), location);
                makeTypeError(error, v);
                break;
            }
            if (!elem.isString()) {
                ErrorInfo error(Tr::tr("Element at index %1 of list property '%2' does not have "
                                       "string type.").arg(i).arg(decl.name()), location);
                makeTypeError(error, v);
                break;
            }
            if (srcDir.isEmpty())
                continue;
            elem = v.engine()->toScriptValue(FileInfo::resolvePath(srcDir, elem.toString()));
            v.setProperty(i, elem);
        }
        break;
    }
    }
}

class PropertyStackManager
{
public:
    PropertyStackManager(const Item *itemOfProperty, const QScriptString &name, const Value *value,
                         std::stack<QualifiedId> &requestedProperties,
                         PropertyDependencies &propertyDependencies)
        : m_requestedProperties(requestedProperties)
    {
        if (value->type() == Value::JSSourceValueType
                && (itemOfProperty->type() == ItemType::ModuleInstance
                    || itemOfProperty->type() == ItemType::Module
                    || itemOfProperty->type() == ItemType::Export)) {
            const VariantValueConstPtr varValue
                    = itemOfProperty->variantProperty(QLatin1String("name"));
            QBS_ASSERT(varValue, return);
            m_stackUpdate = true;
            const QualifiedId fullPropName
                    = QualifiedId::fromString(varValue->value().toString()) << name.toString();
            if (!requestedProperties.empty())
                propertyDependencies[fullPropName].insert(requestedProperties.top());
            m_requestedProperties.push(fullPropName);
        }
    }

    ~PropertyStackManager()
    {
        if (m_stackUpdate)
            m_requestedProperties.pop();
    }

private:
    std::stack<QualifiedId> &m_requestedProperties;
    bool m_stackUpdate = false;
};

QScriptValue EvaluatorScriptClass::property(const QScriptValue &object, const QScriptString &name,
                                            uint id)
{
    const bool foundInParent = m_queryResult.foundInParent;
    const EvaluationData *data = m_queryResult.data;
    const Item * const itemOfProperty = m_queryResult.itemOfProperty;
    m_queryResult.foundInParent = false;
    m_queryResult.data = 0;
    m_queryResult.itemOfProperty = 0;
    QBS_ASSERT(data, return QScriptValue());

    const QueryPropertyType qpt = static_cast<QueryPropertyType>(id);
    if (qpt == QPTParentProperty) {
        return data->item->parent()
                ? data->evaluator->scriptValue(data->item->parent())
                : engine()->undefinedValue();
    }

    ValuePtr value;
    m_queryResult.value.swap(value);
    QBS_ASSERT(value, return QScriptValue());
    QBS_ASSERT(m_queryResult.isNull(), return QScriptValue());

    if (debugProperties)
        qDebug() << "[SC] property " << name;

    PropertyStackManager propStackmanager(itemOfProperty, name, value.get(),
                                          m_requestedProperties, m_propertyDependencies);

    QScriptValue result;
    if (m_valueCacheEnabled) {
        result = data->valueCache.value(name);
        if (result.isValid()) {
            if (debugProperties)
                qDebug() << "[SC] cache hit " << name << ": " << resultToString(result);
            return result;
        }
    }

    if (value->next() && !m_currentNextChain.contains(value.get())) {
        collectValuesFromNextChain(data, &result, name.toString(), value);
    } else {
        QScriptValue parentObject;
        if (foundInParent)
            parentObject = data->evaluator->scriptValue(data->item->parent());
        SVConverter converter(this, foundInParent ? &parentObject : &object, value, itemOfProperty,
                              &name, data, &result);
        converter.start();

        const PropertyDeclaration decl = data->item->propertyDeclaration(name.toString());
        convertToPropertyType(data->item, decl, value.get(), result);
    }

    if (debugProperties)
        qDebug() << "[SC] cache miss " << name << ": " << resultToString(result);
    if (m_valueCacheEnabled)
        data->valueCache.insert(name, result);
    return result;
}

class EvaluatorScriptClassPropertyIterator : public QScriptClassPropertyIterator
{
public:
    EvaluatorScriptClassPropertyIterator(const QScriptValue &object, EvaluationData *data)
        : QScriptClassPropertyIterator(object), m_it(data->item->properties())
    {
    }

    bool hasNext() const override
    {
        return m_it.hasNext();
    }

    void next() override
    {
        m_it.next();
    }

    bool hasPrevious() const override
    {
        return m_it.hasPrevious();
    }

    void previous() override
    {
        m_it.previous();
    }

    void toFront() override
    {
        m_it.toFront();
    }

    void toBack() override
    {
        m_it.toBack();
    }

    QScriptString name() const override
    {
        return object().engine()->toStringHandle(m_it.key());
    }

private:
    QMapIterator<QString, ValuePtr> m_it;
};

QScriptClassPropertyIterator *EvaluatorScriptClass::newIterator(const QScriptValue &object)
{
    EvaluationData *const data = attachedPointer<EvaluationData>(object);
    return data ? new EvaluatorScriptClassPropertyIterator(object, data) : nullptr;
}

void EvaluatorScriptClass::setValueCacheEnabled(bool enabled)
{
    m_valueCacheEnabled = enabled;
}

} // namespace Internal
} // namespace qbs
