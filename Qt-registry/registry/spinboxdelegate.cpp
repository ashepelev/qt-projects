#include "spinboxdelegate.h"
#include <QModelIndex>
#include <QSpinBox>
#include <QLineEdit>

/* Делегат для интуитивно понятного изменения int и string значений */
SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QItemDelegate(parent)
{

}

/* Проверка на возможность парсинга */
bool SpinBoxDelegate::isInt(const QModelIndex &index) const
{
    return index.model()->data(index, Qt::EditRole).convert(QMetaType::Int);
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &,
     const QModelIndex & index ) const
 {
    // Всегда проверяем на int или string
    int value = 0;
     if (isInt(index))
         value = index.model()->data(index, Qt::EditRole).toInt();
     else
         return new QLineEdit(parent); // Создаем элементы задания значений
     QSpinBox *editor = new QSpinBox(parent);
     editor->setMinimum(value-50); // Разброс для примера
     editor->setMaximum(value+50);

     return editor;
 }

void SpinBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
    int value = 0;
     if (isInt(index))
     {
         value = index.model()->data(index, Qt::EditRole).toInt();
         QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
         spinBox->setValue(value);
     }
     else
     {
         QString val = index.model()->data(index,Qt::EditRole).toString();
         QLineEdit * edit = static_cast<QLineEdit*>(editor);
         edit->setText(val);
     }


 }

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
    if (isInt(index))
    {
         QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
         spinBox->interpretText();
         int value = spinBox->value();
         model->setData(index, value, Qt::EditRole);
    }
    else
    {
         QLineEdit * edit = static_cast<QLineEdit*>(editor);
        model->setData(index,edit->text(),Qt::EditRole);
    }
 }

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
