#ifndef KEYTREEEDITDIALOG_H
#define KEYTREEEDITDIALOG_H

#include <QDialog>
#include <QString>
#include <QTreeWidget>
#include "keynode/keynode.h"
#include "keynode/stringutils.h"
#include "elasticnodes/graphwidget.h"
#include "keytreeutil.h"

namespace Ui {
class KeyTreeEditDialog;
}

class KeyTreeEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyTreeEditDialog(const KeyNode& keyNode, TreeChains& treeChains, QWidget *parent = 0);

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

    Node* visit(const KeyNode& keyNode, const std::string& chainName, bool isVerbose,
                QColor nodePrimaryColor, QColor nodeSecondaryColor, Node* currentLeft = NULL);
    std::string getNodeDataString(const KeyNode& keyNode, const std::string& chainName, bool isVerbose);
    void outputExtraKeyNodeData(const KeyNode& keyNode);
    void outputString(const std::string& str);

    ~KeyTreeEditDialog();
public slots:
    void exportClicked();
    void treeWidgetItemSelectionChanged();

private:
    Ui::KeyTreeEditDialog *ui;
    GraphWidget* treeGraphWidget;

    void clearTree();

    void addKeyTreeLevel(QString level);
    void addKeyTreeChild(QTreeWidgetItem *parent, QString chain);
    void buildTreeWidget(uint32_t level, const KeyNode& keyNode, const std::string& chainName);
};

#endif // KEYTREEEDITDIALOG_H
