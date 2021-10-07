#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include "BaseWidget.h"

namespace Utils {
class QRADIANT_UTILS_EXPORT BaseDialog : public BaseWidget
{
    Q_OBJECT
public:
    BaseDialog(QWidget* parent = nullptr);
    virtual ~BaseDialog();
};

}

#endif // BASEDIALOG_H
