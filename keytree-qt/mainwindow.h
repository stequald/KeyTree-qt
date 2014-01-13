#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <string>
#include <stdint.h>
#include "keytree/keytree.h"
#include <map>
#include <QLineEdit>
#include "elasticnodes/graphwidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::string stdStringFromQString(QString str);
    QString qStringFromSTDString(std::string str);
    void outputString(const std::string str);


    void outputExamples();
    int handle_arguments(std::map<std::string, std::string> argsDict);

    void outputExtKeys(KeyTree& keyTree);
    void outputExtKeysFromSeed(const std::string seed, const std::string chainStr, StringUtils::StringFormat seedStringFormat);
    void outputExtKeysFromExtKey(const std::string extKey, const std::string chainStr);
    void outputKeyAddressesFromExtKey(const std::string extKey, uint32_t i_min = 0, uint32_t i_max = 9);
    void outputKeyAddressofExtKey(const std::string extKey);
    void testVector1();
    void testVector2();
    void testOutputExtKeysFromSeed();
    void testOutputExtKeysFromExtKey();
    void testOutputKeyAddressesFromExtKey();
    void testOutputKeyAddressofExtKey();


public slots:
    void highlightBackgroundRed(QLineEdit* lineEdit);
    void unHighlightBackground(QLineEdit* lineEdit);

    void seedRadioButtonClicked();
    void extKeyRadioButtonClicked();
    void defaultChainsComboBoxActivated(int idx);

    void extkeyClicked();
    void bitcoinkeyClicked();

    void seedEditingFinished();
    void seedHexEditingFinished();
    void extkeyEditingFinished();

private:
    Ui::MainWindow *ui;

    GraphWidget* treeWidget;

    void clearTree();
    void unHighlightAllTextEditsBackground();
    bool isValidExtKey(const std::string extKey);

    std::string fullTreeDescription;
};

#endif // MAINWINDOW_H
