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
#include <deque>
#include "keynode/keynode.h"
#include "elasticnodes/graphwidget.h"

using namespace std;
typedef std::pair<uint32_t,uint32_t> Range;
typedef std::pair<bool,Range> IsPrivateNPathRange; // < isPrivate, <min,max> >
typedef std::deque<IsPrivateNPathRange> TreeChains;

namespace TreeTraversal {
    enum Type {
        preorder,
        postorder,
        levelorder
    };
}

static const TreeTraversal::Type defaultTreeTraversalType = TreeTraversal::preorder;
static const uint32_t NODE_IDX_M = -1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void outputExtKeysFromSeed(const std::string& seed, const std::string& chainStr,
                               StringUtils::StringFormat seedStringFormat,
                               TreeTraversal::Type traversalType = defaultTreeTraversalType,
                               bool isVerbose = false);
    void outputExtKeysFromExtKey(const std::string& extKey, const std::string& chainStr,
                                 TreeTraversal::Type traversalType = defaultTreeTraversalType,
                                 bool isVerbose = false);
    void outputKeyAddressofExtKey(const std::string& extKey, bool isVerbose = false);

    void traversePreorder(const KeyNode& keyNode, TreeChains treeChains,
                          const std::string& chainName, bool isVerbose, Node* currentLeft = NULL);
    void traversePostorder(const KeyNode& keyNode, TreeChains treeChains,
                           const std::string& chainName, bool isVerbose, Node* currentLeft = NULL);
    void traverseLevelorder(const KeyNode& keyNode, const TreeChains& treeChains,
                            const std::string& chainName, uint64_t level,
                            std::deque<KeyNode>& keyNodeDeq,
                            std::deque<std::pair<uint64_t,std::string>>& levelNChainDeq,
                            std::deque<Node*>& graphNodeDeq,
                            Node* leaf = NULL, bool isVerbose = false);

    Node* visit(const KeyNode& keyNode, const std::string& chainName, bool isVerbose, Node* currentLeft = NULL);
    std::string getNodeDataString(const KeyNode& keyNode, const std::string& chainName, bool isVerbose);
    void outputExtraKeyNodeData(const KeyNode& keyNode);
    TreeChains parseChainString(const std::string& chainStr, bool isPrivate);
    IsPrivateNPathRange parseRange(const std::string node, bool isPrivate);

    std::string iToString(uint32_t i);
    uchar_vector extKeyBase58OrHexToBytes(const std::string& extKey);
    uchar_vector fromBase58ExtKey(const std::string& extKey);
    inline uint32_t toPrime(uint32_t i) { return 0x80000000 | i; }
    inline uint32_t removePrime(uint32_t i) { return 0x7fffffff & i; }
    inline bool isPrime(uint32_t i) { return 0x80000000 & i; }
    void outputString(const std::string& str);

    void testVector1();
    void testVector2();

    std::string stdStringFromQString(QString str);
    QString qStringFromSTDString(std::string str);

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

    GraphWidget* treeWidget;

    void clearTree();
    void unHighlightAllTextEditsBackground();
    bool isValidExtKey(const std::string extKey);

    std::string fullTreeDescription;
};

#endif // MAINWINDOW_H
