#include "BaseDialog.h"

#include "BaseWidget/Qtitlebar.h"

#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QApplication>

namespace Utils {


BaseDialog::BaseDialog(QWidget *parent)
    : BaseWidget(true, false, parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
}

BaseDialog::~BaseDialog()
{
}

}
