/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *user_label;
    QLineEdit *user_Edit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *mail_label;
    QLineEdit *mail_Edit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *password_label;
    QLineEdit *password_Edit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *comfirmPassword_label;
    QLineEdit *comfirmPassword_Edit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *verify_label;
    QLineEdit *verify_Edit;
    QPushButton *getVerify_Button;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *sure_Button;
    QPushButton *cancel_Button;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 400);
        verticalLayout_2 = new QVBoxLayout(RegisterDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        user_label = new QLabel(RegisterDialog);
        user_label->setObjectName("user_label");
        user_label->setMinimumSize(QSize(0, 25));
        user_label->setMaximumSize(QSize(16777215, 25));
        user_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(user_label);

        user_Edit = new QLineEdit(RegisterDialog);
        user_Edit->setObjectName("user_Edit");
        user_Edit->setMinimumSize(QSize(0, 25));
        user_Edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(user_Edit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        mail_label = new QLabel(RegisterDialog);
        mail_label->setObjectName("mail_label");
        mail_label->setMinimumSize(QSize(0, 25));
        mail_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(mail_label);

        mail_Edit = new QLineEdit(RegisterDialog);
        mail_Edit->setObjectName("mail_Edit");
        mail_Edit->setMinimumSize(QSize(0, 25));
        mail_Edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(mail_Edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        password_label = new QLabel(RegisterDialog);
        password_label->setObjectName("password_label");
        password_label->setMinimumSize(QSize(0, 25));
        password_label->setMaximumSize(QSize(16777215, 25));
        password_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_3->addWidget(password_label);

        password_Edit = new QLineEdit(RegisterDialog);
        password_Edit->setObjectName("password_Edit");
        password_Edit->setMinimumSize(QSize(0, 25));
        password_Edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_3->addWidget(password_Edit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        comfirmPassword_label = new QLabel(RegisterDialog);
        comfirmPassword_label->setObjectName("comfirmPassword_label");
        comfirmPassword_label->setMinimumSize(QSize(0, 25));
        comfirmPassword_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(comfirmPassword_label);

        comfirmPassword_Edit = new QLineEdit(RegisterDialog);
        comfirmPassword_Edit->setObjectName("comfirmPassword_Edit");
        comfirmPassword_Edit->setMinimumSize(QSize(0, 25));
        comfirmPassword_Edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(comfirmPassword_Edit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verify_label = new QLabel(RegisterDialog);
        verify_label->setObjectName("verify_label");
        verify_label->setMinimumSize(QSize(0, 25));
        verify_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(verify_label);

        verify_Edit = new QLineEdit(RegisterDialog);
        verify_Edit->setObjectName("verify_Edit");
        verify_Edit->setMinimumSize(QSize(0, 25));
        verify_Edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(verify_Edit);

        getVerify_Button = new QPushButton(RegisterDialog);
        getVerify_Button->setObjectName("getVerify_Button");
        getVerify_Button->setMinimumSize(QSize(0, 25));
        getVerify_Button->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(getVerify_Button);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        sure_Button = new QPushButton(RegisterDialog);
        sure_Button->setObjectName("sure_Button");
        sure_Button->setMinimumSize(QSize(100, 30));

        horizontalLayout_7->addWidget(sure_Button);

        cancel_Button = new QPushButton(RegisterDialog);
        cancel_Button->setObjectName("cancel_Button");
        cancel_Button->setMinimumSize(QSize(100, 30));

        horizontalLayout_7->addWidget(cancel_Button);


        verticalLayout->addLayout(horizontalLayout_7);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        user_label->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        mail_label->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        password_label->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        comfirmPassword_label->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        verify_label->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        getVerify_Button->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        sure_Button->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\345\256\232", nullptr));
        cancel_Button->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
