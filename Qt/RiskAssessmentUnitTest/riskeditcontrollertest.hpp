#ifndef RISKEDITCONTROLLERTEST_HPP
#define RISKEDITCONTROLLERTEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.hpp"


class RiskAssessmentControllerTest: public ::testing::Test {
public:
    QApplication a;
    RiskEditController controller;

    RiskAssessmentControllerTest(int argc = 0, char** argv = nullptr): a(argc, argv) {
        a.setWindowIcon(QIcon(":/icons/favicon"));

        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "RiskAssesment_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
    }
};


#endif // RISKEDITCONTROLLERTEST_HPP
