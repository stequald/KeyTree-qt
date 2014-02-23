#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <string>
#include <stdint.h>
#include <map>
#include "keynode/keynode.h"
#include "elasticnodes/graphwidget.h"
#include "keytreeutil.h"

using namespace std;

static const TreeTraversal::Type defaultTreeTraversalType = TreeTraversal::preorder;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayStartUpKeyTree();
    void testVector1();
    void testVector2();

    void outputExtKeysFromSeed(const std::string& seed, const std::string& chainStr,
                               StringUtils::StringFormat seedStringFormat,
                               TreeTraversal::Type traversalType = defaultTreeTraversalType,
                               bool isVerbose = false);
    void outputExtKeysFromExtKey(const std::string& extKey, const std::string& chainStr,
                                 TreeTraversal::Type traversalType = defaultTreeTraversalType,
                                 bool isVerbose = false);
    void outputKeyAddressofExtKey(const std::string& extKey, bool isVerbose = false);
    void outputString(const std::string& str);

public slots:
    void highlightBackgroundRed(QLineEdit* lineEdit);
    void unHighlightBackground(QLineEdit* lineEdit);

    void seedRadioButtonClicked();
    void extKeyRadioButtonClicked();
    void defaultChainsComboBoxActivated(int idx);

    void goClicked();

    void seedEditingFinished();
    void seedHexEditingFinished();
    void extkeyEditingFinished();

private:
    Ui::MainWindow *ui;

    void unHighlightAllTextEditsBackground();
    bool isValidExtKey(const std::string extKey);

};

#endif // MAINWINDOW_H
